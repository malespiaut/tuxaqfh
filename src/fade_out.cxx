
#include "tux.h"

static int fade_percentage = 0 ;
static int fade_direction  = 0 ;

int faded_to_black ()
{
  return ( fade_percentage >= 99 ) ;
}

void start_fade ()
{
  fade_percentage = 1 ;
  fade_direction  = 2 ;
}

void update_fade ()
{
  if ( fade_percentage <= 0 )
    return ;

  if ( fade_percentage < 101 )
    fade_percentage += fade_direction ;
  else 
  {
    fade_direction = -2 ;
    fade_percentage = 100 ;
  }

  fademsk_gst -> apply () ;

  float fader = pow ( (float)fade_percentage, 0.55 ) ;
  float f1 =  0.15 * fader + 0.5 ;
  float f2 = -0.15 * fader + 0.5 ;

  glBegin ( GL_TRIANGLE_STRIP ) ;
  glColor4f ( 0.0f, 0.0f, 0.0f, 1.0f ) ;

    glTexCoord2f ( f2, f2 ) ; glVertex2i (   0,   0 ) ;
    glTexCoord2f ( f1, f2 ) ; glVertex2i ( 640,   0 ) ;
    glTexCoord2f ( f2, f1 ) ; glVertex2i (   0, 480 ) ;
    glTexCoord2f ( f1, f1 ) ; glVertex2i ( 640, 480 ) ;

  glEnd () ;
}



