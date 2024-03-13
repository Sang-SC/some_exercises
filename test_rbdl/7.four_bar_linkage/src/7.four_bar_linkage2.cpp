/**
 * @file 7.four_bar_linkage.cpp
 * @author your name (you@domain.com)
 * @brief 四连杆逆运动学仿真，但是是用两组逆运动学实现的，其实应该用末端闭环约束来实现
 * @version 0.1
 * @date 2024-03-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <rbdl/rbdl.h>                        // RDBL库，主头文件
#include <rbdl/addons/luamodel/luamodel.h>    // RDBL库，加载Lua模型插件头文件
#include <rbdl/Kinematics.h>                  // RDBL库，运动学计算头文件
#include "utils/csvtools.h"                   // 用于保存仿真结果，在rbdl-toolkit中进行动画展示
#include <spdlog/spdlog.h>                    // spdlog日志库，主头文件
#include <spdlog/sinks/stdout_color_sinks.h>  // spdlog日志库，多线程彩色控制台输出头文件
#include <spdlog/fmt/ostr.h>                  // spdlog日志库，fmt格式化输出vector向量要用到的头文件
#include <vector>
#include <string>
#include <random>
#include <cmath>

// 定义别名，简化使用
namespace RBDL = RigidBodyDynamics;

/**
 * @brief 一个检查广义坐标范围的函数
 *
 * @param angle
 * @return double
 */
bool checkCoordinateRange(const RBDL::Math::VectorNd coordinates) {
    if (coordinates.size() != 4) {
        return false;
    }

    if (coordinates[1] < -M_PI || coordinates[1] > 0) {
        return false;
    }

    if (coordinates[3] < 0 || coordinates[3] > M_PI) {
        return false;
    }

    return true;
}

/**
 * @brief 利用 RBDL::InverseKinematics 函数，重新编写的逆运动学求解函数，能够自动生成初始猜测值以满足广义坐标范围要求
 *
 * @param model
 * @param Qinit
 * @param CS
 * @param Qres
 * @return true
 * @return false
 */
bool MyInverseKinematics(RBDL::Model &model, const RBDL::Math::VectorNd &Qinit0, RBDL::InverseKinematicsConstraintSet &CS, RBDL::Math::VectorNd &Qres,
                         unsigned int max_iter, std::shared_ptr<spdlog::logger> logger) {
    RBDL::Math::VectorNd Qinit;
    std::random_device rd;                            // 随机设备，用于获取种子
    std::mt19937 gen(rd());                           // 随机数引擎
    std::uniform_real_distribution<> dis1(-M_PI, 0);  // 均匀分布，范围为-Pi到Pi
    std::uniform_real_distribution<> dis2(0, M_PI);   // 均匀分布，范围为-Pi到Pi

    for (unsigned int i = 0; i < max_iter; i++) {
        if (i == 0) {
            Qinit = Qinit0;
        } else {
            Qinit[1] = dis1(gen);
            Qinit[3] = dis2(gen);
        }
        if (RBDL::InverseKinematics(model, Qinit, CS, Qres)) {
            if (checkCoordinateRange(Qres)) {
                // 打印日志，输出迭代的次数
                if (i > 0) {
                    logger->info("Inverse kinematics solved! Iterations: {}", i);
                }
                return true;
            } else {
                continue;
            }
        } else {
            continue;
        };
    }
    return false;
}

int main(int argc, char *argv[]) {
    //---------------------------检查RBDL的API版本---------------------------
    rbdl_check_api_version(RBDL_API_VERSION);

    //---------------------------spdlog日志记录器配置部分---------------------------
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("logger1");
    logger->set_level(spdlog::level::debug);
    // logger->set_level(spdlog::level::info);
    // logger->set_level(spdlog::level::err);
    logger->set_pattern("[%H:%M:%S.%f] [%ius] [%^%l%$] %v");

    //---------------------------仿真初始化部分---------------------------
    // 从Lua文件中加载模型
    RBDL::Model *model = NULL;
    model = new RBDL::Model();
    std::string modelFile = "../../7.four_bar_linkage/model/four_bar_linkage.lua";  // 注意路径从build文件夹下的可执行文件开始算
    RBDL::Addons::LuaModelReadFromFile(modelFile.c_str(), model, false);

    logger->info("model->dof_count: {}", model->dof_count);
    logger->info("\nModel Hierarchy:\n{}", RBDL::Utils::GetModelHierarchy(*model));

    // 系统状态
    RBDL::Math::VectorNd q = RBDL::Math::VectorNd::Zero(model->dof_count);

    // 逆运动学相关参数
    RBDL::InverseKinematicsConstraintSet ik_constraint_set;
    RBDL::Math::VectorNd q_guess = RBDL::Math::VectorNd::Zero(model->dof_count);  // 关节角度初始猜测值设置为 0
    RBDL::Math::VectorNd q_ik = RBDL::Math::VectorNd::Zero(model->dof_count);     // 接收逆运动学求解结果
    RBDL::Math::Vector3d target_pos_ee = RBDL::Math::Vector3d(0., 0, 0);          // 末端执行器endeffector目标位置

    // 仿真参数
    int nPts = 10000;                // 总步长，Number of Points
    double t0 = 0;                   // 开始时间
    double t1 = 10;                  // 结束时间
    double T = t1 - t0;              // 总时间
    double dt = T / ((double)nPts);  // 时间步长
    double t = 0;                    // 当前时间
    double t_pre = 0;                // 上一时刻时间

    // 记录仿真过程
    std::vector<std::vector<double>> matrixData;        // 用于保存仿真结果
    std::vector<double> rowData(1 + model->dof_count);  // 1 个时间 + n 个状态变量
    rowData[0] = 0;
    for (int i = 0; i < model->dof_count; i++) {
        rowData[i + 1] = q[i];
    }
    matrixData.push_back(rowData);
    std::string header = "";
    std::string fname = "../../7.four_bar_linkage/model/meshup.csv";

    // ---------------------------开始仿真----------------------------
    logger->info("simulation start");

    for (int i = 0; i <= nPts; i++) {
        // 记录当前时间
        t = t0 + dt * i;

        if (i == 0) {
            q_guess[1] = -M_PI / 2;
            q_guess[3] = M_PI / 2;
        } else {
            q_guess = q;
        }

        // 小圆形轨迹测试
        // target_pos_ee = RBDL::Math::Vector3d(0.7 + 0.3 * sin(3 * t), -0.7 - 0.3 * cos(3 * t), 0.);
        // 水平直线轨迹测试
        // target_pos_ee = RBDL::Math::Vector3d(0.7 * sin(4 * t), -1.0, 0.);
        // 水平抛物线轨迹测试
        double x = 0.5 * sin(4 * t - M_PI / 2);
        double y = -0.5 - 2 * x * x;
        target_pos_ee = RBDL::Math::Vector3d(x, y, 0.);
        if (i == 0) {
            q_guess[1] = -M_PI / 2;
            q_guess[3] = M_PI / 2;
        } else {
            q_guess = q;
        }

        ik_constraint_set.ClearConstraints();
        ik_constraint_set.constraint_tol = 1.0e-12;
        ik_constraint_set.max_steps = 1000;
        //! 修改腿长的话需要改这里
        ik_constraint_set.AddPointConstraintXY(4, RBDL::Math::Vector3d(0., 0.8, 0.), target_pos_ee);
        ik_constraint_set.AddPointConstraintXY(2, RBDL::Math::Vector3d(0., 0.8, 0.), target_pos_ee);

        if (MyInverseKinematics(*model, q_guess, ik_constraint_set, q_ik, 1000, logger)) {
            q = q_ik;
        } else {
            logger->error("Inverse kinematics failed!");
            return -1;
        };

        // 记录上一时刻的时间
        t_pre = t;

        // 输出每个状态点的状态参数
        if (i % 500 == 0) {
            logger->debug("{}{}{}{}{}", fmt::format("{:<16.6f}", t), fmt::format("{:<16.6f}", q[0]), fmt::format("{:<16.6f}", q[1]),
                          fmt::format("{:<16.6f}", q[2]), fmt::format("{:<16.6f}", q[3]));
        }

        // 记录仿真过程
        rowData[0] = t;
        for (int i = 0; i < model->dof_count; i++) {
            rowData[i + 1] = q[i];
        }
        matrixData.push_back(rowData);
    }

    logger->debug("    {}{}{}{}{}", fmt::format("{:<16}", "t"), fmt::format("{:<16}", "q[0]"), fmt::format("{:<16}", "q[1]"),
                  fmt::format("{:<16}", "q[2]"), fmt::format("{:<16}", "q[3]"));
    logger->info("simulation end");

    printMatrixToFile(matrixData, header, fname);
    logger->info("Wrote: {}  (meshup animation file)", fname);

    delete model;

    return 0;
}