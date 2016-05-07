#include "Cache.h"
#include "Log.h"
Cache::Cache( uint32_t CacheName,
			  uint32_t cache_size,
			  uint32_t blocksize,
			  uint32_t associativity,
			  uint32_t replacePolicy,
			  uint32_t writepolicy ) :
			  m_Name( CacheName ),
			  m_CacheSize(cache_size * 1024),
			  m_Num_Access(0),
			  m_Num_Hit(0),
	          m_Sets( NULL),
			  m_num_set(0)
{
  uint32_t blocksize_log2 = Cache_Set::floorLog2( blocksize ) ;
  uint32_t associativity_log2 = Cache_Set::floorLog2( associativity ) ;
  m_num_set = m_CacheSize >> ( blocksize_log2 + associativity_log2 ) ;
  m_Sets = new Cache_Set*[m_num_set] ;
  for ( int i = 0 ; i < m_num_set ; i++  )
	  m_Sets[i] = new Cache_Set( blocksize_log2, associativity) ;
} // Cache::Cache()

Cache::Cache( uint32_t CacheName,
			  uint32_t cache_size,
			  uint32_t blocksize,
			  uint32_t replacePolicy,
			  uint32_t writepolicy ) :
			  m_Name( CacheName ),
			  m_CacheSize(cache_size * 1024),
			  m_Num_Access(0),
			  m_Num_Hit(0),
	          m_Sets( NULL),
			  m_num_set(0){

  uint32_t blocksize_log2 = Cache_Set::floorLog2( blocksize ) ;
  m_num_set = m_CacheSize >> blocksize_log2 ;
  m_Sets = new Cache_Set*[m_num_set] ;

  for ( int i = 0 ; i < m_num_set ; i++  )
	  m_Sets[i] = new Cache_Set( blocksize_log2, 0 ) ;

} // Cache::Cache()
