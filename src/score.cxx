
#include "tux.h"

static char *tux_aqfh_rc = NULL ;
static FILE * open_tuxrc ( char *mode ) ;
static void  write_tuxrc ( FILE *fd ) ;


void reset_tuxrc ()
{
  for ( int i = 0 ; i < MAX_GOLD_HERRING ; i++ )
    herring_collected [ i ] = FALSE ;

  FILE *fd = open_tuxrc ( "wb" ) ;
  write_tuxrc ( fd ) ;
  fclose ( fd ) ;
}


void parse_tuxrc ( int *start_level )
{
  FILE *fd = open_tuxrc ( "rb" ) ;

  while ( ! feof ( fd ) )
  {
    int c = getc ( fd ) ;

    switch ( c )
    {
      case 'T' :  /* Training? */
        {
	  c = getc ( fd ) ;

	  if ( c != '=' )
	  {
	    fprintf ( stderr, "Tux_AQFH: '%s' is corrupt.\n", tux_aqfh_rc ) ;
	    exit ( 1 ) ;
	  }

	  c = getc ( fd ) ;

          *start_level = (c=='1') ? LEVEL_TRAINING : LEVEL_ANTECHAMBER ;
        }
        break ;

      case 'G' :  /* Gown? */
        {
	  c = getc ( fd ) ;

	  if ( c != '=' )
	  {
	    fprintf ( stderr, "Tux_AQFH: '%s' is corrupt.\n", tux_aqfh_rc ) ;
	    exit ( 1 ) ;
	  }

	  c = getc ( fd ) ;

          play_as_gown = ( c != '0' ) ;
        }
        break ;

      case 'H' :  /* Herring */
        {
	  c = getc ( fd ) ;

	  if ( c != '=' )
	  {
	    fprintf ( stderr, "Tux_AQFH: '%s' is corrupt.\n", tux_aqfh_rc ) ;
	    exit ( 1 ) ;
	  }

	  for ( int i = 0 ; i < MAX_GOLD_HERRING ; i++ )
	  {
	    if ( c >= ' ' )
	    {
	      c = getc ( fd ) ;
	      herring_collected [ i ] = (c=='*') ;
	    }
	    else
	      herring_collected [ i ] = FALSE ;
	  }
        }
        break ;

      default : break ;
    }
  }

  fclose ( fd ) ;
}


static void write_tuxrc ( FILE *fd )
{
  fprintf ( fd, "H=" ) ;
  for ( int i = 0 ; i < MAX_GOLD_HERRING ; i++ )
    fprintf ( fd, "%c", herring_collected [ i ] ? '*' : '.' ) ;
  fprintf ( fd, "\n" ) ;

  if ( currState == NULL )
    fprintf ( fd, "T=1\n" ) ;
  else
    fprintf ( fd, "T=%d\n", currState -> level <= LEVEL_TRAINING ) ;

  fprintf ( fd, "G=%d\n", play_as_gown ) ;
}


void save_tuxrc ()
{
  FILE *fd = open_tuxrc ( "wb" ) ;
  write_tuxrc ( fd ) ;
  fclose ( fd ) ;
}



static FILE *open_tuxrc ( char *mode )
{
  if ( tux_aqfh_rc == NULL )
  {
    tux_aqfh_rc = getenv ( "TUX_AQFH_RC" ) ;

    if ( tux_aqfh_rc == NULL )
    {
#ifdef WIN32
      tux_aqfh_rc = "C:/TUX_AQFH.INI" ;
#else
      if ( getenv ( "HOME" ) == NULL )
      {
        fprintf ( stderr, "Tux_AQFH: Missing $HOME!?!\n" ) ;
        exit ( 1 ) ;
      }

      tux_aqfh_rc = new char [ strlen ( getenv( "HOME" ) ) +
                               strlen ( "/.tux_aqfh_rc"  ) + 1 ] ;
      sprintf ( tux_aqfh_rc, "%s/.tux_aqfh_rc", getenv ( "HOME" ) ) ;
#endif
    }
  }

  FILE *fd = fopen ( tux_aqfh_rc, mode ) ;

  if ( fd == NULL )
  {
    fd = fopen ( tux_aqfh_rc, "wb" ) ;

    if ( fd == NULL )
    {
      fprintf ( stderr, "Tux_AQFH: Can't open '%s'\n", tux_aqfh_rc ) ;
      exit ( 1 ) ;
    }

    write_tuxrc ( fd ) ;
    fclose ( fd ) ;

    fd = fopen ( tux_aqfh_rc, mode ) ;

    if ( fd == NULL )
    {
      fprintf ( stderr, "Tux_AQFH: Can't open '%s'\n", tux_aqfh_rc ) ;
      exit ( 1 ) ;
    }
  }

  return fd ;
}


