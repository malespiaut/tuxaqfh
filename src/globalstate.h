class GlobalState
{
public:
  int new_level ;
  int level ;
  int show_menu ;
  int freeze_baddies ;
  int cheat_mode ;
  int score ;
  int cant_breathe ;
  float heating ;
  int texture_enable ;
  int wireframe_enable ;
  int debug_enable ;

  int pause ;

  int kludge_mode ;
  int kludge_char ;

  GlobalState ()
  {
    heating = 0.0f ;
    pause = FALSE ;
    kludge_mode  = FALSE ;
    kludge_char  = 0 ;
    cant_breathe = FALSE ;
    new_level = level =  -1  ;
    score            =   0   ;
    show_menu        = FALSE ;
    freeze_baddies   = FALSE ;
    cheat_mode       = FALSE ;
    texture_enable   = TRUE  ;
    wireframe_enable = FALSE ;
    debug_enable     = FALSE ;
  }

  void keyboardInput ( int c ) ;
} ;

