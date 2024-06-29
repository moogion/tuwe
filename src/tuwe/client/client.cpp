#include "tuwe/client/client.hpp"

#include "tuwe/client/views.hpp"

namespace tuwe::client {

Application::Application()
    : screen_(ftxui::ScreenInteractive::Fullscreen()) {
  this->thread_pool_ = std::make_unique<boost::asio::thread_pool>(
      std::min(4u, std::thread::hardware_concurrency())
  );

  this->weather_client_ = std::make_unique<WeatherClient>(
      this->thread_pool_->executor(),
      "cd20d1c9a690255e0b7de2b713053f22"
  );

  const std::function<ftxui::Element()> render = std::bind(&Application::Render, this);
  this->renderer_ = ftxui::Renderer(render);

  this->ChangeView(MakeMainView(*this));
}

void Application::ChangeView(ftxui::Component view) {
  if (this->view_) this->view_->Detach();

  this->view_ = std::move(view);
  this->renderer_->Add(this->view_);
}

ftxui::Element Application::Render() {
  if (!this->view_) {
    return ftxui::text("Fallback view");
  }

  return this->view_->Render();
}

int Application::Execute() {
  boost::asio::post(
      *this->thread_pool_,
      std::bind(&ftxui::ScreenInteractive::Loop, std::ref(this->screen_), this->renderer_)
  );

  this->thread_pool_->join();

  return EXIT_SUCCESS;
}

ftxui::ScreenInteractive& Application::getScreen() {
  return this->screen_;
}

asio::thread_pool& Application::GetThreadPool() {
  assert(this->thread_pool_ && "Thread pool is not initialized.");
  return *this->thread_pool_;
}

WeatherClient& Application::GetWeatherClient() {
  assert(this->weather_client_ && "Weather client is not initialized.");
  return *this->weather_client_;
}

}  // namespace tuwe::client