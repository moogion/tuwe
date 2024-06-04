#include "tuwe/client/views.hpp"

namespace tuwe::client {

MainView::MainView(const Application &app) : app(app) {
}

ftxui::Element MainView::Render() {
  return ftxui::text("TODO: Edit");
}

}  // namespace tuwe::client