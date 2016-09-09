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
                                           cfgparser->ParseDevice( "cache_l1", "associativity" ),
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

            if ( m_CacheType[ i ] == Cache::HYBRIDCACHE ) {
                m_Cache_list[ i ] = new Cache( Cache::L2, m_CacheType[ i ],
                                               cfgparser->ParseDevice( "cache_l2", "size" ),
                                               cfgparser->ParseDevice( "cache_l2", "blocksize" ),
                                               cfgparser->ParseDevice( "cache_l2", "associativity" ),
                                               cfgparser->ParseDevice( "cache_l2", "replacepolicy" ),
                                               cfgparser->ParseDevice( "cache_l2", "writepolicy" ), -1,
                                               -1 ) ;
                uint8_t numOfCellType = cfgparser->ParseDevice( "cache_l2", "numofcelltype" ) ;
                Log::PrintMessageToFile(
                        Log::CacheResultInfoFile,
                        "Create cache: Hybridcache_l2\t\tsize: "
                                + std::to_string( m_Cache_list[ i ]->m_CacheSize >> 10 ) + "\tblocksize: "
                                + std::to_string( m_Cache_list[ i ]->m_BlockSize ) + "\tnumOfCellType: "
                                + std::to_string( numOfCellType ) ) ;

                uint8_t numofsub = cfgparser->ParseDevice( "cache_l2", "numofsub" ) ;
                uint8_t *size = new uint8_t[ numOfCellType ] ;
                uint32_t *retentiontime = new uint32_t[ numOfCellType ] ;
                uint8_t *readl = new uint8_t[ numOfCellType ] ;
                uint8_t *writel = new uint8_t[ numOfCellType ] ;
                for ( int ii = 0; ii < numOfCellType; ii++ ) {
                    size[ ii ] = cfgparser->ParseDevice( "cache_l2", "subsize" + std::to_string( ii + 1 ) ) ;
                    retentiontime[ ii ] = cfgparser->ParseDevice( "cache_l2",
                                                                  "retentiontime" + std::to_string( i + 1 ) ) ;
                    readl[ ii ] = cfgparser->ParseDevice( "cache_l2", "readlatency" + std::to_string( ii + 1 ) ) ;
                    writel[ ii ] = cfgparser->ParseDevice( "cache_l2", "writelatency" + std::to_string( ii + 1 ) ) ;
                    Log::PrintMessageToFile(
                            Log::CacheResultInfoFile,
                            "\tCell" + std::to_string( ii + 1 ) + "\tsubsize: " + std::to_string( size[ ii ] )
                                    + "\tretentiontime: " + std::to_string( retentiontime[ ii ] ) + "\treadlatnecy: "
                                    + std::to_string( readl[ ii ] ) + "\twritelatency: "
                                    + std::to_string( writel[ ii ] ) ) ;
                }  // for

                m_Cache_list[ i ]->BuildHybridCache( numOfCellType, numofsub, size, retentiontime, readl, writel,
                                                     cfgparser->ParseDevice( "cache_l2", "blocksize" ),
                                                     cfgparser->ParseDevice( "cache_l2", "associativity" ),
                                                     cfgparser->ParseDevice( "cache_l2", "replacepolicy" ),
                                                     cfgparser->ParseDevice( "cache_l2", "writepolicy" ) ) ;

            }  // if

            else if ( m_CacheType[ i ] == Cache::BUFFERCACHE ) {
                m_Cache_list[ i ] = new Cache( Cache::L2, m_CacheType[ i ],
                                               cfgparser->ParseDevice( "cache_l2", "size" ),
                                               cfgparser->ParseDevice( "cache_l2", "blocksize" ),
                                               cfgparser->ParseDevice( "cache_l2", "associativity" ),
                                               cfgparser->ParseDevice( "cache_l2", "replacepolicy" ),
                                               cfgparser->ParseDevice( "cache_l2", "writepolicy" ),
                                               cfgparser->ParseDevice( "cache_l2", "readlatency" ),
                                               cfgparser->ParseDevice( "cache_l2", "writelatency" ) ) ;
                Log::PrintMessageToFile(
                        Log::CacheResultInfoFile,
                        "Create cache: Buffercache_l2\t\tsize: "
                                + std::to_string( m_Cache_list[ i ]->m_CacheSize >> 10 ) + "\tblocksize:"
                                + std::to_string( m_Cache_list[ i ]->m_BlockSize ) ) ;

                m_Cache_list[ i ]->BuildBufferCache( cfgparser->ParseDevice( "cache_l2", "bufferentry" ),
                                                     m_Cache_list[ i - 1 ]->m_BlockSize,
                                                     cfgparser->ParseDevice( "cache_l2", "breadlatency" ),
                                                     cfgparser->ParseDevice( "cache_l2", "bwritelatency" ) ) ;

                Log::PrintMessageToFile(
                        Log::CacheResultInfoFile,
                        "\tBuffer: \tEntry: " + std::to_string( m_Cache_list[ i ]->mBufferCache->mNumOfEntry )
                                + "\treadlatnecy: " + std::to_string( m_Cache_list[ i ]->mBufferCache->m_ReadLatency )
                                + "\twritelatency: "
                                + std::to_string( m_Cache_list[ i ]->mBufferCache->m_WriteLatency ) ) ;
                Log::PrintMessageToFile(
                        Log::CacheResultInfoFile,
                        "\tSTTRAM: \tsize: " + std::to_string( m_Cache_list[ i ]->m_CacheSize >> 10 ) + "\tblocksize: "
                                + std::to_string( m_Cache_list[ i ]->m_BlockSize ) + "\tReadLatency: "
                                + std::to_string( m_Cache_list[ i ]->m_ReadLatency ) + "\tWriteLatency: "
                                + std::to_string( m_Cache_list[ i ]->m_WriteLatency ) ) ;
            }  // else if
            else {
                m_Cache_list[ i ] = new Cache( Cache::L2, m_CacheType[ i ],
                                               cfgparser->ParseDevice( "cache_l2", "size" ),
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
            }  // else
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

void MemContoller::FinishAllOperation() {
    if ( m_CacheCtrl_list[ 1 ]->mThisCacheType == Cache::BUFFERCACHE ) {
        m_CacheCtrl_list[ 1 ]->FlushOperationInBufferCache() ;
        m_CacheCtrl_list[ 1 ]->AddFinishTimeStampInCacheLine() ;
    }  // if
}  // MemContoller::FinishAllOperation()

void MemContoller::EnableRecord() {
  for ( int i = 0 ; i < m_Cache_level ; i++ )
      m_Cache_list[ i ]->mEnableRecord = true ;
}  // MemContoller::EnableRecord()
