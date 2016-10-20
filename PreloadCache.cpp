/*
 * PreloadCache.cpp
 *
 *  Created on: 2016¦~10¤ë18¤é
 *      Author: luo
 */

#include "Log.h"
#include "PreloadCache.h"

CS::PreloadTable::PreloadTable(uint32_t size, uint32_t replacePolicy ) : mSize( size) {

    m_RP_Manager = new CS::ReplaceManager( mSize, replacePolicy ) ;

    mEntry = new Entry[mSize] ;
    for( uint32_t i = 0 ; i < mSize ; i++ ) {
        mEntry[ i ].mTag = 0 ;
        mEntry[ i ].mIndex = 0 ;
        mEntry[ i ].mTimes = 0 ;
        mEntry[ i ].mState = 0 ;
    }  // for

} // PreloadTable::PreloadTable()

uint8_t CS::PreloadTable::NeedToPreload( uint64_t tag,  uint32_t index) {

    uint32_t i = SearchTable( tag ) ;
    if (  i != (uint32_t)-1 ) {
        m_RP_Manager->UpdateRecord(i) ;
        uint8_t offset = (mEntry[i].mState+1) * 5 ;  // !!key point offset setting
        if ( index - mEntry[i].mIndex <= offset )
            mEntry[i].mTimes++ ;
        if ( mEntry[i].mTimes >= (uint32_t)(mEntry[i].mState+1)*3 ) {
           mEntry[i].mState++  ;
           if ( mEntry[i].mState > 3 ) mEntry[i].mState = 3 ;
        } // if
        return mEntry[i].mState ;
    }  // if

    else {
        i = m_RP_Manager->GetReplaceIndex() ;
        m_RP_Manager->UpdateRecord(i) ;
        mEntry[i].mTag = tag ;
        mEntry[i].mIndex = index ;
        mEntry[i].mState = 0 ;
        mEntry[i].mTimes = 0 ;

        return mEntry[i].mState ;
    } // else
    return 0 ;
}  // PreloadTable::NeedToPreload()

uint32_t CS::PreloadTable::SearchTable( uint64_t tag ) {
    for ( uint32_t i = 0; i < mSize; i++ ) {
        if ( mEntry[ i ].mTag == tag )
            return i ;
    }  // for
    return -1 ;

}  // PreloadTable::SearchTalbe()

CS::PreloadCache::PreloadCache(  uint32_t CacheName, uint8_t CacheType, uint32_t PT_size, uint32_t PTRP,
        uint32_t SRAM_cache_size, uint32_t SRAM_blocksize, uint32_t SRAM_associativity,
        uint32_t SRAM_replacePolicy, uint32_t SRAM_writepolicy, uint8_t SRAM_readlatency, uint8_t SRAM_writelatnecy,
        uint32_t MRAM_cache_size, uint32_t MRAM_blocksize, uint32_t MRAM_associativity,
        uint32_t MRAM_replacePolicy, uint32_t MRAM_writepolicy, uint8_t MRAM_readlatency, uint8_t MRAM_writelatnecy ) :
        Base(CacheName, CacheType), m_Num_SW_Hit (0), m_Num_MW_Hit(0), m_Num_W_Access(0), m_Num_R_Access(0), m_Num_SR_Hit(0), m_Num_MR_Hit(0),
        m_Num_W_Hit(0), m_Num_R_Hit(0) {

  mPreloadTable = new PreloadTable(  PT_size,  PTRP ) ;

  mSRAM =  new Cache( m_Name,m_CacheType, SRAM_cache_size, SRAM_blocksize, SRAM_associativity, SRAM_replacePolicy,
                      SRAM_writepolicy, SRAM_readlatency, SRAM_writelatnecy ) ;

  mMRAM = new Cache( m_Name,m_CacheType, MRAM_cache_size, MRAM_blocksize, MRAM_associativity, MRAM_replacePolicy,
                     MRAM_writepolicy, MRAM_readlatency, MRAM_writelatnecy ) ;

}  // PreloadCache()

uint8_t CS::PreloadCache::AccessCache( uint32_t AccessType, const uint64_t accessTime, const uint64_t address, Byte * Data, uint32_t length ) {
    uint8_t hit = mSRAM->AccessCache( AccessType, accessTime, address, Data, length ) ;
    if ( hit == CS::CACHEHITTYPE::HIT ) {
        if ( this->mEnableRecord ) {
            if ( AccessType == CS::ACCESSTYPE::WRITE ) {
                mSRAM->m_Num_W_Access++ ;
                mSRAM->m_Num_W_Hit++ ;
            }  // if
            else {
                mSRAM->m_Num_R_Access++ ;
                mSRAM->m_Num_R_Hit++ ;
            }  // else
         }  // if
         return CS::CACHEHITTYPE::HIT_SRAM ;
    }  // SRAM hit

    else {  // SRAM miss
        if ( this->mEnableRecord ) {
            if ( AccessType == CS::ACCESSTYPE::WRITE )
                mSRAM->m_Num_W_Access++ ;
            else
                mSRAM->m_Num_R_Access++ ;
         }  // if

        hit = mMRAM->AccessCache( AccessType, accessTime, address, Data, length ) ;
        if ( hit == CS::CACHEHITTYPE::HIT ) {
            if ( this->mEnableRecord ) {
                if ( AccessType == CS::ACCESSTYPE::WRITE ) {
                    mMRAM->m_Num_W_Access++ ;
                    mMRAM->m_Num_W_Hit++ ;
                }  // if
                else {
                    mMRAM->m_Num_R_Access++ ;
                    mMRAM->m_Num_R_Hit++ ;
                }  // else
             }  // if
             return CS::CACHEHITTYPE::HIT_MRAM ;
        }  // MRAM hit
        else {  // MRAM miss
            if ( this->mEnableRecord ) {
                if ( AccessType == CS::ACCESSTYPE::WRITE )
                    mMRAM->m_Num_W_Access++ ;
                else
                    mMRAM->m_Num_R_Access++ ;
            }  // if
        }  // MRAM miss
    }  // // SRAM miss


    return CS::CACHEHITTYPE::MISS ;

}

