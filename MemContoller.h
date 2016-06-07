#ifndef MEMSYSTEM_H
#define MEMSYSTEM_H
#include "Cache.h"
#include "MainMemory.h"
#include "CfgParser.h"
#include "Monitor.h"
class MemContoller {

    public:
        enum ACCESSTYPE {
            READ = 0, WRITE
        } ;

        enum MEMTYPE {
            L1_D = 0, L2, L3, MAINMEM
        } ;


    private:
        uint8_t * m_CacheType ;
        Cache ** m_Cache_list ;
        uint32_t m_Cache_level ;
        CfgParser * cfgparser ;

    public:
        Monitor * m_Monitor ;

        MemContoller( const char * cfg ) ;

        void CoreAccessMem( const uint64_t accessTime, const uint64_t address, const uint32_t AccessType, Byte* Data, uint32_t length ) ;

        void AccessNextLevel( uint32_t Cachetype, const uint64_t accessTime, const uint64_t address, const uint32_t AccessType, Byte* Data,
                uint32_t length ) ;

    private:

        void CreateMemSystem() ;

        void CacheMigrationContrl() ;
} ;

#endif

