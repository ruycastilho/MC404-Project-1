/*  Trabalho 1 - MC404 : Montador de IAS
    Nome: Ruy Castilho Barrichelo RA: 177012 */


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "as_routines.h"

// DEFINES
#define LINE_SIZE 100
#define COMMAND_NUMBER 3

/* 
	Function: Main function of the IAS assembler;
    Param: Input file name, output file name(optional);

    Return: None.
*/
int main(int argc, char **argv) {

    // File naming specifications.
    char *input_file_name = argv[1];
    
    // Input File
    FILE* input_file = fopen(input_file_name, "r");
    
    // File analysis
    if ( input_file != NULL ) {
        char *read_string = (char *)malloc(LINE_SIZE*sizeof(char*));
    
        List command_list = createList();

        // File is read while EOF isnt reached
        while ( fgets(read_string, LINE_SIZE ,input_file) != NULL) {

            //printf("RS || %s\n", read_string);
        	// Content analysis. Only reads 2 strings, since comments are ignored                     

            //printf("\ninicio ||| %s ||| fim \n", read_string);
            
            // Inserts commands in command list
		    list_insert(command_list, read_string, NULL, NULL, 0, 0, 0);
	        


        }

        // Closes input file
        fclose(input_file);

        // Creates and opens output file, if necessary
        char *output_file_name = argv[2];
        FILE *output_file = NULL;

        if ( output_file_name != NULL ) {
            fopen("output_file_name", "w");

        }


        List label_list = createList();

        // Runs first analysis
        bool error_ocurred = first_analysis(command_list, label_list, NULL, output_file);

        // Finishes if an error ocurred
        if ( error_ocurred == true ) {
            return 0;
        
        }


        List memory_map = createList();

        // Runs second analysis
        error_ocurred = second_analysis(command_list, label_list, memory_map, output_file);

        // Finishes if an error ocurred
        if ( error_ocurred == true ) {
            return 0;

        }


        // Prints memory map
        print_map(memory_map, output_file);

        // Memory release
        if ( output_file != NULL ) {
            fclose(output_file);
        
        }

        list_free(label_list);
        list_free(command_list);
        list_free(memory_map);
        free(read_string);


    }
    
    // Error handling
    else {
        fprintf(stdout, "%s", "ERROR\nCould not open the input file!\n");
    
    }


	return 0;

}





/*

Fazer os passos como apresentado nos slides.
Só fazer o mapa de memoria se for certificado que a primeira passagem pelo assembly deu certo.


Por as linhas na lista, cada trecho em 1 string.
mandar pra funcao adequada





*/
