#ifndef CACHE_H
#define CACHE_H
#include "Cache_Set.h"
#include <string.h>
#include <vector>

namespace CS {

    enum MEM_NAME {
        L1_D = 0, L2, L3, MAINMEM
    } ;

    enum ACCESSTYPE {
        READ = 0, WRITE
    } ;

    enum CACHETYPE {
        CACHE = 0
    } ;

    class GTable {
        public:
            struct Entry {
                    uint64_t mTag ;
                    uint32_t times ;
            } ;
            uint32_t m_Size ;
            uint32_t m_Thershold ;
            std::vector < Entry* > * m_GTable ;
            GTable( uint32_t Size, uint8_t ReplacePolicy, uint32_t Thershold ) ;

            bool GTableController( uint64_t tag ) ;
            // if DO NOT need to allocate in cache return false

        private :
            CS::ReplaceManager * mRP ;
            int32_t SearchTable( uint64_t tag ) ;
            void UpdateTable( uint32_t index, uint64_t tag, uint32_t times ) ;

    } ;

    class Cache {

        private:

            uint32_t m_BlockSize_log2 ;
            uint32_t m_Associativity_log2 ;
            uint32_t m_Num_Set_Log2 ;

        public:
            uint8_t m_CacheType ;
            uint8_t m_Name ;
            uint64_t m_CacheSize ;
            uint32_t m_BlockSize ;
            uint64_t m_Num_W_Access ;
            uint64_t m_Num_W_Hit ;
            uint64_t m_Num_R_Access ;
            uint64_t m_Num_R_Hit ;
            uint32_t m_Num_Way ;

            Cache_Set** m_Sets ;

        public:
            uint32_t m_Num_Set ;
            uint32_t m_ReplacePolicy ;
            uint32_t m_WritePolicy ;
            uint8_t m_CacheLineWriteMod ;

            uint8_t m_ReadLatency ;
            uint8_t m_WriteLatency ;
            bool mEnableRecord ;

        private:
            static uint32_t floorLog2( uint32_t number ) ;

        public:

            Cache( uint32_t CacheName, uint8_t CacheType, uint32_t cache_size, uint32_t blocksize,
                    uint32_t associativity, uint32_t replacePolicy, uint32_t writepolicy, uint8_t readlatency,
                    uint8_t writelatnecy ) ;

            bool AccessCache( uint32_t AccessType, const uint64_t accessTime, const uint64_t address, Byte * Data,
                    uint32_t length ) ;

            void SplitAddress( const uint64_t addr, uint64_t& tag, uint32_t& assoc_index, uint32_t& block_offset ) ;

            uint64_t TagToAddress( uint64_t tag, uint32_t index ) ;

            bool AllocateCache( uint32_t set_index ) ;
            void LoadCacheBlock( uint64_t tag, uint32_t set_index, Byte * in ) ;
            void StoreCacheBlock( uint32_t set_index, uint64_t & TatgetAddr, Byte * out ) ;

    } ;
}
;

#endif
