/*
 * CfgParser.h
 *
 *  Created on: May 6, 2016
 *      Author: LuoJing-Yuan
 */

#ifndef CFGPARSER_H_
#define CFGPARSER_H_
#include <stdio.h>
#include <string>
#include <vector>
#include <stdint.h>
#include <string>

class CfgParser {
    private:
        FILE * m_cfgfile ;
        std::vector < std::string > * m_DeviceList ;

        std::vector < std::string > * Split( std::string * input ) ;
        std::string SplitTag( std::string input, std::string tag ) ;
        uint64_t CacheParamter( std::string tag, std::string value ) ;

    public:
        CfgParser( const char * cfg ) ;
        uint64_t ParseDevice( std::string DeviceType, std::string Tag ) ;

        ~CfgParser() ;
} ;

#endif /* CFGPARSER_H_ */
