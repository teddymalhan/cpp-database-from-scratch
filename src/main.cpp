#include "project/main_window.hpp"
#include <gtkmm/application.h>
#include <memory>

int main(int argc, char* argv[])
{
  // Note: Environment variables for font rendering and theme are handled
  // through GTK settings in MainWindow. Theme changes are managed via
  // the dark mode toggle in the UI.
  
  // Using auto for type deduction (modern C++)
  // Application is managed by GTK's internal reference counting (RAII)
  const auto app = Gtk::Application::create("com.postgresmvcc.visualizer");

  return app->make_window_and_run<MainWindow>(argc, argv);
}

