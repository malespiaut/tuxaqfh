/* #define DBG 1 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <math.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <plib/ssg.h>

#include <GL/glut.h>
#include <plib/ssgKeyFlier.h>
/* #include <tty_port.h> */
#include <plib/sl.h>
#include <plib/js.h>
#include <plib/fnt.h>

#ifndef M_PI
#  define M_PI 3.14159265
#endif

class SoundSystem ;
class TuxState    ;
class WhaleState  ;
class GlobalState ;
class Camera      ;
class GUI         ;
class GFX         ;
class Ocean       ;

extern SoundSystem *sound      ;
extern TuxState    *tuxState   ;
extern WhaleState  *whaleState ;
extern GlobalState *currState  ;
extern Camera      *camera     ;
extern GUI         *gui        ;
extern GFX         *gfx        ;
extern Ocean       *ocean      ;
extern int        play_as_gown ;
extern ssgRoot     *scene      ;
extern ssgLoaderOptions *loader_opts ;
extern char  *tux_aqfh_datadir ;

#include "slam.h"
#include "joystick.h"

#include "material.h"
#include "ocean.h"
#include "isect.h"
#include "feature.h"
#include "penguin.h"
#include "rocket.h"
#include "orca.h"

#include "sound.h"
#include "status.h"
#include "camera.h"
#include "level.h"
#include "globalstate.h"
#include "tuxstate.h"
#include "whale.h"
#include "gui.h"
#include "gfx.h"
#include "fade_out.h"
#include "starwing.h"
#include "hooks.h"

int  getClock    () ;
void tuxMainLoop () ;
ssgBranch *process_userdata ( char *data ) ;
ssgState *getAppState ( char *fname ) ;

#define MAX_GOLD_HERRING 32
extern int herring_collected [] ;

void parse_tuxrc ( int *level ) ;
void  save_tuxrc () ;
void reset_tuxrc () ;
