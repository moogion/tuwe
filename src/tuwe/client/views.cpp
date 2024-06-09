#include "tuwe/client/views.hpp"

#include <ftxui/component/component.hpp>
#include <string>

#include "tuwe/client/components.hpp"

namespace tuwe::client {
MainView::MainView(const Application &app) : app(app) {
  const std::function<ftxui::Element()> render = std::bind(&MainView::renderView, this);
  this->scroller = std::make_shared<Scroller>(
      ftxui::Renderer(render),
      ScrollerDirection::HIGHT
  );
  this->Add(this->scroller);

  ftxui::InputOption inputOptions = ftxui::InputOption::Default();
  inputOptions.multiline = false;
  inputOptions.placeholder = "Введите город или индекс местоположения";

  this->locationInput = ftxui::Input(inputOptions);
  this->Add(this->locationInput);

  ftxui::ButtonOption locationButtonOptions{
      .transform = [](const ftxui::EntryState &state) -> ftxui::Element {
        return ftxui::text(state.label) | (state.focused ? ftxui::bgcolor(ftxui::Color::GrayLight) : ftxui::nothing);
      },
  };
  this->locationButton = ftxui::Button(
      "🔍", [] {}, locationButtonOptions
  );
  this->Add(this->locationButton);

  const std::function<ftxui::Element()> renderHourly = std::bind(&MainView::renderHourly, this);
  this->scrollerHourly = std::make_shared<Scroller>(
      ftxui::Renderer(renderHourly),
      ScrollerDirection::WIDTH
  );
  this->Add(this->scrollerHourly);
}

ftxui::Element MainView::Render() {
  return this->scroller->Render();
}

ftxui::Element MainView::renderView() {
  ftxui::Element viewDescription = ftxui::hbox({
      ftxui::color(ftxui::Color::RosyBrown, ftxui::text("TUWE — ")),
      ftxui::color(ftxui::Color::SkyBlue2, ftxui::text("Terminal User Weather Explorer")),
  });
  viewDescription |= ftxui::borderEmpty;

  ftxui::Element locationInput = ftxui::hbox({
      ftxui::separatorEmpty(),
      ftxui::text("🚩"),
      ftxui::separatorEmpty(),
      this->locationInput->Render(),
      ftxui::separatorEmpty(),
      this->locationButton->Render(),
      ftxui::separatorEmpty(),
  });
  locationInput |= ftxui::borderRounded;

  ftxui::Element weatherNow = ftxui::vbox({
      ftxui::text("Сейчас"),
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
                      ftxui::text("Макс.:"),
                      ftxui::separatorEmpty(),
                      ftxui::text("19℃"),
                      ftxui::separatorEmpty(),
                  }),
                  ftxui::separatorEmpty(),
                  ftxui::hbox({
                      ftxui::separatorEmpty(),
                      ftxui::text("Мин.:"),
                      ftxui::separatorEmpty(),
                      ftxui::text("4℃"),
                  }),
              }),
          }),
          ftxui::filler() | ftxui::flex,
          ftxui::vbox({
              ftxui::text("Более солнечно"),
              ftxui::text("Ощущается как 18℃"),
          }),
      }),
  });
  weatherNow |= ftxui::borderEmpty;

  ftxui::Element weatherMessage = ftxui::hbox({
      ftxui::separatorEmpty(),
      ftxui::text("❄"),
      ftxui::separatorEmpty(),
      ftxui::separatorEmpty(),
      ftxui::paragraph("Сегодняшняя температура будет на 4℃ ниже, чем вчера."),
      ftxui::separatorEmpty(),
  });
  weatherMessage |= ftxui::borderRounded;

  ftxui::Element weatherHourly = ftxui::vbox({
      ftxui::text("Часовая сводка") | ftxui::borderEmpty,
      this->scrollerHourly->Render() | ftxui::borderRounded,
  });

  ftxui::Elements wearherConditions{
      ftxui::vbox({
          ftxui::text("Ветер"),
          ftxui::separatorEmpty(),
          ftxui::hbox({
              ftxui::vbox({
                  ftxui::text("14 км/ч"),
                  ftxui::text("Легкий · С севера"),
              }) | ftxui::vcenter,
              ftxui::separatorEmpty(),
              ftxui::filler(),
              ftxui::vbox({
                  ftxui::text("N") | ftxui::hcenter,
                  ftxui::text("🡓") | ftxui::center | ftxui::size(ftxui::WidthOrHeight::WIDTH, ftxui::Constraint::GREATER_THAN, 3) | ftxui::borderRounded | ftxui::hcenter,
              }),
          }),
      }),
      ftxui::vbox({
          ftxui::text("Влажность"),
          ftxui::separatorEmpty(),
          ftxui::hbox({
              ftxui::vbox({
                  ftxui::text("27%"),
                  ftxui::text("Температура конденсации -1℃"),
              }) | ftxui::vcenter,
              ftxui::filler(),
              ftxui::vbox({
                  ftxui::text("100") | ftxui::hcenter,
                  ftxui::hbox({
                      ftxui::gaugeUp(0.27),
                      ftxui::gaugeUp(0.27),
                      ftxui::gaugeUp(0.27),
                  }) | ftxui::size(ftxui::WidthOrHeight::HEIGHT, ftxui::Constraint::GREATER_THAN, 2) |
                      ftxui::borderLight,
                  ftxui::text("0") | ftxui::hcenter,
              }),
              ftxui::separatorEmpty(),
          }),
      }),
      ftxui::vbox({
          ftxui::text("UV индекс"),
          ftxui::separatorEmpty(),
          ftxui::hbox({
              ftxui::vbox({
                  ftxui::text("5"),
                  ftxui::text("Терпимо"),
              }) | ftxui::vcenter,
              ftxui::filler(),
              ftxui::vbox({
                  ftxui::text("11+") | ftxui::hcenter,
                  ftxui::hbox({
                      ftxui::gaugeUp(0.27),
                      ftxui::gaugeUp(0.27),
                      ftxui::gaugeUp(0.27),
                  }) | ftxui::size(ftxui::WidthOrHeight::HEIGHT, ftxui::Constraint::GREATER_THAN, 2) |
                      ftxui::borderDashed,
                  ftxui::text("0") | ftxui::hcenter,
              }),
              ftxui::separatorEmpty(),
          }),
      }),
      ftxui::vbox({
          ftxui::text("Давление"),
          ftxui::separatorEmpty(),
          ftxui::hbox({
              ftxui::vbox({
                  ftxui::text("1,021"),
                  ftxui::text("мбар"),
              }) | ftxui::vcenter,
              ftxui::filler(),
              ftxui::vbox({
                  ftxui::text("Высок") | ftxui::hcenter,
                  ftxui::hbox({
                      ftxui::gaugeUp(0.27),
                      ftxui::gaugeUp(0.27),
                      ftxui::gaugeUp(0.27),
                  }) | ftxui::size(ftxui::WidthOrHeight::HEIGHT, ftxui::Constraint::GREATER_THAN, 2) |
                      ftxui::borderDouble,
                  ftxui::text("Низк") | ftxui::hcenter,
              }),
              ftxui::separatorEmpty(),
          }),
      }),
  };

  for (ftxui::Element &element : wearherConditions) {
    element |= ftxui::borderEmpty;
    element |= ftxui::borderRounded;
    element |= ftxui::flex;
  }

  ftxui::Dimensions terminalSize = ftxui::Terminal::Size();

  ftxui::Element weather = ftxui::vbox({
      ftxui::text("Текущие условия") | ftxui::borderEmpty,
      terminalSize.dimx < 120 ? ftxui::vbox(wearherConditions) : ftxui::hbox(wearherConditions),

  });

  return ftxui::vbox({
             viewDescription,
             locationInput,
             weatherNow,
             weatherMessage,
             weatherHourly,
             weather,
         }) |
         ftxui::borderEmpty;
}

ftxui::Element MainView::renderHourly() {
  return ftxui::hbox({
      ftxui::separatorEmpty(),
      ftxui::vbox({
          ftxui::text("18℃") | ftxui::hcenter,
          ftxui::separatorEmpty(),
          ftxui::text("🌤 ") | ftxui::hcenter,
          ftxui::text("12:00") | ftxui::hcenter,
      }) | ftxui::borderEmpty,
      ftxui::separatorEmpty(),
      ftxui::vbox({
          ftxui::text("18℃") | ftxui::hcenter,
          ftxui::separatorEmpty(),
          ftxui::text("🌤 ") | ftxui::hcenter,
          ftxui::text("12:00") | ftxui::hcenter,
      }) | ftxui::borderEmpty,
      ftxui::separatorEmpty(),
      ftxui::vbox({
          ftxui::text("18℃") | ftxui::hcenter,
          ftxui::separatorEmpty(),
          ftxui::text("🌤 ") | ftxui::hcenter,
          ftxui::text("12:00") | ftxui::hcenter,
      }) | ftxui::borderEmpty,
      ftxui::separatorEmpty(),
      ftxui::vbox({
          ftxui::text("18℃") | ftxui::hcenter,
          ftxui::separatorEmpty(),
          ftxui::text("🌤 ") | ftxui::hcenter,
          ftxui::text("12:00") | ftxui::hcenter,
      }) | ftxui::borderEmpty,
      ftxui::separatorEmpty(),
      ftxui::vbox({
          ftxui::text("18℃") | ftxui::hcenter,
          ftxui::separatorEmpty(),
          ftxui::text("🌤 ") | ftxui::hcenter,
          ftxui::text("12:00") | ftxui::hcenter,
      }) | ftxui::borderEmpty,
      ftxui::separatorEmpty(),
      ftxui::vbox({
          ftxui::text("18℃") | ftxui::hcenter,
          ftxui::separatorEmpty(),
          ftxui::text("🌤 ") | ftxui::hcenter,
          ftxui::text("12:00") | ftxui::hcenter,
      }) | ftxui::borderEmpty,
      ftxui::separatorEmpty(),
      ftxui::vbox({
          ftxui::text("18℃") | ftxui::hcenter,
          ftxui::separatorEmpty(),
          ftxui::text("🌤 ") | ftxui::hcenter,
          ftxui::text("12:00") | ftxui::hcenter,
      }) | ftxui::borderEmpty,
      ftxui::separatorEmpty(),
      ftxui::vbox({
          ftxui::text("18℃") | ftxui::hcenter,
          ftxui::separatorEmpty(),
          ftxui::text("🌤 ") | ftxui::hcenter,
          ftxui::text("12:00") | ftxui::hcenter,
      }) | ftxui::borderEmpty,
      ftxui::separatorEmpty(),
  });
}

}  // namespace tuwe::client