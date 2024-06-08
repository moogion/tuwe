#pragma once

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>
#include <thread>

namespace tuwe::client {

class Application {
 private:
  ftxui::Component view;
  ftxui::ScreenInteractive screen;

  ftxui::ScreenInteractive createScreen();

  ftxui::Component renderer;
  ftxui::Element render();

  std::thread renderThread;

 public:
  explicit Application();

  ~Application() = default;

  void changeView(ftxui::Component view);

  int execute();
};

}  // namespace tuwe::client