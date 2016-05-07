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

class CfgParser {
  private:
	FILE * m_cfgfile ;
    std::vector<std::string> * m_DeviceList ;


  public:
	CfgParser( const char * cfg ) ;
	int ParseDevice( std::string DeviceType, std::string Tag ) ;

	~CfgParser() ;
};




#endif /* CFGPARSER_H_ */
