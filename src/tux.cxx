#include "tux.h"

#ifdef _MSC_VER
/* Oh *thanks* Microsoft :-(  */
#include <io.h>
#include <direct.h>
#define access _access
#define F_OK   0
#define chdir  _chdir
#endif

ssgLoaderOptions *loader_opts = NULL ;                                          

TuxState     *tuxState = NULL ;
WhaleState *whaleState = NULL ;
GlobalState *currState = NULL ;
Camera         *camera = NULL ;
GUI               *gui = NULL ;
GFX               *gfx = NULL ;
SoundSystem     *sound = NULL ;
int       play_as_gown = FALSE ;
int herring_collected [ MAX_GOLD_HERRING ] ;
char *tux_aqfh_datadir = NULL ;

static void banner ()
{
  printf ( "\n\n" ) ;
  printf ( "   WELCOME TO: TUXEDO T. PENGUIN - A QUEST FOR HERRING.\n" ) ;
  printf ( "               by Steve and Oliver Baker\n" ) ;
  printf ( "                 <sjbaker1@airmail.net>\n" ) ;
  printf ( "                  http://tuxaqfh.sourceforge.net\n" ) ;
  printf ( "\n\n" ) ;
  printf ( "                        #####        \n" ) ;       
  printf ( "                       #######       \n" ) ;  
  printf ( "                       #  #  #       \n" ) ;
  printf ( "                       #\" #\" #       \n" ) ;
  printf ( " Linux                ##vvvvv##      \n" ) ;
  printf ( " Rules!              ##  vvv  ##     \n" ) ;
  printf ( "                    #          ##    \n" ) ;
  printf ( "                   ##           ##   \n" ) ;
  printf ( "                   ###          ###  \n" ) ;
  printf ( "                 +++#####       ##++ \n" ) ;
  printf ( "                ++++++#       #++++++\n" ) ;
  printf ( "                +++++++#     #+++++++\n" ) ;
  printf ( "                  +++++#######+++++  \n" ) ;
  printf ( "                    +++       +++    \n" ) ;
  printf ( "\n\n" ) ;
}

static void cmdline_help ()
{
  banner () ;

  printf ( "Usage:\n\n" ) ;
  printf ( "    tux_aqfh [OPTIONS]...\n\n" ) ;
  printf ( "Options:\n" ) ;
  printf ( "  -h, --help            Display this help message.\n" ) ;
  printf ( "  -t, --tux             Play as Tux (the default)\n" ) ;
  printf ( "  -g, --gown            Play as Gown\n" ) ;
  printf ( "  -r, --reset           Reset game to start (BEWARE!!)\n" ) ;
  printf ( "  -d                    Enable various internal debug\n" ) ;
  printf ( "  -D, --datadir DIR     Load the game data from DIR defaults\n" ) ;
  printf ( "                        to /usr/local/share/games/tux_aqfh\n" ) ;
  printf ( "\n" ) ;
}


int main ( int argc, char **argv )
{
  int changed_player   = FALSE ;
  int new_play_as_gown = FALSE ;
  int start_level = LEVEL_TRAINING ;

  for ( int h = 0 ; h < MAX_GOLD_HERRING ; h++ )
    herring_collected [ h ] = FALSE ;

  play_as_gown = FALSE ;

  for ( int i = 1 ; i < argc ; i++ )
  {
    if ( argv[i][0] == '-' )
      switch ( argv[i][1] )
      {
        case '-' :
          {
            if ( strcmp ( & argv[i][2], "gown" ) == 0 )
            {
              new_play_as_gown = TRUE ;
              changed_player   = TRUE ;
            }
            else
            if ( strcmp ( & argv[i][2], "tux" ) == 0 )
            {
              new_play_as_gown = FALSE ;
              changed_player   = TRUE  ;
            }
            else
            if ( strcmp ( & argv[i][2], "reset" ) == 0 )
              reset_tuxrc () ;
            else
            if ( strcmp ( & argv[i][2], "help" ) == 0 )
            {
              cmdline_help () ;
              exit ( 0 ) ;
            }
          }
          break ;

        case 'r' : case 'R' : reset_tuxrc  () ; break ;
        case 'h' : case 'H' : cmdline_help () ; exit ( 0 ) ;
        case 't' : case 'T' :
            {
              new_play_as_gown = FALSE ;
              changed_player   = TRUE  ;
            }
            break ;
        case 'g' : case 'G' :
            {
              new_play_as_gown = TRUE  ;
              changed_player   = TRUE  ;
            }
            break ;
        case 'd' :
#ifndef WIN32
          fprintf ( stderr, "gdb ./src/tux_aqfh %d\n", getpid () ) ;
          sleep ( 20 ) ;
#endif
          break ;
        case 'D' :
          tux_aqfh_datadir = argv[++i] ;
        default  : break ;
      }
  }

  /* Set tux_aqfh_datadir to the correct directory */

  if ( tux_aqfh_datadir == NULL )
  {
    if ( getenv ( "TUX_AQFH_DATADIR" ) != NULL )
      tux_aqfh_datadir = getenv ( "TUX_AQFH_DATADIR" ) ;
    else
    if ( access ( "data/level0.dat", F_OK ) == 0 )
      tux_aqfh_datadir = "." ;
    else
    if ( access ( "../data/level0.dat", F_OK ) == 0 )
      tux_aqfh_datadir = ".." ;
    else
#ifdef TUX_AQFH_DATADIR
      tux_aqfh_datadir = TUX_AQFH_DATADIR ;
#else
      tux_aqfh_datadir = "/usr/local/share/games/tux_aqfh" ;
#endif
  }

  fprintf ( stderr, "Data files will be fetched from: '%s'\n",
                                                    tux_aqfh_datadir ) ;

  if ( chdir ( tux_aqfh_datadir ) == -1 )
  {
    fprintf ( stderr, "Couldn't chdir() to '%s'.\n", tux_aqfh_datadir ) ;
    exit ( 1 ) ;
  }

  banner () ;

  if ( loader_opts == NULL )
  {
    loader_opts = new ssgLoaderOptions () ;
    loader_opts -> setCreateStateCallback ( getAppState ) ;
    loader_opts -> setCreateBranchCallback ( process_userdata ) ;
    ssgSetCurrentOptions ( loader_opts ) ;
  }                                                                             

  init_hooks () ;
  parse_tuxrc ( & start_level ) ;

  if ( changed_player )
  {
    play_as_gown = new_play_as_gown ;
    save_tuxrc () ;
  }

#ifndef WIN32
  sleep ( 1 ) ;
#endif

  gfx          = new GFX         ;
  tuxState     = new TuxState ( play_as_gown ) ;
  whaleState   = new WhaleState  ;
  currState    = new GlobalState ;
  camera       = new Camera      ;
  sound        = new SoundSystem ;
  gui          = new GUI         ;
  currState -> level = start_level ;

  switchDatabase ( currState->level ) ;
  glutMainLoop () ;
  return 0 ;
}

static int master_clock = 0 ;

int getClock ()
{
  return master_clock ;
}

void tuxMainLoop ()
{
  master_clock++ ;

  tuxState   -> update () ;
  whaleState -> update () ;

  camera     -> update () ;
  gfx        -> update () ;
  gui        -> update () ;
  sound      -> update () ;
  gfx        -> done   () ;  /* Swap buffers! */

  update_hooks () ;
}


