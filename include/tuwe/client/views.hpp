#pragma once

#include <ftxui/component/component_base.hpp>
#include <future>
#include <memory>
#include <string>

#include "tuwe/client/client.hpp"

namespace tuwe::client {

class MainView : public ftxui::ComponentBase {
 public:
  explicit MainView(Application &application);
  ftxui::Element Render() override;

 private:
  Application &application_;

  std::shared_future<std::shared_ptr<Geolocation>> geolocation_future_;

  void FetchGeolocation();

  std::unique_ptr<CurrentWeather> current_weather_;
  std::future<void> current_weather_future_;

  void FetchWeather(std::shared_future<std::shared_ptr<Geolocation>> &geolocation_future);

  std::unique_ptr<HourlyForecast> hourly_forecast_;
  std::future<void> hourly_forecast_future_;

  void FetchHourlyForecast(std::shared_future<std::shared_ptr<Geolocation>> &geolocation_future);

  std::string location_;

  ftxui::Component location_input_;
  ftxui::Component location_button_;

  ftxui::Component scroller_;
  ftxui::Element RenderView();

  ftxui::Component scroller_hourly_;
  ftxui::Element RenderHourlyForecast();

  ftxui::Element RenderWeather();
};

using MainViewComponent = std::shared_ptr<MainView>;
MainViewComponent MakeMainView(Application &application);

}  // namespace tuwe::client