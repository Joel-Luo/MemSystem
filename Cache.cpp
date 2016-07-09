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

Cache::Cache( uint32_t CacheName, uint8_t CacheType, uint32_t cache_size, uint32_t blocksize, uint32_t associativity,
        uint32_t replacePolicy, uint32_t writepolicy, uint8_t readlatency, uint8_t writelatnecy ) :
        m_Name( CacheName ), m_CacheType( CacheType ), m_CacheSize( cache_size << 10 ), m_BlockSize( blocksize ), m_Num_W_Access( 0 ), m_Num_W_Hit( 0 ), m_Num_R_Access( 0 ), m_Num_R_Hit( 0 ), m_Sets(
        NULL ), m_Num_Set( 0 ), m_ReplacePolicy( replacePolicy ), m_WritePolicy( writepolicy ), m_ReadLatency( readlatency ), m_WriteLatency( writelatnecy ) {
    m_BlockSize_log2 = Cache::floorLog2( blocksize ) ;
    m_Associativity_log2 = Cache::floorLog2( associativity ) ;
    m_Num_Set = m_CacheSize >> ( m_BlockSize_log2 + m_Associativity_log2 ) ;
    m_Sets = new Cache_Set*[ m_Num_Set ] ;

    if ( CacheType == CACHE ||CacheType == BUFFERCACHE )
        for ( int i = 0; i < m_Num_Set; i++ )
            m_Sets[ i ] =
                    new Cache_Set( blocksize, associativity, replacePolicy, writepolicy, readlatency, writelatnecy ) ;

}  // Cache::Cache()

void Cache::BuildHybridCache( uint8_t numofcellType, uint8_t numofsub, uint8_t * size, uint32_t * retentiontime,
        uint8_t * readlatency, uint8_t * writelatency, uint32_t blocksize, uint32_t associativity,
        uint32_t replacePolicy, uint32_t writepolicy ) {

    int setsize = 0 ;
    int nowIndex = 0 ;
    for ( int i = 0; i < numofcellType; i++ ) {
        setsize += ( m_Num_Set / numofsub ) * size[ i ] ;
        for ( ; nowIndex < setsize; nowIndex++ )
            m_Sets[ nowIndex ] =
                    new Cache_Set( blocksize, associativity, replacePolicy, writepolicy, readlatency[ i ], writelatency[ i ], retentiontime[ i ] ) ;
    }  // for
    delete[] size ;
    delete[] retentiontime ;
    delete[] readlatency ;
    delete[] writelatency ;
}  // Cache::BuildHybridCache

void Cache::SplitAddress( const uint64_t addr, uint64_t& tag, uint32_t& associ_index, uint32_t& block_offset ) {

    uint32_t mask_block = ( 1 << ( m_BlockSize_log2 ) ) - 1 ;
    uint32_t mask_associ = ( ( ( mask_block + 1 ) << m_Associativity_log2 ) - 1 ) ^ mask_block ;
    block_offset = addr & mask_block ;
    associ_index = ( addr & mask_associ ) >> m_BlockSize_log2 ;
    tag = addr >> ( m_BlockSize_log2 + m_Associativity_log2 ) ;

}  // Cache::SplitAddress()

bool Cache::AccessCache( uint32_t AccessType, const uint64_t accessTime, const uint64_t address, Byte * Data,
        uint32_t length ) {

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
        UpdateTimeStamp( index, way_index, accessTime ) ;
        if ( m_WritePolicy == WRITE_BACK )
            m_Sets[ index ]->m_Way[ way_index ].Dirty = true ;
        m_Sets[ index ]->WriteData( Data, way_index, block_offset, length ) ;
    }  // else if

    m_Sets[ index ]->m_RP_Manager->UpdateRecord( way_index, true ) ;

    return true ;
}  // Cache::AccessSingleLine

bool Cache::AccessHybridCache( uint32_t AccessType, const uint64_t accessTime, const uint64_t address, Byte * Data,
        uint32_t length, bool & timeUpWriteBack, Byte * WBData ) {

    uint64_t tag ;
    uint32_t index, block_offset ;
    SplitAddress( address, tag, index, block_offset ) ;

    uint32_t way_index = m_Sets[ index ]->FindTagInWay( tag ) ;

    if ( m_CacheType == HYBRIDCACHE && way_index != -1 )
        timeUpWriteBack = RetentionTimeUp( index, way_index, accessTime, WBData ) ;

    if ( way_index == -1 )
        return false ;  // cache miss

    if ( AccessType == Cache::READ ) {
        m_Sets[ index ]->ReadData( Data, way_index, block_offset, length ) ;
    }  // if

    else if ( AccessType == Cache::WRITE ) {
        UpdateTimeStamp( index, way_index, accessTime ) ;
        if ( m_WritePolicy == WRITE_BACK )
            m_Sets[ index ]->m_Way[ way_index ].Dirty = true ;
        m_Sets[ index ]->WriteData( Data, way_index, block_offset, length ) ;
    }  // else if

    m_Sets[ index ]->m_RP_Manager->UpdateRecord( way_index, true ) ;
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
    m_Sets[ set_index ]->m_Way[ way_index ].mTag = tag ;
    m_Sets[ set_index ]->m_RP_Manager->UpdateRecord( way_index, true ) ;
    m_Sets[ set_index ]->WriteData( in, way_index, 0, m_BlockSize ) ;

}  // Cache::LoadCacheBlock()

void Cache::StoreCacheBlock( uint32_t set_index, uint64_t & TatgetAddr, Byte * out ) {

    uint8_t way_index = m_Sets[ set_index ]->m_RP_Manager->GetReplaceIndex() ;
    m_Sets[ set_index ]->m_Way[ way_index ].Valid = false ;
    m_Sets[ set_index ]->ReadData( out, way_index, 0, m_BlockSize ) ;

}  // Cache::StoreCacheBlock()

bool Cache::RetentionTimeUp( uint32_t set_index, uint32_t & wayindex, uint64_t accessTime, Byte * out ) {

    uint64_t lasttime = m_Sets[ set_index ]->m_Way[ wayindex ].mTimeStamp ;
    if ( lasttime == -1 )
        return false ;
    uint64_t timelength = accessTime - lasttime ;
    if ( timelength >= m_Sets[ set_index ]->m_RetentionTime ) {
        m_Sets[ set_index ]->ReadData( out, wayindex, 0, m_BlockSize ) ;
        m_Sets[ set_index ]->m_Way[ wayindex ].Valid = false ;
        m_Sets[ set_index ]->m_RP_Manager->UpdateRecord( wayindex, false ) ;
        wayindex = -1 ;
        return true ;
    }  // if
    else
        return false ;

}  // Cache::CalRTime()
void Cache::UpdateTimeStamp( uint32_t set_index, uint32_t wayindex, uint64_t accessTime ) {
    m_Sets[ set_index ]->m_Way[ wayindex ].mTimeStamp = accessTime ;
}  // Cache::UpdateTimeStamp()

BufferCache::BufferCache( uint8_t NumOfEntry, uint8_t DataLength, uint8_t ReadLatency, uint8_t WriteLatency ) :
        mNumOfEntry( NumOfEntry ), mDataLength( DataLength ), m_ReadLatency( ReadLatency ), m_WriteLatency( WriteLatency ) {
    mBufferSet = new BufferSet*[ mNumOfEntry ] ;
    mAddtionWriteLantency = 0 ;

    m_Num_W_Access = 0 ;
    m_Num_W_Hit = 0 ;
    m_Num_R_Access = 0 ;
    m_Num_R_Hit = 0 ;

    mBufferQueue = new std::vector < uint8_t >() ;
    mBufferNonUseQueue = new std::vector < uint8_t > ;
    for ( int i = 0; i < mNumOfEntry; i++ ) {
        mBufferNonUseQueue->push_back( i ) ;
        mBufferSet[ i ] = new BufferSet ;
        mBufferSet[ i ]->mAddress = 0 ;
        mBufferSet[ i ]->mAccessTime = 0 ;
#ifdef SIM_DATA
        mBufferSet[i].mData = new Byte [ mDataLength ] ;
#else
        mBufferSet[ i ]->mData = NULL ;
#endif
    }  // for
}  // BufferCache::BufferCache

std::vector < uint8_t > * BufferCache::GetRequestToCache( std::vector < uint8_t > * list, uint64_t nowTime ) {

    if ( mBufferQueue->size() != 0 ) {
        while ( mBufferQueue->size() != 0  ) {
            if ( nowTime - mBufferSet[ mBufferQueue->at( 0 ) ]->mAccessTime > m_WriteLatency ) {
                uint8_t index = mBufferQueue->at( 0) ;
                list->push_back(  index ) ;
                mBufferQueue->erase( mBufferQueue->begin() ) ;
                mBufferNonUseQueue->push_back( index ) ;
            }  // if
            else break ;
        }  // for
    }  // if

    return list ;
}  // BufferCache::PushRequestToCache()

bool BufferCache::BufferAccess( const uint64_t accessTime, const uint64_t address, const uint32_t AccessType,
        Byte * Data, uint32_t length ) {

    if ( AccessType == Cache::READ ) {

        for ( int i = 0; i < mBufferQueue->size(); i++ ) {
            if ( mBufferSet[ mBufferQueue->at( i ) ]->mAddress == address ) {  // hit
                if ( Data != NULL )
                    memcpy( Data, mBufferSet[ mBufferQueue->at( i ) ]->mData, length ) ;
                return true ;
            }  // if
        }  // for

    }  // if

    else {  // Write
        int8_t find = -1 ;
        for ( int i = 0; i < mBufferQueue->size(); ) {
            if ( mBufferSet[ mBufferQueue->at( i ) ]->mAddress == address ) {  // hit
                uint8_t index = mBufferQueue->at( i ) ;
                mBufferQueue->erase( mBufferQueue->begin() + i ) ;
                mBufferNonUseQueue->push_back( index ) ;
                find = index ;
            }  // if
            else
                i++ ;
        }  // for
        if ( find != -1 ) {
            mBufferQueue->push_back( find ) ;
            if ( Data != NULL )
                memcpy( mBufferSet[ find ]->mData, Data, length ) ;
            return true ;
        }  // if
    }  // else

    return false ;
}  //

uint8_t BufferCache::AllocatBufferEntry( uint64_t nowTime, uint32_t &AddLatency, bool & isBufferFull ) {
    uint8_t index = 0 ;

    if ( mBufferNonUseQueue->size() == 0 ) {  // buffer full
        isBufferFull = true ;
        index = mBufferQueue->at( 0 ) ;
        uint64_t timeLength = nowTime - mBufferSet[ index ]->mAccessTime ;
        if ( timeLength > m_WriteLatency )
            AddLatency = m_WriteLatency - timeLength ;
        mBufferQueue->erase( mBufferQueue->begin() ) ;
        mBufferNonUseQueue->push_back( index ) ;
    }  // if
    else {
        index = mBufferNonUseQueue->at( 0 ) ;
    }  // else
    return index ;
}

void BufferCache::PushRequestToBuffer( const uint8_t pushIndex, const uint64_t accessTime, const uint64_t address,
        Byte * Data ) {
    mBufferQueue->push_back( pushIndex ) ;
    mBufferNonUseQueue->erase( mBufferNonUseQueue->begin() ) ;
    mBufferSet[ pushIndex ]->mAccessTime = accessTime ;
    mBufferSet[ pushIndex ]->mAddress = address ;
    if ( Data != NULL )
        memcpy( mBufferSet[ pushIndex ]->mData, Data, mDataLength ) ;
}  //

void Cache::BuildBufferCache( uint8_t numofentry, uint8_t DataLength, uint8_t readlatency, uint8_t writelatency ) {
    mBufferCache = new BufferCache(numofentry, DataLength, readlatency, writelatency )  ;

}  // Cache::BuildBufferCache
