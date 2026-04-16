# 屏幕共享绘图功能 - 新架构实现

## 架构变更总结

### 旧架构（已移除）
- ScreenShareWidget 中集成了所有画线逻辑
- 鼠标事件直接在 ScreenShareWidget 中处理
- 绘图坐标使用图像坐标系
- 代码混合了UI显示和绘图功能

### 新架构（已实现）
- **ScreenShareWidget**: 只负责显示屏幕内容和管理悬浮按钮
- **TransparentDrawingWindow**: 独立的透明窗口，负责绘图功能
- **分离的关注点**: UI显示和绘图逻辑完全解耦
- **更好的可维护性**: 每个类只有单一职责

## 文件结构

### 新增文件
1. **src/ui/widgets/TransparentDrawingWindow.h**
   - 定义透明绘图窗口类
   - 覆盖整个屏幕
   - 处理所有绘图相关的鼠标事件

2. **src/ui/widgets/TransparentDrawingWindow.cpp**
   - 实现透明窗口逻辑
   - 处理鼠标绘图
   - ESC 键关闭窗口

### 修改的文件
1. **src/ui/widgets/ScreenShareWidget.h**
   - 移除所有画线相关的成员变量
   - 移除鼠标事件处理方法
   - 添加悬浮按钮指针
   - 添加 TransparentDrawingWindow 前向声明

2. **src/ui/widgets/ScreenShareWidget.cpp**
   - 移除所有画线逻辑代码
   - 添加 setupDrawingButton() 方法
   - 添加 onDrawingButtonClicked() 方法
   - 简化 paintEvent（仅显示屏幕内容）

3. **CMakeLists.txt**
   - 添加 TransparentDrawingWindow.h 到 HEADERS
   - 添加 TransparentDrawingWindow.cpp 到 SOURCES

## 功能流程

### 初始化阶段
```
ScreenShareWidget 创建
  ↓
setupUI() → setupDrawingButton()
  ↓
创建悬浮绘图按钮（✏️ 按钮）
  ↓
设置按钮样式（绿色圆形，带蓝色边框）
  ↓
按钮初始时隐藏（m_drawingWindow = nullptr）
```

### 屏幕共享开始
```
setSharing(true)
  ↓
显示悬浮按钮
  ↓
按钮位置：右下角（离边界 20px）
  ↓
按钮样式：
  - 正常：绿色背景，蓝色边框
  - 悬浮：更亮的绿色，更亮的蓝色边框
  - 按下：深绿色，深蓝色边框
```

### 用户点击绘图按钮
```
onDrawingButtonClicked()
  ↓
创建 TransparentDrawingWindow（如果不存在）
  ↓
设置窗口属性：
  - 全屏覆盖
  - 半透明黑色叠加层
  - 十字光标
  - 总是在顶部
```

### 绘图过程
```
鼠标在透明窗口上移动
  ↓
mousePressEvent() → 开始新笔画
  ↓
mouseMoveEvent() → 添加点到当前笔画
  ↓
update() → 重绘透明窗口
  ↓
paintEvent() → 显示实时线条
  ↓
mouseReleaseEvent() → 完成笔画
  ↓
保存笔画到 m_drawings
```

### 关闭绘图窗口
```
按 ESC 键
  ↓
keyPressEvent() 检测 Key_Escape
  ↓
close() → 关闭透明窗口
  ↓
emit drawingFinished() → 通知 ScreenShareWidget
  ↓
m_drawingWindow = nullptr → 清空指针
```

## TransparentDrawingWindow 类详解

### 初始化
```cpp
// 窗口属性
setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
setAttribute(Qt::WA_TranslucentBackground);
setAttribute(Qt::WA_NoSystemBackground);

// 外观
setCursor(Qt::CrossCursor);  // 十字光标

// 覆盖整个屏幕
setGeometry(screen->geometry());
```

### 绘图逻辑
- **半透明背景**: 绘制 10% 不透明度的黑色矩形
- **已完成笔画**: 用 QPen 绘制储存的所有笔画
- **当前笔画**: 实时显示正在绘制的笔画
- **线条风格**: 圆形笔帽、圆形连接点，确保平滑

### 数据结构
```cpp
QVector<QVector<QPoint>> m_drawings;  // 所有已完成的笔画
QVector<QPoint> m_currentStroke;      // 当前笔画的点
QColor m_drawingColor = Qt::red;      // 绘图颜色
int m_drawingLineWidth = 2;           // 线宽
```

## ScreenShareWidget 中的悬浮按钮

### 按钮样式
```css
正常状态:
  background: 径向渐变(绿色中心 #4caf50 → 深绿色边缘 #2e7d32)
  border: 3px solid #64b5f6 (蓝色)
  border-radius: 28px (圆形)

悬浮状态:
  background: 更亮的绿色渐变
  border: 更亮的蓝色

按下状态:
  background: 深绿色渐变
  border: 深蓝色
```

### 按钮位置
- **位置**: 右下角
- **距离**: 离窗口边界 20px
- **大小**: 56x56px
- **可见性**: 只在 m_isSharing = true 时显示

### 按钮功能
- **单击**: 创建并显示透明绘图窗口
- **样式反馈**: 提供视觉反馈（悬浮/按下效果）

## 关键改进

1. **关注点分离**
   - ✅ ScreenShareWidget 专注于显示屏幕
   - ✅ TransparentDrawingWindow 专注于绘图

2. **更好的用户体验**
   - ✅ 悬浮按钮易于访问
   - ✅ 清晰的视觉反馈
   - ✅ 全屏透明绘图区域

3. **代码可维护性**
   - ✅ 代码行数减少
   - ✅ 逻辑更清晰
   - ✅ 易于扩展和测试

4. **性能优化**
   - ✅ 减少不必要的坐标转换
   - ✅ 简化的绘制逻辑
   - ✅ 独立的窗口管理

## 使用方式

### 启用屏幕共享时
```cpp
screenShareWidget->setSharing(true);
// 自动显示悬浮按钮
```

### 禁用屏幕共享时
```cpp
screenShareWidget->setSharing(false);
// 自动隐藏悬浮按钮
// 透明窗口如果打开也会被隐藏
```

### 自定义绘图参数（可选）
```cpp
// 在 TransparentDrawingWindow 中设置
drawingWindow->setDrawingColor(Qt::blue);
drawingWindow->setDrawingLineWidth(5);
```

## 编译和构建

### CMake 配置
已在 CMakeLists.txt 中添加：
```cmake
SOURCES: src/ui/widgets/TransparentDrawingWindow.cpp
HEADERS: src/ui/widgets/TransparentDrawingWindow.h
```

### 编译命令
```bash
cd build/Qt_6_11_0_for_macOS-Debug
cmake --build . --config Debug
```

## 版本信息

- **功能版本**: 2.0 (新架构)
- **实现日期**: 2026-04-15
- **变更类型**: 架构重构
- **兼容性**: 向后兼容（API 不变）

---

**相关类**: ScreenShareWidget, TransparentDrawingWindow  
**相关文件**: src/ui/widgets/ScreenShareWidget.*, src/ui/widgets/TransparentDrawingWindow.*
