#include "Cache_Set.h"
#include <string.h>

Cache_Set::Cache_Set( uint32_t blocksize, uint32_t associativity ) :
        m_BlockSize( blocksize ), m_Associativity( associativity ), m_tag( -1 ) {
    m_Data = NULL ;

}

void Cache_Set::ReadLine( Byte * out, const uint32_t offset, uint32_t length ) {
    if ( out != NULL ) memcpy( out, m_Data + length, length ) ;
}  // Cache_Set::ReadLine()

void Cache_Set::WriteLine( Byte * in, uint32_t offset, uint32_t length ) {
    if ( in != NULL ) memcpy( m_Data + length, in, length ) ;
}  // Cache_Set::WriteLine()
