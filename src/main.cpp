#include <csignal>
#include <exception>
#include <iostream>

#include "tuwe/client/client.hpp"

int main() {
  std::set_terminate([] {
    try {
      std::exception_ptr exception = std::current_exception();
      if (!exception) return;

      std::rethrow_exception(exception);
    } catch (const std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
      std::cerr << "Unknown exception caught" << std::endl;
    }

    std::exit(EXIT_FAILURE);
  });

  // TODO: add parsing arguments for application options
  tuwe::client::Application application;

  // Run the application
  return application.Execute();
}
