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
#include <cmath>

// 定义别名，简化使用
namespace RBDL = RigidBodyDynamics;

int main(int argc, char* argv[]) {
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
    RBDL::Model* model = NULL;
    model = new RBDL::Model();
    std::string modelFile = "../../7.four_bar_linkage/model/test_ik_constraint.lua";  // 注意路径从build文件夹下的可执行文件开始算
    std::vector<std::string> constraintSetNames = RBDL::Addons::LuaModelGetConstraintSetNames(modelFile.c_str());
    std::vector<RBDL::ConstraintSet> constraintSets;
    constraintSets.resize(constraintSetNames.size());
    RBDL::Addons::LuaModelReadFromFileWithConstraints(modelFile.c_str(), model, constraintSets, constraintSetNames, false);

    logger->info("model->dof_count: {}", model->dof_count);
    logger->info("\nModel Hierarchy:\n{}", RBDL::Utils::GetModelHierarchy(*model));
    logger->info("constraintSetNames.size(): {}", constraintSetNames.size());
    for (int i = 0; i < constraintSetNames.size(); i++) {
        logger->info("constraintSetNames[{}]: {}", i, constraintSetNames[i]);
    }

    // 系统状态
    RBDL::Math::VectorNd q = RBDL::Math::VectorNd::Zero(model->dof_count);

    // 逆运动学相关参数
    RBDL::Math::VectorNd q_guess = RBDL::Math::VectorNd::Zero(model->dof_count);  // 关节角度初始猜测值设置为 0
    std::vector<unsigned int> body_id = {2};
    std::vector<RBDL::Math::Vector3d> body_point = {RBDL::Math::Vector3d(0., 0.8, 0)};
    std::vector<RBDL::Math::Vector3d> target_pos = {RBDL::Math::Vector3d(0., 0, 0)};
    RBDL::Math::Vector3d target_pos_endeffector = RBDL::Math::Vector3d(0., 0, 0);  // 末端执行器目标位置
    RBDL::Math::VectorNd q_ik = RBDL::Math::VectorNd::Zero(model->dof_count);      // 接收逆运动学求解结果
    double step_tol1 = 1.0e-10;                                                    // 最大容许误差（用于逆运动学算法）
    double step_tol2 = 1.0e-4;                                                     // 最大容许误差（用于检验）

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

    // ---------------------------开始仿真----------------------------
    logger->info("simulation start");

    for (int i = 0; i <= nPts; i++) {
        // 记录当前时间
        t = t0 + dt * i;

        // 运动学测试
        // 测试1：直线轨迹
        // target_pos_endeffector = RBDL::Math::Vector3d(0.5 * sin(t), 0.8, 0.);
        // 测试2：大圆形轨迹
        // target_pos_endeffector = RBDL::Math::Vector3d(1.0 * sin(t), 1.0 * cos(t), 0.);
        // 测试3：小圆形轨迹
        target_pos_endeffector = RBDL::Math::Vector3d(0.7 + 0.3 * sin(3 * t), 0.7 + 0.3 * cos(3 * t), 0.);

        target_pos = {target_pos_endeffector, target_pos_endeffector};
        if (i == 0) {
            q_guess[1] = -M_PI / 2;
        } else {
            q_guess = q;
        }
        // if (RBDL::InverseKinematics(*model, q_guess, body_id, body_point, target_pos, q_ik, step_tol1, 0.001, 1000)) {
        //     q = q_ik;

        // };

        RBDL::InverseKinematicsConstraintSet ik_constraint_set;
        ik_constraint_set.AddPointConstraintXY(2, RBDL::Math::Vector3d(0., 0.8, 0.), target_pos_endeffector);
        logger->debug("ik_constraint_set.num_constraints: {}", ik_constraint_set.num_constraints);
        if (RBDL::InverseKinematics(*model, q_guess, ik_constraint_set, q_ik)) {
            logger->debug("Inverse Kinematics Solved!");
            q = q_ik;
        } else {
            logger->error("Inverse Kinematics Failed!");
            return -1;
        };

        // 记录上一时刻的时间
        t_pre = t;

        // 输出每个状态点的状态参数
        logger->debug("{}{}{}", fmt::format("{:<16.6f}", t), fmt::format("{:<16.6f}", q[0]), fmt::format("{:<16.6f}", q[1]));

        // 记录仿真过程
        rowData[0] = t;
        for (int i = 0; i < model->dof_count; i++) {
            rowData[i + 1] = q[i];
        }
        matrixData.push_back(rowData);
    }
    logger->debug("    {}{}{}", fmt::format("{:<16}", "t"), fmt::format("{:<16}", "q[0]"), fmt::format("{:<16}", "q[1]"));
    logger->info("simulation end");

    // ---------------------------保存仿真结果到csv文件----------------------------
    std::string header = "";
    std::string fname = "../../7.four_bar_linkage/model/meshup.csv";
    printMatrixToFile(matrixData, header, fname);
    logger->info("Wrote: {}  (meshup animation file)", fname);

    delete model;

    return 0;
}