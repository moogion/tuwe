#include "tuwe/client/client.hpp"

#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/component/loop.hpp>
#include <functional>
#include <iostream>
#include <thread>

#include "tuwe/client/views.hpp"

namespace tuwe::client {

Application::Application() : screen(this->createScreen()) {
  const std::function<ftxui::Element()> renderFunction = std::bind(
      &Application::render, this
  );
  this->renderer = ftxui::Renderer(renderFunction);
}

Application::~Application() {
  this->renderThread.join();
}

ftxui::ScreenInteractive Application::createScreen() {
  return ftxui::ScreenInteractive::FitComponent();
}

ftxui::Element Application::render() {
  return this->view->Render();
}

int Application::execute() {
  this->view = std::make_shared<MainView>(*this);

  this->renderThread = std::thread(
      &ftxui::ScreenInteractive::Loop,
      std::ref(this->screen),
      std::ref(this->renderer)
  );

  return 0;
}

}  // namespace tuwe::client