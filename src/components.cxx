
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <math.h>
#include <plib/ssg.h>

#include "penguin.h"


ssgEntity *MakeFlipper ( ssgState *gstate, float dx, float dy, float dz )
{
  sgVec4 *scolors = new sgVec4 [  1 ] ;
  sgVec3 *snorms  = new sgVec3 [  2 ] ;
  sgVec3 *scoords = new sgVec3 [ 14 ] ;

  sgSetVec4 ( scolors [0], 0.0f, 0.0f, 0.0f, 1.0f ) ;
  sgSetVec3 ( snorms  [0], 0.0f, 0.0f, 1.0f ) ;
  sgSetVec3 ( snorms  [1], 0.0f, 0.0f,-1.0f ) ;

  sgSetVec3(scoords[ 0], 0.0 * dx,-0.5 * dy, 0 );
  sgSetVec3(scoords[ 1], 0.0 * dx, 0.5 * dy, 0 );
  sgSetVec3(scoords[ 2], 0.3 * dx,-0.5 * dy, -0.1 * dz );
  sgSetVec3(scoords[ 3], 0.3 * dx, 0.5 * dy, -0.1 * dz );
  sgSetVec3(scoords[ 4], 0.7 * dx,-0.5 * dy, -0.5 * dz );
  sgSetVec3(scoords[ 5], 0.7 * dx, 0.5 * dy, -0.5 * dz );
  sgSetVec3(scoords[ 6], 1.0 * dx, 0.0 * dy, -1.0 * dz );

  sgSetVec3(scoords[ 7], 1.0 * dx, 0.0 * dy, -1.0 * dz );
  sgSetVec3(scoords[ 8], 0.7 * dx, 0.5 * dy, -0.5 * dz );
  sgSetVec3(scoords[ 9], 0.7 * dx,-0.5 * dy, -0.5 * dz );
  sgSetVec3(scoords[10], 0.3 * dx, 0.5 * dy, -0.1 * dz );
  sgSetVec3(scoords[11], 0.3 * dx,-0.5 * dy, -0.1 * dz );
  sgSetVec3(scoords[12], 0.0 * dx, 0.5 * dy, 0 );
  sgSetVec3(scoords[13], 0.0 * dx,-0.5 * dy, 0 );

  ssgLeaf *gset1 = new ssgVTable ( GL_TRIANGLE_STRIP,
                     7, scoords, 1, snorms, 0, NULL, 1, scolors ) ;
  ssgLeaf *gset2 = new ssgVTable ( GL_TRIANGLE_STRIP,
                     7, &scoords[5], 1, &snorms[1], 0, NULL, 1, scolors ) ;

  gset1 -> setState ( gstate ) ;
  gset2 -> setState ( gstate ) ;

  ssgBranch *branch = new ssgBranch () ;
  branch -> setName ( "Flipper" ) ;

  branch -> addKid ( gset1 ) ;
  branch -> addKid ( gset2 ) ;

  return branch ;
}



ssgEntity *MakeCloak (  ssgState *gstate,
                      float dx, float dy, float dz, sgVec3 *vcoord )
{
  sgVec4 *scolors = new sgVec4 [ 1 ] ;
  sgVec2 *tcoords = new sgVec2 [ 16 ] ;
  sgVec3 *snorms  = new sgVec3 [ 2 ] ;
  sgVec3 *scoords = vcoord ;

  sgSetVec4 ( scolors [0], 1.0f, 1.0f, 1.0f, 1.0f ) ;
  sgSetVec3 ( snorms  [0], 0.0f, 0.0f, 1.0f ) ;
  sgSetVec3 ( snorms  [1], 0.0f, 0.0f,-1.0f ) ;

  unsigned short *vtindex = new unsigned short [ 16 ] ;
  unsigned short *cnindex = new unsigned short [ 1 ] ;

  int i ;

  cnindex [ 0 ] = 0 ;

  for ( i = 0 ; i < 8 ; i++ )
    vtindex [ i ] = i ;
  vtindex [ 8 ] = 0 ;
  for ( i = 9 ; i < 16 ; i++ )
    vtindex [ i ] = 16 - i ;

  sgSetVec3(scoords[ 0], 0.0 * dx,-0.35* dy, -0.1 * dz );
  sgSetVec3(scoords[ 1],-0.3 * dx, 0.2 * dy, -0.0 * dz );
  sgSetVec3(scoords[ 2],-0.8 * dx,-0.7 * dy, -0.5 * dz );
  sgSetVec3(scoords[ 3],-1.0 * dx,-1.0 * dy, -1.0 * dz );
  sgSetVec3(scoords[ 4], 0.0 * dx,-1.7 * dy, -1.1 * dz );
  sgSetVec3(scoords[ 5], 1.0 * dx,-1.0 * dy, -1.0 * dz );
  sgSetVec3(scoords[ 6], 0.8 * dx,-0.7 * dy, -0.5 * dz );
  sgSetVec3(scoords[ 7], 0.3 * dx,-0.2 * dy, -0.0 * dz );

  sgSetVec2(tcoords[ 0], 0.0, 0.0 );
  sgSetVec2(tcoords[ 1],-2.0, 0.0 );
  sgSetVec2(tcoords[ 2],-1.1,-0.3 );
  sgSetVec2(tcoords[ 3],-1.0,-1.0 );
  sgSetVec2(tcoords[ 4], 0.0,-1.0 );
  sgSetVec2(tcoords[ 5], 1.0,-1.0 );
  sgSetVec2(tcoords[ 6], 1.1,-0.3 );
  sgSetVec2(tcoords[ 7], 2.0,-0.0 );

  ssgLeaf *gset1 = new ssgVTable ( GL_TRIANGLE_FAN,
                     8, vtindex, scoords,
                     1, cnindex, snorms,
                     8, vtindex, tcoords,
                     1, cnindex, scolors ) ;
  ssgLeaf *gset2 = new ssgVTable ( GL_TRIANGLE_FAN,
                     8, &vtindex[8],  scoords,
                     1,  cnindex   , &snorms[1],
                     8, &vtindex[8],  tcoords,
                     1,  cnindex   ,  scolors ) ;

  gset1 -> setState ( gstate ) ;
  gset2 -> setState ( gstate ) ;

  ssgBranch *branch = new ssgBranch () ;
  branch -> setName ( "Cloak" ) ;

  branch -> addKid ( gset1 ) ;
  branch -> addKid ( gset2 ) ;

  return branch ;
} 


ssgEntity *MakeRocketFin ( ssgState *gstate )
{
  sgVec4 *scolors = new sgVec4 [  1 ] ;
  sgVec3 *snorms  = new sgVec3 [  2 ] ;
  sgVec3 *scoords = new sgVec3 [ 10 ] ;
  sgVec2 *stex    = new sgVec2 [ 10 ] ;

  sgSetVec4 ( scolors [0], 1.0f, 1.0f, 1.0f, 1.0f ) ;
  sgSetVec3 ( snorms  [0], 0.0f, 1.0f, 0.0f ) ;
  sgSetVec3 ( snorms  [1], 0.0f,-1.0f, 0.0f ) ;

  unsigned short *vindex = new unsigned short [ 19 ] ;
  unsigned short *nindex = new unsigned short [ 2 ] ;

  vindex [ 0 ] = 5 ; vindex [ 1 ] = 4 ; vindex [ 2 ] = 6 ; vindex [ 3 ] = 3 ;
  vindex [ 4 ] = 7 ; vindex [ 5 ] = 2 ; vindex [ 6 ] = 8 ; vindex [ 7 ] = 1 ;
  vindex [ 8 ] = 9 ; vindex [ 9 ] = 0 ; vindex [10 ] = 9 ; vindex [11 ] = 1 ;
  vindex [12 ] = 8 ; vindex [13 ] = 2 ; vindex [14 ] = 7 ; vindex [15 ] = 3 ;
  vindex [16 ] = 6 ; vindex [17 ] = 4 ; vindex [18 ] = 5 ;


  sgSetVec3(scoords[ 0], 0.34, 0.0, -0.6 );
  sgSetVec3(scoords[ 1], 0.50, 0.0, -0.8 );
  sgSetVec3(scoords[ 2], 0.60, 0.0, -1.0 );
  sgSetVec3(scoords[ 3], 0.61, 0.0, -1.2 );
  sgSetVec3(scoords[ 4], 0.58, 0.0, -1.4 );
  sgSetVec3(scoords[ 5], 0.45, 0.0, -1.4 );
  sgSetVec3(scoords[ 6], 0.40, 0.0, -1.2 );
  sgSetVec3(scoords[ 7], 0.30, 0.0, -1.1 );
  sgSetVec3(scoords[ 8], 0.20, 0.0, -1.0 );
  sgSetVec3(scoords[ 9], 0.27, 0.0, -0.8 );

  sgSetVec2(stex[ 0], 0.0 , 1.1 );
  sgSetVec2(stex[ 1], 0.75, 1.0 );
  sgSetVec2(stex[ 2], 1.0 , 0.5 );
  sgSetVec2(stex[ 3], 1.0 , 0.0 );
  sgSetVec2(stex[ 4], 1.0 ,-0.1 );
  sgSetVec2(stex[ 5], 0.7 ,-0.1 );
  sgSetVec2(stex[ 6], 0.5 , 0.0 );
  sgSetVec2(stex[ 7], 0.25, 0.25);
  sgSetVec2(stex[ 8], 0.0 , 0.5 );
  sgSetVec2(stex[ 9], 0.25, 1.0 );

  ssgLeaf *gset1 = new ssgVTable ( GL_TRIANGLE_STRIP,
                     9, vindex, scoords,
                     1, nindex, snorms,
                     9, vindex, stex,
                     1, nindex, scolors ) ;
  ssgLeaf *gset2 = new ssgVTable ( GL_TRIANGLE_STRIP,
                     9, &vindex[9],  scoords,
                     1,  nindex   , &snorms[1],
                     9, &vindex[9],  stex,
                     1,  nindex   ,  scolors ) ;

  gset1 -> setState ( gstate ) ;
  gset2 -> setState ( gstate ) ;

  ssgBranch *branch = new ssgBranch () ;
  branch -> setName ( "RocketFin" ) ;

  branch -> addKid ( gset1 ) ;
  branch -> addKid ( gset2 ) ;

  return branch ;
}


ssgEntity *MakeFlukes ( ssgState *gstate, float dx, float dy, float dz, int fluke )
{
  sgVec4 *scolors = new sgVec4 [  1 ] ;
  sgVec3 *snorms  = new sgVec3 [  1 ] ;
  sgVec3 *scoords = new sgVec3 [ 10 ] ;

  sgSetVec4 ( scolors [0], 0.0f, 0.0f, 0.0f, 1.0f ) ;
  sgSetVec3 ( snorms  [0], 0.0f, 0.0f, 1.0f ) ;

  if ( fluke )
  {
    sgSetVec3 ( scoords [0], -1.0 * dx,  1.2 * dy, -0.1 * dz ) ;
    sgSetVec3 ( scoords [1], -1.0 * dx,  2.0 * dy, -0.2 * dz ) ;
    sgSetVec3 ( scoords [2],  0.0 * dx,  0.0 * dy,  0.0 * dz ) ;
    sgSetVec3 ( scoords [3],  0.0 * dx,  1.5 * dy,  0.0 * dz ) ;
    sgSetVec3 ( scoords [4],  1.0 * dx,  1.2 * dy, -0.1 * dz ) ;
    sgSetVec3 ( scoords [5],  1.0 * dx,  2.0 * dy, -0.2 * dz ) ;
    sgSetVec3 ( scoords [6],  0.0 * dx,  0.0 * dy,  0.0 * dz ) ;
    sgSetVec3 ( scoords [7],  0.0 * dx,  1.5 * dy,  0.0 * dz ) ;
    sgSetVec3 ( scoords [8], -1.0 * dx,  1.2 * dy, -0.1 * dz ) ;
    sgSetVec3 ( scoords [9], -1.0 * dx,  2.0 * dy, -0.2 * dz ) ;
  }
  else
  {
    sgSetVec3 ( scoords [0],  0.0 * dx,  0.0 * dy,  0.0 * dz ) ;
    sgSetVec3 ( scoords [1],  0.0 * dx, -1.0 * dy,  0.0 * dz ) ;
    sgSetVec3 ( scoords [2],  0.0 * dx, -0.6 * dy,  1.0 * dz ) ;
    sgSetVec3 ( scoords [3],  0.0 * dx, -1.1 * dy,  0.5 * dz ) ;
    sgSetVec3 ( scoords [4],  0.0 * dx, -1.2 * dy,  1.2 * dz ) ;
    sgSetVec3 ( scoords [5],  0.0 * dx, -1.4 * dy,  1.0 * dz ) ;
    sgSetVec3 ( scoords [6],  0.0 * dx, -0.6 * dy,  1.0 * dz ) ;
    sgSetVec3 ( scoords [7],  0.0 * dx, -1.1 * dy,  0.5 * dz ) ;
    sgSetVec3 ( scoords [8],  0.0 * dx,  0.0 * dy,  0.0 * dz ) ;
    sgSetVec3 ( scoords [9],  0.0 * dx, -1.0 * dy,  0.0 * dz ) ;
  }

  ssgLeaf *gset = new ssgVTable ( GL_TRIANGLE_STRIP,
                    10, scoords,
                     1, snorms,
                     0, NULL,
                     1, scolors ) ;

  gset -> setState ( gstate ) ;

  return gset ;
}


float x2u ( int x ) { return        (float) x / 64.0f ; }
float y2v ( int y ) { return 1.0f - (float) y / 64.0f ; }

ssgEntity *MakePartyHat ( ssgState *gstate, float dx, float dy, float dz )
{
  sgVec4 *scolors = new sgVec4 [  1 ] ;
  sgVec3 *snorms  = new sgVec3 [  1 ] ;
  sgVec3 *scoords = new sgVec3 [ 10 ] ;
  sgVec2 *tcoords = new sgVec2 [ 10 ] ;

  sgSetVec4 ( scolors [0], 1.0f, 1.0f, 1.0f, 1.0f ) ;
  sgSetVec3 ( snorms  [0], 0.0f, 0.0f, 1.0f ) ;

  sgSetVec3 ( scoords[0], 0.0 * dx, 0.0 * dy, 1.0 * dz ) ;
  sgSetVec2 ( tcoords[0], x2u ( 0 ), y2v ( 0 ) ) ;

  for ( int i = 0 ; i < 9 ; i++ )
  {
    float a = (float)((i==8)?0:i) * 2.0f * M_PI / 8.0f ;

    sgSetVec3 ( scoords[i+1], -sin(a) * dx, cos(a) * dy, 0.0 ) ;
    sgSetVec2 ( tcoords[i+1], x2u ( i * 8 ), y2v ( 19 ) ) ;
  }

  ssgLeaf *gset = new ssgVTable ( GL_TRIANGLE_FAN,
                     9, scoords,
                     1, snorms,
                     9, tcoords,
                     1, scolors ) ;

  gset -> setState ( gstate ) ;

  return gset ;
}

ssgEntity *MakeBowTie ( ssgState *gstate, float dx, float /*dy*/, float dz )
{
  sgVec4 *scolors = new sgVec4 [  1 ] ;
  sgVec3 *snorms  = new sgVec3 [  1 ] ;
  sgVec3 *scoords = new sgVec3 [  8 ] ;
  sgVec2 *tcoords = new sgVec2 [  8 ] ;

  sgSetVec4 ( scolors [0], 1.0f, 1.0f, 1.0f, 1.0f ) ;
  sgSetVec3 ( snorms  [0], 0.0f, 1.0f, 0.0f ) ;

  sgSetVec3(scoords[ 0],-1.0 * dx, 0.0, -1.0 * dz );
  sgSetVec3(scoords[ 1],-1.0 * dx, 0.0,  1.0 * dz );
  sgSetVec3(scoords[ 2],-0.3 * dx, 0.0, -0.3 * dz );
  sgSetVec3(scoords[ 3],-0.3 * dx, 0.0,  0.3 * dz );
  sgSetVec3(scoords[ 4], 0.3 * dx, 0.0, -0.3 * dz );
  sgSetVec3(scoords[ 5], 0.3 * dx, 0.0,  0.3 * dz );
  sgSetVec3(scoords[ 6], 1.0 * dx, 0.0, -1.0 * dz );
  sgSetVec3(scoords[ 7], 1.0 * dx, 0.0,  1.0 * dz );

  sgSetVec2(tcoords[ 0], x2u (  0 ), y2v ( 64 ) );
  sgSetVec2(tcoords[ 1], x2u (  0 ), y2v ( 30 ) );
  sgSetVec2(tcoords[ 2], x2u ( 27 ), y2v ( 50 ) );
  sgSetVec2(tcoords[ 3], x2u ( 27 ), y2v ( 40 ) );
  sgSetVec2(tcoords[ 4], x2u ( 43 ), y2v ( 54 ) );
  sgSetVec2(tcoords[ 5], x2u ( 43 ), y2v ( 39 ) );
  sgSetVec2(tcoords[ 6], x2u ( 64 ), y2v ( 64 ) );
  sgSetVec2(tcoords[ 7], x2u ( 64 ), y2v ( 29 ) );

  ssgLeaf *gset = new ssgVTable ( GL_TRIANGLE_STRIP,
                     8, scoords,
                     1, snorms,
                     8, tcoords,
                     1, scolors ) ;

  gset -> setState ( gstate ) ;

  return gset ;
}


ssgEntity *MakeShades ( ssgState *gstate, float dx, float dy, float dz )
{
  sgVec4 *scolors = new sgVec4 [  1 ] ;
  sgVec3 *snorms  = new sgVec3 [  1 ] ;
  sgVec3 *scoords = new sgVec3 [ 11 ] ;
  sgVec2 *tcoords = new sgVec2 [ 11 ] ;

  sgSetVec4 ( scolors [0], 1.0f, 1.0f, 1.0f, 1.0f ) ;
  sgSetVec3 ( snorms  [0], 0.0f, 1.0f, 0.0f ) ;

  sgSetVec3(scoords[ 0],-1.0 * dx, 1.0 * dy,  0.0 * dz );
  sgSetVec3(scoords[ 1],-1.0 * dx, 1.0 * dy,  1.0 * dz );
  sgSetVec3(scoords[ 2],-1.0 * dx, 0.0 * dy,  0.0 * dz );
  sgSetVec3(scoords[ 3],-1.0 * dx, 0.0 * dy,  1.0 * dz );
  sgSetVec3(scoords[ 4],-0.5 * dx, 0.0 * dy, -1.0 * dz );
  sgSetVec3(scoords[ 5], 0.0 * dx, 0.0 * dy,  1.0 * dz );
  sgSetVec3(scoords[ 6], 0.5 * dx, 0.0 * dy, -1.0 * dz );
  sgSetVec3(scoords[ 7], 1.0 * dx, 0.0 * dy,  1.0 * dz );
  sgSetVec3(scoords[ 8], 1.0 * dx, 0.0 * dy,  0.0 * dz );
  sgSetVec3(scoords[ 9], 1.0 * dx, 1.0 * dy,  1.0 * dz );
  sgSetVec3(scoords[10], 1.0 * dx, 1.0 * dy,  0.0 * dz );

  sgSetVec2(tcoords[ 0], x2u (  0 ), y2v ( 28 ) );
  sgSetVec2(tcoords[ 1], x2u (  0 ), y2v ( 20 ) );
  sgSetVec2(tcoords[ 2], x2u ( 15 ), y2v ( 28 ) );
  sgSetVec2(tcoords[ 3], x2u ( 15 ), y2v ( 20 ) );
  sgSetVec2(tcoords[ 4], x2u ( 26 ), y2v ( 40 ) );
  sgSetVec2(tcoords[ 5], x2u ( 32 ), y2v ( 20 ) );
  sgSetVec2(tcoords[ 6], x2u ( 42 ), y2v ( 40 ) );
  sgSetVec2(tcoords[ 7], x2u ( 55 ), y2v ( 20 ) );
  sgSetVec2(tcoords[ 8], x2u ( 55 ), y2v ( 28 ) );
  sgSetVec2(tcoords[ 9], x2u ( 64 ), y2v ( 20 ) );
  sgSetVec2(tcoords[10], x2u ( 64 ), y2v ( 28 ) );

  ssgLeaf *gset = new ssgVTable ( GL_TRIANGLE_STRIP,
                    11, scoords,
                     1, snorms,
                    11, tcoords,
                     1, scolors ) ;

  gset -> setState ( gstate ) ;

  return gset ;
}


