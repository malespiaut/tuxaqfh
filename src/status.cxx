
#include "tux.h"
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>

#define MAX_STRING          30
#define MAX_STRING_LENGTH  256

static fntRenderer *text = NULL ;

static int stopwatch_running = FALSE ;
static int stopwatch_display = FALSE ;
static double start_time = 0.0 ;
static double stop_time  = 0.0 ;

double read_time_of_day ()
{
#if defined( WIN32 ) && !defined( __CYGWIN32__ )
  _int64 u, v ;
  QueryPerformanceCounter   ((LARGE_INTEGER*) &u ) ;
  QueryPerformanceFrequency ((LARGE_INTEGER*) &v ) ;
  return (double)u / (double)v ;
#else
  timeval tv ;
  gettimeofday ( &tv, NULL ) ;

  return (double) tv . tv_sec + (double) tv . tv_usec / 1000000.0 ;
#endif
}


void hide_stopwatch ()
{
  stopwatch_running = FALSE ;
  stopwatch_display = FALSE ;
}



void start_stopwatch ()
{
  stopwatch_running = TRUE ;
  stopwatch_display = TRUE ;
  stPrintf ( "GO, GO, GO!" ) ;
  start_time = read_time_of_day () ;
  stop_time = 0.0 ;
}


void stop_stopwatch ()
{
  if ( stopwatch_running )
  {
    stopwatch_running = FALSE ;
    stopwatch_display = TRUE  ;
    stop_time = read_time_of_day () ;
    stPrintf ( "STOP THE CLOCK !!" ) ;

    check_level_clock ( stop_time - start_time ) ;
  }
}


char debug_strings [ MAX_STRING ][ MAX_STRING_LENGTH ] ;
int next_string   = 0 ;
int stats_enabled = FALSE ;

void stToggle ()
{
  if ( stats_enabled )
    stats_enabled = FALSE ;
  else
  {
    stats_enabled = TRUE ;

    for ( int i = 0 ; i < MAX_STRING ; i++ )
       debug_strings [ i ][ 0 ] = '\0' ;

    next_string = 0 ;
  }
}


void stPrintf ( char *fmt, ... )
{
  char *p = debug_strings [ next_string++ ] ;

  if ( next_string >= MAX_STRING )
    next_string = 0 ;

  va_list ap ;
  va_start ( ap, fmt ) ;
/*
  Ideally, we should use this:

     vsnprintf ( p, MAX_STRING_LENGTH, fmt, ap ) ;

  ...but it's only in Linux   :-(
*/

  vsprintf ( p, fmt, ap ) ;

  va_end ( ap ) ;
}

static int    death_timer = 99999 ;
static int    about_timer = 99999 ;
static int versions_timer = 99999 ;
static int  credits_timer = 99999 ;
static int    intro_timer =    0  ;
static int     help_timer = 99999 ;

void hide_status () { versions_timer = death_timer = credits_timer =
                         intro_timer = help_timer = about_timer = 99999 ; }
void memorial    () {    death_timer = 0 ; }
void about       () {    about_timer = 0 ; }
void credits     () {  credits_timer = 0 ; }
void versions    () { versions_timer = 0 ; }
void help        () {     help_timer = 0 ; }

void drawText ( char *str, int sz, int x, int y )
{
  text -> setFont      ( font ) ;
  text -> setPointSize ( sz ) ;

  text -> begin () ;
    text -> start2f ( x, y ) ;
    text -> puts ( str ) ;
  text -> end () ;
}


void drawInverseDropShadowText ( char *str, int sz, int x, int y )
{
  glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f ) ;
  drawText ( str, sz, x, y ) ;
  glColor4f ( 0.0f, 0.0f, 0.0f, 1.0f ) ;
  drawText ( str, sz, x+1, y+1 ) ;
}


void drawDropShadowText ( char *str, int sz, int x, int y )
{
  if ( play_as_gown )
    glColor4f ( 0.5f, 0.0f, 0.0f, 1.0f ) ;
  else
    glColor4f ( 0.0f, 0.0f, 0.0f, 1.0f ) ;

  drawText ( str, sz, x, y ) ;

  if ( play_as_gown )
    glColor4f ( 1.0f, 0.6f, 0.7f, 1.0f ) ;
  else
    glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f ) ;

  drawText ( str, sz, x+1, y+1 ) ;
}


void drawHelpText ()
{
  drawDropShadowText ( "Press SPACE to toggle the menu.", 18, 70, 400 ) ;
  drawDropShadowText ( "Press ESCAPE to exit the game.", 18, 70, 370 ) ;
  drawDropShadowText ( "Use arrow keys and PgUp,PgDown,Home,End,Ins", 18, 70, 340 ) ;
  drawDropShadowText ( "          or Del to move the camera around.", 18, 70, 310 ) ;
  drawDropShadowText ( "Move Tux with the Joystick", 18, 70, 280 ) ;

  if ( currState->cheat_mode )
  {
    drawDropShadowText ( "Cheat mode controls:"  , 12, 150, 240 ) ;
    drawDropShadowText ( "x   -- Exit Cheat Mode.",10, 160, 220 ) ;
    drawDropShadowText ( "c   -- Console Mode."  , 10, 160, 210 ) ;
    drawDropShadowText ( "t/T -- Texture."       , 10, 160, 200 ) ;
    drawDropShadowText ( "w/W -- Wireframe."     , 10, 160, 190 ) ;
    drawDropShadowText ( "f   -- Freeze-Baddies.", 10, 160, 180 ) ;
    drawDropShadowText ( "s   -- SuperTux."      , 10, 160, 170 ) ;
    drawDropShadowText ( "r   -- RocketPack."    , 10, 160, 160 ) ;
    drawDropShadowText ( "g   -- LaserGun."      , 10, 160, 150 ) ;
  }
  else
  {
    drawDropShadowText ( "Joystick Buttons:", 18, 70, 250 ) ;
    drawDropShadowText ( "  A  - Jump, Swim      B  - Dive, Throw", 12, 90, 230 ) ;
    drawDropShadowText ( " C+D - Roll           L/R - Camera", 12, 90, 210 ) ;
  }
}


void drawTitleText ()
{
  drawDropShadowText ( "A Quest for Herring", 20, 70, 400 ) ;
  drawDropShadowText ( "By Steve & Oliver Baker", 12, 170, 385 ) ;

  drawDropShadowText ( "Starring:", 12, 70, 335 ) ;

  if ( play_as_gown )
    drawDropShadowText ( "Gown T. Penguin", 20, 70, 300 ) ;
  else
    drawDropShadowText ( "Tux T. Penguin", 20, 70, 300 ) ;
}


void drawDeathText ()
{
  if ( play_as_gown )
    drawInverseDropShadowText ( "Gown died because she", 20, 100, 280 ) ;
  else
    drawInverseDropShadowText ( "Tux died because he", 20, 100, 280 ) ;

  if ( death_timer & 8 )
    drawInverseDropShadowText ( tuxState -> death_reason, 20, 100, 240 ) ;
}


void drawIntroText ()
{
  drawTitleText () ;

  drawDropShadowText ( "With special thanks to Larry Ewing",
                       15, 120, 280 ) ;

  if ( intro_timer & 8 )
    drawDropShadowText ( "Press SPACE bar for menu.",
                       15, 10, 430 ) ;
}


char *aboutText [] =
{
  "  When members of the Linux community decided that we needed a mascot, there was",
  "heated discussion over what kind of a creature should be chosen. Linus Torvalds",
  "(the father of Linux) casually mentioned that he was rather fond of Penguins.",
  " ",
  "  In time, Larry Ewing's famous GIMP painting of Tux became the defacto mascot.",
  "It is that image that inspired this game.  Linus says that Tux should look",
  "\"contented\"..\"stuffed on herring\" - well there are plenty of herring in this",
  "game - and you'll have to help Tux (or his girlfriend Gown) to find them.",
  " ",
  "  You might also be interested to know that a group of Linux enthusiasts sponsored",
  "a real, live Penguin as a birthday present for Linus.  So, you can meet Tux in",
  "person at Bristol Zoo in the South of England.",
  NULL
} ;


void drawVersionsText ()
{
  char str [ 256 ] ;

#ifdef VERSION
  sprintf ( str, "Tux_AQFH Version: %s", VERSION ) ;
#else
  sprintf ( str, "Tux_AQFH: Unknown Version." ) ;
#endif
  drawDropShadowText ( str, 15, 20, 250 ) ;

  sprintf ( str, "PLIB Version: %s", ssgGetVersion() ) ;
  drawDropShadowText ( str, 15, 20, 225 ) ;

  sprintf ( str, "OpenGL Version: %s", glGetString ( GL_VERSION ) ) ;
  drawDropShadowText ( str, 15, 20, 200 ) ;

  sprintf ( str, "OpenGL Vendor: %s", glGetString ( GL_VENDOR ) ) ;
  drawDropShadowText ( str, 15, 20, 175 ) ;

  sprintf ( str, "OpenGL Renderer: %s", glGetString ( GL_RENDERER ) ) ;

  if ( strlen ( str ) > 50 )
  {
    int l = strlen ( str ) ;
    int ll = 0 ;

    for ( int i = 0 ; i < l ; i++, ll++ )
    {
      if ( ll > 40 && str[i] == ' ' )
      {
        str[i] = '\n' ;
        ll = 0 ;
      }
    }
  }

  drawDropShadowText ( str, 15, 20, 150 ) ;

  if ( versions_timer & 8 )
    drawDropShadowText ( "Press SPACE to continue",
                       15, 10, 430 ) ;
}


void drawAboutText ()
{
  drawTitleText () ;

  drawDropShadowText ( "The History behind Tux:",
                       20, 20, 250 ) ;

  for ( int i = 0 ; aboutText [ i ] != NULL ; i++ )
    drawDropShadowText ( aboutText [ i ],
                       10, 40, 230 - 10 * i ) ;

  if ( about_timer & 8 )
    drawDropShadowText ( "Press SPACE to continue",
                       15, 10, 430 ) ;
}


char *creditsText [] =
{
  "  Steve  Baker    - Coding, design, bug insertion.",
  "  Oliver Baker    - Many suggestions, 3D objects, much else besides.",
  "  Linus  Torvalds - For saving the world from the Tasteless Empire.",
  "  Larry  Ewing    - For painting the original Tux image.",
  " ",
  "Valuable suggestions came from:",
  " ",
  "  Steven Laxton, Bryan Whitlow",
  " ",
  "Special thanks to:",
  " ",
  "  Vasily Lewis, Curtis Olsen, Daryll Strauss, Brian Paul,",
  "  David Bucciarelli, Mark Kilgard",
  NULL
} ;



void drawCreditsText ()
{
  drawTitleText () ;

  drawDropShadowText ( "Credits:",
                       20, 70, 250 ) ;

  for ( int i = 0 ; creditsText [ i ] != NULL ; i++ )
    drawDropShadowText ( creditsText [ i ],
                       12, 100, 230 - 12 * i ) ;

  if ( credits_timer & 8 )
    drawDropShadowText ( "Press SPACE to continue",
                       15, 10, 430 ) ;
}


static int bubble_wobble [ 20 ] = { 40, 47, 42, 48, 45, 41, 47, 44,
                                    41, 48, 40, 46, 42, 44, 40, 43,
                                    48, 42, 45, 40 } ;


void drawStatusText ()
{
  if ( text == NULL )
    text = new fntRenderer () ;

  int i ;

  glMatrixMode   ( GL_MODELVIEW ) ;
  glPushMatrix   () ;
  glLoadIdentity () ;

  glMatrixMode   ( GL_PROJECTION ) ;
  glPushMatrix   () ;
  glLoadIdentity () ;

  glDisable      ( GL_DEPTH_TEST ) ;

  glPushAttrib   ( GL_ENABLE_BIT | GL_LIGHTING_BIT ) ;
  glDisable      ( GL_LIGHTING   ) ;
  glDisable      ( GL_FOG        ) ;
  glDisable      ( GL_CULL_FACE  ) ;
  glDisable      ( GL_ALPHA_TEST ) ;
  glEnable       ( GL_BLEND      ) ;

  glOrtho        ( 0, 640, 0, 480, 0, 100 ) ;

  update_fade () ;

  char str [ 256 ] ;

  if ( stopwatch_display )
  {
    double tim = ( stopwatch_running ? read_time_of_day () : stop_time ) - start_time ;

    int min     = (int) floor ( tim / 60.0 ) ;
    int sec     = (int) floor ( tim - (double) ( 60 * min ) ) ;
    int tenths  = (int) floor ( 10.0f * ( tim - (double)(sec + 60*min) ) ) ;

    tim = get_level_clock () ;

    int tmin    = (int) floor ( tim / 60.0 ) ;
    int tsec    = (int) floor ( tim - (double) ( 60 * tmin ) ) ;
    int ttenths = (int) floor ( 10.0f * ( tim - (double)(tsec + 60*tmin) ) ) ;

    sprintf ( str, "%3d:%02d.%d | %3d:%02d.%d",
                      min,  sec,  tenths, tmin, tsec, ttenths ) ;
    drawDropShadowText ( str, 18, 10, 430 ) ;
  }

  static time_t old_t = 0 ;
  static int nframes = 0 ;
  static int hz_rate = 0 ;
  time_t t = time ( NULL ) ;

  if ( t != old_t )
  {
    hz_rate = nframes / ( t - old_t ) ;
    nframes = 1 ;
    old_t = t ;
  }
  else
    nframes++ ;

  if ( currState->cheat_mode )
  {
#ifdef STATISTICS
    sprintf ( str, "Stats: %s", ssgShowStats () ) ;
    drawDropShadowText ( str, 12, 10, 130 ) ;
    sprintf ( str, "%dHz %s", hz_rate,
         currState->freeze_baddies ? "Freeze Baddies" : "" ) ;
    drawDropShadowText ( str, 12, 10, 110 ) ;
#else
    sprintf ( str, "%dHz", hz_rate ) ;
    drawDropShadowText ( str, 12, 600, 30 ) ;
#endif
  }

  if ( currState -> pause )
    drawDropShadowText ( "Paused - hit any key",
                       20, 100, 400 ) ;

  glColor4f ( 0.6, 0.0, 0.6, 1.0 ) ;

  if ( stats_enabled )
    for ( i = 0 ; i < MAX_STRING ; i++ )
    {
      glRasterPos2f ( 10, 480 - 12 * (i + 2) ) ;

      int m = (i+next_string) % MAX_STRING ;
      char *s = debug_strings [ m ] ;

      for ( char *q = s ; *q != '\0' ; q++ )
	glutBitmapCharacter ( GLUT_BITMAP_9_BY_15, *q ) ;
    }

  if ( death_timer++ < 200 )
    drawDeathText () ;
  else
  if ( help_timer++ < 400 )
    drawHelpText () ;
  else
  if ( intro_timer++ < 400 )
    drawIntroText () ;
  else
  if ( credits_timer++ < 1600 )
    drawCreditsText () ;
  else
  if ( about_timer++ < 1600 )
    drawAboutText () ;
  else
  if ( versions_timer++ < 1600 )
    drawVersionsText () ;

  glPopAttrib () ;

  glDisable      ( GL_CULL_FACE  ) ;

  if ( tuxState -> help_message != NULL )
  {
    glEnable ( GL_TEXTURE_2D ) ;
    think_gst -> apply () ;

    glBegin ( GL_QUADS ) ;
    glColor4f    ( 1, 1, 1, 1 ) ; 
    glTexCoord2f ( 0, 0 ) ; glVertex2f ( 450, 170 ) ;
    glTexCoord2f ( 1, 0 ) ; glVertex2f ( 640, 170 ) ;
    glTexCoord2f ( 1, 1 ) ; glVertex2f ( 640, 358 ) ;
    glTexCoord2f ( 0, 1 ) ; glVertex2f ( 450, 358 ) ;
    glEnd () ;
    glColor4f ( 0.3, 0.3, 0.3, 1 ) ; 

    if ( tuxState->help_message2 != NULL )
    {
      drawText  ( tuxState->help_message,
                       12, 480, 260 ) ;
      drawText  ( tuxState->help_message2,
                       12, 480, 245 ) ;
    }
    else
      drawText  ( tuxState->help_message,
                       10, 480, 255 ) ;
  }

  glEnable ( GL_ALPHA_TEST ) ;

  if ( tuxState -> temperature > 1.0f )
  {
    if ( tuxState->temperature < MAX_TEMPERATURE * 0.8 ||
         nframes % 8 < 4 )
    {
      thermometer_gst -> apply () ;

      glBegin ( GL_QUADS ) ;
      glColor4f    ( 1, 1, 1, 1 ) ; 
      glTexCoord2f ( 0, 0 ) ; glVertex2f ( 570, 10 ) ;
      glTexCoord2f ( 1, 0 ) ; glVertex2f ( 586, 10 ) ;
      glTexCoord2f ( 1, 1 ) ; glVertex2f ( 586, 74 ) ;
      glTexCoord2f ( 0, 1 ) ; glVertex2f ( 570, 74 ) ;
      glEnd () ;

      glBegin ( GL_QUADS ) ;
      glColor4f    ( 1, 1, 1, 1 ) ; 
      glTexCoord2f (  5.0f/16.0f, 13.0f/64.0f ) ; glVertex2f ( 575.0f, 28.0f ) ;
      glTexCoord2f ( 10.0f/16.0f, 13.0f/64.0f ) ; glVertex2f ( 580.0f, 28.0f ) ;
      glTexCoord2f ( 10.0f/16.0f, 20.0f/64.0f ) ; glVertex2f ( 580.0f, 28.0f + tuxState->temperature ) ;
      glTexCoord2f (  5.0f/16.0f, 20.0f/64.0f ) ; glVertex2f ( 575.0f, 28.0f + tuxState->temperature ) ;
      glEnd () ;
    }
  }

  if ( tuxState -> life > 0 )
  {
    spark_gst -> apply () ;

    for ( i = 0 ; i < tuxState->life ; i++ )
    {
      int x1 = 32 * i + 200 ;
      int y1 = 0 ;

      glBegin ( GL_QUADS ) ;
      glColor4f    ( 1, 1, 1, 1 ) ; 
      glTexCoord2f ( 0, 0 ) ; glVertex2f ( x1   , y1    ) ;
      glTexCoord2f ( 1, 0 ) ; glVertex2f ( x1+32, y1    ) ;
      glTexCoord2f ( 1, 1 ) ; glVertex2f ( x1+32, y1+32 ) ;
      glTexCoord2f ( 0, 1 ) ; glVertex2f ( x1   , y1+32 ) ;
      glEnd () ;
    }
  }

  if ( tuxState -> diving || currState -> cant_breathe )
  {
    bubble_gst -> apply () ;

    for ( i = 0 ; i < 1 + (20 * tuxState -> dive_timer) / HOLD_BREATH_TIME && i < 20 ; i++ )
    {
      int x1 = 14 * i + 200 ;
      int y1 = bubble_wobble [ i ] ;

      glBegin ( GL_QUADS ) ;
      glColor4f    ( 1, 1, 1, 1 ) ; 
      glTexCoord2f ( 0, 0 ) ; glVertex2f ( x1   , y1    ) ;
      glTexCoord2f ( 1, 0 ) ; glVertex2f ( x1+32, y1    ) ;
      glTexCoord2f ( 1, 1 ) ; glVertex2f ( x1+32, y1+32 ) ;
      glTexCoord2f ( 0, 1 ) ; glVertex2f ( x1   , y1+32 ) ;
      glEnd () ;
    }
  }

  herringbones_gst -> apply () ;

  glBegin ( GL_QUADS ) ;
  glColor3f    ( 1, 1, 1 ) ; 
  glTexCoord2f ( 0, 0 ) ; glVertex2f ( 100,  0 ) ;
  glTexCoord2f ( 1, 0 ) ; glVertex2f ( 200,  0 ) ;
  glTexCoord2f ( 1, 1 ) ; glVertex2f ( 200, 40 ) ;
  glTexCoord2f ( 0, 1 ) ; glVertex2f ( 100, 40 ) ;
  glEnd () ;

  herring_gst -> apply () ;

  glBegin ( GL_QUADS ) ;
  glColor3f    ( 0.7, 1, 1 ) ; 
  glTexCoord2f ( 0, 0 ) ; glVertex2f ( 100,  0 ) ;
  glTexCoord2f ( tuxState->energy, 0 ) ; glVertex2f ( 100 + tuxState->energy * 100,  0 ) ;
  glTexCoord2f ( tuxState->energy, 1 ) ; glVertex2f ( 100 + tuxState->energy * 100, 40 ) ;
  glTexCoord2f ( 0, 1 ) ; glVertex2f ( 100, 40 ) ;
  glEnd () ;

  int goldHerrings = 0 ;

  for ( int h = 0 ; h < MAX_GOLD_HERRING ; h++ )
    if ( herring_collected [ h ] )
      goldHerrings++ ;

  for ( i = 0 ; i < 8 ; i++ )
  {
    glPushMatrix () ;
    glTranslatef ( 50, 50, 0 ) ;
    glRotatef ( 360 * i / 8, 0, 0, 1 ) ;

      glBegin ( GL_QUADS ) ;
	glColor4f    ( 1, 1, 0.4, (i >= goldHerrings) ? 0.3 : 1.0 ) ; 
	glTexCoord2f ( 1, 0 ) ; glVertex2f (  5,-12 ) ;
	glTexCoord2f ( 0, 0 ) ; glVertex2f ( 50,-12 ) ;
	glTexCoord2f ( 0, 1 ) ; glVertex2f ( 50, 12 ) ;
	glTexCoord2f ( 1, 1 ) ; glVertex2f (  5, 12 ) ;
      glEnd () ;
    glPopMatrix () ;
  }

  if ( goldHerrings >= 8 )
    for ( i = 8 ; i < 16 ; i++ )
    {
      glPushMatrix () ;
      glTranslatef ( 50, 50, 0 ) ;
      glRotatef ( 22.5f + (float)(360 * i / 8), 0, 0, 1 ) ;

	glBegin ( GL_QUADS ) ;
	  glColor4f    ( 1, 1, 0.4, (i >= goldHerrings) ? 0.3 : 1.0 ) ; 
	  glTexCoord2f ( 1, 0 ) ; glVertex2f ( 15,-12 ) ;
	  glTexCoord2f ( 0, 0 ) ; glVertex2f ( 60,-12 ) ;
	  glTexCoord2f ( 0, 1 ) ; glVertex2f ( 60, 12 ) ;
	  glTexCoord2f ( 1, 1 ) ; glVertex2f ( 15, 12 ) ;
	glEnd () ;
      glPopMatrix () ;
    }


  snowball_gst -> apply () ;

  static int draw_order [ MAX_SNOWBALL ] = { 0, 1, 2, 9, 10, 3, 4, 5, 13,
                                             11, 12, 6, 7, 8 } ; 
  static int hpos [ MAX_SNOWBALL ] = { 32, 62, 92,
                                       16, 46, 76,
                                        0, 30, 60,
                                           34, 64,
                                           18, 48,
                                               38 } ;
  static int vpos [ MAX_SNOWBALL ] = { 32, 30, 28,
                                       16, 14, 12,
                                        0, -2, -4,
                                           40, 38,
                                           24, 22,
                                               48 } ;

  for ( i = 0 ; i < MAX_SNOWBALL ; i++ )
  {
    int p = draw_order [ i ] ;

    if ( p >= tuxState -> snowballs )
      continue ;

    glBegin ( GL_QUADS ) ;
    glColor3f    ( 1, 1, 1 ) ; 
    glTexCoord2f ( 0, 0 ) ; glVertex2f ( 500 + (hpos[p] - 16)/2, 20 + (vpos[p])/2 ) ;
    glTexCoord2f ( 1, 0 ) ; glVertex2f ( 500 + (hpos[p] + 16)/2, 20 + (vpos[p])/2 ) ;
    glTexCoord2f ( 1, 1 ) ; glVertex2f ( 500 + (hpos[p] + 16)/2, 20 + (vpos[p] + 32)/2 ) ;
    glTexCoord2f ( 0, 1 ) ; glVertex2f ( 500 + (hpos[p] - 16)/2, 20 + (vpos[p] + 32)/2 ) ;
    glEnd () ;
  }

  tinytux_gst -> apply () ;

  for ( i = 0 ; i < tuxState -> lives-1 ; i++ )
  {
    float x1 = 100.0f + (float)(( 32 * i ) % 80 ) ; 
    float y1 =  40.0f + (float)(( 32 * i ) / 80 ) ; 
    float x2 = x1 + 32.0f ;
    float y2 = y1 + 40.0f ;

    glBegin ( GL_QUADS ) ;
    glColor4f    ( 1, 1, 1, 1 ) ; 
    glTexCoord2f ( 0, 0 ) ; glVertex2f ( x1, y1 ) ;
    glTexCoord2f ( 1, 0 ) ; glVertex2f ( x2, y1 ) ;
    glTexCoord2f ( 1, 1 ) ; glVertex2f ( x2, y2 ) ;
    glTexCoord2f ( 0, 1 ) ; glVertex2f ( x1, y2 ) ;
    glEnd () ;
  }

  if ( camera -> isFollowing () )
    nocamera_gst -> apply () ;
  else
    camera_gst -> apply () ;

  glBegin ( GL_QUADS ) ;
  glColor3f    ( 1, 1, 1 ) ; 
  glTexCoord2f ( 0, 0 ) ; glVertex2f ( 600,  0 ) ;
  glTexCoord2f ( 1, 0 ) ; glVertex2f ( 632,  0 ) ;
  glTexCoord2f ( 1, 1 ) ; glVertex2f ( 632, 32 ) ;
  glTexCoord2f ( 0, 1 ) ; glVertex2f ( 600, 32 ) ;
  glEnd () ;


  glEnable    ( GL_CULL_FACE  ) ;
  glEnable    ( GL_DEPTH_TEST ) ;
  glPopMatrix () ;
  glMatrixMode( GL_MODELVIEW ) ;
  glPopMatrix () ;
}

