// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <memory>  // for allocator, __shared_ptr_access, shared_ptr
#include <string>  // for string 
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Button, Renderer, Vertical
#include "ftxui/component/component_options.hpp"   // for ButtonOption
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, text, Element, hbox, separator, size, vbox, border, frame, vscroll_indicator, HEIGHT, LESS_THAN
#include "ftxui/screen/color.hpp"  // for Color, Color::Default, Color::GrayDark, Color::White
#include "ftxui/component/component_base.hpp"  // for ComponentBase, Component
#include "ftxui/dom/elements.hpp"  // for operator|, separator, text, Element, flex, vbox, border
 
int main() {
  int g = 0;
  using namespace ftxui;
 //button
//   auto buttons = Container::Tab({
//     // Button(
//     //   "Krill", [&] ,ButtonOption::Animated(Color::Green)),
// });
  int h = 10;
  int s = 125;
  int v = 150;
  int counter = 0;
  auto colder = [&] {
    h += 10;
    //red -= 10; 
   };
  auto hotter = [&] { 
    h -= 10;
    //red += 10; 
    };
 // auto colorchange
  
  auto style = ButtonOption::Animated(Color::Default, Color::DarkSlateGray1,
                                      Color::GrayDark, Color::White);
 
  auto container = Container::Vertical({
    Button("Colder " , colder, style),
    Button("Hotter " , hotter, style),
  });
  // for (int i = 1; i < 2; ++i) {
  //   auto button = Button("Button " + std::to_string(i), on_click, style);
  //   container->Add(button);
  // }
 
  auto rendener = Renderer(container, [&] {
    return vbox({
              hbox({
                filler(),
               color(Color::Gold1,text("TUWE")),
               filler(),
              }),
              hbox({
                filler(),
               color(Color::SkyBlue2 , text("Their Unbelievable Wisdom is Extraordinary")),
               filler(),
              }),
               separatorHSelector(0,1000,Color::CadetBlueBis,Color::CadetBlueBis),
                              hbox({
                  filler(),
                  color(Color::HSV(h, s, v ),text("NIZHNY NOVGOROD")),
                  filler(),
               }),

               separator(),
               hbox({
                filler(),
               container->Render() | vscroll_indicator | frame |
                   size(HEIGHT, LESS_THAN, 40) ,
              // ButtonColder -> Render(),
                filler(),
              // ButtonHotter -> Render() ,
              // filler(),
               }),
               
           }) |
           borderStyled(DOUBLE, Color::CadetBlueBis);
  });
 
  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(rendener);
}