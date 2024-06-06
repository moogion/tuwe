#include <stdlib.h>

#include <ftxui/screen/screen.hpp>  // for Pixel
#include <memory>  // for allocator, __shared_ptr_access, shared_ptr
#include <string>  // for string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Button, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase, Component
#include "ftxui/component/component_options.hpp"   // for ButtonOption
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, text, Element, hbox, separator, size, vbox, border, frame, vscroll_indicator, HEIGHT, LESS_THAN
#include "ftxui/dom/elements.hpp"  // for operator|, separator, text, Element, flex, vbox, border
#include "ftxui/screen/color.hpp"  // for Color, Color::Default, Color::GrayDark, Color::White
#include "tuwe/client/client.hpp"



int main() {
  // TODO: add parsing arguments for application options
  tuwe::client::Application application;

  return application.execute();
}