

class Camera
{
  float range     ;
  float heading   ;
  float elevation ;

  int follow   ;
  int flag_far ;
  int wide_fov ;

  sgVec3  cam_rel_tgt ;
  sgVec3  cam_rel_org ;
  sgCoord cam_rel_org_now ;

public:

  Camera ()
  {
    heading = 0.0f ;
    reset () ;
  }

  void update_cam_rel_tgt ()
  {
    if ( range     <    4.0f ) range     =    4.0f ;
    if ( range     >  300.0f ) range     =  300.0f ;
    if ( elevation < -300.0f ) elevation = -300.0f ;
    if ( elevation >  300.0f ) elevation =  300.0f ;
    if ( heading   <    0.0f ) heading  +=  360.0f ;
    if ( heading   >  360.0f ) heading  -=  360.0f ;

    cam_rel_tgt [ 0 ] = range * sin ( heading * SG_DEGREES_TO_RADIANS ) * ( flag_far ? 5.0f : 1.0f ) ;
    cam_rel_tgt [ 1 ] = range * cos ( heading * SG_DEGREES_TO_RADIANS ) * ( flag_far ? 5.0f : 1.0f ) ;
    cam_rel_tgt [ 2 ] = elevation * ( flag_far ? 5.0f : 1.0f ) ;
  } 

  ~Camera() {}

  void setPan  ( float amnt ) { heading   = amnt ; update_cam_rel_tgt () ; }
  void setLift ( float amnt ) { elevation = amnt ; update_cam_rel_tgt () ; }
  void setZoom ( float amnt ) { range     = amnt ; update_cam_rel_tgt () ; }
  void setFollow ( int f )    { follow = f ; }

  void pan  ( float amnt ) { heading   += amnt ; update_cam_rel_tgt () ; }
  void lift ( float amnt ) { elevation += amnt ; update_cam_rel_tgt () ; }
  void zoom ( float amnt ) { range     += amnt ; update_cam_rel_tgt () ; }
  void setFOV ( int w )    { wide_fov = w ; }

  void toggle_far     () { flag_far = ! flag_far ; update_cam_rel_tgt () ; }
  void toggle_widefov () { setFOV ( ! wide_fov )  ; }

  void toggle_follow  ()   { follow = ! follow ; }
  int  isFollowing () ;

  int  joystickInput ( JoyInfo *j ) ;
  void keyboardInput ( int c ) ;

  void immediate_reset ()
  {
    reset () ;
    follow_tux ( TRUE ) ;
  }

  void reset ()
  {
    range     = 25.0f ;
    elevation =  7.0f ;
    follow    = FALSE ;
    flag_far  = FALSE ;
    setFOV ( FALSE ) ;
    update_cam_rel_tgt () ;
  }

  void update () ;

  void follow_tux   ( int immediate = FALSE ) ;

  float getHeading () { return heading ; }
  int isSubmerged () ;
} ;


#define BLEND_RATE 30.0f

float blend_angle ( float a1, float a2, float rate = BLEND_RATE ) ;

