/**
 * @file 4.constrained_double_pendulum.cpp
 * @author your name (you@domain.com)
 * @brief 双摆模型仿真，其中 Lua
 * 模型中有两个刚性杆，一个通过单自由度旋转关节直接连接到ROOT，另一个通过6自由度球关节连接到ROOT。但是通过在两杆之间增加五个约束，实现了单自由度旋转关节的效果。RBDL
 * 计算状态导数，Boost 计算数值积分。 使用 rbdl-toolkit 打开 Lua 模型和 csv 仿真过程文件可查看动画。主要参考了RBDL官方例程constrainedDoublePendulum。
 * @version 0.1
 * @date 2024-02-24
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <rbdl/addons/luamodel/luamodel.h>
#include <rbdl/addons/luamodel/luatables.h>  // ? 还不知道什么用
#include <rbdl/rbdl.h>                       // 路径已经安装到系统目录 /usr/local/include/ 下，所以不需要指定路径
#include <rbdl/rbdl_utils.h>
#include <spdlog/fmt/ostr.h>                  // 输出 vector 向量例如 RigidBodyDynamics::Math::Vector3d 需要用到
#include <spdlog/sinks/stdout_color_sinks.h>  // 使用 stdout_color_mt 创建多线程、彩色的日志记录器
#include <spdlog/spdlog.h>

#include <boost/numeric/odeint/integrate/integrate_adaptive.hpp>
#include <boost/numeric/odeint/stepper/controlled_runge_kutta.hpp>
#include <boost/numeric/odeint/stepper/generation/make_controlled.hpp>
#include <boost/numeric/odeint/stepper/runge_kutta_cash_karp54.hpp>
#include <string>

#include "utils/csvtools.h"  // 需要使用 printMatrixToFile 函数，用于保存仿真结果

typedef std::vector<double> state_type;
typedef boost::numeric::odeint::runge_kutta_cash_karp54<state_type> error_stepper_type;
typedef boost::numeric::odeint::controlled_runge_kutta<error_stepper_type> controlled_stepper_type;

class rbdlToBoost {
   public:
    rbdlToBoost(RigidBodyDynamics::Model* model, std::vector<RigidBodyDynamics::ConstraintSet>& cs) : model(model), cs(cs) {
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

        // ! 计算广义加速度，注意这里需要使用一个特殊的前向动力学函数，以满足约束
        ForwardDynamicsConstraintsDirect(*model, q, qd, tau, cs[0], qdd);

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
    std::vector<RigidBodyDynamics::ConstraintSet>& cs;
};

int main(int argc, char* argv[]) {
    //---------------------------检查RBDL的API版本---------------------------
    rbdl_check_api_version(RBDL_API_VERSION);

    //---------------------------日志记录器配置部分---------------------------
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("logger1");
    logger->set_level(spdlog::level::debug);
    logger->set_pattern("[%H:%M:%S.%f] [%ius] [%^%l%$] %v");

    // ---------------------------ODE 配置部分---------------------------
    double absTolVal = 1e-6;  // 绝对误差容忍度
    double relTolVal = 1e-6;  // 相对误差容忍度
    double a_x = 1.0;
    double a_dxdt = 1.0;
    controlled_stepper_type controlled_stepper(
        boost::numeric::odeint::default_error_checker<double, boost::numeric::odeint::range_algebra, boost::numeric::odeint::default_operations>(
            absTolVal, relTolVal, a_x, a_dxdt));  // 创建控制步长的步进器，使用 default_error_checker 来检查误差，用于积分过程

    // ---------------------------仿真初始化部分---------------------------
    // 从Lua文件中加载模型，包括约束
    RigidBodyDynamics::Model* model = NULL;
    model = new RigidBodyDynamics::Model();
    std::string modelFile =
        "../../4.constrained_double_pendulum/model/constrainedDoublePendulum.lua";  // ! 注意路径得从 build 文件夹下的可执行文件开始算
    std::vector<std::string> constraintSetNames = RigidBodyDynamics::Addons::LuaModelGetConstraintSetNames(modelFile.c_str());
    std::vector<RigidBodyDynamics::ConstraintSet> constraintSets;
    constraintSets.resize(constraintSetNames.size());
    RigidBodyDynamics::Addons::LuaModelReadFromFileWithConstraints(modelFile.c_str(), model, constraintSets, constraintSetNames, false);
    logger->info("model->dof_count: {}", model->dof_count);
    logger->info("\nDegree of freedom overview: \n{}", RigidBodyDynamics::Utils::GetModelDOFOverview(*model));
    logger->info("\nModel Hierarchy:\n{}", RigidBodyDynamics::Utils::GetModelHierarchy(*model));

    logger->info("model->gravity: {}", model->gravity.transpose());

    // 初始化广义坐标、广义速度、广义加速度和广义力
    RigidBodyDynamics::Math::VectorNd q, qd, qdd, tau;
    q = RigidBodyDynamics::Math::VectorNd::Zero(model->dof_count);
    qd = RigidBodyDynamics::Math::VectorNd::Zero(model->dof_count);
    qdd = RigidBodyDynamics::Math::VectorNd::Zero(model->dof_count);
    tau = RigidBodyDynamics::Math::VectorNd::Zero(model->dof_count);

    q[2] = 1.0;  // Lua 模型中双摆初始位置重合，因此这里需要设置一下

    // 创建 Boost 系统模型，用于数值积分
    rbdlToBoost rbdlModel(model, constraintSets);

    // 仿真参数
    int nPts = 1000;                         // 总步长，Number of Points
    double t0 = 0;                           // 开始时间
    double t1 = 10;                          // 结束时间
    double dt = (t1 - t0) / ((double)nPts);  // 时间步长
    double t = 0;                            // 当前时间
    double t_pre = 0;                        // 上一时刻时间

    double ke = 0;                            // 动能，kinetic energy
    double pe = 0;                            // 势能，potential energy
    double te = 0;                            // 动能和势能之和，total energy
    state_type xState(model->dof_count * 2);  // 系统状态变量，包括广义坐标和广义速度
    int j = 0;
    for (unsigned int i = 0; i < (model->dof_count); ++i) {
        xState[j++] = q(i);
    }
    for (unsigned int i = 0; i < (model->dof_count); ++i) {
        xState[j++] = qd(i);
    }

    RigidBodyDynamics::Math::VectorNd constraintPosError = RigidBodyDynamics::Math::VectorNd::Zero(5);
    RigidBodyDynamics::Math::VectorNd constraintVelError = RigidBodyDynamics::Math::VectorNd::Zero(5);

    double constraintPosNorm, constraintVelNorm;

    // 记录仿真过程
    std::vector<std::vector<double>> matrixData;        // 用于保存仿真结果
    std::vector<double> rowData(1 + model->dof_count);  // 1 个时间 + n 个状态变量
    rowData[0] = 0;
    for (int i = 0; i < model->dof_count; i++) {
        // rowData[i + 1] = xState[i];
        rowData[i + 1] = xState[model->dof_count + i];
    }
    matrixData.push_back(rowData);

    // ---------------------------开始仿真----------------------------
    logger->info("simulation start");
    for (int i = 0; i <= nPts; i++) {
        // 记录当前时间
        t = t0 + dt * i;

        // 进行数值积分，得到下一个状态点
        boost::numeric::odeint::integrate_adaptive(controlled_stepper, rbdlModel, xState, t_pre, t, (t - t_pre) / 10000.0);

        // 记录上一时刻的时间
        t_pre = t;

        // 更新动能和势能
        int j = 0;
        for (unsigned int k = 0; k < model->dof_count; ++k) {
            q[k] = xState[j++];
        }
        for (unsigned int k = 0; k < model->dof_count; ++k) {
            qd[k] = xState[j++];
        }

        // ? 不懂
        // Ensure that the constraint set quantites are up to date
        ForwardDynamicsConstraintsDirect(*model, q, qd, tau, constraintSets[0], qdd);

        // 2d. The position-level and velocity level constraint set errors
        //     can be fished out of the vectors in the ConstraintSet struct
        RigidBodyDynamics::ConstraintSet& ci = constraintSets[0];
        constraintPosNorm = 0;
        constraintVelNorm = 0;
        for (int k = 0; k < 5; ++k) {
            constraintPosError(k) = ci.err(k);
            constraintVelError(k) = ci.errd(k);

            constraintPosNorm += ci.err(k) * ci.err(k);
            constraintVelNorm += ci.errd(k) * ci.errd(k);
        }
        constraintPosNorm = sqrt(constraintPosNorm);
        constraintVelNorm = sqrt(constraintVelNorm);

        // 计算每个状态点的动能和势能。理论上动能势能之和应当为常数
        pe = RigidBodyDynamics::Utils::CalcPotentialEnergy(*model, q, true);
        ke = RigidBodyDynamics::Utils::CalcKineticEnergy(*model, q, qd, true);

        // 如果是第一个状态点，记录总能量
        if (i == 0) te = (ke + pe);

        // 输出每个状态点的状态参数
        logger->debug("{}{}{}{}{}{}{}{}", fmt::format("{:<16.6f}", t), fmt::format("{:<16.6f}", q[0]), fmt::format("{:<16.6f}", qd[0]),
                      fmt::format("{:<16.6f}", ke), fmt::format("{:<16.6f}", pe), fmt::format("{:<16.6f}", (ke + pe - te)),
                      fmt::format("{:<16.6f}", constraintPosNorm), fmt::format("{:<16.6f}", constraintVelNorm));

        // 记录仿真过程
        rowData[0] = t;
        for (int j = 0; j < model->dof_count; j++) {
            rowData[j + 1] = xState[j];
            // rowData[j + 1] = xState[model->dof_count + j];
        }
        matrixData.push_back(rowData);
    }
    logger->debug(" {}{}{}{}{}{}{}{}", fmt::format("{:<16}", "t"), fmt::format("{:<16}", "q"), fmt::format("{:<16}", "qd"),
                  fmt::format("{:<16}", "ke"), fmt::format("{:<16}", "pe"), fmt::format("{:<16}", "ke + pe"), fmt::format("{:<16}", "norm(cons_pos)"),
                  fmt::format("{:<16}", "norm(cons_vel)"));
    logger->info("simulation end");

    // ---------------------------保存仿真结果----------------------------
    std::string header = "";
    std::string fname = "../../4.constrained_double_pendulum/model/meshup.csv";
    printMatrixToFile(matrixData, header, fname);
    logger->info("Wrote: {}  (meshup animation file)", fname);

    delete model;

    return 0;
}