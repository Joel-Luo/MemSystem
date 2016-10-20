/*
 * PreloadCache.h
 *
 *  Created on: 2016¦~10¤ë18¤é
 *      Author: luo
 */

#ifndef PRELOADCACHE_H
#define PRELOADCACHE_H

#include "Cache.h"


namespace CS {
  class PreloadTable {
      private :
          CS::ReplaceManager * m_RP_Manager ;

          uint32_t SearchTable( uint64_t tag ) ;

      public :
          struct Entry {
            uint64_t mTag ;
            uint32_t mIndex ;
            uint32_t mTimes ;
            uint8_t mState ;
          } ;

          Entry * mEntry ;

          uint32_t mSize ;

          PreloadTable( uint32_t size, uint32_t replacePolicy ) ;

          uint8_t NeedToPreload( uint64_t tag,  uint32_t index) ;

  };

  class PreloadCache : public Base {

      public :
          Cache * mSRAM ;
          Cache * mMRAM ;

      public :
          uint64_t m_Num_SW_Hit ;
          uint64_t m_Num_MW_Hit ;
          uint64_t m_Num_W_Access ;
          uint64_t m_Num_R_Access ;
          uint64_t m_Num_SR_Hit ;
          uint64_t m_Num_MR_Hit ;
          uint64_t m_Num_W_Hit ;
          uint64_t m_Num_R_Hit ;


          PreloadCache(  uint32_t CacheName, uint8_t CacheType, uint32_t PT_size, uint32_t PTRP,
                         uint32_t SRAM_cache_size, uint32_t SRAM_blocksize, uint32_t SRAM_associativity,
                         uint32_t SRAM_replacePolicy, uint32_t SRAM_writepolicy, uint8_t SRAM_readlatency, uint8_t SRAM_writelatnecy ,
                         uint32_t MRAM_cache_size, uint32_t MRAM_blocksize, uint32_t MRAM_associativity,
                         uint32_t MRAM_replacePolicy, uint32_t MRAM_writepolicy, uint8_t MRAM_readlatency, uint8_t MRAM_writelatnecy ) ;

          uint8_t AccessCache( uint32_t AccessType, const uint64_t accessTime, const uint64_t address, Byte * Data, uint32_t length );


          uint32_t PreloadTableSize() { return mPreloadTable->mSize ; } ;

      public :
          PreloadTable * mPreloadTable ;

  };  // Preload class

}  // PreloadCache namespace

#endif
