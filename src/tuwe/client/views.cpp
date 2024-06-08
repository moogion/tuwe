#include "tuwe/client/views.hpp"

#include <ftxui/component/component.hpp>

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
             ftxui::hbox({
                 ftxui::text("❄ ") | ftxui::borderEmpty,
                 ftxui::separatorEmpty(),
                 ftxui::text("Today's temperature will be 40 lower than yesterday") | ftxui::vcenter,
             }) | ftxui::borderRounded,
         }) |
         ftxui::borderEmpty;
}

}  // namespace tuwe::client