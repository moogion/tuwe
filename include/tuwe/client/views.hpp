#include <ftxui/component/component.hpp>
#include <memory>

#include "tuwe/client/client.hpp"

namespace tuwe::client {

class MainView : public ftxui::ComponentBase {
 private:
  const Application &app;

 public:
  explicit MainView(const Application &app);
  ftxui::Element Render();
};

}  // namespace tuwe::client