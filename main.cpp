#include <stdio.h>
#include "MemSystem.h"
#include "Log.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char const *argv[] ) {

  if ( argc > 1 ) {
    if ( strcmp( argv[ 1 ], "-h" ) == 0 ) {
      Log::PrintMessage( " -conf : \"config file path\"" ) ;
      Log::PrintMessage( " -input : \"input file path\"" ) ;

    }  // if
    else {
      MemSystem * memsystem = NULL ;
      for ( int i = 1; i < argc; i = i + 2 ) {
        if ( strcmp( argv[ i ], "-conf" ) == 0 ) {
          Log::PrintMessage( "CfgFile=" + std::string( argv[ i+1 ] ) ) ;
          memsystem = new MemSystem( argv[ i+1 ] ) ;
        }  // if

        else if ( strcmp( argv[ i ], "-input" ) == 0 ) {
          FILE * input = fopen( argv[ i + 1 ], "r" ) ;
          uint64_t address = 0x0 ;
          char op = 'x' ;
          for ( int counter = 1 ; !feof( input ); counter ++  ) {
            char * addr_s = new char[ 20 ] ;
            char * op_s = new char[ 2 ] ;
            fscanf( input, "%s", addr_s ) ;
            if ( feof( input ) ) break ;
            fscanf( input, "%s", op_s ) ;
            address = strtoll( addr_s , NULL ,16) ;
            // Log::PrintMessage( "Instruction id : " + std::to_string(counter) ) ;
            if ( strcmp( op_s, "R" ) == 0 )
              memsystem->CoreAccessMem( address, MemSystem::READ, NULL, 8 ) ;
            else if ( strcmp( op_s, "W" ) == 0 )
              memsystem->CoreAccessMem( address, MemSystem::WRITE, NULL, 8 ) ;
            else
              Log::PrintError( "Unknown memory operation" ) ;


            delete[] addr_s ;
            delete[] op_s ;
          }  // while()
        }  // else if
      }  // for

      for ( int i = 0 ; i < memsystem->m_Monitor->m_CacheLevel ; i++ )
        memsystem->m_Monitor->OutputCacheInfo( i ) ;
    }  // else
  }  // if

  else {

    printf( "Incorrect number of Argument\n" ) ;

  }  // else



  return 0 ;

}  // main()

