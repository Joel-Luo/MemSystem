#ifndef CACHE_SET_H
#define CACHE_SET_H
#include <stdint.h>
#include <stdio.h>
#include <vector>

typedef uint8_t Byte ;

// #define SIM_DATA ;

namespace CS {
    enum REPLACEPOLICY {
        LRU = 0, ROUND_ROBIN, RANDOM
    } ;

    enum WRITEPOLICY {
        WRITE_BACK = 0, WRITE_TROUGH
    } ;

    class ReplaceManager {

        private:
            std::vector < uint32_t > * m_Record ;
            uint32_t m_ReplacePolicy ;
        public:
            ReplaceManager( uint32_t size, uint32_t rp ) ;
            uint32_t GetReplaceIndex() ;
            void UpdateRecord( uint32_t index ) ;

    } ;

    class Cache_Set {

        public:
            struct Way {
                    bool Valid ;
                    bool Dirty ;
                    uint64_t mTag ;
                    uint64_t mTimeStamp ;
                    Byte * mData ;  // Data size = blocksize
                    std::vector < uint64_t > * mTimeLog ;
            } ;

            Way * m_Way ;
            ReplaceManager * m_RP_Manager ;
            uint32_t m_BlockSize ;

        private:
            uint32_t m_Associativity ;
            uint32_t m_WritePolicy ;

        public:
            uint32_t m_RetentionTime ;
            uint8_t m_ReadLatency ;
            uint8_t m_WriteLatency ;
            uint32_t m_UsingTime ;

        public:

            Cache_Set( uint32_t blocksize, uint32_t associativity, uint32_t replacePolicy, uint32_t writePolicy,
                    uint8_t ReadLatency, uint8_t WriteLatency ) ;

            uint32_t FindTagInWay( uint64_t tag ) ;

            void ReadData( Byte * out, uint32_t way_index, uint32_t offset, uint32_t length ) ;
            void WriteData( Byte * in, uint32_t way_index, uint32_t offset, uint32_t length ) ;
            void AllocateData( Byte * in, uint64_t tag, uint32_t way_index, uint32_t offset, uint32_t length ) ;

    } ;
}
;
#endif
