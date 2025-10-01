
// Log.cpp - Hazel引擎日志系统实现

// 为了确保UTF-8编码支持，我们在代码文件中添加必要的编译指令
#ifdef _WIN32
#pragma execution_character_set("utf-8") // 启用UTF-8编码支持
#endif

#include "Core.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Hazel
{
    // 静态成员变量定义 - 添加HAZEL_API修饰符以支持DLL导出
    HAZEL_API std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    HAZEL_API std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    // Log::Init()方法的实现 - 添加HAZEL_API修饰符以支持DLL导出
    HAZEL_API void Log::Init() {
        // 设置日志格式：时间戳 + 日志名称 + 消息内容
        spdlog::set_pattern("%^[%T] %n: %v%$");
        
        // 初始化核心日志器
        s_CoreLogger = spdlog::stdout_color_mt("Hazel");
        s_CoreLogger->set_level(spdlog::level::trace);
        
        // 初始化客户端日志器
        s_ClientLogger = spdlog::stdout_color_mt("APP");
        s_ClientLogger->set_level(spdlog::level::trace);
    }
} // namespace Hazel
