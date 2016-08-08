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

void Monitor::OutputCacheInfo( int CacheLevel ) {
    if ( m_Cache_list[ CacheLevel ]->m_CacheType == Cache::CACHE ) {
        uint64_t w_access = m_Cache_list[ CacheLevel ]->m_Num_W_Access ;
        uint64_t w_miss = w_access - m_Cache_list[ CacheLevel ]->m_Num_W_Hit ;
        double w_rate = ( double ) w_miss / ( double ) w_access * 100 ;

        uint64_t r_access = m_Cache_list[ CacheLevel ]->m_Num_R_Access ;
        uint64_t r_miss = r_access - m_Cache_list[ CacheLevel ]->m_Num_R_Hit ;
        double r_rate = ( double ) r_miss / ( double ) r_access * 100 ;

        uint64_t t_access = w_access + r_access ;
        uint64_t t_miss = w_miss + r_miss ;
        double t_rate = ( double ) t_miss / ( double ) t_access * 100 ;

        uint64_t read_l = m_Cache_list[ CacheLevel ]->m_ReadLatency ;
        uint64_t write_l = m_Cache_list[ CacheLevel ]->m_WriteLatency ;
        uint64_t Load_l = ( CacheLevel + 1 < m_CacheLevel ) ? m_Cache_list[ CacheLevel + 1 ]->m_ReadLatency : 50 ;
        Load_l += write_l ;

        Log::PrintMessageToFile( Log::CacheResultInfoFile, "========== L" + std::to_string( CacheLevel + 1 )
                                         + " info ==========\nw_access: " + std::to_string( w_access ) + "\tw_miss: "
                                         + std::to_string( w_miss ) + "\tw_miss_rate: " + std::to_string( w_rate ) + "%"
                                         + "\nr_access: " + std::to_string( r_access ) + "\tr_miss: "
                                         + std::to_string( r_miss ) + "\tr_miss_rate: " + std::to_string( r_rate ) + "%"
                                         + "\nt_access: " + std::to_string( t_access ) + "\tt_miss: "
                                         + std::to_string( t_miss ) + "\tt_miss_rate: " + std::to_string( t_rate )
                                         + "%" ) ;

        uint64_t tr_l = ( m_Cache_list[ CacheLevel ]->m_Num_R_Hit * read_l ) + ( r_miss * ( read_l + Load_l ) ) ;
        uint64_t tw_l = ( m_Cache_list[ CacheLevel ]->m_Num_W_Hit * write_l ) + ( w_miss * ( write_l + Load_l ) ) ;
        Log::PrintMessageToFile( Log::CacheResultInfoFile, "Read_Latency: " + std::to_string( tr_l )
                                         + "\tWrite_Latency: " + std::to_string( tw_l ) ) ;

        Log::PrintMessageToFile( Log::CacheResultInfoFile, "========== End of L" + std::to_string( CacheLevel + 1 )
                                         + " ==========\n" ) ;
    }  // if
    else if ( m_Cache_list[ CacheLevel ]->m_CacheType == Cache::BUFFERCACHE ) {

        Log::PrintMessageToFile( Log::CacheResultInfoFile, "========== L" + std::to_string( CacheLevel + 1 )
                                         + " info ==========" ) ;

        uint64_t bw_access = m_Cache_list[ CacheLevel ]->mBufferCache->m_Num_W_Access ;
        uint64_t bw_miss = bw_access - m_Cache_list[ CacheLevel ]->mBufferCache->m_Num_W_Hit ;
        uint64_t br_access = m_Cache_list[ CacheLevel ]->mBufferCache->m_Num_R_Access ;
        uint64_t br_miss = br_access - m_Cache_list[ CacheLevel ]->mBufferCache->m_Num_R_Hit ;
        uint64_t bt_access = bw_access + br_access ;
        uint64_t bt_miss = bw_miss + br_miss ;
        double bw_rate = ( double ) bw_miss / ( double ) bw_access * 100 ;
        double br_rate = ( double ) br_miss / ( double ) br_access * 100 ;
        double bt_rate = ( double ) bt_miss / ( double ) bt_access * 100 ;

        uint64_t w_access = m_Cache_list[ CacheLevel ]->m_Num_W_Access ;
        uint64_t r_access = m_Cache_list[ CacheLevel ]->m_Num_R_Access ;
        uint64_t t_access = w_access + r_access ;
        uint64_t w_miss = w_access - m_Cache_list[ CacheLevel ]->m_Num_W_Hit ;
        uint64_t r_miss = r_access - m_Cache_list[ CacheLevel ]->m_Num_R_Hit ;
        uint64_t t_miss = w_miss + r_miss ;
        double w_rate = ( double ) w_miss / ( double ) w_access * 100 ;
        double r_rate = ( double ) r_miss / ( double ) r_access * 100 ;
        double t_rate = ( double ) t_miss / ( double ) t_access * 100 ;

        uint64_t Addtion_Wl = m_Cache_list[ CacheLevel ]->mBufferCache->mAddtionWriteLantency ;

        uint64_t bread_l = m_Cache_list[ CacheLevel ]->mBufferCache->m_ReadLatency ;
        uint64_t bwrite_l = m_Cache_list[ CacheLevel ]->mBufferCache->m_WriteLatency ;

        uint64_t read_l = m_Cache_list[ CacheLevel ]->m_ReadLatency ;
        uint64_t write_l = m_Cache_list[ CacheLevel ]->m_WriteLatency ;
        uint64_t Load_l = ( CacheLevel + 1 < m_CacheLevel ) ? m_Cache_list[ CacheLevel + 1 ]->m_ReadLatency : 50 ;
        Load_l += write_l ;

        uint64_t br_l = ( m_Cache_list[ CacheLevel ]->mBufferCache->m_Num_R_Hit * bread_l ) ;
        uint64_t sr_l = ( m_Cache_list[ CacheLevel ]->m_Num_R_Hit * read_l ) + ( r_miss * ( read_l + Load_l ) ) ;
        uint64_t tr_l = br_l + sr_l ;

        uint64_t bw_l = ( m_Cache_list[ CacheLevel ]->mBufferCache->m_Num_W_Hit * bwrite_l
                        + Addtion_Wl ) ;
        uint64_t sw_l = ( m_Cache_list[ CacheLevel ]->m_Num_W_Hit * write_l ) + ( w_miss * ( write_l + Load_l ) ) ;
        uint64_t tw_l = bw_l + sw_l ;

        Log::PrintMessageToFile( Log::CacheResultInfoFile, "Buffer info : \nw_access: " + std::to_string( bw_access )
                                         + "\tw_miss: " + std::to_string( bw_miss ) + "\tw_miss_rate: "
                                         + std::to_string( bw_rate ) + "%\nr_access: " + std::to_string( br_access )
                                         + "\tr_miss: " + std::to_string( br_miss ) + "\tr_miss_rate: "
                                         + std::to_string( br_rate ) + "%\nt_access: " + std::to_string( bt_access )
                                         + "\tt_miss: " + std::to_string( bt_miss ) + "\tt_miss_rate: "
                                         + std::to_string( bt_rate ) + "%\nCache info :\nw_access: "
                                         + std::to_string( w_access ) + "\tw_miss: " + std::to_string( w_miss )
                                         + "\tw_miss_rate: " + std::to_string( w_rate ) + "%\nr_access: "
                                         + std::to_string( r_access ) + "\tr_miss: " + std::to_string( r_miss )
                                         + "\tr_miss_rate: " + std::to_string( r_rate ) + "%\nt_access: "
                                         + std::to_string( t_access ) + "\tt_miss: " + std::to_string( t_miss )
                                         + "\tt_miss_rate: " + std::to_string( t_rate )
                                         + "%\nBuffer Latency:\nReadLatency: " + std::to_string( br_l )
                                         + "\tWriteLatency: " + std::to_string( bw_l )
                                         + "\nSTTRAM Latency:\nReadLatency: " + std::to_string( sr_l )
                                         + "\tWriteLatency: " + std::to_string( sw_l )
                                         + "\nTotal Latency:\nReadLatency: " + std::to_string( tr_l )
                                         + "\tWriteLatency: " + std::to_string( tw_l ) ) ;
        /*
        FILE * temp = fopen( "./logIndex.txt", "w" ) ;
        for ( uint32_t i = 0 ; i <  m_Cache_list[ CacheLevel ]->m_Num_Set ; i++ )
          fprintf( temp, "%d %d\n", i,  m_Cache_list[ CacheLevel ]->m_Sets[i]->m_UsingTime ) ;
        */  
        Log::PrintMessageToFile( Log::CacheResultInfoFile, "========== End of L" + std::to_string( CacheLevel + 1 )
                                         + " ==========\n" ) ;
                                         
    }  // else if

}  // Monitor::OutputL1CacheInfo()
