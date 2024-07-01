#include "tuwe/client/components.hpp"

#include <ftxui/component/event.hpp>
#include <ftxui/component/mouse.hpp>
#include <memory>

namespace tuwe::client {

ScrollerComponent MakeScroller(ftxui::Component child, const ScrollerDirection direction) {
  return std::make_shared<Scroller>(std::move(child), direction);
}

Scroller::Scroller(ftxui::Component child, const ScrollerDirection direction)
    : scroll_width_(0), scroll_height_(0) {
  this->Add(child);
  this->direction_ = std::move(direction);
}

ftxui::Element Scroller::Render() {
  ftxui::Element background = ftxui::ComponentBase::Render();
  background->ComputeRequirement();

  const int focusOffsetWidth = this->box_.x_max - this->box_.x_min - 1;
  const int focusOffsetHeight = this->box_.y_max - this->box_.y_min - 1;

  const ftxui::Requirement requirement = background->requirement();
  this->size_width_ = requirement.min_x - focusOffsetWidth;
  this->size_height_ = requirement.min_y - focusOffsetHeight;

  background |= ftxui::focusPosition(
      focusOffsetWidth / 2 + this->scroll_width_,
      focusOffsetHeight / 2 + this->scroll_height_
  );

  switch (this->direction_) {
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

  return std::move(background) | ftxui::reflect(this->box_);
}

bool Scroller::OnEvent(ftxui::Event event) {
  if (ftxui::ComponentBase::OnEvent(event)) {
    return true;
  }

  ftxui::Mouse& mouse = event.mouse();

  if (event.is_mouse() && !box_.Contain(mouse.x, mouse.y)) {
    return false;
  }

  int scrolledWidth = this->scroll_width_;
  int scrolledHeight = this->scroll_height_;

  if (event == ftxui::Event::ArrowUp || event == ftxui::Event::Character('k') ||
      (event.is_mouse() && mouse.button == ftxui::Mouse::WheelUp)) {
    if (this->direction_ == ScrollerDirection::WIDTH) {
      this->scroll_width_--;
    }
    this->scroll_height_--;
  }

  if (event == ftxui::Event::ArrowDown || event == ftxui::Event::Character('j') ||
      (event.is_mouse() && mouse.button == ftxui::Mouse::WheelDown)) {
    if (this->direction_ == ScrollerDirection::WIDTH) {
      this->scroll_width_++;
    }
    this->scroll_height_++;
  }

  if (event == ftxui::Event::ArrowLeft || event == ftxui::Event::Character('h')) {
    this->scroll_width_--;
  }

  if (event == ftxui::Event::ArrowRight || event == ftxui::Event::Character('l')) {
    this->scroll_width_++;
  }

  if (event == ftxui::Event::PageUp) {
    if (this->direction_ == ScrollerDirection::WIDTH) {
      this->scroll_width_ -= this->box_.x_max - box_.x_min;
    }
    this->scroll_height_ -= this->box_.y_max - box_.y_min;
  }

  if (event == ftxui::Event::PageDown) {
    if (this->direction_ == ScrollerDirection::WIDTH) {
      this->scroll_width_ += this->box_.x_max - box_.x_min;
    }
    this->scroll_height_ += this->box_.y_max - box_.y_min;
  }

  if (event == ftxui::Event::Home) {
    this->scroll_width_ = 0;
    this->scroll_height_ = 0;
  }

  if (event == ftxui::Event::End) {
    this->scroll_width_ = this->size_width_;
    this->scroll_height_ = this->size_height_;
  }

  this->scroll_width_ = std::max(0, std::min(this->size_width_ - 1, this->scroll_width_));
  this->scroll_height_ = std::max(0, std::min(this->size_height_ - 1, this->scroll_height_));

  return scrolledWidth != this->scroll_width_ || scrolledHeight != this->scroll_height_;
}

bool Scroller::Focusable() const {
  return true;
}

}  // namespace tuwe::client