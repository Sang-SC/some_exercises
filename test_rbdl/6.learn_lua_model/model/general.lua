--[[
    自己写了一个 Lua 编写机器人模型简单例子，涵盖了基本元素
--]]

------------------------------定义一些变量------------------------------

-- 定义杆长
link1_length = 1  

-- 定义网格对象，用于rbdl-toolkit显示
meshes = {
    link1_mesh = {
        color = {0, 1, 0},           -- 指定该立方体RGB颜色
        mesh_center = {0, 0, link1_length/2},     -- 指定该立方体中心位置
        dimensions = {0.1, 0.1, link1_length},  -- 指定该立方体长宽高
        src = "unit_cube.obj",        -- rbdl自带的3D模型文件
    },
}

------------------------------定义模型------------------------------
model = {
    ------------------------------设置坐标轴为右手坐标系形式------------------------------
    configuration = {
        axis_front = {1, 0, 0},
        axis_right = {0, 1, 0},
        axis_up    = {0, 0, 1},
    },

    ------------------------------设置重力------------------------------
    gravity = {0., 0., -9.81},

    ------------------------------设置模型坐标系------------------------------
    frames = {
        -- 坐标系1，其中name和parent是必须的，body和joint是可选的。visuals仅仅用于rbdl-toolkit显示，不参与计算
        {
            name = "Link1",
            parent = "ROOT",
            body = {
                mass = 1.0,
                com = {0., 0., 0.},
                inertia = {
                    {1.0, 0.0, 0.0},
                    {0.0, 1.0, 0.0},
                    {0.0, 0.0, 1.0},
                }
            },
            joint = {
                {1., 0., 0., 0., 0., 0.},
                {0., 1., 0., 0., 0., 0.},
                {0., 0., 1., 0., 0., 0.},
                {0., 0., 0., 1., 0., 0.},
                {0., 0., 0., 0., 1., 0.},
                {0., 0., 0., 0., 0., 1.},
                -- "JointTypeTranslationXYZ",
                -- "JointTypeSpherical",
                -- "JointTypeEulerZYX",
            },
            joint_frame = {
                r = {0., 0., 1.},
                E = {
                    {1., 0., 0.},
                    {0., 1., 0.},
                    {0., 0., 1.},
                },
            },
            visuals = {meshes.link1_mesh,},
        },

        -- 坐标系2
        {
            name = "Link2",
            parent = "Link1",
        }
    },

    ------------------------------设置约束集------------------------------
    constraint_sets = {
        -- 这里不是很理解，为什么约束集里不直接是约束，而是约束集1、约束集2这样又套一层名字
        constraint_sets_1 = {
            -- 约束1
            {
                -- constraint_type必须，取值为contact或loop
                -- name可选
                -- 其他的参数分别是AddContactConstraint()和AddLoopConstraint()所需的参数，需要根据constraint_type取值来定义
                -- 下面列举了loop约束的所有参数
                constraint_type = "loop",
                name = "constraint1",
                predecessor_body = "ROOT",
                successor_body = "link1",
                predecessor_transform = {
                    E = {
                        {1, 0, 0},
                        {0, 1, 0},
                        {0, 0, 1},
                    },
                    r = {0.000,  1.0,  0.000,},
                },
                successor_transform = {
                    E = {
                        {1, 0, 0},
                        {0, 1, 0},
                        {0, 0, 1},
                    },
                    r = {0.0, 0.0, 0.0,},
                },
                axis = {0, 0, 0, 1, 0, 0},
                stabilization_coefficient = 1.0,
                baumgarte_enabled = false,
            },
        },
    },
}

return model