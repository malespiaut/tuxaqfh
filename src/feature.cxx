#include "tux.h"

void TuxLife::update ()
{
  sgCoord c = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 } } ;

  c . hpr [ 0 ] = h ;

  h += 11.0f ;

  tr -> setTransform ( &c ) ;
}


TuxLife::TuxLife ()
{
  sgVec4 *scolors = new sgVec4 [ 1 ] ;
  sgVec3 *scoords = new sgVec3 [ 6 ] ;
  sgVec2 *tcoords = new sgVec2 [ 6 ] ;
  sgVec3 *snorms  = new sgVec3 [ 1 ] ;

  sgSetVec4 ( scolors[0], 1.0f, 1.0f, 1.0f, 1.0f ) ;
  sgSetVec3 ( snorms [0], 0.0f, 1.0f, 0.0f ) ;

  sgSetVec3(scoords[0], -0.5, 0.0, 0.0 ) ;
  sgSetVec3(scoords[1],  0.5, 0.0, 0.0 ) ;
  sgSetVec3(scoords[2], -0.5, 0.0, 1.2 ) ;
  sgSetVec3(scoords[3],  0.5, 0.0, 1.2 ) ;
  sgSetVec3(scoords[4], -0.5, 0.0, 0.0 ) ;
  sgSetVec3(scoords[5],  0.5, 0.0, 0.0 ) ;

  sgSetVec2(tcoords[0], 0.0, 0.0 ) ;
  sgSetVec2(tcoords[1], 1.0, 0.0 ) ;
  sgSetVec2(tcoords[2], 0.0, 1.0 ) ;
  sgSetVec2(tcoords[3], 1.0, 1.0 ) ;
  sgSetVec2(tcoords[4], 0.0, 0.0 ) ;
  sgSetVec2(tcoords[5], 1.0, 0.0 ) ;

  ssgLeaf *gset = new ssgVTable ( GL_TRIANGLE_STRIP,
                     6, scoords,
                     1, snorms,
                     6, tcoords,
                     1, scolors ) ;

  gset -> setState ( tinytux_gst ) ;

  h = 0.0f ;

  sh = new Shadow ( -0.5, 0.5, -0.5, 0.5 ) ;

  tr = new ssgTransform () ;

  tr -> addKid ( sh -> getRoot () ) ;
  tr -> addKid ( gset ) ;
  tr -> ref () ; /* Make sure it doesn't get deleted by mistake */
}



void Herring::update ()
{
  sgCoord c = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 } } ;

  c . hpr [ 0 ] = h ;

  h += 5.0f ;

  tr -> setTransform ( &c ) ;
}


Herring::Herring ( sgVec3 colour )
{
  sgVec4 *scolors = new sgVec4 [ 1 ] ;
  sgVec3 *scoords = new sgVec3 [ 6 ] ;
  sgVec2 *tcoords = new sgVec2 [ 6 ] ;
  sgVec3 *snorms  = new sgVec3 [ 1 ] ;

  sgCopyVec3 ( scolors [ 0 ], colour ) ;
  scolors [ 0 ][ 3 ] = 1.0f ;
  sgSetVec3 ( snorms[0],  0.0f,  1.0f,  0.0f ) ;

  sgSetVec3(scoords[0], -1.0, 0.0, 0.0 ) ;
  sgSetVec3(scoords[1],  1.0, 0.0, 0.0 ) ;
  sgSetVec3(scoords[2], -1.0, 0.0, 1.0 ) ;
  sgSetVec3(scoords[3],  1.0, 0.0, 1.0 ) ;
  sgSetVec3(scoords[4], -1.0, 0.0, 0.0 ) ;
  sgSetVec3(scoords[5],  1.0, 0.0, 0.0 ) ;

  sgSetVec2(tcoords[0], 0.0, 0.0 ) ;
  sgSetVec2(tcoords[1], 1.0, 0.0 ) ;
  sgSetVec2(tcoords[2], 0.0, 1.0 ) ;
  sgSetVec2(tcoords[3], 1.0, 1.0 ) ;
  sgSetVec2(tcoords[4], 0.0, 0.0 ) ;
  sgSetVec2(tcoords[5], 1.0, 0.0 ) ;

  ssgLeaf *gset = new ssgVTable ( GL_TRIANGLE_STRIP,
                     6, scoords,
                     1, snorms,
                     6, tcoords,
                     1, scolors ) ;

  gset -> setState ( herring_gst ) ;

  h = 0.0f ;

  sh = new Shadow ( -1.0, 1.0, -0.5, 0.5 ) ;

  tr = new ssgTransform () ;

  tr -> addKid ( sh -> getRoot () ) ;
  tr -> addKid ( gset ) ;
  tr -> ref () ; /* Make sure it doesn't get deleted by mistake */
}


Snowball::Snowball ()
{
  tr = new ssgCutout ( TRUE ) ;
  tr -> clrTraversalMaskBits ( SSGTRAV_ISECT|SSGTRAV_HOT ) ;

  sgVec4 *scolors = new sgVec4 [ 1 ] ;

  sgVec3 *scoords = new sgVec3 [ 3 ] ;
  sgVec2 *tcoords = new sgVec2 [ 3 ] ;
  sgVec3 *snorms  = new sgVec3 [ 1 ] ;

  sgSetVec4(scolors[0], 1.0f, 1.0f, 1.0f, 1.0f ) ;
  sgSetVec3(snorms[0],  0.0f,  1.0f,  0.0f);

  sgSetVec3(scoords[0], 0.0, 0.0, -0.4 ) ;
  sgSetVec3(scoords[1], 0.4, 0.0,  0.3 ) ;
  sgSetVec3(scoords[2],-0.4, 0.0,  0.3 ) ;

  sgSetVec2(tcoords[0], 0.5, 1.3 ) ;
  sgSetVec2(tcoords[1], 1.5, 0.0 ) ;
  sgSetVec2(tcoords[2],-0.5, 0.0 ) ;
 
  ssgLeaf *gs = new ssgVTable ( GL_TRIANGLE_STRIP,
         3, scoords, 1, snorms, 3, tcoords, 1, scolors ) ; 
  gs -> setState ( snowball_gst ) ;
  tr -> addKid ( gs ) ;
  tr -> ref () ; /* Make sure it doesn't get deleted by mistake */
}


SmokePuff::SmokePuff ()
{
  tr = new ssgCutout ( TRUE ) ;
  tr -> clrTraversalMaskBits ( SSGTRAV_ISECT|SSGTRAV_HOT ) ;

  sgVec4 *scolors = new sgVec4 [ 1 ] ;
  sgVec3 *scoords = new sgVec3 [ 3 ] ;
  sgVec2 *tcoords = new sgVec2 [ 3 ] ;
  sgVec3 *snorms  = new sgVec3 [ 1 ] ;

  sgSetVec4 ( scolors[0], 1.0f, 1.0f, 1.0f, 1.0f ) ;
  sgSetVec3 ( snorms [0], 0.0f, 1.0f, 0.0f ) ;

  sgSetVec3 ( scoords[0], 0.0, 0.0, -2.4 ) ;
  sgSetVec3 ( scoords[1], 2.4, 0.0,  1.6 ) ;
  sgSetVec3 ( scoords[2],-2.4, 0.0,  1.6 ) ;

  sgSetVec2 ( tcoords[0], 0.5, 1.3 ) ;
  sgSetVec2 ( tcoords[1], 1.5, 0.0 ) ;
  sgSetVec2 ( tcoords[2],-0.5, 0.0 ) ;
 
  ssgLeaf *gs = new ssgVTable ( GL_TRIANGLE_STRIP,
         3, scoords, 1, snorms, 3, tcoords, 1, scolors ) ; 

  gs -> setState ( smoke_gst ) ;
  tr -> addKid ( gs ) ;
  tr -> ref () ; /* Make sure it doesn't get deleted by mistake */
}



Bubble::Bubble ()
{
  tr = new ssgCutout ( TRUE ) ;
  tr -> clrTraversalMaskBits ( SSGTRAV_ISECT|SSGTRAV_HOT ) ;

  sgVec4 *scolors = new sgVec4 [ 1 ] ;

  sgVec3 *scoords = new sgVec3 [ 3 ] ;
  sgVec2 *tcoords = new sgVec2 [ 3 ] ;
  sgVec3 *snorms  = new sgVec3 [ 1 ] ;

  sgSetVec4(scolors[0], 1.0f, 1.0f, 1.0f, 1.0f ) ;
  sgSetVec3(snorms[0],  0.0f,  1.0f,  0.0f);

  sgSetVec3(scoords[0], 0.0, 0.0, -0.6 ) ;
  sgSetVec3(scoords[1], 0.6, 0.0,  0.4 ) ;
  sgSetVec3(scoords[2],-0.6, 0.0,  0.4 ) ;

  sgSetVec2(tcoords[0], 0.5,-0.3 ) ;
  sgSetVec2(tcoords[1], 1.5, 1.0 ) ;
  sgSetVec2(tcoords[2],-0.5, 1.0 ) ;
 
  ssgLeaf *gs = new ssgVTable ( GL_TRIANGLE_STRIP,
         3, scoords, 1, snorms, 3, tcoords, 1, scolors ) ; 
  gs -> setState ( bubble_gst ) ;
  tr -> addKid ( gs ) ;
  tr -> ref () ; /* Make sure it doesn't get deleted by mistake */
}



MagicRing::MagicRing ( float x1, float y1 )
{
  sh = new ssgCutout ( TRUE ) ;

  tcoords      = new sgVec2 [ 10 ] ;
  safe_tcoords = new sgVec2 [ 10 ] ;

  sgVec3 *scoords = new sgVec3 [ 10 ] ;
  sgVec3 *snorms  = new sgVec3 [  1 ] ;

  sgSetVec3 ( snorms[0], 0.0f, 0.0f, 1.0f ) ;
  sgSetVec3 ( scoords[0], 0.0, 0.0, 0.0 + y1 ) ;
  sgSetVec2 ( tcoords[0], 0.0, 0.0 ) ;

  safe_tcoords[0][0] = tcoords[0][0] ;
  safe_tcoords[0][1] = tcoords[0][1] ;

  for ( int i = 1 ; i < 10 ; i++ )
  {
    sgSetVec3(scoords[i], x1 * sin(-45.0*(float)i*SG_DEGREES_TO_RADIANS), 0.0,
                          y1 * cos(-45.0*(float)i*SG_DEGREES_TO_RADIANS) + y1 );
    sgSetVec2(tcoords[i], sin(-45.0*(float)i*SG_DEGREES_TO_RADIANS),
                          cos(-45.0*(float)i*SG_DEGREES_TO_RADIANS) ) ;

    safe_tcoords[i][0] = tcoords[i][0] ;
    safe_tcoords[i][1] = tcoords[i][1] ;
  }
 
  ssgVTable *gs = new ssgVTable ( GL_TRIANGLE_FAN,
         10, scoords, 1, snorms, 10, tcoords, 0, NULL ) ;

  gs -> setState ( sparkle_gst ) ;
  sh -> addKid ( gs ) ;
  sh -> ref () ; /* Make sure it doesn't get deleted by mistake */
}


void MagicRing::update ()
{
  static float tx = 0.0f ;
  static float ty = 0.0f ;

  tx += 59.134341f / 256.0f ;
  ty += 27.873242f / 256.0f ;
 
  if ( tx > 10.0 ) tx -= 10.0 ;
  if ( ty > 10.0 ) ty -= 10.0 ;

  for ( int i = 0 ; i < 10 ; i++ )
  {
    tcoords[i][0] = 0.2 * safe_tcoords[i][0] + tx ;
    tcoords[i][1] = 0.2 * safe_tcoords[i][1] + ty ;
  }
}

Shadow::Shadow ( float x1, float x2, float y1, float y2 )
{
  sgVec4 *scolors = new sgVec4 [ 1 ] ;
  sgVec3 *scoords = new sgVec3 [ 4 ] ;
  sgVec2 *tcoords = new sgVec2 [ 4 ] ;
  sgVec3 *snorms  = new sgVec3 [ 1 ] ;

  sgSetVec4 ( scolors[0], 0.0f, 0.0f, 0.0f, 1.0f ) ;
  sgSetVec3 ( snorms [0], 0.0f, 0.0f, 1.0f ) ;

  sgSetVec3 ( scoords[0], x1, y1, 0.10 ) ;
  sgSetVec3 ( scoords[1], x2, y1, 0.10 ) ;
  sgSetVec3 ( scoords[2], x1, y2, 0.10 ) ;
  sgSetVec3 ( scoords[3], x2, y2, 0.10 ) ;

  sgSetVec2 ( tcoords[0], 0.0, 0.0 ) ;
  sgSetVec2 ( tcoords[1], 1.0, 0.0 ) ;
  sgSetVec2 ( tcoords[2], 0.0, 1.0 ) ;
  sgSetVec2 ( tcoords[3], 1.0, 1.0 ) ;
 
  sh = new ssgBranch ;
  sh -> clrTraversalMaskBits ( SSGTRAV_ISECT|SSGTRAV_HOT ) ;

  sh -> setName ( "Shadow" ) ;

  ssgVTable *gs = new ssgVTable ( GL_TRIANGLE_STRIP,
                       4, scoords, 1, snorms, 4, tcoords, 1, scolors ) ;

  gs -> clrTraversalMaskBits ( SSGTRAV_ISECT|SSGTRAV_HOT ) ;
  gs -> setState ( fuzzy_gst ) ;
  sh -> addKid ( gs ) ;
  sh -> ref () ; /* Make sure it doesn't get deleted by mistake */
}

