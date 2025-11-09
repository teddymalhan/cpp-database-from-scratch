#ifndef PROJECT_APP_DELEGATE_HPP_
#define PROJECT_APP_DELEGATE_HPP_

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class MainWindowController;

/// Objective-C++ interface for the app delegate
/// Manages the application lifecycle and creates the main window
@interface AppDelegate : NSObject <NSApplicationDelegate>

/// The main window controller for the application
@property (nonatomic, strong, nullable) MainWindowController* mainWindowController;

@end

NS_ASSUME_NONNULL_END

#endif  // PROJECT_APP_DELEGATE_HPP_

