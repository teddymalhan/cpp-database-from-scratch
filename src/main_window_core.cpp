#include "project/main_window_core.hpp"
#include <iostream>

MainWindowCore::MainWindowCore() {
  initialize_theme();
}

void MainWindowCore::initialize_theme() noexcept {
  // On macOS, we'll detect system appearance via AppKit
  // For now, default to light mode
  m_is_dark_mode = false;
}

void MainWindowCore::set_dark_mode(bool enabled) noexcept {
  if (m_is_dark_mode != enabled) {
    m_is_dark_mode = enabled;
    
    if (m_dark_mode_callback) {
      m_dark_mode_callback(enabled);
    }
    
    std::cout << "Dark mode: " << (m_is_dark_mode ? "ON" : "OFF") << '\n';
  }
}

