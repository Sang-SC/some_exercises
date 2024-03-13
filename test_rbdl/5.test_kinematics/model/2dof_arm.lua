--[[
    手动编写，2自由度机械臂Lua模型文件
--]]

--定义一些变量
link1_length = 1  -- 杆长1
link2_length = 1  -- 杆长2

--定义网格对象，后面会用到
meshes = {
    link1_mesh = {
        color = {1, 0, 0},
        mesh_center = {0, 0, link1_length/2},
        dimensions = {0.1, 0.1, link1_length},
        src = "unit_cube.obj"
    },
    link2_mesh = {
        color = {0, 1, 0},
        mesh_center = {0, 0, link2_length/2},
        dimensions = {0.1, 0.1, link2_length},
        src = "unit_cube.obj"
    }
}

--定义模型
model = {
    -- 设置坐标轴为自己熟悉的形式
    configuration = {
        --right, up, front遵循RGB的颜色顺序，
        axis_right = {0, 1, 0},
        axis_up    = {0, 0, 1},
        axis_front = {1, 0, 0}
    },

    frames = {
        {
            name = "Link1",
            parent = "ROOT",
            visuals = {meshes.link1_mesh},
            joint = {
                {1, 0, 0, 0, 0, 0}
            },
            joint_frame = {
                r = {0, 0, 0},
                E = {
                    {1, 0, 0},
                    {0, 1, 0},
                    {0, 0, 1}}
            }
        },
        {
            name = "Link2",
            parent = "Link1",
            visuals = {meshes.link2_mesh},
            joint = {
                {1, 0, 0, 0, 0, 0}
            },
            joint_frame = {
                r = {0, 0, link2_length},
                E = {
                    {1, 0, 0},
                    {0, 1, 0},
                    {0, 0, 1}}
            }
        }
    }
}

return model