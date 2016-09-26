#include <stdio.h>
#include "Log.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "MemContoller.h"
#include <stdint.h>

using namespace CS ;   // name space CS ( Cache Simulator)

FILE * CS::Log::CacheResultInfoFile = NULL ;

unsigned long long  gTotal_inst = 500000000 ;
unsigned long long  gSkip_Inst = 1 ;


void ExecuteMemOperation( FILE * input, MemContoller * memsystem ) {
    uint64_t accessTime = 0x0 ;
    uint64_t address = 0x0 ;
    // char op = 'x' ;
    for ( unsigned long long counter = 1; !feof( input ); counter++ ) {
        if ( counter % 5000000 == 0 )
          Log::PrintMessage( "Int: " + std::to_string( counter) + "\tProgress:" + std::to_string( (double)counter/(double)gTotal_inst *100 ) + "%") ;

        if ( counter == gSkip_Inst )  memsystem->EnableRecord();
        if ( counter > gTotal_inst ) break ;


        char * tempStr = new char[ 20 ] ;
        char * op_s = new char[ 2 ] ;

        if ( fscanf( input, "%s", tempStr ) != 1 ) break  ;

        if ( feof( input ) || strcmp( tempStr, "#eof" ) == 0 ) break ;
        accessTime = strtoll( tempStr, NULL, 16 ) ;

        if ( fscanf( input, "%s", tempStr ) != 1 ) break  ;
        address = strtoll( tempStr, NULL, 16 ) ;

        if ( fscanf( input, "%s", op_s ) != 1 ) break ;
        // Log::PrintMessage( "Instruction id : " + std::to_string(counter) ) ;


        if ( strcmp( op_s, "R" ) == 0 )
            memsystem->CoreAccessMem( accessTime, address, CS::ACCESSTYPE::READ, NULL, 8 ) ;
        else if ( strcmp( op_s, "W" ) == 0 )
            memsystem->CoreAccessMem( accessTime, address, CS::ACCESSTYPE:: WRITE, NULL, 8 ) ;
        else
            Log::PrintError( "Unknown memory operation" ) ;

        delete [] tempStr ;
        delete [] op_s ;
    }  // for ;

}

int main( int argc, char const *argv[] ) {

    if ( argc > 1 ) {
        if ( strcmp( argv[ 1 ], "-h" ) == 0 ) {
            Log::PrintMessage( " --conf : \"config file path\"" ) ;
            Log::PrintMessage( " --input : \"input file path\"" ) ;
            Log::PrintMessage( " --inst_num: \"number\"" ) ;
        }  // if
        else {
            MemContoller * memsystem = NULL ;
            FILE * input = NULL ;
            FILE * output = NULL ;
            FILE * CLU = NULL ;  // cache line usage
            char * configFile = NULL ;

            for ( int i = 1; i < argc; i = i + 2 ) {
                if ( strcmp( argv[ i ], "--conf" ) == 0 ) {
                    Log::PrintMessage( "Confige File = " + std::string( argv[ i + 1 ] ) ) ;
                    configFile =(char*) argv[ i + 1 ] ;
                }  // if

                else if ( strcmp( argv[ i ], "--input" ) == 0 ) {
                    input = fopen( argv[ i + 1 ], "r" ) ;
                    if ( input == NULL )
                        Log::PrintError( "Input file error: no this inputfile." ) ;
                    Log::PrintMessage( "Input File Path = " + std::string( argv[ i + 1 ] ) ) ;
                }  // else if
                else if ( strcmp( argv[ i ], "--output" ) == 0 ) {
                    output = fopen( argv[ i + 1 ], "w" ) ;
                    Log::PrintMessage( "Output File Path = " + std::string( argv[ i + 1 ] ) ) ;
                }  // else if
                else if ( strcmp( argv[ i ], "--inst_num" ) == 0 ) {
                    sscanf( argv[ i + 1 ], "%llu", &gTotal_inst ) ;
                    Log::PrintMessage( "Total Inst: " + std::to_string( gTotal_inst ) ) ;
                } // else if

                else if ( strcmp( argv[ i ], "--skip_inst" ) == 0 ) {
                    sscanf( argv[ i + 1 ], "%llu", &gSkip_Inst ) ;
                    Log::PrintMessage( "Skip Inst: " + std::to_string( gSkip_Inst ) ) ;
                }   // else if

                else if ( strcmp( argv[ i ], "--clu" ) == 0 ) {
                    CLU = fopen( argv[ i + 1 ], "w" ) ;
                    Log::PrintMessage( "CacheLineUsage = " + std::string( argv[ i + 1 ] ) ) ;
                }  // else if

            }  // for

            if ( input == NULL )
                Log::PrintError( "Input file error:There is no inputfile." ) ;
            else if ( output == NULL ) {
                output = fopen( "./CacheResultInfo.out", "w" ) ;
                Log::PrintMessage( "Output File Path = ./CacheResultInfo.out" ) ;
            }  // else
            else if ( configFile == NULL ) {
                Log::PrintMessage( "Configure File error: There is no configure file." ) ;
            }  // else if

            Log::CacheResultInfoFile = output ;

            memsystem = new MemContoller( configFile ) ;
            ExecuteMemOperation( input, memsystem ) ;

            for ( int i = 0; i < memsystem->m_Monitor->m_CacheLevel; i++ )
                memsystem->m_Monitor->OutputCacheInfo( i ) ;
        }  // else
    }  // if

    else
        printf( "Incorrect number of Argument\n" ) ;

    printf( "Finish the simulation.\n" ) ;
    return 0 ;

}  // main()

