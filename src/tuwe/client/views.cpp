#include "tuwe/client/views.hpp"

#include <ftxui/component/component.hpp>
#include <string>

namespace tuwe::client {
MainView::MainView(const Application &app) : app(app) {
  ftxui::InputOption inputOptions{
      .placeholder = "Ваш город?",
      .transform = [](const ftxui::InputState state) {
        return state.element;
      },
      .multiline = false,
  };
  this->locationInput = ftxui::Input(inputOptions);

  this->Add(this->locationInput);
}

ftxui::Element MainView::Render() {
  //int humidity = 50;
  //std::string humidityString = std::to_string(humidity) + "%";
  //ftxui::Component humiditySlider = ftxui::Slider("", &humidity, 0, 100, 1);
  auto ContainerBlocks = ftxui::Container::Horizontal({

  });

  auto container_general_conditions = ftxui::Container::Horizontal({

  });

  auto blocks = ftxui::Renderer(ContainerBlocks, [&] {
    return ftxui::vbox({
        ftxui::hbox({

        }),

    });
  });

  auto block_hourly_forecast = ftxui::Renderer(ContainerBlocks, [&] {
    return ftxui::vbox({
        ftxui::text(" Часовая сводка"),
        ftxui::hbox({
            ftxui::filler(),
            ftxui::vbox({
                ftxui::text("18℃"),

                ftxui::text("🌤"),
                ftxui::text("12:00"),

            }),
            ftxui::filler(),
            ftxui::vbox({
                ftxui::text("18℃"),

                ftxui::text("🌨"),
                ftxui::text("13:00"),

            }),
            ftxui::filler(),
            ftxui::vbox({
                ftxui::text("18℃"),

                ftxui::text("🌧"),
                ftxui::text("14:00"),

            }),
            ftxui::filler(),
            ftxui::vbox({
                ftxui::text("18℃"),

                ftxui::text("🌤"),
                ftxui::text("15:00"),

            }),
            ftxui::filler(),
        }) | ftxui::borderRounded,

    });
  });
  auto block_general_conditions = ftxui::Renderer(ContainerBlocks, [&] {
    return ftxui::vbox({

        // первые два

        ftxui::hbox({
            ftxui::hbox({
                ftxui::filler(),
                ftxui::vbox({

                    ftxui::hbox({
                        ftxui::filler(),
                        ftxui::text("Влажность "),
                        ftxui::filler(),
                    }),
                    ftxui::hbox({
                        ftxui::filler(),
                        ftxui::text("80%"),
                        ftxui::filler(),
                    }),

                    border(ftxui::gauge(0.8)) | ftxui::flex,

                }),
                ftxui::filler(),
            }) | ftxui::border |
                ftxui::flex,
            ftxui::hbox({
                ftxui::filler(),
                ftxui::vbox({

                    ftxui::filler(),
                    ftxui::hbox({
                        ftxui::filler(),
                        ftxui::text("Ветер"),
                        ftxui::filler(),
                    }),
                    ftxui::filler(),
                    ftxui::filler(),
                    ftxui::hbox({

                        ftxui::filler(),

                        ftxui::text("←"),
                        ftxui::filler(),
                    }),
                    ftxui::filler(),
                }),
                ftxui::filler(),
            }) | ftxui::border |
                ftxui::flex,

        }),

        ftxui::hbox({

            ftxui::vbox({

                ftxui::filler(),
                ftxui::hbox({
                    ftxui::filler(),
                    ftxui::text("Ветер"),
                    ftxui::filler(),
                }),
                ftxui::filler(),
                ftxui::filler(),
                ftxui::hbox({

                    ftxui::filler(),

                    ftxui::text("←"),
                    ftxui::filler(),
                }),

            }) | ftxui::border |
                ftxui::flex,

            ftxui::vbox({

                ftxui::filler(),
                ftxui::hbox({
                    ftxui::filler(),
                    ftxui::text("Ветер"),
                    ftxui::filler(),
                }),
                ftxui::filler(),
                ftxui::filler(),
                ftxui::hbox({

                    ftxui::filler(),

                    ftxui::text("←"),
                    ftxui::filler(),
                }),
                ftxui::filler(),
            }) | ftxui::border |
                ftxui::flex,

        }) | ftxui::flex,

    });
  });
  return ftxui::vbox({
             ftxui::hbox({
                 ftxui::color(ftxui::Color::RosyBrown, ftxui::text("TUWE — ")),
                 ftxui::color(ftxui::Color::SkyBlue2, ftxui::text("Terminal User Weather Explorer")),
             }) |
                 ftxui::borderEmpty,
             ftxui::hbox({
                 ftxui::separatorEmpty(),
                 ftxui::text("🚩"),
                 ftxui::separatorEmpty(),
                 this->locationInput->Render(),
                 ftxui::separatorEmpty(),
             }) | ftxui::borderRounded,
             ftxui::vbox({
                 ftxui::text("Now"),
                 ftxui::hbox({
                     ftxui::vbox({
                         ftxui::hbox({
                             ftxui::text("18℃"),
                             ftxui::separatorEmpty(),
                             ftxui::separatorEmpty(),
                             ftxui::text("🌤"),
                         }) | ftxui::borderEmpty,
                         ftxui::hbox({
                             ftxui::hbox({
                                 ftxui::text("High:"),
                                 ftxui::separatorEmpty(),
                                 ftxui::text("19℃"),
                                 ftxui::separatorEmpty(),
                             }),
                             ftxui::separatorEmpty(),
                             ftxui::hbox({
                                 ftxui::separatorEmpty(),
                                 ftxui::text("Low:"),
                                 ftxui::separatorEmpty(),
                                 ftxui::text("4℃"),
                             }),
                         }),
                     }),
                     ftxui::filler() | ftxui::flex,
                     ftxui::vbox({
                         ftxui::text("Mostly sunny"),
                         ftxui::text("Feels like 18℃"),
                     }) | ftxui::borderEmpty,
                 }),
             }) | ftxui::borderEmpty,
             block_hourly_forecast->Render(),
             block_general_conditions->Render(),
             ftxui::hbox({
                 ftxui::text("❄ ") | ftxui::borderEmpty,
                 ftxui::separatorEmpty(),
                 ftxui::text("Today's temperature will be 40 lower than yesterday") | ftxui::vcenter,

             }) | ftxui::borderRounded,

         }) |
         ftxui::borderEmpty;
}

}  // namespace tuwe::client