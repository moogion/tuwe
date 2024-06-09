#include "tuwe/client/components.hpp"

#include <ftxui/component/event.hpp>
#include <ftxui/dom/node.hpp>
#include <iostream>

namespace tuwe::client {

Scroller::Scroller(ftxui::Component child, const ScrollerDirection direction)
    : scrollWidth(0), scrollHeight(0) {
  this->Add(child);
  this->direction = std::move(direction);
}

ftxui::Element Scroller::Render() {
  ftxui::Element background = ftxui::ComponentBase::Render();
  background->ComputeRequirement();

  const int focusOffsetWidth = this->box.x_max - this->box.x_min - 1;
  const int focusOffsetHeight = this->box.y_max - this->box.y_min - 1;

  const ftxui::Requirement requirement = background->requirement();
  this->sizeWidth = requirement.min_x - focusOffsetWidth;
  this->sizeHeight = requirement.min_y - focusOffsetHeight;

  background |= ftxui::focusPosition(
      focusOffsetWidth / 2 + this->scrollWidth,
      focusOffsetHeight / 2 + this->scrollHeight
  );

  switch (this->direction) {
    case ScrollerDirection::ALL: {
      background |= ftxui::vscroll_indicator;
      background |= ftxui::frame;
      background |= ftxui::flex;
    } break;

    case ScrollerDirection::WIDTH: {
      background |= ftxui::vscroll_indicator;
      background |= ftxui::xframe;
      background |= ftxui::xflex;
    } break;

    case ScrollerDirection::HIGHT: {
      background |= ftxui::vscroll_indicator;
      background |= ftxui::yframe;
      background |= ftxui::yflex;
    } break;
  }

  if (this->Focused()) {
    background |= ftxui::focus;
  }

  return std::move(background) | ftxui::reflect(this->box);
}

bool Scroller::OnEvent(ftxui::Event event) {
  if (ftxui::ComponentBase::OnEvent(event)) {
    return true;
  }

  ftxui::Mouse& mouse = event.mouse();

  if (event.is_mouse() && !box.Contain(mouse.x, mouse.y)) {
    return false;
  }

  int scrolledWidth = this->scrollWidth;
  int scrolledHeight = this->scrollHeight;

  if (event == ftxui::Event::ArrowUp || event == ftxui::Event::Character('k') ||
      (event.is_mouse() && mouse.button == ftxui::Mouse::WheelUp)) {
    if (this->direction == ScrollerDirection::WIDTH) {
      this->scrollWidth--;
    }
    this->scrollHeight--;
  }

  if (event == ftxui::Event::ArrowDown || event == ftxui::Event::Character('j') ||
      (event.is_mouse() && mouse.button == ftxui::Mouse::WheelDown)) {
    if (this->direction == ScrollerDirection::WIDTH) {
      this->scrollWidth++;
    }
    this->scrollHeight++;
  }

  if (event == ftxui::Event::ArrowLeft || event == ftxui::Event::Character('h')) {
    this->scrollWidth--;
  }

  if (event == ftxui::Event::ArrowRight || event == ftxui::Event::Character('l')) {
    this->scrollWidth++;
  }

  if (event == ftxui::Event::PageUp) {
    if (this->direction == ScrollerDirection::WIDTH) {
      this->scrollWidth -= this->box.x_max - box.x_min;
    }
    this->scrollHeight -= this->box.y_max - box.y_min;
  }

  if (event == ftxui::Event::PageDown) {
    if (this->direction == ScrollerDirection::WIDTH) {
      this->scrollWidth += this->box.x_max - box.x_min;
    }
    this->scrollHeight += this->box.y_max - box.y_min;
  }

  if (event == ftxui::Event::Home) {
    this->scrollWidth = 0;
    this->scrollHeight = 0;
  }

  if (event == ftxui::Event::End) {
    this->scrollWidth = this->sizeWidth;
    this->scrollHeight = this->sizeHeight;
  }

  this->scrollWidth = std::max(0, std::min(this->sizeWidth - 1, this->scrollWidth));
  this->scrollHeight = std::max(0, std::min(this->sizeHeight - 1, this->scrollHeight));

  return scrolledWidth != this->scrollWidth || scrolledHeight != this->scrollHeight;
}

bool Scroller::Focusable() const {
  return true;
}

}  // namespace tuwe::client