#include "tux.h"

Rocket *rocket = NULL ;

void Penguin::update ( sgCoord *coord,
                       float head_direction,
                       float roll_amount,
                       float squish_height,
                       int flap,
                       int jump,
                       int slide,
                       int swim,
                       int rocket_on,
                       int rocket_gun,
                       int rocket_exhaust,
                       int space_gear,
                       int cloak_on,
                       int hat_on,
                       int tie_on,
                       int shades_on,
                       int dizzy_on )
{
  peng_pos = *coord ;

  if ( roll_amount > 0.0f )
  {
    peng_pos . hpr [ 1 ] += roll_amount ;
    peng_pos . xyz [ 2 ] += 1.2 * (1.0f + cos ( (roll_amount-180.0f)*SG_DEGREES_TO_RADIANS )) ;
  }

  frame++ ;

  static int blinktime = 60 ;
  int blink = frame % blinktime ;

  if ( blink == 0 || blink == 2 )
    blinker -> selectStep ( 1 ) ;
  else
  if ( blink == 1 )
  {
    blinktime = 20 + rand () % 100 ;
    blinker -> selectStep ( 2 ) ;
  }
  else
    blinker -> selectStep ( 0 ) ;


  int moved = ( peng_pos.xyz [ 0 ] != last_pos.xyz [ 0 ] ||
                peng_pos.xyz [ 1 ] != last_pos.xyz [ 1 ] ) ;


  if ( slide && !cloak_on && !rocket_on && !rocket_gun && !rocket_exhaust )
  {
    slider -> selectStep ( 1 ) ;
    slide = TRUE ;
    jump = FALSE ;
  }
  else
  {
    slider -> selectStep ( 0 ) ;
    slide = FALSE ;
  }

  rocket  -> update ( rocket_on, rocket_gun, rocket_exhaust ) ;
  cloak   -> select ( cloak_on  ? 0xFFFFFFFF : 0x00000000 ) ;
  partyhat-> select ( hat_on    ? 0xFFFFFFFF : 0x00000000 ) ;
  bowtie  -> select ( tie_on    ? 0xFFFFFFFF : 0x00000000 ) ;
  shades  -> select ( shades_on ? 0xFFFFFFFF : 0x00000000 ) ;
  dizzy   -> select ( dizzy_on  ? 0xFFFFFFFF : 0x00000000 ) ;
  space_helmet -> select ( space_gear ? 0xFFFFFFFF : 0x00000000 ) ;

  if ( cloak_on )
  {
    static int flutter0 = 0 ;
    static int flutter1 = 0 ;
    static int flutter2 = 0 ;
    static int flutter_dir0 = 1 ;
    static int flutter_dir1 = 1 ;
    static int flutter_dir2 = 1 ;

    if ( ! moved )
    {
      flutter0 -- ;
      flutter1 -- ;
      flutter2 -- ;
    }
    else
    {
      flutter0 += flutter_dir0 ;
      flutter1 += flutter_dir1 ;
      flutter2 += flutter_dir2 ;
    }

    if ( flutter0 < 0 ) flutter0 = 0 ;
    if ( flutter0 > 30 && flutter_dir0 > 0 ) flutter_dir0 = -1 ; 
    if ( flutter0 < 20 && flutter_dir0 < 0 ) flutter_dir0 =  1 ; 
    if ( flutter1 < 0 ) flutter1 = 0 ;
    if ( flutter1 > 34 && flutter_dir1 > 0 ) flutter_dir1 = -1 ; 
    if ( flutter1 < 20 && flutter_dir1 < 0 ) flutter_dir1 =  1 ; 
    if ( flutter2 < 0 ) flutter2 = 0 ;
    if ( flutter2 > 31 && flutter_dir2 > 0 ) flutter_dir2 = -1 ; 
    if ( flutter2 < 20 && flutter_dir2 < 0 ) flutter_dir2 =  1 ; 

    memcpy ( cloak_verts, safe_cloak_verts, 8 * sizeof(sgVec3) ) ;

    cloak_verts [ 3 ][ 2 ] += (float)flutter0/30.0f ;
    cloak_verts [ 2 ][ 2 ] += (float)flutter2/60.0f ;
    cloak_verts [ 4 ][ 2 ] += (float)flutter1/30.0f ;
    cloak_verts [ 5 ][ 2 ] += (float)flutter2/30.0f ;
    cloak_verts [ 6 ][ 2 ] += (float)flutter0/60.0f ;

    cloak_verts [ 3 ][ 1 ] += (float)flutter0/60.0f ;
    cloak_verts [ 4 ][ 1 ] += (float)flutter1/60.0f ;
    cloak_verts [ 5 ][ 1 ] += (float)flutter2/60.0f ;

    supertux  -> selectStep ( 1 ) ;
  }
  else
    supertux  -> selectStep ( 0 ) ;

  beak -> selectStep ( 0 ) ;

  int f = swim ? (flap?frame:(frame/2)) :
                 (flap?(frame*2):frame) ;
  f = f % 18 ;
  if ( f > 9 ) f = 18 - f ;

  sgSetVec3 ( lflip_pos.xyz, 0, 0, 0 ) ;
  sgSetVec3 ( rflip_pos.xyz, 0, 0, 0 ) ;
  
  sgSetVec3 ( lflip_pos.hpr, 0, 0,  ( f - 3 ) * (flap ? 6.0 : 3.0 )) ;
  sgSetVec3 ( rflip_pos.hpr, 0, 0, -( f - 3 ) * (flap ? 6.0 : 3.0 )) ;

  if ( swim & flap )
  { 
    int l = frame % 20 ; if ( l > 10 ) l = 20 - l ;
    sgSetVec3 ( lfoot_pos . xyz, 0, -(float) l / 16.0f, 0.1 + (float) l / 64.0f ) ;
    sgSetVec3 ( rfoot_pos . xyz, 0, -(float) l / 16.0f, 0.1 + (float) l / 64.0f ) ;
    sgSetVec3 ( lfoot_pos . hpr, 0, 120.0f-(float) l * 10.0f, 0 ) ;
    sgSetVec3 ( rfoot_pos . hpr, 0, 120.0f-(float) l * 10.0f, 0 ) ;
    last_moved_frame = frame ;
  }
  else
  if ( swim )
  { 
    int r =  frame     % 20 ; if ( r > 10 ) r = 20 - r ;
    int l = (frame+10) % 20 ; if ( l > 10 ) l = 20 - l ;
    sgSetVec3 ( lfoot_pos . xyz, 0, -(float) l / 16.0f, 0.1 + (float) l / 64.0f ) ;
    sgSetVec3 ( rfoot_pos . xyz, 0, -(float) r / 16.0f, 0.1 + (float) r / 64.0f ) ;
    sgSetVec3 ( lfoot_pos . hpr, 0, 90.0f-(float) l * 5.0f, 0 ) ;
    sgSetVec3 ( rfoot_pos . hpr, 0, 90.0f-(float) r * 5.0f, 0 ) ;
    last_moved_frame = frame ;
  }
  else
  if ( slide )
  {
    int r =  frame    % 10 ; if ( r > 5 ) r = 10 - r ;
    int l = (frame+5) % 10 ; if ( l > 5 ) l = 10 - l ;
    sgSetVec3 ( lfoot_pos . xyz, 0.4, 1.2f, 0.8f + (float) l / 32.0f ) ;
    sgSetVec3 ( rfoot_pos . xyz,-0.4, 1.2f, 0.8f + (float) r / 32.0f ) ;
    sgSetVec3 ( lfoot_pos . hpr, 0, 180.0f - (float) l * 9.0f, 0 ) ;
    sgSetVec3 ( rfoot_pos . hpr, 0, 180.0f - (float) r * 9.0f, 0 ) ;
    last_moved_frame = frame ;
  }
  else
  if ( jump )
  {
    int r = frame % 10 ; if ( r > 5 ) r = 10 - r ;
    int l = frame % 10 ; if ( l > 5 ) l = 10 - l ;
    sgSetVec3 ( lfoot_pos . xyz, 0, -(float) l / 8.0f, (float) l / 32.0f ) ;
    sgSetVec3 ( rfoot_pos . xyz, 0, -(float) r / 8.0f, (float) r / 32.0f ) ;
    sgSetVec3 ( lfoot_pos . hpr, 0, 30.0 -(float) l * 9.0f, 0 ) ;
    sgSetVec3 ( rfoot_pos . hpr, 0, 30.0 -(float) r * 9.0f, 0 ) ;
    last_moved_frame = frame ;
  }
  else
  if ( peng_pos.xyz [ 0 ] != last_pos.xyz [ 0 ] ||
       peng_pos.xyz [ 1 ] != last_pos.xyz [ 1 ] )
  { 
    int r =  frame    % 10 ; if ( r > 5 ) r = 10 - r ;
    int l = (frame+5) % 10 ; if ( l > 5 ) l = 10 - l ;
    sgSetVec3 ( lfoot_pos . xyz, 0, -(float) l / 8.0f, (float) l / 32.0f ) ;
    sgSetVec3 ( rfoot_pos . xyz, 0, -(float) r / 8.0f, (float) r / 32.0f ) ;
    sgSetVec3 ( lfoot_pos . hpr, 0, -(float) l * 9.0f, 0 ) ;
    sgSetVec3 ( rfoot_pos . hpr, 0, -(float) r * 9.0f, 0 ) ;
    last_moved_frame = frame ;
  }
  else
  {
    /* Not moving. */

    static int foot_pattern = 0 ;
    float lfoot_direction = 0.0f ;
    float rfoot_direction = 0.0f ;

    if ( last_moved_frame + 100 < frame )
    {
      int delay = ((frame-100) - last_moved_frame) ;
      int bdelay = delay % 272 ;
      int fdelay = delay % 121 ;
      int hdelay = delay % 421 ;

      if ( bdelay < 5 )
	beak -> selectStep ( 1 ) ;
      else
      if ( bdelay < 30 )
	beak -> selectStep ( 2 ) ;
      else
      if ( bdelay < 35 )
	beak -> selectStep ( 1 ) ;
      else
	beak -> selectStep ( 0 ) ;

      if ( hdelay >= 0 && hdelay < 20 )
	head_direction += hdelay ;
      else
      if ( hdelay >= 20 && hdelay < 40 )
	head_direction += 20 ;
      else
      if ( hdelay >= 40 && hdelay < 60 )
	head_direction += 60 - hdelay ;
      else
      if ( hdelay >= 100 && hdelay < 120 )
	head_direction -= hdelay - 100 ;
      else
      if ( hdelay >= 120 && hdelay < 150 )
	head_direction -= 20 ;
      else
      if ( hdelay >= 150 && hdelay < 170 )
	head_direction -= 170 - hdelay ;

      lfoot_direction = rfoot_direction = 0.0f ;

      if ( fdelay == 120 )
      {
	foot_pattern++ ;
        foot_pattern = foot_pattern % 5 ;
      }

      switch ( foot_pattern )
      {
	case 0 :
	  {
	    int ff = fdelay % 20 ;

	    if ( ff >= 0 && ff < 10 )
	       lfoot_direction = rfoot_direction = (float) ff * 9.0 ;
	    else
	    if ( ff >= 10 && ff < 20 )
	       lfoot_direction = rfoot_direction = (float)(20 - ff) * 9.0f ;
	  }
	  break ;

	case 1 :
          break ;

	case 2 :
	  {
	    int ff = (fdelay-10) % 20 ;

	    if ( fdelay < 10 )
	      lfoot_direction = (float) fdelay * 9.0 ;
	    else
	    if ( fdelay > 110 )
	      lfoot_direction = (float)(120 - fdelay) * 9.0f ;
	    else
	    if ( ff < 10 )
	    {
	      lfoot_direction = (float)(10 - ff) * 9.0f ;
	      rfoot_direction = (float)   ff     * 9.0f ;
	    }
	    else
	    if ( ff >= 10 && ff < 20 )
	    {
	      rfoot_direction = (float)(20 - ff) * 9.0f ;
	      lfoot_direction = (float)(ff - 10) * 9.0f ;
	    }
	  }
	  break ;

	case 3 :
	  {
	    int ff = fdelay ;

	    if ( ff >= 0 && ff < 40 )
	       lfoot_direction = rfoot_direction = (float) ff * 2.2 ;
	    else
	    if ( ff >= 40 && ff < 80 )
	       lfoot_direction = rfoot_direction = (float)(80 - ff) * 2.2f ;
	  }
	  break ;

	case 4 :
        default :
	  break ;
      }
    }

    sgSetVec3 ( lfoot_pos . xyz, 0, -0.75f, 0.2f ) ;
    sgSetVec3 ( rfoot_pos . xyz, 0, -0.75f, 0.2f ) ;

    sgSetVec3 ( lfoot_pos . hpr, -60.0f + lfoot_direction, -70.0f, -60.0f + lfoot_direction ) ;
    sgSetVec3 ( rfoot_pos . hpr,  40.0f - rfoot_direction, -70.0f,  40.0f - rfoot_direction ) ;
  }

  sgCoord c ;
  sgSetCoord ( &c, 0, 0, 0, head_direction, 0, 0 ) ;
  head_dcs -> setTransform ( &c ) ;

  last_pos = peng_pos ;

  lflip  -> setTransform ( & lflip_pos ) ;
  rflip  -> setTransform ( & rflip_pos ) ;
  lfoot  -> setTransform ( & lfoot_pos ) ;
  rfoot  -> setTransform ( & rfoot_pos ) ;

  if ( squish_height < 1.0f )
  {
    float bloat = 2.0f - squish_height ;
    penguin-> setTransform ( &  peng_pos, bloat, bloat, squish_height ) ;
  }
  else
    penguin-> setTransform ( &  peng_pos ) ;
}


sgCoord peng_180 = { { 0.0f, 0.0f, 0.0f }, { 180.0f, 0.0f, 0.0f } } ;

#define flop(x) /*ssgFlatten(x);*//*ssgStripify(x)*/
Penguin::Penguin ( int be_gown )
{
  gown = be_gown ;

  ssgModelPath   ( "penguin" ) ;
  ssgTexturePath ( "penguin" ) ;

  penguin = new ssgTransform () ;
  penguin -> clrTraversalMaskBits ( SSGTRAV_ISECT|SSGTRAV_HOT ) ;
  penguin -> ref () ;

  ssgTransform *scs = new ssgTransform ( & peng_180 ) ;
  penguin -> addKid ( scs ) ;

  ssgEntity *geodeP , *geodeS , *geodePS ;
  ssgEntity *geodeH0, *geodeH1, *geodeH2 ;
  ssgEntity *geodeDz, *geodeSH, *geodeB, *geodeBh, *geodeBo ;
  ssgEntity *lfgeode, *rfgeode, *lgeode, *rgeode  ;

/*  geodeGP = ssgLoadAC ( "gold_penguin.ac", process_userdata ) ;*/

  if ( gown )
  {
    geodeP  = ssgLoadAC ( "gown_body.ac", process_userdata ) ;
    geodeS  = ssgLoadAC ( "supergown_body.ac", process_userdata ) ;
    geodePS = ssgLoadAC ( "gown_slide.ac", process_userdata ) ;

    geodeH0 = ssgLoadAC ( "gown_head.ac"     , process_userdata ) ;
    geodeH1 = ssgLoadAC ( "gown_halfblink.ac", process_userdata ) ;
    geodeH2 = ssgLoadAC ( "gown_blink.ac"    , process_userdata ) ;
    geodeDz = ssgLoadAC ( "gown_dizzy.ac"    , process_userdata ) ;
    geodeSH = ssgLoadAC ( "gown_bubble.ac"   , process_userdata ) ;

    geodeB  = ssgLoadAC ( "gown_beak.ac", process_userdata ) ;
    geodeBh = ssgLoadAC ( "gown_beak_halfopen.ac", process_userdata ) ;
    geodeBo = ssgLoadAC ( "gown_beak_open.ac", process_userdata ) ;

    lfgeode = ssgLoadAC ( "gown_lflipper.ac", process_userdata ) ;
    rfgeode = ssgLoadAC ( "gown_rflipper.ac", process_userdata ) ;
    lgeode  = ssgLoadAC ( "gown_lfoot.ac", process_userdata ) ;
    rgeode  = ssgLoadAC ( "gown_rfoot.ac", process_userdata ) ;
  }
  else
  {
    geodeP  = ssgLoadAC ( "penguin_body.ac", process_userdata ) ;
/*  geodeGP = ssgLoadAC ( "gold_penguin.ac", process_userdata ) ;*/
    geodeS  = ssgLoadAC ( "superpenguin_body.ac", process_userdata ) ;
    geodePS = ssgLoadAC ( "penguin_slide.ac", process_userdata ) ;

    geodeH0 = ssgLoadAC ( "penguin_head.ac"     , process_userdata ) ;
    geodeH1 = ssgLoadAC ( "penguin_halfblink.ac", process_userdata ) ;
    geodeH2 = ssgLoadAC ( "penguin_blink.ac"    , process_userdata ) ;
    geodeDz = ssgLoadAC ( "penguin_dizzy.ac"    , process_userdata ) ;
    geodeSH = ssgLoadAC ( "penguin_bubble.ac"   , process_userdata ) ;

    geodeB  = ssgLoadAC ( "penguin_beak.ac", process_userdata ) ;
    geodeBh = ssgLoadAC ( "penguin_beak_halfopen.ac", process_userdata ) ;
    geodeBo = ssgLoadAC ( "penguin_beak_open.ac", process_userdata ) ;

    lfgeode = ssgLoadAC ( "penguin_lflipper.ac", process_userdata ) ;
    rfgeode = ssgLoadAC ( "penguin_rflipper.ac", process_userdata ) ;
    lgeode  = ssgLoadAC ( "penguin_lfoot.ac", process_userdata ) ;
    rgeode  = ssgLoadAC ( "penguin_rfoot.ac", process_userdata ) ;
  }

  beak = new ssgSelector  ;
  beak -> addKid ( geodeB  ) ; flop ( geodeB  ) ;
  beak -> addKid ( geodeBh ) ; flop ( geodeBh ) ;
  beak -> addKid ( geodeBo ) ; flop ( geodeBo ) ;
  beak -> selectStep ( 0 ) ;

  blinker  = new ssgSelector  ;
  blinker -> addKid ( geodeH0 ) ;/* flop ( geodeH0 ) */;
  blinker -> addKid ( geodeH1 ) ;/* flop ( geodeH1 ) */;
  blinker -> addKid ( geodeH2 ) ;/* flop ( geodeH2 ) */;
  blinker -> selectStep ( 0 ) ;

  dizzy = new ssgSelector ;
  dizzy -> addKid ( geodeDz ) ; flop ( geodeDz ) ;
  dizzy -> select ( 0 ) ;

  space_helmet = new ssgSelector ;
  space_helmet -> addKid ( geodeSH ) ; flop ( geodeSH ) ;
  space_helmet -> select ( 0 ) ;

  head_dcs = new ssgTransform ;
  head_dcs -> addKid ( blinker ) ;
  head_dcs -> addKid ( beak    ) ;
  head_dcs -> addKid ( dizzy   ) ;
  head_dcs -> addKid ( space_helmet ) ;

  supertux  = new ssgSelector () ;
  supertux  -> addKid ( geodeP ) ; flop ( geodeP ) ;
  supertux  -> addKid ( geodeS ) ; flop ( geodeS ) ;
  supertux  -> selectStep ( 0 ) ;

  ssgBranch *body_and_head = new ssgBranch ;
  body_and_head -> addKid ( head_dcs ) ;
/*body_and_head -> addKid ( geodeGP  ) ;*/
  body_and_head -> addKid ( supertux ) ;

  slider = new ssgSelector () ;
  slider -> addKid ( body_and_head  ) ;
  slider -> addKid ( geodePS   ) ; flop ( geodePS ) ;
  slider -> selectStep ( 0 ) ;

  lflip = new ssgTransform () ;
  rflip = new ssgTransform () ;
  lfoot = new ssgTransform () ;
  rfoot = new ssgTransform () ;

  cloak    = new ssgSelector () ;
  partyhat = new ssgSelector () ;
  bowtie   = new ssgSelector () ;
  shades   = new ssgSelector () ;

  sgCoord c ;

  sgSetVec3 ( c.xyz,  0.6, 0.0, 1.2 ) ;
  sgSetVec3 ( c.hpr, 10.0, 0.0, 0.0 ) ;
  ssgTransform *lfscs = new ssgTransform ( & c ) ;
  sgSetVec3 ( c.xyz, -0.6, 0.0, 1.2 ) ;
  sgSetVec3 ( c.hpr,-10.0, 0.0, 0.0 ) ;
  ssgTransform *rfscs = new ssgTransform ( & c ) ;

  sgSetVec3 ( c.xyz, -0.2, 0.0, 0.0 ) ;
  sgSetVec3 ( c.hpr,-15.0, 0.0, 0.0 ) ;
  ssgTransform *lscs = new ssgTransform ( & c ) ;
  sgSetVec3 ( c.xyz,  0.2, 0.0, 0.0 ) ;
  sgSetVec3 ( c.hpr, 15.0, 0.0, 0.0 ) ;
  ssgTransform *rscs = new ssgTransform ( & c ) ;

  sgSetVec3 ( c.hpr,  0.0, 0.0, 0.0 ) ;

  sgSetVec3 ( c.xyz,  0.0, 0.2, 1.8 ) ; ssgTransform *cscs    = new ssgTransform ( & c ) ;

  sgSetVec3 ( c.xyz,  0.0, 0.0, 2.35) ; ssgTransform *hatscs  = new ssgTransform ( & c ) ;
  sgSetVec3 ( c.xyz,  0.0,-0.5, 1.55) ; ssgTransform *tiescs  = new ssgTransform ( & c ) ;
  sgSetVec3 ( c.xyz,  0.0,-0.5, 2.0 ) ; ssgTransform *shdscs  = new ssgTransform ( & c ) ;


  ssgBranch *cgeode    = new ssgBranch () ;  cgeode->setName("Peng c") ;
  ssgBranch *hatgeode  = new ssgBranch () ; hatgeode->setName("Peng hat") ;
  ssgBranch *tiegeode  = new ssgBranch () ; tiegeode->setName("Peng tie") ;
  ssgBranch *shdgeode  = new ssgBranch () ; shdgeode->setName("Peng shd") ;

  cloak_verts      = new sgVec3 [ 8 ] ;
  safe_cloak_verts = new sgVec3 [ 8 ] ;

  cgeode -> addKid ( MakeCloak ( cloak_gst,
                          1.0, -0.8, 1.5, cloak_verts ) ) ;
  memcpy ( safe_cloak_verts, cloak_verts, 8 * sizeof(sgVec3) ) ;

  hatgeode  -> addKid ( MakePartyHat ( party_gst, 0.3, 0.3, 1.0 ) ) ;
  tiegeode  -> addKid ( MakeBowTie   ( party_gst,-0.3, 0.0, 0.2 ) ) ;
  shdgeode  -> addKid ( MakeShades   ( party_gst,-0.4, 0.6, 0.2 ) ) ;

  lfscs -> addKid ( lflip ) ;
  rfscs -> addKid ( rflip ) ;

  lscs -> addKid ( lfoot ) ;
  rscs -> addKid ( rfoot ) ;

  cscs -> addKid ( cloak ) ;

  hatscs -> addKid ( partyhat ) ;
  tiescs -> addKid ( bowtie   ) ;
  shdscs -> addKid ( shades   ) ;

  lflip -> addKid ( lfgeode ) ; flop ( lfgeode ) ;
  rflip -> addKid ( rfgeode ) ; flop ( rfgeode ) ;

  lfoot -> addKid ( lgeode ) ; flop ( lgeode ) ;
  rfoot -> addKid ( rgeode ) ; flop ( rgeode ) ;

  cloak -> addKid ( cgeode ) ;

  partyhat -> addKid ( hatgeode  ) ;
  bowtie   -> addKid ( tiegeode  ) ;
  shades   -> addKid ( shdgeode  ) ;

  scs -> addKid ( slider ) ;

  rocket = new Rocket () ; 
  scs -> addKid ( rocket -> getRoot () ) ;

  scs -> addKid ( lfscs   ) ;
  scs -> addKid ( rfscs   ) ;
  scs -> addKid ( lscs    ) ;
  scs -> addKid ( rscs    ) ;
  scs -> addKid ( cscs    ) ;
  scs -> addKid ( hatscs  ) ;
  scs -> addKid ( tiescs  ) ;
  scs -> addKid ( shdscs  ) ;
  scs -> recalcBSphere () ;

  last_moved_frame = frame = 0 ;
}


