#import <Cocoa/Cocoa.h>
#include "project/app_delegate.hpp"

int main(int argc, char* argv[])
{
  (void)argc; // Suppress unused parameter warning
  (void)argv; // Suppress unused parameter warning
  
  @autoreleasepool {
    NSApplication* const app = [NSApplication sharedApplication];
    
    AppDelegate* const delegate = [[AppDelegate alloc] init];
    app.delegate = delegate;
    
    [app run];
  }
  
  return 0;
}

