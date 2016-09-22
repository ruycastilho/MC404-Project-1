/*  Trabalho 1 - MC404 : Montador de IAS
    Nome: Ruy Castilho Barrichelo RA: 177012 */


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "as_routines.h"

// DEFINES
#define LINE_SIZE 500


/* Function: Main function of the IAS assembler;
   Param: Input file name, output file name(optional);

   Return: None.
*/
int main(int argc, char **argv) {

    // File naming specifications.
    char *input_file_name = argv[1];
    //char *output_file_name = argv[1];
    
    // Input File
    FILE* input_file = fopen(input_file_name, "r");
    
    // File analysis
    if ( input_file != NULL ) {
        char *read_string = (char *)malloc(LINE_SIZE*sizeof(char*));
        
        
        // File is read while EOF isnt reached
        while ( fgets(read_string, LINE_SIZE ,input_file) != NULL) {
        	printf("%s", read_string);
        	
        	
        	// Content analysis
        	int i=0;
        	while( read_string[i] == " " || read_string[i] == "\t") {
        		i = i + 1;
      
        	}
        	
        }
        
        // Memory release
        free(read_string);
        fclose(input_file);

    }
    
    // Error handling
    else {
        printf("Error - Could not open the input file.\n");
    
    }

}










































/*
    LEMBRETE

Fazer os passos como apresentado nos slides.
Só fazer o mapa de memoria se for certificado que a primeira passagem pelo assembly deu certo.

*/
