#include <memory>  // for allocator, __shared_ptr_access, shared_ptr
#include <string>  // for string 
#include <stdlib.h>
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Button, Renderer, Vertical
#include "ftxui/component/component_options.hpp"   // for ButtonOption
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, text, Element, hbox, separator, size, vbox, border, frame, vscroll_indicator, HEIGHT, LESS_THAN
#include "ftxui/screen/color.hpp"  // for Color, Color::Default, Color::GrayDark, Color::White
#include "ftxui/component/component_base.hpp"  // for ComponentBase, Component
#include "ftxui/dom/elements.hpp"  // for operator|, separator, text, Element, flex, vbox, border
using namespace std;
using namespace ftxui; 
 
class MainWindow{
  public:
    //Data
    string usertoa = "fuck";
  void dataReturn(){
    cout << usertoa << endl;
  }
  void windowBodyRender(){
  int g = 0;

  //Strings to something
  string colortest = "50";
  string help = "";
  string userText = "";
  

  int humidity = 80;

  //colours for text
  int colorH = 1;
  int colorS = 220;
  int colorV = 200;

  //SIZE OF 
  const int sizex = 20;
  const int sizey = 3;

  int counter = 0;

  //input components
  Component input_colortest = Input(&help, "Color");
  Component input_user = Input(&userText, "ENTER: ");
  Component user_to_answer = Input(&usertoa, "Enter your request: ");
  //Component answer_area = 
  
  auto input_option = InputOption();
  string input_add_content;
  input_option.on_enter = [&] {
    //input_entries.push_back(input_add_content);
    usertoa = input_add_content;
    input_add_content = " ";
  };
  Component input_add = Input(&input_add_content, "Enter your request: ", input_option);

  //city name color changing func
  auto wtf = [&]{
    int temp = stoi(help);
    if (temp<=0){
      colorH = 120 + 0.8 * abs(temp);
    }
    else{
      colorH = 287 - 0.8 * abs(temp);
    }
  };

  auto InputBox = Container::Vertical({
    input_user,
  });

  auto style = ButtonOption::Animated(Color::Default, Color::DarkSlateGray1,
                                      Color::GrayDark, Color::White);

  Component humiditySlider =   Slider( "Влажность: " , &humidity , 0 , 100 , 1 );
  Component buttonuser = Button("Enter " , wtf, style);

  auto Container1 = Container::Vertical({
    buttonuser,
    input_colortest,
    user_to_answer,
    input_add,
  });

  auto input_renderer = Renderer(InputBox, [&] {
    return vbox({
      vbox({
        hbox({
          filler(),
          text("Enter:"),
          filler(),
         }),
        hbox({
          filler(),
          input_user -> Render(),
          filler(),
        }),
      }),
    });
  });

  auto next_rendener = Renderer(Container1, [&] {
    return vbox({

    hbox({
      filler(),
          color(Color::NavajoWhite1,text("TUWE")),
      filler(),
    }),

    hbox({
      filler(),
            color(Color::SkyBlue2 , text("Their Unbelievable Wisdom is Extraordinary")),
      filler(),
      
    }),

      separatorHSelector(0,1000,Color::White,Color::White),

    hbox({
        filler(),
        color(Color::HSV(colorH, colorS, colorV ),text("NIZHNY NOVGOROD"))|border,
        filler(),  
      }),

      separator(),

      hbox({
      vbox({
        filler(),
        text("Температура: " + help),
        filler(),
        //humiditySlider->Render() ,
        //filler(),
        
      })|border|size(WIDTH, EQUAL, sizex)|size(HEIGHT, EQUAL, sizey),

      vbox({
      filler(),
        
        humiditySlider->Render() ,
        filler(),
      })|border|size(WIDTH, EQUAL, sizex)|size(HEIGHT, EQUAL, sizey),

      }),

    hbox({
      filler(),
      vbox({
      filler(),
      buttonuser->Render() | vscroll_indicator | frame ,
      filler(),
      input_colortest->Render(),
      filler(),
      }),                
      filler(),
    }),

  filler(),

    hbox({
      filler(),
      text("" + usertoa),
      filler(),
      input_add->Render(),
      filler(),
    }),
  }) |
  borderStyled(DOUBLE, Color::White);
  });

  auto mainContainer = Container::Vertical({
    next_rendener,
    input_renderer,
  });

  auto main_rendener = Renderer(mainContainer, [&] {
    Element document = input_renderer -> Render();
    //lazy dont care
    document = next_rendener -> Render();
    // if (userText == "Нижний"){
    //   document = next_rendener -> Render();
    // }
    return document;
  });
 
  
  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(main_rendener);
  }
};


int main() {
  // Window1 * Window2 = new Window1();

  // Window2->windowEx();


  shared_ptr<MainWindow> window(new MainWindow());

  window->windowBodyRender();
  //window->dataReturn();
  
}