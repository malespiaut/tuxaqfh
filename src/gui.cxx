
#include "tux.h"
#include "plib/pu.h"

static jsJoystick *joystick ;
static int mouse_x ;
static int mouse_y ;
static int mouse_dx = 0 ;
static int mouse_dy = 0 ;
static int mouse_buttons = 0 ;
static int mousetrap = TRUE ;
static int prefer_joystick = TRUE ;
static int CD_hack = FALSE ;

fntTexFont *font ;

static void motionfn ( int x, int y )
{
  mouse_x = x ;
  mouse_y = y ;
  mouse_dx += mouse_x - 320 ;
  mouse_dy += mouse_y - 240 ;
  puMouse ( x, y ) ;
}

static void mousefn ( int button, int updown, int x, int y )
{
  mouse_x = x ;
  mouse_y = y ;

  if ( updown == GLUT_DOWN )
    mouse_buttons |= (1<<button) ;
  else
    mouse_buttons &= ~(1<<button) ;

  mouse_dx += mouse_x - 320 ;
  mouse_dy += mouse_y - 240 ;

  puMouse ( button, updown, x, y ) ;

  if ( updown == GLUT_DOWN )
    hide_status () ;
}

static void credits_cb ( puObject * )
{
  hide_status () ;
  credits () ;
}

static void versions_cb ( puObject * )
{
  hide_status () ;
  versions () ;
}

static void about_cb ( puObject * )
{
  hide_status () ;
  about () ;
}

static void help_cb ( puObject * )
{
  hide_status () ;
  help () ;
}

#ifdef SOME_BETTER_WAY_TO_DO_THIS

#include <GL/xmesa.h>

#ifdef XMESA_FX_FULLSCREEN
  static int fullscreen = FALSE ;
  static void setFullscreen () { XMesaSetFXmode ( fullscreen ? XMESA_FX_FULLSCREEN : XMESA_FX_WINDOW ) ; } 
  static void window_off_cb ( puObject * ) { mousetrap = TRUE  ; fullscreen = TRUE  ; setFullscreen () ; } 
  static void window_on_cb  ( puObject * ) { mousetrap = FALSE ; fullscreen = FALSE ; setFullscreen () ; } 
#else
  static void setFullscreen () {}
  static void window_off_cb ( puObject * ) { mousetrap = TRUE  ; setFullscreen () ; } 
  static void window_on_cb  ( puObject * ) { mousetrap = FALSE ; setFullscreen () ; } 
#endif
#else
  static void setFullscreen () {}
  static void window_off_cb ( puObject * ) { mousetrap = TRUE  ; setFullscreen () ; } 
  static void window_on_cb  ( puObject * ) { mousetrap = FALSE ; setFullscreen () ; } 
#endif

static void mousetrap_off_cb ( puObject * ) { mousetrap = FALSE ; } 
static void mousetrap_on_cb  ( puObject * ) { mousetrap = TRUE  ; } 

static void music_off_cb     ( puObject * ) { sound->disable_music () ; } 
static void music_on_cb      ( puObject * ) { sound->enable_music  () ; } 
static void sfx_off_cb       ( puObject * ) { sound->disable_sfx   () ; } 
static void sfx_on_cb        ( puObject * ) { sound->enable_sfx    () ; } 

static void joystick_off_cb  ( puObject * ) { prefer_joystick = FALSE ; mousetrap = TRUE ; } 
static void joystick_on_cb   ( puObject * ) { prefer_joystick = TRUE  ; } 

static void set_rcmode       ( puObject * ) { tuxState->setRCMode  ( TRUE  ) ; } 
static void unset_rcmode     ( puObject * ) { tuxState->setRCMode  ( FALSE ) ; } 


static void exit_cb ( puObject * )
{
  if ( play_as_gown )
    fprintf ( stderr, "Exiting Gown's Quest for Herring.\n" ) ;
  else
    fprintf ( stderr, "Exiting Tux's Quest for Herring.\n" ) ;
  exit ( 1 ) ;
}

/* Menu bar entries: */

static char      *exit_submenu    [] = {  "Exit", NULL } ;
static puCallback exit_submenu_cb [] = { exit_cb, NULL } ;

static char      *sound_submenu    [] = { "Turn off Music", "Turn off Sounds", "Turn on Music", "Turn on Sounds", NULL } ;
static puCallback sound_submenu_cb [] = {  music_off_cb,        sfx_off_cb,     music_on_cb,        sfx_on_cb, NULL } ;

static char      *view_submenu    [] = { "Mousetrap Mode",   "No Mousetrap", "Windowed"  , "Fullscreen" , NULL } ;
static puCallback view_submenu_cb [] = { mousetrap_on_cb , mousetrap_off_cb, window_on_cb, window_off_cb, NULL } ;

static char      *ctrl_submenu    [] = { "Play using Joystick", "Play using Mouse", "RC Mode" , "Screen Mode", NULL } ;
static puCallback ctrl_submenu_cb [] = { joystick_on_cb       , joystick_off_cb   , set_rcmode, unset_rcmode , NULL } ;

static char      *help_submenu    [] = { "Versions...", "Credits...", "About...",  "Help", NULL } ;
static puCallback help_submenu_cb [] = {   versions_cb,   credits_cb,   about_cb, help_cb, NULL } ;



GUI::GUI ()
{
#ifdef XMESA_FX_FULLSCREEN
  fullscreen = TRUE ;
#endif
  hidden = TRUE ;
  mouse_x = 320 ;
  mouse_y = 240 ;

  glutMouseFunc         ( mousefn   ) ;
  glutMotionFunc        ( motionfn  ) ;
  glutPassiveMotionFunc ( motionfn  ) ;
 
  ssgInit () ;
  puInit () ;

  font = new fntTexFont ;
  font -> load ( "fonts/sorority.txf" ) ;
  puFont ff ( font, 15 ) ;
  puSetDefaultFonts        ( ff, ff ) ;
  puSetDefaultStyle        ( PUSTYLE_SMALL_SHADED ) ;
  puSetDefaultColourScheme ( 0.8, 0.8, 0.8, 0.6 ) ;

  /* Make the menu bar */

  main_menu_bar = new puMenuBar () ;

  {
    main_menu_bar -> add_submenu ( "Exit", exit_submenu, exit_submenu_cb ) ;
    main_menu_bar -> add_submenu ( "Sound", sound_submenu, sound_submenu_cb ) ;
    main_menu_bar -> add_submenu ( "View", view_submenu, view_submenu_cb ) ;
    main_menu_bar -> add_submenu ( "Controls", ctrl_submenu, ctrl_submenu_cb ) ;
    main_menu_bar -> add_submenu ( "Help", help_submenu, help_submenu_cb ) ;
  }

  main_menu_bar -> close () ;
  main_menu_bar -> hide  () ;
  
  joystick = new jsJoystick ( 0 ) ;
  joystick -> setDeadBand ( 0, 0.1 ) ;
  joystick -> setDeadBand ( 1, 0.1 ) ;
}


void GUI::show ()
{
  hide_status () ;
  hidden = FALSE ;

  if ( mousetrap )
    glutWarpPointer ( 320, 240 ) ;

#ifdef XMESA_FX_FULLSCREEN
  puShowCursor () ;
#endif
  main_menu_bar -> reveal () ;
}

void GUI::hide ()
{
  hidden = TRUE ;
  hide_status () ;
  puHideCursor () ;
  main_menu_bar -> hide () ;
}

void GUI::update ()
{
  keyboardInput  () ;
  joystickInput  () ;
  drawStatusText () ;

  glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) ;
  glAlphaFunc ( GL_GREATER, 0.1f ) ;
  glEnable    ( GL_BLEND ) ;

  puDisplay () ;

  /*
    Don't let the cursor escape from the screen!
  */

  if ( isHidden () )
    glutWarpPointer ( 320, 240 ) ;
  else
  {
    int warp = FALSE ;

    if ( mouse_x < 1 ) { mouse_x = 2 ; warp = TRUE ; }
    else if ( mouse_x > 639 ) { mouse_x = 638 ; warp = TRUE ; }
    if ( mouse_y < 1 ) { mouse_y = 2 ; warp = TRUE ; }
    else if ( mouse_y > 479 ) { mouse_y = 478 ; warp = TRUE ; }

    if ( warp && mousetrap )
      glutWarpPointer ( mouse_x, mouse_y ) ;

    mouse_dx = mouse_dy = 0 ;
    mouse_buttons = 0 ;
  }
}


void GUI::joystickInput ()
{
  static JoyInfo ji ;

  if ( isHidden() && ( joystick->notWorking() || ! prefer_joystick ) )
  {
    float mscale = 100.0f ;

    /*
      If he just pressed or just released the right
      mouse button then he toggled between moving tux
      and moving the camera - so we need to zero the
      accumulated mouse motion.
    */

    if ( ( ji.buttons & 0x04 ) != ( mouse_buttons & 0x04 ) )
    {
      mouse_dx = 0 ;
      mouse_dy = 0 ;
    }

    ji.buttons = mouse_buttons ;
    ji.data[0] = (float)mouse_dx / mscale ;
    ji.data[1] = (float)mouse_dy / mscale ;

    if ( ji.data[0] < -1.0 ) { ji.data[0] = -1.0 ; mouse_dx = (int) -mscale ; }
    if ( ji.data[0] >  1.0 ) { ji.data[0] =  1.0 ; mouse_dx = (int)  mscale ; }
    if ( ji.data[1] < -1.0 ) { ji.data[1] = -1.0 ; mouse_dy = (int) -mscale ; }
    if ( ji.data[1] >  1.0 ) { ji.data[1] =  1.0 ; mouse_dy = (int)  mscale ; }

    if ( ji.data[0] < 0.1 && ji.data[0] > -0.1 ) ji.data[0] =  0.0 ;
    if ( ji.data[1] < 0.1 && ji.data[1] > -0.1 ) ji.data[1] =  0.0 ;
  }
  else
  {
    mouse_dx = mouse_dy = 0 ;
    mouse_buttons = 0 ;

    joystick -> read ( & ji.buttons, ji.data ) ;
  }

  if ( CD_hack )
  {
    ji.buttons |= 4 + 8 ;  /* Set the C+D buttons - kludhe a 'ROLL'. */
    CD_hack = FALSE ;
  }

  ji.hits        = (ji.buttons ^ ji.old_buttons) &  ji.buttons ;
  ji.releases    = (ji.buttons ^ ji.old_buttons) & ~ji.buttons ;
  ji.old_buttons =  ji.buttons ;

  if ( camera -> joystickInput ( &ji ) )
  {
    ji.data[0] = 0.0f ;
    ji.data[1] = 0.0f ;
  }

  if ( ji.hits != 0 )
    hide_status () ;

  tuxState -> joystickInput ( &ji ) ;
}


void GUI::keyboardInput ()
{
  int c = getGLUTKeystroke () ;

  if ( currState -> cheat_mode && c == 'k' )
    currState->kludge_mode = !currState->kludge_mode ;

  if ( currState->kludge_mode )
  {
    currState->kludge_char = c ;
    c = 0 ;
  }
  else
    currState->kludge_char = 0 ;

  if ( c == 'r' )
    CD_hack = TRUE ;

  currState -> keyboardInput ( c ) ;
  camera    -> keyboardInput ( c ) ;
}


