/*
 * Monitor.h
 *
 *  Created on: May 13, 2016
 *      Author: LuoJing-Yuan
 */

#ifndef MONITOR_H
#define MONITOR_H
#include "Cache.h"

class Monitor {
    private:
        FILE * m_File ;
    public:
        Cache ** m_Cache_list ;

    public:
        Monitor( Cache ** CL ) ;

        void OutputResult() ;

} ;

#endif /* MONITOR_H_ */
