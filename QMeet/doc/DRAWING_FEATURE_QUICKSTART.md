# 屏幕共享画线功能 - 快速起步

## 🎯 5 分钟快速入门

### 对于用户

#### 第一步：开始屏幕共享
```
1. 打开 QMeet 应用
2. 点击"共享屏幕"按钮
3. 选择要共享的屏幕
```

#### 第二步：找到绘图按钮
```
屏幕显示后，在右下角看到 ✏️ 笔按钮
- 绿色背景
- 蓝色边框
- 圆形设计
```

#### 第三步：开始绘图
```
1. 点击 ✏️ 按钮
2. 屏幕变暗，出现十字光标
3. 按住鼠标左键绘画
4. 松开完成一笔
5. 继续绘制更多笔画
```

#### 第四步：关闭绘图
```
按 ESC 键 → 返回屏幕共享显示
```

### 对于开发者

#### 编译项目
```bash
cd /Users/jiangbinbin/test/QMeet/build/Qt_6_11_0_for_macOS-Debug
cmake --build . --config Debug
```

#### 验证编译
```bash
# 应该看到
[100%] Built target QMeet
```

#### 运行应用
```bash
./QMeet
```

#### 测试功能
```
1. 启动屏幕共享
2. 点击绘图按钮
3. 在屏幕上绘制几条线
4. 按 ESC 关闭
5. 验证返回屏幕共享显示
```

---

## 🔍 核心文件

| 文件 | 作用 | 行数 |
|------|------|------|
| `TransparentDrawingWindow.h` | 绘图窗口类声明 | 48 |
| `TransparentDrawingWindow.cpp` | 绘图窗口实现 | 115 |
| `ScreenShareWidget.h` | 屏幕显示类（修改） | 54 |
| `ScreenShareWidget.cpp` | 屏幕显示实现（修改） | 188 |

---

## 📝 关键代码片段

### 启用绘图按钮
```cpp
// 当开始屏幕共享时
m_screenShareWidget->setSharing(true);
// → 自动显示绘图按钮
```

### 处理绘图完成
```cpp
// TransparentDrawingWindow 在关闭时发出信号
connect(m_drawingWindow, &TransparentDrawingWindow::drawingFinished,
        this, [this]() {
            m_drawingWindow = nullptr;
        });
```

### 配置绘图参数
```cpp
// 可选：自定义绘图参数
drawingWindow->setDrawingColor(Qt::blue);      // 改变颜色
drawingWindow->setDrawingLineWidth(4);         // 改变线宽
```

---

## 🎨 按钮样式

### 正常状态
```
绿色背景（中心 #4caf50 → 边缘 #2e7d32）
蓝色边框（#64b5f6）
```

### 悬浮状态
```
更亮的绿色（中心 #66bb6a → 边缘 #388e3c）
更亮的蓝色边框（#42a5f5）
```

### 按下状态
```
深绿色（中心 #43a047 → 边缘 #1b5e20）
深蓝色边框（#1e88e5）
```

---

## 🔧 常用操作

### 清空所有绘图
```cpp
drawingWindow->clearDrawings();
```

### 禁用绘图（只查看）
```cpp
drawingWindow->enableDrawing(false);
```

### 改变笔颜色
```cpp
drawingWindow->setDrawingColor(Qt::red);    // 默认值
drawingWindow->setDrawingColor(Qt::blue);
drawingWindow->setDrawingColor(Qt::green);
```

### 改变笔宽度
```cpp
drawingWindow->setDrawingLineWidth(1);      // 细
drawingWindow->setDrawingLineWidth(2);      // 默认
drawingWindow->setDrawingLineWidth(5);      // 粗
```

---

## 📚 完整文档

### 用户文档
- 📖 [SCREEN_DRAWING_USAGE_GUIDE.md](./SCREEN_DRAWING_USAGE_GUIDE.md)
- 📖 [DRAWING_QUICK_REFERENCE.md](./DRAWING_QUICK_REFERENCE.md)

### 技术文档
- 📖 [DRAWING_DEVELOPER_GUIDE.md](./DRAWING_DEVELOPER_GUIDE.md)
- 📖 [DRAWING_WINDOW_IMPLEMENTATION.md](./DRAWING_WINDOW_IMPLEMENTATION.md)

### 变更和总结
- 📖 [CHANGELOG_DRAWING_FEATURE.md](./CHANGELOG_DRAWING_FEATURE.md)
- 📖 [IMPLEMENTATION_SUMMARY.txt](./IMPLEMENTATION_SUMMARY.txt)

---

## 🐛 故障排除

### 问题：按钮不显示

**原因**：未启用屏幕共享

**解决**：
```cpp
// 确保调用了
screenShareWidget->setSharing(true);
```

### 问题：绘图点不显示

**原因**：窗口焦点问题

**解决**：
```cpp
// 确保窗口获得焦点
drawingWindow->setFocus();
```

### 问题：无法关闭绘图窗口

**原因**：ESC 键未被正确捕获

**解决**：确保透明窗口在前台，再按 ESC

---

## ✅ 验证清单

安装后验证：
- [ ] 应用编译成功（无错误）
- [ ] 应用启动正常
- [ ] 屏幕共享可以启动
- [ ] 绘图按钮出现（右下角）
- [ ] 点击按钮后屏幕变暗
- [ ] 能在屏幕上绘制
- [ ] 按 ESC 关闭窗口
- [ ] 返回屏幕共享显示

---

## 🚀 下一步

### 立即可做
1. 编译项目 ✅
2. 测试功能 ✅
3. 分享给用户 ✅

### 可选改进
1. 添加撤销功能
2. 添加颜色选择器
3. 添加线宽调整
4. 实现绘图保存

### 获取反馈
1. 收集用户意见
2. 根据反馈改进
3. 计划下个版本

---

## 💬 常见问题

**Q: 绘图会保存吗？**  
A: 不会，关闭窗口后绘图消失。未来版本会支持保存功能。

**Q: 可以改变笔颜色吗？**  
A: 可以，通过代码修改。UI 界面的颜色选择器在开发中。

**Q: 支持多屏幕吗？**  
A: 当前支持主屏幕。多屏幕支持在计划中。

**Q: 可以撤销吗？**  
A: 暂不支持。撤销功能在下个版本会实现。

---

## 📞 获得帮助

- 📖 查看文档：`DRAWING_DEVELOPER_GUIDE.md`
- 💻 查看代码：`src/ui/widgets/TransparentDrawingWindow.*`
- 🐛 报告问题：创建 Issue

---

## 版本信息

- **版本**：2.0（稳定）
- **发布**：2026-04-15
- **状态**：✅ 生产就绪
- **测试**：✅ 通过

---

**祝你使用愉快！🎉**

如有任何问题或建议，欢迎反馈。

---

最后更新：2026-04-15
