#ifndef MEMSYSTEM_H
#define MEMSYSTEM_H
#include "Cache.h"
#include "MainMemory.h"
#include "CfgParser.h"


class MemSystem {


  private: 
    Cache ** cache_list ;
    uint32_t m_Cache_level ;
    CfgParser * cfgparser ;

  public:  

 	MemSystem( const char * cfg ) ;

	void CoreAccessMem( const uint64_t address ) ;
    
    void AccessNextLevel( uint32_t Cachetype, const uint64_t address ) ;
  
  private: 


    void CreateMemSystem() ;


};



#endif
  
