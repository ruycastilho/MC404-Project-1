/*  Trabalho 1 - MC404 : IAS Assembler
    Nome: Ruy Castilho Barrichelo RA: 177012 */


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "ias_analysis.h"

// DEFINES
#define LINE_SIZE 150
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
            
            // Inserts commands in command list, using the first string.
		    list_insert(command_list, read_string, NULL, NULL, 0, 0, 0);

        }

        // Closes input file
        fclose(input_file);

        // Creates and opens output file, if necessary
        char *output_file_name = NULL;
        FILE *output_file = NULL;

        if ( argc == 3 ) {
            output_file_name = argv[2];

        }

        if ( output_file_name != NULL ) {
            output_file = fopen(output_file_name, "w");

        }

		// Creates label and SET list
        List label_list = createList();

        // Runs first analysis
        bool error_ocurred = first_analysis(command_list, label_list, NULL, output_file);

        // Finishes if an error ocurred
        if ( error_ocurred == false ) { 
        
            List memory_map = createList();

            // Runs second analysis
            error_ocurred = second_analysis(command_list, label_list, memory_map, output_file);

            // Finishes if an error ocurred
            if ( error_ocurred == false ) {
            
                // Prints memory map
                print_map(memory_map, output_file);

            }
 
            // Memory release 
            list_free(memory_map);
  
        }

        // Memory release
        if ( output_file != NULL ) {
            fclose(output_file);
        
        }

        list_free(label_list);
        list_free(command_list);
        free(read_string);

    }
    
    // Error handling
    else {
        fprintf(stdout, "%s", "ERROR\nCould not open the input file!\n");
    
    }

	return 0;

}
