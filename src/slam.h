
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

/* Limits */

#define MAX_ARGS     16
#define MAX_VARIABLE 16
#define MAX_LABEL    16
#define MAX_TOKEN   256 
#define MAX_CODE    512
#define MAX_STACK    32 
#define MAX_SYMBOL  (MAX_VARIABLE + MAX_LABEL)

/* Code Opcodes */

/* Low nybble is variable address */

#define OPCODE_PUSH_VARIABLE   0xE0
#define OPCODE_POP_VARIABLE    0xF0

/* Remaining opcodes must be in range 0x00 to 0xDF */

#define OPCODE_PUSH_CONSTANT   0x00
#define OPCODE_CALL            0x01
#define OPCODE_SUB             0x02
#define OPCODE_ADD             0x03
#define OPCODE_DIV             0x04
#define OPCODE_MULT            0x05
#define OPCODE_NEG             0x06
#define OPCODE_LESS            0x07
#define OPCODE_LESSEQUAL       0x08
#define OPCODE_GREATER         0x09
#define OPCODE_GREATEREQUAL    0x0A
#define OPCODE_NOTEQUAL        0x0B
#define OPCODE_EQUAL           0x0C
#define OPCODE_JUMP_FALSE      0x0D
#define OPCODE_JUMP            0x0E
#define OPCODE_POP             0x0F
#define OPCODE_HALT            0x10
#define OPCODE_CALLEXT         0x11
#define OPCODE_PAUSE           0x12


enum SLAM_Result
{
  SLAM_PROGRAM_END,
  SLAM_PROGRAM_PAUSE,
  SLAM_PROGRAM_CONTINUE
} ;


/* Token Parser */

void  ungetToken     ( char *c ) ;
void  getToken       ( char *c, FILE *fd = NULL ) ;
void  setDefaultFile ( FILE *fd ) ;

/*
  Address/Opcodes are:

  ???? ????   -- 8 bit opcode.
  ???? xxxx   -- 4 bit opcode with 4 bit variable address.
  ???? ????   -- ditto - plus a float constant stored in the next 4 bytes.
  ???? ????   -- ditto - plus a code address stored in the next 2 bytes.
*/

typedef unsigned short SLAM_Address  ;
typedef unsigned char  SLAM_Opcode   ;
typedef float          SLAM_Variable ;

class SLAM_Symbol
{
public:
  char *symbol ;
  SLAM_Address address ;

  SLAM_Symbol ()
  {
    symbol = NULL ;
    address = 0 ;
  }

  void set ( char *s, SLAM_Address v )
  {
    symbol = new char [ strlen ( s ) + 1 ] ;
    strcpy ( symbol, s ) ;
    address = v ;
  }

  ~SLAM_Symbol () { delete symbol ; }
} ;


class SLAM_Extension
{
public:
  char *symbol ;
  int   argc ;
  float (*func) ( int, float * ) ;
} ;

class SLAM_Program
{
  void pushCodeByte ( unsigned char b ) ;
  void pushCodeAddr ( SLAM_Address a ) ;

  /* Basic low level code generation.  */

  void pushPop          () ;
  void pushSubtract     () ;
  void pushAdd          () ;
  void pushDivide       () ;
  void pushMultiply     () ;
  void pushNegate       () ;
  void pushLess         () ;
  void pushLessEqual    () ;
  void pushGreater      () ;
  void pushGreaterEqual () ;
  void pushNotEqual     () ;
  void pushEqual        () ;
  int  pushJumpIfFalse  ( char *c ) ;
  int  pushJump         ( char *c ) ;

  void pushConstant   ( char *c ) ;
  void pushVariable   ( char *c ) ;
  void pushAssignment ( char *c ) ;
  void pushCall       ( char *c, int argc ) ;

  /* Higher level parsers.  */

  int pushPrimitive      () ;
  int pushMultExpression () ;
  int pushAddExpression  () ;
  int pushRelExpression  () ;
  int pushExpression     () ;

  /* Top level parsers. */

  int  pushPauseStatement      () ;
  int  pushWhileStatement      () ;
  int  pushIfStatement         () ;
  int  pushFunctionCall        ( char *c ) ;
  int  pushAssignmentStatement ( char *c ) ;
  int  pushCompoundStatement   () ;
  int  pushStatement           () ;
  void pushProgram             () ;

  void print_opcode ( FILE *fd, unsigned char op ) ;

  SLAM_Address    getVarSymbol       ( char *s ) ;
  SLAM_Address    getCodeSymbol      ( char *s ) ;
  void            setCodeSymbol      ( char *s, SLAM_Address v ) ;
  int             getExtensionSymbol ( char *s ) ;

private:
  SLAM_Symbol   symtab   [ MAX_SYMBOL ] ;
  int next_var ;
  SLAM_Variable variable [ MAX_VARIABLE ] ;
  int next_code ;
  SLAM_Opcode   code     [ MAX_CODE ] ;
  SLAM_Variable stack    [ MAX_STACK ] ; 
  int           sp    ;
  SLAM_Address  pc    ;

  int next_label ;

  SLAM_Extension *extensions ;
public:

  SLAM_Program ( SLAM_Extension *ext )
  {
    for ( int i = 0 ; i < MAX_SYMBOL ; i++ )
      symtab [ i ] . symbol = NULL ;

    extensions = ext ;
    init () ;
  }

  void reset ()
  {
    for ( int i = 0 ; i < MAX_VARIABLE ; i++ )
      variable [ i ] = 0 ;

    sp = 0 ;
    pc = 0 ;
  }

  void init ()
  {
    int i ;

    for ( i = 0 ; i < MAX_CODE ; i++ )
      code [ i ] = OPCODE_HALT ;

    for ( i = 0 ; i < MAX_SYMBOL ; i++ )
    {
      delete symtab [ i ] . symbol ;
      symtab [ i ] . symbol = NULL ;
    }

    next_label = 0 ;
    next_code  = 0 ;
    next_var   = 0 ;

    reset () ;
  }

  void dump () ;
  int  parse ( char *fname ) ;
  int  parse ( FILE *fd ) ;

  SLAM_Result step () ;
} ;


