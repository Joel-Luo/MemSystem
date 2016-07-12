/*
 * CfgParser.cpp
 *
 *  Created on: May 6, 2016
 *      Author: LuoJing-Yuan
 */
#include "CfgParser.h"
#include "Log.h"
#include "Cache.h"

using namespace std ;

vector < string > * CfgParser::Split( string * input ) {
    vector < string > * vec = new vector < string >() ;
    string src( input->c_str() ) ;
    size_t start = 0 ;
    size_t end = 0 ;

    while ( true ) {
        start = src.find( "<device>" ) ;
        if ( start != std::string::npos ) {
            end = src.find( "</device>" ) + 9 ;
            string device = src.substr( start, ( end - start ) ) ;
            vec->push_back( device ) ;
            src = src.substr( end ) ;
        }  // if
        else
            break ;

    }  // while()
    return vec ;
}  // Split()

string CfgParser::SplitTag( string input, string tag ) {

    size_t start = input.find( "<" + tag + ">" ) ;
    if ( start == string::npos )
        Log::PrintError( "CfgParser.cpp:SplitTag-Configure File is incorrect. Can not find tag = " + tag ) ;
    start += tag.size() + 2 ;
    size_t end = input.find( "</" + tag + ">" ) ;

    return input.substr( start, end - start ) ;
}  // SplitTag()

uint64_t CfgParser::CacheParamter( string tag, string value ) {
    try {

        if ( tag.compare( "replacepolicy" ) == 0 ) {
            if ( value.compare( "LRU" ) == 0 )
                return REPLACEPOLICY::LRU ;
            else if ( value.compare( "ROUND_ROBIN" ) == 0 )
                return REPLACEPOLICY::ROUND_ROBIN ;
            else if ( value.compare( "RANDOM" ) == 0 )
                return REPLACEPOLICY::RANDOM ;
        }  // else if

        else if ( tag.compare( "writepolicy" ) == 0 ) {
            if ( value.compare( "WB" ) == 0 )
                return WRITEPOLICY::WRITE_BACK ;
            else if ( value.compare( "WT" ) == 0 )
                return WRITEPOLICY::WRITE_TROUGH ;
        }  // else if

        else if ( tag.compare( "type" ) == 0 ) {
            if ( value.compare( "Cache" ) == 0 )
                return Cache::CACHE ;
            else if ( value.compare( "hybridcache" ) == 0 )
                return Cache::HYBRIDCACHE ;
            else if ( value.compare( "buffercache" ) == 0 )
                return Cache::BUFFERCACHE ;
        }  // else if


        else
            return std::stoul( value ) ;  // level, size, blocksize, associativity

        return 0 ;
    } catch ( std::exception & e ) {
        Log::PrintError( "CfgParser.cpp:CacheParamter-Configure File is incorrect. Can not convert " + tag + " = "
                + value + "." ) ;
        return 0 ;
    }
}

CfgParser::CfgParser( const char * cfg ) {

    m_cfgfile = fopen( cfg, "r" ) ;
    if ( m_cfgfile == NULL )
        Log::PrintError( "CfgFile is NULL." ) ;
    m_DeviceList = NULL ;
    std::string * cfgstr = new std::string() ;

    while ( !feof( m_cfgfile ) ) {
        char temp ;
        fscanf( m_cfgfile, "%c", &temp ) ;
        cfgstr->append( 1, temp ) ;
    }  // while

    fclose( m_cfgfile ) ;

    m_DeviceList = Split( cfgstr ) ;

}  // CfgParser::CfgParser()

uint64_t CfgParser::ParseDevice( std::string DeviceName, std::string Tag ) {

    for ( uint8_t i = 0; i < m_DeviceList->size(); i++ ) {
        if ( ( *m_DeviceList )[ i ].find( DeviceName ) != string::npos ) {
            string device = ( *m_DeviceList )[ i ] ;
            string value = SplitTag( device, Tag ) ;
            return CacheParamter( Tag, value ) ;
        }  // if
    }  // for()

    Log::PrintError( "CCfgParser.cpp:ParseDevice-Configure File is incorrect. Can not find \"" + DeviceName + "/" + Tag
            + "\"" ) ;
    return 0 ;

}  // CfgParser::SplitComponent()
