#include "tux.h"

#undef  HAT_CAM_TEST

#define LEFT_BUTTON    32
#define RIGHT_BUTTON   16
#define CAMERA_BUTTON   4
#define CAMERA2_BUTTON  8 


int Camera::isFollowing ()
{
   return follow || tuxState->needsCameraFollowMode () ;
}


float blend_angle ( float a2, float a1, float rate )
{
  while ( a1 >= 360.0f ) a1 -= 360.0f ;
  while ( a2 >= 360.0f ) a2 -= 360.0f ;
  while ( a1 <    0.0f ) a1 += 360.0f ;
  while ( a2 <    0.0f ) a2 += 360.0f ;

  if ( fabs ( a1 - a2 ) <= 1.0 )
    return a1 ;

  if ( a1 >= 270.0f && a2 <   90.0f ) a1 -= 360.0f ;
  if ( a1 <   90.0f && a2 >= 270.0f ) a1 += 360.0f ;

  float delta = a1 - a2 ;

  if ( fabs(delta) > 30.0f )
    rate /= 2.0f ;

  return a2 + delta/rate ;
}

static float cam_speed = 1.0f ;
static int tuxified = TRUE ;

void Camera::update ()
{
  if ( faded_to_black () )
    tuxified = ! tuxified ;

  if ( tuxified )
    follow_tux () ;
}


void Camera::follow_tux ( int immediate )
{
  sgCoord tgt_rel_org ;

  sgCopyCoord ( &tgt_rel_org, &(tuxState->coord) ) ;

  if ( immediate )
  {
    heading = - tgt_rel_org . hpr[0] ;
    update_cam_rel_tgt () ;
  }
  else 
  if ( isFollowing() )
  {
    heading = blend_angle ( 180.0f - tgt_rel_org . hpr[0], heading ) ;
    update_cam_rel_tgt () ;
  }

  /*
    cam == The Camera.
    org == The Origin.
    tgt == The Target object.
    lkt == The Lookat point (typically some short distance in
                             front of the target seems best).
  */

  sgVec3 lkt_rel_tgt ;
  sgMat4 mat ;

  if ( tuxState->diving )
    sgSetVec3 ( lkt_rel_tgt, 0.0f, 0.0f, 0.0f ) ;
  else
    sgSetVec3 ( lkt_rel_tgt, 0.0f, range / 4.0f, 2.0f ) ;

  /* Transform the lkt_rel_tgt to tgt_rel_org */

  sgVec3 lkt_rel_org ;
  sgMakeCoordMat4 ( mat, & tgt_rel_org ) ;
  sgXformPnt3 ( lkt_rel_org, lkt_rel_tgt, mat ) ;

  /* Transform the cam_rel_tgt to cam_rel_org */

  sgAddVec3 ( cam_rel_org, cam_rel_tgt, tgt_rel_org.xyz ) ;
  sgSubVec3 ( cam_rel_tgt, cam_rel_org, tgt_rel_org.xyz ) ;

  /*
    We don't want anything to block the view from eyepoint to
    target - so step along the line between target and camera
    looking for any high points along the way and adjusting the
    camera's view upwards if necessary
  */

  sgVec3 posn ;
  sgVec3 step ;
  sgCopyVec3  ( posn, tgt_rel_org.xyz ) ;
  sgScaleVec3 ( step, cam_rel_tgt, 1.0f/10.0f ) ;

  float elev = cam_rel_org[2] ;
#ifdef HAT_CAM_TEST
  float tgt_elev = tgt_rel_org. xyz [ 2 ] ;
#endif

  if ( tuxState -> diving )
  {
    /*
      If tux is underwater - then so must the camera be - otherwise,
      you can see far more than tux could
    */

    if ( elev > -0.5f )
      elev = -0.5f ;
  }
#ifdef HAT_CAM_TEST
  else
  for ( int i = 1 ; i < 9 ; i++ )
  {
    /* Pick a point between target and eye */

    sgAddVec3 ( posn, posn, step ) ;

    /* How high would we need to be to see over this spot? */

    sgVec3 testpos ;
    sgSetVec3 ( testpos, posn[0], posn[1], 1000000.0f ) ;

    sgVec3 normal ;
    float te = tgt_elev +(( getHeightAndNormal ( testpos, normal ) - tgt_elev ) / (float) i ) * 10.0f ;

    if ( normal [ 2 ] > 0.5 && te > elev && te < 10.0f * posn[2] )
      elev = te ;
  }
#endif

  cam_rel_org [ 2 ] = 1000000.0f ;
  float te = getHeight ( cam_rel_org ) + 1.2f ;

  cam_rel_org [ 2 ] = ( te > elev ) ? te : elev ;

  if ( immediate )
  {
    cam_rel_org_now.xyz[0] = cam_rel_org[0] ;
    cam_rel_org_now.xyz[1] = cam_rel_org[1] ;
    cam_rel_org_now.xyz[2] = cam_rel_org[2] ;
  }
  else
  {
    cam_rel_org_now.xyz[0] += (cam_rel_org[0] - cam_rel_org_now.xyz[0]) / 10.0f ;
    cam_rel_org_now.xyz[1] += (cam_rel_org[1] - cam_rel_org_now.xyz[1]) / 10.0f ;
    cam_rel_org_now.xyz[2] += (cam_rel_org[2] - cam_rel_org_now.xyz[2]) / 10.0f ;
  }

  /* Compute the new relative eyepoint */

  sgVec3 lkt_rel_cam ;

  sgSubVec3 ( lkt_rel_cam, lkt_rel_org, cam_rel_org_now.xyz ) ;

  sgVec3 cam_hpr ;

  cam_hpr[0] = -atan2 ( lkt_rel_cam [0], lkt_rel_cam [1] ) * SG_RADIANS_TO_DEGREES ;
  cam_hpr[1] =  atan2 ( lkt_rel_cam [2],
	           sqrt ( lkt_rel_cam [0] * lkt_rel_cam [0] +
                          lkt_rel_cam [1] * lkt_rel_cam [1] ) ) * SG_RADIANS_TO_DEGREES ;
  cam_hpr[2] = 0.0f ;

  if ( isFollowing() || immediate )
  {
    cam_rel_org_now.hpr[0] = cam_hpr[0] ;
    cam_rel_org_now.hpr[1] = cam_hpr[1] ;
    cam_rel_org_now.hpr[2] = cam_hpr[2] ;
  }
  else
  {
    cam_rel_org_now.hpr[0] = blend_angle(cam_rel_org_now.hpr[0], cam_hpr[0]);
    cam_rel_org_now.hpr[1] = blend_angle(cam_rel_org_now.hpr[1], cam_hpr[1]);
    cam_rel_org_now.hpr[2] = blend_angle(cam_rel_org_now.hpr[2], cam_hpr[2]);
  }

  ssgSetFOV    ( wide_fov ? 90.0f : 45.0f, 0.0f ) ;

  ssgSetCamera ( &cam_rel_org_now ) ;
}


int Camera::isSubmerged ()
{
  return cam_rel_org_now . xyz [ 2 ] < 0 ;
}

int Camera::joystickInput ( JoyInfo *j )
{
  if ( (( j->buttons & LEFT_BUTTON ) && ( j->hits    & RIGHT_BUTTON )) ||
       (( j->hits    & LEFT_BUTTON ) && ( j->buttons & RIGHT_BUTTON )) ||
       (( j->hits    & LEFT_BUTTON ) && ( j->hits    & RIGHT_BUTTON )) )
  {
    sound->playSfx ( SOUND_DRIP ) ;
    toggle_follow () ;
  }
  else
  if ( j->buttons == LEFT_BUTTON )
  {
    if ( j->hits == LEFT_BUTTON )
      setFollow ( FALSE ) ;

    pan ( 0.5 ) ;
  }
  else
  if ( j->buttons == RIGHT_BUTTON )
  {
    if ( j->hits == RIGHT_BUTTON )
      setFollow ( FALSE ) ;

    pan ( -0.5 ) ;
  }

  if ( j->buttons == CAMERA_BUTTON )
  {
    pan  ( - j->data[0] ) ;
    zoom (   j->data[1] ) ;
    return TRUE ;
  }

  if ( j->buttons == CAMERA2_BUTTON )
  {
    pan  ( - j->data[0] ) ;
    lift ( - j->data[1] ) ;
    return TRUE ;
  }

  return FALSE ;
}


void Camera::keyboardInput ( int c )
{
  switch ( c )
  {
/*  case 's' : start_fade () ; break ; */
    case '>' : cam_speed += 0.3f ; break ;
    case '<' : cam_speed -= 0.3f ;
               if ( cam_speed < 0.1f )
                 cam_speed = 0.1f ;
               break ;
    case 0x7F /* DEL */         : toggle_far    () ; break ;
    case 256+GLUT_KEY_HOME      : reset         () ; break ;
    case 256+GLUT_KEY_END       : toggle_follow () ; break ;
    case 256+GLUT_KEY_UP        : zoom ( -1.0 )    ; break ;
    case 256+GLUT_KEY_DOWN      : zoom (  1.0 )    ; break ;
    case 256+GLUT_KEY_RIGHT     : pan  ( -1.0 )    ; break ;
    case 256+GLUT_KEY_LEFT      : pan  (  1.0 )    ; break ;
    case 256+GLUT_KEY_PAGE_UP   : lift (  1.0 )    ; break ;
    case 256+GLUT_KEY_PAGE_DOWN : lift ( -1.0 )    ; break ;
    case 256+GLUT_KEY_INSERT    : toggle_widefov (); break ;
    default : /* DO NOTHING */    break ;
  }
}


