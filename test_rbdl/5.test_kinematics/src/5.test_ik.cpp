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
    logger->set_level(spdlog::level::info);
    // logger->set_level(spdlog::level::err);
    logger->set_pattern("[%H:%M:%S.%f] [%ius] [%^%l%$] %v");

    //---------------------------仿真初始化部分---------------------------
    // 从Lua文件中加载模型
    RBDL::Model* model = NULL;
    model = new RBDL::Model();
    std::string modelFile = "../../5.test_kinematics/model/2dof_arm.lua";  // ! 注意路径得从 build 文件夹下的可执行文件开始算
    RBDL::Addons::LuaModelReadFromFile(modelFile.c_str(), model, false);

    logger->info("model->dof_count: {}", model->dof_count);
    logger->debug("\nDegree of freedom overview: \n{}", RBDL::Utils::GetModelDOFOverview(*model));
    logger->info("\nModel Hierarchy:\n{}", RBDL::Utils::GetModelHierarchy(*model));

    // 定义系统状态变量并初始化
    RBDL::Math::VectorNd q = RBDL::Math::VectorNd::Zero(model->dof_count);
    q[0] = 0.00001;  // 如果q初始为0，求解逆运动学的时候不知道为什么总是能求解但不对，赋一个很小的值就可以了。不太理解，
    RBDL::Math::VectorNd qd = RBDL::Math::VectorNd::Zero(model->dof_count);
    RBDL::Math::VectorNd qdd = RBDL::Math::VectorNd::Zero(model->dof_count);

    // 逆运动学相关参数
    RBDL::Math::VectorNd q_guess = RBDL::Math::VectorNd::Zero(model->dof_count);  // 关节角度初始猜测值初始化为 0
    std::vector<unsigned int> body_id = {2};
    std::vector<RBDL::Math::Vector3d> body_point = {RBDL::Math::Vector3d(0., 0, 1)};
    std::vector<RBDL::Math::Vector3d> target_pos = {RBDL::Math::Vector3d(0., 0, 0)};
    RBDL::Math::VectorNd q_ik = RBDL::Math::VectorNd::Zero(model->dof_count);  // 接收逆运动学求解结果
    double step_tol1 = 1.0e-10;                                                // 最大容许误差（用于逆运动学算法）
    double step_tol2 = 1.0e-4;                                                 // 最大容许误差（用于检验）

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

        // 逆运动学求解的初始猜测值设置为上一时刻的关节角度
        q_guess = q;

        // ---------------------------轨迹1：直线轨迹---------------------------
        target_pos = {RBDL::Math::Vector3d(0., sin(t), 1)};

        // ---------------------------轨迹2：正方形轨迹---------------------------
        if (t <= T / 4) {
            target_pos = {RBDL::Math::Vector3d(0, 0.2, 0.2) + RBDL::Math::Vector3d(0, 0, t * 4 / T)};
        } else if (T / 4 < t && t <= T / 2) {
            target_pos = {RBDL::Math::Vector3d(0, 0.2, 0.2) + RBDL::Math::Vector3d(0, t * 4 / T - 1, 1)};
        } else if (T / 2 < t && t <= T * 3 / 4) {
            target_pos = {RBDL::Math::Vector3d(0, 0.2, 0.2) + RBDL::Math::Vector3d(0, 1, 3 - t * 4 / T)};
        } else if (T * 3 / 4 < t && t <= T) {
            target_pos = {RBDL::Math::Vector3d(0, 0.2, 0.2) + RBDL::Math::Vector3d(0, 4 - t * 4 / T, 0)};
        }

        // ---------------------------轨迹3：大圆形轨迹---------------------------
        target_pos = {RBDL::Math::Vector3d(0, 1.5 * sin(t), 1.5 * cos(t))};

        // // ---------------------------轨迹4：小圆形轨迹---------------------------
        // target_pos = {RBDL::Math::Vector3d(0, 0.7 + 0.5 * sin(3 * t), 0.7 + 0.5 * cos(3 * t))};

        // 开始求解
        if (RBDL::InverseKinematics(*model, q_guess, body_id, body_point, target_pos, q_ik, step_tol1, 0.001, 500)) {
            std::vector<RBDL::Math::Vector3d> actual_pos(body_id.size());
            actual_pos[0] = RBDL::CalcBodyToBaseCoordinates(*model, q_ik, body_id[0], body_point[0], false);
            RBDL::Math::VectorNd e = RBDL::Math::VectorNd::Zero(3 * body_id.size());
            // 计算 e
            for (unsigned int i = 0; i < body_id.size(); i++) {
                for (unsigned int j = 0; j < 3; j++) {
                    e[3 * i + j] = target_pos[i][j] - actual_pos[i][j];
                    logger->trace("e: {}", e[3 * i + j]);
                }
                if (e.norm() > step_tol2) {
                    logger->error("Inverse Kinematics Solved But Not Accurate!");
                    logger->error("e.norm() - step_tol2 = {}", e.norm() - step_tol2);
                    logger->error("target_pos: {}", target_pos[0].transpose());
                    logger->error("actual_pos: {}", actual_pos[0].transpose());
                    logger->error("q_ik: {}", q_ik.transpose());
                } else {
                    logger->debug("Inverse Kinematics Solved!");
                    logger->debug("target_pos: {}", target_pos[0].transpose());
                    logger->debug("actual_pos: {}", actual_pos[0].transpose());
                    logger->debug("q_ik: {}", q_ik.transpose());
                    q = q_ik;
                }
            }
        } else {
            logger->error("Inverse Kinematics Failed!");
        }

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
    logger->debug("    {}{}{}", fmt::format("{:<16}", "t"), fmt::format("{:<16}", "q"), fmt::format("{:<16}", "qd"));
    logger->info("simulation end");

    // ---------------------------保存仿真结果到csv文件----------------------------
    std::string header = "";
    std::string fname = "../../5.test_kinematics/model/meshup.csv";
    printMatrixToFile(matrixData, header, fname);
    logger->info("Wrote: {}  (meshup animation file)", fname);

    delete model;

    return 0;
}