/*

 * Cache_Ctrl.h
 *
 *  Created on: Jun 20, 2016
 *      Author: LuoJing-Yuan
 */

#ifndef CACHE_CTRL_H
#define CACHE_CTRL_H

#include "Cache.h"



namespace CS {
    class Cache_Ctrl {
        private:
            Cache * mThis ;
            Cache_Ctrl * mNextLevel ;

        public:

            uint8_t mThisCacheType ;
            uint8_t mThisCacheName ;
            uint8_t mNextLevelCacheName ;

            Cache_Ctrl( Cache* thisLevel, uint8_t cachetype ) ;

            void Access( const uint64_t accessTime, const uint64_t address, const uint32_t AccessType, Byte * Data,
                    uint32_t length ) ;

            void SetNextLevelCacheCtrl( Cache_Ctrl * nextLevelCacheCtrl ) ;

    } ;

}
;
#endif /* CACHE_CTRL_H_ */
