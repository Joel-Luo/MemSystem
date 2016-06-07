#include <stdio.h>
#include "MemSystem.h"
#include "Log.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

FILE * Log::CacheResultInfoFile = NULL ;

void ExecuteMemOperation( FILE * input, MemSystem * memsystem ) {
    uint64_t accessTime = 0x0 ;
    uint64_t address = 0x0 ;
    char op = 'x' ;
    for ( int counter = 1; !feof( input ); counter++ ) {
        char * tempStr = new char[ 20 ] ;
        char * op_s = new char[ 2 ] ;
        fscanf( input, "%s", tempStr ) ;
        if ( feof( input ) || strcmp( tempStr, "#eof" ) == 0 )
            break ;
        accessTime = strtoll( tempStr, NULL, 16 ) ;
        fscanf( input, "%s", tempStr ) ;
        address = strtoll( tempStr, NULL, 16 ) ;
        fscanf( input, "%s", op_s ) ;
        // Log::PrintMessage( "Instruction id : " + std::to_string(counter) ) ;
        if ( strcmp( op_s, "R" ) == 0 )
            memsystem->CoreAccessMem( accessTime, address, MemSystem::READ, NULL, 8 ) ;
        else if ( strcmp( op_s, "W" ) == 0 )
            memsystem->CoreAccessMem( accessTime, address, MemSystem::WRITE, NULL, 8 ) ;
        else
            Log::PrintError( "Unknown memory operation" ) ;

        delete[] tempStr ;
        delete[] op_s ;
    }  // while()
}

int main( int argc, char const *argv[] ) {

    if ( argc > 1 ) {
        if ( strcmp( argv[ 1 ], "-h" ) == 0 ) {
            Log::PrintMessage( " -conf : \"config file path\"" ) ;
            Log::PrintMessage( " -input : \"input file path\"" ) ;

        }  // if
        else {
            MemSystem * memsystem = NULL ;
            FILE * input = NULL ;
            FILE * output = NULL ;
            char * configFile = NULL ;

            for ( int i = 1; i < argc; i = i + 2 ) {
                if ( strcmp( argv[ i ], "-conf" ) == 0 ) {
                    Log::PrintMessage( "Confige File = " + std::string( argv[ i + 1 ] ) ) ;
                    configFile =(char*) argv[ i + 1 ] ;
                }  // if

                else if ( strcmp( argv[ i ], "-input" ) == 0 ) {
                    input = fopen( argv[ i + 1 ], "r" ) ;
                    if ( input == NULL )
                        Log::PrintError( "Input file error: no this inputfile." ) ;
                    Log::PrintMessage( "Input File Path = " + std::string( argv[ i + 1 ] ) ) ;
                }  // else if
                else if ( strcmp( argv[ i ], "-output" ) == 0 ) {
                    output = fopen( argv[ i + 1 ], "w" ) ;
                    Log::PrintMessage( "Output File Path = " + std::string( argv[ i + 1 ] ) ) ;
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
            memsystem = new MemSystem( configFile ) ;
            ExecuteMemOperation( input, memsystem ) ;

            for ( int i = 0; i < memsystem->m_Monitor->m_CacheLevel; i++ )
                memsystem->m_Monitor->OutputCacheInfo( i ) ;
        }  // else
    }  // if

    else {

        printf( "Incorrect number of Argument\n" ) ;

    }  // else

    return 0 ;

}  // main()

