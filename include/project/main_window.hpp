#ifndef PROJECT_MAIN_WINDOW_HPP_
#define PROJECT_MAIN_WINDOW_HPP_

#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/label.h>
#include <gtkmm/switch.h>
#include <gtkmm/settings.h>
#include <string_view>

class MainWindow : public Gtk::ApplicationWindow
{
public:
  MainWindow();
  ~MainWindow() override = default;
  
  // Disable copy and move
  MainWindow(const MainWindow&) = delete;
  MainWindow& operator=(const MainWindow&) = delete;
  MainWindow(MainWindow&&) = delete;
  MainWindow& operator=(MainWindow&&) = delete;

protected:
  void on_activate() noexcept;

private:
  // dimensions
  static constexpr int DEFAULT_WINDOW_WIDTH = 800;
  static constexpr int DEFAULT_WINDOW_HEIGHT = 600;
  
  // margins
  static constexpr int CONTENT_MARGIN = 20;
  static constexpr int MAIN_BOX_MARGIN = 10;
  
  // themes
  static constexpr std::string_view THEME_LIGHT = "Adwaita";
  static constexpr std::string_view THEME_DARK = "Adwaita-dark";
  
  // text
  static constexpr const char* WINDOW_TITLE = "PostgreSQL MVCC Visualizer";
  static constexpr const char* TITLE_LABEL_TEXT = "PostgreSQL MVCC Visualizer";
  static constexpr const char* CONTENT_LABEL_TEXT = 
    "Welcome to the PostgreSQL MVCC Visualizer!\n\n"
    "This application will help you understand how PostgreSQL's\n"
    "Multi-Version Concurrency Control (MVCC) system works.";
  
  // sighandlers
  void on_dark_mode_toggled() noexcept;
  
  // helpers
  void setup_header_bar() noexcept;
  void setup_content() noexcept;
  void initialize_theme() noexcept;
  
  // widgets
  Gtk::Box m_main_box;
  Gtk::HeaderBar m_header_bar;
  Gtk::Label m_title_label;
  Gtk::Label m_content_label;
  Gtk::Switch m_dark_mode_switch;
  
  Glib::RefPtr<Gtk::Settings> m_settings;
  bool m_is_dark_mode{false};
};

#endif  // PROJECT_MAIN_WINDOW_HPP_

