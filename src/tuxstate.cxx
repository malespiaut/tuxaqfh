
#include "tux.h"
#include "plib/ssgKeyFlier.h"

#define JUMP_BUTTON     1
#define FIRE_BUTTON     2
#define ROLL_COMBO     (4+8)

ssgKeyFlier   kludgeFlier ;

#define AIR_RESISTANCE 0.05
#define FRICTION       0.40
#define WATER_DRAG     0.40
#define STICKTION      0.10
#define SLIDE_SLOPE    0.30

#define G_FORCE (19.8f/(10.0f*10.0f))

#define COLLISION_SPHERE_RADIUS  0.8f
#define ISECT_STEP_SIZE (COLLISION_SPHERE_RADIUS/2.0f)

static void pr_from_normal ( sgVec3 hpr, sgVec3 nrm )
{
  float sy = sin ( -hpr [ 0 ] * SG_DEGREES_TO_RADIANS ) ;
  float cy = cos ( -hpr [ 0 ] * SG_DEGREES_TO_RADIANS ) ;

  hpr[2] =  SG_RADIANS_TO_DEGREES * atan2 ( nrm[0] * cy - nrm[1] * sy, nrm[2] ) ;
  hpr[1] = -SG_RADIANS_TO_DEGREES * atan2 ( nrm[1] * cy + nrm[0] * sy, nrm[2] ) ;
}

static void hpr_from_normal ( sgVec3 hpr, sgVec3 nrm )
{
  pr_from_normal ( hpr, nrm ) ;
  hpr[0] = -SG_RADIANS_TO_DEGREES * atan2 ( nrm[0], nrm[1] ) ;
}



void TuxState::setDefault ( sgVec3 xyz, sgVec3 hpr )
{
  setup () ;
  sgCopyVec3 ( coord.xyz, xyz ) ;
  sgCopyVec3 ( coord.hpr, hpr ) ;
}

void TuxState::scorch ()
{
  if ( scorch_timer < 0 )
    return ;

  --scorch_timer ;
  temperature += MAX_TEMPERATURE / 100.0f ;

  int which = 0 ;
  float furthest = -1.0f;

  for ( int i = 0 ; i < MAX_SMOKE ; i++ )
  {
    if ( ! smoke[i].active () )
    {
      which = i ;
      break ;
    }

    float d = fabs ( smoke[i].xyz[2] - coord.xyz[2] ) ;

    if ( d > furthest )
    {
      furthest = d ;
      which = i ;
    }
  }

  sgVec3 xyz ;
  sgMat4 mm  ;

  sgSetVec3 ( xyz, 0.0f, 0.0f, -0.5f ) ;
  sgMakeCoordMat4 ( mm, &coord ) ;
  sgXformPnt3 ( xyz, xyz, mm ) ;
  smoke[which].setPos ( xyz ) ;
}


void TuxState::gasp ()
{
  if ( --dive_timer <= 0 )
    kill ( "ran out of Air." ) ;
}

void TuxState::blub ()
{
  temperature = 0 ;  /* Water keeps us cool. */

  if ( --dive_timer <= 0 )
    kill ( "ran out of Air." ) ;
  else
  if (( dive_timer % 15 ) == 0 )
  {
    sound->playSfx ( SOUND_DRIP ) ;

    for ( int i = 0 ; i < MAX_BUBBLE ; i++ )
      if ( ! bubble[i].active () )
      {
        sgVec3 xyz ;
        sgMat4 mm  ;

        sgSetVec3 ( xyz, 0.0f, 0.5f, 1.9f ) ;
        sgMakeCoordMat4 ( mm, &coord ) ;
        sgXformPnt3 ( xyz, xyz, mm ) ;
	bubble[i].setPos ( xyz ) ;
	break ;
      }
  }
}

void TuxState::checkShadow ( float hot )
{
  pr_from_normal ( shadow_coord. hpr, normal ) ;

  shadow_coord . xyz [ 0 ] = coord.xyz [ 0 ] ;
  shadow_coord . xyz [ 1 ] = coord.xyz [ 1 ] ;
  shadow_coord . xyz [ 2 ] = over_water ? -1000000.0f : hot ;

  shadow_dcs -> setTransform ( & shadow_coord ) ;
}


void TuxState::checkTransporters ()
{
  magicRing -> update () ;

  if ( forced_spin_delay > 0 )
  {
    forced_spin_delay-- ;

    if ( forced_spin_speed > 0 )
    {
      forced_spin_speed -= 2 ;

      if ( forced_spin_speed <= 0 )
	sound->playSfx ( SOUND_POP ) ;
    }
    else
      forced_spin_speed = 0 ;

    return ;
  }

  for ( int i = 0 ; i < num_rings ; i++ )
  {
    RingInstance *r = & (ring [ i ]) ;

    if ( sgDistanceVec3 ( coord.xyz, r->xyz ) > 2.0f )
      continue ;

    if ( forced_spin_speed < 80 )
      forced_spin_speed++ ;
    else
    {
      int j = i + 1 ;

      if ( j >= num_rings )
	j = 0 ;

      r = & (ring [ j ]) ;

      sgCopyVec3 ( coord.xyz, r->xyz ) ;
      coord.xyz [ 2 ] += 5.0f ;
      forced_spin_delay = 100 ;
      sound->playSfx ( SOUND_WEE ) ;
    }

    return ;
  }
}


void TuxState::checkHerrings ()
{
  int i ;

  tuxlife_h -> update () ;
  silver_h  -> update () ;
  gold_h    -> update () ;
  red_h     -> update () ;
  green_h   -> update () ;

  for ( i = 0 ; i < num_tuxlife ; i++ )
  {
    TuxLifeInstance *l = & (tuxlife [ i ]) ;

    if ( l->eaten || sgDistanceVec3 ( coord.xyz, l -> xyz ) > 2.0f )
      continue ;

    sound->playSfx ( SOUND_CLAP ) ;
    grabbedLife () ;

    l -> scs -> getParent ( 0 ) -> removeKid ( l->scs ) ;
    l -> scs   = NULL ;
    l -> eaten = TRUE ;
  }

  for ( i = 0 ; i < num_herring ; i++ )
  {
    HerringInstance *h = & (herring [ i ]) ;

    if ( h->eaten || sgDistanceVec3 ( coord.xyz, h -> xyz ) > 2.0f )
      continue ;

    switch ( h->type )
    {
      case HE_GOLD   : 
        herring_collected [ h->ident ] = TRUE ;
	energy = 1.0f ;
	sound->playSfx ( SOUND_CLAP ) ;
	currState->score += 200 ;
        much_better () ;
	break ;

      case HE_SILVER :
        energy += 0.2 ;
	if ( energy > 1.0f ) energy = 1.0f ;
	sound->playSfx ( SOUND_BURP ) ;
	currState->score += 10 ;
        better () ;
	break ;

      case HE_RED    :
	currState->score += 50 ;

	switch ( h->effect )
	{
	  case EFFECT_ROCKET   : setRocket   ( TRUE ) ; break ;
	  case EFFECT_GUN      : setGun      ( TRUE ) ; break ;
	  case EFFECT_SUPERTUX : setSuperTux ( TRUE ) ; break ;
	  case EFFECT_PARTYHAT : setPartyHat ( TRUE ) ; break ;
	  case EFFECT_BOWTIE   : setBowTie   ( TRUE ) ; break ;
	  case EFFECT_SHADES   : setShades   ( TRUE ) ; break ;
	  case EFFECT_DEFAULT  : sound->playSfx ( SOUND_SINKDR ); break ;
	}
	break ;

      case HE_GREEN  :
	sound->playSfx ( SOUND_BARF ) ;
	ouch () ;
	break ;
    }

    h -> scs -> getParent ( 0 ) -> removeKid ( h->scs ) ;
    h -> scs   = NULL ;
    h -> eaten = TRUE ;
  }
}





TuxState::TuxState ( int /*be_gown*/ )
{
  space_helmet    = FALSE ;
  temperature     =   0   ;
  snowballs       =   0   ;
  rc_mode         = FALSE ;
  energy          = 1.0f  ;
  life            =   8   ;
  lives           = INIT_LIVES ;
  joy_hit_timer   =   0   ;
  scorch_timer    =  -1   ;
  roll_timer      =  -1   ;
  dizzy_timer     =  -1   ;
  squish_amount   = 1.0f  ;
  squish_pain     = 3 * OUCH_STEP ;
  squished        = FALSE ;
  just_died       = FALSE ;
  setup () ;
}



void TuxState::setup ()
{
  help_message    = NULL  ;
  help_message2   = NULL  ;

  on_ground       = TRUE  ;
  on_steep_slope  = FALSE ;
  over_water      = FALSE ;
  flapping        = FALSE ;
  super_tux       = FALSE ;
  have_rocket     = FALSE ;
  have_gun        = FALSE ;
  have_hat        = FALSE ;
  have_tie        = FALSE ;
  have_shades     = FALSE ;

  temperature     =  0    ;
  slide_timer     =  0    ;
  roll_timer      = -1    ;
  dizzy_timer     = -1    ;
  sliding         = FALSE ;
  forced_spin_delay = 0   ;
  forced_spin_speed = 0   ;

  bob_offset      = 0.0f  ;
  bob_delta       = 0.01f ;
  sgSetVec3 ( rel_acceleration, 0.0f, 0.0f, 0.0f ) ;
  sgSetVec3 ( abs_acceleration, 0.0f, 0.0f, 0.0f ) ;
  sgSetVec3 ( abs_velocity    , 0.0f, 0.0f, 0.0f ) ;
  right_left = 0.0f ;
  fore_aft   = 0.0f ;

  sgZeroCoord ( & coord ) ;
  sgZeroCoord ( & shadow_coord ) ;
}

void TuxState::init ( int be_gown )
{
  tux        = new Penguin ( be_gown ) ;
  shadow     = new Shadow  ( -1.5f, 1.5f, -1.5f, 1.5f ) ;
  shadow_dcs = new ssgTransform () ;

  help_message    = NULL  ;
  help_message2   = NULL  ;

  tux_dcs    = tux -> getRoot () ;
  shadow_dcs -> addKid ( shadow -> getRoot () ) ;
  tux_dcs    -> ref () ;
  shadow_dcs -> ref () ;
}



float TuxState::updateBob ()
{
  bob_offset += bob_delta ;

  if ( bob_offset > 0.4 || bob_offset < -0.4 )
    bob_delta = -bob_delta ;

  return bob_offset ;
}


void TuxState::print ()
{
  stPrintf ( "TuxState:" ) ;
  stPrintf ( "  XYZ=(%f,%f,%f)", coord.xyz[0], coord.xyz[1], coord.xyz[2] ) ;
  stPrintf ( "  HPR=(%f,%f,%f)", coord.hpr[0], coord.hpr[1], coord.hpr[2] ) ;
}


float TuxState::collectIsectData ( sgVec3 start, sgVec3 end,
                                   sgVec3 platform_velocity,
                                   float *friction,
                                   int firsttime )
{
  sgVec3 vel ;

  sgSubVec3 ( vel, end, start ) ;

  float speed = sgLengthVec3 ( vel ) ;

  /* At slow speeds, test the line from 'start' to 'end' in one chunk */

  if ( speed <= ISECT_STEP_SIZE || firsttime )
    return getIsectData ( start, end, platform_velocity, friction, firsttime ) ;

  /*
    At higher speeds, we must test frequently so we can't fall
    through something thin.
  */

  int nsteps = (int) ceil ( speed / ISECT_STEP_SIZE ) ;

  if ( nsteps == 0 ) nsteps = 1 ;

  sgScaleVec3 ( vel, vel, 1.0f / (float) nsteps ) ;

  sgVec3 pos1, pos2 ;

  sgCopyVec3 ( pos1, start ) ;

  float hat = 0.0f ;

  for ( int i = 0 ; i < nsteps ; i++ )
  {
    sgAddVec3 ( pos2, pos1, vel ) ;    
    hat = getIsectData ( pos1, pos2, platform_velocity, friction, FALSE ) ;
    sgCopyVec3 ( pos1, pos2 ) ;    
  }

  sgCopyVec3 ( end, pos2 ) ;    
  return hat ;
}

float TuxState::getIsectData ( sgVec3 start, sgVec3 end,
                               sgVec3 platform_velocity,
                               float *friction, 
                               int firsttime )
{
  ssgHit *results ;
  int num_hits ;

  sgZeroVec3 ( platform_velocity ) ;

  sgSphere sphere ;
  sgMat4   invmat ;

  /*
    It's necessary to lift the center of the bounding sphere
    somewhat so that Tux can stand on a slope.
  */

  sphere.setRadius ( COLLISION_SPHERE_RADIUS ) ;
  sphere.setCenter ( 0.0f, 0.0f, COLLISION_SPHERE_RADIUS + 0.3 ) ;

  /* Do a bounding-sphere test for Tux. */

  sgMakeIdentMat4 ( invmat ) ;
  invmat[3][0] = -end[0] ;
  invmat[3][1] = -end[1] ;
  invmat[3][2] = -end[2] ;

  if ( firsttime )
    num_hits = 0 ;
  else
    num_hits = ssgIsect ( scene, &sphere, invmat, &results ) ;
 
  on_sproingpad = FALSE ;
  in_danger     = FALSE ;  
  too_hot       = FALSE ;  

  sgSetVec3 ( surface_avoidance_vector, 0.0f, 0.0f, 0.0f ) ;

  int i ;

  /* Look at all polygons near to Tux */

  for ( i = 0 ; i < num_hits ; i++ )
  {
    ssgHit *h = &results [ i ] ;

    for ( int ent = h -> getNumPathEntries () - 1 ; ent >= 0 ; ent-- )
      hit_hook ( h -> getPathEntry ( ent ), NULL, NULL ) ;

    if ( getMaterial ( h->leaf ) -> isOxygen () )
      breathe () ;

    if ( getMaterial ( h->leaf ) -> isCold () &&
         temperature > 0.0f )
      temperature -= 5.0 * HEATING_RATE ;

    if ( getMaterial ( h->leaf ) -> isIgnore () )
    {
      if ( getMaterial ( h->leaf ) -> isStartClock () )
        start_stopwatch () ;

      if ( getMaterial ( h->leaf ) -> isStopClock  () )
        stop_stopwatch () ;

      continue ;
    }

    float dist = sgDistToPlaneVec3 ( h->plane, sphere.getCenter() ) ;

    /*
      This is a nasty kludge to stop a weird interaction
      between collision detection and height-of-terrain
      that causes tux to get 'stuck' on some polygons
      corners. This should be fixed more carefully.

      Surfaces that are this close to horizontal
      are handled by the height-of-terrain code anyway.
    */

    if ( h -> plane[2] > 0.5 )
      continue ;

    if ( dist > 0 && dist < sphere.getRadius() )
    {
      dist = sphere.getRadius() - dist ;
      sgVec3 nrm ;
      sgCopyVec3  ( nrm, h->plane ) ;
      sgScaleVec3 ( nrm, nrm, dist ) ;

      /*
        If we were caught beneath something - squish
        up instead of moving away.
      */

      if ( on_ground && h->plane[2]<-0.5 )
      {
        squish_amount = 1.0 + nrm [ 2 ] ;
        nrm [ 2 ] = 0.0f ;

        if ( squish_amount < 0.5 && ! squished )
        {
          if ( squish_pain > 0 )
            sound->playSfx ( SOUND_OW ) ;

          ouch(squish_pain) ;
          squished = TRUE ;
        }
      }

      sgAddVec3 ( surface_avoidance_vector, nrm ) ;

      sgVec3 tmp ;
      sgCopyVec3 ( tmp, sphere.getCenter() ) ;
      sgAddVec3 ( tmp, nrm ) ;
      sphere.setCenter ( tmp ) ;

      if ( getMaterial ( h->leaf )->isLethal () && ! currState->cheat_mode )
	kill ( "touched something Bad." ) ;
      else
      {
	if ( getMaterial ( h->leaf )->isVeryHot () && ! currState->cheat_mode )
	  too_hot = TRUE ;

	if ( getMaterial ( h->leaf )->isDangerous () && ! currState->cheat_mode )
	  in_danger = TRUE ;

	if ( getMaterial ( h->leaf ) -> isExit () )
	  currState -> new_level = getExitLevel ( coord.xyz ) ;
      }
    }
  }

  /* Look for the nearest polygon *beneath* Tux (assuming avoidance) */

  sgAddVec3 ( end, surface_avoidance_vector ) ;

  float hot ;        /* H.O.T == Height Of Terrain */
  sgVec3 HOTvec ;

  invmat[3][0] = - end [0] ;
  invmat[3][1] = - end [1] ;
  invmat[3][2] = 0.0 ;

  float top = COLLISION_SPHERE_RADIUS +
                            (( start[2] > end[2] ) ? start[2] : end[2] ) ;

  sgSetVec3 ( HOTvec, 0.0f, 0.0f, top ) ;

  num_hits = ssgHOT ( scene, HOTvec, invmat, &results ) ;
  
  *friction = 1.0f ;

  hot = -1000000.0f ;

  for ( i = 0 ; i < num_hits ; i++ )
  {
    ssgHit *h = &results [ i ] ;

    if ( getMaterial ( h->leaf ) -> isIgnore () )
    {
      if ( getMaterial ( h->leaf ) -> isStartClock () )
        start_stopwatch () ;        

      if ( getMaterial ( h->leaf ) -> isStopClock  () )
        stop_stopwatch () ;        

      continue ;
    }

    float hgt = - h->plane[3] / h->plane[2] ;

    if ( hgt >= hot )
    {
      sgMat4 mat_now ;
      sgMat4 mat_last ;

      sgMakeIdentMat4 ( mat_now  ) ;
      sgMakeIdentMat4 ( mat_last ) ;

      for ( int ent = h -> getNumPathEntries () - 1 ; ent >= 0 ; ent-- )
      {
        ssgEntity *e = h -> getPathEntry ( ent ) ;
        hit_hook ( e, NULL, NULL ) ;

        if ( e -> isAKindOf ( ssgTypeTransform() ) )
        {
          ssgTransform *bt = (ssgTransform *) e ;

	  sgMat4 now ;
	  sgMat4 last ;

          bt -> getTransform     ( now  ) ;
          bt -> getLastTransform ( last ) ;

          sgPostMultMat4 ( mat_now , now  ) ;
          sgPostMultMat4 ( mat_last, last ) ;
        }
      }

#ifdef DEBUGGING
fprintf ( stderr, "MN=%f,%f,%f\n",
                    mat_now [ 3 ][ 0 ],
                    mat_now [ 3 ][ 1 ],
                    mat_now [ 3 ][ 2 ] ) ;

fprintf ( stderr, "ML=%f,%f,%f\n",
                    mat_last[ 3 ][ 0 ],
                    mat_last[ 3 ][ 1 ],
                    mat_last[ 3 ][ 2 ] ) ;
#endif

      sgCopyVec3 ( platform_velocity, mat_now [3] ) ;
      sgSubVec3  ( platform_velocity, mat_last[3] ) ;
 
      *friction = getMaterial ( h->leaf ) -> friction ;

      hot = hgt ;
      sgCopyVec3 ( normal, h->plane ) ;

      if ( hot >= end [ 2 ] - 0.01 )
      {
        if ( getMaterial ( h->leaf ) -> isOxygen () )
          breathe () ;

        if ( getMaterial ( h->leaf ) -> isCold () && temperature > 0.0f )
          temperature -= 5.0 * HEATING_RATE ;

	if ( getMaterial ( h->leaf ) -> isLethal () && ! currState->cheat_mode )
	  kill ( "touched something Bad." ) ;
        else
        {
	  if ( getMaterial ( h->leaf ) -> isVeryHot () && ! currState->cheat_mode )
	    too_hot = TRUE ;

	  if ( getMaterial ( h->leaf ) -> isDangerous () && ! currState->cheat_mode )
	    in_danger = TRUE ;

	  if ( getMaterial ( h->leaf ) -> isSproingPad () )
	    on_sproingpad = TRUE ;

	  if ( getMaterial ( h->leaf ) -> isExit () )
	    currState -> new_level = getExitLevel ( coord.xyz ) ;
        }
      }
    }
  }

  if ( too_hot )
  {
    in_danger = TRUE ;
    scorch_timer = 50 ;
  }

  swimming   = FALSE ;
  diving     = FALSE ;
  on_ground  = ( hot >= end [ 2 ] - 0.01 ) ;
  over_water = ( ocean != NULL &&
                 hot < 0.0f &&
                 ocean -> getType() == OCEAN_WATER ) ;

  in_blackhole = ( ocean != NULL &&
                 end[2] < -20.0f &&
                 ocean -> getType() == OCEAN_BLACKHOLE ) ;

  if ( over_water )
  {
    on_ground = FALSE ;

    diving = (( end [ 2 ] <  0.0f ) && ( ji.buttons & FIRE_BUTTON ) != 0 ) ||
              ( end [ 2 ] < -2.0f ) ;

    if ( ! diving )
    {
      swimming = ( end [ 2 ] < 0.0f ) ;

      if ( end [ 2 ] < -1.9 )
        end [ 2 ] = -1.9 ;
    }
 
    normal [ 0 ] = 0.0f ;
    normal [ 1 ] = 0.0f ;
    normal [ 2 ] = 1.0f ;
  }

  if ( end [ 2 ] < hot )
    end [ 2 ] = hot ;

  if ( hot+0.5 >= end[2] && roll_timer < 0 )
  {
    on_steep_slope = ( hot+0.5 >= end[2] &&
                     (1.0 - normal [ 2 ]) /
                           (*friction * *friction) > SLIDE_SLOPE ) ;

    if ( ! on_steep_slope )
    {
      if ( slide_timer > 0 && (ji.hits & JUMP_BUTTON) == 0 )
      {
	slide_timer-- ;
	sliding = TRUE ;
      }
      else
      {
	slide_timer = 0 ;
	sliding = FALSE ;
      }
    }
    else
    if ( on_steep_slope && *friction < 1.0 )
    {
      if ( slide_timer < 15 )
      {
	slide_timer++ ;
	sliding = FALSE ;
      } 
      else
      {
	if ( slide_timer == 15 )
	  sound -> playSfx ( SOUND_WEE ) ;

	sliding = TRUE ;
	slide_timer = 90 ;
      }
    }
  }

  return hot ;
}


void TuxState::joystickInput ( JoyInfo *j )
{
  ji = *j ;

  /*
    In RC mode, the motion is computed directly
    from the joystick inputs. In 'follow' mode,
    screen-space motion goes crazy - and (effective)
    RC mode does "what you'd expect"
  */

  if ( camera->isFollowing() || rc_mode )
  {
    right_left = -ji.data [ 0 ] ;
    fore_aft   = -ji.data [ 1 ] ;
  }
  else
  {
    /*
      In videogame mode, the motion is interpreted in
      screen space.
    */

    sgVec3 joyres ;
    sgVec3 joyvec ;
    sgMat4 joymat ;
    sgVec3 up = { 0, 0, 1 } ;

    sgMakeRotMat4 ( joymat, camera->getHeading(), up ) ;
    sgSetVec3     ( joyvec, ji.data [ 0 ], ji.data [ 1 ], 0.0f ) ;
    sgXformVec3   ( joyres, joyvec, joymat ) ;
    fore_aft = sqrt ( joyres[0]*joyres[0] + joyres[1]*joyres[1] ) ;

    if ( fore_aft < 0.01 )
    {
      right_left = fore_aft = 0.0f ;
    }
    else
    {
      if ( fore_aft < 0.1 )
	fore_aft = 0.0f ;

      float new_heading = atan2 ( joyres[0], joyres[1] ) *
				       SG_RADIANS_TO_DEGREES  ;

      while ( new_heading - coord.hpr[0] >  180.0 ) new_heading -= 360.0 ;
      while ( new_heading - coord.hpr[0] < -180.0 ) new_heading += 360.0 ;

      new_heading -= coord.hpr[0] ;

      right_left = new_heading / 20.0f ;
    }
  }

  if ( ! on_ground  && ! swimming && ! diving &&
       energy > 0.4f && ( ji.hits & JUMP_BUTTON ) )
    flapping = TRUE ;

  if ( energy <= 0.1f || (ji.buttons & JUMP_BUTTON) == 0 ) 
    flapping = FALSE ;
}


void TuxState::doDiving ( float hot )
{
  roll_timer = -1 ;
  dizzy_timer = -1 ;
  sliding = FALSE ;

  coord . hpr [ 0 ] += right_left * 2.0f ;
  coord . hpr [ 1 ] -= fore_aft   * 2.0f ;
  coord . hpr [ 2 ]  = right_left * -30.0f ;

  if ( coord.xyz [ 2 ] < hot + 1.5f )
    coord.xyz [ 2 ] = hot + 1.5f ;

  if ( ji.buttons & JUMP_BUTTON )  /* FLAP!! */
  {
    flapping = TRUE ;
    fore_aft = 2.0f ;
    blub () ;  /* Use air faster if flapping */
  }

  blub () ;

  if ( ji.buttons & FIRE_BUTTON )  /* SINK!! */
    abs_velocity [ 2 ] = -0.08 ;
  else
    abs_velocity [ 2 ] = 0.08 ;

  rel_acceleration [ 1 ] = fore_aft * 0.066f ;
  rel_acceleration [ 2 ] = fore_aft * 0.066f ;
}


void TuxState::doSwimming ( float hot )
{
  if ( temperature > 0.0f )
    temperature -= 5.0 * HEATING_RATE ;

  if ( ! currState -> cant_breathe )
    breathe () ;

  roll_timer = -1 ;
  dizzy_timer = -1 ;
  sliding = FALSE ;

  coord . xyz [ 2 ]  = ( hot < -0.9f ) ? -0.9f : hot ;

  coord . hpr [ 0 ] += right_left *   1.0f ;
  coord . hpr [ 1 ]  = fore_aft   * -25.0f ;
  coord . hpr [ 2 ]  = right_left *   4.0f ;

  abs_velocity     [ 2 ] = 0.0 ;
  rel_acceleration [ 1 ] = fore_aft * 0.2f ;

  if ( ji.hits & JUMP_BUTTON )    /* JUMP! */
  {
    rel_acceleration [ 1 ]  = G_FORCE * 6.0 ;
    rel_acceleration [ 2 ]  = G_FORCE * 6.0 ;
    abs_acceleration [ 2 ]  = 0.5 ;
  }
}

void TuxState::doDizzy ( float )
{
  if ( ! currState -> cant_breathe )
    breathe () ;

  roll_timer = -1 ;
  dizzy_timer-- ;

  abs_velocity [ 0 ] =
    abs_velocity [ 1 ] =
      abs_velocity [ 2 ] = 
        rel_acceleration [ 0 ] =
          rel_acceleration [ 1 ] =
            rel_acceleration [ 2 ] = 0.0 ;  /* Kill all motion */

  pr_from_normal ( coord. hpr, normal ) ;
}


void TuxState::doSpinning ( float )
{
  if ( ! currState -> cant_breathe )
    breathe () ;

  roll_timer = -1 ;
  dizzy_timer = -1 ;

  abs_velocity [ 0 ] =
    abs_velocity [ 1 ] =
      abs_velocity [ 2 ] = 
        rel_acceleration [ 0 ] =
          rel_acceleration [ 1 ] =
            rel_acceleration [ 2 ] = 0.0 ;  /* Kill all motion */

  coord . hpr [ 0 ] += (float) forced_spin_speed ;

  pr_from_normal ( coord. hpr, normal ) ;
}


void TuxState::doSliding ( float, float friction )
{
  roll_timer = -1 ;
  dizzy_timer = -1 ;

  if ( ! currState -> cant_breathe )
    breathe () ;

  abs_velocity [ 2 ] = 0.0 ;  /* Kill any existing vertical component */

  float old_heading = coord.hpr[0] ;

  hpr_from_normal ( coord. hpr, normal ) ;

  coord.hpr [ 0 ] = blend_angle ( old_heading, coord.hpr[0], 7 ) +
                    right_left * 2.0f ;

  rel_acceleration [ 1 ] = 3.0f * G_FORCE * (1.0 - normal[2]) / friction +
                                               fore_aft * 0.05f ;

  return ;
}


void TuxState::rechargeSnowballs ()
{
  if ( snowballs < MAX_SNOWBALL )
    sound -> playSfx ( SOUND_BOING ) ;

  snowballs = MAX_SNOWBALL ;
}

void TuxState::fireSnowball ()
{
  if ( snowballs <= 0 || squish_amount < 0.8f )
    return ;

  snowballs-- ;

  for ( int i = 0 ; i < MAX_SNOWBALL ; i++ )
    if ( ! snowball[i].active () )
    {
      sgVec3 xyz ;
      sgVec3 dir ;
      sgMat4 mm  ;

      sgSetVec3 ( xyz, 0.5f, 0.0f, 0.9f ) ;

      if ( joy_hit_timer > 30 )
	joy_hit_timer = 30 ;

      sgSetVec3 ( dir, 0.0f, 1.2 * cos ( (20.0f + 2.0f * joy_hit_timer) * SG_DEGREES_TO_RADIANS ),
			     1.2 * sin ( (20.0f + 2.0f * joy_hit_timer) * SG_DEGREES_TO_RADIANS ) ) ;

      sgMakeCoordMat4 ( mm, &coord ) ;
      sgXformPnt3 ( xyz, xyz, mm ) ;
      sgXformVec3 ( dir, dir, mm ) ;
      sgAddVec3   ( dir, abs_velocity ) ;

      snowball[i].setPos   ( xyz ) ;
      snowball[i].setSpeed ( dir ) ;
      break ;
    }
}


void TuxState::doRolling ( float, float friction )
{
  if ( ! currState -> cant_breathe )
    breathe () ;

  if ( roll_timer == 0 )
  {
    sound->playSfx ( SOUND_GLASBK ) ;
    roll_timer = -1 ;
    dizzy_timer = 30 ;
  }
  else
  {
    roll_timer-- ;
    dizzy_timer = -1 ;
  }

  abs_velocity [ 2 ] = 0.0 ;  /* Kill any existing vertical component */

  sgVec3 smoothed_normal ;
  sgSetVec3 ( smoothed_normal, normal [ 0 ] / 2.0f,
			       normal [ 1 ] / 2.0f,
			       normal [ 2 ] ) ;
  sgNormalizeVec3 ( smoothed_normal ) ;
  pr_from_normal ( coord . hpr, smoothed_normal ) ;

  rel_acceleration [ 1 ] = 1.0f * friction * friction ;
}


void TuxState::doWalking ( float )
{
  if ( ! currState -> cant_breathe )
    breathe () ;

  dizzy_timer = -1 ;
  abs_velocity [ 2 ] = 0.0 ;  /* Kill any existing vertical component */

  sgVec3 smoothed_normal ;
  sgSetVec3 ( smoothed_normal, normal [ 0 ] / 2.0f,
			       normal [ 1 ] / 2.0f,
			       normal [ 2 ] ) ;
  sgNormalizeVec3 ( smoothed_normal ) ;
  pr_from_normal ( coord . hpr, smoothed_normal ) ;

  rel_acceleration [ 1 ] = fore_aft * 0.2f ;

  if ( ( ji.buttons & ROLL_COMBO ) == ROLL_COMBO )
  {
    sound->playSfx ( SOUND_TARZAN ) ;
    roll_timer = 15 ;
    return ;
  }

  if ( ji.releases & FIRE_BUTTON )
    fireSnowball () ;

  if ( ji.buttons & FIRE_BUTTON )
    joy_hit_timer++ ;
  else
    joy_hit_timer = 0 ;

  if ( ji.hits & JUMP_BUTTON )    /* JUMP! */
  {
    if ( on_sproingpad )
    {
      sound->playSfx ( SOUND_TARZAN ) ;
      rel_acceleration [ 1 ] = G_FORCE * 9.0 ;
      rel_acceleration [ 2 ] = G_FORCE * 20.0 ;
      abs_acceleration [ 2 ] = 0.5 ;
    }
    else
    {
      rel_acceleration [ 1 ] = G_FORCE * (fore_aft * 6.0) ;
      rel_acceleration [ 2 ] = G_FORCE * 6.0 ;
      abs_acceleration [ 2 ] = 0.5 ;
    }
  }

  coord.hpr [ 0 ] += right_left * 4.0f ;
}


void TuxState::doFreeFall ( float hot )
{
  if ( ! currState -> cant_breathe )
    breathe () ;

  roll_timer = -1 ;

  /*
    Change pitch if joystick is pushed fore/aft
  */

  if ( fore_aft > 0.0f && coord . hpr [ 1 ] > -60.0 * fore_aft ) 
    coord . hpr [ 1 ]-- ;
  else
  if ( fore_aft < 0.0f && coord . hpr [ 1 ] < -60.0 * fore_aft ) 
    coord . hpr [ 1 ]++ ;

  if ( flapping && energy > 0.0f && (ji.buttons & JUMP_BUTTON) )  /* FLAP!! */
  {
    float alt         = coord.xyz[2] - hot ;
    float lift        = 0.9 ;
    float rocket_lift = 1.0 + (10.0f-alt) / 20.0f ;

    abs_acceleration [ 2 ] = G_FORCE * (getRocket() ? rocket_lift : lift) ;
    energy -= (getRocket()? 0.02 : 0.08) ;

    if ( energy < 0.0f ) energy = 0.0f ;
  }

  rel_acceleration [ 1 ] = G_FORCE * (fore_aft / 40.0) ;
}


void TuxState::update ()
{
  if ( currState -> pause )
  {
    tux -> update ( & coord, 0.0f +
                    ( dizzy_timer < 0 ) ? 0.0f : ( dizzy_timer * 17.6f ),
                    ( roll_timer < 0 ) ? 0.0f : ( roll_timer * 360.0f / 15.0f ),
                    squish_amount,
                    flapping, !on_ground, sliding, diving, getRocket(),
                    have_gun, flapping,
                    space_helmet,  /* Space helmet */
                    super_tux,
                    have_hat, have_tie, have_shades, dizzy_timer >= 0 ) ;
    return ;
  }

  if ( currState -> kludge_mode )
  {
    kludgeFlier . set_coord ( & coord ) ;

    kludgeFlier . incoming_keystroke ( currState -> kludge_char ) ;
    currState -> kludge_char = 0 ;

    kludgeFlier . update () ;

    coord = *( kludgeFlier . get_coord () ) ;

    tux -> update ( & coord, 0.0f, 0.0f, 1.0f,
                  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
                  FALSE, FALSE, FALSE, FALSE, FALSE, TRUE ) ;
    return ;
  }

  static sgVec3 platform_velocity ;
  static float friction = 1.0f ;
  static float hot = 0.0f ;
  static int firsttime = TRUE ;

  if ( firsttime )
  {
    firsttime = FALSE ;

    sgSetVec3 ( platform_velocity, 0.0f, 0.0f, 0.0f ) ;
    hot = collectIsectData ( coord.xyz, coord.xyz,
                             platform_velocity, &friction, TRUE ) ;

    if ( coord.xyz [ 2 ] < hot )
      coord.xyz [ 2 ] = hot ;
  }

  in_blackhole = FALSE ;

  if ( squish_amount < 0.3 )
    squish_amount += 0.001 ;
  else
    squish_amount += 0.01 ;

  if ( squish_amount >= 1.0f )
  {
    squished = FALSE ;
    squish_amount = 1.0f ;
  }

  sgSetVec3 ( rel_acceleration, 0.0f, 0.0f, 0.0f ) ;
  sgSetVec3 ( abs_acceleration, 0.0f, 0.0f, 0.0f ) ;

  if ( currState -> cant_breathe  && ! currState->cheat_mode )
    gasp () ;

  if ( forced_spin_speed ) doSpinning ( hot ) ; else
  if ( swimming          ) doSwimming ( hot ) ; else
  if ( diving            ) doDiving   ( hot ) ; else
  if ( on_ground         )
  {
    if ( dizzy_timer >= 0 ) doDizzy   ( hot ) ; else
    if ( roll_timer >= 0  ) doRolling ( hot, friction ) ; else
    if ( sliding          ) doSliding ( hot, friction ) ; else
                            doWalking ( hot ) ;
  }
  else
    doFreeFall ( hot ) ;

  /* Contribution due to gravity... */

  if ( ! diving )
    abs_acceleration [2] -= G_FORCE ;

  if ( in_danger )
  {
    sound->playSfx ( SOUND_OW ) ;
    ouch () ;
    ouch () ;
    abs_acceleration [2] += 20.0 * G_FORCE ;
    rel_acceleration [1] += 10.0 * G_FORCE ;
    in_danger = FALSE ;
  }

  scorch () ;

  /*
    Rotate relative accelleration into world coords, add
    absolute accelleration and accumulate onto abs_velocity.
  */

  sgMakeCoordMat4 ( mat, &coord ) ;
  sgXformVec3     ( rel_acceleration, rel_acceleration, mat ) ;

  sgAddVec3       ( abs_velocity, rel_acceleration ) ;
  sgAddVec3       ( abs_velocity, abs_acceleration ) ;

  /* Ground reaction */

  if ( on_ground )
  {
    sgVec3 ground_reaction ;
    sgScaleVec3 ( ground_reaction, normal, G_FORCE ) ;
    ground_reaction [ 2 ] = 0.0f ;
    sgAddVec3   ( abs_velocity, ground_reaction ) ; 

    float speed = sgLengthVec2 ( abs_velocity ) ;

    if ( speed < STICKTION * friction )
      abs_velocity[0] = abs_velocity[1] = 0.0f ;
  }

  sgScaleVec2 ( abs_velocity, abs_velocity, 1.0f - ( on_ground ? FRICTION * friction :
					     swimming  ? WATER_DRAG :
					     diving    ? WATER_DRAG :
					                 AIR_RESISTANCE ) ) ;

  static float last_direction = 0.0f ;
  float head_direction ;

  if ( forced_spin_speed != 0 )
    head_direction = 0.5 * ( coord.hpr[0] - last_direction ) ;
  else
  {
    head_direction = 4.0 * ( coord.hpr[0] - last_direction ) ;

    sgVec3 next_coord ;
    sgAddVec3 ( next_coord, coord.xyz,abs_velocity ) ;

    hot = collectIsectData ( coord.xyz, next_coord, platform_velocity, &friction, FALSE ) ;

#ifdef DEBUGGING
fprintf ( stderr, "PV=%f,%f,%f\n",
                    platform_velocity [ 0 ],
                    platform_velocity [ 1 ],
                    platform_velocity [ 2 ] ) ;
#endif

    sgSubVec3 ( abs_velocity, next_coord, coord.xyz ) ;
    sgCopyVec3 ( coord.xyz, next_coord ) ;
    sgAddVec3 ( coord.xyz, platform_velocity ) ;
  }

  last_direction = coord.hpr[0] ;

  if ( in_blackhole )
    kill ( "Fell off the edge." ) ;

  if ( currState -> heating > 0.0f && ! currState->cheat_mode )
    temperature += currState -> heating ;
  else
  if ( temperature > 0.0f )
    temperature -= currState -> heating ;

  if ( temperature > MAX_TEMPERATURE )
    kill ( "became too Hot." ) ;

  tux -> update ( & coord, head_direction +
                  ( dizzy_timer < 0 ) ? 0.0f : ( dizzy_timer * 17.6f ),
                  ( roll_timer < 0 ) ? 0.0f : ( roll_timer * 360.0f / 15.0f ),
                  squish_amount,
                  flapping, !on_ground, sliding, diving, getRocket(),
                  have_gun, flapping,
                  space_helmet,  /* Space helmet */
                  super_tux,
                  have_hat, have_tie, have_shades, dizzy_timer >= 0 ) ;

  if ( energy < 0.8f ) energy += 0.001 ;

  checkShadow  ( hot ) ;
  checkTransporters () ;
  checkHerrings     () ;
}


