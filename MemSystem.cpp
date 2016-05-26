#include "MemSystem.h"
#include "Log.h"
#include <sstream>
MemSystem::MemSystem( const char * cfg ) {
    cache_list = NULL ;
    cfgparser = new CfgParser( cfg ) ;
    m_Cache_level = 0 ;
    CreateMemSystem() ;
}  // MemSystem::MemSystem()

void MemSystem::CreateMemSystem() {
    m_Cache_level = cfgparser->ParseDevice( "cache_level", "level" ) ;
    cache_list = new Cache*[ m_Cache_level ] ;
    for ( int i = 0; i < m_Cache_level; i++ ) {
        if ( i == Cache::L1_D ) {
            cache_list[ i ] =
                    new Cache( Cache::L1_D, cfgparser->ParseDevice( "cache_l1_D", "size" ), cfgparser->ParseDevice( "cache_l1_D", "blocksize" ), cfgparser->ParseDevice( "cache_l2", "associativity" ), cfgparser->ParseDevice( "cache_l1_D", "replacepolicy" ), cfgparser->ParseDevice( "cache_l1_D", "writepolicy" ) ) ;
            Log::PrintMessage( "Create cache: cache_l1_D\tsize: " + std::to_string( cache_list[ i ]->m_CacheSize >> 10 )
                    + "\tblocksize:" + std::to_string( cache_list[ i ]->m_BlockSize ) ) ;
        }   // if

        else if ( i == Cache::L2 ) {
            cache_list[ i ] =
                    new Cache( Cache::L2, cfgparser->ParseDevice( "cache_l2", "size" ), cfgparser->ParseDevice( "cache_l2", "blocksize" ), cfgparser->ParseDevice( "cache_l2", "associativity" ), cfgparser->ParseDevice( "cache_l2", "replacepolicy" ), cfgparser->ParseDevice( "cache_l2", "writepolicy" ) ) ;
            Log::PrintMessage( "Create cache: cache_l2\t\tsize: " + std::to_string( cache_list[ i ]->m_CacheSize >> 10 )
                    + "\tblocksize:" + std::to_string( cache_list[ i ]->m_BlockSize ) ) ;

        }  // else if

        else if ( i == Cache::L3 ) {
            cache_list[ i ] =
                    new Cache( Cache::L3, cfgparser->ParseDevice( "cache_l3", "size" ), cfgparser->ParseDevice( "cache_l3", "blocksize" ), cfgparser->ParseDevice( "cache_l3", "associativity" ), cfgparser->ParseDevice( "cache_l3", "replacepolicy" ), cfgparser->ParseDevice( "cache_l3", "writepolicy" ) ) ;
            Log::PrintMessage( "Create cache: cache_l3\t\tsize: " + std::to_string( cache_list[ i ]->m_CacheSize >> 10 )
                    + "\tblocksize:" + std::to_string( cache_list[ i ]->m_BlockSize ) ) ;

        }  // else if
    }  // for ()

}  // MemSystem::CreateMemSystem

void MemSystem::CoreAccessMem( const uint64_t address, const uint32_t AccessType, Byte* Data, uint32_t length ) {

    if ( cache_list[ 0 ]->AccessCache( AccessType, address, Data, length ) ) {  // hit
        cache_list[ 0 ]->m_Num_Access++ ;
        cache_list[ 0 ]->m_Num_Hit++ ;
        Log::PrintDebugLog( "CACHE L1 hit  : address:" ) ;
        printf( "0x%016llX length: %d\n", ( long long ) address, length ) ;

    }  // if

    else {  // miss
        cache_list[ 0 ]->m_Num_Access++ ;
        Log::PrintDebugLog( "CACHE L1 miss : address:" ) ;
        printf( "0x%016llX length: %d\n", ( long long ) address, length ) ;
        uint64_t tag ;
        uint32_t set_index, block_offset ;
        cache_list[ 0 ]->SplitAddress( address, tag, set_index, block_offset ) ;

        bool isNeedToWriteBack = !cache_list[ 0 ]->AllocateCache( set_index ) ;
        if ( isNeedToWriteBack ) {  // if write through always false
#ifdef SIM_DATA
                Byte * wire_L1ToL2 = new Byte[ cache_list[ 0 ]->m_BlockSize ] ;
#else
                Byte * wire_L1ToL2 = NULL ;
#endif

            uint64_t StoreAddress ;
            cache_list[ 0 ]->StoreCacheBlock( tag, set_index, StoreAddress, wire_L1ToL2 ) ;
            AccessNextLevel( Cache::L2, StoreAddress, Cache::WRITE, wire_L1ToL2, cache_list[ 0 ]->m_BlockSize ) ;
            delete[] wire_L1ToL2 ;
        }  // if

        uint64_t LoadAddress = cache_list[ 0 ]->TagToAddress( tag, set_index ) ;

#ifdef SIM_DATA
        Byte * wire_L2ToL1 = new Byte[ cache_list[ 0 ]->m_BlockSize ] ;
#else
                Byte * wire_L2ToL1  = NULL ;
#endif
        AccessNextLevel( Cache::L2, LoadAddress, Cache::READ, wire_L2ToL1, cache_list[ 0 ]->m_BlockSize ) ;
        cache_list[ 0 ]->LoadCacheBlock( tag, set_index, wire_L2ToL1 ) ;
        delete[] wire_L2ToL1 ;
        cache_list[ 0 ]->AccessCache( AccessType, address, Data, length ) ;
    }  // else

}  // MemSystem::CoreAccessMem()

void MemSystem::AccessNextLevel( uint32_t Cachetype, const uint64_t address, const uint32_t AccessType, Byte* Data,
        uint32_t length ) {

    if ( Cachetype == MAINMEM ) {
        // Read and Write data from MAINMEM

        Log::PrintDebugLog( "R/W from Memory address:" ) ;
        printf( "0x%016llX length: %d\n", ( long long ) address, length ) ;

    }  // if
    else {

        if ( cache_list[ Cachetype ]->AccessCache( AccessType, address, Data, length ) ) {  // hit
            cache_list[ Cachetype ]->m_Num_Access++ ;
            cache_list[ Cachetype ]->m_Num_Hit++ ;
            Log::PrintDebugLog( "CACHE L"+ std::to_string(Cachetype+1) + " hit  : address:" ) ;
            printf( "0x%016llX length: %d\n", ( long long ) address, length ) ;

        }  // if

        else {  // miss
            cache_list[ Cachetype ]->m_Num_Access++ ;
            Log::PrintDebugLog( "CACHE L"+ std::to_string(Cachetype+1) + " miss : address:" ) ;
            printf( "0x%016llX length: %d\n", ( long long ) address, length ) ;
            uint64_t tag ;
            uint32_t set_index, block_offset ;
            cache_list[ Cachetype ]->SplitAddress( address, tag, set_index, block_offset ) ;

            bool isNeedToWriteBack = !cache_list[ Cachetype ]->AllocateCache( set_index ) ;
            if ( isNeedToWriteBack ) {  // if write through always false
#ifdef SIM_DATA
                Byte * wire_L1ToL2 = new Byte[ cache_list[ Cachetype ]->m_BlockSize ] ;
#else
                Byte * wire_L1ToL2 = new Byte[ cache_list[ Cachetype ]->m_BlockSize ] ;
#endif
                uint64_t StoreAddress ;
                cache_list[ Cachetype ]->StoreCacheBlock( tag, set_index, StoreAddress, wire_L1ToL2 ) ;
                AccessNextLevel( Cachetype+1, StoreAddress, Cache::WRITE, wire_L1ToL2, cache_list[ Cachetype ]->m_BlockSize ) ;
                delete[] wire_L1ToL2 ;

            }  // if

            uint64_t LoadAddress = cache_list[ 0 ]->TagToAddress( tag, set_index ) ;

#ifdef SIM_DATA
        Byte * wire_L2ToL1 = new Byte[ cache_list[ 0 ]->m_BlockSize ] ;
#else
                Byte * wire_L2ToL1  = NULL ;
#endif
            AccessNextLevel( Cachetype+1, LoadAddress, Cache::READ, wire_L2ToL1, cache_list[ Cachetype ]->m_BlockSize ) ;

            cache_list[ Cachetype ]->LoadCacheBlock( tag, set_index, wire_L2ToL1 ) ;
            delete[] wire_L2ToL1 ;
            cache_list[ Cachetype ]->AccessCache( AccessType, address, Data, length ) ;

        }  // else

    }  // else

}  // MemSystem::AccessNextLevel()

