# ImGui键盘API冲突问题修复文档

## 错误描述
在ImGuiLayer的使用过程中，点击界面时出现以下断言失败错误：

```
IM_ASSERT(KeyMap[n] == -1 && "Backend needs to either only use io.AddKeyEvent(), either only fill legacy io.KeysDown[] + io.KeyMap[]. Not both!")
```

该错误发生在ImGuiIO::AddKeyAnalogEvent()函数中，表明ImGui检测到了新旧两种键盘输入API的混合使用。

## 问题分析
通过分析代码，发现了以下问题：

1. **旧键盘API的使用**：在ImGuiLayer.cpp的OnAttach方法中，我们设置了KeyMap数组将ImGuiKey映射到GLFW键盘码：
   ```cpp
   // imgui输入key对应glfw的key
   io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
   io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
   // ...其他键映射
   ```
   并且在事件处理函数中使用了KeysDown数组：
   ```cpp
   bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
   {
       ImGuiIO& io = ImGui::GetIO();
       io.KeysDown[e.GetKeyCode()] = true;
       // ...
   }
   ```

2. **新键盘API的使用**：同时，在ImGui_ImplGlfw_InitForOpenGL初始化时，第二个参数为true，这会安装GLFW的事件回调：
   ```cpp
   ImGui_ImplGlfw_InitForOpenGL(window, true);
   ```
   这些回调会使用新的键盘API（io.AddKeyEvent()）处理键盘事件，如ImGui_ImplGlfw_KeyCallback函数所示：
   ```cpp
   io.AddKeyEvent(imgui_key, (action == GLFW_PRESS));
   io.SetKeyEventNativeData(imgui_key, keycode, scancode);
   ```

3. **冲突导致断言失败**：ImGui明确要求后端只能使用一种键盘输入API，不能同时使用新旧两种，因此触发了断言失败。

## 解决方案
修改ImGuiLayer.cpp文件，将ImGui_ImplGlfw_InitForOpenGL的第二个参数从true改为false，禁用GLFW的事件回调安装，这样就只会使用我们在ImGuiLayer中实现的键盘事件处理（旧API）。

## 修改的代码位置
文件路径：d:\CDev2\gitclient\miniGame\src\Hazel\ImGui\ImGuiLayer.cpp

修改内容：
```cpp
// 原代码
ImGui_ImplGlfw_InitForOpenGL(window, true);

// 修改后代码
// 设置install_callbacks为false，避免GLFW后端使用新的键盘API
// 我们在ImGuiLayer的事件处理函数中使用旧的键盘API（KeyMap和KeysDown）
ImGui_ImplGlfw_InitForOpenGL(window, false);
```

## 修复效果
通过禁用GLFW的事件回调，我们避免了新旧键盘API的混合使用，解决了断言失败问题，使ImGui能够正常接收和处理键盘事件。

## 修改日期
2024-01-28