#pragma once

#include <boost/asio/thread_pool.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <memory>

#include "tuwe/client/weather.hpp"

namespace tuwe::client {

class Application {
 public:
  explicit Application();

  int Execute();

  void ChangeView(ftxui::Component view);

  ftxui::ScreenInteractive& getScreen();

  asio::thread_pool& GetThreadPool();

  WeatherClient& GetWeatherClient();

 private:
  ftxui::ScreenInteractive screen_;

  ftxui::Component view_;
  ftxui::Component renderer_;

  ftxui::Element Render();

  std::unique_ptr<asio::thread_pool> thread_pool_;

  std::unique_ptr<WeatherClient> weather_client_;
};

}  // namespace tuwe::client