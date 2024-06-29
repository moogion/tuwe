#include "tuwe/client/views.hpp"

#include <fmt/format.h>

#include <boost/asio/use_future.hpp>
#include <chrono>

#include "tuwe/client/components.hpp"

namespace tuwe::client {

MainViewComponent MakeMainView(Application &application) {
  return std::make_shared<MainView>(application);
}

MainView::MainView(Application &application) : application_(application) {
  const std::function<ftxui::Element()> render = std::bind(&MainView::RenderView, this);
  this->scroller_ = MakeScroller(
      ftxui::Renderer(render),
      ScrollerDirection::HIGHT
  );
  this->Add(this->scroller_);

  ftxui::InputOption input_options = ftxui::InputOption::Default();
  input_options.multiline = false;
  input_options.placeholder = "Введите город или индекс местоположения";

  this->location_input_ = ftxui::Input(&this->location_, input_options);
  this->Add(this->location_input_);

  ftxui::ButtonOption location_button_options{
      .transform = [&future = this->geolocation_future_](const ftxui::EntryState &state) -> ftxui::Element {
        const bool is_ready = !future.valid() || future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;

        const ftxui::Element icon = is_ready ? ftxui::text(state.label) : ftxui::text("Загрузка");
        return icon | (state.focused ? ftxui::inverted : ftxui::nothing);
      },
  };
  this->location_button_ = ftxui::Button(
      "🔍", std::bind(&MainView::FetchGeolocation, this), location_button_options
  );
  this->Add(this->location_button_);

  const std::function<ftxui::Element()> render_hourly = std::bind(&MainView::RenderHourlyForecast, this);
  this->scroller_hourly_ = MakeScroller(
      ftxui::Renderer(render_hourly),
      ScrollerDirection::WIDTH
  );
  this->Add(this->scroller_hourly_);
}

ftxui::Element MainView::RenderView() {
  ftxui::Elements elements;

  ftxui::Element viewDescription = ftxui::hbox({
      ftxui::color(ftxui::Color::GreenLight, ftxui::text("TUWE — ")),
      ftxui::color(ftxui::Color::SkyBlue2, ftxui::text("Terminal User Weather Explorer")),
  });
  viewDescription |= ftxui::borderEmpty;

  elements.push_back(viewDescription);

  ftxui::Element locationInput = ftxui::hbox({
      ftxui::separatorEmpty(),
      ftxui::text("🚩"),
      ftxui::separatorEmpty(),
      this->location_input_->Render(),
      ftxui::separatorEmpty(),
      this->location_button_->Render(),
      ftxui::separatorEmpty(),
  });
  locationInput |= ftxui::borderRounded;

  elements.push_back(locationInput);

  if (this->geolocation_future_.valid()) {
    const std::future_status &location_future_status = this->geolocation_future_.wait_for(std::chrono::seconds(0));
    if (location_future_status != std::future_status::timeout) {
      try {
        this->geolocation_future_.get();
      } catch (std::runtime_error &exception) {
        // TODO: migrate to error code handle
        ftxui::Element status_message = ftxui::text(exception.what());

        status_message |= ftxui::center;
        status_message |= ftxui::borderRounded;

        elements.push_back(status_message);
      }
    }
  }

  ftxui::Element weather = this->RenderWeather();
  if (weather) {
    elements.push_back(weather);
  }

  return ftxui::vbox(elements) | ftxui::borderEmpty;
}

ftxui::Element MainView::RenderWeather() {
  if (this->current_weather_future_.valid()) {
    const std::future_status &current_weather_future_status = this->current_weather_future_.wait_for(std::chrono::seconds(0));
    if (current_weather_future_status != std::future_status::timeout) {
      try {
        this->current_weather_future_.get();
      } catch (std::runtime_error &exception) {
        // TODO: migrate to error code handle
        ftxui::Element status_message = ftxui::text(exception.what());

        status_message |= ftxui::center;
        status_message |= ftxui::borderRounded;

        return status_message;
      }
    }
  }

  if (!this->current_weather_) {
    return nullptr;
  }

  CurrentWeather &current_weather = *this->current_weather_;

  static const std::map<std::string, std::string> weather_desc_icons({
      {"01d", "☀️"},
      {"01n", "🌑"},
      {"02d", "🌤"},
      {"02n", "☁"},
      {"03d", "☁"},
      {"03n", "☁"},
      {"04d", "☁"},
      {"04n", "☁"},
      {"09d", "🌧"},
      {"09n", "🌧"},
      {"10d", "🌦"},
      {"10n", "🌧"},
      {"11d", "🌩"},
      {"11n", "🌩"},
      {"13d", "❄"},
      {"13n", "❄"},
      {"50d", "🌫"},
      {"50n", "🌫"},
  });

  std::string weather_desc_icon = "";
  std::string weather_desc_message = "";

  if (current_weather.descriptions.size() > 0) {
    weather_desc_icon = weather_desc_icons.at(
        current_weather.descriptions.front().icon.c_str()
    );
    weather_desc_message = current_weather.descriptions.front().description;
  } else {
    weather_desc_icon = (*weather_desc_icons.begin()).second;
  }

  ftxui::Element weather_now_temp = ftxui::hbox({
      ftxui::text(fmt::format("{:.2f}℃", current_weather.main.temp)),
      ftxui::separatorEmpty(),
      ftxui::separatorEmpty(),
      ftxui::text(std::move(weather_desc_icon)),
  });
  weather_now_temp |= ftxui::borderEmpty;

  ftxui::Element weather_now_min_max = ftxui::hbox({
      ftxui::hbox({
          ftxui::text("Макс.:"),
          ftxui::separatorEmpty(),
          ftxui::text(fmt::format("{:.2f}℃", current_weather.main.temp_max)),
          ftxui::separatorEmpty(),
      }),
      ftxui::separatorEmpty(),
      ftxui::hbox({
          ftxui::separatorEmpty(),
          ftxui::text("Мин.:"),
          ftxui::separatorEmpty(),
          ftxui::text(fmt::format("{:.2f}℃", current_weather.main.temp_min)),
      }),
  });

  ftxui::Element weather_now = ftxui::vbox({
      ftxui::text("Сейчас"),
      ftxui::hbox({
          ftxui::vbox({
              std::move(weather_now_temp),
              std::move(weather_now_min_max),
          }),
          ftxui::filler() | ftxui::flex,
          ftxui::vbox({
              ftxui::text("Ощущается как " + fmt::format("{:.2f}℃", current_weather.main.feels_like)),
              weather_desc_message.empty() ? ftxui::emptyElement() : ftxui::text(std::move(weather_desc_message)),
          }),
      }),
  });
  weather_now |= ftxui::borderEmpty;

  ftxui::Elements weather_conditions;

  ftxui::Element gust;
  if (current_weather.wind.gust > 0) {
    gust = ftxui::text("До " + fmt::format("{:.2f} м/с", current_weather.wind.gust));
  } else {
    gust = ftxui::emptyElement();
  }

  static const std::vector<std::string> wind_direction_names({"З", "С", "В", "Ю"});
  static const std::vector<std::string> wind_direction_icons({"🡐", "🡑", "🡒", "🡓"});

  const int wind_direction_index = current_weather.wind.deg / 90 % 4;

  ftxui::Element wind_direction_name = ftxui::text(wind_direction_names[wind_direction_index]);
  wind_direction_name |= ftxui::hcenter;

  ftxui::Element wind_direction_icon = ftxui::text(wind_direction_icons[wind_direction_index]);
  wind_direction_icon |= ftxui::center;
  wind_direction_icon |= ftxui::size(ftxui::WidthOrHeight::WIDTH, ftxui::Constraint::GREATER_THAN, 3);
  wind_direction_icon |= ftxui::borderRounded;
  wind_direction_icon |= ftxui::hcenter;

  ftxui::Element wind = ftxui::vbox({
      ftxui::text("Ветер"),
      ftxui::separatorEmpty(),
      ftxui::hbox({
          ftxui::vbox({
              ftxui::text(fmt::format("{:.2f} м/с", current_weather.wind.speed)),
              std::move(gust),
          }) | ftxui::vcenter,
          ftxui::separatorEmpty(),
          ftxui::filler(),
          ftxui::vbox({
              std::move(wind_direction_name),
              std::move(wind_direction_icon),
          }),
      }),
  });
  weather_conditions.push_back(std::move(wind));

  ftxui::Element humidity = ftxui::vbox({
      ftxui::text("Влажность"),
      ftxui::separatorEmpty(),
      ftxui::hbox({
          ftxui::vbox({
              ftxui::text(fmt::format("{:d}%", current_weather.main.humidity)),
              ftxui::text(current_weather.main.humidity > 50 ? "Влажно" : "Сухо"),
          }) | ftxui::vcenter,
          ftxui::filler(),
          ftxui::vbox({
              ftxui::text("100") | ftxui::hcenter,
              ftxui::hbox({
                  ftxui::gaugeUp(current_weather.main.humidity / 100.0F),
                  ftxui::gaugeUp(current_weather.main.humidity / 100.0F),
                  ftxui::gaugeUp(current_weather.main.humidity / 100.0F),
              }) | ftxui::size(ftxui::WidthOrHeight::HEIGHT, ftxui::Constraint::GREATER_THAN, 2) |
                  ftxui::borderLight,
              ftxui::text("0") | ftxui::hcenter,
          }),
          ftxui::separatorEmpty(),
      }),
  });
  weather_conditions.push_back(std::move(humidity));

  ftxui::Element pressure = ftxui::vbox({
      ftxui::text("Давление"),
      ftxui::separatorEmpty(),
      ftxui::hbox({
          ftxui::vbox({
              ftxui::text(fmt::format("{:d}", current_weather.main.pressure)),
              ftxui::text("мбар"),
          }) | ftxui::vcenter,
          ftxui::filler(),
          ftxui::vbox({
              ftxui::text("Высок") | ftxui::hcenter,
              ftxui::hbox({
                  ftxui::gaugeUp(current_weather.main.pressure / 1057.8F),
                  ftxui::gaugeUp(current_weather.main.pressure / 1057.8F),
                  ftxui::gaugeUp(current_weather.main.pressure / 1057.8F),
              }) | ftxui::size(ftxui::WidthOrHeight::HEIGHT, ftxui::Constraint::GREATER_THAN, 2) |
                  ftxui::borderDouble,
              ftxui::text("Низк") | ftxui::hcenter,
          }),
          ftxui::separatorEmpty(),
      }),
  });
  weather_conditions.push_back(std::move(pressure));

  for (ftxui::Element &element : weather_conditions) {
    element |= ftxui::borderEmpty;
    element |= ftxui::borderRounded;
    element |= ftxui::flex;
  }

  const ftxui::Dimensions &terminalSize = ftxui::Terminal::Size();

  return ftxui::vbox({
      std::move(weather_now),
      ftxui::text("Текущие условия") | ftxui::borderEmpty,
      terminalSize.dimx < 120 ? ftxui::vbox(std::move(weather_conditions)) : ftxui::hbox(std::move(weather_conditions)),
  });
}

ftxui::Element MainView::RenderHourlyForecast() {
  // TODO: Implement hourly weather rendering
  return nullptr;
}

void MainView::FetchGeolocation() {
  if (this->geolocation_future_.valid()) {
    const std::future_status &status = this->geolocation_future_.wait_for(std::chrono::seconds(0));
    if (status == std::future_status::timeout) {
      return;
    }
  }

  asio::thread_pool &thread_pool = this->application_.GetThreadPool();
  WeatherClient &weather_client = this->application_.GetWeatherClient();

  std::function<std::shared_ptr<Geolocation>()> fetch_geolocation = [&weather_client, location = this->location_] {
    if (location.empty()) {
      throw std::runtime_error("Некорректно введено местоположение");
    }

    boost::unique_future<std::unique_ptr<Geolocations>> future = weather_client.FetchGeolocations(location, 1);
    if (future.has_exception()) {
      boost::rethrow_exception(future.get_exception_ptr());

      return std::shared_ptr<Geolocation>(nullptr);
    }

    std::unique_ptr<Geolocations> geolocations = future.get();
    if (!geolocations || geolocations->empty()) {
      throw std::runtime_error("Местоположение не найдено");
    }

    return std::make_shared<Geolocation>(geolocations->front());
  };
  this->geolocation_future_ = asio::post(thread_pool, asio::use_future(std::move(fetch_geolocation)));

  this->FetchWeather(this->geolocation_future_);
  this->FetchHourlyForecast(this->geolocation_future_);
}

void MainView::FetchWeather(std::shared_future<std::shared_ptr<Geolocation>> &geolocation_future) {
  if (this->current_weather_future_.valid()) {
    const std::future_status &status = this->current_weather_future_.wait_for(std::chrono::seconds(0));
    if (status == std::future_status::timeout) {
      return;
    }
  }

  ftxui::ScreenInteractive &screen = this->application_.getScreen();
  asio::thread_pool &thread_pool = this->application_.GetThreadPool();
  WeatherClient &weather_client = this->application_.GetWeatherClient();

  std::function<void()> fetch_weather = [&screen, &weather_client, geolocation_future, &current_weather_ = this->current_weather_] {
    std::shared_ptr<Geolocation> geolocation;

    try {
      geolocation = geolocation_future.get();
    } catch (std::runtime_error &exception) {
      // Ignore? :)
    }

    if (!geolocation) {
      current_weather_ = nullptr;

      return;
    }

    boost::unique_future<std::unique_ptr<CurrentWeather>> future = weather_client.FetchWeather(*geolocation, "metric", "ru");
    if (future.has_exception()) {
      boost::rethrow_exception(future.get_exception_ptr());

      return;
    }

    std::unique_ptr<CurrentWeather> current_weather = future.get();
    if (!current_weather) {
      throw std::runtime_error("Данные о погоде в " + geolocation->name + " не найдены");
    }

    // Thread-safe?
    current_weather_ = std::move(current_weather);
    screen.PostEvent(ftxui::Event::Custom);
  };
  this->current_weather_future_ = asio::post(thread_pool, asio::use_future(std::move(fetch_weather)));
}

void MainView::FetchHourlyForecast(std::shared_future<std::shared_ptr<Geolocation>> &geolocation_future) {
}

ftxui::Element MainView::Render() {
  return this->scroller_->Render();
}

}  // namespace tuwe::client