#ifndef CACHE_SET_H
#define CACHE_SET_H
#include <stdint.h>
#include <stdio.h>

typedef unsigned char Byte  ;


class Cache_Set {    
  public:
	static uint32_t floorLog2( uint32_t number ) ;


  private:
    uint32_t m_Associativity ;  // if associativity == 0 then is direct-map
    uint32_t m_BlockSize_log2 ;
    uint64_t m_tag ; 
    Byte * Data ;  // Data size = way * blocksize 
    
    void ReplaceMent() ;

  public:
  	Cache_Set( uint32_t blocksize_log2, uint32_t associativity );
	  

    uint32_t GetBlockSize_log2() { return m_BlockSize_log2 ; }
    uint32_t GetAssociativity() { return m_Associativity ; } 
    
    uint64_t GetTag() { return m_tag ; } 
    void SetTag(  uint64_t tag ) {  m_tag = tag ; } 

	void ReadLine( Byte * out, uint32_t offset, uint32_t length ) ;
	void WriteLine( Byte * in, uint32_t offset, uint32_t length ) ;

};

#endif
