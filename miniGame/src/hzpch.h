#pragma once

// 禁用fmt库的Unicode支持，解决静态断言失败问题
#define FMT_UNICODE 0


#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#ifdef HZ_PLATFORM_WINDOWS
#include <Windows.h>
#endif

// 包含Hazel核心头文件，使断言和日志宏在所有文件中可用
#include "Hazel/Log.h"