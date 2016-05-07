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
  Log::PrintMessage( std::to_string(m_Cache_level) ) ;
  for ( int i = 0 ; i < m_Cache_level ; i++ ) {
	    if ( i == Cache::L1_D ) {
	      Log::PrintMessage( std::to_string( cfgparser->ParseDevice( "cache_l1_D", "size" ) )) ;
	      Log::PrintMessage( std::to_string( cfgparser->ParseDevice( "cache_l1_D", "blocksize" ) ) );
	      Log::PrintMessage( std::to_string( cfgparser->ParseDevice( "cache_l1_D", "replacepolicy" ) ) ) ;
	      Log::PrintMessage( std::to_string( cfgparser->ParseDevice( "cache_l1_D", "writepolicy" ) ) );
	    }   // if

	    else if ( i == Cache::L2 ) {


	    }  // else if

	    else if ( i == Cache::L3 ) {


	    } // else if

	  }  // for ()


}  // MemSystem::CreateMemSystem

