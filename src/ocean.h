
#define DEEPEST_HELL -1000000.0f

enum OceanType
{
  OCEAN_WATER,
  OCEAN_LAVA,
  OCEAN_ICE,
  OCEAN_BLACKHOLE
} ;

class Ocean
{
  sgVec3 *wvlist ;
  sgVec3 *wnlist ;
  sgVec2 *wtlist ;
  sgVec4 *wclist ;

  OceanType type ;
  ssgBranch *ocean ;

public:
 
  Ocean ( OceanType t ) ;
  ~Ocean () ;

  void update () ;

  OceanType getType () { return type ; }
  ssgEntity *getRoot () { return ocean ; }
} ;



