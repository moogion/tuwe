#pragma once

#include <boost/asio/any_io_executor.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/thread/future.hpp>
#include <boost/url/url_view.hpp>

#include "tuwe/client/models.hpp"

namespace tuwe::client {

namespace beast = boost::beast;
namespace http = beast::http;

namespace asio = boost::asio;
namespace ssl = boost::asio::ssl;

using tcp = asio::ip::tcp;

using Geolocation = models::Geolocation;
using Geolocations = models::Geolocations;
using CurrentWeather = models::CurrentWeather;
using HourlyForecast = models::HourlyForecast;

class WeatherClient {
 public:
  explicit WeatherClient(const asio::any_io_executor &executor, const std::string &&api_key);

  ~WeatherClient() = default;

  static const boost::urls::url_view kURL;

  boost::unique_future<std::unique_ptr<Geolocations>> FetchGeolocations(
      const std::string &query,
      const int &limit = -1
  );

  boost::unique_future<std::unique_ptr<CurrentWeather>> FetchWeather(
      const Geolocation &geolocation,
      const std::string &units = "metric",
      const std::string &lang = "en"
  );

  boost::unique_future<std::unique_ptr<HourlyForecast>> FetchHourlyForecast(
      const Geolocation &geolocation,
      const int &counts = -1,
      const std::string &units = "metric",
      const std::string &lang = "en"
  );

 private:
  const std::string api_key_;

  const asio::any_io_executor executor_;

  tcp::resolver resolver_;
  ssl::context ssl_context_;

  template <class ResponseBody, class RequestBody = ResponseBody>
  boost::optional<http::response<ResponseBody>> Fetch(
      const http::request<RequestBody> &request,
      boost::system::error_code &error_code,
      asio::yield_context &yield
  );
};

}  // namespace tuwe::client
