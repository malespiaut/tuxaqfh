
/* Texture wrapping mechanisms */

#define COMP_WRAP             1
#define COMP_PROJECT          2
#define COMP_PROJECT_ONE_SIDE 3
#define COMP_PROJECT_TOP      4

/*
  Distances are in meters.
  Lambda is expressed in meters-per-map-repeat in PROJECT mode,
  Number of map repeats per surface in WRAP mode.
*/

ssgEntity *MakeFlukes    ( ssgState *gst, float dx, float dy, float dz, int fluke ) ;
ssgEntity *MakeRocketFin ( ssgState *gst ) ;
ssgEntity *MakeFlipper   ( ssgState *gst, float dx, float dy, float dz ) ;
ssgEntity *MakeCloak     ( ssgState *gst, float dx, float dy, float dz, sgVec3 *vcoord ) ;
ssgVTable *MakeSurfRev   ( ssgState *gst, int npts, float *dx, float *dz, int nsteps,
                                int tex_alg, float lambda_x, float lambda_z ) ;
ssgEntity *MakeXSection  ( ssgState *gstate, int npts,
                                 float dy[], float dz[], float dz_tex[], float radii[], int nsteps,
                                 float umin, float ymin, float uscale, float yscale,
                                 float vmin, float zmin, float vscale, float zscale ) ;

ssgEntity *MakePartyHat  ( ssgState *gst, float dx, float dy, float dz ) ;
ssgEntity *MakeBowTie    ( ssgState *gst, float dx, float dy, float dz ) ;
ssgEntity *MakeShades    ( ssgState *gst, float dx, float dy, float dz ) ;

