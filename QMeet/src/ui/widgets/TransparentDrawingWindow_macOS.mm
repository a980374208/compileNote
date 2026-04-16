#include <AppKit/AppKit.h>
#include "TransparentDrawingWindow.h"

// Helper function to enable/disable mouse event passthrough on macOS
extern "C" void setupMouseEventPassthrough(void *winId, bool enable)
{
    // winId is actually a QNSView pointer, not NSWindow
    NSView *view = reinterpret_cast<NSView *>(winId);
    if (!view) return;
    
    NSWindow *nsWindow = [view window];
    if (!nsWindow) return;
    
    if (enable) {
        // When passthrough is enabled (焦点穿透):
        // 1. Tell NSWindow to ignore mouse events completely
        [nsWindow setIgnoresMouseEvents:YES];
        
        // 2. Disable mouse tracking
        [nsWindow setAcceptsMouseMovedEvents:NO];
        
        // 3. Use a very high window level to stay on top
        // Using NSStatusWindowLevel + 100 instead of NSScreenSaverWindowLevel
        // to avoid interfering with system UI and other windows
        [nsWindow setLevel:NSStatusWindowLevel + 100];
        
        // 4. Make sure window is visible but doesn't interfere
        view.hidden = NO;
    } else {
        // When passthrough is disabled (drawing mode - 绘图模式):
        // Restore normal event handling
        [nsWindow setIgnoresMouseEvents:NO];
        [nsWindow setAcceptsMouseMovedEvents:YES];
        
        // Keep the window at a high level for drawing
        [nsWindow setLevel:NSStatusWindowLevel + 50];
        
        view.hidden = NO;
    }
}

// Helper function to check if window is in front
extern "C" bool isWindowInFront(void *winId)
{
    NSView *view = reinterpret_cast<NSView *>(winId);
    if (!view) return false;
    
    NSWindow *nsWindow = [view window];
    if (!nsWindow) return false;
    
    // Get all visible windows in z-order (front to back)
    NSArray *windows = [NSApp orderedWindows];
    if ([windows count] == 0) return false;
    
    // Check if our window is the first visible non-minimized window
    for (NSWindow *window in windows) {
        // Skip minimized windows
        if ([window isMiniaturized]) continue;
        
        // Skip invisible or hidden windows
        if (![window isVisible]) continue;
        
        // If this is our window, we're in front
        if (window == nsWindow) return true;
        
        // If this is another window and it's visible, we're not in front
        // Only check windows that are actually opaque or significantly visible
        if ([window alphaValue] > 0.1) {
            return false;
        }
    }
    
    return true;
}

// Helper function to bring the window to front only if needed
extern "C" void bringWindowToFrontIfNeeded(void *winId)
{
    // Only bring to front if not already in front to avoid flashing
    if (!isWindowInFront(winId)) {
        NSView *view = reinterpret_cast<NSView *>(winId);
        if (!view) return;
        
        NSWindow *nsWindow = [view window];
        if (!nsWindow) return;
        
        // Bring window to front WITHOUT activating the application
        // This prevents interfering with other window's focus and input
        [nsWindow orderFront:nil];
        
        // Do NOT call [NSApp activateIgnoringOtherApps:YES] here
        // This was causing the focus/input issues
    }
}

// Helper function to bring the window to front
extern "C" void bringWindowToFront(void *winId)
{
    NSView *view = reinterpret_cast<NSView *>(winId);
    if (!view) return;
    
    NSWindow *nsWindow = [view window];
    if (!nsWindow) return;
    
    [nsWindow setLevel:NSScreenSaverWindowLevel];
    // Bring window to front and make it key
    [nsWindow orderFront:nil];
    [NSApp activateIgnoringOtherApps:YES];
}
