#include "tux.h"

void WhaleState::update ()
{
  if ( !enable )
  {
    sgCoord c ;
    sgSetCoord ( &c, 1000000.0f, 1000000.0f, 1000000.0f, 0.0f, 0.0f, 0.0f ) ;
    whale -> setCoord ( & c ) ;
    return ;
  }


  float hot = getHeightAndNormal ( coord.xyz, normal ) ;

  if ( hot < -1000.0f )
    hot = coord.xyz[2]+0.1 ;

  /*
    While we are underground, back out along the terrain
    surface normal.
  */

  while ( coord.xyz[2] <= hot+2.0f && coord.xyz[2] <= 0.0f )
  {
    sgAddVec3 ( coord.xyz, coord.xyz, normal ) ;
    hot = getHeightAndNormal ( coord.xyz, normal ) ;

    if ( hot < -1000.0f )
      hot = coord.xyz[2]+0.1 ;
  }

  if ( munch_timer > 0 )
    munch_timer-- ;

  sgVec3 to_tux   ;
  sgVec3 heading ;

  sgSubVec3 ( to_tux, tuxState->coord.xyz, coord.xyz ) ;
  sgHPRfromVec3 ( heading, to_tux ) ;

  float dx = fabs ( to_tux [ 0 ] ) ;
  float dy = fabs ( to_tux [ 1 ] ) ;
  float dz = fabs ( to_tux [ 2 ] ) ;

  float range = sqrt ( dx * dx + dy + dy ) ;

  if ( ! currState->freeze_baddies && munch_timer <= 0 &&
         dx < 2.0 && dy < 2.0 && dz < 2.0 )
  {
    sgSetVec3 ( velocity, 0.0f, -0.1f, 0.0f ) ;
    sound->playSfx ( SOUND_OW ) ;
    tuxState -> ouch () ;
    munch_timer = 100 ;
  }

  /*
    If we are in nice deep water (at least 20 meters below)
    then we have the luxury of chasing Tux. If Tux isn't
    in the water then we can at least stretch our flukes
    with a nice fast run.
  */

  if ( hot+10.0f < coord . xyz[2] )
  {
    /* If tux is over water then we chase him.  */

    if ( tuxState->over_water && munch_timer <= 0 )
    {
      if ( range > 10.0 )
      {
	coord.hpr[0] = blend_angle ( coord.hpr[0], heading[0] ) ;
	coord.hpr[1] = blend_angle ( coord.hpr[1], 0.0f ) ;
	sgSetVec3 ( velocity, 0.0f, 0.5f, 0.0f ) ;
      }
      else
      {
	sgSetVec3 ( velocity, 0.0f, range/10.0f + 0.1, 0.0f ) ;

	coord.hpr[0] = heading[0] ;
	coord.hpr[1] = blend_angle ( coord.hpr[1], -heading[1] ) ;

	if ( dx < 1.0  && dy < 1.0 && dz < 1.0 )
	  sgSetVec3 ( velocity, 0.0f, -0.1f, 0.0f ) ;
      }
    }
    else
      sgSetVec3 ( velocity, 0.0f, 0.5f, 0.0f ) ;
  }
  else
  {
    /*
      We are in shallow water - let's try to get back
      into deeper water.
    */

    int tquad ;
    int oquad ;

    /*
      Figure out which quadrant the normal is pointing
      towards:
			    |
			 1  |  0
		      ______|______
			    | 
			 2  |  3
			    |
    */

    if ( normal [ 0 ] > 0 )
      if ( normal [ 1 ] > 0 ) tquad = 0 ; else tquad = 3 ;
    else
      if ( normal [ 1 ] > 0 ) tquad = 1 ; else tquad = 2 ;

    /*
      Figure out which quadrant the whale is pointing towards...
    */

    while ( coord.hpr[0] < 0.0f )
      coord.hpr [ 0 ] += 360.0f ;

    int heading = (int) coord.hpr [ 0 ] % 360 ;

    if ( heading <  90 ) oquad = 1 ; else
    if ( heading < 180 ) oquad = 2 ; else
    if ( heading < 270 ) oquad = 3 ; else oquad = 0 ;

    if ( oquad == 0 && tquad == 3 ) tquad = -1 ;
    if ( oquad == 3 && tquad == 0 ) tquad =  4 ;

    if ( oquad == tquad )
      sgSetVec3 ( velocity, 0.0f, 0.5f,-0.1f ) ;
    else
    if ( oquad+1 == tquad )
    {
      coord.hpr[0] += 2.0 ;
      sgSetVec3 ( velocity, 0.0f, 0.2f, 0.0f ) ;
    }
    else
    if ( oquad-1 == tquad )
    {
      coord.hpr[0] -= 2.0 ;
      sgSetVec3 ( velocity, 0.0f, 0.2f, 0.0f ) ;
    }
    else
    {
      coord.hpr[0] -= 3.0 ;
      sgSetVec3 ( velocity, 0.0f, 0.03f,
		       (coord.xyz[2] >= 0.0) ? -0.1 : 0.2f ) ;
    }

    coord.hpr[1] = blend_angle ( coord.hpr[1], 0.0f ) ;
  }

  sgMat4 mat ;
  sgMakeCoordMat4 ( mat, &coord ) ;

  if ( ! currState->freeze_baddies )
  {
    sgXformPnt3 ( coord.xyz, velocity, mat ) ;

    if ( coord.xyz[2] > 0.0f )
      coord.xyz[2] = 0.0f ;
  }

  whale -> setCoord ( & coord ) ;
  whale -> update () ;
  dcs -> setTransform ( & coord ) ;
}

