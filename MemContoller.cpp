#include "MemContoller.h"

#include "Log.h"
#include <sstream>
CS::MemContoller::MemContoller( const char * cfg ) {
    m_Cache_list = NULL ;
    cfgparser = new CfgParser( cfg ) ;
    m_Cache_level = 0 ;
    m_Monitor = NULL ;
    CreateMemSystem() ;
}  // MemSystem::MemSystem()

void CS::MemContoller::CreateMemSystem() {
    Log::PrintMessageToFile( Log::CacheResultInfoFile, "========== Cache Configuration ==========" ) ;
    m_Cache_level = cfgparser->ParseDevice( "cache_level", "level" ) ;
    m_Cache_list = new Base*[ m_Cache_level ] ;
    m_CacheCtrl_list = new Cache_Ctrl*[ m_Cache_level ] ;
    m_CacheType = new uint8_t[ m_Cache_level ] ;
    for ( uint8_t i = 0; i < m_Cache_level; i++ ) {
        if ( i == CS::MEM_NAME::L1_D ) {
            m_CacheType[ i ] = cfgparser->ParseDevice( "cache_l1_D", "type" ) ;
            m_Cache_list[ i ] = new Cache( CS::MEM_NAME::L1_D, m_CacheType[ i ],
                                           cfgparser->ParseDevice( "cache_l1_D", "size" ),
                                           cfgparser->ParseDevice( "cache_l1_D", "blocksize" ),
                                           cfgparser->ParseDevice( "cache_l1_D", "associativity" ),
                                           cfgparser->ParseDevice( "cache_l1_D", "replacepolicy" ),
                                           cfgparser->ParseDevice( "cache_l1_D", "writepolicy" ),
                                           cfgparser->ParseDevice( "cache_l1_D", "readlatency" ),
                                           cfgparser->ParseDevice( "cache_l1_D", "writelatency" ) ) ;
            Log::PrintMessageToFile(
                    Log::CacheResultInfoFile,
                    "Create cache: cache_l1_D\tsize: " + std::to_string( ((Cache*)m_Cache_list[ i ])->m_CacheSize >> 10 )
                            + "\tblocksize: " + std::to_string( ((Cache*)m_Cache_list[ i ])->m_BlockSize )
                            + "\tSet_Size: " + std::to_string( ((Cache*)m_Cache_list[ i ])->m_Num_Set )
                            + "\tReadLatency: " + std::to_string( ((Cache*)m_Cache_list[ i ])->m_ReadLatency )
                            + "\tWriteLatency: " + std::to_string( ((Cache*)m_Cache_list[ i ])->m_WriteLatency ) ) ;

            m_CacheCtrl_list[ i ] = new Cache_Ctrl( ((Cache*)m_Cache_list[ i ]), m_CacheType[ i ] ) ;

        }   // if

        else if ( i == CS::MEM_NAME::L2 ) {
            m_CacheType[ i ] = cfgparser->ParseDevice( "cache_l2", "type" ) ;

            m_Cache_list[ i ] = new Cache( CS::MEM_NAME::L2, m_CacheType[ i ], cfgparser->ParseDevice( "cache_l2", "size" ),
                                           cfgparser->ParseDevice( "cache_l2", "blocksize" ),
                                           cfgparser->ParseDevice( "cache_l2", "associativity" ),
                                           cfgparser->ParseDevice( "cache_l2", "replacepolicy" ),
                                           cfgparser->ParseDevice( "cache_l2", "writepolicy" ),
                                           cfgparser->ParseDevice( "cache_l2", "readlatency" ),
                                           cfgparser->ParseDevice( "cache_l2", "writelatency" ) ) ;
            Log::PrintMessageToFile(
                    Log::CacheResultInfoFile,
                    "Create cache: cache_l2\t\tsize: " + std::to_string( ((Cache*)m_Cache_list[ i ])->m_CacheSize >> 10 )
                            + "\tblocksize: " + std::to_string( ((Cache*)m_Cache_list[ i ])->m_BlockSize )
                            + "\tSet_Size: " + std::to_string( ((Cache*)m_Cache_list[ i ])->m_Num_Set )
                            + "\tReadLatency: " + std::to_string( ((Cache*)m_Cache_list[ i ])->m_ReadLatency )
                            + "\tWriteLatency: " + std::to_string( ((Cache*)m_Cache_list[ i ])->m_WriteLatency ) ) ;
            m_CacheCtrl_list[ i ] = new Cache_Ctrl( ((Cache*)m_Cache_list[ i ]), m_CacheType[ i ] ) ;
            m_CacheCtrl_list[ i - 1 ]->SetNextLevelCacheCtrl( m_CacheCtrl_list[ i ] ) ;

        }  // else if

        else if ( i == CS::MEM_NAME::L3 ) {
            m_CacheType[ i ] = cfgparser->ParseDevice( "cache_l3", "type" ) ;
            if ( m_CacheType[ i ] == CS::CACHETYPE::NORMAL ) {
                m_Cache_list[ i ]= new Cache( CS::MEM_NAME::L3, m_CacheType[ i ],
                                              cfgparser->ParseDevice( "cache_l3", "size" ),
                                              cfgparser->ParseDevice( "cache_l3", "blocksize" ),
                                              cfgparser->ParseDevice( "cache_l3", "associativity" ),
                                              cfgparser->ParseDevice( "cache_l3", "replacepolicy" ),
                                              cfgparser->ParseDevice( "cache_l3", "writepolicy" ),
                                              cfgparser->ParseDevice( "cache_l3", "readlatency" ),
                                              cfgparser->ParseDevice( "cache_l3", "writelatency" ) ) ;
                Log::PrintMessageToFile( Log::CacheResultInfoFile,
                                  "Create cache: cache_l3\t\tsize: " + std::to_string( ((Cache*)m_Cache_list[ i ])->m_CacheSize >> 10 )
                                + "\tblocksize: " + std::to_string( ((Cache*)m_Cache_list[ i ])->m_BlockSize )
                                + "\tSet_Size: " + std::to_string( ((Cache*)m_Cache_list[ i ])->m_Num_Set )
                                + "\tReadLatency: " + std::to_string( ((Cache*)m_Cache_list[ i ])->m_ReadLatency )
                                + "\tWriteLatency: " + std::to_string( ((Cache*)m_Cache_list[ i ])->m_WriteLatency ) ) ;
                m_CacheCtrl_list[ i ] = new Cache_Ctrl( ((Cache*)m_Cache_list[ i ]), m_CacheType[ i ] ) ;
                m_CacheCtrl_list[ i - 1 ]->SetNextLevelCacheCtrl( m_CacheCtrl_list[ i ] ) ;
            } // if

            else if (  m_CacheType[ i ] == CS::CACHETYPE::PRELOAD ) {

                m_Cache_list[ i ] = new PreloadCache( CS::MEM_NAME::L3, m_CacheType[ i ],
                                                      cfgparser->ParseDevice( "cache_l3", "PT_size" ),
                                                      cfgparser->ParseDevice( "cache_l3", "PT_replacepolicy" ),
                                                      cfgparser->ParseDevice( "cache_l3", "SRAM_size" ),
                                                      cfgparser->ParseDevice( "cache_l3", "SRAM_blocksize" ),
                                                      cfgparser->ParseDevice( "cache_l3", "SRAM_associativity" ),
                                                      cfgparser->ParseDevice( "cache_l3", "SRAM_replacepolicy" ),
                                                      cfgparser->ParseDevice( "cache_l3", "SRAM_writepolicy" ),
                                                      cfgparser->ParseDevice( "cache_l3", "SRAM_readlatency" ),
                                                      cfgparser->ParseDevice( "cache_l3", "SRAM_writelatency" ),
                                                      cfgparser->ParseDevice( "cache_l3", "MRAM_size" ),
                                                      cfgparser->ParseDevice( "cache_l3", "MRAM_blocksize" ),
                                                      cfgparser->ParseDevice( "cache_l3", "MRAM_associativity" ),
                                                      cfgparser->ParseDevice( "cache_l3", "MRAM_replacepolicy" ),
                                                      cfgparser->ParseDevice( "cache_l3", "MRAM_writepolicy" ),
                                                      cfgparser->ParseDevice( "cache_l3", "MRAM_readlatency" ),
                                                      cfgparser->ParseDevice( "cache_l3", "MRAM_writelatency" ) ) ;

                 Log::PrintMessageToFile( Log::CacheResultInfoFile,
                         "Create cache: cache_l3\t\tPreloadTable entry:" + std::to_string( ((PreloadCache*)m_Cache_list[ i ])->PreloadTableSize() )
                                 +"\n\t\t\t\t\t\t\tSRAM_size: " + std::to_string( ((PreloadCache*)m_Cache_list[ i ])->mSRAM->m_CacheSize >> 10 )
                                 + "\tSRAM_blocksize: " + std::to_string( ((PreloadCache*)m_Cache_list[ i ])->mSRAM->m_BlockSize )
                                 + "\tSRAM_Set_Size: " + std::to_string( ((PreloadCache*)m_Cache_list[ i ])->mSRAM->m_Num_Set )
                                 + "\tSRAM_ReadLatency: " + std::to_string( ((PreloadCache*)m_Cache_list[ i ])->mSRAM->m_ReadLatency )
                                 + "\tSRAM_WriteLatency: " + std::to_string( ((PreloadCache*)m_Cache_list[ i ])->mSRAM->m_WriteLatency )
                                 +"\n\t\t\t\t\t\t\tMRAM_size: " + std::to_string( ((PreloadCache*)m_Cache_list[ i ])->mMRAM->m_CacheSize >> 10 )
                                 + "\tMRAM_blocksize: " + std::to_string( ((PreloadCache*)m_Cache_list[ i ])->mMRAM->m_BlockSize )
                                 + "\tMRAM_Set_Size: " + std::to_string( ((PreloadCache*)m_Cache_list[ i ])->mMRAM->m_Num_Set )
                                 + "\tMRAM_ReadLatency: " + std::to_string( ((PreloadCache*)m_Cache_list[ i ])->mMRAM->m_ReadLatency )
                                 + "\tMRAM_WriteLatency: " + std::to_string( ((PreloadCache*)m_Cache_list[ i ])->mMRAM->m_WriteLatency ) ) ;

                 m_CacheCtrl_list[ i ] = new Cache_Ctrl( ((PreloadCache*)m_Cache_list[ i ]), m_CacheType[ i ] ) ;
                 m_CacheCtrl_list[ i - 1 ]->SetNextLevelCacheCtrl( m_CacheCtrl_list[ i ] ) ;

            }  //else if  preload cache

        }  // else if
    }  // for ()

    // connect memory control , now assume is null
    m_CacheCtrl_list[ m_Cache_level - 1 ]->SetNextLevelCacheCtrl( NULL ) ;

    m_Monitor = new Monitor( m_Cache_level, m_Cache_list ) ;
    Log::PrintMessageToFile( Log::CacheResultInfoFile, "========== End of Cache Configuration ==========\n" ) ;
    fflush( Log::CacheResultInfoFile ) ;
}  // MemSystem::CreateMemSystem

void CS::MemContoller::CoreAccessMem( const uint64_t accessTime, const uint64_t address, const uint32_t AccessType,
        Byte* Data, uint32_t length ) {

    m_CacheCtrl_list[ 0 ]->Access( accessTime, address, AccessType, Data, length ) ;

}  // MemSystem::CoreAccessMem()


void CS::MemContoller::EnableRecord() {
    for ( int i = 0; i < m_Cache_level; i++ )
       m_Cache_list[ i ]->mEnableRecord = true ;
}  // MemContoller::EnableRecord()
