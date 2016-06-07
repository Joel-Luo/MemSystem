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
    m_CacheType = new uint8_t [ m_Cache_level ] ;
    for ( int i = 0; i < m_Cache_level; i++ ) {
        if ( i == Cache::L1_D ) {
            m_CacheType[ i ] =  cfgparser->ParseDevice( "cache_l1_D", "type" );
            m_Cache_list[ i ] =
                    new Cache( Cache::L1_D, m_CacheType[ i ], cfgparser->ParseDevice( "cache_l1_D", "size" ), cfgparser->ParseDevice( "cache_l1_D", "blocksize" ), cfgparser->ParseDevice( "cache_l1", "associativity" ), cfgparser->ParseDevice( "cache_l1_D", "replacepolicy" ), cfgparser->ParseDevice( "cache_l1_D", "writepolicy" ), cfgparser->ParseDevice( "cache_l1_D", "readlatency" ), cfgparser->ParseDevice( "cache_l1_D", "writelatency" ) ) ;
            Log::PrintMessageToFile( Log::CacheResultInfoFile, "Create cache: cache_l1_D\tsize: "
                    + std::to_string( m_Cache_list[ i ]->m_CacheSize >> 10 ) + "\tblocksize:"
                    + std::to_string( m_Cache_list[ i ]->m_BlockSize ) + "\tReadLatency:"
                    + std::to_string( m_Cache_list[ i ]->m_ReadLatency ) + "\tWriteLatency:"
                    + std::to_string( m_Cache_list[ i ]->m_WriteLatency ) ) ;
        }   // if

        else if ( i == Cache::L2 ) {
            m_CacheType[ i ] =  cfgparser->ParseDevice( "cache_l2", "type" );

            if ( m_CacheType[ i ] == Cache::HYBRIDCACHE ) {
                m_Cache_list[ i ] = new Cache( Cache::L2, m_CacheType[ i ],cfgparser->ParseDevice( "cache_l2", "size" ), cfgparser->ParseDevice( "cache_l2", "blocksize" ), cfgparser->ParseDevice( "cache_l2", "associativity" ), cfgparser->ParseDevice( "cache_l2", "replacepolicy" ), cfgparser->ParseDevice( "cache_l2", "writepolicy" ) , -1 , -1 ) ;
                uint8_t numOfCellType = cfgparser->ParseDevice( "cache_l2", "numofcelltype" ) ;
                Log::PrintMessageToFile( Log::CacheResultInfoFile, "Create cache: Hybridcache_l2\t\tsize: "
                                    + std::to_string( m_Cache_list[ i ]->m_CacheSize >> 10 ) + "\tblocksize:"
                                    + std::to_string( m_Cache_list[ i ]->m_BlockSize ) + "\tnumOfCellType:"
                                    + std::to_string( numOfCellType )  ) ;

                uint8_t numofsub = cfgparser->ParseDevice( "cache_l2", "numofsub" ) ;
                uint8_t *size = new uint8_t [ numOfCellType ] ;
                uint32_t *retentiontime = new uint32_t [ numOfCellType ] ;
                uint8_t *readl = new uint8_t [ numOfCellType ] ;
                uint8_t *writel = new uint8_t [ numOfCellType ] ;
                for ( int ii = 0 ; ii < numOfCellType ; ii++ ) {
                  size[ii] = cfgparser->ParseDevice( "cache_l2", "subsize" + std::to_string(ii+1) ) ;
                  retentiontime[ii] = cfgparser->ParseDevice( "cache_l2", "retentiontime" + std::to_string(i+1) ) ;
                  readl[ii] = cfgparser->ParseDevice( "cache_l2", "readlatency" + std::to_string(ii+1) ) ;
                  writel[ii] = cfgparser->ParseDevice( "cache_l2", "writelatency" + std::to_string(ii+1) ) ;
                  Log::PrintMessageToFile( Log::CacheResultInfoFile, "\tCell" + std::to_string( ii+1) + "\tsubsize:"
                                      + std::to_string( size[ii]) + "\tretentiontime:"
                                      + std::to_string( retentiontime[ii]) + "\treadlatnecy:"
                                      + std::to_string( readl[ii]) + "\twritelatency:"
                                      + std::to_string( writel[ii]) ) ;
                }  // for

                m_Cache_list[ i ]->BuildHybridCache( numOfCellType, numofsub, size, retentiontime, readl, writel, cfgparser->ParseDevice( "cache_l3", "blocksize" ), cfgparser->ParseDevice( "cache_l3", "associativity" ), cfgparser->ParseDevice( "cache_l3", "replacepolicy" ), cfgparser->ParseDevice( "cache_l3", "writepolicy" ) ) ;


            } // if
            else {
                m_Cache_list[ i ] = new Cache( Cache::L2, m_CacheType[ i ],cfgparser->ParseDevice( "cache_l2", "size" ), cfgparser->ParseDevice( "cache_l2", "blocksize" ), cfgparser->ParseDevice( "cache_l2", "associativity" ), cfgparser->ParseDevice( "cache_l2", "replacepolicy" ), cfgparser->ParseDevice( "cache_l2", "writepolicy" ), cfgparser->ParseDevice( "cache_l2", "readlatency" ), cfgparser->ParseDevice( "cache_l2", "writelatency" ) ) ;
                Log::PrintMessageToFile( Log::CacheResultInfoFile, "Create cache: cache_l2\t\tsize: "
                                    + std::to_string( m_Cache_list[ i ]->m_CacheSize >> 10 ) + "\tblocksize:"
                                    + std::to_string( m_Cache_list[ i ]->m_BlockSize ) + "\tReadLatency:"
                                    + std::to_string( m_Cache_list[ i ]->m_ReadLatency ) + "\tWriteLatency:"
                                    + std::to_string( m_Cache_list[ i ]->m_WriteLatency ) ) ;
            }  // else
        }  // else if

        else if ( i == Cache::L3 ) {
            m_CacheType[ i ] =  cfgparser->ParseDevice( "cache_l3", "type" );
            m_Cache_list[ i ] =
                    new Cache( Cache::L3, m_CacheType[ i ], cfgparser->ParseDevice( "cache_l3", "size" ), cfgparser->ParseDevice( "cache_l3", "blocksize" ), cfgparser->ParseDevice( "cache_l3", "associativity" ), cfgparser->ParseDevice( "cache_l3", "replacepolicy" ), cfgparser->ParseDevice( "cache_l3", "writepolicy" ), cfgparser->ParseDevice( "cache_l3", "readlatency" ), cfgparser->ParseDevice( "cache_l3", "writelatency" ) ) ;
            Log::PrintMessageToFile( Log::CacheResultInfoFile, "Create cache: cache_l3\t\tsize: "
                    + std::to_string( m_Cache_list[ i ]->m_CacheSize >> 10 ) + "\tblocksize:"
                    + std::to_string( m_Cache_list[ i ]->m_BlockSize ) + "\tReadLatency:"
                    + std::to_string( m_Cache_list[ i ]->m_ReadLatency ) + "\tWriteLatency:"
                    + std::to_string( m_Cache_list[ i ]->m_WriteLatency ) ) ;

        }  // else if
    }  // for ()


    m_Monitor = new Monitor( m_Cache_level, m_Cache_list ) ;
    Log::PrintMessageToFile( Log::CacheResultInfoFile, "========== End of Cache Configuration ==========\n" ) ;
    fflush(Log::CacheResultInfoFile ) ;
}  // MemSystem::CreateMemSystem

void MemContoller::CoreAccessMem( const uint64_t accessTime, const uint64_t address, const uint32_t AccessType, Byte* Data, uint32_t length ) {

    bool TimeUpWriteBack = false ;
#ifdef SIM_DATA
        Byte * wire_L1ToL2 = new Byte[ m_Cache_list[ 0 ]->m_BlockSize ] ;
#else
        Byte * wire_L1ToL2 = NULL ;
#endif

    if ( m_Cache_list[ 0 ]->AccessCache( AccessType, accessTime , address, Data, length, TimeUpWriteBack, wire_L1ToL2 ) ) {  // hit
        if ( AccessType == Cache::WRITE ) {
            m_Cache_list[ 0 ]->m_Num_W_Access++ ;
            m_Cache_list[ 0 ]->m_Num_W_Hit++ ;
        }  // if
        else {
            m_Cache_list[ 0 ]->m_Num_R_Access++ ;
            m_Cache_list[ 0 ]->m_Num_R_Hit++ ;
        }  // else


         Log::PrintDebugLog( "CACHE L1 hit  : address:" ) ;
         printf( "0x%016llX length: %d\n", ( long long ) address, length ) ;

    }  // if

    else {  // miss
        if ( AccessType == Cache::WRITE )
            m_Cache_list[ 0 ]->m_Num_W_Access++ ;
        else
            m_Cache_list[ 0 ]->m_Num_R_Access++ ;


         Log::PrintDebugLog( "CACHE L1 miss : address:" ) ;
         printf( "0x%016llX length: %d\n", ( long long ) address, length ) ;


        uint64_t tag ;
        uint32_t set_index, block_offset ;
        m_Cache_list[ 0 ]->SplitAddress( address, tag, set_index, block_offset ) ;

        if ( TimeUpWriteBack ) {  // Time up write back
            uint64_t StoreAddress ;
            m_Cache_list[ 0 ]->StoreCacheBlock( set_index, StoreAddress, wire_L1ToL2 ) ;
            AccessNextLevel( Cache::L2, accessTime, StoreAddress, Cache::WRITE, wire_L1ToL2, m_Cache_list[ 0 ]->m_BlockSize ) ;
            delete[] wire_L1ToL2 ;
        }  // if

        bool isNeedToWriteBack = !m_Cache_list[ 0 ]->AllocateCache( set_index ) ;
        if ( isNeedToWriteBack ) {  // if write through and always false
            uint64_t StoreAddress ;
            m_Cache_list[ 0 ]->StoreCacheBlock( set_index, StoreAddress, wire_L1ToL2 ) ;
            AccessNextLevel( Cache::L2, accessTime, StoreAddress, Cache::WRITE, wire_L1ToL2, m_Cache_list[ 0 ]->m_BlockSize ) ;
            delete[] wire_L1ToL2 ;
        }  // if

        uint64_t LoadAddress = m_Cache_list[ 0 ]->TagToAddress( tag, set_index ) ;

#ifdef SIM_DATA
        Byte * wire_L2ToL1 = new Byte[ m_Cache_list[ 0 ]->m_BlockSize ] ;
#else
        Byte * wire_L2ToL1 = NULL ;
#endif
        AccessNextLevel( Cache::L2, accessTime, LoadAddress, Cache::READ, wire_L2ToL1, m_Cache_list[ 0 ]->m_BlockSize ) ;
        m_Cache_list[ 0 ]->LoadCacheBlock( tag, set_index, wire_L2ToL1 ) ;
        delete[] wire_L2ToL1 ;
        m_Cache_list[ 0 ]->AccessCache( AccessType, accessTime , address, Data, length, TimeUpWriteBack, wire_L1ToL2 ) ;
    }  // else

}  // MemSystem::CoreAccessMem()

void MemContoller::AccessNextLevel( uint32_t Cachetype, const uint64_t accessTime,  const uint64_t address, const uint32_t AccessType, Byte* Data,
        uint32_t length ) {

    if ( Cachetype == MAINMEM ) {
        // Read and Write data from MAINMEM
        /*
         Log::PrintDebugLog( "R/W from Memory address:" ) ;
         printf( "0x%016llX length: %d\n", ( long long ) address, length ) ;
         */
    }  // if
    else {
        bool TimeUpWriteBack = false ;
#ifdef SIM_DATA
        Byte * wire_L1ToL2 = new Byte[ m_Cache_list[ 0 ]->m_BlockSize ] ;
#else
        Byte * wire_L1ToL2 = NULL ;
#endif
        if ( m_Cache_list[ Cachetype ]->AccessCache( AccessType, accessTime , address, Data, length, TimeUpWriteBack, wire_L1ToL2 ) ) {  // hit
            if ( AccessType == Cache::WRITE ) {
                m_Cache_list[ Cachetype ]->m_Num_W_Access++ ;
                m_Cache_list[ Cachetype ]->m_Num_W_Hit++ ;
            }  // if
            else {
                m_Cache_list[ Cachetype ]->m_Num_R_Access++ ;
                m_Cache_list[ Cachetype ]->m_Num_R_Hit++ ;
            }  // else

             Log::PrintDebugLog( "CACHE L"+ std::to_string(Cachetype+1) + " hit  : address:" ) ;
             printf( "0x%016llX length: %d\n", ( long long ) address, length ) ;

        }  // if

        else {  // miss
            if ( AccessType == Cache::WRITE )
                m_Cache_list[ Cachetype ]->m_Num_W_Access++ ;
            else
                m_Cache_list[ Cachetype ]->m_Num_R_Access++ ;

            Log::PrintMessage( "here" + std::to_string(m_Cache_list[ Cachetype ]->m_Num_R_Access)) ;

             Log::PrintDebugLog( "CACHE L"+ std::to_string(Cachetype+1) + " miss : address:" ) ;
             printf( "0x%016llX length: %d\n", ( long long ) address, length ) ;


            uint64_t tag ;
            uint32_t set_index, block_offset ;
            m_Cache_list[ Cachetype ]->SplitAddress( address, tag, set_index, block_offset ) ;


            if ( TimeUpWriteBack ) {  // Time up write back
                uint64_t StoreAddress ;
                m_Cache_list[ Cachetype ]->StoreCacheBlock( set_index, StoreAddress, wire_L1ToL2 ) ;
                AccessNextLevel( Cachetype + 1 , accessTime, StoreAddress, Cache::WRITE, wire_L1ToL2, m_Cache_list[ Cachetype ]->m_BlockSize ) ;
                delete[] wire_L1ToL2 ;
            }  // if


            bool isNeedToWriteBack = !m_Cache_list[ Cachetype ]->AllocateCache( set_index ) ;
            if ( isNeedToWriteBack ) {  // if write through always false
                uint64_t StoreAddress ;
                m_Cache_list[ Cachetype ]->StoreCacheBlock( set_index, StoreAddress, wire_L1ToL2 ) ;
                AccessNextLevel( Cachetype + 1, accessTime, StoreAddress, Cache::WRITE, wire_L1ToL2, m_Cache_list[ Cachetype ]->m_BlockSize ) ;
                delete[] wire_L1ToL2 ;
            }  // if

            uint64_t LoadAddress = m_Cache_list[ 0 ]->TagToAddress( tag, set_index ) ;

#ifdef SIM_DATA
            Byte * wire_L2ToL1 = new Byte[ m_Cache_list[ 0 ]->m_BlockSize ] ;
#else
            Byte * wire_L2ToL1 = NULL ;
#endif
            AccessNextLevel( Cachetype + 1, accessTime, LoadAddress, Cache::READ, wire_L2ToL1, m_Cache_list[ Cachetype ]->m_BlockSize ) ;

            m_Cache_list[ Cachetype ]->LoadCacheBlock( tag, set_index, wire_L2ToL1 ) ;
            delete[] wire_L2ToL1 ;
            m_Cache_list[ Cachetype ]->AccessCache( AccessType, accessTime , address, Data, length, TimeUpWriteBack, wire_L1ToL2 ) ;

        }  // else

    }  // else

}  // MemSystem::AccessNextLevel()

void MemContoller::CacheMigrationContrl() {


}  // MemContoller::CacheMigrationContrl

