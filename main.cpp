#include <stdio.h>
#include "MemSystem.h"
#include "Log.h"

int main(int argc, char const *argv[]) {

    if ( argc > 1 ) {
      	
      Log::PrintMessage( "CfgFile=" + std::string(argv[1]) ) ;
      MemSystem * memsystem = new MemSystem( argv[1] ) ;
    
    } // if 

    else {

      printf("Incorrect number of Argument\n" );

    }  // else 

	return 0;

}  // main()

