
class WhaleState
{
  Orca         *whale    ;
  sgCoord       coord    ;
  sgVec3        velocity ;
  int           munch_timer ;
  ssgTransform *dcs      ;
  sgVec3        normal   ;
  int           enable   ;

public:

  WhaleState ()
  {
    whale = NULL ;
    enable = FALSE ;
  }

  void setEnable ( int e ) { enable = e ; }
  void init ()
  {
    whale = new Orca () ;
    dcs = whale -> getRoot () ;
    munch_timer = 0 ;
    enable = FALSE ;
  }

  ssgTransform *getDCS () { return dcs ; }

  void setDefault ( sgVec3 xyz, sgVec3 hpr )
  {
    sgCopyVec3 ( coord.xyz, xyz ) ;
    sgCopyVec3 ( coord.hpr, hpr ) ;
  }

  void update    () ;
} ;
