# QMeet 项目文档

## 📚 文档组织结构

本文件夹包含 QMeet 项目的所有帮助文档和参考资料。

## 📖 文档分类

### 🎨 屏幕共享绘图功能

#### 用户文档
- **DRAWING_FEATURE_README.md** - 功能总体说明
- **DRAWING_FEATURE_QUICKSTART.md** - 快速入门指南
- **SCREEN_DRAWING_USAGE_GUIDE.md** - 详细使用教程

#### 开发者文档
- **DRAWING_DEVELOPER_GUIDE.md** - 实现技术细节
- **DRAWING_WINDOW_IMPLEMENTATION.md** - 架构和类设计

#### 快速参考
- **DRAWING_FEATURE_INDEX.md** - 文档导航索引
- **DRAWING_QUICK_REFERENCE.md** - 快速参考卡

#### 特性文档
- **DRAWING_FEATURE_GUIDE.md** - 绘图功能指南
- **DRAWING_WINDOW_IMPLEMENTATION.md** - 窗口实现

### 🖱️ 鼠标焦点穿透功能

- **MOUSE_PASSTHROUGH_IMPLEMENTATION.md** - 完整的技术实现说明
- **MOUSE_PASSTHROUGH_SUMMARY.md** - 快速参考说明

### 🎨 样式表整合

- **STYLESHEET_GUIDE.md** - 样式表完整指南
- **STYLESHEET_COMPLETION_REPORT.md** - 完成状态报告
- **STYLESHEET_QUICK_REFERENCE.md** - 快速参考
- **STYLE_MIGRATION_SUMMARY.md** - 迁移总结

### 📋 项目总结

- **IMPLEMENTATION_SUMMARY.txt** - 总体完成总结
- **CHANGELOG_DRAWING_FEATURE.md** - 绘图功能变更记录

## 🎯 快速导航

### 👤 普通用户
推荐阅读顺序：
1. `DRAWING_FEATURE_QUICKSTART.md` - 快速了解（5分钟）
2. `SCREEN_DRAWING_USAGE_GUIDE.md` - 详细使用（15分钟）
3. `DRAWING_QUICK_REFERENCE.md` - 快速查询

### 👨‍💻 开发人员
推荐阅读顺序：
1. `DRAWING_FEATURE_QUICKSTART.md` - 快速入门
2. `DRAWING_DEVELOPER_GUIDE.md` - 实现细节（重点）
3. `DRAWING_WINDOW_IMPLEMENTATION.md` - 架构设计
4. `DRAWING_QUICK_REFERENCE.md` - 开发参考

### 🔧 维护人员
推荐阅读顺序：
1. `IMPLEMENTATION_SUMMARY.txt` - 完成总结
2. `CHANGELOG_DRAWING_FEATURE.md` - 变更记录
3. `DRAWING_DEVELOPER_GUIDE.md` - 实现细节
4. 所有其他相关文档

## 📊 文档统计

| 分类 | 文件数 | 总计 |
|------|--------|------|
| 屏幕共享绘图 | 9 | |
| 鼠标穿透功能 | 2 | |
| 样式表整合 | 4 | |
| 项目总结 | 2 | |
| **合计** | **16** | **~160KB** |

## 📑 文件清单

```
doc/
├── README.md (本文件)
│
├── 屏幕共享绘图功能/
│   ├── DRAWING_FEATURE_README.md
│   ├── DRAWING_FEATURE_QUICKSTART.md
│   ├── DRAWING_FEATURE_INDEX.md
│   ├── DRAWING_FEATURE_GUIDE.md
│   ├── SCREEN_DRAWING_USAGE_GUIDE.md
│   ├── DRAWING_DEVELOPER_GUIDE.md
│   ├── DRAWING_WINDOW_IMPLEMENTATION.md
│   ├── DRAWING_QUICK_REFERENCE.md
│   └── CHANGELOG_DRAWING_FEATURE.md
│
├── 鼠标穿透功能/
│   ├── MOUSE_PASSTHROUGH_IMPLEMENTATION.md
│   └── MOUSE_PASSTHROUGH_SUMMARY.md
│
├── 样式表整合/
│   ├── STYLESHEET_GUIDE.md
│   ├── STYLESHEET_COMPLETION_REPORT.md
│   ├── STYLESHEET_QUICK_REFERENCE.md
│   └── STYLE_MIGRATION_SUMMARY.md
│
└── 项目总结/
    └── IMPLEMENTATION_SUMMARY.txt
```

## 🔗 关键文档链接

### 入门必读
- 快速开始：[DRAWING_FEATURE_QUICKSTART.md](./DRAWING_FEATURE_QUICKSTART.md)
- 功能说明：[DRAWING_FEATURE_README.md](./DRAWING_FEATURE_README.md)
- 文档索引：[DRAWING_FEATURE_INDEX.md](./DRAWING_FEATURE_INDEX.md)

### 用户文档
- 使用指南：[SCREEN_DRAWING_USAGE_GUIDE.md](./SCREEN_DRAWING_USAGE_GUIDE.md)
- 快速参考：[DRAWING_QUICK_REFERENCE.md](./DRAWING_QUICK_REFERENCE.md)

### 技术文档
- 开发指南：[DRAWING_DEVELOPER_GUIDE.md](./DRAWING_DEVELOPER_GUIDE.md)
- 架构设计：[DRAWING_WINDOW_IMPLEMENTATION.md](./DRAWING_WINDOW_IMPLEMENTATION.md)
- 穿透实现：[MOUSE_PASSTHROUGH_IMPLEMENTATION.md](./MOUSE_PASSTHROUGH_IMPLEMENTATION.md)

### 参考资料
- 项目总结：[IMPLEMENTATION_SUMMARY.txt](./IMPLEMENTATION_SUMMARY.txt)
- 变更记录：[CHANGELOG_DRAWING_FEATURE.md](./CHANGELOG_DRAWING_FEATURE.md)
- 样式指南：[STYLESHEET_GUIDE.md](./STYLESHEET_GUIDE.md)

## 💡 文档使用技巧

### 搜索文档
在任何文档中使用浏览器的查找功能（Ctrl+F 或 Cmd+F）快速搜索关键词。

### 导航
每个文档的开头都有目录，可以快速跳转到需要的部分。

### 代码示例
开发相关文档中包含大量代码示例，可以直接参考使用。

## 📚 文档版本

| 版本 | 更新日期 | 说明 |
|------|---------|------|
| 2.3 | 2026-04-15 | 鼠标穿透代码注释 |
| 2.2 | 2026-04-15 | 窗口置顶功能 |
| 2.1 | 2026-04-15 | 鼠标穿透功能 |
| 2.0 | 2026-04-15 | 屏幕共享绘图功能 |
| 1.0 | 2026-04-15 | 样式表整合 |

## ❓ 常见问题

### 我应该从哪里开始？
→ 从 `DRAWING_FEATURE_QUICKSTART.md` 开始，然后根据你的角色选择相应的文档。

### 如何快速查找信息？
→ 使用 `DRAWING_FEATURE_INDEX.md` 或各个文档中的目录。

### 代码在哪里？
→ 源代码在 `src/` 文件夹中，文档中包含相关的代码片段和引用。

### 如何更新文档？
→ 文档都是 Markdown 格式，可以用任何文本编辑器编辑。

## 📞 支持

- 问题报告：提交 Issue
- 功能建议：Discussion
- 文档改进：Pull Request

---

**维护人员**：QMeet 开发团队  
**最后更新**：2026-04-15  
**文档质量**：✅ 完善  
**编码**：UTF-8
