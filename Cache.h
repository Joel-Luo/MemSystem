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

        enum REPLACEPOLICY {
            LRU = 0, ROUND_ROBIN, RANDOM
        } ;

        enum WRITE_POLICY {
            WRITE_BACK = 0, WRITE_TROUGH
        } ;

    private:
        bool m_NeedWriteBack ;
        uint64_t m_Num_Access ;
        uint64_t m_Num_Hit ;

        uint32_t m_BlockSize_log2 ;
        uint32_t m_Associativity_log2 ;

        Cache_Set** m_Sets ;
        std::vector < int > * RP_record ;

    public:
        uint64_t m_CacheSize ;
        uint32_t m_Num_Set ;
        uint32_t m_Name ;
        uint32_t m_ReplacePolicy ;
        uint32_t m_WrtiePolicy ;
        uint32_t m_BlockSize ;

    private:
        static uint32_t floorLog2( uint32_t number ) ;
        uint32_t FindTagSet( uint32_t tag ) ;
    public:

        Cache( uint32_t CacheName, uint32_t cache_size, uint32_t blocksize, uint32_t associativity,
                uint32_t replacePolicy, uint32_t writepolicy ) ;

        Cache( uint32_t CacheName, uint32_t cache_size, uint32_t blocksize, uint32_t replacePolicy,
                uint32_t writepolicy ) ;

        void AccessSingleLine( bool hit, uint32_t AccessType, const uint64_t address, Byte * Data,
                uint32_t length ) ;

        void SplitAddress( const uint64_t addr, uint64_t& tag, uint32_t& assoc_index,
                uint32_t& block_offset ) ;

        bool PeekSingleLine( const uint64_t addr ) ;

        bool ReplaceSet( uint32_t set_index, const uint64_t address ) ;

        uint32_t MaintainLRU( uint32_t index ) ;

        uint32_t MaintainReplacePolicy( const uint64_t address ) ;

        uint64_t AppendAddress( Cache_Set * cache_set ) ;

        Cache_Set * GetCacheSet( uint32_t index ) {
            return m_Sets[ index ] ;
        }

} ;

#endif
