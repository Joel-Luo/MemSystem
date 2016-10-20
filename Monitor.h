/*
 * Monitor.h
 *
 *  Created on: May 13, 2016
 *      Author: LuoJing-Yuan
 */

#ifndef MONITOR_H
#define MONITOR_H
#include "Cache.h"
#include "PreloadCache.h"

#include <stdio.h>

namespace CS {
    class Monitor {
        public:
            Base ** m_Cache_list ;
            int m_CacheLevel ;
        public:
            Monitor( int level, Base ** CL ) ;

            void OutputCacheInfo( int CacheType ) ;
            void OutputResult() ;

    } ;
}
;
#endif /* MONITOR_H_ */
