#ifndef CACHE_H
#define CACHE_H
#include "Cache_Set.h"
#include <vector>

class Cache {
    public:
        enum CACHE_NAME {
            L1_D = 0, L2, L3, MAINMEM
        } ;

        enum ACCESSTYPE {
            READ = 0, WRITE
        } ;


    private:

        uint32_t m_BlockSize_log2 ;
        uint32_t m_Associativity_log2 ;

        Cache_Set** m_Sets ;

    public:
        uint64_t m_Num_Access ;
        uint64_t m_Num_Hit ;
        uint64_t m_CacheSize ;
        uint32_t m_Num_Set ;
        uint32_t m_Name ;
        uint32_t m_ReplacePolicy ;
        uint32_t m_WritePolicy ;
        uint32_t m_BlockSize ;

    private:
        static uint32_t floorLog2( uint32_t number ) ;
    public:

        Cache( uint32_t CacheName, uint32_t cache_size, uint32_t blocksize, uint32_t associativity,
                uint32_t replacePolicy, uint32_t writepolicy ) ;


        bool AccessCache( uint32_t AccessType, const uint64_t address, Byte * Data, uint32_t length ) ;

        void SplitAddress( const uint64_t addr, uint64_t& tag, uint32_t& assoc_index, uint32_t& block_offset ) ;

        uint64_t TagToAddress( uint64_t tag, uint32_t index ) ;

        bool AllocateCache( uint32_t set_index ) ;
        void LoadCacheBlock( uint64_t tag, uint32_t set_index, Byte * in ) ;
        void StoreCacheBlock( uint64_t tag, uint32_t set_index, uint64_t & TatgetAddr, Byte * out ) ;

} ;

#endif
