/*
 * Cache_Ctrl.cpp
 *
 *  Created on: Jun 20, 2016
 *      Author: LuoJing-Yuan
 */
#include "Cache_Ctrl.h"
#include "Log.h"
Cache_Ctrl::Cache_Ctrl( Cache* thisLevel, uint8_t cachetype ) :
        mThis( thisLevel ), mThisCacheType( cachetype ) {

    mThisCacheName = mThis->m_Name ;
    mNextLevelCacheName = -1 ;
    mNextLevel = NULL ;

}  // Cache_Ctrl()

void Cache_Ctrl::SetNextLevelCacheCtrl( Cache_Ctrl * nextLevelCacheCtrl ) {

    mNextLevel = nextLevelCacheCtrl ;
    if ( mNextLevel == NULL )
        mNextLevelCacheName = Cache::MAINMEM ;
    else
        mNextLevelCacheName = mNextLevel->mThisCacheName ;

}  // Cache_Ctrl::SetNextLevelCacheCtrl()

void Cache_Ctrl::Access( const uint64_t accessTime, const uint64_t address, const uint32_t AccessType, Byte * Data,
        uint32_t length ) {

    if ( mThisCacheType == Cache::CACHE ) {
        if ( mThis->AccessCache( AccessType, accessTime, address, Data, length ) ) {  //if true cache  hit
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

            uint64_t tag ;
            uint32_t set_index, block_offset ;
            mThis->SplitAddress( address, tag, set_index, block_offset ) ;

            // To do allocate from cache, and evicted, write back to next level
#ifdef SIM_DATA
            Byte * wire_L1ToL2 = new Byte[ mThis->m_BlockSize ] ;
#else
            Byte * wire_L1ToL2 = NULL ;
#endif

            bool isNeedToWriteBack = !mThis->AllocateCache( set_index ) ;
            if ( isNeedToWriteBack ) {  // if write through and always false
                uint64_t StoreAddress ;
                mThis->StoreCacheBlock( set_index, StoreAddress, wire_L1ToL2 ) ;
                if ( mNextLevelCacheName != Cache::MAINMEM )
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

            if ( mNextLevelCacheName != Cache::MAINMEM )
                mNextLevel->Access( accessTime, LoadAddress, Cache::READ, wire_L2ToL1, mThis->m_BlockSize ) ;
            mThis->LoadCacheBlock( tag, set_index, wire_L2ToL1 ) ;
            delete[] wire_L2ToL1 ;

            // To normal access
            mThis->AccessCache( AccessType, accessTime, address, Data, length ) ;
        }  // else
    }  // if  normal Cache

    else if ( mThisCacheType == Cache::HYBRIDCACHE ) {
        bool TimeUpWriteBack = false ;
#ifdef SIM_DATA
        Byte * wire_L1ToL2 = new Byte[ mThis->m_BlockSize ] ;
#else
        Byte * wire_L1ToL2 = NULL ;
#endif

        if ( mThis->AccessHybridCache( AccessType, accessTime, address, Data, length, TimeUpWriteBack, wire_L1ToL2 ) ) {  //if true cache  hit
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
                if ( mNextLevelCacheName != Cache::MAINMEM )
                    mNextLevel->Access( accessTime, StoreAddress, Cache::WRITE, wire_L1ToL2, mThis->m_BlockSize ) ;
                delete[] wire_L1ToL2 ;
            }  // if

            // To do allocate from cache, and evicted, write back to next level

            bool isNeedToWriteBack = !mThis->AllocateCache( set_index ) ;
            if ( isNeedToWriteBack ) {  // if write through and always false
                uint64_t StoreAddress ;
                mThis->StoreCacheBlock( set_index, StoreAddress, wire_L1ToL2 ) ;
                if ( mNextLevelCacheName != Cache::MAINMEM )
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

            if ( mNextLevelCacheName != Cache::MAINMEM )
                mNextLevel->Access( accessTime, LoadAddress, Cache::READ, wire_L2ToL1, mThis->m_BlockSize ) ;
            mThis->LoadCacheBlock( tag, set_index, wire_L2ToL1 ) ;
            delete[] wire_L2ToL1 ;

            // To normal access
            mThis->AccessHybridCache( AccessType, accessTime, address, Data, length, TimeUpWriteBack, wire_L1ToL2 ) ;
        }  // else
    }  // else if Cache::HYBRIDCACHE

    else if ( mThisCacheType == Cache::BUFFERCACHE ) {
        std::vector < uint8_t > * list = new std::vector < uint8_t > ;
        // move request from buffer to cache
        list = mThis->mBufferCache->GetRequestToCache( list, accessTime ) ;

        for ( int i = 0; i < list->size(); i++ ) {  // do the request which need to finish.
            BufferCache::BufferSet * op = mThis->mBufferCache->mBufferSet[ list->at( i ) ] ;
            if ( mThis->AccessCache( Cache::WRITE, op->mAccessTime, op->mAddress, op->mData, mThis->mBufferCache->mDataLength ) ) {
                mThis->m_Num_W_Access++ ;
                mThis->m_Num_W_Hit++ ;
            }  // if

            else {
                mThis->m_Num_W_Access++ ;

                uint64_t tag ;
                uint32_t set_index, block_offset ;
                mThis->SplitAddress( op->mAddress, tag, set_index, block_offset ) ;

                // To do allocate from cache, and evicted, write back to next level
#ifdef SIM_DATA
                Byte * wire_L1ToL2 = new Byte[ mThis->m_BlockSize ] ;
#else
                Byte * wire_L1ToL2 = NULL ;
#endif

                bool isNeedToWriteBack = !mThis->AllocateCache( set_index ) ;
                if ( isNeedToWriteBack ) {  // if write through and always false
                    uint64_t StoreAddress ;
                    mThis->StoreCacheBlock( set_index, StoreAddress, wire_L1ToL2 ) ;
                    if ( mNextLevelCacheName != Cache::MAINMEM )
                        mNextLevel->Access( op->mAccessTime, StoreAddress, Cache::WRITE, wire_L1ToL2, mThis->m_BlockSize ) ;
                    delete[] wire_L1ToL2 ;
                }  // if

                // load data from next level
                uint64_t LoadAddress = mThis->TagToAddress( tag, set_index ) ;

#ifdef SIM_DATA
                Byte * wire_L2ToL1 = new Byte[ mThis->>m_BlockSize ] ;
#else
                Byte * wire_L2ToL1 = NULL ;
#endif

                if ( mNextLevelCacheName != Cache::MAINMEM )
                    mNextLevel->Access( op->mAccessTime, LoadAddress, Cache::READ, wire_L2ToL1, mThis->m_BlockSize ) ;
                mThis->LoadCacheBlock( tag, set_index, wire_L2ToL1 ) ;
                delete[] wire_L2ToL1 ;

                // To normal access
                mThis->AccessCache( Cache::WRITE, op->mAccessTime, op->mAddress, op->mData, mThis->mBufferCache->mDataLength ) ;
            }  // else
        }  // for  Do list loop

        delete list ;

        // Access buffer

        if ( mThis->mBufferCache->BufferAccess( accessTime, address, AccessType, Data, length ) ) {

            // buffer hit
            if ( AccessType == Cache::WRITE ) {
                mThis->mBufferCache->m_Num_W_Access++ ;
                mThis->mBufferCache->m_Num_W_Hit++ ;

            }  // if
            else {
                mThis->mBufferCache->m_Num_R_Access++ ;
                mThis->mBufferCache->m_Num_R_Hit++ ;
            }  // else
        }  // if

        else {  // buffer  miss
            if ( AccessType == Cache::READ ) {
                mThis->mBufferCache->m_Num_R_Access++ ;
                if ( mThis->AccessCache( AccessType, accessTime, address, Data, length ) ) {  //if true cache  hit
                    mThis->m_Num_R_Access++ ;
                    mThis->m_Num_R_Hit++ ;
                }  // if
                else {  // Read miss
                    mThis->m_Num_R_Access++ ;

                    uint64_t tag ;
                    uint32_t set_index, block_offset ;
                    mThis->SplitAddress( address, tag, set_index, block_offset ) ;

                    // To do allocate from cache, and evicted, write back to next level
#ifdef SIM_DATA
                    Byte * wire_L1ToL2 = new Byte[ mThis->m_BlockSize ] ;
#else
                    Byte * wire_L1ToL2 = NULL ;
#endif

                    bool isNeedToWriteBack = !mThis->AllocateCache( set_index ) ;
                    if ( isNeedToWriteBack ) {  // if write through and always false
                        uint64_t StoreAddress ;
                        mThis->StoreCacheBlock( set_index, StoreAddress, wire_L1ToL2 ) ;
                        if ( mNextLevelCacheName != Cache::MAINMEM )
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

                    if ( mNextLevelCacheName != Cache::MAINMEM )
                        mNextLevel->Access( accessTime, LoadAddress, Cache::READ, wire_L2ToL1, mThis->m_BlockSize ) ;
                    mThis->LoadCacheBlock( tag, set_index, wire_L2ToL1 ) ;
                    delete[] wire_L2ToL1 ;

                    // To normal access
                    mThis->AccessCache( AccessType, accessTime, address, Data, length ) ;
                }  // else

            }  // if
            else if ( AccessType == Cache::WRITE ) {
                mThis->mBufferCache->m_Num_W_Access++ ;
                // if buffer is full add latency
                bool isBufferFull = false ;
                uint32_t Addtionlatency = 0 ;
                uint8_t pushindex =
                        mThis->mBufferCache->AllocatBufferEntry( accessTime, Addtionlatency, isBufferFull ) ;
                if ( isBufferFull ) {
                    BufferCache::BufferSet * op = mThis->mBufferCache->mBufferSet[ pushindex ] ;
                    mThis->mBufferCache->mAddtionWriteLantency += Addtionlatency ;

                    if ( mThis->AccessCache( Cache::WRITE, op->mAccessTime, op->mAddress, op->mData, mThis->mBufferCache->mDataLength ) ) {
                        mThis->m_Num_W_Access++ ;
                        mThis->m_Num_W_Hit++ ;
                    }  // if

                    else {
                        mThis->m_Num_W_Access++ ;

                        uint64_t tag ;
                        uint32_t set_index, block_offset ;
                        mThis->SplitAddress( op->mAddress, tag, set_index, block_offset ) ;

                        // To do allocate from cache, and evicted, write back to next level
#ifdef SIM_DATA
                        Byte * wire_L1ToL2 = new Byte[ mThis->m_BlockSize ] ;
#else
                        Byte * wire_L1ToL2 = NULL ;
#endif

                        bool isNeedToWriteBack = !mThis->AllocateCache( set_index ) ;
                        if ( isNeedToWriteBack ) {  // if write through and always false
                            uint64_t StoreAddress ;
                            mThis->StoreCacheBlock( set_index, StoreAddress, wire_L1ToL2 ) ;
                            if ( mNextLevelCacheName != Cache::MAINMEM )
                                mNextLevel->Access( op->mAccessTime, StoreAddress, Cache::WRITE, wire_L1ToL2, mThis->m_BlockSize ) ;
                            delete[] wire_L1ToL2 ;
                        }  // if

                        // load data from next level
                        uint64_t LoadAddress = mThis->TagToAddress( tag, set_index ) ;

#ifdef SIM_DATA
                        Byte * wire_L2ToL1 = new Byte[ mThis->>m_BlockSize ] ;
#else
                        Byte * wire_L2ToL1 = NULL ;
#endif

                        if ( mNextLevelCacheName != Cache::MAINMEM )
                            mNextLevel->Access( op->mAccessTime, LoadAddress, Cache::READ, wire_L2ToL1, mThis->m_BlockSize ) ;
                        mThis->LoadCacheBlock( tag, set_index, wire_L2ToL1 ) ;
                        delete[] wire_L2ToL1 ;

                        // To normal access
                        mThis->AccessCache( Cache::WRITE, op->mAccessTime, op->mAddress, op->mData, mThis->mBufferCache->mDataLength ) ;
                    }  // else
                }  // if
                mThis->mBufferCache->PushRequestToBuffer( pushindex, accessTime, address, Data ) ;

            }  //else if
        }   // else  buffer miss
    }  // else if Type is BufferCache

}  // Cache_Ctrl::Access()

void Cache_Ctrl::FlushOperationInBufferCache() {
    int size = mThis->mBufferCache->mBufferQueue->size() ;
    if ( size == 0 )
        return ;
    uint64_t accesstime =
            mThis->mBufferCache->mBufferSet[ mThis->mBufferCache->mBufferQueue->at( size - 1 ) ]->mAccessTime ;
    accesstime += 50 ;

    std::vector < uint8_t > * list = new std::vector < uint8_t > ;
    // move request from buffer to cache
    list = mThis->mBufferCache->GetRequestToCache( list, accesstime ) ;

    for ( int i = 0; i < list->size(); i++ ) {  // do the request which need to finish.
        BufferCache::BufferSet * op = mThis->mBufferCache->mBufferSet[ list->at( i ) ] ;
        if ( mThis->AccessCache( Cache::WRITE, op->mAccessTime, op->mAddress, op->mData, mThis->mBufferCache->mDataLength ) ) {
            mThis->m_Num_W_Access++ ;
            mThis->m_Num_W_Hit++ ;
        }  // if

        else {
            mThis->m_Num_W_Access++ ;

            uint64_t tag ;
            uint32_t set_index, block_offset ;
            mThis->SplitAddress( op->mAddress, tag, set_index, block_offset ) ;

            // To do allocate from cache, and evicted, write back to next level
#ifdef SIM_DATA
            Byte * wire_L1ToL2 = new Byte[ mThis->m_BlockSize ] ;
#else
            Byte * wire_L1ToL2 = NULL ;
#endif

            bool isNeedToWriteBack = !mThis->AllocateCache( set_index ) ;
            if ( isNeedToWriteBack ) {  // if write through and always false
                uint64_t StoreAddress ;
                mThis->StoreCacheBlock( set_index, StoreAddress, wire_L1ToL2 ) ;
                if ( mNextLevelCacheName != Cache::MAINMEM )
                    mNextLevel->Access( op->mAccessTime, StoreAddress, Cache::WRITE, wire_L1ToL2, mThis->m_BlockSize ) ;
                delete[] wire_L1ToL2 ;
            }  // if

            // load data from next level
            uint64_t LoadAddress = mThis->TagToAddress( tag, set_index ) ;

#ifdef SIM_DATA
            Byte * wire_L2ToL1 = new Byte[ mThis->>m_BlockSize ] ;
#else
            Byte * wire_L2ToL1 = NULL ;
#endif

            if ( mNextLevelCacheName != Cache::MAINMEM )
                mNextLevel->Access( op->mAccessTime, LoadAddress, Cache::READ, wire_L2ToL1, mThis->m_BlockSize ) ;
            mThis->LoadCacheBlock( tag, set_index, wire_L2ToL1 ) ;
            delete[] wire_L2ToL1 ;

            // To normal access
            mThis->AccessCache( Cache::WRITE, op->mAccessTime, op->mAddress, op->mData, mThis->mBufferCache->mDataLength ) ;
        }  // else
    }  // for  Do list loop

    delete list ;

}

