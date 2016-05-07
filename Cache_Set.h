#ifndef CACHE_SET_H
#define CACHE_SET_H
#include <stdint.h>
#include <stdio.h>

typedef unsigned char Byte  ;

enum REPLACEPOLICY {
  LRU = 0,
  ROUND_ROBIN,
  RANDOM
} ;


class Cache_Set {    

  private:
    uint32_t m_ReplacePolicy ;
    uint32_t m_Associativity ;
    uint32_t m_BlockSize ;
    uint64_t m_tag ; 
    Byte * Data ;  // Data size = way * blocksize 
    
    void ReplaceMent() ;

  public:
  	Cache_Set( uint32_t replacePolicy, uint32_t blocksize, uint32_t associativity );
	  

    uint32_t GetBlockSize() { return m_BlockSize ; } 
    uint32_t GetAssociativity() { return m_Associativity ; } 
    
    uint64_t GetTag() { return m_tag ; } 
    void SetTag(  uint64_t tag ) {  m_tag = tag ; } 

	void ReadLine( Byte * out, uint32_t offset, uint32_t length ) ;
	void WriteLine( Byte * in, uint32_t offset, uint32_t length ) ;

	
};

#endif
