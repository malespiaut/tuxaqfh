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
#include <plib/ssgKeyFlier.h>
/* #include <tty_port.h> */

#include "components.h"

#ifndef M_PI
#  define M_PI 3.14159265
#endif

class Penguin
{
  ssgLeaf   *body_gset ;

  sgVec3 *cloak_verts ;
  sgVec3 *safe_cloak_verts ;

  ssgSelector *cloak ;
  ssgSelector *partyhat ;
  ssgSelector *bowtie ;
  ssgSelector *shades ;
  ssgSelector *dizzy ;
  ssgSelector *space_helmet ;

  ssgSelector *slider    ;  /* 0==Walk, 1==Slide    */
  ssgSelector *supertux  ;  /* 0==Tux,  1==SuperTux */
  ssgSelector *blinker   ;  /* 0==Eyes Open, 1==Half Shut, 2==Closed */
  ssgSelector *beak      ;  /* 0==Beak Shut, 1==Half Open, 2==Open   */

  ssgTransform *lflip   ;
  ssgTransform *rflip   ;
  ssgTransform *lfoot   ;
  ssgTransform *rfoot   ;
  ssgTransform *penguin ;
  ssgTransform *head_dcs ;

  sgCoord lflip_pos ;
  sgCoord rflip_pos ;
  sgCoord lfoot_pos ;
  sgCoord rfoot_pos ;
  sgCoord  peng_pos ;
  sgCoord  last_pos ;

  int last_moved_frame ;
  int frame ;
  int gown ;

public:
  Penguin ( int be_gown ) ;

  ssgTransform *getRoot () { return penguin ; }

  void update ( sgCoord *coord, float head_direction,
                float roll_amount, float squish_amount,
                int flap, int jump, int slide, int swim, int rocket,
                int rocket_gun, int rocket_exhaust,
                int space_gear, int supertux,
                int hat, int tie, int glasses, int dizzy ) ;
} ;

