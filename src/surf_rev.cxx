
#include "tux.h"

ssgVTable *MakeSurfRev ( ssgState *gstate, int npts,
                          float dx[], float dz[], int nsteps,
                          int tex_alg, float lambda_x, float lambda_z )
{
  int num = (npts-1) * (nsteps+1) * 2 ;

  sgVec3 *vcoord = new sgVec3 [ num ] ;
  sgVec3 *ncoord = new sgVec3 [ num ] ;
  sgVec2 *tcoord = new sgVec2 [ num ] ;
  
  int next = 0 ;

  for ( int j = 0 ; j < npts-1 ; j++ )
    for ( int i = 0 ; i <= nsteps ; i++ )
    {
      float a = (float)((i==nsteps) ? 0 : i ) * 2.0 * M_PI / nsteps ;
      float x1 = dx [ j ] * cos ( a ) ;
      float y1 = dx [ j ] * sin ( a ) ;
      float z1 = dz [ j ] ;
      float x2 = dx [j+1] * cos ( a ) ;
      float y2 = dx [j+1] * sin ( a ) ;
      float z2 = dz [j+1] ;

      float nx1 = cos ( a ) ;
      float ny1 = sin ( a ) ;
      float nz1 = 0.0 ;
      float nx2 = cos ( a ) ;
      float ny2 = sin ( a ) ;
      float nz2 = 0.0 ;

      float tx1, ty1, tx2, ty2 ;

      switch ( tex_alg )
      {
        case COMP_WRAP :
          tx1 = lambda_x * (float) i / (float) nsteps ;
          ty1 = lambda_z * (float) j / (float)( npts - 1 ) ;
          tx2 = lambda_x * (float) i / (float) nsteps ;
          ty2 = lambda_z * (float)(j+1) / (float)( npts - 1 ) ;
          break ;

        case COMP_PROJECT :
          tx1 = x1 / lambda_x - 0.5 ;
          ty1 = z1 / lambda_z ;
          tx2 = x2 / lambda_x - 0.5 ;
          ty2 = z2 / lambda_z ;
          break ;

        case COMP_PROJECT_TOP :
          tx1 = x1 / lambda_x - 0.5 ;
          ty1 = y1 / lambda_z - 0.5 ;
          tx2 = x2 / lambda_x - 0.5 ;
          ty2 = y2 / lambda_z - 0.5 ;
          break ;

        case COMP_PROJECT_ONE_SIDE :
        default :
	  tx1 = x1 / lambda_x - 0.5 ;
	  ty1 = z1 / lambda_z ;
	  tx2 = x2 / lambda_x - 0.5 ;
	  ty2 = z2 / lambda_z ;

          if ( i <= nsteps / 2 )
          {
	    tx1 = -1.0 ;
	    tx2 = -1.0 ;
          }
          break ;
      }

      sgSetVec3 ( vcoord [ next ],  x1,  y1,  z1 ) ;
      sgSetVec3 ( ncoord [ next ], nx1, ny1, nz1 ) ;
      sgSetVec2 ( tcoord [ next ], tx1, ty1 ) ;
      next++ ;
      sgSetVec3 ( vcoord [ next ],  x2,  y2,  z2 ) ;
      sgSetVec3 ( ncoord [ next ], nx2, ny2, nz2 ) ;
      sgSetVec2 ( tcoord [ next ], tx2, ty2 ) ;
      next++ ;
    }

  ssgVTable *gset = new ssgVTable ( GL_TRIANGLE_STRIP,
         num, vcoord,
         num, ncoord,
         num, tcoord,
           0, NULL ) ;

  gset -> setState ( gstate ) ;
  return gset ;
}


ssgEntity *MakeXSection ( ssgState *gstate, int npts,
                         float dy[], float dz[], float dz_tex[], float radii[], int nsteps,
                         float umin, float ymin, float uscale, float yscale,
                         float vmin, float zmin, float vscale, float zscale ) 
{
  int num = (npts-1) * (nsteps+1) * 2 ;

  sgVec3  *vcoord = new sgVec3 [ num ] ;
  sgVec3  *ncoord = new sgVec3 [ num ] ;
  sgVec2  *tcoord = new sgVec2 [ num ] ;

  int next = 0 ;

  for ( int j = 0 ; j < npts-1 ; j++ )
    for ( int i = 0 ; i <= nsteps ; i++ )
    {
      float a = (float)((i==nsteps) ? 0 : i ) * 2.0 * M_PI / nsteps ;

      float x1  =            radii [ j ] * cos ( a ) ;
      float z1  = dz [ j ] + radii [ j ] * sin ( a ) ;
      float z1t = dz_tex [ j ] + radii [ j ] * sin ( a ) ;
      float y1  = dy [ j ] ;
      float x2  =            radii [j+1] * cos ( a ) ;
      float z2  = dz [j+1] + radii [j+1] * sin ( a ) ;
      float z2t = dz_tex [j+1] + radii [j+1] * sin ( a ) ;
      float y2  = dy [j+1] ;

      float nx1 = cos ( a ) ;
      float nz1 = sin ( a ) ;
      float ny1 = 0.0 ;
      float nx2 = cos ( a ) ;
      float nz2 = sin ( a ) ;
      float ny2 = 0.0 ;

      float tu1, tv1, tu2, tv2 ;

      tu1 = (( y1  - ymin ) / yscale ) * uscale + umin ;
      tv1 = (( z1t - zmin ) / zscale ) * vscale + vmin ;
      tu2 = (( y2  - ymin ) / yscale ) * uscale + umin ;
      tv2 = (( z2t - zmin ) / zscale ) * vscale + vmin ;

      sgSetVec3 ( vcoord [ next ],  x1,  y1,  z1 ) ;
      sgSetVec3 ( ncoord [ next ], nx1, ny1, nz1 ) ;
      sgSetVec2 ( tcoord [ next ], tu1, tv1 ) ;
      next++ ;
      sgSetVec3 ( vcoord [ next ],  x2,  y2,  z2 ) ;
      sgSetVec3 ( ncoord [ next ], nx2, ny2, nz2 ) ;
      sgSetVec2 ( tcoord [ next ], tu2, tv2 ) ;
      next++ ;
    }

  ssgVTable *gset = new ssgVTable ( GL_TRIANGLE_STRIP,
           num, vcoord,
           num, ncoord,
           num, tcoord,
             0, NULL ) ;

  gset -> setState ( gstate ) ;
  return gset ;
}


