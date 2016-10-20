/*
 * Cache_Ctrl.cpp
 *
 *  Created on: Jun 20, 2016
 *      Author: LuoJing-Yuan
 */
#include "Cache_Ctrl.h"
#include "Log.h"
CS::Cache_Ctrl::Cache_Ctrl( Base* thisLevel, uint8_t cachetype ) :
        mThis( thisLevel ), mThisCacheType( cachetype ) {

    mThisCacheName = ((Cache*)mThis)->m_Name ;
    mNextLevelCacheName = -1 ;
    mNextLevel = NULL ;

}  // Cache_Ctrl()

void CS::Cache_Ctrl::SetNextLevelCacheCtrl( Cache_Ctrl * nextLevelCacheCtrl ) {

    mNextLevel = nextLevelCacheCtrl ;
    if ( mNextLevel == NULL )
        mNextLevelCacheName = CS::MEM_NAME::MAINMEM ;
    else
        mNextLevelCacheName = ((Cache_Ctrl*)mNextLevel)->mThisCacheName ;

}  // Cache_Ctrl::SetNextLevelCacheCtrl()

void CS::Cache_Ctrl::Access( const uint64_t accessTime, const uint64_t address, const uint32_t AccessType, Byte * Data,
        uint32_t length ) {

    if ( mThisCacheType == CS::CACHETYPE::NORMAL ) {
        uint8_t hit = ((Cache*)mThis)->AccessCache( AccessType, accessTime, address, Data, length ) ;
        if ( hit == CS::CACHEHITTYPE::HIT  ) {  //if true cache  hit
            if ( ((Cache*)mThis)->mEnableRecord ) {
              if ( AccessType == CS::ACCESSTYPE::WRITE ) {
                  ((Cache*)mThis)->m_Num_W_Access++ ;
                  ((Cache*)mThis)->m_Num_W_Hit++ ;
              }  // if
              else {
                  ((Cache*)mThis)->m_Num_R_Access++ ;
                  ((Cache*)mThis)->m_Num_R_Hit++ ;
              }  // else
            }  // if
            /*
             Log::PrintDebugLog( "CACHE L1 hit  : address:" ) ;
             printf( "0x%016llX length: %d\n", ( long long ) address, length ) ;
             */
        }  // if

        else {  // cache miss
            if ( ((Cache*)mThis)->mEnableRecord ) {
              if ( AccessType == CS::ACCESSTYPE::WRITE )
                  ((Cache*)mThis)->m_Num_W_Access++ ;
              else
                  ((Cache*)mThis)->m_Num_R_Access++ ;
            } // if
            /*
             Log::PrintDebugLog( "CACHE L1 miss : address:" ) ;
             printf( "0x%016llX length: %d\n", ( long long ) address, length ) ;
             */

            uint64_t tag ;
            uint32_t set_index, block_offset ;
            ((Cache*)mThis)->SplitAddress( address, tag, set_index, block_offset ) ;

            // To do allocate from cache, and evicted, write back to next level
#ifdef SIM_DATA
            Byte * wire_MToS = new Byte[ ((Cache*)mThis)->m_BlockSize ] ;
#else
            Byte * wire_MToS = NULL ;
#endif

            bool isNeedToWriteBack = !((Cache*)mThis)->AllocateCache( set_index ) ;
            if ( isNeedToWriteBack ) {  // if write through and always false
                uint64_t StoreAddress ;
                ((Cache*)mThis)->StoreCacheBlock( set_index, StoreAddress, wire_MToS ) ;
                if ( mNextLevelCacheName != CS::MEM_NAME::MAINMEM )
                    ((Cache_Ctrl*)mNextLevel)->Access( accessTime, StoreAddress, CS::ACCESSTYPE::WRITE, wire_MToS, ((Cache*)mThis)->m_BlockSize ) ;
                delete[] wire_MToS ;
            }  // if

            // load data from next level
            uint64_t LoadAddress = ((Cache*)mThis)->TagToAddress( tag, set_index ) ;

#ifdef SIM_DATA
            Byte * wire_SToM = new Byte[ ((Cache*)mThis)->>m_BlockSize ] ;
#else
            Byte * wire_SToM = NULL ;
#endif

            if ( mNextLevelCacheName != CS::MEM_NAME::MAINMEM )
                ((Cache_Ctrl*)mNextLevel)->Access( accessTime, LoadAddress, CS::ACCESSTYPE::READ, wire_SToM, ((Cache*)mThis)->m_BlockSize ) ;
            ((Cache*)mThis)->LoadCacheBlock( tag, set_index, wire_SToM ) ;
            delete[] wire_SToM ;

            // To normal access
            ((Cache*)mThis)->AccessCache( AccessType, accessTime, address, Data, length ) ;
        }  // else

    }  // if  normal Cache

    else if ( CS::CACHETYPE::PRELOAD ) {

        uint8_t hit = ((PreloadCache*)mThis)->AccessCache( AccessType, accessTime, address, Data, length ) ;
        if ( hit >= CS::CACHEHITTYPE::HIT ) {  //if true cache  hit
            if ( ((PreloadCache*)mThis)->mEnableRecord ) {
              if ( AccessType == CS::ACCESSTYPE::WRITE ) {
                  ((PreloadCache*)mThis)->m_Num_W_Access++ ;
                  ((PreloadCache*)mThis)->m_Num_W_Hit++ ;
                  if ( hit == CS::CACHEHITTYPE::HIT_SRAM )
                    ((PreloadCache*)mThis)->m_Num_SW_Hit++ ;
                  else
                    ((PreloadCache*)mThis)->m_Num_MW_Hit++ ;
              }  // if
              else {
                  ((PreloadCache*)mThis)->m_Num_R_Access++ ;
                  ((PreloadCache*)mThis)->m_Num_R_Hit++ ;
                  if ( hit == CS::CACHEHITTYPE::HIT_SRAM )
                    ((PreloadCache*)mThis)->m_Num_SW_Hit++ ;
                  else
                    ((PreloadCache*)mThis)->m_Num_MW_Hit++ ;
              }  // else
            }  // if
            /*
             Log::PrintDebugLog( "CACHE L1 hit  : address:" ) ;
             printf( "0x%016llX length: %d\n", ( long long ) address, length ) ;
             */
        }  // if

        else {  // cache miss
            if ( ((PreloadCache*)mThis)->mEnableRecord ) {
              if ( AccessType == CS::ACCESSTYPE::WRITE )
                  ((PreloadCache*)mThis)->m_Num_W_Access++ ;
              else
                  ((PreloadCache*)mThis)->m_Num_R_Access++ ;
            } // iif
            /*
             Log::PrintDebugLog( "CACHE L1 miss : address:" ) ;
             printf( "0x%016llX length: %d\n", ( long long ) address, length ) ;
             */


            uint64_t tag ;
            uint32_t set_index, block_offset ;
            ((PreloadCache*)mThis)->mSRAM->SplitAddress( address, tag, set_index, block_offset ) ;

            uint8_t Preload_state = ((PreloadCache*)mThis)->mPreloadTable->NeedToPreload( tag, set_index ) ;

            if ( Preload_state > 0 ) {  // need to preload

                // To do allocate from cache, and evicted, write back to next level
#ifdef SIM_DATA
                 Byte * wire_MToS = new Byte[ ((PreloadCache*)mThis)->mSRAM->m_BlockSize ] ;
#else
                 Byte * wire_MToS = NULL ;
#endif

                 bool isNeedToWriteBack = ! ((PreloadCache*)mThis)->mSRAM->AllocateCache( set_index ) ;
                 if ( isNeedToWriteBack ) {  // if write through and always false
                     uint64_t StoreAddress ;
                     ((PreloadCache*)mThis)->mSRAM->StoreCacheBlock( set_index, StoreAddress, wire_MToS ) ;
                     if ( mNextLevelCacheName != CS::MEM_NAME::MAINMEM )
                         ((Cache_Ctrl*)mNextLevel)->Access( accessTime, StoreAddress, CS::ACCESSTYPE::WRITE, wire_MToS, ((PreloadCache*)mThis)->mSRAM->m_BlockSize ) ;
                     delete[] wire_MToS ;
                 }  // if


                 // load data from next level
                 uint64_t LoadAddress =  ((PreloadCache*)mThis)->mSRAM->TagToAddress( tag, set_index ) ;

#ifdef SIM_DATA
                 Byte * wire_SToM = new Byte[ ((PreloadCache*)mThis)->mSRAM->m_BlockSize ] ;
#else
                 Byte * wire_SToM = NULL ;
#endif

                 if ( mNextLevelCacheName != CS::MEM_NAME::MAINMEM )
                     ((Cache_Ctrl*)mNextLevel)->Access( accessTime, LoadAddress, CS::ACCESSTYPE::READ, wire_SToM, ((PreloadCache*)mThis)->mSRAM->m_BlockSize ) ;

                 ((PreloadCache*)mThis)->mSRAM->LoadCacheBlock( tag, set_index, wire_SToM ) ;
                 delete[] wire_SToM ;

                 // To normal access
                 ((PreloadCache*)mThis)->mSRAM->AccessCache( AccessType, accessTime, address, Data, length ) ;


                 // preload MRAM

                 ((PreloadCache*)mThis)->mMRAM->SplitAddress( address, tag, set_index, block_offset ) ;

                 for ( uint8_t i = 0 ; i < Preload_state ; i++, set_index+=1 ) {

#ifdef SIM_DATA
                 Byte * wire_MToS = new Byte[ ((PreloadCache*)mThis)->mSRAM->m_BlockSize ] ;
#else
                     Byte * wire_MToS = NULL ;
#endif

                     bool isNeedToWriteBack = ! ((PreloadCache*)mThis)->mMRAM->AllocateCache( set_index ) ;
                     if ( isNeedToWriteBack ) {  // if write through and always false
                         uint64_t StoreAddress ;
                         ((PreloadCache*)mThis)->mMRAM->StoreCacheBlock( set_index, StoreAddress, wire_MToS ) ;
                         if ( mNextLevelCacheName != CS::MEM_NAME::MAINMEM )
                             ((Cache_Ctrl*)mNextLevel)->Access( accessTime, StoreAddress, CS::ACCESSTYPE::WRITE, wire_MToS, ((PreloadCache*)mThis)->mMRAM->m_BlockSize ) ;
                         delete[] wire_MToS ;
                     }  // if

                     // load data from next level
                     uint64_t LoadAddress =  ((PreloadCache*)mThis)->mMRAM->TagToAddress( tag, set_index ) ;

#ifdef SIM_DATA
                     Byte * wire_SToM = new Byte[ ((PreloadCache*)mThis)->mSRAM->m_BlockSize ] ;
#else
                     Byte * wire_SToM = NULL ;
#endif

                     if ( mNextLevelCacheName != CS::MEM_NAME::MAINMEM )
                         ((Cache_Ctrl*)mNextLevel)->Access( accessTime, LoadAddress, CS::ACCESSTYPE::READ, wire_SToM, ((PreloadCache*)mThis)->mMRAM->m_BlockSize ) ;

                     ((PreloadCache*)mThis)->mMRAM->LoadCacheBlock( tag, set_index, wire_SToM ) ;
                     delete[] wire_SToM ;

                 } // for

            }  // if need to preload

            else { // not to preload

                // To do allocate from cache, and evicted, write back to next level
#ifdef SIM_DATA
                 Byte * wire_MToS = new Byte[ ((PreloadCache*)mThis)->mSRAM->m_BlockSize ] ;
#else
                 Byte * wire_MToS = NULL ;
#endif

                 bool isNeedToWriteBack = ! ((PreloadCache*)mThis)->mSRAM->AllocateCache( set_index ) ;
                 if ( isNeedToWriteBack ) {  // if write through and always false
                     uint64_t StoreAddress ;
                     ((PreloadCache*)mThis)->mSRAM->StoreCacheBlock( set_index, StoreAddress, wire_MToS ) ;
                     if ( mNextLevelCacheName != CS::MEM_NAME::MAINMEM )
                         ((Cache_Ctrl*)mNextLevel)->Access( accessTime, StoreAddress, CS::ACCESSTYPE::WRITE, wire_MToS, ((PreloadCache*)mThis)->mSRAM->m_BlockSize ) ;
                     delete[] wire_MToS ;
                 }  // if

                 // load data from next level
                 uint64_t LoadAddress =  ((PreloadCache*)mThis)->mSRAM->TagToAddress( tag, set_index ) ;

#ifdef SIM_DATA
                 Byte * wire_SToM = new Byte[ ((PreloadCache*)mThis)->mSRAM->m_BlockSize ] ;
#else
                 Byte * wire_SToM = NULL ;
#endif

                 if ( mNextLevelCacheName != CS::MEM_NAME::MAINMEM )
                     ((Cache_Ctrl*)mNextLevel)->Access( accessTime, LoadAddress, CS::ACCESSTYPE::READ, wire_SToM, ((PreloadCache*)mThis)->mSRAM->m_BlockSize ) ;

                 ((PreloadCache*)mThis)->mSRAM->LoadCacheBlock( tag, set_index, wire_SToM ) ;
                 delete[] wire_SToM ;

                 // To normal access
                 ((PreloadCache*)mThis)->mSRAM->AccessCache( AccessType, accessTime, address, Data, length ) ;


            }  // else not to preload

        }  // else cache miss

    }  // else if Preload Cache

}  // Cache_Ctrl::Access()

