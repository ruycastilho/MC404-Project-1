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
        
        // Markers used to split the lines in 3 sections.
        char *string_marks[2];


        List command_list = createList();

        // File is read while EOF isnt reached
        while ( fgets(read_string, LINE_SIZE ,input_file) != NULL) {

        	// Content analysis. Only reads 2 strings, since comments are ignored
		    int j = 0;
			for(int i=0; i < COMMAND_NUMBER - 1 && read_string[j] != '\0'; i++) {

			    // Skipping spaces and tabs
		    	while ( read_string[j] == ' ' || read_string[j] == '\t' ) {
		    		j = j + 1;
				}

				
				string_marks[i] = &(read_string[j]);
				j = j + 1;


			    // Reaching the next space between commands
		    	while ( read_string[j] != ' ' && read_string[j] != '\t' && read_string[j] != '\n' && read_string != '\0') {
		    		j = j + 1;
				}

                if ( strcmp(string_marks[i], "") == 0 ) {
                    string_marks[i] = NULL;

                }
			}

            printf("%s", string_marks[0]);
            // Inserts commands in command list
			list_insert(command_list, string_marks[0], string_marks[1], NULL, 0, 0, 0);
	

        }

        // Closes input file
        fclose(input_file);

        List label_list = createList();

        // Runs first analysis
        bool error_ocurred = first_analysis(command_list, label_list);

        // Finishes if an error ocurred
        if ( error_ocurred == true ) {
            return 0;
        
        }

        List memory_map = createList();

        // Runs second analysis
        error_ocurred = second_analysis(command_list, label_list, memory_map);

        // Finishes if an error ocurred
        if ( error_ocurred == true ) {
            return 0;

        }

        // Prints memory map
        char *output_file_name = argv[2];
        print_map(memory_map, output_file_name);
        
        // Memory release
        free(read_string);


        

    }
    
    // Error handling
    else {
        fprintf(stdout, "%s", "ERROR\nCould not open the input file!\n");
    
    }


	return 0;

}





// TIRAR STDIO.H DA LIST


/*

Fazer os passos como apresentado nos slides.
Só fazer o mapa de memoria se for certificado que a primeira passagem pelo assembly deu certo.


Por as linhas na lista, cada trecho em 1 string.
mandar pra funcao adequada





*/
