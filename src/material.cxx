
#include "tux.h"

ssgSimpleState *fire_gst   , *herring_gst , *cloak_gst     , *fin_gst    ,
               *wood2_gst, *sand_gst, *poster_gst,
               *wood_gst   , *tuxbeak_gst , *water_gst     , *ice_gst    ,
               *orca_gst   , *orcafin_gst , *party_gst     , *supertx_gst,
               *tree_gst   , *tuxfoot_gst , *grass_gst     , *sparkle_gst,
               *tux_gst    , *fuzzy_gst   , *stripes_gst   , *camera_gst,
               *pebbles_gst, *gun_gst     , *fademsk_gst   , *swirl_gst  ,
               *brick_gst  , *brick2_gst  , *xmastree_gst  , *think_gst  ,
               *bubble_gst , *tinytux_gst , *default_gst   , *herringbones_gst,
               *lava_gst   , *spark_gst   , *snowball_gst  , *sproingpad_gst,
               *splash_gst , *smoke_gst   , *clearice_gst  , *waterfall_gst,
               *rock_gst   , *nocamera_gst, *lightning_gst , *blueneon_gst,
               *glass_gst  , *moonrock_gst, *startline_gst , *finishline_gst,
               *snow_gst   , *smoke2_gst  , *thermometer_gst, *oxygen_gst,
               *gown_gst   , *gownbeak_gst, *bow_gst       , *solidblack_gst,
               *cloud_gst  , *kill_water_gst ;

Material gs_setup [] =
{
  /*    gst          texture_map            clamp, trans,aref,light,frctn,flags */
  { &default_gst, "",                       FALSE, FALSE,0.0, TRUE, 1.0, TUXMAT_BORING },

  { &oxygen_gst,  "images/oxygen.rgb",      FALSE, TRUE ,0.2, FALSE, 1.0, TUXMAT_OXYGEN | TUXMAT_IGNORE },
  { &cloud_gst,  "images/clouds.rgb",       FALSE, FALSE,0.2, FALSE, 1.0, TUXMAT_BORING },

/* Penguin Materials. */
  { &    tux_gst, "penguin/Penguin_body.rgb", FALSE, FALSE,0.0, FALSE, 1.0, TUXMAT_IGNORE },
  { &tuxbeak_gst, "penguin/Penguin_beak.rgb", FALSE, FALSE,0.0, FALSE, 1.0, TUXMAT_IGNORE },
  { &   gown_gst, "penguin/Gown_body.rgb",    FALSE, FALSE,0.0, FALSE, 1.0, TUXMAT_IGNORE },
  { &gownbeak_gst,"penguin/Gown_beak.rgb",    FALSE, FALSE,0.0, FALSE, 1.0, TUXMAT_IGNORE },
  { &    bow_gst, "penguin/bow.rgb",          FALSE, FALSE,0.0, FALSE, 1.0, TUXMAT_IGNORE },
  { &supertx_gst, "penguin/SuperTux.rgb",     FALSE, FALSE,0.0, FALSE, 1.0, TUXMAT_IGNORE },
  { &  cloak_gst, "penguin/red_ripple.rgb",   FALSE, FALSE,0.0, FALSE, 1.0, TUXMAT_IGNORE },
  { &  party_gst, "penguin/partygear.rgba",   FALSE, TRUE ,0.0, FALSE, 1.0, TUXMAT_IGNORE },
  { &tuxfoot_gst, "penguin/Penguin_foot.rgba",TRUE , TRUE ,0.6, FALSE, 1.0, TUXMAT_IGNORE },

  { &    gun_gst, "images/gun.rgba",          FALSE, FALSE,0.0, FALSE, 1.0, TUXMAT_BORING },
  { &    ice_gst, "images/ice.rgb",           FALSE, FALSE,0.0, TRUE , 0.4, TUXMAT_COLD },
  { &clearice_gst,"images/ice.rgb",           FALSE, TRUE ,0.2, TRUE , 0.4, TUXMAT_COLD },
  { &lightning_gst,"images/lightning.rgba",   FALSE, TRUE ,0.2, FALSE, 1.0, TUXMAT_LETHAL },
  { &blueneon_gst,"images/blue_neon.rgba",    FALSE, TRUE ,0.2, FALSE, 1.0, TUXMAT_LETHAL },
  { &tinytux_gst, "images/tinytux.rgba",      TRUE , TRUE ,0.2, FALSE, 1.0, TUXMAT_IGNORE },
  { & camera_gst, "images/camera.rgba",       TRUE , TRUE ,0.2, FALSE, 1.0, TUXMAT_IGNORE },
  { &thermometer_gst, "images/thermometer.rgb",TRUE, FALSE,0.0, FALSE, 1.0, TUXMAT_IGNORE },
  { &nocamera_gst,"images/nocamera.rgba",     TRUE , TRUE ,0.2, FALSE, 1.0, TUXMAT_IGNORE },
  { &   fire_gst, "images/flames.rgba",       FALSE, TRUE ,0.2, FALSE, 1.0, TUXMAT_VERY_HOT },
  { &   orca_gst, "images/orca_body.rgb",     TRUE , FALSE,0.0, FALSE, 0.3, TUXMAT_BORING },
  { &   tree_gst, "images/tree.rgba",         FALSE/*TRUE*/ , TRUE ,0.9, FALSE, 1.0, TUXMAT_BORING },
  { &xmastree_gst,"images/xmastree.rgba",     TRUE , TRUE ,0.9, FALSE, 1.0, TUXMAT_BORING },

  /*    gst          texture_map            clamp, trans,aref,light,frctn,flags */
  { &   wood_gst, "images/wood_tile.rgb",     FALSE, FALSE,0.0, TRUE , 1.2, TUXMAT_BORING },
  { &  brick_gst, "images/brick.rgb",         FALSE, FALSE,0.0, TRUE , 1.2, TUXMAT_BORING },
  { & brick2_gst, "images/brick2.rgb",        FALSE, FALSE,0.0, TRUE , 1.2, TUXMAT_BORING },
  { &  fuzzy_gst, "images/fuzzy.inta",        TRUE , TRUE ,0.2, FALSE, 1.0, TUXMAT_IGNORE },
  { &  grass_gst, "images/ground2.rgb",       FALSE, FALSE,0.0, TRUE , 1.0, TUXMAT_BORING },
  { &   rock_gst, "images/rock.rgb",          FALSE, FALSE,0.0, TRUE , 1.0, TUXMAT_BORING },
  { &  water_gst, "images/water2.rgba",       FALSE, TRUE ,0.2, FALSE, 0.3, TUXMAT_BORING },
  { &kill_water_gst, "images/water.rgb",      FALSE, FALSE,0.2, FALSE, 0.3, TUXMAT_LETHAL },
  { &waterfall_gst,"images/waterfall.rgba",   FALSE, TRUE ,0.2, FALSE, 0.3, TUXMAT_IGNORE },
  { &   lava_gst, "images/lava.rgb",          FALSE, FALSE,0.0, FALSE, 0.3, TUXMAT_VERY_HOT },
  { &  swirl_gst, "images/swirl.rgb",         FALSE, FALSE,0.0, FALSE, 1.0, TUXMAT_EXIT_LEVEL },
  { & bubble_gst, "images/bubble.rgba",       TRUE,  TRUE ,0.2, FALSE, 1.0, TUXMAT_IGNORE },
  { &  glass_gst, "images/glass.rgba",        FALSE, TRUE ,0.0, FALSE, 0.02,TUXMAT_BORING },
  { &  smoke_gst, "images/smoke.rgba",        TRUE , TRUE ,0.2, FALSE, 1.0, TUXMAT_IGNORE },
  { & splash_gst, "images/splash.rgba",       TRUE , TRUE ,0.2, FALSE, 1.0, TUXMAT_IGNORE },
  { &  think_gst, "images/think.rgba",        TRUE , TRUE ,0.0, FALSE, 1.0, TUXMAT_IGNORE },
  { &snowball_gst,"images/snowball.rgba",     TRUE , TRUE ,0.7, FALSE, 1.0, TUXMAT_IGNORE },
  { &   snow_gst, "images/snow.rgb",          FALSE, FALSE,0.0, TRUE , 1.0, TUXMAT_COLD   },
  { &herring_gst, "images/herring.inta",      TRUE , TRUE ,0.9, FALSE, 1.0, TUXMAT_IGNORE },
  { &herringbones_gst,
                "images/herringbones.inta",   TRUE , TRUE ,0.2, FALSE, 1.0, TUXMAT_IGNORE },

  /*    gst          texture_map            clamp, trans,aref,light,frctn,flags */
  { &orcafin_gst, "",                         FALSE, FALSE,0.0, FALSE, 1.0, TUXMAT_BORING },
  { &sparkle_gst, "images/sparkle.rgba",      FALSE, TRUE, 0.0, FALSE, 1.0, TUXMAT_TELEPORTER },
  { &  spark_gst, "images/spark.rgba",        FALSE, TRUE, 0.0, FALSE, 1.0, TUXMAT_IGNORE },
  { &stripes_gst, "images/blue_stripe.rgb",   FALSE, FALSE,0.0, TRUE , 1.0, TUXMAT_BORING },
  { &fademsk_gst, "images/tux_text.inta",     TRUE , TRUE ,0.1, FALSE, 1.0, TUXMAT_IGNORE },
  { &pebbles_gst, "images/pebbles.rgb",       FALSE, FALSE,0.0, TRUE , 1.0, TUXMAT_BORING },
  { &sproingpad_gst,"images/sproing.rgb",     FALSE, FALSE,0.0, TRUE , 2.0, TUXMAT_SPROINGPAD },
  { &moonrock_gst,"images/moonscape.rgb",     FALSE, FALSE,0.0, TRUE , 1.0, TUXMAT_SPROINGPAD },
  { &startline_gst ,"images/startline.rgb",   FALSE, FALSE,0.0, TRUE , 2.0, TUXMAT_STARTCLOCK },
  { &finishline_gst,"images/finishline.rgb",  FALSE, FALSE,0.0, TRUE , 2.0, TUXMAT_STOPCLOCK },

  { &solidblack_gst, ""                    ,  FALSE, FALSE,0.0, FALSE , 1.0, TUXMAT_BORING },
  { &  wood2_gst, "images/wood.rgb",     FALSE, FALSE,0.0, TRUE , 1.2, TUXMAT_BORING },
  { & poster_gst, "images/poster.rgb",     FALSE, FALSE,0.0, TRUE , 1.2, TUXMAT_BORING },
  { & sand_gst, "images/sand.rgb",     FALSE, FALSE,0.0, TRUE , 1.2, TUXMAT_BORING },
  { NULL, "", FALSE, FALSE, 0.0, FALSE, 1.0, TUXMAT_BORING }
} ;


void Material::install ( int index )
{
  *gst = new ssgSimpleState ;

  (*gst) -> ref () ;
  (*gst) -> setExternalPropertyIndex ( index ) ;

  if ( texture_map [ 0 ] != '\0' )
  {
    (*gst) -> setTexture ( texture_map, !clamp_tex, !clamp_tex ) ;
    (*gst) -> enable ( GL_TEXTURE_2D ) ;
  }
  else
    (*gst) -> disable ( GL_TEXTURE_2D ) ;

  if ( lighting )
    (*gst) -> enable ( GL_LIGHTING ) ;
  else
    (*gst) -> disable ( GL_LIGHTING ) ;

  (*gst) -> setShadeModel ( GL_SMOOTH ) ;
  (*gst) -> enable ( GL_COLOR_MATERIAL ) ;
  (*gst) -> enable ( GL_CULL_FACE      ) ;
  (*gst) -> setColourMaterial ( GL_AMBIENT_AND_DIFFUSE ) ;
  (*gst) -> setMaterial ( GL_EMISSION, 0, 0, 0, 1 ) ;
  (*gst) -> setMaterial ( GL_SPECULAR, 1, 1, 1, 1 ) ;
  (*gst) -> setShininess ( 0 ) ;

  if ( transparency )
  {
    (*gst) -> setTranslucent () ;
    (*gst) -> enable ( GL_ALPHA_TEST ) ;
    (*gst) -> setAlphaClamp ( alpha_ref ) ;
    (*gst) -> enable ( GL_BLEND ) ;
  }
  else
  {
    (*gst) -> setOpaque () ;
    (*gst) -> disable ( GL_BLEND ) ;
  }
}



ssgState *getAppState ( char *fname )
{
  if ( fname == NULL || fname[0] == '\0' )
    return gs_setup [ 0 ] . getState() ;

  char *fn ;

  /* Remove all leading path information. */

  for ( fn = & fname [ strlen ( fname ) - 1 ] ; fn != fname &&
                                               *fn != '/' ; fn-- )
    /* Search back for a '/' */ ;

  if ( *fn == '/' )
    fn++ ;

  char basename [ 1024 ] ;

  strcpy ( basename, fn ) ;

  /* Remove last trailing extension. */

  for ( fn = & basename [ strlen ( basename ) - 1 ] ; fn != basename &&
                                                     *fn != '.' ; fn-- )
    /* Search back for a '.' */ ;

  if ( *fn == '.' )
    *fn = '\0' ;

  for ( int i = 0 ; ! gs_setup [ i ] . isNull () ; i++ )
  {
    char *fname2 = gs_setup [ i ] . getTexFname () ;

    if ( fname2 != NULL && fname2[0] != '\0' )
    {
      char *fn2 ;

      /* Remove all leading path information. */

      for ( fn2 = & fname2 [ strlen ( fname2 ) -1 ] ; fn2 != fname2 &&
                                                     *fn2 != '/' ; fn2-- )
        /* Search back for a '/' */ ;

      if ( *fn2 == '/' )
        fn2++ ;

      char basename2 [ 1024 ] ;

      strcpy ( basename2, fn2 ) ;

      /* Remove last trailing extension. */

      for ( fn2 = & basename2 [ strlen ( basename2 ) - 1 ] ; fn2 != basename2 &&
                                                         *fn2 != '.' ; fn2-- )
        /* Search back for a '.' */ ;

      if ( *fn2 == '.' )
        *fn2 = '\0' ;

      if ( strcmp ( basename, basename2 ) == 0 )
        return gs_setup [ i ] . getState() ;
    }
  }

  return NULL ;
}



void initMaterials ()
{
  for ( int i = 0 ; ! gs_setup [ i ] . isNull () ; i++ )
    gs_setup [ i ] . install ( i ) ;

  ssgSetAppStateCallback ( getAppState ) ;
}


Material *getMaterial ( ssgState *s )
{
  return & ( gs_setup [ s -> getExternalPropertyIndex () ] ) ;
}


Material *getMaterial ( ssgLeaf *l )
{
  return getMaterial ( l -> getState () ) ;
}

