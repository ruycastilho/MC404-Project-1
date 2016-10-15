/*  Trabalho 1 - MC404 : Montador de IAS
    Nome: Ruy Castilho Barrichelo RA: 177012 */


// INCLUDES
#include "as_routines.h"

/* 
	Function: Performs first run through the code, thus creates the label list;
    Param: Pointers to command list, label list and memory map (NULL in this case, 
           used in other functions) and pointer to output file, if the latter exists;

    Return: True if error ocurred during analysis. False, if not.
*/
bool first_analysis(List command_list, List label_list, List memory_map, FILE* output_file) {

    //printf("entrou primeira analise\n");

	node *current_node = command_list->next;

    int j;

	char *code;
	
	bool error_ocurred = false;
    bool is_label;
    bool should_end = false;

	int line = 1;
	int address = 0;
	int side = LEFT;

	// Runs through the list
	while ( current_node != NULL && error_ocurred == false && address <= MAX_MEMORY ) {

        j = 0;
        code = current_node->strings[0];

        while ( isspace(code[j]) ) {
            j = j + 1;

        }
        
        code = &(code[j]);
        current_node->strings[0] = code;
        j = 0;

       	should_end = false;
		// Checks the characters in a string
		while ( code[j] != '\0' && should_end == false && code != NULL && error_ocurred == false ) {
            is_label = false;
		// printf("comando: %s\n", code);


			// If it is a directive
			if ( code[j] == '.' ) {
			//printf("entered directive if\n");
			 //  printf("comando: %s\n", &(code[j]));
		        should_end = true;
                directive_treatment(&(code[j]), memory_map, label_list, &address, &side, &line, &error_ocurred, output_file);

			}

			// If it is a label
			else if ( code[j] == ':' ) {
//printf("entered label if\n");
  // printf("comando: %s\n", &(code[j]));

         		label_treatment(code, label_list, address, side, &line, &error_ocurred, output_file);  

                // Deletes label from string, since it is not used anymore
                j = j + 1;  
                while ( code[j] == ' ' || code[j] == '\t' ) {
                    j = j + 1;

                }
                
                code = &(code[j]);
                current_node->strings[0] = code;

                is_label = true;
                j = 0;
			  //printf("new comando:%s\n", code);	
			}
            
			else if ( code[j] == '#' ) {
		        should_end = true;
		        
			
			}

			// If it is a command
			else if ( isspace(code[j]) && j != 0 && code[j-1] != ':' && !( isspace(code[j-1]) ) ) {
//printf("entered instruction if, char:%c:j:%d\n", code[j], j);
   printf("instrucao comando: %s\n", code);
 		        should_end = true;
				instruction_treatment(code, NULL, label_list, &address, &side, &line, &error_ocurred, output_file);

			    if ( side == LEFT ) {
				    side = RIGHT;

			    }
			    else {
				    address = address + 1;
				    side = LEFT;
				
			    }

			}
			
		    if ( is_label == false ) {
                j = j + 1;
		    
		    }
                

        }


        if ( error_ocurred == false ) {
      		line = line + 1;
		    current_node = current_node->next;      
        
        }

	}

	//     POR OS FREES   free(read_string);


    // If IAS' memory capacity is not enough
    if ( address > MAX_MEMORY ) {
        error_ocurred = true;
        
        if ( output_file == NULL ) {
        	fprintf(stdout, "%s %d %s", "ERROR on line", line,  "\nThere is not enough memory!\n");         
        
        }
        else {
            fprintf(output_file, "%s %d %s", "ERROR on line", line,  "\nThere is not enough memory!\n");         


        }

    }

	return error_ocurred;

}

/* 
	Function: Treats line, checking if elements are in valid order;
    Param: String pointing to the end of a instruction/directive, pointers to 
           integer input text line, boolean error flag, and output_file;

    Return: None.
*/
bool line_treatment(char* end_line, int* line, bool* error_ocurred, FILE* output_file) {

    int i = 0;

    // Reaches non-space char, if necessary
    while (  end_line != '\0' && isspace(end_line[i]) ) {
        i = i + 1;

    }


    if ( end_line[i] != '\0' && end_line[i] != '#' ) {
        *(error_ocurred) = true;

        if ( output_file == NULL ) {
        	fprintf(stdout, "%s %d %s", "ERROR on line", *(line),  "\nInvalid line input!\n");         
        
        }
        else {
            fprintf(output_file, "%s %d %s", "ERROR on line", *(line),  "\nInvalid line input!\n");         


        }
        return false;

    }

    return true;

}



/* 
	Function: Stores a new label and its address;
    Param: String containing label, label list, 'address' and 
            'side' integers, pointers to both 'line' integer, boolean error flag,
            and output file;

    Return: None.
*/
void label_treatment(char* label, List list, int address, int side, int* line, bool* error_ocurred, FILE* output_file) {


    // Removes ':' char, if necessary
    int i=0;
    bool should_undo = false;

    while (label[i] != ':' && label[i] != '\0') {
        i = i + 1;

    }
    
    // Checks if label is valid
    if ( label[i] == ':' && ( label[i+1] == ' ' || label[i+1] == '\t' || label[i+1] == '\0' || label[i+1] == '\n' ) ) {
        label[i] = '\0';
        should_undo = true;

    }

    else {
        while ( label[i] != ' ' && label[i] != '\t' && label[i] != '\0' && label[i] != '\n' ) {
            i = i + 1;

        }
        label[i] = '\0';

    }

    // Checks if label is valid
    if ( sym_treatment(label) == true ) {

        // If it isn't already stored, inserts it in the list
        if ( list_search_string1(list, label) == NULL ) {
            list_insert(list, label, NULL, NULL, address, side, 0);

        }

    }
    
    // In case of error
    else {

        if ( output_file == NULL ) {
        	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", label, "is not a valid sym argument!\n");         
        
        }
        else {
            fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", label,  "is not a valid sym argument!\n");         


        }
 
        *(error_ocurred) = true;
    
    }
    
    if ( should_undo == true ) {
        label[i] = ':';
    
    }


}

/* 
	Function: Identifies instructions;
    Param: String containing instruction, pointer to both memory map and label lists, pointers to 'address',
            'side' and 'line' integers and to boolean error flag, pointer to output file;

    Return: None.
*/
void instruction_treatment(char* instruction, List memory_map, List label_list, int* address, int* side, int* line, bool* error_ocurred, FILE* output_file) {

    //printf("entrou tratamento de inst: %s\n", instruction);

    // Separates instruction from address
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
    // Reaches non-space char
	do {
		j = j + 1;

	} while ( isspace(instruction[j]) );

 //  printf("instruc:%s\n", instruction);
    
    inst_address = &instruction[j+1];
 
    // If there is an argument, obtains the string cointaining the address
    if ( instruction[j] == '"' ) {
    

        j = 0;
	    while ( inst_address[j] != '\"' ) {
		    j = j + 1;

	    }
   // printf("instruc add:%s\n", &(inst_address[i]));
	    // Checks if line elements are valid
	    if ( line_treatment(&(inst_address[j + 1]), line, error_ocurred, output_file) == false ) {
	        return;
	
	    }

        inst_address[j] = '\0';


        // Hexadecimal argument
	    if ( inst_address[0] == '0' && inst_address[1] == 'x' ) {
            numerical_address = hex_treatment(inst_address);
      
            // If error ocurred
            if ( numerical_address < 0 ) {
            
                if ( output_file == NULL ) {
                	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", inst_address, "is not a valid hex number!\n");         
                
                }
                else {
                    fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", inst_address,  "is not a valid hex number!\n");          


                }
                *(error_ocurred) = true;
                return;

            }
            
	    }

	    // Decimal argument
	    else if ( (int)inst_address[0] < 58 && (int)inst_address[0] > 47 ) {
            numerical_address = dec_treatment(inst_address);
            
            // If error ocurred
            if ( numerical_address < 0 ) {
                if ( output_file == NULL ) {
                	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", inst_address, "is not a valid decimal number!\n");         
                
                }
                else {
                    fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", inst_address,  "is not a valid decimal number!\n");          


                }
                *(error_ocurred) = true;
                return;

            }
        }

	    // Label argument      
        else {
        
            node* label = list_search_string1(label_list, inst_address);

            if ( label == NULL ) {
                if ( output_file == NULL ) {
                	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", inst_address, "is not a valid label!\n");         
                
                }
                else {
                    fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", inst_address,  "is not a valid label!\n");          


                }
                
                *(error_ocurred) = true;
            
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

        if ( inst_address != NULL ) {
        
            if ( output_file == NULL ) {
            	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", instruction, "should not have a memory address!\n");         
            
            }
            else {
                fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", instruction,  "should not have a memory address!\n");          


            }
            
            *(error_ocurred) = true;
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


        if ( inst_address != NULL ) {
        
            if ( output_file == NULL ) {
            	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", instruction, "should not have a memory address!\n");         
            
            }
            else {
                fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", instruction,  "should not have a memory address!\n");          


            }
            
            *(error_ocurred) = true;
            return;
        
        }

    }

    else if ( strcmp (instruction, "RSH") == 0 ) {
        hex_equivalent = "15";

        if ( inst_address != NULL ) {
        
            if ( output_file == NULL ) {
            	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", instruction, "should not have a memory address!\n");         
            
            }
            else {
                fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", instruction,  "should not have a memory address!\n");          


            }
            
            *(error_ocurred) = true;
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

    else {

        if ( output_file == NULL ) {
        	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", instruction, "is not a valid mnemonic!\n");         
        
        }
        else {
            fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", instruction,  "is not a valid mnemonic!\n");          


        }
        
        *(error_ocurred) = true;
        return;
    }

    //printf("HEX %s MAP ADDRESS %d SIDE %d HEX ADR %d\n\n", hex_equivalent, *(address), *(side), numerical_address);

    // Adds instruction to memory map, if second analysis is running
    if ( memory_map != NULL ) {
        list_insert_sorted(memory_map, hex_equivalent, NULL, NULL, *(address), *(side), numerical_address);

    }   


    // Repairs separated string
	instruction[i] = ' ';
    if ( inst_address != NULL ) {
        inst_address[j] = '"';

    }


}

/* 
	Function: Treats symbol arguments;
    Param: String containing sym argument;
f
    Return: Flase if sym is invalid. True, if valid.
*/
bool sym_treatment(char* macro) {

    if ( macro != NULL ) {


        // If first char is letter or '_'
        if ( (int)macro[0] == 95 || ( (int)macro[0] > 64 && (int)macro[0] < 91 ) ||
            ( (int)macro[0] > 96 && (int)macro[0] < 123 ) ) {


            for(int i=0; macro[i] != '\0' ;i++) {

                // If not alphanumerical nor '_'
                if ( (int)macro[i] != 95 && !( (int)macro[i] > 64 && (int)macro[i] < 91 ) &&
                    !( (int)macro[i] > 96 && (int)macro[i] < 123 ) && !( (int)macro[i] > 47 && (int)macro[i] < 58 ) ) {
                    return false;
                }

            }
        
            return true;

        }

        

    }

    return false;
    

}

/* 
	Function: Treats hexadecimal arguments;
    Param: ;

    Return: Negative integer if number is invalid. Converted number, if is valid.
*/
int hex_treatment(char* macro) {

	int address = 0;
	char *end;
    bool is_zero = true;

    // Checks if size is correct
	if ( macro[12] == '\0' ) {


        // Check if it is zero
		for(int l=2; l < 12 && is_zero;l++) {

			if ( macro[l] != '0' ) {
				is_zero = false;

			} 

		}

        // If it is not zero
		if ( is_zero == false ) {
			address = strtol(macro, &end, 16);
			
			// If error ocurred
			if ( address == 0 ) {
			    return -1;
			
			}
			
			// Valid result
			return address;


		}
		
		// If it is zero
		else {
		    return 0;
		
		}	

	}

    // If the size is incorrect
	return -1;
}

/* 
	Function: Treats decimal arguments;
    Param: ;

    Return: Negative integer if number is invalid. Positive, if is valid.
*/
int dec_treatment(char* macro) {

	int address = 0;
	char *end;
    bool is_zero = true;

	for(int l=0; l < 4 && is_zero;l++) {

		if ( macro[l] != '0' ) {
			is_zero = false;

		} 

	}

    // If it is not zero
	if ( is_zero == false ) {
		address = strtol(macro, &end, 10);

		// If error ocurred
		if ( address == 0 ) {
		    return -1;
		
		}
		
		// Valid result
		return address;
		
		
		            /*if ( errno == ERANGE ) {
			            printf("ERROR on line %d %s is not a valid hex number!\n", *line, macro2);

		            }*/

	}
	
	// If it is zero
	else {
	    return 0;
	
	}	


}


/* 
	Function: Identifies and treats directives;
    Param: String containing directives, pointers to memory map(null if not used)
           and label lists, pointers to 'address' and 'side' integer and to 
           boolean error flag, pointer to output_file;

    Return: None.
*/
void directive_treatment(char* directive, List memory_map, List label_list, int *address, int *side, int *line, bool* error_ocurred, FILE* output_file) {

	char *macro1 = NULL, *macro2 = NULL;

	// Separates the directive
	int i=0;
	do {
		i = i + 1;

	} while ( !isspace(directive[i]) );

	directive[i] = '\0';

    //printf("direc: %s\n", directive);
    
	// Separates the first argument
	macro1 = &directive[i+1];

	// Separates the second argument
	int j=0;
	do {
		j = j + 1;

	} while ( !isspace(macro1[j]) );

	macro1[j] = '\0';

    //printf("macro 1:%s:\n", macro1);
    
    
    // Reaches second argument, if necessary
    int k = j + 1;
	while ( isspace(macro1[k]) ) {
        k = k + 1;

    }


    // In case there's a second argument
    if ( macro1[k] != '\0' && macro1[k] != '#' ) {

        macro2 = &macro1[k];
        //printf("macro 2:%s:\n", macro2);
        
	    // Puts '\0' in the end of macro2
	    k = 0;
	    do {
		    k = k + 1;

	    } while ( !isspace(macro2[k]) && macro2[k] != '\0' );


        // Checks if line elements are valid
	    if ( line_treatment(&(macro2[k]), line, error_ocurred, output_file) == false ) {

            return;

        }

	    macro2[k] = '\0';

    }

    // In case of error, calls the line_treatment function, that explicits said error
    else {

	    if ( line_treatment(&(macro1[k]), line, error_ocurred, output_file) == false ) {
            return;

        }

    }

	int directive_argument = 0;

	/* ----------------------------------------------*/

	// If the directive is .set
	if ( strcmp(directive, ".set") == 0 ) {

        if ( sym_treatment(macro1) == false ) {

            if ( output_file == NULL ) {
            	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1, "is not a valid sym argument!\n");         
            
            }
            else {
                fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1,  "is not a valid sym argument!\n");        


            }
	        
            *(error_ocurred) = true;
            return;
        
        }

        // Hexadecimal argument
		if ( macro2[0] == '0' && macro2[1] == 'x' ) {
            directive_argument = hex_treatment(macro2);

            // If error ocurred
            if ( directive_argument < 0 ) {
		        
                if ( output_file == NULL ) {
                	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro2, "is not a valid hex number!\n");         
                
                }
                else {
                    fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro2,  "is not a valid hex number!\n");        


                }
		        
                *(error_ocurred) = true;
                return;

            }
            
		}

		// Decimal argument
		else {
            directive_argument = dec_treatment(macro2);

            // If error ocurred
            if ( directive_argument < 0 ) {
	            
                if ( output_file == NULL ) {
                	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro2, "is not a valid decimal number!\n");         
                
                }
                else {
                    fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro2,  "is not a valid decimal number!\n");         


                }
		        
                *(error_ocurred) = true;
                return;

            }
		}

        // Sym treatment is called inside label_treatment.
        label_treatment(macro1, label_list, directive_argument, LEFT, line, error_ocurred, output_file);
        
	}
	
	
	/* ----------------------------------------------*/


	// If the directive is .org
	else if ( strcmp(directive, ".org") == 0 ) {

       // Hexadecimal argument
		if ( macro1[0] == '0' && macro1[1] == 'x' ) {
            directive_argument = hex_treatment(macro1);

            // If error ocurred
            if ( directive_argument < 0 ) {
	            
                if ( output_file == NULL ) {
                	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1, "is not a valid hex number!\n");         
                
                }
                else {
                    fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1,  "is not a valid hex number!\n");


                }
                
                *(error_ocurred) = true;
                return;

            }

		}

		// Decimal argument
		else {
            directive_argument = dec_treatment(macro1);


            // If error ocurred
            if ( directive_argument < 0 ) {
	            
                if ( output_file == NULL ) {
                	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1, "is not a valid decimal number!\n");         
                
                }
                else {
                    fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1,  "is not a valid decimal number!\n");          


                }
                
                *(error_ocurred) = true;
                return;

            }

		}

        // If there are no errors, current address is changed
        *(address) = directive_argument;


    }

	/* ----------------------------------------------*/
	

	// wfill
	else if ( strcmp(directive, ".wfill") == 0 ) {

        bool is_negative = false;

        if ( *side == RIGHT ) {

            if ( output_file == NULL ) {
            	fprintf(stdout, "%s %d%s", "ERROR on line", *line, "\nword cannot be placed on the right side!\n");         
            
            }
            else {
            	fprintf(output_file, "%s %d%s", "ERROR on line", *line, "\nword cannot be placed on the right side!\n");               


            }

            *(error_ocurred) = true;
            return;

        }

        // Initially, checks the first argument (decimal) for errors
        int word_quantity = dec_treatment(macro1);


        // If error ocurred
        if ( word_quantity < 0 ) {

            
            if ( output_file == NULL ) {
            	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1, "is not a valid amount of words!\n");         
            
            }
            else {
                fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1,  "is not a valid amount of words!\n");        


            }

            *(error_ocurred) = true;
            return;

        }

        // Now, for the second argument
        else {

            // Hexadecimal argument
		    if ( macro2[0] == '0' && macro2[1] == 'x' ) {
                directive_argument = hex_treatment(macro2);

                // If error ocurred
                if ( directive_argument < 0 ) {
                    if ( output_file == NULL ) {
                    	fprintf(stdout, "%s %d%s%s %s", "ERROR on line ", *line, "\n", macro2, "is not a valid hex number!\n");         
                    
                    }
                    else {
                        fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro2,  "is not a valid hex number!\n");          


                    }
                    *(error_ocurred) = true;
                    return;

                }

		    }

            // Decimal argument
            else if ( (int)macro2[0] < 58 && (int)macro2[0] > 47 ) {

                // If number is negative
		        if ( macro2[0] == '-' ) {
                    is_negative = true;
                    macro2 = &(macro2[1]);

                }
                
                directive_argument = dec_treatment(macro2);


                // If error ocurred
                if ( directive_argument < 0 ) {
                    if ( output_file == NULL ) {
                    	fprintf(stdout, "%s %d%s%s %s", "ERROR on line ", *line, "\n", macro2, "is not a valid decimal number!\n");         
                    
                    }
                    else {
                        fprintf(output_file, "%s %d%s%s %s", "ERROR on line ", *line, "\n", macro2,  "is not a valid decimal number!\n");          


                    }
	                
                    *(error_ocurred) = true;
                    return;

                }

            }
            
            // Label as argument
            else {

                if ( sym_treatment(macro2) == true ) {
                         
                    // Searches sym in labels/sets list
                    node *found_label = list_search_string1(label_list, macro2);

                    // Retrieves address, if search was 
                    if ( found_label != NULL ) {
                        directive_argument = found_label->integers[0];

                    }

                    else if ( memory_map != NULL ) {
                        
                        if ( output_file == NULL ) {
                        	fprintf(stdout, "%s %d%s%s %s", "ERROR on line ", *line, "\n", macro2, "is not a known label!\n");         
                        
                        }
                        else {
                            fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro2,  "is not a known label!\n");         


                        }    
                        *(error_ocurred) = true;
                        return;
                    
                    }

                }
                else {
                
                    if ( output_file == NULL ) {
                    	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro2, "is not a valid sym argument!\n");         
                    
                    }
                    else {
                        fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro2,  "is not a valid sym argument!\n");         


                    }    
                    *(error_ocurred) = true;
                    return;
                    
                }
            
            }


        
        }
        if ( memory_map != NULL ) {

            char formatted_word[11];
            sprintf(formatted_word, "%.10X", directive_argument);                            

            if ( is_negative == true ) {
                formatted_word[0] = 'F';
                formatted_word[1] = 'F';

            }

	        //printf("HEX %s MAP ADDRESS %d SIDE %d HEX ADR %d\n\n", formatted_word, *(address), *(side), -1);
            for(int l=0; l < word_quantity ;l++) { 

                list_insert_sorted(memory_map, formatted_word, NULL, NULL, *(address), *(side), -1);        
                *(address) = *(address) + 1;     
            }
    
        }

        else {
          //  printf("w %d\n", word_quantity);
            *(address) = word_quantity;

        }
   
    }


	/* ----------------------------------------------*/
	

	// word
	else if ( strcmp(directive, ".word") == 0 ) {

        if ( *side == RIGHT ) {

            if ( output_file == NULL ) {
            	fprintf(stdout, "%s %d%s", "ERROR on line", *line, "\nword cannot be placed on the right side!\n");         
            
            }
            else {
            	fprintf(output_file, "%s %d%s", "ERROR on line", *line, "\nword cannot be placed on the right side!\n");               


            }

            *(error_ocurred) = true;
            return;

        }

        // Hexadecimal argument
        if ( macro1[0] == '0' && macro1[1] == 'x' ) {
            directive_argument = hex_treatment(macro2);

            // If error ocurred
            if ( directive_argument < 0 ) {

                if ( output_file == NULL ) {
                	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1, "is not a valid hex number!\n");         
                
                }
                else {
                    fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1,  "is not a valid hex number!\n");          


                }
                
                *(error_ocurred) = true;
                return;

            }

        }

        // Decimal argument
        else if ( (int)macro1[0] < 58 && (int)macro1[0] > 47 ) {
        
            directive_argument = dec_treatment(macro1);


            // If error ocurred
            if ( directive_argument < 0 ) {
                if ( output_file == NULL ) {
                	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1, "is not a valid hex number!\n");         
                
                }
                else {
                    fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1,  "is not a valid hex number!\n");          


                }
                
                *(error_ocurred) = true;
                return;

            }

        }
        
        // Label as argument
        else {
            if ( sym_treatment(macro1) == true ) {

                // If there is a ':' char next to the label, it is removed
                if ( macro1[j-i-1] == ':' ) {
                    macro1[j-i-1] = '\0';

                }

                // Searches sym in labels/sets list
                node *found_label = list_search_string1(label_list, macro1);

                // Retrieves address, if search was 
                if ( found_label != NULL ) {
                    directive_argument = found_label->integers[0];

                }

                else if ( memory_map != NULL ) {
                    
                    if ( output_file == NULL ) {
                    	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1, "is not a known label!\n");         
                    
                    }
                    else {
                        fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1,  "is not a known label!\n");         


                    }    
                    *(error_ocurred) = true;
                    return;
                
                }

            }
            
            // In case of error
            else if ( memory_map != NULL ) {
            
                if ( output_file == NULL ) {
                	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1, "is not a valid sym argument!\n");         
                
                }
                else {
                    fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1,  "is not a valid sym argument!\n");         


                }   
                *(error_ocurred) = true;
                return;
                
            }
        
        }

        // If second analysis is running
        if ( memory_map != NULL ) {
            char formatted_word[11];
            sprintf(formatted_word, "%.10X", directive_argument);
            

	       // printf("HEX %s MAP ADDRESS %d SIDE %d HEX ADR %d\n\n", formatted_word, *(address), *(side), -1);
            list_insert_sorted(memory_map, formatted_word, NULL, NULL, *(address), *(side), -1);        
        
        }


        *(address) = *(address) + 1;

    }


	/* ----------------------------------------------*/
	

	// align
    else if ( strcmp(directive, ".align") == 0 ) {

           directive_argument = dec_treatment(macro1);

            // If error ocurred
            if ( directive_argument < 0 ) {
                if ( output_file == NULL ) {
                	fprintf(stdout, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1, "is not a valid decimal number!\n");         
                
                }
                else {
                    fprintf(output_file, "%s %d%s%s %s", "ERROR on line", *line, "\n", macro1,  "is not a valid decimal number!\n");          


                }   
	            
                *(error_ocurred) = true;
                return;

            }


        // If there are no errors
        *(side) = LEFT;

        // If memory is unreachable, propagates error.
        if ( directive_argument > MAX_MEMORY ) {
            *(address) = directive_argument;

            return;
        
        }
        
        else {
            while ( *(address) % directive_argument != 0 ) {
                *(address) = *(address) + 1;
            
            }
        }

	}
	

	/* ----------------------------------------------*/

        // Repairs separated string
		directive[i] = ' ';
		macro1[j] = ' ';
}


/* 
	Function: Performs second run through the code, thus creates the label list;
    Param: Command, label and memory map lists, pointer to output file;

    Return: True if error ocurred during analysis. False, if not.
*/
bool second_analysis(List command_list, List label_list, List memory_map, FILE* output_file) {

    //printf("entrou segunda analise\n");

	node *current_node = command_list->next;

	/*while ( current_node->next != NULL ) {

        printf("%s\n", current_node->strings[0]);

        current_node = current_node->next;
    }
    */

	char *code;
	
	bool error_ocurred = false;
    bool should_end = false;
    bool is_label = false;

	int line = 1;
	int address = 0;
	int side = LEFT;

	// Runs through the list
	while ( current_node != NULL && error_ocurred == false && address <= MAX_MEMORY ) {

        int j = 0;
        code = current_node->strings[0];
        
        while ( code[j] == ' ' || code[j] == '\t' ) {
            j = j + 1;

        }
        
        should_end = false;
		// Checks the characters in a string
		while ( code[j] != '\0' && code[j] != '\n' && should_end == false  && code != NULL && error_ocurred == false ) {
            is_label = false;
   //printf("comando:%s\n", code);
			// If it is a directive
			if ( code[j] == '.' ) {
				directive_treatment(code, memory_map, label_list, &address, &side, &line, &error_ocurred, output_file);
				should_end = true;
				
			}

            // If it is a comment
			else if ( code[j] == '#' ) {
				should_end = true;
			
			}

			// If it is a command
			else if ( isspace(code[j]) && j != 0 && code[j-1] != ':' && !isspace(code[j-1]) )  {
				should_end = true;

//printf("entered instruction if, char:%d:j:%d\n", code[j], j);
				instruction_treatment(code, memory_map, label_list, &address, &side, &line, &error_ocurred, output_file);

				if ( side == LEFT ) {
					side = RIGHT;

				}
				else {
					address = address + 1;
					side = LEFT;

				}
			
			
			}

            if ( is_label == false ) {
                j = j + 1;
                
            }

	    }


		line = line + 1;
		current_node = current_node->next;
		
	}

	//     POR OS FREES   free(read_string);


    
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
            
                fprintf(output_file, "%s %.3X", current_node->strings[0], current_node->integers[2]);

            
            
            
            
            }
            
            else {    

                word = current_node->strings[0];
                fprintf(output_file, "%.3X %.2s %.3s %.2s %.3s\n", current_node->integers[0], word, 
                        &(word[2]), &(word[5]), &(word[7]));
            
 
            
            }
            
            current_node = current_node->next;

        }

    }


}

// usar ctype
// usar mais um integer pra wfill?
