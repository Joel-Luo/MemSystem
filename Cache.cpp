#include "Cache.h"
#include "Log.h"

uint32_t Cache::floorLog2( uint32_t number ) {
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

Cache::Cache( uint32_t CacheName, uint32_t cache_size, uint32_t blocksize, uint32_t associativity,
        uint32_t replacePolicy, uint32_t writepolicy, uint32_t readlatency, uint32_t writelatnecy ) :
        m_Name( CacheName ), m_CacheSize( cache_size << 10 ), m_BlockSize( blocksize ), m_Num_W_Access( 0 ), m_Num_W_Hit( 0 ), m_Num_R_Access( 0 ), m_Num_R_Hit( 0 ), m_Sets(
        NULL ), m_Num_Set( 0 ), m_ReplacePolicy( replacePolicy ), m_WritePolicy( writepolicy ), m_ReadLatency( readlatency ), m_WriteLatency( writelatnecy ) {
    m_BlockSize_log2 = Cache::floorLog2( blocksize ) ;
    m_Associativity_log2 = Cache::floorLog2( associativity ) ;
    m_Num_Set = m_CacheSize >> ( m_BlockSize_log2 + m_Associativity_log2 ) ;
    m_Sets = new Cache_Set*[ m_Num_Set ] ;

    for ( int i = 0; i < m_Num_Set; i++ )
        m_Sets[ i ] = new Cache_Set( blocksize, associativity, replacePolicy, writepolicy ) ;

}  // Cache::Cache()

void Cache::SplitAddress( const uint64_t addr, uint64_t& tag, uint32_t& associ_index, uint32_t& block_offset ) {

    uint32_t mask_block = ( 1 << ( m_BlockSize_log2 ) ) - 1 ;
    uint32_t mask_associ = ( ( ( mask_block + 1 ) << m_Associativity_log2 ) - 1 ) ^ mask_block ;
    block_offset = addr & mask_block ;
    associ_index = ( addr & mask_associ ) >> m_BlockSize_log2 ;
    tag = addr >> ( m_BlockSize_log2 + m_Associativity_log2 ) ;

}  // Cache::SplitAddress()

bool Cache::AccessCache( uint32_t AccessType, const uint64_t address, Byte * Data, uint32_t length ) {

    uint64_t tag ;
    uint32_t index, block_offset ;
    SplitAddress( address, tag, index, block_offset ) ;

    uint32_t way_index = m_Sets[ index ]->FindTagInWay( tag ) ;
    if ( way_index == -1 )
        return false ;  // cache miss

    if ( AccessType == Cache::READ ) {

        m_Sets[ index ]->ReadData( Data, way_index, block_offset, length ) ;
    }  // if

    else if ( AccessType == Cache::WRITE ) {
        if ( m_WritePolicy == WRITE_BACK )
            m_Sets[ index ]->m_Way[ way_index ].Dirty = true ;
        m_Sets[ index ]->WriteData( Data, way_index, block_offset, length ) ;
    }  // else if

    m_Sets[ index ]->m_RP_Manager->UpdateRecord( way_index ) ;
    return true ;
}  // Cache::AccessSingleLine

uint64_t Cache::TagToAddress( uint64_t tag, uint32_t index ) {
    uint64_t address ;
    address = ( tag << m_Associativity_log2 ) + index ;
    return address ;
}  // Cache::TagToAddress()

bool Cache::AllocateCache( uint32_t set_index ) {

    uint8_t way_index = m_Sets[ set_index ]->m_RP_Manager->GetReplaceIndex() ;
    Cache_Set::Way wayentry = m_Sets[ set_index ]->m_Way[ way_index ] ;
    if ( wayentry.Dirty )
        return false ;  // Allocation is not successful.
    else
        return true ;

}  // Cache::AllocateCache

void Cache::LoadCacheBlock( uint64_t tag, uint32_t set_index, Byte * in ) {

    uint8_t way_index = m_Sets[ set_index ]->m_RP_Manager->GetReplaceIndex() ;
    m_Sets[ set_index ]->m_Way[ way_index ].Valid = true ;
    m_Sets[ set_index ]->m_Way[ way_index ].tag = tag ;
    m_Sets[ set_index ]->m_RP_Manager->UpdateRecord( way_index ) ;
    m_Sets[ set_index ]->WriteData( in, way_index, 0, m_BlockSize ) ;

}  // Cache::LoadCacheBlock()

void Cache::StoreCacheBlock( uint64_t tag, uint32_t set_index, uint64_t & TatgetAddr, Byte * out ) {

    uint8_t way_index = m_Sets[ set_index ]->m_RP_Manager->GetReplaceIndex() ;
    m_Sets[ set_index ]->m_Way[ way_index ].Valid = false ;
    m_Sets[ set_index ]->ReadData( out, way_index, 0, m_BlockSize ) ;

}  // Cache::StoreCacheBlock()

