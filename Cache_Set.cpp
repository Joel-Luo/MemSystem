#include "Cache_Set.h"
#include <string.h>
#include <stdio.h>

CS::ReplaceManager::ReplaceManager( uint32_t size, uint32_t rp ) :
        m_ReplacePolicy( rp ) {
    m_Record = new std::vector < uint32_t >() ;
    if ( m_ReplacePolicy == CS::REPLACEPOLICY::LRU )
        for ( uint32_t i = 0; i < size; i++ )
            m_Record->push_back( i ) ;
    // TODO  else if ( m_RP == ROUND_ROBIN ) ;
}  // ReplaceManager::ReplaceManager()

uint32_t CS::ReplaceManager::GetReplaceIndex() {
    if ( m_ReplacePolicy == CS::REPLACEPOLICY::LRU )
        return ( *m_Record )[ 0 ] ;
    // TODO  else if ( m_RP == ROUND_ROBIN ) ;
    return -1 ;
}  //  ReplaceManager::GetReplaceIndex()

void CS::ReplaceManager::UpdateRecord( uint32_t index ) {
    if ( m_ReplacePolicy == CS::REPLACEPOLICY::LRU ) {
        for ( uint8_t i = 0; i < m_Record->size(); i++ ) {
            if ( ( *m_Record )[ i ] == index ) {
                m_Record->erase( m_Record->begin() + i ) ;
                m_Record->push_back( index ) ;
                return ;
            }  // if
        }  // for
    }  // if
       // TODO  else if ( m_RP == ROUND_ROBIN ) ;
}  //  ReplaceManager::UpdateRecord()

CS::Cache_Set::Cache_Set( uint32_t blocksize, uint32_t associativity, uint32_t replacePolicy, uint32_t writePolicy,
        uint8_t ReadLatency, uint8_t WriteLatency ) :
        m_BlockSize( blocksize ), m_Associativity( associativity ), m_WritePolicy( writePolicy ), m_RetentionTime( 0 ), m_ReadLatency( ReadLatency ), m_WriteLatency( WriteLatency ) {
    m_RP_Manager = new ReplaceManager( associativity, CS::REPLACEPOLICY::LRU ) ;
    m_Way = new Way[ m_Associativity ] ;
    m_UsingTime = 0 ;
    for ( uint32_t i = 0; i < m_Associativity; i++ ) {
        m_Way[ i ].mTag = -1 ;
        m_Way[ i ].Valid = false ;
        m_Way[ i ].Dirty = false ;
#ifdef SIM_DATA
        m_Way[ i ].mData = new Byte [ m_BlockSize ] ;
#else
        m_Way[ i ].mData = NULL ;
#endif
        m_Way[ i ].mTimeStamp = -1 ;
        m_Way[ i ].mTimeLog = new std::vector<uint64_t>() ;
    }  // for

}  // Cache_Set::Cache_Set

void CS::Cache_Set::ReadData( Byte * out, uint32_t way_index, uint32_t offset, uint32_t length ) {
    //if ( out != NULL )
        // memcpy( out, m_Way[ way_index ].mData + offset, length ) ;
    m_UsingTime++ ;
}  // Cache_Set::ReadLine()

void CS::Cache_Set::WriteData( Byte * in, uint32_t way_index, uint32_t offset, uint32_t length ) {
    //if ( in != NULL )
        // memcpy( m_Way[ way_index ].mData + offset, in, length ) ;
    m_UsingTime++ ;
}  // Cache_Set::WriteLine()

void CS::Cache_Set::AllocateData( Byte * in, uint64_t tag, uint32_t way_index, uint32_t offset, uint32_t length ) {
    if ( m_WritePolicy == CS::WRITEPOLICY::WRITE_BACK ) {
        m_Way[ way_index ].Valid = true ;
        m_Way[ way_index ].mTag = tag ;
        //if ( in != NULL )
            // memcpy( m_Way[ way_index ].mData + offset, in, length ) ;
    }  // if
}  // Cache_Set::WriteLine()

uint32_t CS::Cache_Set::FindTagInWay( uint64_t tag ) {

    for ( uint32_t i = 0; i < m_Associativity; i++ ) {
        if ( m_Way[ i ].mTag == tag && m_Way[ i ].Valid )
            return i ;
    }  // for
    return -1 ;
}  // Cache_Set::FindTagInWay()

