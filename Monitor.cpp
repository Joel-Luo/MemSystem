/*
 * Monitor.cpp
 *
 *  Created on: May 13, 2016
 *      Author: LuoJing-Yuan
 */
#include "Monitor.h"
#include "Log.h"
#include <stdint.h>
Monitor::Monitor( int level, Cache ** CL ) {
    m_Cache_list = CL ;
    m_CacheLevel = level ;
}  // Monitor::Monitor()

void Monitor::OutputCacheInfo( int CacheType ) {
    int w_access = m_Cache_list[ CacheType ]->m_Num_W_Access ;
    int w_miss = w_access - m_Cache_list[ CacheType ]->m_Num_W_Hit ;
    double w_rate = ( double ) w_miss / ( double ) w_access * 100 ;

    int r_access = m_Cache_list[ CacheType ]->m_Num_R_Access ;
    int r_miss = r_access - m_Cache_list[ CacheType ]->m_Num_R_Hit ;
    double r_rate = ( double ) r_miss / ( double ) r_access * 100 ;

    int t_access = w_access + r_access ;
    int t_miss = w_miss + r_miss ;
    double t_rate = ( double ) t_miss / ( double ) t_access * 100 ;

    uint64_t read_l = m_Cache_list[ CacheType ]->m_ReadLatency ;
    uint64_t write_l = m_Cache_list[ CacheType ]->m_WriteLatency ;
    uint64_t Load_l = ( CacheType + 1 < m_CacheLevel ) ? m_Cache_list[ CacheType + 1 ]->m_ReadLatency : 50 ;
    Load_l += write_l ;

    Log::PrintMessageToFile( Log::CacheResultInfoFile, "L" + std::to_string( CacheType + 1 ) + " info :\nw_access: " + std::to_string( w_access )
            + "\tw_miss: " + std::to_string( w_miss ) + "\tw_miss_rate: " + std::to_string( w_rate ) + "%"
            + "\nr_access: " + std::to_string( r_access ) + "\tr_miss: " + std::to_string( r_miss ) + "\tr_miss_ rate: "
            + std::to_string( r_rate ) + "%" + "\nt_access: " + std::to_string( t_access ) + "\tt_miss: "
            + std::to_string( t_miss ) + "\tt_miss_rate: " + std::to_string( t_rate ) + "%" ) ;

    uint64_t tr_l = ( m_Cache_list[ CacheType ]->m_Num_R_Hit * read_l ) + ( r_miss * ( read_l + Load_l ) ) ;
    uint64_t tw_l = ( m_Cache_list[ CacheType ]->m_Num_W_Hit * write_l ) + ( w_miss * ( write_l + Load_l ) ) ;
    Log::PrintMessageToFile( Log::CacheResultInfoFile, "Read_Latency: " + std::to_string( tr_l ) + "\tWrite_Latency: " + std::to_string( tw_l ) ) ;

}  // Monitor::OutputL1CacheInfo()
