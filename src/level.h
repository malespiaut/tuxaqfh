
extern Ocean     *ocean        ;
extern Bubble    *bubble_obj   ;
extern SmokePuff *smokepuff_obj;
extern Snowball  *snowball_obj ;
extern MagicRing *magicRing    ;
 
extern TuxLife *tuxlife_h ;
extern Herring *silver_h  ;
extern Herring *gold_h    ;
extern Herring *red_h     ;
extern Herring *green_h   ;

extern Ocean   *ocean    ;
extern Penguin *tux      ;
extern Orca    *whale    ;
extern Shadow  *sh       ;
extern MagicRing *magicRing ;

extern int num_herring   ;
extern int num_tuxlife   ;
extern int num_rings     ;
extern int num_messages  ;

#define EFFECT_DEFAULT     0
#define EFFECT_ROCKET      1
#define EFFECT_GUN         2
#define EFFECT_SUPERTUX    3
#define EFFECT_PARTYHAT    4
#define EFFECT_BOWTIE      5
#define EFFECT_SHADES      6


class ActiveThingInstance
{
public:
  sgVec3 xyz ;
  ssgTransform *scs ;

  ssgTransform *setup ( ssgEntity *thing, sgCoord *pos )
  {
    sgCopyVec3 ( xyz, pos->xyz );

    scs = new ssgTransform ;
    scs -> setTransform ( pos ) ;
    scs -> addKid ( thing ) ;

    return scs ;
  }

  ssgTransform *setup ( ssgEntity *thing, sgVec3 pos )
  {
    sgCoord c ;
    sgSetVec3  ( c.hpr, 0.0f, 0.0f, 0.0f ) ;
    sgCopyVec3 ( c.xyz, pos ) ;
    return setup ( thing, &c ) ;
  }

  int active () { return xyz [ 2 ] > -1000000.0f ; }

  void setPos ( sgVec3 pos )
  {
    sgCopyVec3 ( xyz, pos ) ;
  }

  virtual void update () = 0 ;
} ;

class SnowballInstance : public ActiveThingInstance
{
public:
  sgVec3 speed ;

  virtual void update ()
  {
    if ( ! active () )
      return ;

    if ( xyz [ 2 ] <= 0.0f )
      xyz [ 2 ] = -2000000.0f ;

    speed [ 2 ] -= 0.1 ;
    sgAddVec3 ( xyz, speed ) ;
    scs -> setTransform ( xyz ) ;
  }

  void setSpeed ( sgVec3 spd )
  {
    sgCopyVec3 ( speed, spd ) ;
  }
} ;

class SmokePuffInstance : public ActiveThingInstance
{
public:

  virtual void update ()
  {
    if ( ! active () )
      return ;

    if ( xyz [ 2 ] >= 100000.0f )
      xyz [ 2 ] = -2000000.0f ;

    xyz [ 2 ] += 0.2 ;
    scs -> setTransform ( xyz ) ;
  }
} ;


class BubbleInstance : public ActiveThingInstance
{
public:

  virtual void update ()
  {
    if ( ! active () )
      return ;

    if ( xyz [ 2 ] >= 0.0f )
      xyz [ 2 ] = -2000000.0f ;

    xyz [ 2 ] += 0.09 ;
    scs -> setTransform ( xyz ) ;
  }
} ;


class RingInstance : public ActiveThingInstance
{
public:
  virtual void update () {} ;
} ;


class TuxLifeInstance : public ActiveThingInstance
{
public:
  TuxLife *life ;
  int eaten ;
  virtual void update () {} ;
} ;

class HerringInstance : public ActiveThingInstance
{
public:
  Herring *her ;
  int eaten ;
  int type  ;
  int effect ;
  int ident ;   /* For Gold Herring! */
  virtual void update () {} ;
} ;


class MessageInstance
{
public:
  char  *message1 ;
  char  *message2 ;
  sgVec3 pos     ;
  float  radius  ;

  virtual void update () {} ;
  void test_and_show ( sgVec3 p ) ;
  void dump () ;
} ;


#define HE_RED    0
#define HE_GREEN  1
#define HE_GOLD   2
#define HE_SILVER 3

#define MAX_TUXLIFE  10
#define MAX_HERRING  50
#define MAX_RING      3
#define MAX_BUBBLE   10
#define MAX_SMOKE    20
#define MAX_SNOWBALL 14
#define MAX_MESSAGE  20
 
extern TuxLifeInstance  tuxlife  [ MAX_TUXLIFE  ] ;
extern HerringInstance  herring  [ MAX_HERRING  ] ;
extern RingInstance     ring     [ MAX_RING     ] ;
extern BubbleInstance   bubble   [ MAX_BUBBLE   ] ;
extern SmokePuffInstance smoke   [ MAX_SMOKE    ] ;
extern SnowballInstance snowball [ MAX_SNOWBALL ] ;
extern MessageInstance  messages [ MAX_MESSAGE  ] ;
 
extern ssgTransform *  sh_dcs ;
extern ssgTransform * tux_dcs ;
extern ssgTransform * whale_dcs ;

void  switchDatabase    ( int level ) ;
void  load_library      () ;
int   getExitLevel      ( sgVec3 xyz ) ;
void  check_level_clock ( float time ) ;
float get_level_clock   () ;
void  init_hooks        () ;

extern sgCoord whale_default ;

/* "Special" Level names */

#define LEVEL_TRAINING            0
#define LEVEL_ANTECHAMBER      9999

