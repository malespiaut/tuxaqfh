
#include "tux.h"

static inline float sq ( float x ) { return x * x ; }

static StarWing *first_starwing = NULL ;

#define ENERGY_RECOVERY_RATE    1.0f
#define ENERGY_FLAP_COST        3.0f
#define ENERGY_START_FLAPPING 200.0f

#define TOO_LOW_ALTITUDE    5.0f
#define TOO_HIGH_ALTITUDE  55.0f
#define TOO_CLOSE_DISTANCE  5.0f
#define TOO_CLOSE_HEADING  45.0f

#define MAX_SPEED       0.6f   /* Meter per frame */
#define WRAP_DISTANCE 300.0f
#define MAX_ALTITUDE   60.0f
#define MIN_ALTITUDE    1.0f

StarWing::StarWing ( ssgTransform *o )
{
  obj = o ;

  energy = (float)( rand () % 100 ) ;
  state = SG_RESTING ;

  coord . xyz [ 0 ] = (float) ( rand () % 100 ) ;
  coord . xyz [ 1 ] = (float) ( rand () % 100 ) ;
  coord . xyz [ 2 ] = (float) ( rand () % 100 + 10 ) ;
  coord . hpr [ 0 ] = (float) ( rand () % 360 ) ;
  coord . hpr [ 1 ] =  0.0f ;
  coord . hpr [ 2 ] =  0.0f ;

  while ( coord.xyz [ 0 ] >  WRAP_DISTANCE ) coord.xyz[0] -= WRAP_DISTANCE ;
  while ( coord.xyz [ 0 ] < -WRAP_DISTANCE ) coord.xyz[0] += WRAP_DISTANCE ;
  while ( coord.xyz [ 1 ] >  WRAP_DISTANCE ) coord.xyz[1] -= WRAP_DISTANCE ;
  while ( coord.xyz [ 1 ] < -WRAP_DISTANCE ) coord.xyz[1] += WRAP_DISTANCE ;

  if    ( coord.xyz [ 2 ] >  MAX_ALTITUDE  ) coord.xyz[2] = MAX_ALTITUDE ;
  if    ( coord.xyz [ 2 ] <  MIN_ALTITUDE  ) coord.xyz[2] = MIN_ALTITUDE ;

  last_coord = coord ;
  last_coord . xyz [ 0 ] -= (float)(rand()%200-100)/100.0f ;
  last_coord . xyz [ 1 ] -= (float)(rand()%200-100)/100.0f ;
  last_coord . xyz [ 2 ] -= (float)(rand()%10-5)/100.0f ;

  if ( first_starwing == NULL )
  {
    next = NULL ;
    first_starwing = this ;
  }
  else
  {
    next = first_starwing ;
    first_starwing = this ;
  }
}



void StarWing::find_nearest ( sgVec3 nearest )
{
  if ( first_starwing == NULL )
    return ;

  float dist = 10000000000000000000.0 ;

  for ( StarWing *i = first_starwing ; i != NULL ; i = i -> getNext () )
    if ( i != this )
    {
      float d = sgDistanceVec3 ( coord.xyz, i->getCoord()->xyz ) ;

      if ( d < dist )
      {
	dist = d ;
	sgCopyVec3 ( nearest, i->getCoord()->xyz ) ;
      }
    }
}



void find_flock_center ( sgVec3 center, sgVec3 dirn )
{
  sgSetVec3 ( center, 0, 0, 0 ) ;
  sgSetVec3 ( dirn  , 0, 1, 0 ) ;

  if ( first_starwing == NULL )
    return ;

  int n = 0 ;

  for ( StarWing *i = first_starwing ; i != NULL ; i = i -> getNext () )
  {
    n++ ;
    sgAddVec3 ( center, center, i -> getCoord () -> xyz ) ;
    sgAddVec3 ( dirn  , dirn  , i -> getDirn() ) ;
  }

  if ( n != 0 )
  {
    sgScaleVec3 ( center, center, 1.0 / (float) n ) ;
    sgNormalizeVec3 ( dirn ) ;
  }
}



void StarWing::strategy ( sgVec3 new_direction )
{
  sgVec3 force ;
  sgSetVec3 ( force, 0.0f, 0.0f, 0.0f ) ;

  /* Priority #1 - Don't hit the ground! */

  sgVec3 new_pos ;

  sgAddVec3 ( new_pos, coord.xyz, getDirn () ) ;

  if ( new_pos[2] > TOO_HIGH_ALTITUDE )
    force [ 2 ] -= 300.0 ;
  if ( new_pos[2] < TOO_LOW_ALTITUDE )
    force [ 2 ] += 1000.0 ;

  sgVec3 nearest   ;
  sgVec3 flk_center;
  sgVec3 flk_dirn  ;

  find_nearest ( nearest ) ;
  find_flock_center ( flk_center, flk_dirn ) ;

  sgAddVec3 ( force, force, flk_dirn  ) ; /* Direction to fly parallel to flock */

  if ( sgDistanceVec3 ( coord.xyz, nearest ) < TOO_CLOSE_DISTANCE )
  {
    sgSubVec3 ( nearest, nearest, coord.xyz ) ; /* Direction to nearest */
    sgSubVec3 ( force, force, nearest       ) ; /* Direction to avoid nearest */
  }
  else
  {
    sgSubVec3 ( nearest, flk_center, coord.xyz ) ;
    sgAddVec3 ( force, force, flk_dirn ) ;
    sgAddVec3 ( force, force, flk_dirn ) ;
    sgAddVec3 ( force, force, flk_dirn ) ;
    sgAddVec3 ( force, force, nearest ) ;
  }

  sgScaleVec3 ( force, force, 0.0001 ) ;
  sgAddVec3 ( force, force, dirn ) ;
  sgCopyVec3 ( new_direction, force ) ;
  sgNormalizeVec3 ( new_direction ) ;
}


void StarWing::update ()
{
  sgSubVec3 ( dirn, coord.xyz, last_coord.xyz ) ;

  if ( dirn [ 0 ] == 0.0 && dirn [ 1 ] == 0.0 && dirn [ 2 ] == 0.0 )
    dirn [ 1 ] = 1.0 ;

  sgNormalizeVec3 ( dirn ) ;

  energy += ENERGY_RECOVERY_RATE ;

  sgVec3 new_direction ;

  strategy ( new_direction ) ;

  sgCopyCoord ( &last_coord, &coord ) ;

  float h = atan2 ( -new_direction[0], new_direction[1] ) * SG_RADIANS_TO_DEGREES ;
  float p = atan2 (  new_direction[2], sqrt(sq(new_direction[0])+
                                            sq(new_direction[1])) ) * SG_RADIANS_TO_DEGREES ;

  sgScaleVec3 ( new_direction, new_direction, MAX_SPEED ) ;
  sgAddVec3   ( coord.xyz, coord.xyz, new_direction ) ;

  while ( coord.xyz [ 0 ] >  WRAP_DISTANCE ) coord.xyz[0] -= WRAP_DISTANCE ;
  while ( coord.xyz [ 0 ] < -WRAP_DISTANCE ) coord.xyz[0] += WRAP_DISTANCE ;
  while ( coord.xyz [ 1 ] >  WRAP_DISTANCE ) coord.xyz[1] -= WRAP_DISTANCE ;
  while ( coord.xyz [ 1 ] < -WRAP_DISTANCE ) coord.xyz[1] += WRAP_DISTANCE ;

  if    ( coord.xyz [ 2 ] >  MAX_ALTITUDE  ) coord.xyz[2] = MAX_ALTITUDE ;
  if    ( coord.xyz [ 2 ] <  MIN_ALTITUDE  ) coord.xyz[2] = MIN_ALTITUDE ;

  while ( h - coord.hpr [ 0 ] >  180.0 ) h -= 360.0 ;
  while ( h - coord.hpr [ 0 ] < -180.0 ) h += 360.0 ;
  while ( p - coord.hpr [ 1 ] >  180.0 ) p -= 360.0 ;
  while ( p - coord.hpr [ 1 ] < -180.0 ) p += 360.0 ;

  float r = ( h - last_coord.hpr[0]) * -15.0 ;

  while ( r - coord.hpr [ 2 ] >  180.0 ) r -= 360.0 ;
  while ( r - coord.hpr [ 2 ] < -180.0 ) r += 360.0 ;

  coord.hpr [ 0 ] += ( h - coord.hpr [ 0 ] ) / 10.0f ;
  coord.hpr [ 1 ] += ( p - coord.hpr [ 1 ] ) / 10.0f ;
  coord.hpr [ 2 ] += ( r - coord.hpr [ 2 ] ) / 10.0f ;

  obj -> setTransform ( & coord ) ;

  if ( energy <= 0.0 )
    state = SG_RESTING ;

  if ( state != SG_RESTING )
    energy -= ENERGY_FLAP_COST ;
}


