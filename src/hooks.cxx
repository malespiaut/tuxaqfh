#include "tux.h"

void to_uppercase ( char *s )
{
  while ( *s != '\0' )
  {
    if ( *s >= 'a' && *s <= 'z' )
      *s = *s - 'a' + 'A' ;

    s++ ;
  }
}

#define MODE_FORWARD     1
#define MODE_CYCLE       2
#define MODE_SHUTTLE     3
#define MODE_SINESHUTTLE 4

class AutoSwitchParam
{
public:
  AutoSwitchParam ()
  {
    handle = 0  ;
  }

  int on_or_off ;
  int handle ;
} ;


class AutoDCSParam
{
public:
  AutoDCSParam ()
  {
    sgSetCoord ( &init , 0, 0, 0, 0, 0, 0 ) ;
    sgSetCoord ( &delta, 0, 0, 0, 0, 0, 0 ) ;
    mode  = MODE_FORWARD ;
    phase =  0.0f ;
    cycle = 30.0f ;
    isDCS = TRUE  ;
  }

  sgCoord init  ;
  sgCoord delta ;
  int   mode  ;
  int   isDCS ;
  float phase ;
  float cycle ;
} ;


void autodcsHook ( ssgBranch *br, void *param )
{
  AutoDCSParam *p = (AutoDCSParam *) param ;

  sgCoord now, add ;

  sgCopyCoord ( & now, & ( p -> init  ) ) ;
  sgCopyCoord ( & add, & ( p -> delta ) ) ;

  float frames = (float) getClock () + p -> phase ;

  if ( p->cycle != 0.0 && p->mode != MODE_FORWARD )
  {
    if ( p->mode == MODE_SHUTTLE )
    {
      float cframes = fmod ( frames, p->cycle ) ;

      if ( cframes > p->cycle / 2.0f )
        frames = p->cycle - cframes ; 
      else
        frames = cframes ; 
    }
    else
    if ( p->mode == MODE_SINESHUTTLE )
      frames = sin ( frames * 2.0f * M_PI / p->cycle ) * p->cycle / 2.0f ;
    else
      frames = fmod ( frames, p->cycle ) ;
  }

  sgScaleVec3 ( add . xyz, frames ) ;
  sgScaleVec3 ( add . hpr, frames ) ;

  sgAddVec3 ( now . xyz, add . xyz ) ;
  sgAddVec3 ( now . hpr, add . hpr ) ;

  /*
    To avoid roundoff problems with very large values
    accumulated after long runs all rotations
    can be modulo-360 degrees.
  */

  now . hpr [ 0 ] = fmod ( now . hpr [ 0 ], 360.0 ) ;
  now . hpr [ 1 ] = fmod ( now . hpr [ 1 ], 360.0 ) ;
  now . hpr [ 2 ] = fmod ( now . hpr [ 2 ], 360.0 ) ;

  ((ssgBaseTransform *) br) -> setTransform ( & now ) ;
}


static void *autoTexOrDCSInit ( char *data, int isDCS )
{
  AutoDCSParam *param = new AutoDCSParam ;
  
  param -> isDCS = isDCS ;

  char *s = data ;

  to_uppercase ( s ) ;

  while ( s != NULL && *s != '\0' )
  {
    while ( *s > ' ' ) s++ ;     /* Skip previous token */
    while ( *s <= ' ' && *s != '\0' ) s++ ; /* Skip spaces */

    if ( *s == '\0' ) break ;

    float f ;

    if ( sscanf ( s,  "H=%f", & f ) == 1 ) param->delta . hpr [ 0 ] = f ; else
    if ( sscanf ( s,  "P=%f", & f ) == 1 ) param->delta . hpr [ 1 ] = f ; else
    if ( sscanf ( s,  "R=%f", & f ) == 1 ) param->delta . hpr [ 2 ] = f ; else
    if ( sscanf ( s,  "X=%f", & f ) == 1 ) param->delta . xyz [ 0 ] = f ; else
    if ( sscanf ( s,  "Y=%f", & f ) == 1 ) param->delta . xyz [ 1 ] = f ; else
    if ( sscanf ( s,  "Z=%f", & f ) == 1 ) param->delta . xyz [ 2 ] = f ; else
    if ( sscanf ( s,  "C=%f", & f ) == 1 ) param->cycle = f ; else
    if ( sscanf ( s,  "M=%f", & f ) == 1 ) param->mode  = (int) f ; else
    if ( sscanf ( s,  "O=%f", & f ) == 1 ) param->phase = f ; else
      fprintf ( stderr, "Unrecognised @autodcs string: '%s'\n", 
                   data ) ;
  }

  sgSetCoord ( & ( param -> init ), 0, 0, 0, 0, 0, 0 ) ;
  return param ;
}


void *autodcsInit ( ssgBranch **br, char *data )
{
  *br = new ssgTransform () ;

  return autoTexOrDCSInit ( data, TRUE ) ;
}


void *autotexInit ( ssgBranch **br, char *data )
{
  *br = new ssgTexTrans () ;

  return autoTexOrDCSInit ( data, FALSE ) ;
}

void *invisibleInit ( ssgBranch **br, char * )
{
  *br = new ssgInvisible () ;

  return NULL ;
}


SLAM_Program *current_prog     = NULL ;
ssgTransform *current_slam_dcs = NULL ;
float        *current_arg_list = NULL ;
float         global [ 1024 ] ;

float slam_settrans ( int, float *f )
{
  sgCoord c ;
  sgSetCoord ( & c, f, & f[3] ) ;
  current_slam_dcs -> setTransform ( & c ) ;
  return 0 ;
}


float slam_setpos ( int, float *f )
{
  current_slam_dcs -> setTransform ( f ) ;
  return 0 ;
}


float slam_dump ( int, float * )
{
  current_prog -> dump () ;
  return 0 ;
}


float slam_show ( int argc, float *f )
{
  char str [ 1024 ] ;

  sprintf ( str, "%s: ", current_slam_dcs -> getPrintableName () ) ;

  for ( int i = 0 ; i < argc ; i++ )
    sprintf ( & (str[strlen(str)]), "%f, ", f[i] ) ;

  stPrintf ( "%s", str ) ;
  return 0 ;
}


float slam_print ( int argc, float *f )
{
  printf ( "%s: ", current_slam_dcs -> getPrintableName () ) ;

  for ( int i = 0 ; i < argc ; i++ )
    printf ( "%f, ", f[i] ) ;

  printf ( "\n" ) ;
  return 0 ;
}


float slam_sin ( int, float *f )
{
  return (float) sin ( f[0] * SG_DEGREES_TO_RADIANS ) ;
}


float slam_cos ( int, float *f )
{
  return (float) cos ( f[0] * SG_DEGREES_TO_RADIANS ) ;
}


float slam_tan ( int, float *f )
{
  return (float) tan ( f[0] * SG_DEGREES_TO_RADIANS ) ;
}


float slam_atan2 ( int, float *f )
{
  return (float) atan2 ( f[0], f[1] ) * SG_RADIANS_TO_DEGREES ;
}


float slam_sqrt ( int, float *f )
{
  return (float) sqrt ( f[0] ) ;
}


float slam_getglobal ( int, float *f )
{
  return global [ (int)(f[0]) ] ;
}


float slam_setglobal ( int, float *f )
{
  global [ (int)(f[0]) ] = f[1] ;
  return 0.0f ;
}


float slam_getarg ( int, float *f )
{
  return current_arg_list == NULL ? 0.0f :
                           current_arg_list [ (int)f[0] ] ;
}


float slam_getTuxRange ( int, float * )
{
  sgMat4  m ;

  current_slam_dcs -> getTransform ( m ) ;

  return sgDistanceVec3 ( m[3], tuxState->coord.xyz ) ;
}


float slam_getTuxHeading ( int, float * )
{
  sgVec3 hpr ;
  sgMat4  m ;

  current_slam_dcs -> getTransform ( m ) ;
  sgSubVec3 ( m[3], tuxState->coord.xyz ) ;
  sgHPRfromVec3 ( hpr, m[3] ) ;

  return hpr[0] ;
}


SLAM_Extension extensions[] =
{
  /* Handy math functions */

  { "sin"     , 1, slam_sin      },
  { "cos"     , 1, slam_cos      },
  { "tan"     , 1, slam_tan      },
  { "atan2"   , 2, slam_atan2    },
  { "sqrt"    , 1, slam_sqrt     },

  /* Standard Utility functions */

  { "print"   ,-1, slam_print    },
  { "show"    ,-1, slam_show     },
  { "dump"    , 0, slam_dump     },
  { "getarg"  , 1, slam_getarg   },
  { "setglobal"  , 2, slam_setglobal },
  { "getglobal"  , 1, slam_getglobal },

  /* DCS motion commands */

  { "setpos"  , 3, slam_setpos   },
  { "settrans", 6, slam_settrans },

  /* Game-specific stuff */

  { "getTuxRange", 0, slam_getTuxRange },
  { "getTuxHeading", 0, slam_getTuxHeading },
  { NULL      , 0, NULL          }
} ;



void slamdcsHook ( ssgBranch *br, void *param )
{
  if ( param == NULL )
    return ;

  current_prog     = (SLAM_Program *) param ;
  current_slam_dcs = (ssgTransform *) br ;
  current_arg_list = NULL ;

  int i ;

  for ( i = 0 ; i < 1000 &&
         current_prog -> step () == SLAM_PROGRAM_CONTINUE ; i++ )
    /* Run SLAM code */ ;

  if ( i >= 1000 )
    fprintf ( stderr,
       "WARNING: SLAM program ran >1000 instructions without a pause\n" ) ;
}


void *slamdcsInit ( ssgBranch **br, char *param )
{
  *br = new ssgTransform () ;
  
  while ( *param != '\"' && *param != '\0' )
    param++ ;

  if ( *param != '\"' )
    return NULL ;

  param++ ;

  char fname [ 128 ] ;
  int i ;

  for ( i = 0 ; param[i] != '\"' && param[i] != '\0' ; i++ )
    fname [ i ] = param [ i ] ;

  fname [ i ] = '\0' ;

  FILE *fd = fopen ( fname, "r" ) ;

  if ( fd == NULL )
  {
    fprintf ( stderr, "Failed to load SLAM code from '%s'\n", fname ) ;
    return NULL ;
  }

  SLAM_Program *prog = new SLAM_Program ( extensions ) ;

  fprintf ( stderr, "Loading Slamcode from '%s'\n", fname ) ;

  prog -> parse ( fd ) ;
  
  fclose ( fd ) ;

  return prog ;
}


void switchHit ( ssgBranch * /*br*/, void *param )
{
  AutoSwitchParam *p = (AutoSwitchParam *) param ;

  if ( p -> on_or_off )
    getHandle ( p->handle ) ->
               setTraversalMaskBits ( SSGTRAV_CULL|SSGTRAV_ISECT|SSGTRAV_HOT ) ;
  else
    getHandle ( p->handle ) ->
               clrTraversalMaskBits ( SSGTRAV_CULL|SSGTRAV_ISECT|SSGTRAV_HOT ) ;
}


void *switchOnInit ( ssgBranch **br, char *data )
{
  while ( ! isdigit ( *data ) && *data != '\0' )
    data++ ;

  if ( *data == '\0' )
  {
    fprintf ( stderr, "Missing handle in model.\n" ) ;
    *br = new ssgBranch ;
    return NULL ;
  }

  AutoSwitchParam *param = new AutoSwitchParam ;
  param -> handle    = atoi ( data ) ;
  param -> on_or_off = TRUE ;

  *br = new ssgBranch ;
  return param ;
}

void *switchOffInit ( ssgBranch **br, char *data )
{
  while ( ! isdigit ( *data ) && *data != '\0' )
    data++ ;

  if ( *data == '\0' )
  {
    fprintf ( stderr, "Missing handle in model.\n" ) ;
    *br = new ssgBranch ;
    return NULL ;
  }

  AutoSwitchParam *param = new AutoSwitchParam ;
  param -> handle = atoi ( data ) ;
  param -> on_or_off = FALSE ;

  *br = new ssgBranch ;
  return param ;
}


void animHook ( ssgBranch * /*br*/, void * /*param*/ )
{
}


void *animInit ( ssgBranch **br, char *data )
{
  while ( ! isdigit ( *data ) && *data != '\0' )
    data++ ;

  int   startlim = strtol ( data, &data, 0 ) ;
  int   endlim   = strtol ( data, &data, 0 ) ;
  float timelim  = strtod ( data, &data ) ;

  while ( *data <= ' ' && *data != '\0' )
    data++ ;

  char mode = toupper ( *data ) ;

  *br = new ssgTimedSelector ;

  ((ssgTimedSelector *)(*br)) -> setLimits   ( startlim+1, endlim+1 ) ;
  ((ssgTimedSelector *)(*br)) -> setDuration ( timelim ) ;
  ((ssgTimedSelector *)(*br)) -> setMode     ( (mode=='O') ?  SSG_ANIM_ONESHOT :
                                               (mode=='S') ?  SSG_ANIM_SWING :
                                                              SSG_ANIM_SHUTTLE ) ;
  ((ssgTimedSelector *)(*br)) -> control     ( SSG_ANIM_START ) ;

  return NULL ;
}


void snowballHit ( ssgBranch *, void * )
{
  tuxState -> rechargeSnowballs () ;
}


void *snowballInit ( ssgBranch **br, char * )
{
  *br = new ssgBranch ;
  return NULL ;
}



void *billboardInit ( ssgBranch **br, char * )
{
  *br = new ssgCutout ;
  return NULL ;
}

void tuxcopterHook ( ssgBranch *, void * )
{
}

void *tuxcopterInit ( ssgBranch **br, char * )
{
  *br = new ssgTransform ;
  return NULL ;
}

#ifdef STARWINGS_FLY_CIRCLES
void starwingHook ( ssgBranch *br, void *param )
#else
void starwingHook ( ssgBranch *, void *param )
#endif
{
  ((StarWing *) param ) -> update () ;

#ifdef STARWINGS_FLY_CIRCLES
  ssgTransform *tr = (ssgTransform *) br ;

  int tick = getClock () % 144 ;

  float h = (float) tick * 2.5f ;

  sgCoord crd ;
  crd.hpr [ 0 ] =    h   ;
  crd.hpr [ 1 ] = -30.0f ;
  crd.hpr [ 2 ] =   0.0f ;

  crd.xyz [ 0 ] =  30.0f * sin ( h * SG_DEGREES_TO_RADIANS ) ;
  crd.xyz [ 1 ] = -30.0f * cos ( h * SG_DEGREES_TO_RADIANS ) ;
  crd.xyz [ 2 ] = 5.0f ;

  tr -> setTransform ( & crd ) ;
#endif
}

void *starwingInit ( ssgBranch **br, char * )
{
  *br = new ssgTransform ;
  return new StarWing ( (ssgTransform *)*br ) ;
}

