/*  Trabalho 1 - MC404 : Montador de IAS
    Nome: Ruy Castilho Barrichelo RA: 177012 */


// INCLUDES
//#include "list.c"


// DEFINES
#define LABEL_SIZE 65

typedef enum {

	LEFT,
	RIGHT	

} side;

/* 
	Function: ;
    Param: ;

    Return: .
*/


/* 
	Function: Realizes first run through the code, thus creates the label list;
    Param: Command list;

    Return: None.
*/

void first_analysis(List list) {

	node *current_node = list;
	char *code
	bool is_directive;
	bool is_label;
	bool is_instruction;
	bool is_word;
	bool should_end;
	int address = 0;
	int side = LEFT;
	List label_list = createList();


	while ( current_node->next != NULL ) {

		word = current_node->strings[0];

		is_directive = false;
		is_label = false;
		is_instruction = false;
		is_word = false;
		should_end = false;

		for(int i=0; i < (LABEL_SIZE - 1) && !should_end ; i++) {

			if ( word[i] == ':' ) {
				is_label = true;
				should_end = true;
				word[i+1] = '\0';

			}

			else if ( word[i] == '.' ) {
				is_directive = true;
				should_end = true;

			}
			
			else if ( word[i] == ' ' ) {
				is_label = false;
				should_end = true;
				word[i] = '\0';
			}

		}


/*
		if ( side == LEFT ) {
			side = RIGHT;

		}
		else {
			side = LEFT;
			position = position + 1;

		}
*/
		/* if align or org, call directive_treatment that returns new position? */
		
	}

	return;
}




























