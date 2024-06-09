#include "tuwe/client/client.hpp"

#include <chrono>
#include <exception>
#include <ftxui/component/component.hpp>
#include <ftxui/component/loop.hpp>
#include <functional>
#include <iostream>
#include <thread>

#include "tuwe/client/components.hpp"
#include "tuwe/client/views.hpp"

namespace tuwe::client {

Application::Application() : screen(this->createScreen()) {
  const std::function<ftxui::Element()> render = std::bind(&Application::render, this);
  this->renderer = ftxui::Renderer(render);
}

ftxui::ScreenInteractive Application::createScreen() {
  return ftxui::ScreenInteractive::Fullscreen();
}

void Application::changeView(ftxui::Component view) {
  if (this->view) this->view->Detach();

  this->view = std::move(view);
  this->renderer->Add(this->view);
}

ftxui::Element Application::render() {
  if (!this->view) {
    return ftxui::text("Fallback view");
  }

  return this->view->Render();
}

int Application::execute() {
  this->changeView(
      std::make_shared<MainView>(*this)
  );

  this->renderThread = std::thread(
      &ftxui::ScreenInteractive::Loop,
      std::ref(this->screen),
      std::ref(this->renderer)
  );

  this->renderThread.join();

  return EXIT_SUCCESS;
}

}  // namespace tuwe::client