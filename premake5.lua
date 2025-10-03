workspace "miniGame"
architecture "x64"
configurations{
    "Debug",
    "Release",
    "Dist"
}

-- 创建了一个名为 "miniGame" 的工作区
-- 指定架构为 x64（64位）
-- 定义了三种构建配置：Debug（调试）、Release（发布）和 Dist（分发）

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- 定义包含目录
IncludeDir = {
    GLFW = "miniGame/Vendor/GLFW/include",
    Glad = "miniGame/Vendor/Glad/include"
}

-- 定义GLAD库项目
project "Glad"
    location "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    
    targetdir (outputdir)
    objdir ("output/intermediate/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")

    files {
        "miniGame/Vendor/Glad/include/glad/glad.h",
        "miniGame/Vendor/Glad/include/KHR/khrplatform.h",
        "miniGame/Vendor/Glad/src/glad.c"
    }

    includedirs {
        "miniGame/Vendor/Glad/include"
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"
        defines {
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"

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
            "kernel32.lib",
            "ucrt.lib",
            "msvcrt.lib"
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
        "%{IncludeDir.Glad}",
        "miniGame/src"
    }
    
    -- 链接GLFW和Glad库
    links {
        "GLFW",
        "Glad",
        "opengl32.lib"
    }
    
    -- 设置依赖项，确保先编译GLFW和Glad
    dependson {
        "GLFW",
        "Glad"
    }
    
    -- 添加UTF-8编译选项
    filter {"system:windows"}
        defines {
            "HZ_PLATFORM_WINDOWS",
            "HZ_BUILD_DLL",
            "_CRT_SECURE_NO_WARNINGS",
            "GLFW_INCLUDE_NONE" -- 让GLFW不包含OpenGL
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