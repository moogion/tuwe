#include "tuwe/client/views.hpp"

#include <stdlib.h>

#include <ftxui/screen/screen.hpp>  // for Pixel
#include <memory>                   // for allocator, __shared_ptr_access, shared_ptr
#include <string>                   // for string

#include "ftxui/component/captured_mouse.hpp"      // for ftxui
#include "ftxui/component/component.hpp"           // for Button, Renderer, Vertical
#include "ftxui/component/component_base.hpp"      // for ComponentBase, Component
#include "ftxui/component/component_options.hpp"   // for ButtonOption
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"                  // for operator|, text, Element, hbox, separator, size, vbox, border, frame, vscroll_indicator, HEIGHT, LESS_THAN
#include "ftxui/dom/elements.hpp"                  // for operator|, separator, text, Element, flex, vbox, border
#include "ftxui/screen/color.hpp"                  // for Color, Color::Default, Color::GrayDark, Color::White
#include "tuwe/client/client.hpp"

using namespace std;
using namespace ftxui;
namespace tuwe::client {

MainView::MainView(const Application &app) : app(app) {
}

ftxui::Element MainView::Render() {
  // Data
  Element document;
  int g = 0;
  string userEntersLetters = "null";
  // Strings to something
  string colortest = "50";
  string temperatureString = "";

  int humidity = 80;
  string humidityString = to_string(humidity) + "%";

  // colours for text
  int colorH = 100;
  int colorS = 220;
  int colorV = 200;

  // func for scaling(USELESS NOW)

  int sizeg = Terminal::Size().dimy;
  int sizef = Terminal::Size().dimx;
  string sizew = to_string(sizeg);
  auto funcsize = [&] {
    int sizeg = Terminal::Size().dimy;
    string sizew = to_string(sizeg);
  };

  // func for scaling(USELESS NOW)

  // SIZE OF
  int sizex = sizef / 3;
  int sizey = sizeg / 3;

  int counter = 0;

  // input components
  Component input_color_textbox = Input(&temperatureString, "Color");

  Component user_enters_letters = Input(&userEntersLetters, "Enter your request: ");

  // Enter string on pressed button 
  auto input_option = InputOption();
  string input_add_content;
  input_option.on_enter = [&] {
    userEntersLetters = input_add_content;
    input_add_content = " ";
  };
  Component input_add =
      Input(&input_add_content, "Enter your request: ", input_option);

  // city name color changing func
  auto color_change = [&] {
    int temp = stoi(temperatureString);
    temperatureString += "	°";
    if (temp <= 0) {
      colorH = 120 + 0.8 * abs(temp);
    } else {
      colorH = 287 - 0.8 * abs(temp);
    }
  };

  // Canva testing

  // auto renderer_line_braille = Renderer([&] {
  //   auto c = Canvas(100, 100);
  //   c.DrawText(0, 0, "Several lines (braille)");
  //   c.DrawPointLine(80, 10, 80, 40, Color::Blue);
  //   return canvas(std::move(c));
  // });

  auto canv = Canvas(30, 30);

  canv.DrawBlockLine(6, 5, 29, 25, Color::LightCoral);

  auto buttonStyle = ButtonOption::Animated(Color::Default, Color::DarkSlateGray1, Color::GrayDark, Color::White);

  Component humiditySlider = Slider("", &humidity, 0, 100, 1);
  Component buttonForColourEnter = Button("Enter ", funcsize, buttonStyle);

  auto Container1 = Container::Vertical({
      buttonForColourEnter,
      input_color_textbox,
      user_enters_letters,
      input_add,

  });

  auto screen = ScreenInteractive::Fullscreen();

  auto ContainerFutureWeather = Container::Horizontal({

  });
  auto future_weather_renderer = Renderer(ContainerFutureWeather, [&] {
    return vbox({
        hbox({
            filler(),
            hbox({
                filler(),
                text("" + sizew),
                filler(),
            }) | border,
            filler(),
            hbox({
                filler(),
                text(to_string(sizeg)),
                filler(),
            }) | border,
            filler(),
            hbox({
                filler(),
                text("sus:"),
                filler(),
            }) | border,
            filler(),
            hbox({
                filler(),
                text("sus:"),
                filler(),
            }) | border,
            filler(),
        }),
        filler(),
    });
  });

  // виджеты
  auto widget_rendener = Renderer(Container1, [&] {
    return vbox({

        // виджеты темп и влаж
        hbox({

            vbox({

                filler(),
                hbox({
                    filler(),
                    text("Температура"),
                    filler(),
                }),

                filler(),
                hbox({
                    filler(),
                    text(temperatureString),
                    filler(),
                }),

            }) | border |
                size(WIDTH, EQUAL, sizex) | size(HEIGHT, EQUAL, sizey),

            filler(),
            // виджет
            vbox({
                filler(),
                hbox({
                    filler(),
                    text("Влажность "),
                    filler(),
                }),
                hbox({
                    filler(),
                    text(humidityString),
                    filler(),
                }),
                filler(),
                filler(),
                humiditySlider->Render(),

                filler(),

            }) | border |
                size(WIDTH, EQUAL, sizex) | size(HEIGHT, EQUAL, sizey),

        }),
        filler(),

        hbox({
            // weather
            filler(),
            vbox({
                filler(),

                hbox({
                    filler(),
                    text("Погода"),
                    filler(),
                }),
                filler(),

                canvas(&canv),
                filler(),

                filler(),
            }) | border |
                size(WIDTH, EQUAL, sizex) | size(HEIGHT, EQUAL, sizey),

            filler(),
            vbox({
                // w
                filler(),
                hbox({
                    filler(),
                    text("Ветер"),
                    filler(),
                }),
                filler(),
                filler(),
                hbox({

                    filler(),

                    text("←"),
                    filler(),
                }),
                filler(),
            }) | border |
                size(WIDTH, EQUAL, sizex) | size(HEIGHT, EQUAL, sizey),

        }),
    });
  });
  //оставил next_rendener пока что что бы не путаться
  auto next_rendener = Renderer(Container1, [&] {
    return vbox({

               hbox({

                   color(Color::RosyBrown, text("TUWE — ")),
                   color(Color::SkyBlue2, text("Their Unbelievable Wisdom is Extraordinary")),
                   filler(),
               }),

               separatorHSelector(0, 1000, Color::White, Color::White),

               hbox({

                   color(Color::HSV(colorH, colorS, colorV), text("NIZHNY NOVGOROD")),
                   filler(),
               }),
               filler(),
               separator(),
               future_weather_renderer->Render(),
               // коробка для виджетов
               hbox({
                   filler(),
                   widget_rendener->Render(),
                   filler(),
               }),

               // box with color input(useless later)
               hbox({
                   filler(),
                   vbox({
                       filler(),
                       buttonForColourEnter->Render() | vscroll_indicator | frame,
                       filler(),
                       input_color_textbox->Render(),
                       filler(),
                   }),
                   filler(),
               }),
               filler(),
               separator(),
               filler(),

               vbox({
                   filler(),
                   hbox({
                       filler(),
                       input_add->Render(),

                       filler(),
                   }),
                   filler(),
                   hbox({

                       filler(),
                       text(userEntersLetters),
                       filler(),
                   }),
                   filler(),
               }),
               filler(),
               text(""),
           }) |
           borderStyled(DOUBLE, Color::White);
  });

  auto mainContainer = Container::Vertical({
      next_rendener,

  });

  auto main_rendener = Renderer(mainContainer, [&] {
    // Element document = input_renderer -> Render();
    // lazy dont care
    // Element document = next_rendener->Render();
    // if (userText == "Нижний"){
    //   document = next_rendener -> Render();
    // }
    // int sizeg = Terminal::Size().dimy;
    // int sizef = Terminal::Size().dimx;

    return mainContainer->Render();
  });

  screen.Loop(main_rendener);

  return mainContainer->Render();
  ;
}

}  // namespace tuwe::client