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

class Orca
{
  ssgTransform    *orca ;
  ssgSelector     *orcaSeq ;

  sgCoord  orca_pos ;
  sgCoord  last_pos ;

  int frame ;

public:
  Orca () ;

  ssgTransform *getRoot () { return orca ; }

  void setCoord ( sgCoord *p )
  {
    orca_pos = *p ;
  }

  void update () ;
} ;

