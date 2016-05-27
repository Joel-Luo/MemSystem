/*
 * Monitor.cpp
 *
 *  Created on: May 13, 2016
 *      Author: LuoJing-Yuan
 */
#include "Monitor.h"
#include "Log.h"
Monitor::Monitor( int level, Cache ** CL ) {
  m_Cache_list = CL ;
  m_File = NULL ;
  m_CacheLevel = level ;
}  // Monitor::Monitor()

void Monitor::OutputCacheInfo( int CacheType ) {
   int access = m_Cache_list[CacheType]->m_Num_Access ;
   int hit = m_Cache_list[CacheType]->m_Num_Hit ;
   double rate = (double)hit / (double)access * 100  ;
   Log::PrintMessage( "\nL" + std::to_string(CacheType+1)+ " info :\naccess: " + std::to_string(access) +
                           "\nhit: " + std::to_string(hit) +
                           "\nhit rate: " + std::to_string(rate) + "%" ) ;

}  // Monitor::OutputL1CacheInfo()
