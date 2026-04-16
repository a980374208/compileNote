cmake_minimum_required(VERSION 3.16)
project(QMeet VERSION 1.0.0 LANGUAGES CXX OBJC OBJCXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt packages
find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Gui 
    Widgets 
    Network 
    Multimedia 
    MultimediaWidgets
    WebSockets
)

# WebRTC (optional - for real video conferencing)
# find_package(webrtc REQUIRED)

# Project source files
set(SOURCES
    src/main.cpp
    src/app/Application.cpp
    src/app/Settings.cpp
    
    # UI Windows
    src/ui/LoginWindow.cpp
    src/ui/MainWindow.cpp
    src/ui/MeetingRoomWindow.cpp
    src/ui/SettingsDialog.cpp
    
    # Widgets
    src/ui/widgets/VideoWidget.cpp
    src/ui/widgets/ParticipantWidget.cpp
    src/ui/widgets/ChatWidget.cpp
    src/ui/widgets/ControlBar.cpp
    src/ui/widgets/ScreenShareWidget.cpp
    src/ui/widgets/TransparentDrawingWindow.cpp
    src/ui/widgets/TransparentDrawingWindow_macOS.mm
    
    # Delegates
    src/ui/delegates/MeetingListDelegate.cpp
    src/ui/delegates/ParticipantListDelegate.cpp
    
    # Core modules
    src/core/MeetingManager.cpp
    src/core/NetworkManager.cpp
    src/core/MediaManager.cpp
    src/core/ChatManager.cpp
    src/core/ScreenShareManager.cpp
    
    # Models
    src/models/User.cpp
    src/models/Meeting.cpp
    src/models/Message.cpp
    src/models/MeetingListModel.cpp
    src/models/ParticipantListModel.cpp
)

set(HEADERS
    src/app/Application.h
    src/app/Settings.h
    
    # UI Windows
    src/ui/LoginWindow.h
    src/ui/MainWindow.h
    src/ui/MeetingRoomWindow.h
    src/ui/SettingsDialog.h
    
    # Widgets
    src/ui/widgets/VideoWidget.h
    src/ui/widgets/ParticipantWidget.h
    src/ui/widgets/ChatWidget.h
    src/ui/widgets/ControlBar.h
    src/ui/widgets/ScreenShareWidget.h
    src/ui/widgets/TransparentDrawingWindow.h
    
    # Core modules
    src/core/MeetingManager.h
    src/core/NetworkManager.h
    src/core/MediaManager.h
    src/core/ChatManager.h
    src/core/ScreenShareManager.h
    
    # Models
    src/models/User.h
    src/models/Meeting.h
    src/models/Message.h
    src/models/MeetingListModel.h
    src/models/ParticipantListModel.h
    
    # Delegates
    src/ui/delegates/MeetingListDelegate.h
    src/ui/delegates/ParticipantListDelegate.h
)

set(FORMS
    src/ui/LoginWindow.ui
    src/ui/MainWindow.ui
    src/ui/MeetingRoomWindow.ui
    src/ui/SettingsDialog.ui
)

set(RESOURCES
    resources/resources.qrc
)

# Create executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    Qt6::WebSockets
)

# Install rules
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

# macOS specific: link CoreGraphics and AppKit frameworks
if(APPLE)
    find_library(COREGRAPHICS_FRAMEWORK CoreGraphics)
    find_library(APPKIT_FRAMEWORK AppKit)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        ${COREGRAPHICS_FRAMEWORK}
        ${APPKIT_FRAMEWORK}
    )
endif()

