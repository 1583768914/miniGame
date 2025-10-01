-- 注意：请使用生成在 vendor/bin/premake 目录下的 miniGame.sln 文件，而不是 miniGame 目录下的旧解决方案
workspace "miniGame"
architecture "x64"
configurations{
    "Debug",
    "Release",
    "Dist"
}

-- 
--#region- 创建了一个名为 "miniGame" 的工作区
-- 指定架构为 x64（64位）
-- 定义了三种构建配置：Debug（调试）、Release（发布）和 Dist（分发）

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
-- 路径组成解析
-- 1. 1.
--    output/
   
--    - 这是基础输出目录，所有构建产物都将放置在项目根目录下的 output 文件夹中
-- 2. 2.
--    %{cfg.buildcfg}
   
--    - Premake 内置变量，表示构建配置类型
--    - 在当前项目中，对应的值可以是 Debug 、 Release 或 Dist （在脚本的 configurations 部分定义）
-- 3. 3.
--    %{cfg.system}
   
--    - Premake 内置变量，表示目标操作系统
--    - 根据构建环境不同，可能的值包括 windows 、 linux 或 macosx
-- 4. 4.
--    %{cfg.architecture}
   
--    - Premake 内置变量，表示目标架构
--    - 在当前项目中，固定为 x64 （由 architecture "x64" 配置指定）

-- 定义包含目录
IncludeDir = {
    GLFW = "miniGame/Vendor/GLFW/include"
}

-- 定义GLFW项目
project "GLFW"
    location "GLFW"  -- 项目位置
    kind "StaticLib" --设置为静态库
    language "C"  --使用C语言开发
    
    targetdir (outputdir) --输出目录
    objdir ("output/intermediate/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
    --设置中间目录，用于存放编译中间文件

    includedirs {
        "miniGame/Vendor/GLFW/include"
    }

    -- Windows平台特定配置
    filter {"system:windows"}
        defines {
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }
        links {
            "opengl32.lib",
            "gdi32.lib",
            "user32.lib",
            "kernel32.lib"
        }
        files {
            "miniGame/Vendor/GLFW/src/win32_*.c",
            "miniGame/Vendor/GLFW/src/win32_*.h",
            "miniGame/Vendor/GLFW/src/wgl_context.c",
            "miniGame/Vendor/GLFW/src/wgl_context.h",
            "miniGame/Vendor/GLFW/src/init.c",
            "miniGame/Vendor/GLFW/src/input.c",
            "miniGame/Vendor/GLFW/src/monitor.c",
            "miniGame/Vendor/GLFW/src/window.c",
            "miniGame/Vendor/GLFW/src/context.c",
            "miniGame/Vendor/GLFW/src/vulkan.c",
            "miniGame/Vendor/GLFW/src/platform.c",
            "miniGame/Vendor/GLFW/src/internal.h",
            "miniGame/Vendor/GLFW/src/platform.h",
            "miniGame/Vendor/GLFW/src/null_*.c",
            "miniGame/Vendor/GLFW/src/null_*.h",
            "miniGame/Vendor/GLFW/src/egl_context.c",
            "miniGame/Vendor/GLFW/src/osmesa_context.c"
        }
    
    filter {"system:windows"}
        defines {
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }
        links {
            "opengl32.lib",
            "gdi32.lib",
            "user32.lib",
            "kernel32.lib"
        }
        -- 在Windows平台下排除非Windows特有的源文件
        removefiles {
            "miniGame/Vendor/GLFW/src/**.m",
            "miniGame/Vendor/GLFW/src/**.mm",
            "miniGame/Vendor/GLFW/src/x11_*.c",
            "miniGame/Vendor/GLFW/src/wl_*.c",
            "miniGame/Vendor/GLFW/src/mir_*.c",
            "miniGame/Vendor/GLFW/src/cocoa_*.c",
            "miniGame/Vendor/GLFW/src/cocoa_*.m"
        }
    
    filter {"system:linux"}
        defines {
            "_GLFW_X11"
        }
        links {
            "X11",
            "Xi",
            "Xrandr",
            "Xxf86vm",
            "Xinerama",
            "Xcursor",
            "GL"
        }
    
    filter {"system:macosx"}
        defines {
            "_GLFW_COCOA"
        }
        links {
            "OpenGL.framework",
            "Cocoa.framework",
            "IOKit.framework",
            "CoreVideo.framework"
        }
    
    -- 排除测试和示例代码
    removefiles {
        "miniGame/Vendor/GLFW/tests/**",
        "miniGame/Vendor/GLFW/examples/**"
    }

project "miniGame" -- 修改为与实际项目名称匹配
    location "miniGame" -- 正确的项目位置
    kind "SharedLib"
    language "C++"
    targetdir (outputdir)
    objdir ("output/intermediate/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
    
    -- 明确列出所有源文件，确保都能被包含
    files{
        "miniGame/src/**.h",
        "miniGame/src/**.cpp",
        "miniGame/src/Hazel/**.h",
        "miniGame/src/Hazel/**.cpp",
        "miniGame/src/Platform/**.h",
        "miniGame/src/Platform/**.cpp"
    }
    
    includedirs{
        "miniGame/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "miniGame/src"
    }
    
    -- 链接GLFW库
    links {
        "GLFW",
        "opengl32.lib"
    }
    
    -- 设置GLFW为依赖项，确保先编译GLFW
    dependson {
        "GLFW"
    }
    
    -- 添加UTF-8编译选项
    filter {"system:windows"}
        defines {
            "HZ_PLATFORM_WINDOWS",
            "HZ_BUILD_DLL",
            "_CRT_SECURE_NO_WARNINGS"
        }
        buildoptions "/utf-8"
        characterset ("Unicode")
    
project "sandBox" -- 注意大小写
    location "sandBox" -- 正确的项目位置
    kind "ConsoleApp"
    language "C++"
    
    targetdir (outputdir)
    objdir ("output/intermediate/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
    
    files{
        "sandBox/src/**.h",
        "sandBox/src/**.cpp"
    }
    
    includedirs{
        "miniGame/vendor/spdlog/include",
        "miniGame/src"
    }
    
    links{
        "miniGame" -- 链接到正确的库名称
    }
    
    -- 添加UTF-8编译选项
    filter {"system:windows"}
        defines {
            "HZ_PLATFORM_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS"
        }
        buildoptions "/utf-8"
        characterset ("Unicode")
    
    -- 其他设置保持不变...