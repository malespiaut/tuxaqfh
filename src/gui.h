
class GUI
{
  class puMenuBar *main_menu_bar ;

  int hidden ;

public:

  GUI () ;
  void update () ;
  void show   () ;
  void hide   () ;
  int  isHidden () { return hidden ; }

  void keyboardInput () ;
  void joystickInput () ;
} ;


int getGLUTKeystroke () ;

extern fntTexFont *font ;

