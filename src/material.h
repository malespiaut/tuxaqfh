

/* These flags are OR'ed together */

#define TUXMAT_BORING      0
#define TUXMAT_DANGEROUS   1
#define TUXMAT_SPROINGPAD  2
#define TUXMAT_TELEPORTER  4
#define TUXMAT_EXIT_LEVEL  8
#define TUXMAT_VERY_HOT    16
#define TUXMAT_IGNORE      32
#define TUXMAT_LETHAL      64
#define TUXMAT_STARTCLOCK  128
#define TUXMAT_STOPCLOCK   256
#define TUXMAT_COLD        512
#define TUXMAT_OXYGEN      1024

struct Material
{
public:
  ssgSimpleState **gst ;

  char *texture_map  ;
  int   clamp_tex    ;
  int   transparency ;
  float alpha_ref    ;
  int   lighting     ;
  float friction     ;
  unsigned int flags ;

  int  isNull () { return gst == NULL ; } ;
  void install ( int index ) ;
  
  ssgState *getState    () { return *gst ; }
  char     *getTexFname () { return texture_map ; }

  int  isCold       () { return flags & TUXMAT_COLD ; }
  int  isOxygen     () { return flags & TUXMAT_OXYGEN ; }
  int  isIgnore     () { return flags & (TUXMAT_IGNORE|TUXMAT_STARTCLOCK|TUXMAT_STOPCLOCK) ; }
  int  isDangerous  () { return flags & TUXMAT_DANGEROUS  ; }
  int  isVeryHot    () { return flags & TUXMAT_VERY_HOT   ; }
  int  isLethal     () { return flags & TUXMAT_LETHAL     ; }
  int  isSproingPad () { return flags & TUXMAT_SPROINGPAD ; }
  int  isTeleporter () { return flags & TUXMAT_TELEPORTER ; }
  int  isExit       () { return flags & TUXMAT_EXIT_LEVEL ; }
  int  isStartClock () { return flags & TUXMAT_STARTCLOCK ; }
  int  isStopClock  () { return flags & TUXMAT_STOPCLOCK  ; }
} ;


void initMaterials () ;
Material *getMaterial ( ssgState *s ) ;
Material *getMaterial ( ssgLeaf  *l ) ;


extern ssgSimpleState
           *fire_gst   , *herring_gst , *cloak_gst       , *fin_gst     ,
           *wood_gst   , *tuxbeak_gst , *water_gst       , *ice_gst     ,
           *orca_gst   , *orcafin_gst , *party_gst       , *supertx_gst ,
           *tree_gst   , *tuxfoot_gst , *grass_gst       , *sparkle_gst ,
           *tux_gst    , *tuxflip_gst , *fuzzy_gst       , *stripes_gst ,
           *pebbles_gst, *gun_gst     , *fademsk_gst     , *swirl_gst   ,
           *bubble_gst , *snowball_gst, *herringbones_gst, *tinytux_gst ,
           *smoke_gst  , *spark_gst   , *think_gst       , *lava_gst    ,
           *camera_gst , *nocamera_gst, *clearice_gst    ,
           *thermometer_gst, *startline_gst , *finishline_gst,
           *solidblack_gst ;

