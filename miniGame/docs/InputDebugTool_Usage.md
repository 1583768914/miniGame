# 输入系统调试工具使用指南

## 概述

本指南介绍如何使用新添加的调试层（DebugLayer）来验证WindowsInput类的功能是否正确实现。通过这个调试工具，您可以实时查看和测试键盘和鼠标输入状态。

## 功能特点

调试层提供以下功能：

1. **实时显示鼠标位置**：显示当前鼠标在窗口中的X、Y坐标
2. **常用按键状态监控**：显示W、A、S、D、空格键、Shift、Ctrl等常用按键的按下/释放状态
3. **鼠标按钮状态监控**：显示左键、右键、中键等鼠标按钮的状态
4. **按键测试功能**：提供按钮测试特定键的状态

## 如何使用

### 启动应用程序

运行Sandbox应用程序后，您会看到主窗口中显示ImGui调试界面，其中包含"Input Debug"窗口。

### 验证鼠标输入功能

1. 在窗口中移动鼠标，观察"Mouse Position"数值是否实时更新
2. 按下鼠标左键、右键或中键，观察"Mouse Buttons"部分对应的按钮状态是否变为"Pressed"
3. 释放鼠标按钮，确认状态回到"Released"

### 验证键盘输入功能

1. 按下键盘上的W、A、S、D、空格等键，观察"Common Keys Status"部分对应的键状态是否变为"Pressed"
2. 释放按键，确认状态回到"Released"
3. 点击"Test IsKeyPressed('T')"按钮，会弹出一个对话框显示当前'T'键的状态

## 技术实现说明

### DebugLayer类

调试层通过以下方式实现输入监控：

1. 在`OnUpdate`方法中调用`Input`类的静态方法来获取当前输入状态
2. 在`OnImGuiRender`方法中使用ImGui创建调试界面，显示输入状态
3. 使用ImGui的弹窗功能提供交互测试

### 核心验证代码

调试层中验证WindowsInput功能的核心代码如下：

```cpp
// 更新键盘状态
for (int i = 0; i < 512; i++)
{
    m_KeyPressed[i] = Input::IsKeyPressed(i);
}

// 更新鼠标按钮状态
for (int i = 0; i < 8; i++)
{
    m_MouseButtonPressed[i] = Input::IsMouseButtonPressed(i);
}

// 更新鼠标位置
auto [x, y] = Input::GetMousePosition();
m_MouseX = x;
m_MouseY = y;
```

## 扩展建议

如果需要验证更多的输入功能，可以考虑扩展DebugLayer类，添加：

1. 更全面的键盘键位监控
2. 鼠标滚轮输入监控
3. 自定义按键测试功能
4. 输入事件日志记录

## 故障排除

如果调试界面没有显示或输入状态不正确：

1. 确认ImGuiLayer已正确添加到应用程序中
2. 检查WindowsInput类的实现是否正确
3. 验证Input基类的接口定义是否与WindowsInput的实现匹配

## 版本信息

- 创建日期：2024年4月
- 适用版本：miniGame引擎