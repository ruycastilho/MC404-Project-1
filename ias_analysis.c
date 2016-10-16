/*  Trabalho 1 - MC404 : Montador de IAS
    Nome: Ruy Castilho Barrichelo RA: 177012 */

// INCLUDES
#include "ias_analysis.h"

/* 
	Function: Performs first run through the commands, thus creates the label list;
    Param: Pointers to command list, label list and memory map (NULL in this case, 
           used in other functions) and pointer to output file, if the latter exists;

    Return: True if error ocurred during analysis. False, if not.
*/
bool first_analysis(List command_list, List label_list, List memory_map, FILE* output_file) {

	node *current_node = command_list->next;

    int j;

	// String containing commands
	char *command;

	// Boolean variables to control the string-checking loop
	bool error_ocurred = false;
    bool is_label = false;
    bool should_end = false;

	int line = 1; // Counts lines from input file
	int address = 0; // Controls current memory address
	int side = LEFT; // Controls the side of the memory word

	// Runs through the list
	while ( current_node != NULL && error_ocurred == false  ) {

        j = 0;
        // Obtains command from command list
        command = current_node->strings[0];

		// Skips initial space characters
        while ( isspace(command[j]) ) {
            j = j + 1;

        }

		// Updates strings, according to the deletion of space characters
        command = &(command[j]);
        current_node->strings[0] = command;
        j = 0;

       	should_end = false;
       	
		// Checks the characters in a string
		while ( command[j] != '\0' && should_end == false && command != NULL && error_ocurred == false ) {
            is_label = false;

			// If it is a directive
			if ( command[j] == '.' ) {
		        should_end = true;	// Rest of the line is not important, if there are no errors

                directive_analysis(&(command[j]), memory_map, label_list, &address, &side, &line, &error_ocurred, output_file);

			}

			// If it is a label
			else if ( command[j] == ':' ) {

         		label_analysis(command, label_list, address, side, &line, &error_ocurred, output_file);  

                // Deletes label from string, since it is not used anymore.
                j = j + 1;
                // Skips space characters
                while ( command[j] == ' ' || command[j] == '\t' ) {
                    j = j + 1;

                }

				// Updates the string, according to the deletion of the label
                command = &(command[j]);
                current_node->strings[0] = command;

				// Flag informs that string should keep being checked for other elements
                is_label = true;
                j = 0;	// Resets counter, since string was updated

			}
            
			else if ( command[j] == '#' ) {
		        should_end = true;	// Rest of the line is not important
		        
			
			}

			// If it is an instruction
			else if ( isspace(command[j]) && j != 0 && command[j-1] != ':' && !( isspace(command[j-1]) ) ) {
 		        should_end = true;	// Rest of the line is not important, if there are no errors

			    if ( side == LEFT ) {
				    side = RIGHT;

			    }
			    else {
				    address = address + 1;
				    side = LEFT;
				
			    }

			}

			// If there was no label, updates counter
		    if ( is_label == false ) {
                j = j + 1;
		    
		    }

            // If IAS' memory capacity is not enough
            if ( address > MAX_MEMORY ) {
                error_ocurred = true;
                error_treatment(&line, NULL, "memory address. Limit excedeed", &error_ocurred, output_file);

            }

        }

		// Updates line value and progresses in the list, if no errors ocurred
        if ( error_ocurred == false ) {
      		line = line + 1;
		    current_node = current_node->next;      
        
        }

	}

	return error_ocurred;

}


/* 
	Function: Stores a new label or set constant and its address;
    Param: String containing label/constant, label list, 'address' and 
            'side' integers, pointers to both 'line' integer, boolean error flag,
            and output file;

    Return: None.
*/
void label_analysis(char* label, List list, int address, int side, int* line, bool* error_ocurred, FILE* output_file) {

    int i=0;

	// Reaches ':' character
    while ( label[i] != ':' && label[i] != '\0' ) {
        i = i + 1;

    }
    
    // Checks if label(only) is valid ( no characters after ':' )
    if ( label[i] == ':' && ( isspace(label[i+1]) || label[i+1] == '\0' ) ) {
        label[i] = '\0';

    }

    // Checks if label/set is valid
    if ( sym_treatment(label) == true ) {

        // If it isn't already stored, inserts it in the list
        if ( list_search_string1(list, label) == NULL ) {
            list_insert(list, label, NULL, NULL, address, side, 0);

        }

    }
    
    // In case of error
    else {
		error_treatment(line, label, "sym argument", error_ocurred, output_file);
    
    }


}

/* 
	Function: Identifies instructions;
    Param: String containing instruction, pointer to both memory map and label lists, pointers to 'address',
            'side' and 'line' integers and to boolean error flag, pointer to output file;

    Return: None.
*/
void instruction_analysis(char* instruction, List memory_map, List label_list, int* address, int* side, int* line, bool* error_ocurred, FILE* output_file) {

    // Separates instruction from address argument
	int i = 0;
	do {
		i = i +1;

	} while ( !isspace(instruction[i]) );

	instruction[i] = '\0';

    // Instruction address data
    char *inst_address = NULL;
    int numerical_address = 0;

    // Address information to determine correct instructions(JMP, JMP+, STaddr)
    int inst_side = LEFT;

    int j = i;

    // Reaches non-space character
	do {
		j = j + 1;

	} while ( isspace(instruction[j]) );

	// Separates instruction address
    inst_address = &instruction[j+1];
 
    // If there is an argument, obtains the string cointaining the address
    if ( instruction[j] == '"' ) {

		// Reaches second '"' char, in the end of the argument
        j = 0;
	    while ( inst_address[j] != '\"' ) {
		    j = j + 1;

	    }
	    
	    // Checks if line elements are valid
	    if ( line_treatment(&(inst_address[j + 1]), line, error_ocurred, output_file) == false ) {
	        return;
	
	    }

        inst_address[j] = '\0';

        // Hexadecimal argument
	    if ( inst_address[0] == '0' && inst_address[1] == 'x' ) {

			// Checks if argument is valid
            numerical_address = hex_treatment(inst_address);
      
            // If error ocurred
            if ( numerical_address < 0 ) {
            	error_treatment(line, inst_address, "hexadecimal number", error_ocurred, output_file);
                return;

            }
            
	    }

	    // Decimal argument
	    else if ( isdigit(inst_address[0]) ) {

			// Checks if argument is valid
            numerical_address = dec_treatment(inst_address);
            
            // If error ocurred
            if ( numerical_address < 0 ) {
            	error_treatment(line, inst_address, "decimal number", error_ocurred, output_file);
                return;

            }
        }

	    // Label argument      
        else {

			// Searches label in the list
            node* label = list_search_string1(label_list, inst_address);

			// If label was not found
            if ( label == NULL ) {
            	error_treatment(line, inst_address, "label argument", error_ocurred, output_file);

            }

            // If label was found, address and side are retrieved
            else {
                numerical_address = label->integers[0];
                inst_side = label->integers[1];

            }

	    }
    }

    // If there is no argument, it is not needed. So, it is stored as 0.
    else {
        numerical_address = 0;

        // Checks if line elements are valid
	    if ( line_treatment(&(inst_address[j]), line, error_ocurred, output_file) == false ) {
            return;

        }
        inst_address = NULL;
    }


	// Obtains hexadecimal equivalent of the instruction
    char *hex_equivalent;

    if ( strcmp (instruction, "LD") == 0 ) {
        hex_equivalent = "01";

    }

    else if ( strcmp (instruction, "LD-") == 0 ) {
        hex_equivalent = "02";

    }

    else if ( strcmp (instruction, "LD|") == 0 ) {
        hex_equivalent = "03";

    }

    else if ( strcmp (instruction, "LDmq") == 0 ) {
        hex_equivalent = "0A";

		// If instruction should not have an argument
        if ( inst_address != NULL ) {
            error_treatment(line, instruction, "memory address. Instruction should not have an argument", error_ocurred, output_file);
            return;
        
        }

    }

    else if ( strcmp (instruction, "LDmq_mx") == 0 ) {
        hex_equivalent = "09";

    }

    else if ( strcmp (instruction, "ST") == 0 ) {
        hex_equivalent = "21";

    }

    else if ( strcmp (instruction, "JMP") == 0 ) {

        if (inst_side == LEFT) {
            hex_equivalent = "0D";
        
        }

        else {
            hex_equivalent = "0E";
        
        }

    }

    else if ( strcmp (instruction, "JUMP+") == 0 ) {

        if (inst_side == LEFT) {
            hex_equivalent = "0F";
        
        }

        else {
            hex_equivalent = "10";
        
        }

    }

    else if ( strcmp (instruction, "ADD") == 0 ) {
        hex_equivalent = "05";

    }

    else if ( strcmp (instruction, "ADD|") == 0 ) {
        hex_equivalent = "07";

    }

    else if ( strcmp (instruction, "SUB") == 0 ) {
        hex_equivalent = "06";

    }

    else if ( strcmp (instruction, "SUB|") == 0 ) {
        hex_equivalent = "08";

    }

    else if ( strcmp (instruction, "MUL") == 0 ) {
        hex_equivalent = "0B";

    }

    else if ( strcmp (instruction, "DIV") == 0 ) {
        hex_equivalent = "0C";

    }

    else if ( strcmp (instruction, "LSH") == 0 ) {
        hex_equivalent = "14";

		// If instruction should not have an argument
        if ( inst_address != NULL ) {
            error_treatment(line, instruction, "memory address. Instruction should not have an argument", error_ocurred, output_file);
            return;
        
        }

    }

    else if ( strcmp (instruction, "RSH") == 0 ) {
        hex_equivalent = "15";

		// If instruction should not have an argument
        if ( inst_address != NULL ) {
            error_treatment(line, instruction, "memory address. Instruction should not have an argument", error_ocurred,  output_file);
            return;
        
        }

    }

    else if ( strcmp (instruction, "STaddr") == 0 ) {
        if (inst_side == LEFT) {
            hex_equivalent = "12";
        
        }

        else {
            hex_equivalent = "13";
        
        }

    }

	// If error ocurred
    else {
        error_treatment(line, instruction, "mnemonic", error_ocurred, output_file);
        return;

    }

    /*  Adds instruction to memory map, if second analysis is running
		Uses first string to store OPCODE, first integer to store the memory map
		address, second one to store the memory word side and the third one to
		store the memory address related to the instrucion.
	*/
    if ( memory_map != NULL ) {
        list_insert_sorted(memory_map, hex_equivalent, NULL, NULL, *(address), *(side), numerical_address);

    }

    // Repairs separated string, if necessary
	instruction[i] = ' ';
    if ( inst_address != NULL ) {
        inst_address[j] = '"';

    }

}


/* 
	Function: Identifies and treats directives;
    Param: String containing directives, pointers to memory map(null if not used)
           and label lists, pointers to 'address' and 'side' integer and to 
           boolean error flag, pointer to output_file;

    Return: None.
*/
void directive_analysis(char* directive, List memory_map, List label_list, int *address, int *side, int *line, bool* error_ocurred, FILE* output_file) {

	// String containing arguments
	char *argument1 = NULL, *argument2 = NULL;

	// Flag to inform if there are 2 arguments
    bool double_arguments = false;

	// Separates the directive
	int i=0;
	do {
		i = i + 1;

	} while ( !isspace(directive[i]) );

	directive[i] = '\0';
    
	// Separates the first argument
	argument1 = &directive[i+1];

	// Separates the second argument
	int j=0;
	do {
		j = j + 1;

	} while ( !isspace(argument1[j]) );

	argument1[j] = '\0';   
    
    // Reaches second argument, skipping space characters, if necessary
    int k = j + 1;
	while ( isspace(argument1[k]) ) {
        k = k + 1;

    }

    // In case there's a second argument
    if ( argument1[k] != '\0' && argument1[k] != '#' ) {

		// Separates second argument
        argument2 = &argument1[k];
        
	    // Puts '\0' in the end of argument2
	    k = 0;
	    do {
		    k = k + 1;

	    } while ( !isspace(argument2[k]) && argument2[k] != '\0' );


        // Checks if line elements are valid
	    if ( line_treatment(&(argument2[k]), line, error_ocurred, output_file) == false ) {

            return;

        }

	    argument2[k] = '\0';
		// Flag informing there were stored 2 arguments
        double_arguments = true;

    }

    // In case of error, calls the line_treatment function, that explicits said error
    else {

	    if ( line_treatment(&(argument1[k]), line, error_ocurred, output_file) == false ) {
            return;

        }

    }

	int directive_argument = 0;

	/* ------------------------------Directives------------------------------ */

	// If the directive is .set
	if ( strcmp(directive, ".set") == 0 ) {

		// Checks if argument is valid
        if ( sym_treatment(argument1) == false ) {
        	error_treatment(line, argument1, "SYM argument", error_ocurred, output_file);
            return;
        
        }

        // Hexadecimal argument
		if ( argument2[0] == '0' && argument2[1] == 'x' ) {

			// Checks if argument is valid
            directive_argument = hex_treatment(argument2);

            // If error ocurred
            if ( directive_argument < 0 ) {
        		error_treatment(line, argument2, "hexadecimal number", error_ocurred, output_file);
                return;

            }
            
		}

		// Decimal argument
		else {

			// Checks if argument is valid
            directive_argument = dec_treatment(argument2);

            // If error ocurred
            if ( directive_argument < 0 ) {
	            error_treatment(line, argument2, "decimal argument", error_ocurred, output_file);
                return;

            }
		}

        // Sym treatment is called inside label_treatment.
        label_analysis(argument1, label_list, directive_argument, LEFT, line, error_ocurred, output_file);
        
	}
	
	/* ---------------------------------------------------------------------- */

	// If the directive is .org
	else if ( strcmp(directive, ".org") == 0 ) {

		// If there are 2 arguments when only 1 is required
        if ( double_arguments == true ) {
        	error_treatment(line, NULL, "number of arguments", error_ocurred, output_file);
            return;

        }

       // Hexadecimal argument
		if ( argument1[0] == '0' && argument1[1] == 'x' ) {

			// Checks if argument is valid
            directive_argument = hex_treatment(argument1);

            // If error ocurred
            if ( directive_argument < 0 ) {
        		error_treatment(line, argument1, "hexadecimal number", error_ocurred, output_file);
                return;

            }

		}

		// Decimal argument
		else {

			// Checks if argument is valid
            directive_argument = dec_treatment(argument1);


            // If error ocurred
            if ( directive_argument < 0 ) {
        		error_treatment(line, argument1, "decimal number", error_ocurred, output_file);   
                return;

            }

		}

        // If there are no errors, current address is changed
        *(address) = directive_argument;
        *(side) = LEFT;

    }

	/* ---------------------------------------------------------------------- */

	// If the directive is .wfill
	else if ( strcmp(directive, ".wfill") == 0 ) {

        bool is_negative = false;

		// If current side is RIGHT, word cannot be inserted
        if ( *side == RIGHT ) {
        	error_treatment(line, NULL, "word placement (right side)", error_ocurred, output_file);
            return;

        }

        // Initially, checks if the first argument (decimal) is valid
        int word_quantity = dec_treatment(argument1);


        // If error ocurred
        if ( word_quantity < 0 ) {
        	error_treatment(line, argument1, "quantity of words", error_ocurred, output_file);  
            return;

        }

        // Now, for the second argument
        else {

            // Hexadecimal argument
		    if ( argument2[0] == '0' && argument2[1] == 'x' ) {

				// Checks if argument is valid
                directive_argument = hex_treatment(argument2);

                // If error ocurred
                if ( directive_argument < 0 ) {
        			error_treatment(line, argument2, "hexadecimal number", error_ocurred, output_file);
                    return;

                }

		    }

            // Decimal argument
            else if ( isdigit(argument2[0]) || argument2[0] == '-' ) {

                // If number is negative
		        if ( argument2[0] == '-' ) {
                    is_negative = true;
                    argument2 = &(argument2[1]);

                }

				// Checks if argument is valid            
                directive_argument = dec_treatment(argument2);

                // If error ocurred
                if ( directive_argument < 0 ) {
        			error_treatment(line, argument2, "decimal number", error_ocurred, output_file);
                    return;

                }

            }
            
            // Label as argument
            else {

                if ( sym_treatment(argument2) == true ) {
                         
                    // Searches sym in labels/sets list
                    node *found_label = list_search_string1(label_list, argument2);

                    // Retrieves address, if search was 
                    if ( found_label != NULL ) {
                        directive_argument = found_label->integers[0];

                    }

                    else if ( memory_map != NULL ) {
        				error_treatment(line, argument2, "SYM argument", error_ocurred, output_file);
                        return;
                    
                    }

                }
                else {
        			error_treatment(line, argument2, "SYM argument", error_ocurred, output_file);
                    return;
                    
                }
            
            }
        
        }
        if ( memory_map != NULL ) {

            char formatted_word[11];

            if ( is_negative == true ) {
                directive_argument = - directive_argument;   

            }
            sprintf(formatted_word, "%.10X", directive_argument);                            

            for(int l=0; l < word_quantity ;l++) { 

				/*  Adds word to memory map, if second analysis is running
					Uses first string to store word, first integer to store the memory map
					address, second one to store the memory word side and the third one to
					store flag to inform that node contains a word.
				*/
                list_insert_sorted(memory_map, formatted_word, NULL, NULL, *(address), *(side), -1);        
                *(address) = *(address) + 1;     
            }
    
        }

        else {

            *(address) = *(address) + word_quantity;

        }
   
    }

	/* ---------------------------------------------------------------------- */	

	// If the directive is .word
	else if ( strcmp(directive, ".word") == 0 ) {

		// If there are 2 arguments when only 1 is required
        if ( double_arguments == true ) {
        	error_treatment(line, NULL, "number of arguments", error_ocurred, output_file);
            return;

        }

		// If current side is RIGHT, word cannot be inserted
        if ( *side == RIGHT ) {
        	error_treatment(line, NULL, "word placement (right side)", error_ocurred, output_file);
            return;

        }

        // Hexadecimal argument
        if ( argument1[0] == '0' && argument1[1] == 'x' ) {

			// Checks if argument is valid
            directive_argument = hex_treatment(argument1);

            // If error ocurred
            if ( directive_argument < 0 ) {
        		error_treatment(line, argument1, "hexadecimal number", error_ocurred, output_file);
                return;

            }

        }

        // Decimal argument
        else if ( isdigit(argument1[0]) ) {

			// Checks if argument is valid
            directive_argument = dec_treatment(argument1);

            // If error ocurred
            if ( directive_argument < 0 ) {
        		error_treatment(line, argument1, "decimal number", error_ocurred, output_file);
                return;

            }

        }
        
        // Label as argument
        else {
            if ( sym_treatment(argument1) == true ) {

                // Searches sym in labels/sets list
                node *found_label = list_search_string1(label_list, argument1);

                // Retrieves address, if search was succesful
                if ( found_label != NULL ) {
                    directive_argument = found_label->integers[0];

                }

				// If second analysis is running, it means label is non existent
                else if ( memory_map != NULL ) {
        			error_treatment(line, argument1, "SYM argument", error_ocurred, output_file);
                    return;
                
                }

            }
            
            // In case of error
            else if ( memory_map != NULL ) {
        		error_treatment(line, argument1, "SYM argument", error_ocurred, output_file);
                return;
                
            }
        
        }

        // If second analysis is running
        if ( memory_map != NULL ) {
            char formatted_word[11];
            
            // Stores a formatted word containing the hexadecimal memory word
            sprintf(formatted_word, "%.10X", directive_argument);

			/*  Adds word to memory map, if second analysis is running
				Uses first string to store word, first integer to store the memory map
				address, second one to store the memory word side and the third one to
				store flag to inform that node contains a word.
			*/
            list_insert_sorted(memory_map, formatted_word, NULL, NULL, *(address), *(side), -1);        
        
        }

        *(address) = *(address) + 1;

    }

	/* ---------------------------------------------------------------------- */

	// If the directive is .align
    else if ( strcmp(directive, ".align") == 0 ) {

		// If there are 2 arguments when only 1 is required
        if ( double_arguments == true ) {
        	error_treatment(line, NULL, "number of arguments", error_ocurred, output_file);
            return;

        }

		// Checks if argument is valid
        directive_argument = dec_treatment(argument1);

        // If error ocurred
        if ( directive_argument < 0 ) {
        	error_treatment(line, argument1, "decimal argument", error_ocurred, output_file);
            return;

        }

        // If memory is unreachable, propagates error.
        if ( directive_argument > MAX_MEMORY ) {
            *(address) = directive_argument;
            return;
        
        }
        
        else {

			// If .align is necessary
            if ( !( *(address) % directive_argument == 0 && *(side) == LEFT ) ) {

				// If current address is 000
                if ( *(address) == 0 && *(side) == RIGHT ) {
                    *(address) = *(address) + directive_argument; // Updates address
                    *(side) = LEFT; // Updates side of memory word
                    
                }

				// If current address is not zero
                else if ( *(address) != 0 ) {

					// Increments address until requirement is met
                    do {
                        *(address) = *(address) + 1;
                    
                    } while ( *(address) % directive_argument != 0 );

					// Updates side of memory word
                    *(side) = LEFT;

                }

            }      

        }

	}

	/* ---------------------------------------------------------------------- */

    // Repairs separated string
	directive[i] = ' ';
	argument1[j] = ' ';
}


/* 
	Function: Performs second run through the commands;
    Param: Command, label and memory map lists, pointer to output file;

    Return: True if error ocurred during analysis. False, if not.
*/
bool second_analysis(List command_list, List label_list, List memory_map, FILE* output_file) {

	node *current_node = command_list->next;

	// String containing commands
	char *command;

	// Boolean variables to control the string-checking loop
	bool error_ocurred = false;
    bool should_end = false;

	int line = 1; // Counts lines from input file
	int address = 0; // Controls current memory address
	int side = LEFT; // Controls the side of the memory word

	// Runs through the command list
	while ( current_node != NULL && error_ocurred == false && address <= MAX_MEMORY ) {

        int j = 0;
        
        // Obtains string from command_lists
        command = current_node->strings[0];

		// Skips space characters
        while ( isspace(command[j]) ) {
            j = j + 1;

        }
        
        should_end = false;

		// Checks the characters in the command string
		while ( command[j] != '\0' && should_end == false && command != NULL && error_ocurred == false ) {

			// If it is a directive
			if ( command[j] == '.' ) {
				directive_analysis(command, memory_map, label_list, &address, &side, &line, &error_ocurred, output_file);
				should_end = true;	// Rest of the line is not important, if there are no errors

			}

            // If it is a comment
			else if ( command[j] == '#' ) {
				should_end = true;	// Rest of the line is not important

			}

			// If it is an instruction
			else if ( isspace(command[j]) && j != 0 && command[j-1] != ':' && !isspace(command[j-1]) )  {
				should_end = true;	// Rest of the line is not important, if there are no errors

				instruction_analysis(command, memory_map, label_list, &address, &side, &line, &error_ocurred, output_file);

				// Updates the address and side of memory word
				if ( side == LEFT ) {
					side = RIGHT;

				}
				else {
					address = address + 1;
					side = LEFT;

				}

			}
			j = j + 1;

	    }

		line = line + 1;
		current_node = current_node->next;
		
	}
    
	return error_ocurred;

}


/* 
	Function: Prints memory map either in stdout or in a output file;
    Param: Pointer to memory map list, string containing name of the output
           file or NUll if map should be printed in stdout;
           
    Return: None.
*/
void print_map(List memory_map, FILE* output_file) {
    
    // Skips head-node
    node* current_node = memory_map->next;

    // Auxiliary string for words
    char *word = NULL;

    // Controls the line being printed
    int current_side = 0;

    // Memory map being printed in stdout
    if ( output_file == NULL ) {

        while ( current_node != NULL ) {
        
            current_side = current_node->integers[1];

            // If it is on the left side
            if (  current_side == LEFT && current_node->integers[2] >= 0 ) {
                fprintf(stdout, "%.03X %s %.03X", current_node->integers[0], current_node->strings[0], current_node->integers[2]);


                // If right side is empty
                if ( current_node->next == NULL || ( current_node->next != NULL && current_node->next->integers[1] == LEFT ) ) {
                    fprintf(stdout, "%s", " 00 000\n");

                }

            }
            
            // If it is on the right side
            else if ( current_node->integers[2] >= 0 ) {            
                fprintf(stdout, " %s %.03X\n", current_node->strings[0], current_node->integers[2]);

            }

			// If it is a word
            else {
                word = current_node->strings[0];
                fprintf(stdout, "%.3X %.2s %.3s %.2s %.3s\n", current_node->integers[0], word, 
                        &(word[2]), &(word[5]), &(word[7]));
            
            }

            current_node = current_node->next;
            
        }
    }

    // Memory map being printed in file
    else {

		// Runs through the map
        while ( current_node != NULL ) {
        
            current_side = current_node->integers[1];
 
            // If it is on the left side
            if (  current_side == LEFT && current_node->integers[2] >= 0 ) {

                fprintf(output_file, "%.3X %s %.3X", current_node->integers[0], current_node->strings[0], current_node->integers[2]);


                // If right side is empty
                if ( current_node->next == NULL || ( current_node->next != NULL && current_node->next->integers[1] == LEFT ) ) {
                    fprintf(output_file, "%s", " 00 000\n");

                }

            }
            
            // If it is on the right side
            else if ( current_node->integers[2] >= 0 ) { 
                fprintf(output_file, " %s %.3X\n", current_node->strings[0], current_node->integers[2]);
            
            }

			// If it is a word            
            else {    

                word = current_node->strings[0];
                fprintf(output_file, "%.3X %.2s %.3s %.2s %.3s\n", current_node->integers[0], word, 
                        &(word[2]), &(word[5]), &(word[7]));
                 
            }
            
            current_node = current_node->next;

        }

    }


}
