#include "tux.h"


static float wtmaster [ 6 ][ 2 ] =
{
  { -10000.0f/64.0f, -10000.0f/64.0f },
  {  10000.0f/64.0f, -10000.0f/64.0f },
  { -10000.0f/64.0f,  10000.0f/64.0f },
  {  10000.0f/64.0f,  10000.0f/64.0f },
  { -10000.0f/64.0f, -10000.0f/64.0f },
  {  10000.0f/64.0f, -10000.0f/64.0f },
} ;

void Ocean::update ()
{
  /* Moving texture for the water. */

  static float e_offset = 0.0f ;
  static float e_delta  = 0.01f;
  static float u_offset = 0.0f ;
  static float v_offset = 0.0f ;

  if ( type == OCEAN_WATER || type == OCEAN_LAVA )
  {
    e_offset += e_delta ;
    u_offset += 0.0008 ;
    v_offset += 0.0005 ;

    if ( e_offset >= 0.2f ||
	 e_offset < -0.1f ) e_delta = -e_delta ;

    if ( u_offset >= 1.0f ) u_offset -= 1.0f ;
    if ( v_offset >= 1.0f ) v_offset -= 1.0f ;

    for ( int i = 0 ; i < 6 ; i++ )
    {
      wtlist [ i ] [ 0 ] = wtmaster [ i ] [ 0 ] + u_offset ;
      wtlist [ i ] [ 1 ] = wtmaster [ i ] [ 1 ] + u_offset ;
      wvlist [ i ] [ 2 ] = e_offset ;
    }
  }
}


Ocean::~Ocean ()
{
  ssgDeRefDelete ( ocean ) ;
}

Ocean::Ocean ( OceanType t )
{
  type = t ;
  ocean = new ssgBranch () ;
  ocean -> setName ( "Ocean" ) ;

  wvlist = new sgVec3 [ 6 ] ;
  wtlist = new sgVec2 [ 6 ] ;
  wnlist = new sgVec3 [ 1 ] ;
  wclist = new sgVec4 [ 1 ] ;

  float elevation ;

  switch ( type )
  {
    case OCEAN_WATER : elevation = 0.0f ; break ;
    case OCEAN_LAVA  : elevation = 0.1f ; break ;
    case OCEAN_ICE   : elevation = 0.1f ; break ;
    case OCEAN_BLACKHOLE : elevation = -22.0f ; break ;
    default          : elevation = -1000000.0f ; break ;
  }

  sgSetVec3 ( wvlist [ 0 ], -10000.0f, -10000.0f, elevation ) ;
  sgSetVec3 ( wvlist [ 1 ],  10000.0f, -10000.0f, elevation ) ;
  sgSetVec3 ( wvlist [ 2 ], -10000.0f,  10000.0f, elevation ) ;
  sgSetVec3 ( wvlist [ 3 ],  10000.0f,  10000.0f, elevation ) ;
  sgSetVec3 ( wvlist [ 4 ], -10000.0f, -10000.0f, elevation ) ;
  sgSetVec3 ( wvlist [ 5 ],  10000.0f, -10000.0f, elevation ) ;

  sgSetVec3 ( wnlist [ 0 ],  0.0f, 0.0f, 1.0f ) ;

  update () ;  /* Computes initial texture position conveniently */

  ssgVTable *wgs = new ssgVTable ( GL_TRIANGLE_STRIP,
                          6, wvlist, 1, wnlist, 6, wtlist, 1, wclist ) ;

  switch ( type )
  {
    case OCEAN_WATER : wgs -> setState ( water_gst ) ;
                       sgSetVec4 ( wclist [ 0 ],  1.0f, 1.0f, 1.0f, 0.7f ) ;
fprintf(stderr,"Zapping traversal masks!\n" ) ;
                       ocean -> clrTraversalMaskBits ( SSGTRAV_ISECT|SSGTRAV_HOT ) ;
                       break ;
    case OCEAN_LAVA  : wgs -> setState ( lava_gst ) ;
                       sgSetVec4 ( wclist [ 0 ],  1.0f, 1.0f, 1.0f, 1.0f ) ;
                       break ;
    case OCEAN_ICE   : wgs -> setState ( clearice_gst ) ;
                       sgSetVec4 ( wclist [ 0 ],  0.5f, 0.5f, 1.0f, 0.7f ) ;
                       break ;
    case OCEAN_BLACKHOLE : wgs -> setState ( solidblack_gst ) ;
                       sgSetVec4 ( wclist [ 0 ],  0.0f, 0.0f, 0.0f, 1.0f ) ;
                       break ;
  }

  ocean -> addKid ( wgs ) ;
  ocean -> recalcBSphere () ;
  ocean -> ref () ;
}


