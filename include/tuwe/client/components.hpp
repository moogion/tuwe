#pragma once

#include <ftxui/component/component_base.hpp>
#include <memory>

namespace tuwe::client {

enum ScrollerDirection {
  ALL,
  WIDTH,
  HIGHT,
};

class Scroller : public ftxui::ComponentBase {
 public:
  explicit Scroller(ftxui::Component child) : Scroller(child, ScrollerDirection::ALL) {};
  explicit Scroller(ftxui::Component child, const ScrollerDirection direction);

 private:
  ftxui::Element Render() override;

  ScrollerDirection direction_;

  int scroll_width_;
  int scroll_height_;

  int size_width_;
  int size_height_;

  ftxui::Box box_;

  bool OnEvent(ftxui::Event event) override;

  bool Focusable() const override;
};

using ScrollerComponent = std::shared_ptr<Scroller>;
ScrollerComponent MakeScroller(ftxui::Component child, const ScrollerDirection direction = ScrollerDirection::ALL);

}  // namespace tuwe::client