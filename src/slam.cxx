
#include "slam.h"

int SLAM_Program::parse ( char *fname )
{
  init () ;

  FILE *fd = fopen ( fname, "ra" ) ;

  if ( fd == NULL )
  {
#ifdef SHOUT_ABOUT_SLAM_ERRORS
    perror ( "SLAM:" ) ;
    fprintf ( stderr, "SLAM: Failed while opening '%s' for reading.\n", fname );
#endif
    return FALSE ;
  }

  parse  ( fd ) ;
  fclose ( fd ) ;
  return TRUE ;
}


int SLAM_Program::parse ( FILE *fd )
{
  setDefaultFile ( fd ) ;
  pushProgram () ;
  return TRUE ;
}


