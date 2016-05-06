/*
 * Log.h
 *
 *  Created on: 2016/5/5
 *      Author: luo
 */

#ifndef LOG_H
#define LOG_H
#include <string>
#include <stdio.h>
#include <stdlib.h>

class Log {
  public :
    static void PrintError( std::string ErrorMsg ) {
      printf( "%s\n", ErrorMsg.c_str() ) ;
      exit(-1) ;
    }  // ExcptionPrint

    static void PrintWarning( std::string WarningMsg  ) {
	  printf( "%s\n", WarningMsg.c_str() ) ;
    } // printWarning()

};



#endif /* LOG_H_ */
