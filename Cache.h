#ifndef CACHE_H
#define CACHE_H
#include "Cache_Set.h"
#include <vector>

class BufferCache {
    public :
      struct BufferSet {
        uint64_t mAddress ;
        Byte * mData ;
        uint64_t mAccessTime ;
      };

    public :
      BufferSet ** mBufferSet ;
      uint8_t mDataLength ;
      uint8_t mNumOfEntry ;

      uint64_t mAddtionWriteLantency ;
      uint8_t m_ReadLatency ;
      uint8_t m_WriteLatency ;
      uint64_t m_Num_W_Access ;
      uint64_t m_Num_W_Hit ;
      uint64_t m_Num_R_Access ;
      uint64_t m_Num_R_Hit ;


      std::vector<uint8_t> * mBufferQueue ;
      std::vector<uint8_t> * mBufferNonUseQueue ;

      BufferCache(uint8_t NumOfEntry, uint8_t DataLength, uint8_t ReadLatency, uint8_t WriteLatency ) ;

      std::vector<uint8_t> * GetRequestToCache( std::vector < uint8_t > * list, uint64_t nowTime ) ;

      bool BufferAccess(  const uint64_t accessTime, const uint64_t address, const uint32_t AccessType,Byte * Data, uint32_t length ) ;

      uint8_t AllocatBufferEntry( uint64_t nowTime, uint32_t &AddLatency,  bool & isBufferFull ) ;

      void PushRequestToBuffer( const uint8_t pushIndex, const uint64_t accessTime, const uint64_t address, Byte * Data ) ;



};


class Cache {
    public:
        enum CACHE_NAME {
            L1_D = 0, L2, L3, MAINMEM
        } ;

        enum ACCESSTYPE {
            READ = 0, WRITE
        } ;

        enum CACHETYPE {
            CACHE = 0 , HYBRIDCACHE, BUFFERCACHE
        };

    private:

        uint32_t m_BlockSize_log2 ;
        uint32_t m_Associativity_log2 ;
        Cache_Set** m_Sets ;

    public:
        uint8_t m_CacheType ;
        uint64_t m_Num_W_Access ;
        uint64_t m_Num_W_Hit ;
        uint64_t m_Num_R_Access ;
        uint64_t m_Num_R_Hit ;
        uint64_t m_CacheSize ;
        uint32_t m_Num_Set ;
        uint8_t  m_Name ;
        uint32_t m_ReplacePolicy ;
        uint32_t m_WritePolicy ;
        uint32_t m_BlockSize ;
        uint8_t m_ReadLatency ;
        uint8_t m_WriteLatency ;

        BufferCache * mBufferCache ;

    private:
        static uint32_t floorLog2( uint32_t number ) ;
    public:

        Cache( uint32_t CacheName, uint8_t CacheType, uint32_t cache_size, uint32_t blocksize, uint32_t associativity,
                uint32_t replacePolicy, uint32_t writepolicy, uint8_t readlatency, uint8_t writelatnecy ) ;
        void BuildHybridCache( uint8_t numofcellType, uint8_t numofsub, uint8_t * size, uint32_t * retentiontime, uint8_t * readlatency, uint8_t * writelatency, uint32_t blocksize, uint32_t associativity, uint32_t replacePolicy, uint32_t writepolicy ) ;

        void BuildBufferCache( uint8_t numofentry, uint8_t DataLength, uint8_t readlatency, uint8_t writelatency ) ;

        bool AccessCache( uint32_t AccessType, const uint64_t accessTime, const uint64_t address, Byte * Data, uint32_t length) ;
        bool AccessHybridCache( uint32_t AccessType, const uint64_t accessTime, const uint64_t address, Byte * Data, uint32_t length, bool & timeUpWriteBack , Byte * WBData  ) ;

        void SplitAddress( const uint64_t addr, uint64_t& tag, uint32_t& assoc_index, uint32_t& block_offset ) ;

        uint64_t TagToAddress( uint64_t tag, uint32_t index ) ;

        bool AllocateCache( uint32_t set_index ) ;
        void LoadCacheBlock( uint64_t tag, uint32_t set_index, Byte * in ) ;
        void StoreCacheBlock(  uint32_t set_index, uint64_t & TatgetAddr, Byte * out ) ;

        bool RetentionTimeUp( uint32_t set_index, uint32_t & wayindex, uint64_t accessTime,  Byte * out) ;
        void UpdateTimeStamp(  uint32_t set_index, uint32_t  wayindex, uint64_t accessTime) ;

} ;


#endif
