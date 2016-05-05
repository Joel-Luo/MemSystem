#ifndef MEMSYSTEM_H
#define MEMSYSTEM_H
#include "Cache.h"


class MemSystem {


  private: 
    Cache ** cache_list ;

  public:  
 	MemSystem( const char * cfg ) ;

	void CoreAccessMem( const uint64_t address ) ;
    
    void AccessNextLevel( uint32_t Cachetype, const uint64_t address ) ;
  
  private: 
    void ParseCfg( const char * path, uint32_t CacheType ) ;

    void CreateMemSystem( uint32_t numOflevel ) ;


};

#endif
  
