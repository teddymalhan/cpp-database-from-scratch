#ifndef PROJECT_MAIN_WINDOW_CONTROLLER_HPP_
#define PROJECT_MAIN_WINDOW_CONTROLLER_HPP_

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

/// Objective-C++ interface for the main window controller
/// Bridges the C++ MainWindowCore to AppKit for native macOS UI
@interface MainWindowController : NSWindowController <NSToolbarDelegate>

/// Shows the window and activates the application
- (void)showWindow;

/// Toggles dark mode (informational - system appearance cannot be changed programmatically)
- (void)toggleDarkMode;

@end

NS_ASSUME_NONNULL_END

#endif  // PROJECT_MAIN_WINDOW_CONTROLLER_HPP_

