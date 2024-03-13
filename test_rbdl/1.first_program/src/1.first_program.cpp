/**
 * @file first_program.cpp
 * @author your name (you@domain.com)
 * @brief 手动建立三自由度机器人模型，计算正动力学并输出相关信息。主要参考了RBDL官方例程simple
 * @version 0.1
 * @date 2024-02-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <rbdl/rbdl.h>  // 路径已经安装到系统目录 /usr/local/include/ 下，所以不需要指定路径
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

    unsigned int body_a_id, body_b_id, body_c_id;
    RigidBodyDynamics::Body body_a, body_b, body_c;
    RigidBodyDynamics::Joint joint_a, joint_b, joint_c;

    model->gravity = RigidBodyDynamics::Math::Vector3d(0.0, -9.81, 0.0);

    body_a = RigidBodyDynamics::Body(1.0, RigidBodyDynamics::Math::Vector3d(0.5, 0.0, 0.0), RigidBodyDynamics::Math::Vector3d(1.0, 1.0, 1.0));
    joint_a = RigidBodyDynamics::Joint(RigidBodyDynamics::JointTypeRevolute, RigidBodyDynamics::Math::Vector3d(0.0, 0.0, 1.0));
    body_a_id = model->AddBody(0, RigidBodyDynamics::Math::Xtrans(RigidBodyDynamics::Math::Vector3d(0.0, 0.0, 0.0)), joint_a, body_a);

    body_b = RigidBodyDynamics::Body(2.0, RigidBodyDynamics::Math::Vector3d(0.0, 0.5, 0.0), RigidBodyDynamics::Math::Vector3d(1.0, 1.0, 1.0));
    joint_b = RigidBodyDynamics::Joint(RigidBodyDynamics::JointTypeRevolute, RigidBodyDynamics::Math::Vector3d(0.0, 0.0, 1.0));
    body_b_id = model->AddBody(body_a_id, RigidBodyDynamics::Math::Xtrans(RigidBodyDynamics::Math::Vector3d(1.0, 0.0, 0.0)), joint_b, body_b);

    body_c = RigidBodyDynamics::Body(3.0, RigidBodyDynamics::Math::Vector3d(0.0, 0.5, 0.0), RigidBodyDynamics::Math::Vector3d(1.0, 1.0, 1.0));
    joint_c = RigidBodyDynamics::Joint(RigidBodyDynamics::JointTypeRevolute, RigidBodyDynamics::Math::Vector3d(0.0, 0.0, 1.0));
    body_c_id = model->AddBody(body_b_id, RigidBodyDynamics::Math::Xtrans(RigidBodyDynamics::Math::Vector3d(1.0, 0.0, 0.0)), joint_c, body_c);

    RigidBodyDynamics::Math::VectorNd q = RigidBodyDynamics::Math::VectorNd::Zero(model->q_size);
    RigidBodyDynamics::Math::VectorNd qd = RigidBodyDynamics::Math::VectorNd::Zero(model->qdot_size);
    RigidBodyDynamics::Math::VectorNd tau = RigidBodyDynamics::Math::VectorNd::Zero(model->qdot_size);
    RigidBodyDynamics::Math::VectorNd qdd = RigidBodyDynamics::Math::VectorNd::Zero(model->qdot_size);

    RigidBodyDynamics::ForwardDynamics(*model, q, qd, tau, qd);

    // ---------------------------输出部分---------------------------
    logger->info("model->dof_count: {}", model->dof_count);
    logger->info("\nDegree of freedom overview:\n{}", RigidBodyDynamics::Utils::GetModelDOFOverview(*model));
    logger->info("\nModel Hierarchy:\n{}", RigidBodyDynamics::Utils::GetModelHierarchy(*model));

    logger->info("model->gravity: {}\n", model->gravity.transpose());

    logger->info("body_a.mMass: {}", body_a.mMass);
    logger->info("body_a.mCenterOfMass: {}", body_a.mCenterOfMass.transpose());
    logger->info("body_a.mInertia: \n{}\n", body_a.mInertia);

    logger->info("joint_a.mJointType: {}", joint_a.mJointType);
    logger->info("joint_a.mJointAxes[0]: {}", joint_a.mJointAxes[0].transpose());
    logger->info("joint_a.mJointAxes[1]: {}", joint_a.mJointAxes[1].transpose());
    logger->info("joint_a.mJointAxes[2]: {}\n", joint_a.mJointAxes[2].transpose());

    logger->info("body_a_id: {}", body_a_id);
    logger->info("body_b_id: {}", body_b_id);
    logger->info("body_c_id: {}\n", body_c_id);

    logger->info("q: {}", q.transpose());
    logger->info("qd: {}", qd.transpose());
    logger->info("tau: {}", tau.transpose());
    logger->info("qdd: {}", qdd.transpose());

    delete model;

    return 0;
}
