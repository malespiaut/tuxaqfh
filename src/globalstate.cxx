#include "tux.h"

extern GUI *gui ;

void GlobalState::keyboardInput ( int c )
{
  /* Change level if needed */

  if ( new_level >= 0 )
  {
    level = new_level ;
    new_level = -9999 ;
    switchDatabase ( level ) ;
  }

  if ( c <= 0 )
    return ;

  if ( c != 'p' )
    pause = FALSE ;

  static int cheat_counter = 0 ;

  /*
    OK, if you can be bothered to read this much source
    code, you deserve to know that you can type "Tuxedo"
    (capital 'T') at any time during the game and turn
    on a bunch of keyboard cheat options. Press 'x' to
    go back to honest mode.

    Just let's keep this a secret between you and me - OK?
  */

  if ( ! cheat_mode )
  {
    if ( ( cheat_counter==0 && c=='T' ) || ( cheat_counter==1 && c=='u' ) ||
         ( cheat_counter==2 && c=='x' ) || ( cheat_counter==3 && c=='e' ) ||
         ( cheat_counter==4 && c=='d' ) || ( cheat_counter==5 && c=='o' ) )
    {
      cheat_counter++ ;

      if ( cheat_counter > 5 )
        cheat_mode = TRUE ;
    }
    else
      cheat_counter = 0 ;
  }
  else
  if ( c =='x' )
    cheat_mode = FALSE ;

  switch ( c )
  {
    case 0x1B /* Escape */      :
    case 0x03 /* Control-C */   : exit ( 0 ) ; break ;

    case 'h' : hide_status () ; help  () ; break ;
    case 'a' : hide_status () ; about () ; break ;

    case ' ' : if ( gui -> isHidden () )
	       {
		 sound->playSfx ( SOUND_WHO_ELSE ) ;
		 gui -> show () ;
	       }
	       else
	       {
		 sound->playSfx ( SOUND_UGH ) ;
		 gui -> hide () ;
	       }
	       break ;

    case 'p' : pause = ! pause ;
               break ;

    default : /* DO NOTHING */ break ;
  }

  if ( currState->cheat_mode )
  {
    switch ( c )
    {
      case '-' : switchDatabase ( --level ) ; break ; 
      case '+' : switchDatabase ( ++level ) ; break ;
 
      case '?' : tuxState->print () ;
                 break ;

      case 'c' : stToggle () ;
                 debug_enable = TRUE ;
                 break ;

      case 't' : texture_enable = ! texture_enable ;

                 if ( texture_enable )
                   glEnable ( GL_TEXTURE_2D ) ;
                 else
                   glDisable ( GL_TEXTURE_2D ) ;

                 ssgOverrideTexture ( ! texture_enable ) ;
                 break ;

      case 'w' : wireframe_enable = ! wireframe_enable ;

                 if ( wireframe_enable )
                   glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE ) ;
                 else
                   glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL ) ;
                 break ;

      case 'f' : currState->freeze_baddies = ! currState->freeze_baddies ;
                 break ;

      case 'r' : tuxState -> much_better () ;
                 tuxState -> grabbedLife () ;
                 tuxState -> grabbedLife () ;
                 tuxState -> breathe () ;
                 tuxState -> snowballs = 14 ;
                 tuxState -> energy = 1000 ;
                 break ;

      default : /* DO NOTHING */ ;
    }
  }
}


