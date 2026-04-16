#include "app/Application.h"

/**
 * @brief QMeet - Qt视频会议应用程序
 * 
 * 一个仿照腾讯会议的视频会议应用程序，使用Qt和C++开发
 * 支持音视频通话、屏幕共享、即时聊天等功能
 * 
 * @author QMeet Team
 * @version 1.0.0
 */
int main(int argc, char *argv[])
{
    // Create application instance
    Application app(argc, argv);

    // Initialize the application
    if (!app.initialize()) {
        return -1;
    }

    // Run the event loop
    return app.exec();
}
