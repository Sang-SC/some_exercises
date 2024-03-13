/**
 * @file 2.read_urdf.cpp
 * @author your name (you@domain.com)
 * @brief 从urdf文件加载模型，并输出机器人结构树、广义加速度等信息。主要参考了RBDL官方例程urdfreader。
 * @version 0.1
 * @date 2024-02-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <rbdl/addons/urdfreader/urdfreader.h>
#include <rbdl/rbdl.h>  // 路径已经安装到系统目录 /usr/local/include/ 下，所以不需要指定路径
#include <rbdl/rbdl_utils.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

int main(int argc, char* argv[]) {
    //---------------------------创建logger对象，设置日志级别---------------------------
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("logger1");
    logger->set_level(spdlog::level::info);

    // ---------------------------RBDL部分---------------------------
    rbdl_check_api_version(RBDL_API_VERSION);

    RigidBodyDynamics::Model* model = NULL;
    model = new RigidBodyDynamics::Model();

    RigidBodyDynamics::Addons::URDFReadFromFile("../../2.read_urdf/model/nao.urdf", model, false);  // ! 注意路径得从可执行文件所在的文件夹开始算

    RigidBodyDynamics::Math::VectorNd q = RigidBodyDynamics::Math::VectorNd::Zero(model->q_size);
    RigidBodyDynamics::Math::VectorNd qd = RigidBodyDynamics::Math::VectorNd::Zero(model->qdot_size);
    RigidBodyDynamics::Math::VectorNd tau = RigidBodyDynamics::Math::VectorNd::Zero(model->qdot_size);
    RigidBodyDynamics::Math::VectorNd qdd = RigidBodyDynamics::Math::VectorNd::Zero(model->qdot_size);

    RigidBodyDynamics::ForwardDynamics(*model, q, qd, tau, qd);

    // ---------------------------输出部分---------------------------
    logger->info("model->dof_count: {}", model->dof_count);
    logger->info("\nDegree of freedom overview: \n{}", RigidBodyDynamics::Utils::GetModelDOFOverview(*model));
    logger->info("\nModel Hierarchy:\n{}", RigidBodyDynamics::Utils::GetModelHierarchy(*model));

    logger->info("model->gravity: {}", model->gravity.transpose());

    logger->info("q: {}", q.transpose());
    logger->info("qd: {}", qd.transpose());
    logger->info("tau: {}", tau.transpose());
    logger->info("qdd: {}", qdd.transpose());

    delete model;

    return 0;
}