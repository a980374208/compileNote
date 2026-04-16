# 屏幕共享画线功能指南

## 功能概述

ScreenShareWidget 现已支持在屏幕共享上直接使用鼠标进行画线标注。用户可以在共享屏幕上绘制标记、箭头和注释。

## 功能特性

- ✅ **实时画线** - 按下鼠标即可开始画线，松开鼠标完成笔画
- ✅ **平滑渲染** - 使用圆形笔帽和线条连接，确保平滑的线条
- ✅ **自定义颜色** - 支持设置绘制线条的颜色
- ✅ **自定义线宽** - 支持调整绘制线条的粗细
- ✅ **启用/禁用** - 可随时启用或禁用绘制功能
- ✅ **清除绘制** - 一键清除所有绘制内容

## API 使用

### 启用/禁用绘制功能

```cpp
ScreenShareWidget *screenShare = new ScreenShareWidget();

// 启用绘制功能
screenShare->setDrawingEnabled(true);

// 禁用绘制功能
screenShare->setDrawingEnabled(false);

// 检查是否启用
if (screenShare->isDrawingEnabled()) {
    // 绘制功能已启用
}
```

### 设置绘制参数

```cpp
// 设置绘制颜色（默认为红色）
screenShare->setDrawingColor(Qt::red);
screenShare->setDrawingColor(Qt::green);
screenShare->setDrawingColor(Qt::blue);
screenShare->setDrawingColor(QColor("#FF6B6B"));

// 设置线条宽度（默认为2像素）
screenShare->setDrawingLineWidth(1);   // 细线
screenShare->setDrawingLineWidth(2);   // 正常（默认）
screenShare->setDrawingLineWidth(5);   // 粗线
screenShare->setDrawingLineWidth(10);  // 很粗
```

### 清除绘制

```cpp
// 清除所有绘制内容
screenShare->clearDrawings();
```

## 实现细节

### 绘制原理

1. **鼠标按下 (mousePressEvent)**
   - 初始化新的笔画（stroke）
   - 将窗口坐标转换为图像坐标
   - 记录起始点

2. **鼠标移动 (mouseMoveEvent)**
   - 持续记录鼠标位置
   - 将窗口坐标转换为图像坐标
   - 实时重绘窗口

3. **鼠标释放 (mouseReleaseEvent)**
   - 记录最后一个点（转换为图像坐标）
   - 完成当前笔画
   - 将笔画保存到绘制列表
   - 清空临时笔画

### 坐标转换系统

绘制功能使用**双向坐标转换**系统，确保绘制精确对应屏幕上的实际位置：

#### 窗口坐标 → 图像坐标 (windowToImageCoordinates)
```
1. 获取图像在窗口中的显示矩形
2. 检查鼠标点是否在图像区域内
3. 计算相对于图像矩形的位置
4. 根据缩放比例转换为图像坐标
5. 限制坐标在图像边界内
```

#### 图像坐标 → 窗口坐标 (imageToWindowCoordinates)
```
1. 获取图像在窗口中的显示矩形
2. 计算显示尺寸与实际图像尺寸的比例
3. 将图像坐标按比例放大
4. 加上图像矩形的偏移量
```

**优势**：
- ✅ 绘制准确对应屏幕内容
- ✅ 自动适应窗口大小变化
- ✅ 支持任意缩放比例
- ✅ 鼠标超出图像区域时不会误绘

### 数据结构

```cpp
// 所有已完成的笔画集合
QVector<QVector<QPoint>> m_drawings;

// 当前正在绘制的笔画
QVector<QPoint> m_currentStroke;

// 绘制颜色和线宽
QColor m_drawingColor;
int m_drawingLineWidth;
```

### 渲染流程

```
paintEvent()
  ├─ 绘制屏幕共享的图像
  ├─ 绘制所有已完成的笔画
  └─ 绘制当前正在绘制的笔画
```

## 使用场景

### 1. 讲座和演示
在共享屏幕时，实时标注重点内容。

```cpp
screenShare->setDrawingEnabled(true);
screenShare->setDrawingColor(Qt::red);
screenShare->setDrawingLineWidth(3);
```

### 2. 代码审查
在共享代码时，指出需要改进的地方。

```cpp
screenShare->setDrawingColor(QColor("#FF6B6B"));  // 红色表示问题
screenShare->setDrawingColor(QColor("#51CF66"));  // 绿色表示好的代码
```

### 3. 设计评审
在共享设计稿时，提出修改建议。

```cpp
screenShare->setDrawingColor(Qt::blue);
screenShare->setDrawingLineWidth(2);
```

## 完整示例

```cpp
#include "ScreenShareWidget.h"

// 创建屏幕共享组件
ScreenShareWidget *screenShare = new ScreenShareWidget(this);

// 启用绘制功能并设置参数
screenShare->setDrawingEnabled(true);
screenShare->setDrawingColor(Qt::red);
screenShare->setDrawingLineWidth(2);

// 设置屏幕图像
QImage screenImage = captureScreen();
screenShare->setScreenImage(screenImage);

// 用户可以现在在屏幕上绘制

// 后续可以改变颜色
screenShare->setDrawingColor(Qt::green);

// 清除所有绘制
screenShare->clearDrawings();

// 禁用绘制功能
screenShare->setDrawingEnabled(false);
```

## 技术实现

### 关键类和方法

| 方法 | 描述 |
|------|------|
| `setDrawingEnabled(bool)` | 启用或禁用绘制功能 |
| `isDrawingEnabled()` | 检查绘制功能是否启用 |
| `setDrawingColor(const QColor&)` | 设置笔刷颜色 |
| `setDrawingLineWidth(int)` | 设置线条宽度 |
| `clearDrawings()` | 清除所有绘制内容 |
| `mousePressEvent()` | 处理鼠标按下（含坐标转换） |
| `mouseMoveEvent()` | 处理鼠标移动（含坐标转换） |
| `mouseReleaseEvent()` | 处理鼠标释放（含坐标转换） |
| `windowToImageCoordinates(QPoint)` | 将窗口坐标转换为图像坐标 |
| `imageToWindowCoordinates(QPoint)` | 将图像坐标转换为窗口坐标 |

### 性能考虑

- **内存**：绘制数据按笔画存储，每笔画包含多个 QPoint
- **CPU**：绘制在 paintEvent 中完成，使用高效的 QPainter 绘制
- **响应性**：setMouseTracking(true) 确保平滑的鼠标跟踪

## 坐标转换详解

### 问题背景

当屏幕图像缩放显示时（非全尺寸显示），鼠标在窗口上的坐标与图像上的实际坐标不匹配。例如：
- 原始屏幕: 1920×1080 像素
- 显示窗口: 960×540 像素（50% 缩放）
- 鼠标在窗口上点击 (480, 270) 应该对应图像上的 (960, 540)

### 解决方案

#### windowToImageCoordinates 函数

将鼠标在窗口中的坐标转换为在实际图像上的坐标：

```cpp
QPoint windowToImageCoordinates(const QPoint &windowPoint) const
{
    // 1. 获取图像在窗口中的显示区域
    QRect imageRect = calculateImageRect();
    
    // 2. 计算相对位置
    int relativeX = windowPoint.x() - imageRect.left();
    int relativeY = windowPoint.y() - imageRect.top();
    
    // 3. 计算缩放比例
    double scaleX = imageSize.width() / displaySize.width();
    double scaleY = imageSize.height() / displaySize.height();
    
    // 4. 转换到图像坐标
    int imageX = relativeX * scaleX;
    int imageY = relativeY * scaleY;
    
    return QPoint(imageX, imageY);
}
```

#### imageToWindowCoordinates 函数

将图像坐标转换回窗口坐标（用于逆向转换）：

```cpp
QPoint imageToWindowCoordinates(const QPoint &imagePoint) const
{
    // 反向过程：从图像坐标转换为窗口坐标
    QRect imageRect = calculateImageRect();
    double scaleX = displaySize.width() / imageSize.width();
    double scaleY = displaySize.height() / imageSize.height();
    
    int windowX = imagePoint.x() * scaleX + imageRect.left();
    int windowY = imagePoint.y() * scaleY + imageRect.top();
    
    return QPoint(windowX, windowY);
}
```

### 实际例子

假设：
- 原始屏幕分辨率：1920×1080
- 窗口显示尺寸：960×540（50% 缩放）
- 图像在窗口中显示的位置：(100, 50) 到 (1060, 590)

当用户在窗口上点击 (500, 300) 时：

```
相对位置 = (500 - 100, 300 - 50) = (400, 250)
缩放比例 = (1920/960, 1080/540) = (2.0, 2.0)
图像坐标 = (400 * 2.0, 250 * 2.0) = (800, 500)
```

这样，绘制会精确对应原始屏幕的 (800, 500) 位置！

## 扩展功能

### 可选的增强功能

1. **撤销功能**
   ```cpp
   void undo();
   ```

2. **绘制工具选择**
   ```cpp
   enum DrawingTool { Pen, Arrow, Rectangle, Circle };
   void setDrawingTool(DrawingTool tool);
   ```

3. **透明度支持**
   ```cpp
   void setDrawingOpacity(qreal opacity);
   ```

4. **保存绘制**
   ```cpp
   QImage getDrawingImage();
   void saveDrawings(const QString &filename);
   ```

5. **手势识别**
   - 自动识别箭头、形状等

6. **多点触控支持**
   - 在触摸设备上支持笔触输入
   - 压力感应（如果硬件支持）

## 故障排除

### 绘制不显示
- 检查 `setDrawingEnabled(true)` 是否已调用
- 确认屏幕共享图像已设置：`setScreenImage(image)`

### 绘制性能差
- 减少 `setDrawingLineWidth()` 的值
- 减少单次绘制的点数
- 优化 paintEvent 的性能

### 鼠标移动不平滑
- 确认 `setMouseTracking(true)` 在 setupUI 中已调用
- 检查系统鼠标跟踪设置

## 版本信息

- **功能版本**: 1.0
- **添加日期**: 2026-04-15
- **支持平台**: Windows, macOS, Linux

---

**相关类**: ScreenShareWidget  
**相关文件**: `src/ui/widgets/ScreenShareWidget.h`, `src/ui/widgets/ScreenShareWidget.cpp`
