
class Snowball
{
  ssgCutout *tr ;

public:
  Snowball () ;
  ssgEntity *getRoot () { return tr ; }
} ;


class SmokePuff
{
  ssgCutout *tr ;

public:
  SmokePuff () ;
  ssgEntity *getRoot () { return tr ; }
} ;


class Bubble
{
  ssgCutout *tr ;

public:
  Bubble () ;
  ssgEntity *getRoot () { return tr ; }
} ;


class MagicRing
{
  ssgCutout *sh ;
  sgVec2 *tcoords ;
  sgVec2 *safe_tcoords ;

public:
  MagicRing ( float x1, float y1 ) ;
  ssgEntity *getRoot () { return sh ; }
  void update () ;
} ;


class Shadow
{
  ssgBranch *sh ;

public:
  Shadow ( float x1, float x2, float y1, float y2 ) ;
  ssgEntity *getRoot () { return sh ; }
} ;


class Herring
{
  float h ;
  ssgTransform *tr ;

  Shadow *sh ;

public:
  Herring ( sgVec4 colour ) ;
  ssgTransform *getRoot () { return tr ; }
  void update () ;
} ;


class TuxLife
{
  float h ;
  ssgTransform *tr ;

  Shadow *sh ;

public:
  TuxLife () ;
  ssgTransform *getRoot () { return tr ; }
  void update () ;
} ;


