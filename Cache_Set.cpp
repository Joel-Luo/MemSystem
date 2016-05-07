#include "Cache_Set.h"

uint32_t Cache_Set::floorLog2( uint32_t number ) {
	int p = 0;
		if (number == 0) return -1;
		if (number & 0xffff0000) {
			p += 16;
			number >>= 16;
		}  // if
		if (number & 0x0000ff00) {
			p +=  8;
			number >>=  8;
		}  //if
		if (number & 0x000000f0) {
			p +=  4;
			number >>=  4;
		} // if
		if (number & 0x0000000c) {
			p +=  2;
			number >>=  2;
		}  // if
		if (number & 0x00000002) {
			p +=  1;
		}  // if
		return p;
}  // Cache_Set::floorLog2()

Cache_Set::Cache_Set( uint32_t blocksize_log2,
		              uint32_t associativity ) :
					  m_BlockSize_log2( blocksize_log2 ),
					  m_Associativity( associativity ),
					  m_tag( 0 )
{
  Data = NULL ;
}
