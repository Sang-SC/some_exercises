/**
 * @file 3.pendulum.cpp
 * @author your name (you@domain.com)
 * @brief 从Lua文件读取倒立摆模型，利用boost进行仿真，并进行系统能量分析。主要参考了RBDL官方例程pendulum。
 * @version 0.1
 * @date 2024-02-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "utils/csvtools.h"  // 需要使用 printMatrixToFile 函数，用于保存仿真结果

#include <rbdl/rbdl.h>                        // RDBL库，主头文件
#include <rbdl/rbdl_utils.h>                  // RDBL库，工具函数头文件，这里用到了 CalcPotentialEnergy 和 CalcKineticEnergy
#include <rbdl/addons/luamodel/luamodel.h>    // RDBL库，加载Lua模型插件头文件
#include <spdlog/spdlog.h>                    // spdlog日志库，主头文件
#include <spdlog/sinks/stdout_color_sinks.h>  // spdlog日志库，多线程彩色控制台输出头文件
#include <boost/numeric/odeint/integrate/integrate_adaptive.hpp>
#include <boost/numeric/odeint/stepper/controlled_runge_kutta.hpp>
#include <boost/numeric/odeint/stepper/generation/make_controlled.hpp>
#include <boost/numeric/odeint/stepper/runge_kutta_cash_karp54.hpp>
#include <string>

typedef std::vector<double> state_type;
typedef boost::numeric::odeint::runge_kutta_cash_karp54<state_type> error_stepper_type;
typedef boost::numeric::odeint::controlled_runge_kutta<error_stepper_type> controlled_stepper_type;

/**
 * @brief 类 rbdlToBoost 用于将 RBDL 的模型转换为 Boost 的模型。
 *
 */
class rbdlToBoost {
   public:
    rbdlToBoost(RigidBodyDynamics::Model* model) : model(model) {
        q = RigidBodyDynamics::Math::VectorNd::Zero(model->dof_count);
        qd = RigidBodyDynamics::Math::VectorNd::Zero(model->dof_count);
        qdd = RigidBodyDynamics::Math::VectorNd::Zero(model->dof_count);
        tau = RigidBodyDynamics::Math::VectorNd::Zero(model->dof_count);
    }
    void operator()(const state_type& x, state_type& dxdt, const double t) {
        // 利用状态变量 x 得到广义坐标 q 和广义速度 qd
        int j = 0;
        for (int i = 0; i < model->dof_count; i++) {
            q[i] = (double)x[j];
            j++;
        }
        for (int i = 0; i < model->dof_count; i++) {
            qd[i] = (double)x[j];
            j++;
        }
        // 设置广义力为 0
        for (int i = 0; i < model->dof_count; i++) {
            tau[i] = 0;
        }
        // 计算广义加速度
        ForwardDynamics(*model, q, qd, tau, qdd);
        // 用 qd 和 qdd 填充状态导数 state derivative
        j = 0;
        for (int i = 0; i < model->dof_count; i++) {
            dxdt[j] = (double)qd[i];
            j++;
        }
        for (int i = 0; i < model->dof_count; i++) {
            dxdt[j] = (double)qdd[i];
            j++;
        }
    }

   private:
    RigidBodyDynamics::Model* model;
    RigidBodyDynamics::Math::VectorNd q, qd, qdd, tau;
};

int main(int argc, char* argv[]) {
    //---------------------------检查RBDL的API版本---------------------------
    rbdl_check_api_version(RBDL_API_VERSION);

    //---------------------------日志记录器配置部分---------------------------
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("logger1");
    logger->set_level(spdlog::level::info);
    logger->set_pattern("[%H:%M:%S.%f] [%ius] [%^%l%$] %v");

    // ---------------------------ODE 配置部分---------------------------
    double absTolVal = 1e-10;  // 绝对误差容忍度
    double relTolVal = 1e-6;   // 相对误差容忍度
    double a_x = 1.0;
    double a_dxdt = 1.0;
    controlled_stepper_type controlled_stepper(
        boost::numeric::odeint::default_error_checker<double, boost::numeric::odeint::range_algebra, boost::numeric::odeint::default_operations>(
            absTolVal, relTolVal, a_x, a_dxdt));  // 创建控制步长的步进器，使用 default_error_checker 来检查误差，用于积分过程

    // ---------------------------仿真初始化部分---------------------------
    // 从Lua文件中加载模型
    RigidBodyDynamics::Model* model = NULL;
    model = new RigidBodyDynamics::Model();
    std::string modelFile = "../../3.pendulum/model/pendulum.lua";
    RigidBodyDynamics::Addons::LuaModelReadFromFile(modelFile.c_str(), model, false);
    logger->info("model->dof_count: {}", model->dof_count);

    // 初始化广义坐标和广义速度
    RigidBodyDynamics::Math::VectorNd q, qd;
    q = RigidBodyDynamics::Math::VectorNd::Zero(model->dof_count);
    qd = RigidBodyDynamics::Math::VectorNd::Zero(model->dof_count);

    // 创建 Boost 系统模型，用于数值积分
    rbdlToBoost rbdlModel(model);

    // 仿真参数
    int nPts = 2000;                         // 总步长，Number of Points
    double t0 = 0;                           // 开始时间
    double t1 = 10;                          // 结束时间
    double dt = (t1 - t0) / ((double)nPts);  // 时间步长
    double t = 0;                            // 当前时间
    double t_pre = 0;                        // 上一时刻时间

    state_type xState(2 * model->dof_count);  // 系统状态变量，包括广义坐标和广义速度
    xState[0] = -M_PI / 6.0;                  // 初始角度（这里已知是单自由度的倒立摆）
    xState[1] = 0.0;                          // 初始角速度
    double ke = 0;                            // 动能，kinetic energy
    double pe = 0;                            // 势能，potential energy
    double te = 0;                            // 动能和势能之和，total energy

    // 记录仿真过程
    std::vector<std::vector<double>> matrixData;        // 用于保存仿真结果
    std::vector<double> rowData(1 + model->dof_count);  // 1 个时间 + n 个状态变量
    rowData[0] = 0;
    for (int i = 0; i < model->dof_count; i++) {
        rowData[i + 1] = xState[i];
    }
    matrixData.push_back(rowData);

    // ---------------------------开始仿真----------------------------
    logger->info("simulation start");
    logger->debug("    t,           q,           qd,          ke,           pe,     ke+pe-te");
    for (int i = 0; i <= nPts; i++) {
        // 记录当前时间
        t = t0 + dt * i;

        // 进行数值积分，得到下一个状态点
        boost::numeric::odeint::integrate_adaptive(controlled_stepper, rbdlModel, xState, t_pre, t, (t - t_pre) / 10.0);

        // 记录上一时刻的时间
        t_pre = t;

        // 每个状态点的动能和势能相加之和，理论上应当为常数，但积分会带来误差。
        q[0] = xState[0];
        qd[0] = xState[1];

        // 计算每个状态点的动能和势能。理论上动能势能之和应当为常数
        pe = RigidBodyDynamics::Utils::CalcPotentialEnergy(*model, q, true);
        ke = RigidBodyDynamics::Utils::CalcKineticEnergy(*model, q, qd, true);

        // 如果是第一个状态点，记录总能量
        if (i == 0) te = (ke + pe);

        // 输出每个状态点的状态参数
        logger->debug("{} {} {} {} {} {}", fmt::format("{:<12.6f}", t), fmt::format("{:<12.6f}", q[0]), fmt::format("{:<12.6f}", qd[0]),
                      fmt::format("{:<12.6f}", ke), fmt::format("{:<12.6f}", pe), fmt::format("{:<12.6f}", (ke + pe - te)));

        // 记录仿真过程
        rowData[0] = t;
        for (int j = 0; j < model->dof_count; j++) {
            rowData[j + 1] = xState[j];
        }
        matrixData.push_back(rowData);
    }
    logger->debug("    t,           q,           qd,          ke,           pe,     ke+pe-te");
    logger->info("simulation end");

    // ---------------------------保存仿真结果----------------------------
    std::string header = "";
    std::string fname = "../../3.pendulum/model/meshup.csv";
    printMatrixToFile(matrixData, header, fname);
    logger->info("Wrote: {}  (meshup animation file)", fname);

    delete model;

    return 0;
}