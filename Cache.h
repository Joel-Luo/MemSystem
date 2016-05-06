#ifndef CACHE_H
#define CACHE_H
#include "Cache_Set.h"



class Cache {
  public:
	enum  {
		ACCESS_TYPE_LOAD = 0,
		ACCESS_TYPE_STORE,
		ACCESS_TYPE_WRITEBACK
	} ACCESS_TYPE;

	enum  {
	  L1_D = 0,
	  L2,
	  L3
	} CACHE_NAME ;

  private:
	uint64_t m_Num_Access ;
	uint64_t m_Num_Hit ;

    Cache_Set** m_Sets ;

    uint64_t m_CacheSize ;
    uint32_t m_num_set ;

  public:

	Cache( int CacheType, uint32_t replacePolicy, uint32_t cache_size, uint32_t blocksize, uint32_t associativity );

    void AccessSingleLine( uint32_t AccessType, uint64_t address, Byte * Data, uint32_t length ) ;
    
	void splitAddress(const uint64_t addr, uint64_t& tag, uint32_t& set_index, uint32_t& block_offset) ;

    bool peekSingleLine( const uint64_t address ) ;

};


#endif
