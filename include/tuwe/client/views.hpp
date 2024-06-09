#pragma once

#include <ftxui/component/component.hpp>
#include <memory>

#include "tuwe/client/client.hpp"

namespace tuwe::client {

class MainView : public ftxui::ComponentBase {
 private:
  const Application &app;

  ftxui::Component scroller;

  ftxui::Component locationInput;
  ftxui::Component locationButton;
  
  ftxui::Component scrollerHourly;

  ftxui::Element renderHourly();

  ftxui::Element renderView();

 public:
  explicit MainView(const Application &app);
  ftxui::Element Render() override;
};

}  // namespace tuwe::client