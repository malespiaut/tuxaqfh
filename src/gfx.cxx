
#include  "tux.h"

static unsigned int lastGLUTKeystroke = 0 ;

static void getGLUTSpecialKeystroke ( int key, int, int )
{
  lastGLUTKeystroke = 256 + key ;
}

static void getGLUTKeystroke ( unsigned char key, int, int )
{
  lastGLUTKeystroke = key ;
}

int getGLUTKeystroke ()
{
  int k = lastGLUTKeystroke ;
  lastGLUTKeystroke = 0 ;
  return k ;
}

void reshape ( int w, int h )
{
  glViewport ( 0, 0, w, h ) ;
}

void initWindow ( int w, int h )
{
  int fake_argc = 1 ;
  char *fake_argv[3] ;

  if ( play_as_gown )
  {
    fake_argv[0] = "Gown" ;
    fake_argv[1] = "Gown T. Penguin stars in 'A Quest for Herring'." ;
    fake_argv[2] = NULL ;
  }
  else
  {
    fake_argv[0] = "Tux" ;
    fake_argv[1] = "Tuxedo T. Penguin stars in 'A Quest for Herring'." ;
    fake_argv[2] = NULL ;
  }

  glutInitWindowPosition ( 0, 0 ) ;
  glutInitWindowSize     ( w, h ) ;
  glutInit               ( &fake_argc, fake_argv ) ;
  glutInitDisplayMode    ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH ) ;
  glutCreateWindow       ( fake_argv[1] ) ;
  glutDisplayFunc        ( tuxMainLoop ) ;
  glutKeyboardFunc       ( getGLUTKeystroke ) ;
  glutSpecialFunc        ( getGLUTSpecialKeystroke ) ;
  glutReshapeFunc        ( reshape ) ;
#ifndef WIN32
  glutIdleFunc           ( glutPostRedisplay ) ;
#endif
}


GFX::GFX ()
{
  initWindow ( 640, 480 ) ;

  ssgInit  () ;
 
  sgVec4 clear ;
  sgSetVec4 ( clear, 0.2, 0.7, 1.0, 1.0 ) ;
 
  static int firsttime = 1 ;

  if ( firsttime )
  {
    firsttime = 0 ;
    initMaterials () ;
  }

  ssgSetFOV ( 45.0f, 0.0f ) ;
  ssgSetNearFar ( 1.0f, 700.0f ) ;

  sgCoord cam ;
  sgSetVec3 ( cam.xyz, 0, 0, 20 ) ;
  sgSetVec3 ( cam.hpr, 0, 0,  0 ) ;

  ssgSetCamera ( & cam ) ;
}


void GFX::update ()
{
  sgVec4 skyfogcol ;
  sgVec3 sunposn ;

  sgSetVec3 ( sunposn, 0.2, 0.5, 0.5 ) ;

  if ( ocean )
    ocean -> update () ;

  if ( ocean && camera -> isSubmerged () )
  {
    switch ( ocean -> getType () )
    {
      case OCEAN_BLACKHOLE :
        sgSetVec4 ( skyfogcol, 0.0, 0.0, 0.0, 1.0 ) ;
        break ;
      case OCEAN_ICE :
        sgSetVec4 ( skyfogcol, 0.6, 0.6, 0.9, 1.0 ) ;
        break ;
      case OCEAN_WATER :
        sgSetVec4 ( skyfogcol, 0.2, 0.4, 0.7, 1.0 ) ;
        break ;
      case OCEAN_LAVA :
	{
	  float t = (float)( getClock () % 240 ) ;
	  if ( t > 120.0 ) t = 240.0 - t ;
	  t /= 120.0f ;
	  sgSetVec4 ( skyfogcol, 0.6 * t, 0.0, 0.0, 1.0 ) ;
	}
        break ;
    }
  }
  else
    sgSetVec4 ( skyfogcol, tuxState->skycol[0],
                           tuxState->skycol[1],
                           tuxState->skycol[2], 1.0 ) ;

  ssgGetLight ( 0 ) -> setPosition ( sunposn ) ;

  /* Clear the screen */

  glClearColor ( skyfogcol[0], skyfogcol[1], skyfogcol[2], skyfogcol[3] ) ;
  glClear      ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;

  glEnable ( GL_DEPTH_TEST ) ;

  if ( ocean && camera -> isSubmerged () )
  {
    switch ( ocean -> getType () )
    {
      case OCEAN_BLACKHOLE : glFogf ( GL_FOG_DENSITY, 0.07 ) ; break ;
      case OCEAN_ICE       : glFogf ( GL_FOG_DENSITY, 0.07 ) ; break ;
      case OCEAN_WATER     : glFogf ( GL_FOG_DENSITY, 0.03 ) ; break ;
      case OCEAN_LAVA      : glFogf ( GL_FOG_DENSITY, 0.09 ) ; break ;
    }
  }
  else
    glFogf ( GL_FOG_DENSITY, 0.005 / 100.0f ) ;

  glFogfv( GL_FOG_COLOR  , skyfogcol ) ;
  glFogf ( GL_FOG_START  , 0.0       ) ;
  glFogi ( GL_FOG_MODE   , GL_EXP2   ) ;
  glHint ( GL_FOG_HINT   , GL_NICEST ) ;

  glEnable ( GL_FOG ) ;
  ssgCullAndDraw ( scene ) ;
  glDisable ( GL_FOG ) ;
}


void GFX::done ()
{
  glutPostRedisplay () ;
  glutSwapBuffers () ;
}

