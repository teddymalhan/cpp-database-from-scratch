#include "project/main_window.hpp"
#include <iostream>
#include <gtkmm/settings.h>
#include <string>

MainWindow::MainWindow()
  : m_main_box(Gtk::Orientation::VERTICAL),
    m_title_label(TITLE_LABEL_TEXT),
    m_content_label(CONTENT_LABEL_TEXT)
{
  set_title(WINDOW_TITLE);
  set_default_size(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

  m_settings = Gtk::Settings::get_default();
  
  initialize_theme();
  setup_header_bar();
  setup_content();
  
  // Set main box as child
  set_child(m_main_box);
}

void MainWindow::initialize_theme() noexcept
{
  const auto current_theme = m_settings->property_gtk_theme_name().get_value();
  if (current_theme == std::string{THEME_DARK}) {
    m_is_dark_mode = true;
    m_dark_mode_switch.set_active(true);
  }
}

void MainWindow::setup_header_bar() noexcept
{
  m_header_bar.set_title_widget(m_title_label);
  m_header_bar.set_show_title_buttons(true);
  
  m_header_bar.set_use_native_controls(true);
  
  m_dark_mode_switch.set_tooltip_text("Toggle Dark Mode");
  m_dark_mode_switch.set_active(m_is_dark_mode);
  
  m_dark_mode_switch.property_active().signal_changed().connect(
    [this]() { on_dark_mode_toggled(); }
  );
  
  m_header_bar.pack_end(m_dark_mode_switch);
  
  set_titlebar(m_header_bar);
}

void MainWindow::setup_content() noexcept
{
  m_content_label.set_margin(CONTENT_MARGIN);
  m_content_label.set_halign(Gtk::Align::CENTER);
  m_content_label.set_valign(Gtk::Align::CENTER);
  m_content_label.set_expand(true);

  m_main_box.append(m_content_label);
  m_main_box.set_margin(MAIN_BOX_MARGIN);
}

void MainWindow::on_activate() noexcept
{
  present();
}

void MainWindow::on_dark_mode_toggled() noexcept
{
  m_is_dark_mode = m_dark_mode_switch.get_active();
  
  const auto theme_name = m_is_dark_mode 
    ? std::string{THEME_DARK} 
    : std::string{THEME_LIGHT};
  
  m_settings->property_gtk_theme_name().set_value(theme_name);
  
  std::cout << "Dark mode: " << (m_is_dark_mode ? "ON" : "OFF") << '\n';
}

