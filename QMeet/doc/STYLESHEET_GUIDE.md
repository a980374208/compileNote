# QMeet Stylesheet Guide

## Overview
All styling for the QMeet application has been consolidated into a single stylesheet file: `resources/styles/main.qss`. This guide explains the organization and how to use object names for styling.

## Stylesheet File Location
- **Main Stylesheet**: `resources/styles/main.qss`
- **Resource Reference**: Loaded via `resources/resources.qrc`

## How Styling Works

### Loading the Stylesheet
The stylesheet is loaded in `Application::loadStylesheet()` which reads `:/styles/main.qss` and applies it to the entire application.

### Using Object Names for Styling
Each UI component uses `setObjectName()` to assign a unique identifier, which is then matched with CSS selectors in the stylesheet.

Example:
```cpp
// In C++ code:
QPushButton *btn = new QPushButton("Click me");
btn->setObjectName("customButton");

// In QSS file:
#customButton {
    background-color: #0078d4;
    color: white;
    padding: 8px 16px;
}
```

## Stylesheet Organization

The stylesheet is organized into the following sections:

### 1. Global Styles
- Default widget properties (font family, size)
- Main window background

### 2. Button Styles
- Primary buttons (default blue)
- Secondary buttons (white with border)
- Danger buttons (red for destructive actions)
- Control buttons (circular)

### 3. Input Widgets
- QLineEdit styling
- QTextEdit styling
- QComboBox styling

### 4. Scroll Bar Styles
- Vertical scroll bar styling
- Handle and hover states

### 5. List View and List Widget Styles
- Item padding and selection
- Hover and selected states

### 6. Menu Styles
- Menu background and border
- Menu item styling and selection

### 7. Label Styles
- Standard labels
- Special label properties (heading, title, subtle)
- Status indicators (online/offline)
- Message bubbles

### 8. Component-Specific Styles

#### MainWindow
- Sidebar styling (`#mainSidebar`)
- Avatar label (`#avatarLabel`)
- Welcome label (`#welcomeLabel`)
- Join label (`#joinLabel`)
- Meeting code edit (`#meetingCodeEdit`)
- Quick join button (`#quickJoinBtn`)
- Meeting list label (`#meetingListLabel`)

#### MeetingRoomWindow
- Main window background (`MeetingRoomWindow`)
- Header styling (`#meetingHeader`)
- Meeting title label (`#meetingTitleLabel`)
- Meeting number label (`#meetingNumberLabel`)
- Duration label (`#durationLabel`)
- More button (`#moreBtn`)
- Video area (`#videoArea`)
- Side panel (`#sidePanel`)

#### ControlBar
- Main control bar background (`ControlBar`)
- Leave button (`#leaveBtn`)
- Control buttons (`#controlBtn`)
- Divider line (`#divider`)

#### ChatWidget
- Main chat area (`ChatWidget`)
- Chat header (`#chatHeader`)
- Chat title label (`#chatTitleLabel`)
- Message scroll area (`#messageScrollArea`)
- Emoji button (`#emojiBtn`)
- Input edit (`#inputEdit`)
- Send button (`#sendBtn`)
- Input area (`#chatInputArea`)

#### ParticipantWidget
- Main participant area (`ParticipantWidget`)
- Participant header (`#participantHeader`)
- Participant count label (`#participantCountLabel`)
- Participant list view (`#participantListView`)

#### VideoWidget & ScreenShareWidget
- Video widget background
- Screen share widget background

## Color Palette

### Primary Colors
- **Primary Blue**: `#0078d4` - Main action color
- **Primary Blue Hover**: `#106ebe` - Hover state
- **Primary Blue Pressed**: `#005a9e` - Pressed state

### Secondary Colors
- **Danger Red**: `#d13438` - Destructive actions
- **Danger Red Hover**: `#b22b2e` - Hover state
- **Success Green**: `#107c10` - Success indicators

### Neutral Colors
- **Light Background**: `#f5f5f5` - Main window background
- **Lighter Background**: `#f0f0f0` - Hover states
- **White**: `#ffffff` - Primary element background
- **Dark Background**: `#2d2d2d` - Meeting room background
- **Darker Background**: `#1a1a1a` - Alternative dark background
- **Darkest Background**: `#0a0a0a` - Video area background
- **Dark Hover**: `#3d3d3d` - Element hover states
- **Dark Pressed**: `#4d4d4d` - Element pressed states
- **Dark Active**: `#5d5d5d` - Element active state

### Text Colors
- **Dark Text**: `#333333` - Main text color
- **Black Text**: `#000000` - Sidebar text
- **Subtle Text**: `#666666` - Secondary text
- **Light Text**: `#999999` - Tertiary text
- **White Text**: `#ffffff` - Text on dark backgrounds

### Border Colors
- **Light Border**: `#d1d1d1` - Default border
- **Lighter Border**: `#e0e0e0` - Subtle border
- **Medium Border**: `#c7c7c7` - Focus border
- **Dark Border**: `#444444` - Dark background border

## Adding New Styles

### Steps to Add a New Component Style

1. **Assign Object Name in C++**:
   ```cpp
   MyWidget *widget = new MyWidget();
   widget->setObjectName("myCustomWidget");
   ```

2. **Add CSS Rules to Stylesheet**:
   ```css
   #myCustomWidget {
       background-color: #ffffff;
       border: 1px solid #d1d1d1;
       border-radius: 4px;
       padding: 8px;
   }
   ```

3. **Test the Styling**:
   - Rebuild the project
   - Verify the styling is applied correctly

## Best Practices

1. **Use Object Names**: Always use `setObjectName()` for all styled elements
2. **Remove setStyleSheet()**: Replace inline `setStyleSheet()` calls with object names
3. **Keep Hierarchy**: Organize related styles together in the stylesheet
4. **Use Color Variables**: Refer to the color palette reference at the end of the stylesheet
5. **Maintain Consistency**: Follow existing naming conventions for object names
6. **Document Changes**: Update this guide when adding new component styles

## Troubleshooting

### Styles Not Applied
- **Problem**: Changes to stylesheet are not visible
- **Solution**: Rebuild the project with `cmake --build . --config Debug`

### Selector Not Matching
- **Problem**: CSS rules not working for an element
- **Solution**: Verify the object name matches exactly (case-sensitive)

### Inheritance Issues
- **Problem**: Child widgets not inheriting parent styles
- **Solution**: Explicitly style child widgets or use selector hierarchy (e.g., `ChatWidget QLabel`)

## Related Files
- `resources/styles/main.qss` - Main stylesheet
- `resources/resources.qrc` - Resource file reference
- `src/app/Application.cpp` - Stylesheet loading code
