-- 定义GLFW的包含目录
IncludeDir = IncludeDir or {}
IncludeDir["GLFW"] = "miniGame/Vendor/GLFW/include"

-- 注意：GLFW的premake配置已经在主premake5.lua文件中包含
-- 这里只需确保IncludeDir正确设置，避免重复包含导致的错误

-- 注意：miniGame项目的主要配置在主premake5.lua文件中
-- 这里只需确保GLFW的配置可用，实际的链接关系在主配置中定义
