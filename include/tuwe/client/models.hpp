#pragma once

#include <boost/json/value.hpp>
#include <boost/json/value_to.hpp>
#include <boost/optional/optional.hpp>
#include <map>
#include <string>
#include <vector>

namespace json = boost::json;

namespace tuwe::client::models {

struct WeatherCoord {
  const float lon;
  const float lat;
};

WeatherCoord tag_invoke(const json::value_to_tag<WeatherCoord>& to_tag, const json::value& value);

struct WeatherMain {
  const float temp;
  const float feels_like;

  const unsigned pressure;
  const short humidity;

  const float temp_min;
  const float temp_max;

  const unsigned sea_level;
  const unsigned grnd_level;
};

WeatherMain tag_invoke(const json::value_to_tag<WeatherMain>& to_tag, const json::value& value);

struct WeatherDescription {
  const int id;

  const std::string main;
  const std::string description;

  const std::string icon;
};

using WeatherDescriptions = std::vector<WeatherDescription>;

WeatherDescriptions tag_invoke(const json::value_to_tag<WeatherDescriptions>& to_tag, const json::value& value);

struct WeatherWind {
  const float speed;
  const short deg;
  const float gust;
};

WeatherWind tag_invoke(const json::value_to_tag<WeatherWind>& to_tag, const json::value& value);

struct WeatherClouds {
  const short all;
};

WeatherClouds tag_invoke(const json::value_to_tag<WeatherClouds>& to_tag, const json::value& value);

struct WeatherRain {
  float _1h;
  float _3h;
};

boost::optional<WeatherRain> tag_invoke(const json::value_to_tag<boost::optional<WeatherRain>>& to_tag, const json::value& value);

struct WeatherSnow {
  float _1h;
  float _3h;
};

boost::optional<WeatherSnow> tag_invoke(const json::value_to_tag<boost::optional<WeatherSnow>>& to_tag, const json::value& value);

struct Geolocation {
  const std::string name;
  const std::map<std::string, std::string> local_names;

  const double lon;
  const double lat;

  const std::string country;
  boost::optional<std::string> state;
};

Geolocation tag_invoke(const json::value_to_tag<Geolocation>& to_tag, const json::value& value);

using Geolocations = std::vector<Geolocation>;

struct CurrentWeather {
  const WeatherCoord coord;

  const WeatherMain main;
  const WeatherDescriptions descriptions;

  const float visibility;

  const WeatherWind wind;
  const WeatherClouds clouds;

  boost::optional<WeatherRain> rain;
  boost::optional<WeatherSnow> snow;

  const std::time_t dt;
  const std::time_t timezone;
};

CurrentWeather tag_invoke(const json::value_to_tag<CurrentWeather>& to_tag, const json::value& value);

struct HourlyForecast {
  const unsigned cnt;

  struct Forecast {
    const std::time_t dt;
    const std::string dt_txt;

    const WeatherMain main;
    const WeatherDescriptions description;

    const float visibility;

    const WeatherWind wind;
    const WeatherClouds clouds;

    boost::optional<WeatherRain> rain;
    boost::optional<WeatherSnow> snow;

    const float pop;

    const struct Sys {
      const char pod;
    } sys;
  };

  using Forecasts = std::vector<Forecast>;
  const Forecasts forecasts;

  const struct City {
    const WeatherCoord coord;

    const long long population;

    const std::time_t sunrise;
    const std::time_t sunset;

    const std::time_t timezone;
  } city;
};

HourlyForecast tag_invoke(const json::value_to_tag<HourlyForecast>& to_tag, const json::value& value);

}  // namespace tuwe::client::models