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
            cache_list[ i ] = new Cache( Cache::L1_D,
                    cfgparser->ParseDevice( "cache_l1_D", "size" ),
                    cfgparser->ParseDevice( "cache_l1_D", "blocksize" ),
                    cfgparser->ParseDevice( "cache_l1_D", "replacepolicy" ),
                    cfgparser->ParseDevice( "cache_l1_D", "writepolicy" ) ) ;
            Log::PrintMessage(
                    "Create cache: cache_l1_D\tsize: "
                            + std::to_string( cache_list[ i ]->m_CacheSize >> 10 ) + "\tblocksize:"
                            + std::to_string( cache_list[ i ]->m_BlockSize ) ) ;
        }   // if

        else if ( i == Cache::L2 ) {
            cache_list[ i ] = new Cache( Cache::L2, cfgparser->ParseDevice( "cache_l2", "size" ),
                    cfgparser->ParseDevice( "cache_l2", "blocksize" ),
                    cfgparser->ParseDevice( "cache_l2", "associativity" ),
                    cfgparser->ParseDevice( "cache_l2", "replacepolicy" ),
                    cfgparser->ParseDevice( "cache_l2", "writepolicy" ) ) ;
            Log::PrintMessage(
                    "Create cache: cache_l2\t\tsize: "
                            + std::to_string( cache_list[ i ]->m_CacheSize >> 10 ) + "\tblocksize:"
                            + std::to_string( cache_list[ i ]->m_BlockSize ) ) ;

        }  // else if

        else if ( i == Cache::L3 ) {
            cache_list[ i ] = new Cache( Cache::L3, cfgparser->ParseDevice( "cache_l3", "size" ),
                    cfgparser->ParseDevice( "cache_l3", "blocksize" ),
                    cfgparser->ParseDevice( "cache_l3", "associativity" ),
                    cfgparser->ParseDevice( "cache_l3", "replacepolicy" ),
                    cfgparser->ParseDevice( "cache_l3", "writepolicy" ) ) ;
            Log::PrintMessage(
                    "Create cache: cache_l3\t\tsize: "
                            + std::to_string( cache_list[ i ]->m_CacheSize >> 10 ) + "\tblocksize:"
                            + std::to_string( cache_list[ i ]->m_BlockSize ) ) ;

        }  // else if
    }  // for ()

}  // MemSystem::CreateMemSystem

void MemSystem::CoreAccessMem( const uint64_t address, const uint32_t AccessType, Byte* Data,
        uint32_t length ) {

    if ( cache_list[ 0 ]->PeekSingleLine( address ) ) {  // hit
        cache_list[ 0 ]->MaintainReplacePolicy( address ) ;
        cache_list[ 0 ]->AccessSingleLine( true, AccessType, address, Data, 8 ) ;

        Log::PrintDebugLog( "CACHE L1 hit : address:" ) ;
        printf( "%llX length: %d\n", ( long long ) address, length ) ;

    }  // if

    else {  // miss
        Log::PrintDebugLog( "CACHE L1 miss : address:" ) ;
        printf( "%llX length: %d\n", ( long long ) address, length ) ;

        uint32_t set_index = cache_list[ 0 ]->MaintainReplacePolicy( address ) ;
        Cache_Set * cache_set = cache_list[ 0 ]->GetCacheSet( set_index ) ;
        bool isNeedToWriteBack = cache_list[ 0 ]->ReplaceSet( set_index, address ) ;

        if ( isNeedToWriteBack ) {
            uint64_t WB_Addr = cache_list[ 0 ]->AppendAddress( cache_set ) ;
            AccessNextLevel( Cache::L2, WB_Addr, MemSystem::WRITE, cache_set->GetSetData(),
                    cache_list[ 0 ]->m_BlockSize ) ;
        }  // if

        AccessNextLevel( Cache::L2, address, MemSystem::READ, cache_set->GetSetData(),
                cache_list[ 0 ]->m_BlockSize ) ;

        // AccessNextLevel( Cache::L2, address, MemSystem::READ,NULL, cache_list[0]->m_BlockSize ) ;

        cache_list[ 0 ]->AccessSingleLine( false, AccessType, address, Data, 8 ) ;

    }  // else

}  // MemSystem::CoreAccessMem()

void MemSystem::AccessNextLevel( uint32_t Cachetype, const uint64_t address,
        const uint32_t AccessType, Byte* Data, uint32_t length ) {

    if ( Cachetype == MAINMEM ) {
        // Read and Write data from MAINMEM

        Log::PrintDebugLog( "Read and Write from Mem address:" ) ;
        printf( "%llX length: %d\n", ( long long ) address, length ) ;

    }  // if
    else {

        if ( cache_list[ Cachetype ]->PeekSingleLine( address ) ) {  // hit
            cache_list[ Cachetype ]->MaintainReplacePolicy( address ) ;
            cache_list[ Cachetype ]->AccessSingleLine( true, AccessType, address, Data,
                    cache_list[ Cachetype - 1 ]->m_BlockSize ) ;

            Log::PrintDebugLog( "CACHE L" + std::to_string( Cachetype + 1 ) + " hit : address:" ) ;
            printf( "%llX length: %d\n", ( long long ) address, length ) ;

        }  // if

        else {  // miss

            Log::PrintDebugLog( "CACHE L" + std::to_string( Cachetype + 1 ) + " miss : address:" ) ;
            printf( "%llX length: %d\n", ( long long ) address, length ) ;

            uint32_t set_index = cache_list[ Cachetype ]->MaintainReplacePolicy( address ) ;
            Cache_Set * cache_set = cache_list[ Cachetype ]->GetCacheSet( set_index ) ;
            bool isNeedToWriteBack = cache_list[ Cachetype ]->ReplaceSet( set_index, address ) ;

            if ( isNeedToWriteBack ) {
                uint64_t WB_Addr = cache_list[ Cachetype ]->AppendAddress( cache_set ) ;
                AccessNextLevel( Cachetype + 1, WB_Addr, MemSystem::WRITE, cache_set->GetSetData(),
                        cache_list[ Cachetype ]->m_BlockSize ) ;
            }  // if

            AccessNextLevel( Cachetype + 1, address, MemSystem::READ, cache_set->GetSetData(),
                    cache_list[ Cachetype ]->m_BlockSize ) ;

            cache_list[ Cachetype ]->AccessSingleLine( false, AccessType, address, Data,
                    cache_list[ Cachetype - 1 ]->m_BlockSize ) ;

        }  // else
    }  // else

}  // MemSystem::AccessNextLevel()

