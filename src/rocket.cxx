#include "tux.h"

void Rocket::update ( int all, int /*gun*/, int exhaust )
{
      all_switch -> select (  all     ? 0xFFFFFFFF : 0x00000000 ) ;
  exhaust_switch -> select (  exhaust ? 0xFFFFFFFF : 0x00000000 ) ;
}


Rocket::Rocket ()
{
  ssgModelPath   ( "models" ) ;
  ssgTexturePath ( "images" ) ;

  sgCoord rock_coord ;
  sgSetVec3 ( rock_coord.xyz, 0.0,  0.9, 1.7 ) ;
  sgSetVec3 ( rock_coord.hpr, 90.0, 0.0,-15.0 ) ;
  rocket = new ssgTransform ( &rock_coord ) ;
  rocket -> clrTraversalMaskBits ( SSGTRAV_ISECT|SSGTRAV_HOT ) ;
  all_switch     = new ssgSelector () ;
  exhaust_switch = new ssgSelector () ;
  ssgEntity *rr  = ssgLoad ( "rocket.ac", loader_opts ) ;
  ssgEntity *re  = ssgLoad ( "exhaust.ac", loader_opts ) ;
  ssgBranch *rg  = new ssgBranch () ;

  rg -> addKid ( rr ) ; ssgFlatten ( rr ) ;
  rg -> addKid ( exhaust_switch ) ;
  ssgStripify ( rg ) ;

  all_switch -> addKid ( rg ) ;
  exhaust_switch -> addKid ( re ) ; ssgFlatten ( re ) ;
  ssgStripify ( exhaust_switch ) ;
  rocket -> addKid ( all_switch ) ;
  rocket -> ref () ;
  
  update ( 0, 0, 0 ) ;
}


