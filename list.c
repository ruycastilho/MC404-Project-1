/*  Trabalho 1 - MC404 : Montador de IAS
    Nome: Ruy Castilho Barrichelo RA: 177012 */

// Implementation of a linked list with head-node

// INCLUDES
#include "list.h"

/* 
	Function: Inserts a node in end of the list;
    Param: Three string variables to be stored in the new node;

    Return: void.
*/
void list_insert(List list, char *string1, char *string2, char *string3, int value1, int value2) {

    node *current_node = list;
    node *new_node = (node*)malloc(sizeof(node));
    new_node->strings = (char **)malloc(STRING_NUMBER*sizeof(char *));
    new_node->integers = (int *)malloc(INTEGER_NUMBER*sizeof(int));

	for(int i=0; i < STRING_NUMBER ;i++) {
		new_node->strings[i] = (char *)malloc(STRING_SIZE*sizeof(char));

	}

	if ( string1 != NULL ) {
    	strcpy(new_node->strings[0], string1);

	}

	if ( string2 != NULL ) {
    	strcpy(new_node->strings[1], string2);

	}

	if ( string3 != NULL ) {
    	strcpy(new_node->strings[2], string3);

	}

    new_node->integers[0] = value1;
    new_node->integers[1] = value2;

    new_node->next = NULL;



    while (current_node->next != NULL) {
        current_node = current_node->next;

    }

    current_node->next = new_node;

    return;

}


/* 
	Function: Inserts a node in a list, sorted by two integers;
    Param: Three string variables to be stored in the new node;

    Return: void.
*/
void list_insert_sorted(List list, char *string1, char *string2, char *string3, int value1, int value2) {

    node *current_node = list;
    node *new_node = (node*)malloc(sizeof(node));
    new_node->strings = (char **)malloc(STRING_NUMBER*sizeof(char *));
    new_node->integers = (int *)malloc(INTEGER_NUMBER*sizeof(int));

	for(int i=0; i < STRING_NUMBER ;i++) {
		new_node->strings[i] = (char *)malloc(STRING_SIZE*sizeof(char));

	}

	if ( string1 != NULL ) {
    	strcpy(new_node->strings[0], string1);

	}

	if ( string2 != NULL ) {
    	strcpy(new_node->strings[1], string2);

	}

	if ( string3 != NULL ) {
    	strcpy(new_node->strings[2], string3);

	}

    new_node->integers[0] = value1;
    new_node->integers[1] = value2;

    new_node->next = NULL;


    while ( current_node->next->integers[0] < value1 ) {
        current_node = current_node->next;

    }

    if ( current_node->next->integers[0] == value1 && current_node->next->integers[1] < value2 ) {
       current_node = current_node->next;  
    
    }

    new_node->next = current_node->next;
    current_node->next = new_node;

    return;


}


/* 
	Function: Searches a key label in a list;
    Param: Pointer to the head-node, String used as key;

    Return: Pointer to node if key was found. NULL, if not.
*/
node* list_search_string1(List list, char* key) {

    node *current_node = list;

    while ( current_node->next != NULL ) {
		current_node = current_node->next;

	    // If strings are the same
	    if ( strcmp(current_node->strings[0], key) == 0 ) {
	        return current_node;

		}

    }

    return NULL;

}



/* 
	Function: Creates a list;
    Param: None;

    Return: Pointer to a new list.
*/
List createList() {
    List new_list = (List)malloc(sizeof(node));
    
    //new_list->strings = (char **)malloc(STRING_NUMBER*sizeof(char **));
    new_list->next = NULL;

    //new_list->integers = (int *)malloc(STRING_NUMBER*sizeof(int *));

    return new_list;

}






/* ------------------------------------ */

/* 
	Function: ;
    Param: ;

    Return: .
*/
