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

class Rocket
{
  ssgTransform *    rocket     ;
  ssgSelector  *    all_switch ;
/*ssgSelector  *    gun_switch ; */
  ssgSelector  *exhaust_switch ;

public:
  Rocket () ;

  ssgEntity *getRoot () { return rocket ; }

  void update ( int self, int gun, int exhaust ) ;
} ;

