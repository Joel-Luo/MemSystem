/*
 * CfgParser.cpp
 *
 *  Created on: May 6, 2016
 *      Author: LuoJing-Yuan
 */
#include "CfgParser.h"
#include "Log.h"
#include "Cache.h"
#include <string>

using namespace std ;

vector<string> * Split( string * input ){
  vector<string> * vec = new vector<string>()  ;
  string src ( input->c_str() )  ;
  size_t start = 0;
  size_t end = 0 ;

  while ( true ) {
    start = src.find( "<device>" ) ;
    if ( start != std::string::npos ) {
      end = src.find( "</device>" ) + 9 ;
      string device = src.substr( start, ( end - start ) ) ;
      vec->push_back( device ) ;
      src = src.substr( end ) ;
    } // if
    else break ;

  } // while()
  return vec ;
}  // Split()

string SplitTag( string input, string tag ) {


  size_t start = input.find( "<" + tag + ">" ) ;
  if ( start == string::npos ) Log::PrintError( "CfgParser.cpp:SplitTag-Configure File is incorrect. Can not find tag = " + tag  ) ;
  start += tag.size() + 2 ;
  size_t end = input.find( "</" + tag + ">" ) ;

  return input.substr( start, end - start ) ;
} // SplitTag()

uint32_t CacheParamter( string tag, string value) {
  try {

    if ( tag.compare( "replacepolicy" )  == 0 ) {
      if ( value.compare( "LRU" )  == 0 ) return Cache::LRU ;
      else if ( value.compare( "ROUND_ROBIN" )  == 0 ) return Cache::ROUND_ROBIN ;
      else if ( value.compare( "RANDOM" )  == 0 ) return Cache::RANDOM ;
    }  // else if

    else if ( tag.compare( "writepolicy" )  == 0 ) {
       if ( value.compare( "WB" ) == 0 ) return Cache::WRITE_BACK ;
       else if ( value.compare( "WT" ) == 0 ) return Cache::WRITE_TROUGH ;
    }  // else if

    else return std::stoul( value ) ; // level, size, blocksize, associativity

    return -1 ;
  } catch( std::exception & e ) {
	Log::PrintError( "CfgParser.cpp:CacheParamter-Configure File is incorrect. Can not convert " + tag + " = " + value + "." ) ;
	return -1 ;
  }
}


CfgParser::CfgParser(const char * cfg ){

	m_cfgfile = fopen( cfg, "r") ;
	if ( m_cfgfile == NULL ) Log::PrintError( "CfgFile is NULL." ) ;
    m_DeviceList = NULL ;
	std::string * cfgstr = new std::string() ;

    while ( !feof( m_cfgfile) ) {
      char temp;
      fscanf( m_cfgfile, "%c", &temp ) ;
      cfgstr->append( 1, temp )  ;
    }  // while

    fclose( m_cfgfile ) ;

    m_DeviceList = Split( cfgstr ) ;

}  // CfgParser::CfgParser()

int CfgParser::ParseDevice( std::string DeviceName, std::string Tag ) {

	 for ( int i = 0 ; i < m_DeviceList->size() ; i++  ) {
       if ( (*m_DeviceList)[i].find( DeviceName ) != string::npos ) {
    	 string device =  (*m_DeviceList)[i] ;
         string value = SplitTag( device, Tag ) ;
         return CacheParamter( Tag, value ) ;
       }  // if
	 }  // for()

	 Log::PrintError( "CCfgParser.cpp:ParseDevice-Configure File is incorrect. Can not find \"" + DeviceName + "/" + Tag + "\"") ;
     return -1 ;

}  // CfgParser::SplitComponent()