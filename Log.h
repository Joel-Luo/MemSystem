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

#define DEBUG ;

class Log {



    public:
        static FILE * CacheResultInfoFile ;

        static void PrintError( std::string ErrorMsg ) {
            printf( "%s\n", ErrorMsg.c_str() ) ;
            exit( -1 ) ;
        }  // ExcptionPrint

        static void PrintWarning( std::string WarningMsg ) {
            printf( "%s\n", WarningMsg.c_str() ) ;
        }  // printWarning()

#ifdef DEBUG
        static void PrintDebugLog( std::string Msg ) {
            printf( "%s", Msg.c_str() ) ;
        }  // printWarning()
#endif

        static void PrintMessage( std::string Msg ) {
            printf( "%s\n", Msg.c_str() ) ;
        }  // printWarning()

        static void PrintMessageToFile( FILE * outfile, std::string Msg ) {
            fprintf( outfile, "%s\n", Msg.c_str() ) ;
        }  // printWarning()

} ;

#endif /* LOG_H_ */
