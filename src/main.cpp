#include <memory>  // for allocator, __shared_ptr_access, shared_ptr
#include <string>  // for string 
#include <stdlib.h>
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include <ftxui/screen/screen.hpp>  // for Pixel

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
    string usertoa = "null";
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
  string humid =to_string(humidity) + "%";

  //colours for text
  int colorH = 1;
  int colorS = 220;
  int colorV = 200;

  //SIZE OF 
  const int sizex = 20;
  const int sizey = 10;

  int counter = 0;

  //input components
  Component input_colortest = Input(&help, "Color");
  Component input_user = Input(&userText, "ENTER!: ");
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
    help += "	°";
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




  // auto renderer_line_braille = Renderer([&] {
  //   auto c = Canvas(100, 100);
  //   c.DrawText(0, 0, "Several lines (braille)");
  //   c.DrawPointLine(80, 10, 80, 40, Color::Blue);
  //   return canvas(std::move(c));
  // });


  auto canv = Canvas(30, 30);
  //canv.DrawText(0, 5, "Погода:");

  canv.DrawBlockLine(6,5, 29, 25, Color::LightCoral);
   


  auto style = ButtonOption::Animated(Color::Default, Color::DarkSlateGray1,
                                      Color::GrayDark, Color::White);

  Component humiditySlider =   Slider( "" , &humidity , 0 , 100 , 1 );
  Component buttonuser = Button("Enter " , wtf, style);

  auto Container1 = Container::Vertical({
    buttonuser,
    input_colortest,
    user_to_answer,
    input_add,
    
  });
  auto ContainerFuture = Container::Horizontal({

  });
auto future_renderer = Renderer(ContainerFuture, [&] {
    return vbox({
      hbox({
        filler(),
        hbox({
        filler(),
        text("sus:"),
        filler(),
        })|border,
        filler(),
        hbox({
          filler(),
        text("sus:"),
        filler(),
        })|border,
        filler(),
        hbox({
          filler(),
        text("sus:"),
        filler(),
        })|border,
        filler(),
        hbox({
          filler(),
        text("sus:"),
        filler(),
        })|border,
        filler(),
      }),
      filler(),
    });
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



    //виджеты
  auto box_rendener = Renderer(Container1, [&] {
  return vbox({

  
  //виджеты темп и влаж 
  hbox({
  

  vbox({
    
filler(),
    hbox({filler(),text("Температура"),filler(),}),


filler(),
  hbox({
  filler(),
    text(help),
  filler(),
  }),
    
    //humiditySlider->Render() ,
    //filler(),
    
  })|border|size(WIDTH, EQUAL, sizex)|size(HEIGHT, EQUAL, sizey),
  
  filler(),
  //виджет
  vbox({
    filler(),
  hbox({
    filler(),
    text("Влажность "),
    filler(),
    }),
    hbox({
    filler(),
    text(humid),
    filler(),
    }),
  filler(),
    filler(),
  humiditySlider->Render() ,

    filler(),
    
  //canv->Render(),
  })|border|size(WIDTH, EQUAL, sizex)|size(HEIGHT, EQUAL, sizey),
  
  }),
  filler(),

  hbox({        //weather
  filler(),
  vbox({
  filler(),
    
    hbox({filler(),text("Погода"),filler(),}),
    filler(),
    
    
  canvas(&canv),
  filler(),

  filler(),})|border|size(WIDTH, EQUAL, sizex)|size(HEIGHT, EQUAL, sizey),


  filler(),
  vbox({        //w
  filler(),
  hbox({
    filler(),
    text("Ветер" ) ,
    filler(),
  }),
  filler(),
  filler(),
  hbox({

    filler(),

    text("←") ,
    filler(),
  }),
  filler(),
  })|border|size(WIDTH, EQUAL, sizex)|size(HEIGHT, EQUAL, sizey),
  
  //canvas(&canv) | border,
  //filler(),
    
    
    
    
}),
   });
});

  auto next_rendener = Renderer(Container1, [&] {
    return vbox({

    hbox({
      
          color(Color::RosyBrown,text("TUWE — ")),
          color(Color::SkyBlue2 , text("Their Unbelievable Wisdom is Extraordinary")),
      filler(),
    }),

    // hbox({
    //   filler(),
    //         color(Color::SkyBlue2 , text("Their Unbelievable Wisdom is Extraordinary")),
    //   filler(),
      
    // }),

      separatorHSelector(0,1000,Color::White,Color::White),

    hbox({
        
        color(Color::HSV(colorH, colorS, colorV ),text("NIZHNY NOVGOROD")),
        filler(),  
      }),
    filler(), 
      separator(),
      future_renderer->Render(),
        //коробка для виджетов
        hbox({
          filler(), 
        box_rendener->Render(),
        filler(), 
        }),
      
      // box with color input
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
  separator(),
filler(),

    vbox({
      filler(),
      hbox({filler(),
      input_add->Render(),
      
      filler(),
      }),
      filler(),
      hbox({
      
      filler(),
      text(usertoa),
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