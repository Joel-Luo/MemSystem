#ifndef MEMSYSTEM_H
#define MEMSYSTEM_H
#include "Cache.h"

class CfgParser {
  private:
	FILE * m_cfgfile ;

  public:
	CfgParser( const char * cfg ) ;
	void ParseCfg( const char * path, uint32_t CacheType ) ;

	~CfgParser() ;
};


class MemSystem {


  private: 
    Cache ** cache_list ;
    CfgParser * cfgparser ;

  public:  
 	MemSystem( const char * cfg ) ;

	void CoreAccessMem( const uint64_t address ) ;
    
    void AccessNextLevel( uint32_t Cachetype, const uint64_t address ) ;
  
  private: 


    void CreateMemSystem(  uint32_t numOflevel ) ;


};



#endif
  
