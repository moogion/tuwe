#pragma once

#include <ftxui/component/component_base.hpp>

namespace tuwe::client {

enum ScrollerDirection {
  ALL,
  WIDTH,
  HIGHT,
};

class Scroller : public ftxui::ComponentBase {
 private:
  ftxui::Element Render() override;

  ScrollerDirection direction;

  int scrollWidth;
  int scrollHeight;

  int sizeWidth;
  int sizeHeight;

  ftxui::Box box;

  bool OnEvent(ftxui::Event event) override;

  bool Focusable() const override;

 public:
  explicit Scroller(ftxui::Component child) : Scroller(child, ScrollerDirection::ALL){};
  explicit Scroller(ftxui::Component child, const ScrollerDirection direction);
};

}  // namespace tuwe::client