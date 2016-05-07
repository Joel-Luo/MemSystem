#include "Cache.h"
#include "Log.h"
Cache::Cache( uint32_t CacheName,
			  uint32_t cache_size,
			  uint32_t blocksize,
			  uint32_t associativity,
			  uint32_t replacePolicy,
			  uint32_t writepolicy ) :
			  m_Name( CacheName ),
			  m_CacheSize(cache_size << 10),
			  m_BlockSize( blocksize ),
			  m_Num_Access(0),
			  m_Num_Hit(0),
	          m_Sets( NULL),
			  m_Num_Set(0),
			  m_ReplacePolicy( replacePolicy ),
			  m_WrtiePolicy( writepolicy)
{
  uint32_t blocksize_log2 = Cache_Set::floorLog2( blocksize ) ;
  uint32_t associativity_log2 = Cache_Set::floorLog2( associativity ) ;
  m_Num_Set = m_CacheSize >> ( blocksize_log2 + associativity_log2 ) ;
  m_Sets = new Cache_Set*[m_Num_Set] ;
  for ( int i = 0 ; i < m_Num_Set ; i++  )
	  m_Sets[i] = new Cache_Set( blocksize_log2, associativity) ;


} // Cache::Cache()

Cache::Cache( uint32_t CacheName,
			  uint32_t cache_size,
			  uint32_t blocksize,
			  uint32_t replacePolicy,
			  uint32_t writepolicy ) :
			  m_Name( CacheName ),
			  m_CacheSize(cache_size << 10),
			  m_BlockSize( blocksize ),
			  m_Num_Access(0),
			  m_Num_Hit(0),
	          m_Sets( NULL),
			  m_Num_Set(0),
			  m_ReplacePolicy( replacePolicy ),
			  m_WrtiePolicy( writepolicy)
{

  uint32_t blocksize_log2 = Cache_Set::floorLog2( blocksize ) ;
  m_Num_Set = m_CacheSize >> blocksize_log2 ;
  m_Sets = new Cache_Set*[m_Num_Set] ;

  for ( int i = 0 ; i < m_Num_Set ; i++  )
	  m_Sets[i] = new Cache_Set( blocksize_log2, 0 ) ;

} // Cache::Cache()
