#include "MemSystem.h"
#include "Log.h"

MemSystem::MemSystem( const  char * cfg ) {
  cache_list = NULL  ;
  cfgparser = new CfgParser( cfg ) ;
  m_Cache_level = 0 ;
  CreateMemSystem() ;
}  // MemSystem::MemSystem()

void MemSystem::CreateMemSystem() {
  m_Cache_level = cfgparser->ParseDevice( "cache_level", "level" ) ;
  cache_list = new Cache* [m_Cache_level] ;
  for ( int i = 0 ; i < m_Cache_level ; i++ ) {
	    if ( i == Cache::L1_D ) {
          cache_list[i] = new Cache( Cache::L1_D,
	                                 cfgparser->ParseDevice( "cache_l1_D", "size" ),
	                                 cfgparser->ParseDevice( "cache_l1_D", "blocksize" ),
	                                 cfgparser->ParseDevice( "cache_l1_D", "replacepolicy" ) ,
	                                 cfgparser->ParseDevice( "cache_l1_D", "writepolicy" ) );
	    }   // if

	    else if ( i == Cache::L2 ) {
	          cache_list[i] = new Cache( Cache::L2,
		                                 cfgparser->ParseDevice( "cache_l2", "size" ),
		                                 cfgparser->ParseDevice( "cache_l2", "blocksize" ),
										 cfgparser->ParseDevice( "cache_l2", "associativity" ),
		                                 cfgparser->ParseDevice( "cache_l2", "replacepolicy" ) ,
		                                 cfgparser->ParseDevice( "cache_l2", "writepolicy" ) ) ;
	    }  // else if

	    else if ( i == Cache::L3 ) {
	          cache_list[i] = new Cache( Cache::L3,
		                                 cfgparser->ParseDevice( "cache_l3", "size" ),
		                                 cfgparser->ParseDevice( "cache_l3", "blocksize" ),
										 cfgparser->ParseDevice( "cache_l3", "associativity" ),
										 cfgparser->ParseDevice( "cache_l3", "replacepolicy" ) ,
		                                 cfgparser->ParseDevice( "cache_l3", "writepolicy" ) );
	    } // else if
	  }  // for ()


}  // MemSystem::CreateMemSystem

