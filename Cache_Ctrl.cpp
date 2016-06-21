/*
 * Cache_Ctrl.cpp
 *
 *  Created on: Jun 20, 2016
 *      Author: LuoJing-Yuan
 */
#include "Cache_Ctrl.h"
#include "Log.h"
Cache_Ctrl::Cache_Ctrl( Cache* thisLevel ) :
        mThis( thisLevel ) {

    mThisCacheType = mThis->m_CacheType ;
    mNextLevelCacheType = -1 ;
    mNextLevel = NULL ;

}  // Cache_Ctrl()

void Cache_Ctrl::SetNextLevelCacheCtrl( Cache_Ctrl * nextLevelCacheCtrl ) {

    mNextLevel = nextLevelCacheCtrl ;
    if ( mNextLevel == NULL )
        mNextLevelCacheType = Cache::MAINMEM ;
    else
        mNextLevelCacheType = mNextLevel->mThisCacheType ;

}  // Cache_Ctrl::SetNextLevelCacheCtrl()

void Cache_Ctrl::Access( const uint64_t accessTime, const uint64_t address, const uint32_t AccessType, Byte * Data,
        uint32_t length ) {

    bool TimeUpWriteBack = false ;
#ifdef SIM_DATA
    Byte * wire_L1ToL2 = new Byte[ mThis->m_BlockSize ] ;
#else
    Byte * wire_L1ToL2 = NULL ;
#endif

    if ( mThis->AccessCache( AccessType, accessTime, address, Data, length, TimeUpWriteBack, wire_L1ToL2 ) ) {  //if true cache  hit
        if ( AccessType == Cache::WRITE ) {
            mThis->m_Num_W_Access++ ;
            mThis->m_Num_W_Hit++ ;
        }  // if
        else {
            mThis->m_Num_R_Access++ ;
            mThis->m_Num_R_Hit++ ;
        }  // else

        /*
         Log::PrintDebugLog( "CACHE L1 hit  : address:" ) ;
         printf( "0x%016llX length: %d\n", ( long long ) address, length ) ;
         */
    }  // if

    else {  // cache miss
        if ( AccessType == Cache::WRITE )
            mThis->m_Num_W_Access++ ;
        else
            mThis->m_Num_R_Access++ ;

        /*
         Log::PrintDebugLog( "CACHE L1 miss : address:" ) ;
         printf( "0x%016llX length: %d\n", ( long long ) address, length ) ;
         */

        // To do retention time valid condition
        uint64_t tag ;
        uint32_t set_index, block_offset ;
        mThis->SplitAddress( address, tag, set_index, block_offset ) ;

        if ( TimeUpWriteBack ) {  // Time up write back
            uint64_t StoreAddress ;
            mThis->StoreCacheBlock( set_index, StoreAddress, wire_L1ToL2 ) ;
            if ( mNextLevelCacheType != Cache::MAINMEM )
                mNextLevel->Access( accessTime, StoreAddress, Cache::WRITE, wire_L1ToL2, mThis->m_BlockSize ) ;
            delete[] wire_L1ToL2 ;
        }  // if

        // To do allocate from cache, and evicted, write back to next level

        bool isNeedToWriteBack = !mThis->AllocateCache( set_index ) ;
        if ( isNeedToWriteBack ) {  // if write through and always false
            uint64_t StoreAddress ;
            mThis->StoreCacheBlock( set_index, StoreAddress, wire_L1ToL2 ) ;
            if ( mNextLevelCacheType != Cache::MAINMEM )
                mNextLevel->Access( accessTime, StoreAddress, Cache::WRITE, wire_L1ToL2, mThis->m_BlockSize ) ;
            delete[] wire_L1ToL2 ;
        }  // if

        // load data from next level
        uint64_t LoadAddress = mThis->TagToAddress( tag, set_index ) ;

#ifdef SIM_DATA
        Byte * wire_L2ToL1 = new Byte[ mThis->>m_BlockSize ] ;
#else
        Byte * wire_L2ToL1 = NULL ;
#endif

        if ( mNextLevelCacheType != Cache::MAINMEM )
            mNextLevel->Access( accessTime, LoadAddress, Cache::READ, wire_L2ToL1, mThis->m_BlockSize ) ;
        mThis->LoadCacheBlock( tag, set_index, wire_L2ToL1 ) ;
        delete[] wire_L2ToL1 ;

        // To normal access
        mThis->AccessCache( AccessType, accessTime, address, Data, length, TimeUpWriteBack, wire_L1ToL2 ) ;
    }  // else

}  // Cache_Ctrl::Access()
