#include <rbdl/rbdl.h>                        // RDBL库，主头文件
#include <rbdl/addons/luamodel/luamodel.h>    // RDBL库，加载Lua模型插件头文件
#include <spdlog/spdlog.h>                    // spdlog日志库，主头文件
#include <spdlog/sinks/stdout_color_sinks.h>  // spdlog日志库，多线程彩色控制台输出头文件
#include <spdlog/fmt/ostr.h>                  // spdlog日志库，fmt格式化输出vector向量要用到的头文件

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
    std::string modelFile = "../../6.learn_lua_model/model/general.lua";  // 注意路径从build文件夹下的可执行文件开始算
    // std::string modelFile = "./../6.learn_lua_model/example.lua";  // 注意路径从build文件夹下的可执行文件开始算
    std::vector<std::string> constraintSetNames = RBDL::Addons::LuaModelGetConstraintSetNames(modelFile.c_str());
    std::vector<RBDL::ConstraintSet> constraintSets;
    constraintSets.resize(constraintSetNames.size());
    RBDL::Addons::LuaModelReadFromFileWithConstraints(modelFile.c_str(), model, constraintSets, constraintSetNames, false);

    // ---------------------------输出部分---------------------------
    logger->info("model->gravity: {}", model->gravity.transpose());
    logger->info("model->dof_count: {}", model->dof_count);
    logger->info("\nModel Hierarchy:\n{}", RBDL::Utils::GetModelHierarchy(*model));
    logger->info("\nBody Origins Overview:\n{}", RBDL::Utils::GetNamedBodyOriginsOverview(*model));
    logger->debug("\nDegree of freedom overview: \n{}", RBDL::Utils::GetModelDOFOverview(*model));

    logger->info("constraintSetNames.size(): {}", constraintSetNames.size());
    for (int i = 0; i < constraintSetNames.size(); i++) {
        logger->info("constraintSetNames[{}]: {}", i, constraintSetNames[i]);
    }
    logger->info("");

    logger->info("constraintSets.size(): {}", constraintSets.size());
    for (int i = 0; i < constraintSets.size(); i++) {
        logger->info("constraintSets[{}].constraints.size: {}", i, constraintSets[i].constraints.size());
        for (int j = 0; j < constraintSets[i].constraints.size(); j++) {
            logger->info("constraintSets[{}].constraintType[{}]: {}", i, j,
                         constraintSets[i].constraintType[j]);  // 查看枚举类型可发现，0代表Contact，1代表Loop
        }
        logger->info("");
    }

    delete model;

    return 0;
}