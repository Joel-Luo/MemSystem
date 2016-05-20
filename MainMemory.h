#ifndef DRAM_H
#define DRAM_H
#include <stdint.h>

class MainMemory {

    public:
        enum MAINMEM_NAME {
            DRAM = 0

        } ;
        MainMemory() ;
        ~MainMemory() ;

        void WriteToMainMemory() ;
        void ReadFromMainMemory() ;

} ;

#endif
