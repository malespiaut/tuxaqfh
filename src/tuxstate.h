
#define OUCH_STEP     1 
#define INIT_LIVES    2
#define MAX_LIVES     6
#define HOLD_BREATH_TIME  2400

#define HEATING_RATE    (MAX_TEMPERATURE / (30.0f * 30.0f))
#define MAX_TEMPERATURE 39.0f

class TuxState
{
  float bob_offset      ;
  float bob_delta       ;

  JoyInfo ji ;
  int joy_hit_timer ;
  float right_left  ;
  float fore_aft    ;

  /* Shadow-related stuff */

  Shadow       *shadow       ;
  sgCoord       shadow_coord ;
  ssgTransform *shadow_dcs   ;

  void checkShadow ( float h ) ;
  void checkTransporters    () ;
  void checkHerrings        () ;
  void fireSnowball         () ;

public:

  int   over_water      ;
  int   in_blackhole    ;
  int   on_steep_slope  ;
  int   on_sproingpad   ;
  int   in_danger       ;
  float temperature     ;
  int   too_hot         ;
  int   on_ground       ;
  int   swimming        ;
  int   diving          ;
  int   sliding         ;
  int   flapping        ;
  float energy          ;
  int   snowballs       ;
  float life            ;
  int   lives           ;
  int   dive_timer      ;
  int   scorch_timer    ;
  int   slide_timer     ;
  int   roll_timer      ;
  int   dizzy_timer     ;
  float squish_amount   ;
  float squish_pain     ;
  int   squished        ;
  int   just_died       ;

  int   super_tux       ;
  int   space_helmet    ;
  int   have_rocket     ;
  int   have_gun        ;
  int   have_hat        ;
  int   have_tie        ;
  int   have_shades     ;

  int   forced_spin_delay ;
  int   forced_spin_speed ;

  int   rc_mode         ;

  sgCoord coord         ;
  sgVec3  abs_velocity     ;
  sgVec3  rel_acceleration ;
  sgVec3  abs_acceleration ;
  sgMat4  mat           ;
  sgVec3  normal        ;
  sgVec3  surface_avoidance_vector ;
  float   floatation    ;

  sgVec3  skycol ;

  char *death_reason  ; 
  char *help_message  ;
  char *help_message2 ;

  Penguin      *tux ;
  ssgTransform *tux_dcs ;

public:

  TuxState ( int be_gown ) ;
  void init   ( int be_gown ) ;
  void update () ;

  void  doSwimming ( float hot ) ;
  void  doDiving   ( float hot ) ;
  void  doSpinning ( float hot ) ;
  void  doWalking  ( float hot ) ;
  void  doRolling  ( float hot, float friction ) ;
  void  doSliding  ( float hot, float friction ) ;
  void  doDizzy    ( float hot ) ;
  void  doFreeFall ( float hot ) ;

  float updateBob () ;
  void  joystickInput ( JoyInfo *j ) ;
  float collectIsectData   ( sgVec3 st, sgVec3 en, sgVec3 platform_velocity,
                                           float *friction, int firsttime ) ;
  float getIsectData       ( sgVec3 st, sgVec3 en, sgVec3 platform_velocity,
                                            float *friction, int firsttime ) ;

  void addTo ( ssgBranch *scene )
  {
    scene -> addKid ( shadow_dcs ) ;
    scene -> addKid (    tux_dcs ) ;
  }

  void getTransform ( sgMat4 m )
  {
    tux_dcs -> getTransform ( m ) ;
  }

  void setup () ;
  void setDefault ( sgVec3 xyz, sgVec3 hpr ) ;

  void print () ;

  void setSuperTux ( int x ) { super_tux   = x ; if ( x ) { have_rocket = FALSE; sound->playSfx(SOUND_FROG); } }
  void setRocket   ( int x ) { have_rocket = x ; if ( x ) { super_tux   = FALSE; sound->playSfx(SOUND_WHIP); } }
  void setGun      ( int x ) { have_gun    = x ; if ( x ) { setRocket ( TRUE ) ; sound->playSfx(SOUND_MOO ); } }
  void setPartyHat ( int x ) { have_hat    = x ; if ( x ) sound->playSfx(SOUND_TARZAN); }
  void setBowTie   ( int x ) { have_tie    = x ; if ( x ) sound->playSfx(SOUND_AHOOGA); }
  void setShades   ( int x ) { have_shades = x ; if ( x ) sound->playSfx(SOUND_GLASBK); }
  void setRCMode   ( int x ) { rc_mode     = x ; }

  int getSuperTux () { return super_tux   ; }
  int getRocket   () { return have_rocket ; }
  int getGun      () { return have_gun    ; }
  int getPartyHat () { return have_hat    ; }
  int getBowTie   () { return have_tie    ; }
  int getShades   () { return have_shades ; }
  int getRCMode   () { return rc_mode     ; }
  void breathe    () { dive_timer = HOLD_BREATH_TIME ; }

  void grabbedLife ()
  {
    breathe () ;
    life = 8 ;
    if ( lives < MAX_LIVES )
      lives++ ;
  }

  void kill ( char *reason )
  {
    death_reason = reason ;
    memorial () ;
    temperature = 0.0f ;
    breathe () ;
    life = 8 ;
    lives-- ;

    if ( lives <= 0 )
    {
      just_died = TRUE ;
      lives = INIT_LIVES ;
      currState -> new_level = (currState->level <= LEVEL_TRAINING ) ?
                                      LEVEL_TRAINING : 
                                      LEVEL_ANTECHAMBER ;
    }
    else
    {
      just_died = FALSE ;
      currState -> new_level = currState->level ;
    }
  }

  void gasp () ;
  void blub () ;
  void scorch () ;

  void much_better ()
  {
    life = 8 ;
  }

  void better ()
  {
    life += OUCH_STEP ;

    if ( life > 8 )
      life = 8 ;
  }

  void ouch ( float amount = OUCH_STEP )
  {
    if ( life > amount )
      life -= amount ;
    else
      kill ( "had no health left." ) ;
  }

  void rechargeSnowballs () ;
  int needsCameraFollowMode () { return diving ; }
} ;

