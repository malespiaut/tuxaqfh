
#include "tux.h"

TuxLifeInstance  tuxlife  [ MAX_TUXLIFE  ] ;
HerringInstance  herring  [ MAX_HERRING  ] ;
RingInstance     ring     [ MAX_RING     ] ;
BubbleInstance   bubble   [ MAX_BUBBLE   ] ;
SmokePuffInstance smoke   [ MAX_SMOKE    ] ;
SnowballInstance snowball [ MAX_SNOWBALL ] ;
MessageInstance  messages [ MAX_MESSAGE  ] ;

#define MAX_HANDLE 100
ssgEntity *handle_list [ MAX_HANDLE ] ;

ssgEntity *getHandle ( int h )
{
  return handle_list [ h % MAX_HANDLE ] ;
}

int num_herring  = 0 ;
int num_tuxlife  = 0 ;
int num_rings    = 0 ;
int num_messages = 0 ;

Ocean     *ocean = NULL ;
MagicRing *magicRing    ;
Bubble    *bubble_obj   ;
SmokePuff *smokepuff_obj;
Snowball  *snowball_obj ;

TuxLife *tuxlife_h ;
Herring *silver_h  ;
Herring *gold_h    ;
Herring *red_h     ;
Herring *green_h   ;

/*
  I'm sick of half the machines on the planet supporting
  strcasecmp and the other half stricmp - so here is my own
  offering:
*/

static int _StrEqual ( char *s1, char *s2 )
{
  int l1 = (s1==NULL) ? 0 : strlen ( s1 ) ;
  int l2 = (s2==NULL) ? 0 : strlen ( s2 ) ;

  if ( l2 != l1 )
    return FALSE ;

  for ( int i = 0 ; i < l1 ; i++ )
  {
    char c1 = s1[i] ;
    char c2 = s2[i] ;

    if ( c1 == c2 )
     continue ;

    if ( c1 >= 'a' && c1 <= 'z' )
      c1 = c1 - ('a'-'A') ;

    if ( c2 >= 'a' && c2 <= 'z' )
      c2 = c2 - ('a'-'A') ;

    if ( c1 != c2 )
     return FALSE ;
  }

  return TRUE ;
}



void scanCoord ( sgCoord *c, char *s )
{
  if ( sscanf ( s, "%f,%f,%f,%f,%f,%f",
		& (c->xyz[0]), & (c->xyz[1]), & (c->xyz[2]),
		& (c->hpr[0]), & (c->hpr[1]), & (c->hpr[2]) ) != 6 )
  {
    if ( sscanf ( s, "%f,%f,%f",
		  & (c->xyz[0]), & (c->xyz[1]), & (c->hpr[0]) ) != 3 )
    {
      sscanf ( s, "%f,%f", & (c->xyz[0]), & (c->xyz[1]) ) ;
      c->hpr[0] = 0.0f ;
    }
    c->xyz[2] = 1000000.0f ;
    c->xyz[2] = getHeight ( c->xyz ) ;
    c->hpr[1] = c->hpr[2] = 0.0f ;
  }
}



int   exit_level        =   0     ;
float time_limit        = 9999.0f ;
int   time_limit_handle =  -1     ;

float get_level_clock ()
{
  return time_limit ;
}

void check_level_clock ( float time )
{
  if ( time <= time_limit && time_limit_handle >= 0 )
  {
    ssgEntity *prize = getHandle ( time_limit_handle ) ;

    if ( prize != NULL )
      prize -> clrTraversalMaskBits ( SSGTRAV_CULL|SSGTRAV_ISECT|SSGTRAV_HOT ) ;
  }
}

int getExitLevel ( sgVec3 xyz )
{
  if ( exit_level < 0 )
  {
    /* When exiting from the lobby, where you go
       depends on you coordinate when you leave! */

    int a = (int) floor ( atan2 ( xyz[1], xyz[0] ) *
                                 SG_RADIANS_TO_DEGREES / 45.0 + 0.5 ) ;

    if ( a < 0 ) a += 8 ;
    if ( a > 7 ) a -= 8 ;

    return a ;
  }
  else
    return exit_level ;
}

void MessageInstance::dump ()
{
  if ( tuxState -> help_message == message1 )
  {
    tuxState -> help_message  = NULL ;
    tuxState -> help_message2 = NULL ;
  }  

  delete message1 ;
  delete message2 ;
  message1 = message2 = NULL ;
}

void MessageInstance::test_and_show ( sgVec3 p )
{
  if ( message1 != NULL &&
       fabs ( p[0] - pos[0] ) < radius &&
       fabs ( p[1] - pos[1] ) < radius &&
       fabs ( p[2] - pos[2] ) < radius )
  {
    tuxState -> help_message  = message1 ;
    tuxState -> help_message2 = message2 ;
  }
  else
  if ( tuxState -> help_message == message1 )
  {
    tuxState -> help_message  = NULL ;
    tuxState -> help_message2 = NULL ;
  }  
}


void load_library ()
{
  tuxState   -> init ( play_as_gown ) ;
  whaleState -> init () ;

  magicRing    = new MagicRing ( 2.0, 2.0 ) ;
  bubble_obj   = new Bubble ;
  smokepuff_obj= new SmokePuff ;
  snowball_obj = new Snowball ;

  sgVec3 cyan   = { 0.4, 1.0, 1.0 } ;
  sgVec3 yellow = { 1.0, 1.0, 0.4 } ;
  sgVec3 red    = { 0.8, 0.0, 0.0 } ;
  sgVec3 green  = { 0.0, 0.8, 0.0 } ;

  tuxlife_h = new TuxLife () ;
  silver_h  = new Herring ( cyan   ) ;
  gold_h    = new Herring ( yellow ) ;
  red_h     = new Herring ( red    ) ;
  green_h   = new Herring ( green  ) ;
}


ssgRoot *scene = NULL ;


struct DBaseHook
{
  char *tag ;
  void  (*hook)(ssgBranch  *, void *) ;
  void *(*init)(ssgBranch **, char *) ;
  void  (*hit )(ssgBranch  *, void *) ;
} ;


DBaseHook hooktable [] =
{
  { "slamdcs"   , slamdcsHook   , slamdcsInit  , NULL        },
  { "autodcs"   , autodcsHook   , autodcsInit  , NULL        },
  { "autotex"   , autodcsHook   , autotexInit  , NULL        },
  { "tuxcopter" , tuxcopterHook , tuxcopterInit, NULL        },
  { "billboard" , NULL          , billboardInit, NULL        },
  { "invisible" , NULL          , invisibleInit, NULL        },
  { "starwing"  , starwingHook  , starwingInit , NULL        },
  { "switchon"  , NULL          , switchOnInit , switchHit   },
  { "switchoff" , NULL          , switchOffInit, switchHit   },
  { "animate"   , animHook      , animInit     , NULL        },
  { "snowballs" , NULL          , snowballInit , snowballHit },
  { NULL        , NULL          , NULL         , NULL        }
} ;


#define MAX_HOOKS 200

class Hook : public ssgBase
{
public:
  Hook ()
  {
    br    = NULL ;
    param = NULL ;
    hook  = NULL ;
    hit   = NULL ;
  }

  ssgBranch *br    ;
  void      *param ;
  void (*hook)(ssgBranch *, void *) ;
  void (*hit )(ssgBranch *, void *) ;
} ;


int next_hook = 0 ;
Hook *hooklist [ MAX_HOOKS ] ;


void init_hooks ()
{
  for ( int i = 0 ; i < MAX_HOOKS ; i++ )
  {
    hooklist [ i ] = new Hook ; 
    hooklist [ i ] -> ref () ;
  }
}


void hit_hook ( ssgEntity *ent, sgMat4 * /*last_mat*/, sgMat4 * /*curr_mat*/ )
{
  /*
*************************
Steve:
    Don't forget that last_mat and/or curr_mat
    are allowed to be NULL.
*************************
  */


  Hook *h = (Hook *) (ent -> getUserData ()) ;

  if ( ent -> isAKindOf ( ssgTypeBaseTransform() ) )
  {
    if ( ent -> isAKindOf ( ssgTypeTexTrans() ) )
    {
    }
    else
    {
    }
  }

  if ( h == NULL || h -> hit == NULL )
    return ;

  h -> hit ((ssgBranch *) ent, h -> param ) ;
}


void update_hooks ()
{
  int i ;

  for ( i = 0 ; i < MAX_SMOKE ; i++ )
    smoke [ i ] . update () ;

  for ( i = 0 ; i < MAX_BUBBLE ; i++ )
    bubble [ i ] . update () ;

  for ( i = 0 ; i < MAX_SNOWBALL ; i++ )
    snowball [ i ] . update () ;

  for ( i = 0 ; i < next_hook ; i++ )
    if ( hooklist [ i ] -> hook != NULL )
      hooklist [ i ] -> hook ( hooklist [ i ] -> br,
                               hooklist [ i ] -> param ) ;

  for ( i = 0 ; i < num_messages ; i++ )
    messages [ i ] . test_and_show ( tuxState -> coord.xyz ) ;
}


ssgBranch *process_userdata ( char *data )
{
  int handle ;

  ssgBranch *b = NULL ;

  if ( data == NULL || data [ 0 ] != '@' )
    return NULL ;

  data++ ;   /* Skip the '@' */

  if ( isdigit ( *data ) )
  {
    handle = strtol ( data, NULL, 10 ) ;

    while ( *data != '\0' && *data != '@' )
      data++ ;

    if ( *data != '@' )
    {
      b = new ssgBranch ;
      handle_list [ handle % MAX_HANDLE ] = b ;
      return b ;
    }

    data++ ;   /* Skip the '@' */
  }
  else
    handle = 0 ;
  
  for ( int i = 0 ; hooktable [ i ] . tag != NULL ; i++ )
  {
    char *s = hooktable [ i ] . tag ;

    if ( strncmp ( s, data, strlen ( s ) ) == 0 )
    {
      void *res = NULL ;

      if ( hooktable[i].init != NULL )
        res = (*(hooktable[i].init))( & b, data ) ;
      else
        b = new ssgBranch () ;

      if ( hooktable[i].hook != NULL ||
           hooktable[i].hit  != NULL )
      {
        hooklist [ next_hook ] -> hook  = hooktable[i].hook ;
        hooklist [ next_hook ] -> hit   = hooktable[i].hit  ;
        hooklist [ next_hook ] -> br    = b ;
        hooklist [ next_hook ] -> param = res ;

        b -> setUserData ( hooklist [ next_hook ] ) ;

        next_hook++ ;

        if ( next_hook >= MAX_HOOKS )
        {
          fprintf ( stderr, "More than %d hooknodes in database!\n", MAX_HOOKS ) ;
          exit ( 1 ) ;
        }
      }

      handle_list [ handle % MAX_HANDLE ] = b ;
      return b ;
    }
  }

  fprintf ( stderr, "No hook to match '%s'\n", data ) ;

  return NULL ;
}



class LevelLoaderStatus
{
public:
  char level_file [ 256 ] ;
  float cam_pan    ;
  float cam_lift   ;
  float cam_range  ;
  int   cam_set    ;
  int   cam_follow ;
  int   gold_herring_counter ;
  int   new_level  ;

  LevelLoaderStatus ( int _new_level )
  {
    new_level = _new_level ;
    sprintf ( level_file, "data/level%d.dat", _new_level ) ;
    cam_pan    = 0.0f  ;
    cam_lift   = 0.0f  ;
    cam_range  = 0.0f  ;
    cam_set    = FALSE ;
    cam_follow = FALSE ;
    gold_herring_counter = 0 ;
  }
} ;



/******************************************\
*                                          *
* These functions are called to parse      *
* individual commands from the levelXX.dat *
* file.                                    *
*                                          *
\******************************************/

static void music_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  while ( *s != '\"' && *s >= ' ' )
    s++ ;

  if ( s [ 0 ] == '\"' )
  {
    if ( s [ strlen (s) - 2 ] != '\"' )
      fprintf ( stderr, "ERROR: %s mismatched doublequotes: '%s %s'\n",
                                                   stat->level_file, str, s ) ;
    else
    {
      s [ strlen (s) - 2 ] = '\0' ;
      sound -> change_track ( s+1 ) ;
    }
  }
  else
    fprintf ( stderr, "ERROR: %s missing filename: '%s %s'\n",
                                                   stat->level_file, str, s ) ;
}


static void heat_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  sscanf ( s, "%f", & currState -> heating ) ;
}

static void space_helmet_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  tuxState -> space_helmet = TRUE ;
}

static void no_air_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  currState -> cant_breathe = TRUE ;
}

static void squish_pain_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  sscanf ( s, "%f", & tuxState->squish_pain ) ;
}

static void camera_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  stat->cam_set = TRUE ;
  sscanf ( s, "%f,%f,%f,%d",
	   &stat->cam_pan, &stat->cam_lift, &stat->cam_range, &stat->cam_follow ) ;
}

static void sky_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  sscanf ( s, "%f,%f,%f", & tuxState->skycol[0], & tuxState->skycol[1],
			  & tuxState->skycol[2] ) ;
}

static void blackhole_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  if ( ocean != NULL ) return ;
  ocean = new Ocean ( OCEAN_BLACKHOLE ) ;
  scene -> addKid ( ocean -> getRoot () ) ;
}

static void ocean_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  if ( ocean != NULL ) return ;
  ocean = new Ocean ( OCEAN_WATER ) ;
  scene -> addKid ( ocean -> getRoot () ) ;
}

static void ice_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  if ( ocean != NULL ) return ;
  ocean = new Ocean ( OCEAN_ICE ) ;
  scene -> addKid ( ocean -> getRoot () ) ;
}

static void lava_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  if ( ocean != NULL ) return ;
  ocean = new Ocean ( OCEAN_LAVA ) ;
  scene -> addKid ( ocean -> getRoot () ) ;
}

static void message_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  if ( num_messages >= MAX_MESSAGE )
  {
    fprintf ( stderr, "Too many messages\n" ) ;
    return ;
  }

  float f = 5.0f ;
  sgVec3 xyz ;

  messages[num_messages].message1 = new char [ 128 ] ;
  messages[num_messages].message2 = new char [ 128 ] ;
  
  sscanf ( s, "%f,%f,%f,%s %s", & xyz[0], & xyz[1], &f,
		      messages[num_messages].message1,
		      messages[num_messages].message2  ) ;
  xyz[2] = 1000000.0f ;
  xyz[2] = getHeight ( xyz ) - 0.1 ;

  char *p ;

  for ( p = messages[num_messages].message1 ; *p != '\0' ; p++ )
    if ( *p == '_' ) *p = ' ' ;

  for ( p = messages[num_messages].message2 ; *p != '\0' ; p++ )
    if ( *p == '_' ) *p = ' ' ;

  sgCopyVec3 ( messages[num_messages].pos, xyz ) ;
  messages[num_messages].radius = f ;

  num_messages++ ;
}


static void life_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  if ( num_tuxlife >= MAX_TUXLIFE )
  {
    fprintf ( stderr, "Too many Tux Lives in one level.\n" ) ;
    return ;
  }

  TuxLifeInstance *l = & tuxlife[num_tuxlife] ;
  sgCoord c ;
  scanCoord ( &c, s ) ;
  c.xyz[2] += 0.6 ;

  sgCopyVec3 ( l->xyz, c.xyz ) ;

  l->life  = tuxlife_h ;
  l->eaten = FALSE ;
  l->scs = new ssgTransform ;
  l->scs -> setTransform ( &c ) ;
  l->scs -> addKid ( l -> life -> getRoot () ) ;
  scene  -> addKid ( l -> scs ) ;    
  num_tuxlife++ ;
}

static void herring_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  if ( num_herring >= MAX_HERRING )
  {
    fprintf ( stderr, "Too many herring\n" ) ;
    return ;
  }

  HerringInstance *h = & herring[num_herring] ;
  char tag [ 256 ] ;
  sgVec3 xyz ;

  if ( sscanf ( s, "%f,%f,%f", &xyz[0], &xyz[1], &xyz[2] ) != 3 )
  {
    if ( sscanf ( s, "%f,%f,%s", &xyz[0], &xyz[1], tag ) != 3 )
    {
      sscanf ( s, "%f,%f", & xyz[0], & xyz[1] ) ;
      tag [ 0 ] = '\0' ;
    }

    xyz[2] = 1000000.0f ;
    xyz[2] = getHeight ( xyz ) + 0.6 ;
  }
  else
    tag [ 0 ] = '\0' ;

  if ( str[0]=='Y' || str[0]=='y' )
  {
    h -> ident = stat->new_level * 2 + stat->gold_herring_counter++ ;

    if ( herring_collected [ h -> ident ] )
    {
      /* Already got this one - turn it into a Silver Herring. */
      str[0] = 'S' ;
    }
  }

  sgCoord c ;

  sgCopyVec3 ( h->xyz, xyz ) ;
  sgSetVec3  ( c.hpr, 0.0f, 0.0f, 0.0f ) ;
  sgCopyVec3 ( c.xyz, h->xyz ) ;

  if ( str[0]=='Y' || str[0]=='y' ){ h->her = gold_h   ; h->type = HE_GOLD   ;}
  if ( str[0]=='G' || str[0]=='g' ){ h->her = green_h  ; h->type = HE_GREEN  ;}
  if ( str[0]=='R' || str[0]=='r' ){ h->her = red_h    ; h->type = HE_RED    ;}
  if ( str[0]=='S' || str[0]=='s' ){ h->her = silver_h ; h->type = HE_SILVER ;} 

  h->eaten = FALSE ;
  h->scs   = new ssgTransform ;
  h->scs -> setTransform ( &c ) ;
  h->scs -> addKid ( h->her->getRoot () ) ;
  scene  -> addKid ( h->scs ) ;    

  if ( _StrEqual ( tag, "ROCKET"   ) ) h->effect = EFFECT_ROCKET ; else
  if ( _StrEqual ( tag, "GUN"      ) ) h->effect = EFFECT_GUN ; else
  if ( _StrEqual ( tag, "SUPERTUX" ) ) h->effect = EFFECT_SUPERTUX ; else
  if ( _StrEqual ( tag, "PARTYHAT" ) ) h->effect = EFFECT_PARTYHAT ; else
  if ( _StrEqual ( tag, "BOWTIE"   ) ) h->effect = EFFECT_BOWTIE ; else
  if ( _StrEqual ( tag, "SHADES"   ) ) h->effect = EFFECT_SHADES ; else
                                       h->effect = EFFECT_DEFAULT ;

  num_herring++ ;
}

static void teleporter_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  if ( num_rings >= MAX_RING )
  {
    fprintf ( stderr, "Too many rings\n" ) ;
    return ;
  }

  sgCoord c ;
  scanCoord ( &c, s ) ;
  c.xyz[2] += 0.1 ;

  scene -> addKid ( ring[num_rings].setup ( magicRing->getRoot(), c.xyz ) ) ;
  num_rings++ ;
}

static void exit_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  sgCoord c ;

  ssgModelPath   ( "models" ) ;
  ssgTexturePath ( "images" ) ;

  sscanf ( s, "%f,%f,%d", & c.xyz[0], & c.xyz[1], &exit_level ) ;
  c.xyz[2] = 1000000.0f ;
  c.xyz[2] = getHeight ( c.xyz ) - 0.1 ;
  c.hpr[0] = c.hpr[1] = c.hpr[2] = 0.0f ;

  ssgTransform *tf = new ssgTransform ( & c ) ;

  ssgEntity *obj = ssgLoad ( "folly.ac", loader_opts ) ;
  tf -> addKid   ( obj ) ;
  ssgFlatten     ( obj ) ;
  ssgStripify    ( tf ) ;
  scene ->addKid ( tf ) ;
}

static void whale_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  sgCoord c ;
  scanCoord ( &c, s ) ;
  c.xyz[2] = -2.0f ;

  whaleState -> setEnable ( TRUE ) ;
  scene -> addKid ( whaleState -> getDCS ()  ) ;
  whaleState->setDefault ( c.xyz, c.hpr ) ;
}

static void race_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  sscanf ( s, "%f,%d", & time_limit, & time_limit_handle ) ;
}

static void tux_command ( LevelLoaderStatus *stat, char *s, char *str )
{
  sgCoord c ;
  scanCoord ( &c, s ) ;
  c.xyz[2] += 0.1f ;
  tuxState->setDefault ( c.xyz, c.hpr ) ;
}




struct LevelCommand 
{
  char *name ;
  void (*action)( LevelLoaderStatus *, char *, char * ) ;
} ;

LevelCommand levelFileCommands [] =
{
  { "MUSIC"       ,        music_command },
  { "TUX"         ,          tux_command },
  { "CAMERA"      ,       camera_command },
  { "WHALE"       ,        whale_command },
  { "RACE"        ,         race_command },
  { "EXIT"        ,         exit_command },
  { "TELEPORTER"  ,   teleporter_command },
  { "LIFE"        ,         life_command },
  { "MESSAGE"     ,      message_command },
  { "SKY"         ,          sky_command },
  { "HEAT"        ,         heat_command },
  { "SPACE_HELMET", space_helmet_command },
  { "NO_AIR"      ,       no_air_command },
  { "SQUISH_PAIN" ,  squish_pain_command },

  { "SHERRING"    ,      herring_command },
  { "GHERRING"    ,      herring_command },
  { "RHERRING"    ,      herring_command },
  { "YHERRING"    ,      herring_command },

  { "LAVA"        ,         lava_command },
  { "ICE"         ,          ice_command },
  { "BLACKHOLE"   ,    blackhole_command },
  { "OCEAN"       ,        ocean_command },

  { NULL, NULL }   /* End marker - do not remove! */
} ;



int parseDBaseString ( LevelLoaderStatus *stat, FILE *lfd )
{
  char str [ 1024 ] ;

  if ( feof ( lfd ) || fgets ( str, 1023, lfd ) == NULL )
    return FALSE ;

  if ( str[0] == '#' || str[0] < ' ' )
    return TRUE ;

  char *s ;

  for ( s = str ; *s > ' ' ; s++ )
    /* Skip to space */ ;

  *s++ = '\0' ;

  for ( LevelCommand *p = &(levelFileCommands[0]) ; p -> name != NULL ; p++ )
    if ( _StrEqual ( str, p->name ) )
    {
      (p -> action)( stat, s, str ) ;
      return TRUE ;
    }

  /*
    If we didn't match any commands and the line doesn't start with
    double-quotes then it's an error.
  */

  if ( str [ 0 ] != '\"' )
  {
    fprintf ( stderr, "ERROR: %s contains: '%s %s'\n",
                                                   stat->level_file, str, s ) ;
    return FALSE ;
  }

  /*
    If it doesn't end with matching doublequotes - then that's an
    error too.
  */

  if ( str [ strlen (str) - 1 ] != '\"' )
  {
    fprintf ( stderr, "ERROR: %s mismatched doublequotes: '%s %s'\n",
                                                   stat->level_file, str, s ) ;
    return FALSE ;
  }    

  /*
    So - it's a model load command.
  */

  sgCoord c ;

  ssgModelPath   ( "models" ) ;
  ssgTexturePath ( "images" ) ;

  str [ strlen (str) - 1 ] = '\0' ;

  scanCoord ( &c, s ) ;
  c.xyz[2] -= 0.1 ;

  ssgTransform *tf = new ssgTransform ( & c ) ;

  ssgEntity *obj = ssgLoad ( str+1, loader_opts ) ;
  tf -> addKid ( obj ) ;
  ssgFlatten  ( obj ) ;
  ssgStripify ( tf ) ;
  scene ->addKid ( tf ) ;

  return TRUE ;
}

void switchDatabase ( int new_level )
{
  if ( new_level < 0 ) new_level = LEVEL_TRAINING ; 

  LevelLoaderStatus status ( new_level ) ;

  int i ;

  next_hook = 0 ;

  currState -> level = status.new_level ;

  if ( status.new_level == LEVEL_ANTECHAMBER )
  {
    if ( tuxState -> just_died )
    {
      parse_tuxrc ( & status.new_level ) ;
      currState -> level = status.new_level ;
    }
    else
      save_tuxrc () ;
  }

  for ( i = 0 ; i < MAX_MESSAGE ; i++ )
    messages [ i ] . dump () ;

  hide_stopwatch () ;

  if ( sound == NULL )
  {
    fprintf(stderr,"SOUND IS NULL...TELL <sjbaker1@airmail.net>!!\n" ) ;
    exit ( 1 ) ;
  }

  /*
    Frantic attempt to keep sound going during level switch.
    This is hard because the sound buffer is fairly short on
    some OS's and doing stuff like loading files can take time.
    So, we set the safety margin up to two seconds and update
    the sound system as often as possible.  It seems to work OK.
  */

  //sound -> setSafetyMargin ( 2.0f ) ;
  //sound -> update () ;

  if ( scene )
  { 
    ssgDelete ( scene ) ;
    delete ocean ;
    ocean = NULL ;
  }
  else
    load_library () ;

  scene = new ssgRoot ;

  sound -> update () ;

  tuxState -> addTo ( scene ) ;

  sound -> update () ;

  whaleState -> setEnable ( FALSE ) ;

  sgVec3 xyz ;

  currState -> cant_breathe = FALSE     ;
  currState -> heating      = 0.0f      ;

  tuxState -> squish_pain   = 3     ;
  tuxState -> space_helmet  = FALSE ;

  time_limit = 9999.0f ;
  time_limit_handle = -1 ;

  num_herring  = 0 ;
  num_tuxlife  = 0 ;
  num_rings    = 0 ;
  num_messages = 0 ;

  xyz [ 0 ] = xyz [ 1 ] = xyz [ 2 ] = -2000000.0f ;

  sound -> update () ;
  for ( i = 0 ; i < MAX_SMOKE ; i++ )
    scene -> addKid ( smoke[i].setup ( smokepuff_obj->getRoot(), xyz ) ) ;
  sound -> update () ;
  for ( i = 0 ; i < MAX_BUBBLE ; i++ )
    scene -> addKid ( bubble[i].setup ( bubble_obj->getRoot(), xyz ) ) ;
  sound -> update () ;
  for ( i = 0 ; i < MAX_SNOWBALL ; i++ )
    scene -> addKid ( snowball[i].setup ( snowball_obj->getRoot(), xyz ) ) ;

  sound -> update () ;

  FILE *lfd = fopen ( status.level_file, "r" ) ;

  if ( lfd == NULL )
  {
    fprintf ( stderr, "Can't open %s for reading\n", status.level_file ) ;

    sprintf ( status.level_file, "data/the_end.dat" ) ;

    lfd = fopen ( status.level_file, "r" ) ;

    if ( lfd == NULL )
    {
      fprintf ( stderr, "Can't open %s for reading either!\n", status.level_file ) ;
      exit ( 1 ) ;
    }
  }

  do
  {
    sound -> update () ;
  } while ( parseDBaseString ( & status, lfd ) ) ;

  fclose ( lfd ) ;

  sound -> update () ;
  camera -> immediate_reset () ;

  if ( status.cam_set )
  {
    camera -> setPan    ( status.cam_pan    ) ;
    camera -> setLift   ( status.cam_lift   ) ;
    camera -> setZoom   ( status.cam_range  ) ;
    camera -> setFollow ( status.cam_follow ) ;
  }

  sound -> update () ;

#ifdef DEBUG
  fprintf ( stderr, "Level Loaded, %gMtexels consumed.\n",
           (float)(ssgGetNumTexelsLoaded()/1049)/1000.0f ) ;
#endif

  //sound -> setSafetyMargin () ;
  //sound -> update () ;
}


