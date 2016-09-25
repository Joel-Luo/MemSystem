#include "Cache.h"
#include "Log.h"

uint32_t CS::Cache::floorLog2( uint32_t number ) {
    int p = 0 ;
    if ( number == 0 )
        return -1 ;
    if ( number & 0xffff0000 ) {
        p += 16 ;
        number >>= 16 ;
    }  // if
    if ( number & 0x0000ff00 ) {
        p += 8 ;
        number >>= 8 ;
    }  //if
    if ( number & 0x000000f0 ) {
        p += 4 ;
        number >>= 4 ;
    }  // if
    if ( number & 0x0000000c ) {
        p += 2 ;
        number >>= 2 ;
    }  // if
    if ( number & 0x00000002 ) {
        p += 1 ;
    }  // if
    return p ;
}  // Cache::floorLog2()

CS::Cache::Cache( uint32_t CacheName, uint8_t CacheType, uint32_t cache_size, uint32_t blocksize, uint32_t associativity,
        uint32_t replacePolicy, uint32_t writepolicy,  uint8_t readlatency, uint8_t writelatnecy ) :
         m_CacheType( CacheType ), m_Name( CacheName ), m_CacheSize( cache_size << 10 ), m_BlockSize( blocksize ), m_Num_W_Access( 0 ),  m_Num_W_Hit( 0 ), m_Num_R_Access( 0 ), m_Num_R_Hit( 0 ), m_Num_Way( associativity ),m_Sets(
        NULL ), m_Num_Set( 0 ), m_ReplacePolicy( replacePolicy ), m_WritePolicy( writepolicy ), m_ReadLatency( readlatency ), m_WriteLatency( writelatnecy ) {
    mEnableRecord = false ;

    m_BlockSize_log2 = Cache::floorLog2( blocksize ) ;
    m_Associativity_log2 = Cache::floorLog2( associativity ) ;
    m_Num_Set = m_CacheSize >> ( m_BlockSize_log2 + m_Associativity_log2 ) ;
    m_Num_Set_Log2 = Cache::floorLog2( m_Num_Set ) ;
    m_Sets = new Cache_Set*[ m_Num_Set ] ;

    if ( CacheType == CS::CACHETYPE::NORMAL )
        for ( uint32_t i = 0; i < m_Num_Set; i++ )
            m_Sets[ i ] =
                    new Cache_Set( blocksize, associativity, replacePolicy, writepolicy, readlatency, writelatnecy ) ;

}  // Cache::Cache()


bool CS::Cache::AccessCache( uint32_t AccessType, const uint64_t accessTime, const uint64_t address, Byte * Data,
        uint32_t length ) {

    uint64_t tag ;
    uint32_t index, block_offset ;
    SplitAddress( address, tag, index, block_offset ) ;


    uint32_t way_index = m_Sets[ index ]->FindTagInWay( tag ) ;

    if ( way_index == (uint32_t)-1 )
        return false ;  // cache miss

    if ( AccessType == CS::ACCESSTYPE::READ ) {
        m_Sets[ index ]->ReadData( Data, way_index, block_offset, length ) ;
    }  // if

    else if ( AccessType == CS::ACCESSTYPE::WRITE ) {
        if ( m_WritePolicy == CS::WRITEPOLICY::WRITE_BACK )
            m_Sets[ index ]->m_Way[ way_index ].Dirty = true ;
        m_Sets[ index ]->WriteData( Data, way_index, block_offset, length ) ;
    }  // else if

    m_Sets[ index ]->m_RP_Manager->UpdateRecord( way_index ) ;

    return true ;
}  // Cache::AccessSingleLine

void CS::Cache::SplitAddress( const uint64_t addr, uint64_t& tag, uint32_t& set_index, uint32_t& block_offset ) {

    uint32_t mask_block = ( 1 << ( m_BlockSize_log2 ) ) - 1 ;
    uint32_t mask_setindex = ( ( ( mask_block + 1 ) << m_Num_Set_Log2 ) - 1 ) ^ mask_block ;
    block_offset = addr & mask_block ;
    set_index = ( addr & mask_setindex ) >> m_BlockSize_log2 ;
    tag = addr >> ( m_BlockSize_log2 + m_Num_Set_Log2 ) ;

}  // Cache::SplitAddress()

uint64_t CS::Cache::TagToAddress( uint64_t tag, uint32_t index ) {
    uint64_t address ;
    address = ( tag << m_Num_Set_Log2) + index  ;
    return address << m_BlockSize_log2 ;
}  // Cache::TagToAddress()

bool CS::Cache::AllocateCache( uint32_t set_index ) {

    uint8_t way_index = m_Sets[ set_index ]->m_RP_Manager->GetReplaceIndex() ;
    Cache_Set::Way wayentry = m_Sets[ set_index ]->m_Way[ way_index ] ;
    if ( wayentry.Dirty )
        return false ;  // Allocation is not successful.
    else
        return true ;

}  // Cache::AllocateCache

void CS::Cache::LoadCacheBlock( uint64_t tag, uint32_t set_index, Byte * in ) {

    uint8_t way_index = m_Sets[ set_index ]->m_RP_Manager->GetReplaceIndex() ;
    m_Sets[ set_index ]->m_Way[ way_index ].Valid = true ;
    m_Sets[ set_index ]->m_Way[ way_index ].mTag = tag ;
    m_Sets[ set_index ]->m_RP_Manager->UpdateRecord( way_index ) ;
    m_Sets[ set_index ]->WriteData( in, way_index, 0, m_BlockSize ) ;

}  // Cache::LoadCacheBlock()

void CS::Cache::StoreCacheBlock( uint32_t set_index, uint64_t & TatgetAddr, Byte * out ) {

    uint8_t way_index = m_Sets[ set_index ]->m_RP_Manager->GetReplaceIndex() ;
    m_Sets[ set_index ]->m_Way[ way_index ].Valid = false ;
    m_Sets[ set_index ]->ReadData( out, way_index, 0, m_BlockSize ) ;
    TatgetAddr = TagToAddress( m_Sets[ set_index ]->m_Way[ way_index ].mTag, set_index  ) ;
}  // Cache::StoreCacheBlock()

CS::GTable::GTable( uint32_t Size, uint8_t ReplacePolicy, uint32_t thershold ) : m_Size( Size ), m_Thershold( thershold) {
    m_GTable = new std::vector< Entry* > () ;
    mRP = new CS::ReplaceManager( m_Size, ReplacePolicy ) ;
    for ( uint32_t i = 0 ; i < m_Size ; i++ ) {
        Entry * temp = new Entry ;
        temp->mTag = -1 ;
        temp->times = -1 ;
        m_GTable->push_back( temp ) ;
    }  // for
}   // CS::GTable()

int32_t CS::GTable::SearchTable( uint64_t tag ) {
    for ( uint32_t i = 0 ; i < m_Size ; i++ )
        if ( m_GTable->at(i)->mTag == tag )
          return i ;
    return -1 ;
}  // CS::GTable::SearchTable()

void CS::GTable::UpdateTable( uint32_t index, uint64_t tag, uint32_t times ) {

    Entry * en = m_GTable->at( index ) ;

    if ( en->mTag != tag ) en->mTag = tag ;

    en->times = times ;

}  // CS::GTable::UpdateTable()

bool CS::GTable::GTableController( uint64_t tag ) {

    int32_t index = SearchTable( tag ) ;
    if ( index == -1 ) {
        UpdateTable( mRP->GetReplaceIndex(), tag, 1 ) ;
        mRP->UpdateRecord( index ) ;
        return true ;
    }  // if

    else {
        Entry * en = m_GTable->at( index ) ;
        UpdateTable( index, tag, en->times++ ) ;
        mRP->UpdateRecord( index ) ;

        if ( en->times < m_Thershold )
            return true ;
        else
            return false ;
    }  // else

    return true ;
}  // CS::GTable::GTableController()


