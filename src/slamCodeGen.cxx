
#include "slam.h"

void SLAM_Program::pushCodeByte ( SLAM_Opcode op )
{
  code [ next_code++ ] = op ;
}


void SLAM_Program::pushCodeAddr ( SLAM_Address a )
{
  pushCodeByte ( a & 0xFF ) ;
  pushCodeByte ( ( a >> 8 ) & 0xFF ) ;
}


void SLAM_Program::pushConstant ( char *c )
{
  float f = atof ( c ) ; 
  char *ff = (char *) & f ;

  pushCodeByte ( OPCODE_PUSH_CONSTANT ) ;
  pushCodeByte ( ff [ 0 ] ) ;
  pushCodeByte ( ff [ 1 ] ) ;
  pushCodeByte ( ff [ 2 ] ) ;
  pushCodeByte ( ff [ 3 ] ) ;
}

void SLAM_Program::pushVariable ( char *c )
{
  int a = getVarSymbol ( c ) ;

  pushCodeByte ( OPCODE_PUSH_VARIABLE | a ) ;
} 

void SLAM_Program::pushAssignment ( char *c )
{
  int a = getVarSymbol ( c ) ;

  pushCodeByte ( OPCODE_POP_VARIABLE | a ) ;
} 


void SLAM_Program::pushCall ( char *c, int argc )
{
  int ext = getExtensionSymbol ( c ) ;

  if ( ext < 0 )
  {
    int a = getCodeSymbol ( c ) ;

    pushCodeByte ( OPCODE_CALL ) ;
    pushCodeAddr ( a ) ;
    pushCodeByte ( argc ) ;
  }
  else
  {
    pushCodeByte ( OPCODE_CALLEXT ) ;
    pushCodeByte ( ext ) ;
    pushCodeByte ( argc ) ;
  }
} 


void SLAM_Program::pushPop          () { pushCodeByte ( OPCODE_POP   ) ; } 
void SLAM_Program::pushSubtract     () { pushCodeByte ( OPCODE_SUB   ) ; } 
void SLAM_Program::pushAdd          () { pushCodeByte ( OPCODE_ADD   ) ; } 
void SLAM_Program::pushDivide       () { pushCodeByte ( OPCODE_DIV   ) ; } 
void SLAM_Program::pushMultiply     () { pushCodeByte ( OPCODE_MULT  ) ; } 
void SLAM_Program::pushNegate       () { pushCodeByte ( OPCODE_NEG   ) ; } 

void SLAM_Program::pushLess         () { pushCodeByte ( OPCODE_LESS ) ; } 
void SLAM_Program::pushLessEqual    () { pushCodeByte ( OPCODE_LESSEQUAL ) ; } 
void SLAM_Program::pushGreater      () { pushCodeByte ( OPCODE_GREATER ) ; } 
void SLAM_Program::pushGreaterEqual () { pushCodeByte ( OPCODE_GREATEREQUAL ) ; } 
void SLAM_Program::pushNotEqual     () { pushCodeByte ( OPCODE_NOTEQUAL ) ; } 
void SLAM_Program::pushEqual        () { pushCodeByte ( OPCODE_EQUAL ) ; } 

int SLAM_Program::pushJumpIfFalse  ( char *c )
{
  int a = getCodeSymbol ( c ) ;

  pushCodeByte ( OPCODE_JUMP_FALSE ) ;

  int res = next_code ;

  pushCodeAddr ( a ) ;

  return res ;
}

int SLAM_Program::pushJump ( char *c )
{
  int a = getCodeSymbol ( c ) ;

  pushCodeByte ( OPCODE_JUMP ) ;

  int res = next_code ;

  pushCodeAddr ( a ) ;

  return res ;
}


int SLAM_Program::pushPauseStatement()
{ 
  pushCodeByte ( OPCODE_PAUSE ) ;
  return TRUE ;
} 


