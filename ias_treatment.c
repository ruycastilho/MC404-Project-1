/*  Trabalho 1 - MC404 : IAS Assembler
    Nome: Ruy Castilho Barrichelo RA: 177012 */
    
    
// INCLUDES
#include "ias_treatment.h"    
    
/* 
	Function: Treats symbol arguments;
    Param: String containing sym argument;
f
    Return: Flase if sym is invalid. True, if valid.
*/
bool sym_treatment(char* argument) {

    if ( argument != NULL ) {

        // Checks if first char is alphabetical or '_'
        if ( isalpha(argument[0]) || argument[0] == '_' ) {

            for(int i=0; argument[i] != '\0' ;i++) {

                // Checks if char is not alphanumerical nor '_'
                if ( !isalnum(argument[i]) && argument[i] != '_' ) {
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
int hex_treatment(char* argument) {

	int address = 0;
	char *end;
    bool is_zero = true;

    // Checks if size is correct
	if ( argument[12] == '\0' ) {

        int j = 2;

        // Checks if all digits are zero
	    while ( !isspace(argument[j]) && argument[j] != '\0' ) {

		    if ( argument[j] != '0' ) {
			    is_zero = false;

		    } 
            j = j + 1;

	    }

        // If it is not zero
		if ( is_zero == false ) {

			// Converts string into integer
			address = strtol(argument, &end, 16);
			
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
int dec_treatment(char* argument) {

	int address = 0;
	char *end;
    bool is_zero = true;

    int j = 0;

	// Checks if all digits are zero
	while ( !isspace(argument[j]) && argument[j] != '\0' ) {

		if ( argument[j] != '0' ) {
			is_zero = false;

		} 
        j = j + 1;

	}

    // If it is not zero
	if ( is_zero == false ) {

		// Converts string into integer
		address = strtol(argument, &end, 10);

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

	// Check if line is valid. If line ended, or reached a comment, is valid.
    if ( end_line[i] != '\0' && end_line[i] != '#' ) {
		error_treatment(line, NULL, "line input", error_ocurred, output_file);
        return false;

    }

    return true;

}

/* 
	Function: Treats errors, prints message accordingly.
    Param:  Pointer to line integer, strings containing element with error and
    		corresponding error message, pointer to output_file;

    Return: None.
*/
void error_treatment(int* line, char* element, char* event, bool* error_ocurred, FILE* output_file) {

	// String to be printed
	char message[50];

	// Sets error flag as true
    *(error_ocurred) = true;


	// If message contains an element	
	if ( element == NULL ) {

		// Formats output message
		strcpy(message, "Invalid ");
		strcat(message, event);
		strcat(message, "!\n");

		// If there is no output file
        if ( output_file == NULL ) {
        	fprintf(stdout, "%s %d%s%s", "ERROR on line", *line, "\n", message);         
        
        }
        else {
            fprintf(output_file, "%s %d%s%s", "ERROR on line", *line, "\n", message);        

        }

	}

	// If message only contains an event
	else {

		// Formats output message
		strcpy(message, ": invalid ");
		strcat(message, event);
		strcat(element, message);
		strcat(element, "!\n");

		// If there is no output file
        if ( output_file == NULL ) {
        	fprintf(stdout, "%s %d%s%s", "ERROR on line", *line, "\n", element);         
        
        }
        else {
            fprintf(output_file, "%s %d%s%s", "ERROR on line", *line, "\n", element);

		}

	}

}

/* 
	Function: Treats words, converting values to hexadecimal algarisms in a string;
    Param: Integer to be converted, string to store the converted number;

    Return: None.
*/
void word_treatment(long int integer, char* hex_string) {

    int value = 0;
    int bits[64]; // Stores 64 bits of a long int
    int i;
    int bit_value = 1; // Used as the value of the bit during the conversion 

    // Obtains the bits
    for(i=0; i < sizeof(long int)*8 ; i++ ) {
        bits[i] = (integer >> i) & 1;

    }

    // Runs through the bits in packs of 4 bits
    for(int k=0; k < sizeof(long int)*8 ; k = k + 4) {

        // Obtains the value of each bit
        for(i=k, value=0, bit_value=1; i < (k + 4) ; i++, bit_value = bit_value*2 ) {
            value = value + bits[i]*bit_value;

        }

        // Converts into hexadecimal
        if ( value < 10 ) {
            hex_string[15-(k/4)] = '0' + value;

        }

        else {
            hex_string[15-(k/4)] = 'A' + value - 10;
        
        }

    }

}
