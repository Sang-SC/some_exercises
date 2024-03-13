--[[
    四杆机构，用于运动学测试的模型，因此没定义动力学属性
--]]

------------------------------定义一些变量------------------------------

link1_length = 0.6;
link2_length = 0.8;

joints = {
    revZ = {
      {0, 0, 1, 0, 0, 0},
    },
}

-- 定义网格对象，用于rbdl-toolkit显示
meshes = {
    link1_mesh = {
        color = {1, 0, 0},           -- 指定该立方体RGB颜色
        mesh_center = {0, link1_length/2, 0},     -- 指定该立方体中心位置
        dimensions = {0.05, link1_length, 0.05},  -- 指定该立方体长宽高
        src = "unit_cube.obj",        -- rbdl自带的3D模型文件
    },
    link2_mesh = {
        color = {0, 0, 1},
        mesh_center = {0, link2_length/2, 0},
        dimensions = {0.05, link2_length, 0.05},
        src = "unit_cube.obj",
    },
    link3_mesh = {
        color = {1, 0, 0.5},
        mesh_center = {0, link1_length/2, 0},
        dimensions = {0.05, link1_length, 0.05},
        src = "unit_cube.obj",
    },
    link4_mesh = {
        color = {0.5, 0, 1},
        mesh_center = {0, link2_length/2, 0},
        dimensions = {0.05, link2_length, 0.05},
        src = "unit_cube.obj",
    },
}

------------------------------定义模型------------------------------
model = {
    -- 设置坐标轴
    configuration = {
        axis_front = {0, 0, 1},
        axis_right = {1, 0, 0},
        axis_up    = {0, 1, 0},
    },

    -- 设置模型坐标系
    frames = {
        {
            name = "Link1",
            parent = "ROOT",
            joint = joints.revZ,
            joint_frame = {
                r = {0., 0., 0.},
                E = {
                    {1., 0., 0.},
                    {0., 1., 0.},
                    {0., 0., 1.},
                },
            },
            visuals = {meshes.link1_mesh,},
        },
        {
            name = "Link2",
            parent = "Link1",
            joint = joints.revZ,
            joint_frame = {
                r = {0., link1_length, 0.},
                E = {
                    {1., 0., 0.},
                    {0., 1., 0.},
                    {0., 0., 1.},
                },
            },
            visuals = {meshes.link2_mesh,},
        },
        {
            name = "Link3",
            parent = "ROOT",
            joint = joints.revZ,
            joint_frame = {
                r = {0., 0., 0.},
                E = {
                    {1., 0., 0.},
                    {0., 1., 0.},
                    {0., 0., 1.},
                },
            },
            visuals = {meshes.link3_mesh,},
        },
        {
            name = "Link4",
            parent = "Link3",
            joint = joints.revZ,
            joint_frame = {
                r = {0., link1_length, 0.},
                E = {
                    {1., 0., 0.},
                    {0., 1., 0.},
                    {0., 0., 1.},
                },
            },
            visuals = {meshes.link4_mesh,},
        },
    },
}

return model