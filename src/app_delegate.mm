#import <Cocoa/Cocoa.h>
#include "project/app_delegate.hpp"
#include "project/main_window_controller.hpp"

@implementation AppDelegate

#pragma mark - NSApplicationDelegate

- (void)applicationDidFinishLaunching:(NSNotification*)notification {
  (void)notification; // Suppress unused parameter warning
  
  // Create and show the main window
  self.mainWindowController = [[MainWindowController alloc] init];
  [self.mainWindowController showWindow];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender {
  (void)sender; // Suppress unused parameter warning
  return YES;
}

@end

