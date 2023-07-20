
#define SOUND_AHH		0
#define SOUND_AHOOGA		1
#define SOUND_BARF		2
#define SOUND_BOING		3
#define SOUND_BONK		4
#define SOUND_BURP		5
#define SOUND_CLAP		6
#define SOUND_DRIP		7
#define SOUND_FROG		8
#define SOUND_GLASBK		9
#define SOUND_LASER		10
#define SOUND_MOO		11
#define SOUND_OW		12
#define SOUND_POP		13
#define SOUND_SINKDR		14
#define SOUND_TARZAN		15
#define SOUND_UGH		16
#define SOUND_WEE		17
#define SOUND_WHIP		18
#define SOUND_WHISTLE		19
#define SOUND_WHO		20
#define SOUND_WHO_ELSE		21
#define SOUND_FEET    		22
 
class SoundSystem
{
  char current_track [ 256 ] ;
  slScheduler *sched ;

public:
  SoundSystem () ;

  void update () ;
  void playSfx ( int sound ) ;

  /*
  void setSafetyMargin ( float t = 0.25 )
  {
    sched -> setSafetyMargin ( t ) ;
  }
  */

  void  change_track ( char *fname ) ;
  void disable_music () ;
  void  enable_music () ;

  void disable_sfx   () ;
  void  enable_sfx   () ;
} ;

