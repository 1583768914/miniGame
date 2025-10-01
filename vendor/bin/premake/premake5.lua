-- 注意：请使用生成在 vendor/bin/premake 目录下的 miniGame.sln 文件，而不是 miniGame 目录下的旧解决方案
workspace "miniGame"
architecture "x64"
configurations{
    "Debug",
    "Release",
    "Dist"
}

outputdir = "output/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- 定义包含目录
IncludeDir = {
    GLFW = "miniGame/Vendor/GLFW/include"
}

-- 定义GLFW项目
project "GLFW"
    location "GLFW"  -- 更简单的路径，确保能正确生成项目文件
    kind "StaticLib"
    language "C"
    
    targetdir (outputdir)
    objdir ("output/intermediate/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
    
    files {
        "miniGame/Vendor/GLFW/src/**.h",
        "miniGame/Vendor/GLFW/src/**.c",
        "miniGame/Vendor/GLFW/src/**.m",
        "miniGame/Vendor/GLFW/src/**.mm"
    }
    
    includedirs {
        "miniGame/Vendor/GLFW/include"
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
        "miniGame/src/**.def",
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
        buildoptions "/utf-8"
        characterset ("Unicode")
    
    -- 其他设置保持不变...