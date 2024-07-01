#include "tuwe/client/models.hpp"

namespace tuwe::client::models {

WeatherCoord tag_invoke(const json::value_to_tag<WeatherCoord>& to_tag, const json::value& value) {
  const json::object& object = value.as_object();

  return {
      .lon = object.at("lon").to_number<float>(),
      .lat = object.at("lat").to_number<float>(),
  };
}

WeatherMain tag_invoke(const json::value_to_tag<WeatherMain>& to_tag, const json::value& value) {
  const json::object& object = value.as_object();

  return {
      .temp = object.at("temp").to_number<float>(),
      .feels_like = object.at("feels_like").to_number<float>(),
      .pressure = object.at("pressure").to_number<unsigned>(),
      .humidity = object.at("humidity").to_number<short>(),
      .temp_min = object.at("temp_min").to_number<float>(),
      .temp_max = object.at("temp_max").to_number<float>(),
      .sea_level = object.at("sea_level").to_number<unsigned>(),
      .grnd_level = object.at("grnd_level").to_number<unsigned>(),
  };
}

WeatherDescriptions tag_invoke(const json::value_to_tag<WeatherDescriptions>& to_tag, const json::value& value) {
  const json::array& array = value.as_array();

  WeatherDescriptions descriptions;
  descriptions.reserve(array.size());

  for (const json::value& descriptionValue : array) {
    descriptions.push_back({
        .id = descriptionValue.at("id").to_number<int>(),
        .main = json::value_to<std::string>(descriptionValue.at("main")),
        .description = json::value_to<std::string>(descriptionValue.at("description")),
        .icon = json::value_to<std::string>(descriptionValue.at("icon")),
    });
  }

  return std::move(descriptions);
}

WeatherWind tag_invoke(const json::value_to_tag<WeatherWind>& to_tag, const json::value& value) {
  const json::object& object = value.as_object();

  return {
      .speed = object.at("speed").to_number<float>(),
      .deg = object.at("deg").to_number<short>(),
      .gust = object.contains("gust") ? object.at("gust").to_number<float>() : -1,
  };
}

WeatherClouds tag_invoke(const json::value_to_tag<WeatherClouds>& to_tag, const json::value& value) {
  const json::object& object = value.as_object();

  return {
      .all = object.at("all").to_number<short>(),
  };
}

boost::optional<WeatherRain> tag_invoke(const json::value_to_tag<boost::optional<WeatherRain>>& to_tag, const json::value& value) {
  if (value.is_null()) {
    return boost::none;
  }

  const json::object& object = value.as_object();

  return boost::optional<WeatherRain>({
      ._1h = object.contains("1h") ? object.at("1h").to_number<float>() : -1,
      ._3h = object.contains("3h") ? object.at("3h").to_number<float>() : -1,
  });
}

boost::optional<WeatherSnow> tag_invoke(const json::value_to_tag<boost::optional<WeatherSnow>>& to_tag, const json::value& value) {
  if (value.is_null()) {
    return boost::none;
  }

  const json::object& object = value.as_object();

  return boost::optional<WeatherSnow>({
      ._1h = object.contains("1h") ? object.at("1h").to_number<float>() : -1,
      ._3h = object.contains("3h") ? object.at("3h").to_number<float>() : -1,
  });
}

Geolocation tag_invoke(const json::value_to_tag<Geolocation>& to_tag, const json::value& value) {
  const json::object& object = value.as_object();

  std::map<std::string, std::string> local_names;
  if (object.contains("local_names")) {
    const json::object& local_names_ = object.at("local_names").as_object();
    for (const auto& [key, value] : local_names_) {
      local_names[key] = json::value_to<std::string>(value);
    }
  }

  return Geolocation{
      .name = json::value_to<std::string>(object.at("name")),
      .local_names = std::move(local_names),

      .lon = object.at("lon").to_number<double>(),
      .lat = object.at("lat").to_number<double>(),

      .country = json::value_to<std::string>(object.at("country")),
      .state = object.contains("state") ? boost::optional<std::string>(json::value_to<std::string>(object.at("state")))
                                        : boost::none,
  };
}

CurrentWeather tag_invoke(const json::value_to_tag<CurrentWeather>& to_tag, const json::value& value) {
  const json::object& object = value.as_object();

  return {
      .coord = json::value_to<WeatherCoord>(object.at("coord")),

      .main = json::value_to<WeatherMain>(object.at("main")),
      .descriptions = json::value_to<WeatherDescriptions>(object.at("weather")),

      .visibility = object.at("visibility").to_number<float>(),

      .wind = json::value_to<WeatherWind>(object.at("wind")),
      .clouds = json::value_to<WeatherClouds>(object.at("clouds")),

      .rain = json::value_to<boost::optional<WeatherRain>>(
          object.contains("rain") ? object.at("rain") : json::value(nullptr)
      ),
      .snow = json::value_to<boost::optional<WeatherSnow>>(
          object.contains("snow") ? object.at("snow") : json::value(nullptr)
      ),

      .dt = object.at("dt").to_number<std::time_t>(),
      .timezone = object.at("timezone").to_number<std::time_t>(),
  };
}

HourlyForecast tag_invoke(const json::value_to_tag<HourlyForecast>& to_tag, const json::value& value) {
  const json::object& object = value.as_object();

  const json::array& forecastArray = object.at("list").as_array();

  HourlyForecast::Forecasts forecasts;
  forecasts.reserve(forecastArray.size());

  for (const json::value& forecastValue : forecastArray) {
    const json::object& forecastObject = forecastValue.as_object();

    forecasts.push_back({
        .dt = forecastObject.at("dt").to_number<std::time_t>(),
        .dt_txt = json::value_to<std::string>(forecastObject.at("dt_txt")),

        .main = json::value_to<WeatherMain>(forecastObject.at("main")),
        .description = json::value_to<WeatherDescriptions>(forecastObject.at("weather")),

        .visibility = forecastObject.at("visibility").to_number<float>(),

        .wind = json::value_to<WeatherWind>(forecastObject.at("wind")),
        .clouds = json::value_to<WeatherClouds>(forecastObject.at("clouds")),

        .rain = json::value_to<boost::optional<WeatherRain>>(
            forecastObject.contains("rain") ? forecastObject.at("rain") : json::value(nullptr)
        ),
        .snow = json::value_to<boost::optional<WeatherSnow>>(
            forecastObject.contains("snow") ? forecastObject.at("snow") : json::value(nullptr)
        ),

        .pop = forecastObject.at("pop").to_number<float>(),

        .sys = {
            .pod = forecastObject.at("sys").at("pod").as_string().front(),
        },
    });
  }

  const json::object& cityObject = object.at("city").as_object();
  const HourlyForecast::City city = {
      .coord = json::value_to<WeatherCoord>(cityObject.at("coord")),

      .population = cityObject.at("population").to_number<long long>(),

      .sunrise = cityObject.at("sunrise").to_number<std::time_t>(),
      .sunset = cityObject.at("sunset").to_number<std::time_t>(),

      .timezone = cityObject.at("timezone").to_number<std::time_t>(),
  };

  return {
      .cnt = object.at("cnt").to_number<unsigned>(),
      .forecasts = std::move(forecasts),
      .city = std::move(city),
  };
}

}  // namespace tuwe::client::models