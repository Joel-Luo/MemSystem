#include "Cache.h"
#include "Log.h"

uint32_t Cache::floorLog2( uint32_t number ) {
	int p = 0 ;
	if ( number == 0 ) return -1 ;
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
	} // if
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
		uint32_t replacePolicy, uint32_t writepolicy ) :
		m_Name( CacheName ), m_CacheSize( cache_size << 10 ), m_BlockSize( blocksize ), m_Num_Access(
				0 ), m_Num_Hit( 0 ), m_Sets( NULL ), m_Num_Set( 0 ), m_ReplacePolicy(
				replacePolicy ), m_WrtiePolicy( writepolicy ), m_NeedWriteBack( false ) {
	m_BlockSize_log2 = Cache::floorLog2( blocksize ) ;
	m_Associativity_log2 = Cache::floorLog2( associativity ) ;
	m_Num_Set = m_CacheSize >> ( m_BlockSize_log2 + m_Associativity_log2 ) ;
	m_Sets = new Cache_Set*[ m_Num_Set ] ;
	RP_record = new std::vector < int >() ;
	for ( int i = 0; i < m_Num_Set; i++ )
		m_Sets[ i ] = new Cache_Set( blocksize, associativity ) ;

} // Cache::Cache()

Cache::Cache( uint32_t CacheName, uint32_t cache_size, uint32_t blocksize, uint32_t replacePolicy,
		uint32_t writepolicy ) :
		m_Name( CacheName ), m_CacheSize( cache_size << 10 ), m_BlockSize( blocksize ), m_Num_Access(
				0 ), m_Num_Hit( 0 ), m_Sets( NULL ), m_Num_Set( 0 ), m_ReplacePolicy(
				replacePolicy ), m_WrtiePolicy( writepolicy ), m_NeedWriteBack( false ) {

	m_BlockSize_log2 = Cache::floorLog2( blocksize ) ;
	m_Associativity_log2 = 0 ;
	m_Num_Set = m_CacheSize >> m_BlockSize_log2 ;
	m_Sets = new Cache_Set*[ m_Num_Set ] ;
	RP_record = new std::vector < int >() ;
	for ( int i = 0; i < m_Num_Set; i++ )
		m_Sets[ i ] = new Cache_Set( blocksize, 0 ) ;

} // Cache::Cache()

void Cache::SplitAddress( const uint64_t addr, uint64_t& tag, uint32_t& associ_index,
		uint32_t& block_offset ) {
	uint32_t mask_block = ( 1 << ( m_BlockSize_log2 ) ) - 1 ;
	uint32_t mask_associ = ( ( ( mask_block + 1 ) << m_Associativity_log2 ) - 1 ) ^ mask_block ;

	associ_index = ( addr & mask_associ ) >> m_BlockSize_log2 ;
	block_offset = addr & mask_block ;
	tag = addr >> ( m_BlockSize_log2 + m_Associativity_log2 ) ;
} // Cache::SplitAddress()

void Cache::AccessSingleLine( bool hit, uint32_t AccessType, const uint64_t address, Byte * Data,
		uint32_t length ) {
	m_Num_Access++ ;
	if ( hit ) m_Num_Hit++ ;

	uint64_t tag ;
	uint32_t associ_index, block_offset ;
	SplitAddress( address, tag, associ_index, block_offset ) ;
	int i = FindTagSet( tag ) ;
	if ( AccessType == Cache::READ ) {  // READ
		m_Sets[ i ]->ReadLine( Data, associ_index * m_BlockSize + block_offset, length ) ;
	}  // if
	else {  // WRITE
		m_Sets[ i ]->WriteLine( Data, associ_index * m_BlockSize + block_offset, length ) ;
	}  // else
} // Cache::AccessSingleLine

bool Cache::PeekSingleLine( const uint64_t addr ) {
	uint64_t tag ;
	uint32_t associ_index, block_offset ;
	SplitAddress( addr, tag, associ_index, block_offset ) ;
	int i = FindTagSet( tag ) ;
	return ( i == -1 ) ? false : true ;
} // Cache::AccessSingleLine

uint32_t Cache::FindTagSet( uint32_t tag ) {

	for ( int i = 0; i < m_Num_Set; i++ ) {
		if ( m_Sets[ i ]->GetTag() == tag ) return i ;
	}  // for

	return -1 ;

}  // Cache::FindTagSet

uint32_t Cache::MaintainLRU( uint32_t index ) {

	for ( int i = 0; index != -1 && i < RP_record->size(); i++ ) {
		if ( RP_record->at( i ) == index ) {
			RP_record->erase( RP_record->begin() ) ;
			RP_record->insert( RP_record->begin(), index ) ;
			return index ;
		} // if
	}  // for

	if ( RP_record->size() != m_Num_Set ) {
		int index = RP_record->size() ;
		RP_record->insert( RP_record->begin(), index ) ;
		return index ;
	}  // if
	else {
		int i = RP_record->at( RP_record->size() - 1 ) ;
		RP_record->insert( RP_record->begin(), i ) ;
		m_NeedWriteBack = true ;
		return i ;
	}  // else
	return -1 ;
}

bool Cache::ReplaceSet( uint32_t set_index, const uint64_t address ) {

	uint64_t tag ;
	uint32_t associ_index, block_offset ;
	SplitAddress( address, tag, associ_index, block_offset ) ;
	m_Sets[ set_index ]->SetTag( tag ) ;

	return m_NeedWriteBack ;

}

uint64_t Cache::AppendAddress( Cache_Set * cache_set ) {
	uint64_t addr = cache_set->GetTag() ;
	addr = addr << ( m_BlockSize_log2 + m_Associativity_log2 ) ;
	return addr ;
}  // Cache::AppendAddress()

uint32_t Cache::MaintainReplacePolicy( const uint64_t address ) {

	uint64_t tag ;
	uint32_t associ_index, block_offset ;
	SplitAddress( address, tag, associ_index, block_offset ) ;
	uint32_t i = FindTagSet( tag ) ;

	if ( m_ReplacePolicy == LRU ) return MaintainLRU( i ) ;
	return -1 ;
} //

