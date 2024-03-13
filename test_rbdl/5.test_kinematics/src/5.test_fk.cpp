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
    RBDL::Math::VectorNd qd = RBDL::Math::VectorNd::Zero(model->dof_count);
    RBDL::Math::VectorNd qdd = RBDL::Math::VectorNd::Zero(model->dof_count);

    // 逆运动学相关参数
    RBDL::Math::VectorNd q_guess = RBDL::Math::VectorNd::Zero(model->dof_count);  // 关节角度初始猜测值设置为 0
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

        // ---------------------------测试1：机器人静止---------------------------
        q = RBDL::Math::VectorNd::Zero(model->dof_count);

        // ---------------------------测试2：指定关节线性轨迹---------------------------
        q[0] = t;
        q[1] = -t;

        // ---------------------------测试3：指定关节正弦波轨迹---------------------------
        q[0] = M_PI / 4 * std::sin(t);
        q[1] = M_PI / 4 * std::sin(t);

        // body1 坐标系原点在世界坐标系的位置
        RBDL::Math::Vector3d body1_point = RBDL::Math::Vector3d::Zero();
        logger->trace("body1_point: {}", body1_point.transpose());
        RBDL::Math::Vector3d body1_to_base_coordinate = RBDL::CalcBodyToBaseCoordinates(*model, q, 1, body1_point, true);
        logger->trace("body1_to_base_coordinate: {}", body1_to_base_coordinate.transpose());

        // body2 坐标系原点在世界坐标系的位置
        RBDL::Math::Vector3d body2_point = RBDL::Math::Vector3d::Zero();
        logger->trace("body2_point: {}", body2_point.transpose());
        RBDL::Math::Vector3d body2_to_base_coordinate = RBDL::CalcBodyToBaseCoordinates(*model, q, 2, body2_point, true);
        logger->trace("body2_to_base_coordinate: {}", body2_to_base_coordinate.transpose());

        // body1 在世界坐标系的姿态
        RBDL::Math::Matrix3d body1_world_orientation = RBDL::CalcBodyWorldOrientation(*model, q, 1, true);
        logger->trace("body1_world_orientation: \n{}", body1_world_orientation);
        logger->trace("rotx({:.6f}): \n{}", q[0], RBDL::Math::rotx(q[0]));

        // body2 在世界坐标系的姿态
        RBDL::Math::Matrix3d body2_world_orientation = RBDL::CalcBodyWorldOrientation(*model, q, 2, true);
        logger->trace("body2_world_orientation: \n{}", body2_world_orientation);
        logger->trace("rotx({:.6f}): \n{}", q[0] + q[1], RBDL::Math::rotx(q[0] + q[1]));

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