
#include "tux.h"

void Orca::update ()
{
  frame++ ;
  last_pos = orca_pos ;
  orca -> setTransform ( & orca_pos ) ;

  int step = frame % 38 ;

  if ( step > 19 )
    orcaSeq -> selectStep ( 38 - step ) ;
  else
    orcaSeq -> selectStep ( step ) ;
}


Orca::Orca ()
{
  orca = new ssgTransform ;
  orca -> ref () ;

  orcaSeq = new ssgTimedSelector ;

  sgCoord c ;

  /* Flipper coordinates */

  ssgBranch *lfgeode    = new ssgBranch () ; lfgeode -> setName ( "Orca_lf" ) ;
  ssgBranch *rfgeode    = new ssgBranch () ; rfgeode -> setName ( "Orca_rf" ) ;
  ssgBranch *flukegeode = new ssgBranch () ; flukegeode -> setName ( "Orca_fluke" ) ;
  ssgBranch *fingeode   = new ssgBranch () ; fingeode -> setName ( "Orca_fin" ) ;

float lmult = 1.3 ;
  lfgeode   -> addKid ( MakeFlipper ( orcafin_gst,  1.0, lmult* 0.77, 1.0 ) ) ;
  rfgeode   -> addKid ( MakeFlipper ( orcafin_gst, -1.0, lmult* 0.77, 1.0 ) ) ;
  flukegeode-> addKid ( MakeFlukes  ( orcafin_gst,  1.3, lmult*-0.77, 1.0, TRUE )) ;
  fingeode  -> addKid ( MakeFlukes  ( orcafin_gst,  1.0, lmult* 1.1, 1.1, FALSE)) ;

                    /* Tail.....................................Nose */
  float orca_y [7] = { -5.13, -4.62, -2.97,  0.00,  1.65,  1.98,  2.20 } ;
  float orca_z [7] = { -0.65, -0.65, -0.05,  0.15, -0.32, -0.48, -0.45 } ;
  float orca_r [7] = {  0.00,  0.27,  1.02,  1.18,  0.68,  0.26,  0.00 } ;

for ( int q = 0 ; q < 7 ; q++ )
  orca_y[q]*=lmult;

  for ( int i = 0 ; i < 20 ; i++ )
  {
    ssgBranch *group = new ssgBranch () ; group->setName ( "Orca Grp #" ) ;
    ssgBranch *geode = new ssgBranch () ; geode->setName ( "Orca Gd  #" ) ;
    float orca_z2 [ 7 ] ;

    memcpy ( orca_z2, orca_z, 7 * sizeof(float) ) ; 

    orca_z2 [ 0 ] += 1.60 * sin ( (float) (i-10) * M_PI / 20.0 ) ;
    orca_z2 [ 1 ] += 1.80 * sin ( (float) (i-10) * M_PI / 20.0 ) ;
    orca_z2 [ 2 ] += 0.60 * sin ( (float) (i-10) * M_PI / 20.0 ) ;
    orca_z2 [ 3 ] += 0.10 * sin ( (float) (i-10) * M_PI / 20.0 ) ;
    orca_z2 [ 4 ] -= 0.06 * sin ( (float) (i-10) * M_PI / 20.0 ) ;
    orca_z2 [ 5 ] -= 0.10 * sin ( (float) (i-10) * M_PI / 20.0 ) ;
    orca_z2 [ 6 ] -= 0.11 * sin ( (float) (i-10) * M_PI / 20.0 ) ;

    geode -> addKid ( MakeXSection ( orca_gst, 7, orca_y,
                                           orca_z2, orca_z, orca_r, 11,
                      25.0/256.0, -4.50, (217.0-25.0)/256.0, (4.95+2.09)*lmult,
                       9.0/128.0, -1.08, ( 47.0- 9.0)/128.0, 1.26+1.08 ) ) ;

    sgSetVec3 ( c.xyz, 0.0, lmult*-4.2, orca_z2[1] ) ;
    sgSetVec3 ( c.hpr, 0.0,  (float)(i-10)*0.8, 0.0 ) ;
    ssgTransform *flukescs = new ssgTransform ( & c ) ;

    sgSetVec3 ( c.xyz, 0.0, 0.0, 1.10 ) ;
    sgSetVec3 ( c.hpr, 0.0, -(float)(i-20), 0.0 ) ;
    ssgTransform *finscs = new ssgTransform ( & c ) ;

    sgSetVec3 ( c.xyz,  1.0, 0.0, -0.2 ) ;
    sgSetVec3 ( c.hpr, 10.0, 0.0, (float)(i-10)*2.0 ) ;
    ssgTransform *lfscs = new ssgTransform ( & c ) ;

    sgSetVec3 ( c.xyz, -1.0, 0.0, -0.2 ) ;
    sgSetVec3 ( c.hpr,-10.0, 0.0, -(float)(i-10)*2.0 ) ;
    ssgTransform *rfscs = new ssgTransform ( & c ) ;

    lfscs   -> addKid ( lfgeode    ) ;
    rfscs   -> addKid ( rfgeode    ) ;

    finscs  -> addKid ( fingeode   ) ;
    flukescs-> addKid ( flukegeode ) ;
    group   -> addKid ( finscs     ) ;
    group   -> addKid ( flukescs   ) ;
    group   -> addKid ( lfscs      ) ;
    group   -> addKid ( rfscs      ) ;
    group   -> addKid ( geode      ) ;
    orcaSeq -> addKid ( group      ) ;
  }

/*
  orcaSeq -> setDuration ( 0.03, -1 ) ;
  orcaSeq -> setLimits   ( SSG_ANIM_SWING, 0, 19 ) ;
*/
  orca -> addKid ( orcaSeq ) ;
}


