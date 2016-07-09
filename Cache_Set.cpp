#include "Cache_Set.h"
#include <string.h>
#include <stdio.h>

ReplaceManager::ReplaceManager( uint32_t way, uint32_t rp ) :
        m_ReplacePolicy( rp ) {
    m_Record = new std::vector < uint8_t >() ;
    if ( m_ReplacePolicy == LRU )
        for ( int i = 0; i < way; i++ )
            m_Record->push_back( i ) ;
    // TODO  else if ( m_RP == ROUND_ROBIN ) ;
}  // ReplaceManager::ReplaceManager()

uint8_t ReplaceManager::GetReplaceIndex() {
    if ( m_ReplacePolicy == LRU )
        return ( *m_Record )[ 0 ] ;
    // TODO  else if ( m_RP == ROUND_ROBIN ) ;
    return -1 ;
}  //  ReplaceManager::GetReplaceIndex()

void ReplaceManager::UpdateRecord( uint8_t index, bool Endposition ) {
    if ( m_ReplacePolicy == LRU ) {
        for ( int i = 0; i < m_Record->size(); i++ ) {
            if ( ( *m_Record )[ i ] == index ) {
                m_Record->erase( m_Record->begin() + i ) ;
                if ( Endposition )
                  m_Record->push_back( index ) ;
                else
                    m_Record->insert( m_Record->begin(), index ) ;
                return ;
            }  // if
        }  // for
    }  // if
       // TODO  else if ( m_RP == ROUND_ROBIN ) ;
}  //  ReplaceManager::UpdateRecord()


Cache_Set::Cache_Set( uint32_t blocksize, uint32_t associativity, uint32_t replacePolicy, uint32_t writePolicy, uint8_t ReadLatency, uint8_t WriteLatency  ) :
        m_BlockSize( blocksize ), m_Associativity( associativity ), m_WritePolicy( writePolicy ), m_RetentionTime(0), m_ReadLatency(ReadLatency), m_WriteLatency(WriteLatency){
    m_RP_Manager = new ReplaceManager( associativity, REPLACEPOLICY::LRU ) ;
    m_Way = new Way[ m_Associativity ] ;
    for ( int i = 0; i < m_Associativity; i++ ) {
        m_Way[ i ].mTag = -1 ;
        m_Way[ i ].Valid = false ;
        m_Way[ i ].Dirty = false ;
#ifdef SIM_DATA
        m_Way[ i ].mData = new Byte [ m_BlockSize ] ;
#else
        m_Way[ i ].mData = NULL ;
#endif
        m_Way[i].mTimeStamp = -1 ;
    }  // for

}  // Cache_Set::Cache_Set

Cache_Set::Cache_Set( uint32_t blocksize, uint32_t associativity, uint32_t replacePolicy, uint32_t writePolicy, uint8_t ReadLatency, uint8_t WriteLatency, uint32_t retentionTime  ) :
        m_BlockSize( blocksize ), m_Associativity( associativity ), m_WritePolicy( writePolicy ), m_RetentionTime( retentionTime ), m_ReadLatency(ReadLatency), m_WriteLatency(WriteLatency) {
    m_RP_Manager = new ReplaceManager( associativity, REPLACEPOLICY::LRU ) ;
    m_Way = new Way[ m_Associativity ] ;
    for ( int i = 0; i < m_Associativity; i++ ) {
        m_Way[ i ].mTag = -1 ;
        m_Way[ i ].Valid = false ;
        m_Way[ i ].Dirty = false ;
#ifdef SIM_DATA
        m_Way[ i ].mData = new Byte [ m_BlockSize ] ;
#else
        m_Way[ i ].mData = NULL ;
#endif
        m_Way[i].mTimeStamp = 0 ;
    }  // for

}  // Cache_Set::Cache_Set
void Cache_Set::ReadData( Byte * out, uint32_t way_index, uint32_t offset, uint32_t length ) {
    if ( out != NULL )
        memcpy( out, m_Way[ way_index ].mData + offset, length ) ;
}  // Cache_Set::ReadLine()

void Cache_Set::WriteData( Byte * in, uint32_t way_index, uint32_t offset, uint32_t length ) {
    if ( in != NULL )
        memcpy( m_Way[ way_index ].mData + offset, in, length ) ;
}  // Cache_Set::WriteLine()

void Cache_Set::AllocateData( Byte * in, uint64_t tag, uint32_t way_index, uint32_t offset, uint32_t length ) {
    m_Way[ way_index ].Valid = true ;
    m_Way[ way_index ].mTag = tag ;
    if ( in != NULL )
        memcpy( m_Way[ way_index ].mData + offset, in, length ) ;
}  // Cache_Set::WriteLine()

uint32_t Cache_Set::FindTagInWay( uint64_t tag ) {
    for ( int i = 0; i < m_Associativity; i++ ) {
        if ( m_Way[ i ].mTag == tag && m_Way[ i ].Valid )
            return i ;
    }  // for
    return -1 ;
}  // Cache_Set::FindTagInWay()

