#include "MemSystem.h"
#include "Log.h"
MemSystem::MemSystem( const  char * cfg ) {
  cache_list = NULL  ;
  cfgparser = new CfgParser( cfg ) ;

}  // MemSystem::MemSystem()

void MemSystem::CreateMemSystem( uint32_t numOflevel ) {

	  for ( int i = 0 ; i < numOflevel ; i++ ) {
	    if ( i == Cache::L1_D ) {


	    }   // if

	    else if ( i == Cache::L2 ) {


	    }  // else if

	    else if ( i == Cache::L3 ) {


	    } // else if

	  }  // for ()


}  // MemSystem::CreateMemSystem

CfgParser::CfgParser(const char * cfg ){
	m_cfgfile = fopen( cfg, "r") ;
	if ( m_cfgfile == NULL ) Log::PrintError( "CfgFile is NULL." ) ;
}  // CfgParser::CfgParser()
