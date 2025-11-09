#ifndef PROJECT_MAIN_WINDOW_CORE_HPP_
#define PROJECT_MAIN_WINDOW_CORE_HPP_

#include <string_view>
#include <string>
#include <functional>

// Pure C++ core class - no UI dependencies
class MainWindowCore {
public:
  MainWindowCore();
  ~MainWindowCore() = default;
  
  // Disable copy and move
  MainWindowCore(const MainWindowCore&) = delete;
  MainWindowCore& operator=(const MainWindowCore&) = delete;
  MainWindowCore(MainWindowCore&&) = delete;
  MainWindowCore& operator=(MainWindowCore&&) = delete;

  // Getters - all marked [[nodiscard]] to prevent ignoring return values
  [[nodiscard]] std::string_view get_window_title() const noexcept { return WINDOW_TITLE; }
  [[nodiscard]] std::string_view get_title_label_text() const noexcept { return TITLE_LABEL_TEXT; }
  [[nodiscard]] std::string_view get_content_label_text() const noexcept { return CONTENT_LABEL_TEXT; }
  
  [[nodiscard]] int get_default_width() const noexcept { return DEFAULT_WINDOW_WIDTH; }
  [[nodiscard]] int get_default_height() const noexcept { return DEFAULT_WINDOW_HEIGHT; }
  [[nodiscard]] int get_content_margin() const noexcept { return CONTENT_MARGIN; }
  [[nodiscard]] int get_main_box_margin() const noexcept { return MAIN_BOX_MARGIN; }
  
  [[nodiscard]] bool is_dark_mode() const noexcept { return m_is_dark_mode; }
  
  // Setters
  void set_dark_mode(bool enabled) noexcept;
  
  // Callback registration for UI updates
  using DarkModeChangedCallback = std::function<void(bool)>;
  void set_dark_mode_changed_callback(DarkModeChangedCallback callback) noexcept {
    m_dark_mode_callback = callback;
  }

private:
  // dimensions
  static constexpr int DEFAULT_WINDOW_WIDTH = 800;
  static constexpr int DEFAULT_WINDOW_HEIGHT = 600;
  
  // margins
  static constexpr int CONTENT_MARGIN = 20;
  static constexpr int MAIN_BOX_MARGIN = 10;
  
  // text
  static constexpr const char* WINDOW_TITLE = "PostgreSQL MVCC Visualizer";
  static constexpr const char* TITLE_LABEL_TEXT = "PostgreSQL MVCC Visualizer";
  static constexpr const char* CONTENT_LABEL_TEXT = 
    "Welcome to the PostgreSQL MVCC Visualizer!\n\n"
    "This application will help you understand how PostgreSQL's\n"
    "Multi-Version Concurrency Control (MVCC) system works.";
  
  bool m_is_dark_mode{false};
  DarkModeChangedCallback m_dark_mode_callback;
  
  void initialize_theme() noexcept;
};

#endif  // PROJECT_MAIN_WINDOW_CORE_HPP_

