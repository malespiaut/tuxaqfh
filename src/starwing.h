
enum SG_State
{
  SG_UPBEAT,
  SG_DOWNBEAT,
  SG_RESTING
} ;


class StarWing
{
  float       energy ;  /* Percentage */
  SG_State     state ;
  StarWing     *next ;
  sgCoord      coord ;
  sgCoord last_coord ;
  sgVec3        dirn ;
  ssgTransform  *obj ;

  void find_nearest ( sgVec3 nearest ) ;
  void strategy     ( sgVec3 new_direction ) ;

public:

  StarWing ( ssgTransform *o ) ;

  void     update   () ;

  void     setCoord ( sgCoord *c ) { sgCopyCoord ( & coord  , c ) ; }
  void     setHPR   ( float *c   ) { sgCopyVec3  ( coord.hpr, c ) ; }
  void     setXYZ   ( float *c   ) { sgCopyVec3  ( coord.xyz, c ) ; }

  float    *getDirn  () { return  dirn  ; }
  sgCoord  *getCoord () { return &coord ; }
  StarWing *getNext  () { return  next  ; }
} ;

