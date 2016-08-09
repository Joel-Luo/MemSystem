/* 
 * File:   main.cpp
 * Author: LuoJing-Yuan
 *
 * Created on August 9, 2016, 1:47 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>

int main(int argc, char** argv) {
  std::vector<std::vector<uint32_t>*> * CacheLineList ;  
  if ( argc == 3 ) {
    FILE * input = NULL ;
    FILE * output = NULL    ;
    input = fopen( argv[1], "r"  ) ;
    output = fopen( argv[2], "w" ) ;
    
    CacheLineList = new std::vector<std::vector<uint32_t>*>() ;
    uint32_t set_index = 0 ;
    uint32_t way_index = 0 ;
  
    fscanf( input, "%u %u", &set_index, &way_index ) ;
    printf( "%u %u\n", set_index, way_index ) ;
    while ( !feof( input ) ) {
      uint64_t nowTime = 0 ;
      uint64_t lastTime = 0 ;
      std::vector<uint32_t> * FrqDiff = new std::vector<uint32_t>() ;
      uint32_t diff = -1 ;
      fscanf( input, "%llx", &nowTime ) ;
      printf( "%llx ", nowTime ) ;
      while ( nowTime != (uint64_t) -1 ) {
        diff = abs( ( nowTime - lastTime ) - 100 );
        FrqDiff->push_back(diff) ;
        lastTime = nowTime ;
        fscanf( input, "%llx", &nowTime) ;
        printf( "%llx ", nowTime ) ;
      } // while 
     
      CacheLineList->push_back( FrqDiff ) ;
      fscanf( input, "%u %u", &set_index, &way_index ) ;
      if ( !feof( input ) )printf( "\n%u %u\n", set_index, way_index ) ;
    } // while
    
    for ( uint8_t j = 0 ; j <= way_index ;j++ ) {
      for ( uint32_t i = 0 ; i <= set_index ; i++ ) {
        fprintf( output, "%d-%d ", i, j ) ;
        for ( uint64_t k = 0 ; k < CacheLineList->at( i*(way_index+1)+ j )->size() ; k++ )
          fprintf( output, "%x ",  CacheLineList->at( i*(way_index+1)+ j )->at(k) ) ;  
      } // for
      fprintf( output, "\n" ) ;
    } //for
    
    
    
  }  // if 
  else printf( "Incorrect Number of parameter \n" ) ;
  
  return 0;
}

