
#include "slam.h"


SLAM_Address SLAM_Program::getVarSymbol ( char *s )
{
  for ( int i = 0 ; i < MAX_SYMBOL ; i++ )
  {
    if ( symtab [ i ] . symbol == NULL )
    {
      if ( next_var >= MAX_VARIABLE-1 )
      {
        fprintf ( stderr, "SLAM: Too many variables.\n" ) ;
        next_var-- ;
      }

      symtab [ i ] . set ( s, next_var++ ) ;
      return symtab [ i ] . address ;
    }
    else
    if ( strcmp ( s, symtab [ i ] . symbol ) == 0 )
      return symtab [ i ] . address ;
  }

  fprintf ( stderr, "SLAM: Too many symbols in one program.\n" ) ;
  return MAX_VARIABLE-1 ;
}


int SLAM_Program::getExtensionSymbol ( char *s )
{
  for ( int i = 0 ; extensions [ i ] . symbol != NULL ; i++ )
    if ( strcmp ( s, extensions [ i ] . symbol ) == 0 )
      return i ;

  return -1 ;
}


SLAM_Address SLAM_Program::getCodeSymbol ( char *s )
{
  for ( int i = 0 ; i < MAX_SYMBOL ; i++ )
  {
    if ( symtab [ i ] . symbol == NULL )
    {
      symtab [ i ] . set ( s, 0 ) ;
      return 0 ;
    }
    else
    if ( strcmp ( s, symtab [ i ] . symbol ) == 0 )
      return symtab [ i ] . address ;
  }

  fprintf ( stderr, "SLAM: Too many symbols in one program.\n" ) ;
  return 0 ;
}



void SLAM_Program::setCodeSymbol ( char *s, SLAM_Address v )
{
  for ( int i = 0 ; i < MAX_SYMBOL ; i++ )
  {
    if ( symtab [ i ] . symbol == NULL )
    {
      symtab [ i ] . set ( s, v ) ;
      return ;
    }
    else
    if ( strcmp ( s, symtab [ i ] . symbol ) == 0 )
    {
      symtab [ i ] . address = v ;
      return ;
    }
  }

  fprintf ( stderr, "SLAM: Too many symbols in one program.\n" ) ;
}


