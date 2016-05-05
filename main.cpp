#include <stdio.h>
#include "MemSystem.h"

int main(int argc, char const *argv[]) {

    if ( argc > 1 ) {
      	
      printf( "CfgFile=%s\n", argv[1] ) ;
      MemSystem * memsystem = new MemSystem( argv[1] ) ;
      
    
    } // if 

    else {
      printf("Incorrect number of Argument\n" );

    }  // else 
	return 0;

}  // main()
