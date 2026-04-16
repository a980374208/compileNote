# QMeet Stylesheet Quick Reference

## Quick Index

### Main UI Components Object Names

| Component | Object Name | File |
|-----------|-------------|------|
| Main Window | `MainWindow` | MainWindow.cpp |
| Meeting Room | `MeetingRoomWindow` | MeetingRoomWindow.cpp |
| Chat Widget | `ChatWidget` | ChatWidget.cpp |
| Control Bar | `ControlBar` | ControlBar.cpp |
| Participant Widget | `ParticipantWidget` | ParticipantWidget.cpp |

### MainWindow Child Components

```
MainWindow
├── mainSidebar (left sidebar)
│   ├── avatarLabel (user avatar)
│   └── (sidebar buttons use default styling)
├── welcomeLabel (main heading)
├── joinLabel (meeting join text)
├── meetingCodeEdit (meeting code input)
├── quickJoinBtn (join button)
├── meetingListLabel (meeting list heading)
└── (list items styled by delegate)
```

### MeetingRoomWindow Child Components

```
MeetingRoomWindow
├── meetingHeader (top header)
│   ├── meetingTitleLabel (meeting title)
│   ├── meetingNumberLabel (meeting number)
│   ├── durationLabel (elapsed time)
│   └── moreBtn (options menu)
├── videoArea (video display area)
└── sidePanel (side panel)
    ├── ChatWidget (see below)
    └── ParticipantWidget (see below)
```

### ChatWidget Child Components

```
ChatWidget
├── chatHeader (header section)
│   └── chatTitleLabel (chat title "💬 聊天")
├── messageScrollArea (message area)
│   └── (message items)
└── chatInputArea (input section)
    ├── emojiBtn (emoji button)
    ├── inputEdit (message input field)
    └── sendBtn (send button)
```

### ControlBar Child Components

```
ControlBar
├── controlBtn (microphone button)
├── controlBtn (camera button)
├── controlBtn (screen share button)
├── controlBtn (chat button)
├── controlBtn (participants button)
├── divider (vertical separator)
└── leaveBtn (leave meeting button)
```

### ParticipantWidget Child Components

```
ParticipantWidget
├── participantHeader (header section)
│   └── participantCountLabel (count display)
└── participantListView (participant list)
    └── (participant items - styled by delegate)
```

## Common CSS Properties Quick Reference

### Colors
```css
Primary Blue: #0078d4
Dark Background: #2d2d2d
Light Background: #f5f5f5
Light Border: #d1d1d1
Text Color: #333333
```

### Standard Padding/Margin
```css
padding: 8px;           /* Standard inner spacing */
padding: 12px 16px;     /* Vertical and horizontal */
margin: 4px;            /* Small spacing */
```

### Standard Border Radius
```css
border-radius: 4px;     /* Sharp corners */
border-radius: 8px;     /* Rounded corners */
border-radius: 16px;    /* Very rounded */
border-radius: 28px;    /* Circle (with fixed size) */
```

### Standard Font Sizes
```css
font-size: 12px;        /* Small text */
font-size: 14px;        /* Regular text (default) */
font-size: 16px;        /* Large text */
font-size: 18px;        /* Title */
font-size: 28px;        /* Main heading */
```

## Example: Adding Styling to a New Button

### Step 1: In C++ Code
```cpp
QPushButton *myButton = new QPushButton("My Button");
myButton->setObjectName("myCustomButton");
layout->addWidget(myButton);
```

### Step 2: In `main.qss`
```css
#myCustomButton {
    background-color: #0078d4;
    color: white;
    border: none;
    border-radius: 8px;
    padding: 8px 16px;
    font-size: 14px;
    font-weight: bold;
}

#myCustomButton:hover {
    background-color: #106ebe;
}

#myCustomButton:pressed {
    background-color: #005a9e;
}
```

### Step 3: Rebuild
```bash
cd build/Qt_6_11_0_for_macOS-Debug
cmake --build . --config Debug
```

## Button Style Classes (Pseudo-States)

### Hover State
```css
#buttonName:hover {
    background-color: #106ebe;
}
```

### Pressed State
```css
#buttonName:pressed {
    background-color: #005a9e;
}
```

### Disabled State
```css
#buttonName:disabled {
    background-color: #d1d1d1;
    color: #999999;
}
```

### Focus State (for inputs)
```css
#inputName:focus {
    border: 2px solid #0078d4;
}
```

## Common Widget Selectors

### All widgets of a type
```css
QPushButton {
    background-color: #0078d4;
    color: white;
}
```

### Specific widget by ID
```css
#myWidget {
    background-color: white;
}
```

### Child widgets (descendants)
```css
ChatWidget QLabel {
    color: white;
}
```

### Direct children (one level)
```css
MainWindow > QWidget {
    background-color: #f5f5f5;
}
```

## Troubleshooting

### Styles Not Applied
```bash
# Make sure to rebuild the project
cd build/Qt_6_11_0_for_macOS-Debug
cmake --build . --config Debug
```

### Check Object Name
```cpp
// Verify object name is set before styling
widget->setObjectName("correctName");  // ✅ Correct
widget->setObjectName("CorrectName");  // ❌ Different! (case-sensitive)
```

### Check QSS Syntax
- Use `#` for ID selectors (object names)
- Use spaces between selectors and braces
- Use `:` for pseudo-states (`:hover`, `:pressed`, etc.)
- Use `::` for sub-elements (`::handle`, `::-arrow`, etc.)

## Color Palette

| Purpose | Color | Hex |
|---------|-------|-----|
| Primary Action | Blue | `#0078d4` |
| Hover State | Darker Blue | `#106ebe` |
| Pressed State | Dark Blue | `#005a9e` |
| Danger Action | Red | `#d13438` |
| Success | Green | `#107c10` |
| Light Background | Light Gray | `#f5f5f5` |
| Dark Background | Dark Gray | `#2d2d2d` |
| Border | Medium Gray | `#d1d1d1` |
| Text | Dark Gray | `#333333` |
| Light Text | Gray | `#999999` |

## Useful Links

- Full Guide: `STYLESHEET_GUIDE.md`
- Migration Summary: `STYLE_MIGRATION_SUMMARY.md`
- Main Stylesheet: `resources/styles/main.qss`

---

**Last Updated**: April 2026
**Status**: Complete ✅
