#import <Cocoa/Cocoa.h>
#include "project/main_window_controller.hpp"
#include "project/main_window_core.hpp"
#include <memory>
#include <iostream>
#include <string>

NS_ASSUME_NONNULL_BEGIN

// Private extension for implementation details
@interface MainWindowController () {
  std::unique_ptr<MainWindowCore> _core;
}

/// Sets up the content view with labels and layout
- (void)setupContentView;

/// Sets up the toolbar with dark mode toggle
- (void)setupToolbar;

/// Updates the appearance based on the current system appearance
- (void)updateAppearance;

/// Handles system appearance change notifications
- (void)interfaceStyleChanged:(NSNotification*)notification;

@end

NS_ASSUME_NONNULL_END

@implementation MainWindowController

#pragma mark - Initialization

- (instancetype)init {
  // Create the C++ core first
  _core = std::make_unique<MainWindowCore>();
  
  // Create the window with default dimensions
  const NSRect contentRect = NSMakeRect(0, 0, 
                                        _core->get_default_width(), 
                                        _core->get_default_height());
  
  const NSWindowStyleMask styleMask = (NSWindowStyleMaskTitled | 
                                       NSWindowStyleMaskClosable |
                                       NSWindowStyleMaskMiniaturizable |
                                       NSWindowStyleMaskResizable);
  
  NSWindow* const window = [[NSWindow alloc] initWithContentRect:contentRect
                                                        styleMask:styleMask
                                                          backing:NSBackingStoreBuffered
                                                            defer:NO];
  
  // Set window title from C++ core
  const std::string_view title = _core->get_window_title();
  window.title = [NSString stringWithUTF8String:title.data()];
  [window center];
  
  // Initialize NSWindowController with the window
  self = [super initWithWindow:window];
  if (self) {
    [self setupContentView];
    [self setupToolbar];
    [self registerForAppearanceChanges];
    [self updateAppearance];
    [self setupDarkModeCallback];
  }
  return self;
}

- (void)dealloc {
  [[NSDistributedNotificationCenter defaultCenter] removeObserver:self];
  [super dealloc];
}

#pragma mark - Private Setup Methods

- (void)registerForAppearanceChanges {
  [[NSDistributedNotificationCenter defaultCenter]
   addObserver:self
   selector:@selector(interfaceStyleChanged:)
   name:@"AppleInterfaceThemeChangedNotification"
   object:nil];
}

- (void)setupDarkModeCallback {
  _core->set_dark_mode_changed_callback([self](bool enabled) {
    // This will be called when dark mode is toggled programmatically
    // The actual appearance update is handled by the system
    (void)enabled; // Suppress unused parameter warning
  });
}

- (void)setupContentView {
  NSView* const contentView = self.window.contentView;
  if (!contentView) {
    return;
  }
  
  // Create main container using modern NSStackView
  NSStackView* const stackView = [[NSStackView alloc] init];
  stackView.orientation = NSUserInterfaceLayoutOrientationVertical;
  stackView.alignment = NSLayoutAttributeCenterX;
  stackView.distribution = NSStackViewDistributionFill;
  stackView.spacing = 20.0;
  stackView.edgeInsets = NSEdgeInsetsMake(20, 20, 20, 20);
  
  // Create title label from C++ core
  const std::string_view titleText = _core->get_title_label_text();
  NSTextField* const titleLabel = [NSTextField labelWithString:
    [NSString stringWithUTF8String:titleText.data()]];
  titleLabel.font = [NSFont boldSystemFontOfSize:24];
  titleLabel.alignment = NSTextAlignmentCenter;
  
  // Create content label from C++ core
  const std::string_view contentText = _core->get_content_label_text();
  NSTextField* const contentLabel = [NSTextField labelWithString:
    [NSString stringWithUTF8String:contentText.data()]];
  contentLabel.font = [NSFont systemFontOfSize:14];
  contentLabel.alignment = NSTextAlignmentCenter;
  contentLabel.preferredMaxLayoutWidth = static_cast<CGFloat>(_core->get_default_width() - 80);
  
  [stackView addArrangedSubview:titleLabel];
  [stackView addArrangedSubview:contentLabel];
  
  // Add constraints using modern Auto Layout API
  stackView.translatesAutoresizingMaskIntoConstraints = NO;
  [contentView addSubview:stackView];
  
  const CGFloat margin = 20.0;
  [NSLayoutConstraint activateConstraints:@[
    [stackView.centerXAnchor constraintEqualToAnchor:contentView.centerXAnchor],
    [stackView.centerYAnchor constraintEqualToAnchor:contentView.centerYAnchor],
    [stackView.leadingAnchor constraintGreaterThanOrEqualToAnchor:contentView.leadingAnchor 
                                                          constant:margin],
    [stackView.trailingAnchor constraintLessThanOrEqualToAnchor:contentView.trailingAnchor 
                                                         constant:-margin],
    [stackView.topAnchor constraintGreaterThanOrEqualToAnchor:contentView.topAnchor 
                                                     constant:margin],
    [stackView.bottomAnchor constraintLessThanOrEqualToAnchor:contentView.bottomAnchor 
                                                      constant:-margin]
  ]];
}

- (void)setupToolbar {
  NSToolbar* const toolbar = [[NSToolbar alloc] initWithIdentifier:@"MainToolbar"];
  toolbar.displayMode = NSToolbarDisplayModeIconOnly;
  toolbar.delegate = self;
  self.window.toolbar = toolbar;
}

- (void)updateAppearance {
  NSAppearance* const appearance = self.window.effectiveAppearance;
  BOOL isDark = NO;
  
  if (@available(macOS 10.14, *)) {
    const NSString* const appearanceName = appearance.name;
    isDark = ([appearanceName isEqualToString:NSAppearanceNameDarkAqua] ||
              [appearanceName isEqualToString:NSAppearanceNameVibrantDark] ||
              [appearanceName isEqualToString:NSAppearanceNameAccessibilityHighContrastDarkAqua] ||
              [appearanceName isEqualToString:NSAppearanceNameAccessibilityHighContrastVibrantDark]);
  }
  
  _core->set_dark_mode(isDark);
}

- (void)interfaceStyleChanged:(NSNotification*)notification {
  (void)notification; // Suppress unused parameter warning
  [self updateAppearance];
}

#pragma mark - Public Methods

- (void)toggleDarkMode {
  // On macOS, we can't directly toggle system appearance programmatically
  // This would typically open System Preferences or use a third-party solution
  std::cout << "Dark mode toggle requested (system appearance cannot be changed programmatically)\n";
}

- (void)showWindow {
  [self.window makeKeyAndOrderFront:nil];
  [NSApp activateIgnoringOtherApps:YES];
}

#pragma mark - NSToolbarDelegate

- (NSArray<NSToolbarItemIdentifier>*)toolbarAllowedItemIdentifiers:(NSToolbar*)toolbar {
  (void)toolbar; // Suppress unused parameter warning
  return @[@"DarkModeToggle"];
}

- (NSArray<NSToolbarItemIdentifier>*)toolbarDefaultItemIdentifiers:(NSToolbar*)toolbar {
  (void)toolbar; // Suppress unused parameter warning
  return @[@"DarkModeToggle"];
}

- (nullable NSToolbarItem*)toolbar:(NSToolbar*)toolbar 
              itemForItemIdentifier:(NSToolbarItemIdentifier)itemIdentifier 
            willBeInsertedIntoToolbar:(BOOL)flag {
  (void)toolbar; // Suppress unused parameter warning
  (void)flag; // Suppress unused parameter warning
  
  if (![itemIdentifier isEqualToString:@"DarkModeToggle"]) {
    return nil;
  }
  
  NSToolbarItem* const item = [[NSToolbarItem alloc] initWithItemIdentifier:itemIdentifier];
  item.label = @"Dark Mode";
  item.toolTip = @"Toggle Dark Mode";
  item.target = self;
  item.action = @selector(toggleDarkMode);
  
  // Use modern SF Symbols on macOS 11.0+, fallback to template image
  if (@available(macOS 11.0, *)) {
    item.image = [NSImage imageWithSystemSymbolName:@"moon.fill" 
                                accessibilityDescription:@"Dark Mode"];
  } else {
    item.image = [NSImage imageNamed:NSImageNameActionTemplate];
  }
  
  return item;
}

@end

