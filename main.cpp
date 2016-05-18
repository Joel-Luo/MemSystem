#include <stdio.h>
#include "MemSystem.h"
#include "Log.h"
#include <stdint.h>

int main(int argc, char const *argv[]) {

    if ( argc > 1 ) {
      	
      Log::PrintMessage( "CfgFile=" + std::string(argv[1]) ) ;
      MemSystem * memsystem = new MemSystem( argv[1] ) ;
      uint64_t address =  0x0000000000000001 ;
      for ( int i = 0 ; i < 10 ; i++ ) {

    	  memsystem->CoreAccessMem( address ,MemSystem::READ, NULL, 8 ) ;
    	  address = address + 64 ;
    	  Log::PrintMessage("\n") ;

      } // for
    } // if 

    else {

      printf("Incorrect number of Argument\n" );

    }  // else 

	return 0;

}  // main()

