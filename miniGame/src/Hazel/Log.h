// Log.h - Hazel引擎日志系统
#pragma once

// 为了确保UTF-8编码支持，我们在代码文件中添加必要的编译指令
#ifdef _WIN32
#pragma execution_character_set("utf-8") // 启用UTF-8编码支持
#endif

#include "Core.h"
#include "spdlog/spdlog.h"

// 显式实例化模板以解决DLL导出警告
extern template class HAZEL_API std::shared_ptr<spdlog::logger>;

namespace Hazel {
    class HAZEL_API Log {
    public:
        static void Init(); // 初始化日志系统
        
        // 提供获取日志器的方法
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    
    private:
        // 静态成员变量定义
        static std::shared_ptr<spdlog::logger> s_CoreLogger; // Hazel项目日志器
        static std::shared_ptr<spdlog::logger> s_ClientLogger; // Sandbox项目日志器
    };
}

// Core log macros
// ...是接受函数参数包，__VA_ARGS__转发函数参数包
#define HZ_CORE_TRACE(...)    ::Hazel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HZ_CORE_INFO(...)     ::Hazel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_WARN(...)     ::Hazel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_ERROR(...)    ::Hazel::Log::GetCoreLogger()->error(__VA_ARGS__)
//#define HZ_CORE_FATAL(...)    ::Hazel::Log::GetCoreLogger()->fatal(__VA_ARGS__) 

// Client log macros
#define HZ_TRACE(...)	      ::Hazel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HZ_INFO(...)	      ::Hazel::Log::GetClientLogger()->info(__VA_ARGS__)
#define HZ_WARN(...)	      ::Hazel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HZ_ERROR(...)	      ::Hazel::Log::GetClientLogger()->error(__VA_ARGS__)
