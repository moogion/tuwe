#include "tuwe/client/weather.hpp"

#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/beast/version.hpp>
#include <boost/json/parse.hpp>
#include <boost/url/urls.hpp>

namespace tuwe::client {

const boost::urls::url_view WeatherClient::kURL{"https://api.openweathermap.org"};

WeatherClient::WeatherClient(const asio::any_io_executor& executor, const std::string&& api_key)
    : api_key_(std::move(api_key)),
      executor_(executor),
      resolver_(executor),
      ssl_context_(ssl::context::tls_client) {
  this->ssl_context_.set_default_verify_paths();
};

template <class ResponseBody, class RequestBody>
boost::optional<http::response<ResponseBody>> WeatherClient::Fetch(
    const http::request<RequestBody>& request,
    boost::system::error_code& error_code,
    asio::yield_context& yield
) {
  ssl::stream<beast::tcp_stream> ssl_stream(this->executor_, this->ssl_context_);

  const std::string host = this->kURL.host_address();

  if (!SSL_set_tlsext_host_name(ssl_stream.native_handle(), host.c_str())) {
    error_code.assign(::ERR_get_error(), asio::error::get_ssl_category());

    return boost::none;
  }

  const std::string default_port = this->kURL.scheme() == "https" ? "443" : "80";
  const std::string service = this->kURL.has_port() ? static_cast<std::string>(this->kURL.port()) : default_port;

  const tcp::resolver::query query(host, service);

  const tcp::resolver::results_type resolved = this->resolver_.async_resolve(query, yield[error_code]);

  if (error_code) {
    return boost::none;
  }

  beast::lowest_layer_type<ssl::stream<beast::tcp_stream>>& lowest_layer = beast::get_lowest_layer(ssl_stream);

  lowest_layer.expires_after(std::chrono::seconds(30));
  lowest_layer.async_connect(resolved, yield[error_code]);

  if (error_code) {
    return boost::none;
  }

  lowest_layer.expires_after(std::chrono::seconds(30));
  ssl_stream.async_handshake(ssl::stream_base::client, yield[error_code]);

  if (error_code) {
    return boost::none;
  }

  lowest_layer.expires_after(std::chrono::seconds(30));
  http::async_write(ssl_stream, request, yield[error_code]);

  if (error_code) {
    return boost::none;
  }

  beast::flat_buffer buffer;
  http::response<http::string_body> response;

  lowest_layer.expires_after(std::chrono::seconds(30));
  http::async_read(ssl_stream, buffer, response, yield[error_code]);

  if (error_code) {
    return boost::none;
  }

  return std::move(response);
}

boost::unique_future<std::unique_ptr<Geolocations>> WeatherClient::FetchGeolocations(const std::string& query, const int& limit) {
  boost::promise<std::unique_ptr<Geolocations>> promise;
  boost::unique_future<std::unique_ptr<Geolocations>> future = promise.get_future();

  asio::spawn(this->executor_, [this, promise = std::move(promise), query, limit](asio::yield_context yield) mutable {
    boost::system::error_code error_code;

    boost::urls::url method{"/geo/1.0/direct"};
    method.params().set("appid", this->api_key_);
    method.params().set("q", query);

    if (limit > 0) {
      method.params().set("limit", std::to_string(limit));
    }

    boost::urls::url url;

    const boost::urls::result<void>& url_resolved = boost::urls::resolve(this->kURL, method, url);
    if (url_resolved.has_error()) {
      promise.set_exception(boost::system::system_error(url_resolved.error()));

      return;
    }

    http::request<http::string_body> request;
    request.method(http::verb::get);
    request.target(url);

    request.set(http::field::host, this->kURL.host_address());
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    boost::optional<http::response<http::string_body>> response = this->Fetch<http::string_body>(request, error_code, yield);

    if (error_code || !response) {
      promise.set_exception(boost::system::system_error(error_code));

      return;
    }

    if (response->result() != http::status::ok) {
      promise.set_exception(std::runtime_error("HTTP error: " + std::to_string(response->result_int())));

      return;
    }

    const http::string_body::value_type& body = response->body();
    try {
      const models::Geolocations geolocations = json::value_to<models::Geolocations>(
          json::parse(body, error_code)
      );

      if (error_code) {
        promise.set_exception(boost::system::system_error(error_code));

        return;
      }

      promise.set_value(std::make_unique<models::Geolocations>(std::move(geolocations)));
    } catch (boost::system::system_error& exception) {
      // TODO: Normalize exceptions plz
      promise.set_exception(std::runtime_error(
          std::string(exception.what()) + "\n" + body
      ));
    }
  });

  return std::move(future);
}

boost::unique_future<std::unique_ptr<CurrentWeather>> WeatherClient::FetchWeather(const Geolocation& geolocation, const std::string& units, const std::string& lang) {
  boost::promise<std::unique_ptr<CurrentWeather>> promise;
  boost::unique_future<std::unique_ptr<CurrentWeather>> future = promise.get_future();

  asio::spawn(this->executor_, [this, promise = std::move(promise), geolocation, units, lang](asio::yield_context yield) mutable {
    boost::system::error_code error_code;

    boost::urls::url method{"/data/2.5/weather"};
    method.params().set("appid", this->api_key_);
    method.params().set("lat", std::to_string(geolocation.lat));
    method.params().set("lon", std::to_string(geolocation.lon));
    method.params().set("units", units);
    method.params().set("lang", lang);

    boost::urls::url url;

    const boost::urls::result<void>& url_resolved = boost::urls::resolve(this->kURL, method, url);
    if (url_resolved.has_error()) {
      promise.set_exception(boost::system::system_error(url_resolved.error()));

      return;
    }

    http::request<http::string_body> request;
    request.method(http::verb::get);
    request.target(url);

    request.set(http::field::host, this->kURL.host_address());
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    boost::optional<http::response<http::string_body>> response = this->Fetch<http::string_body>(request, error_code, yield);

    if (error_code || !response) {
      promise.set_exception(boost::system::system_error(error_code));

      return;
    }

    if (response->result() != http::status::ok) {
      promise.set_exception(std::runtime_error("HTTP error: " + std::to_string(response->result_int())));

      return;
    }

    const models::CurrentWeather current_weather = json::value_to<models::CurrentWeather>(
        json::parse(response->body(), error_code)
    );

    if (error_code) {
      promise.set_exception(boost::system::system_error(error_code));

      return;
    }

    promise.set_value(std::make_unique<models::CurrentWeather>(std::move(current_weather)));
  });

  return std::move(future);
}

boost::unique_future<std::unique_ptr<HourlyForecast>> WeatherClient::FetchHourlyForecast(const Geolocation& geolocation, const int& counts, const std::string& units, const std::string& lang) {
  boost::promise<std::unique_ptr<HourlyForecast>> promise;
  boost::unique_future<std::unique_ptr<HourlyForecast>> future = promise.get_future();

  asio::spawn(this->executor_, [this, promise = std::move(promise), geolocation, counts, units, lang](asio::yield_context yield) mutable {
    boost::system::error_code error_code;

    boost::urls::url method{"/data/2.5/forecast"};
    method.params().set("appid", this->api_key_);
    method.params().set("lat", std::to_string(geolocation.lat));
    method.params().set("lon", std::to_string(geolocation.lon));
    method.params().set("cnt", std::to_string(counts));
    method.params().set("units", units);
    method.params().set("lang", lang);

    boost::urls::url url;

    const boost::urls::result<void>& url_resolved = boost::urls::resolve(this->kURL, method, url);
    if (url_resolved.has_error()) {
      promise.set_exception(boost::system::system_error(url_resolved.error()));

      return;
    }

    http::request<http::string_body> request;
    request.method(http::verb::get);
    request.target(url);

    request.set(http::field::host, this->kURL.host_address());
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    boost::optional<http::response<http::string_body>> response = this->Fetch<http::string_body>(request, error_code, yield);

    if (error_code || !response) {
      promise.set_exception(boost::system::system_error(error_code));

      return;
    }

    if (response->result() != http::status::ok) {
      promise.set_exception(std::runtime_error("HTTP error: " + std::to_string(response->result_int())));

      return;
    }

    const models::HourlyForecast current_weather = json::value_to<models::HourlyForecast>(
        json::parse(response->body(), error_code)
    );

    if (error_code) {
      promise.set_exception(boost::system::system_error(error_code));

      return;
    }

    promise.set_value(std::make_unique<models::HourlyForecast>(std::move(current_weather)));
  });

  return std::move(future);
}

}  // namespace tuwe::client
