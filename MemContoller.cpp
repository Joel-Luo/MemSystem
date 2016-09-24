#include "MemContoller.h"

#include "Log.h"
#include <sstream>
MemContoller::MemContoller( const char * cfg ) {
    m_Cache_list = NULL ;
    cfgparser = new CfgParser( cfg ) ;
    m_Cache_level = 0 ;
    m_Monitor = NULL ;
    CreateMemSystem() ;
}  // MemSystem::MemSystem()

void MemContoller::CreateMemSystem() {
    Log::PrintMessageToFile( Log::CacheResultInfoFile, "========== Cache Configuration ==========" ) ;
    m_Cache_level = cfgparser->ParseDevice( "cache_level", "level" ) ;
    m_Cache_list = new Cache*[ m_Cache_level ] ;
    m_CacheCtrl_list = new Cache_Ctrl*[ m_Cache_level ] ;
    m_CacheType = new uint8_t[ m_Cache_level ] ;
    for ( uint8_t i = 0; i < m_Cache_level; i++ ) {
        if ( i == Cache::L1_D ) {
            m_CacheType[ i ] = cfgparser->ParseDevice( "cache_l1_D", "type" ) ;
            m_Cache_list[ i ] = new Cache( Cache::L1_D, m_CacheType[ i ],
                                           cfgparser->ParseDevice( "cache_l1_D", "size" ),
                                           cfgparser->ParseDevice( "cache_l1_D", "blocksize" ),
                                           cfgparser->ParseDevice( "cache_l1_D", "associativity" ),
                                           cfgparser->ParseDevice( "cache_l1_D", "replacepolicy" ),
                                           cfgparser->ParseDevice( "cache_l1_D", "writepolicy" ),
                                           cfgparser->ParseDevice( "cache_l1_D", "readlatency" ),
                                           cfgparser->ParseDevice( "cache_l1_D", "writelatency" ) ) ;
            Log::PrintMessageToFile(
                    Log::CacheResultInfoFile,
                    "Create cache: cache_l1_D\tsize: " + std::to_string( m_Cache_list[ i ]->m_CacheSize >> 10 )
                            + "\tblocksize: " + std::to_string( m_Cache_list[ i ]->m_BlockSize ) + "\tReadLatency: "
                            + std::to_string( m_Cache_list[ i ]->m_ReadLatency ) + "\tWriteLatency: "
                            + std::to_string( m_Cache_list[ i ]->m_WriteLatency ) ) ;

            m_CacheCtrl_list[ i ] = new Cache_Ctrl( m_Cache_list[ i ], m_CacheType[ i ] ) ;

        }   // if

        else if ( i == Cache::L2 ) {
            m_CacheType[ i ] = cfgparser->ParseDevice( "cache_l2", "type" ) ;

            m_Cache_list[ i ] = new Cache( Cache::L2, m_CacheType[ i ], cfgparser->ParseDevice( "cache_l2", "size" ),
                                           cfgparser->ParseDevice( "cache_l2", "blocksize" ),
                                           cfgparser->ParseDevice( "cache_l2", "associativity" ),
                                           cfgparser->ParseDevice( "cache_l2", "replacepolicy" ),
                                           cfgparser->ParseDevice( "cache_l2", "writepolicy" ),
                                           cfgparser->ParseDevice( "cache_l2", "readlatency" ),
                                           cfgparser->ParseDevice( "cache_l2", "writelatency" ) ) ;
            Log::PrintMessageToFile(
                    Log::CacheResultInfoFile,
                    "Create cache: cache_l2\t\tsize: " + std::to_string( m_Cache_list[ i ]->m_CacheSize >> 10 )
                            + "\tblocksize: " + std::to_string( m_Cache_list[ i ]->m_BlockSize ) + "\tReadLatency: "
                            + std::to_string( m_Cache_list[ i ]->m_ReadLatency ) + "\tWriteLatency: "
                            + std::to_string( m_Cache_list[ i ]->m_WriteLatency ) ) ;
            m_CacheCtrl_list[ i ] = new Cache_Ctrl( m_Cache_list[ i ], m_CacheType[ i ] ) ;
            m_CacheCtrl_list[ i - 1 ]->SetNextLevelCacheCtrl( m_CacheCtrl_list[ i ] ) ;

        }  // else if

        else if ( i == Cache::L3 ) {
            m_CacheType[ i ] = cfgparser->ParseDevice( "cache_l3", "type" ) ;
            m_Cache_list[ i ] = new Cache( Cache::L3, m_CacheType[ i ], cfgparser->ParseDevice( "cache_l3", "size" ),
                                           cfgparser->ParseDevice( "cache_l3", "blocksize" ),
                                           cfgparser->ParseDevice( "cache_l3", "associativity" ),
                                           cfgparser->ParseDevice( "cache_l3", "replacepolicy" ),
                                           cfgparser->ParseDevice( "cache_l3", "writepolicy" ),
                                           cfgparser->ParseDevice( "cache_l3", "readlatency" ),
                                           cfgparser->ParseDevice( "cache_l3", "writelatency" ) ) ;
            Log::PrintMessageToFile(
                    Log::CacheResultInfoFile,
                    "Create cache: cache_l3\t\tsize: " + std::to_string( m_Cache_list[ i ]->m_CacheSize >> 10 )
                            + "\tblocksize: " + std::to_string( m_Cache_list[ i ]->m_BlockSize ) + "\tReadLatency: "
                            + std::to_string( m_Cache_list[ i ]->m_ReadLatency ) + "\tWriteLatency: "
                            + std::to_string( m_Cache_list[ i ]->m_WriteLatency ) ) ;
            m_CacheCtrl_list[ i ] = new Cache_Ctrl( m_Cache_list[ i ], m_CacheType[ i ] ) ;
            m_CacheCtrl_list[ i - 1 ]->SetNextLevelCacheCtrl( m_CacheCtrl_list[ i ] ) ;
        }  // else if
    }  // for ()

    // connect memory control , now assume is null
    m_CacheCtrl_list[ m_Cache_level - 1 ]->SetNextLevelCacheCtrl( NULL ) ;

    m_Monitor = new Monitor( m_Cache_level, m_Cache_list ) ;
    Log::PrintMessageToFile( Log::CacheResultInfoFile, "========== End of Cache Configuration ==========\n" ) ;
    fflush( Log::CacheResultInfoFile ) ;
}  // MemSystem::CreateMemSystem

void MemContoller::CoreAccessMem( const uint64_t accessTime, const uint64_t address, const uint32_t AccessType,
        Byte* Data, uint32_t length ) {

    m_CacheCtrl_list[ 0 ]->Access( accessTime, address, AccessType, Data, length ) ;

}  // MemSystem::CoreAccessMem()


void MemContoller::EnableRecord() {
    for ( int i = 0; i < m_Cache_level; i++ )
        m_Cache_list[ i ]->mEnableRecord = true ;
}  // MemContoller::EnableRecord()
