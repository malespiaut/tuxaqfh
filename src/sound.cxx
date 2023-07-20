
#include "tux.h"

struct Sound
{
   char *fname ;
   slSample *s ;
   int   squeaky ;  /* True if needs to be squeaky playing as Gown */
} ;


static Sound sfx [] =
{
  { "wavs/ahh.wav"	, NULL, TRUE  },          
  { "wavs/ahooga.wav"	, NULL, FALSE },       
  { "wavs/barf.wav"	, NULL, TRUE  },         
  { "wavs/boing.wav"	, NULL, FALSE },        
  { "wavs/bonk.wav"	, NULL, FALSE },         
  { "wavs/burp.wav"	, NULL, TRUE  },         
  { "wavs/clap.wav"	, NULL, FALSE },         
  { "wavs/drip.wav"	, NULL, FALSE },         
  { "wavs/frog.wav"	, NULL, FALSE },         
  { "wavs/glasbk.wav"	, NULL, FALSE },       
  { "wavs/laser.wav"	, NULL, FALSE },        
  { "wavs/moo.wav"	, NULL, FALSE },          
  { "wavs/ow.wav"	, NULL, TRUE  },           
  { "wavs/pop.wav"	, NULL, FALSE },          
  { "wavs/sinkdr.wav"	, NULL, FALSE },
  { "wavs/tarzan.wav"	, NULL, TRUE  },          
  { "wavs/ugh.wav"	, NULL, TRUE  },
  { "wavs/wee.wav"	, NULL, TRUE  },
  { "wavs/whip.wav"	, NULL, FALSE },
  { "wavs/whistle.wav"	, NULL, FALSE },
  { "wavs/who.wav"	, NULL, FALSE },
  { "wavs/who_else.wav" , NULL, FALSE },
  { NULL, NULL }
} ;

static int music_off = FALSE ;
static int   sfx_off = FALSE ;

void SoundSystem::disable_music ()
{
  /*
  sched -> stopMusic () ;
  sched -> update    () ;  // Ugh! Nasty Kludge!
  sched -> update    () ;  // Ugh! Nasty Kludge!

  music_off = TRUE  ;
  */
}


void SoundSystem::change_track ( char *fname )
{
  /*
  if ( fname == NULL )
    fname = "" ;

  if ( strcmp ( fname, current_track ) != 0  )
  {
    strcpy ( current_track, fname ) ;

    if ( ! music_off )
      enable_music  () ;
  }
  */
}

void SoundSystem::enable_music ()
{
  /*
  sched -> stopMusic () ;

  if ( current_track [ 0 ] != '\0' )
    sched -> loopMusic ( current_track ) ;
 
  music_off = FALSE ;
  */
}


void SoundSystem::disable_sfx () { //sfx_off = TRUE  ;
                                   }
void SoundSystem:: enable_sfx () { //sfx_off = FALSE ; 
                                   }



void SoundSystem::playSfx ( int sfx_num )
{
  /*
  if ( ! sfx_off )
    sched -> playSample ( sfx[sfx_num].s, 1, SL_SAMPLE_MUTE, 2, NULL ) ;
    */
}


SoundSystem::SoundSystem ()
{
  /*
  sched = new slScheduler ;

  setSafetyMargin () ;

  for ( Sound *currsfx = &(sfx[0]) ; currsfx -> fname != NULL ; currsfx++ )
  {
    currsfx -> s  = new slSample ( currsfx -> fname, sched ) ;

    if ( currsfx -> squeaky && play_as_gown )
      currsfx -> s -> changeRate ( ( currsfx -> s -> getRate () * 2 ) / 3 ) ;
  }

  enable_sfx   () ;
  change_track ( "" ) ;
  enable_music () ;
  */
}


void SoundSystem::update ()
{
/*
  Comment this next line out if the
  sound causes big glitches on your
  IRIX machine!
*/

  //sched -> update () ;
}


