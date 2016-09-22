/*  Trabalho 1 - MC404 : Montador de IAS
    Nome: Ruy Castilho Barrichelo RA: 177012 */

// Implementation of a linked list with head-node

// INCLUDES
#include <stdbool.h>
#include <string.h>

// DEFINES
#define STRING_SIZE 64
#define STRING_NUMBER 3

// List struct with head-node containing strings as data variables
typedef struct node {
    char **strings;
    
	struct node *next;

} *List, node;

/* 
	Function: Inserts a node in end of the list;
    Param: Three string variables to be stored in the new node;

    Return: void.
*/

void list_insert(List list, char *string1, char *string2, char *string3) {

    node *current_node = list, *new_node = (node*)malloc(sizeof(node*));
    new_node->strings = (char **)malloc(STRING_NUMBER*sizeof(char **));

    new_node->strings[0] = string1;
    new_node->strings[1] = string2;
    new_node->strings[2] = string3;

    new_node->next = NULL;

    while (current_node->next != NULL) {
        current_node = current_node->next;

    }
    
    current_node->next = new_node;

    return;

}


/* 
	Function: Searches a key label in a list;
    Param: Pointer to the head-node, String used as key;

    Return: Boolean that equals true if key was found.
*/
node* list_search(List list, char* key) {

    node *current_node = list->next;
    
    while (current_node != NULL) {
    
    	for(int i=0; i < STRING_NUMBER; i++) {
    	
		    // If strings are the same
		    if ( strcmp(current_node->strings[i], key) == 0 ) {
		        return current_node;

			}
        }
        current_node = current_node->next;  
    }

    return NULL;

}



/* 
	Function: Creates a list;
    Param: None;

    Return: Pointer to a new list.
*/
List createList() {
    List new_list = (List)malloc(sizeof(List));
    
    new_list->strings = (char **)malloc(STRING_NUMBER*sizeof(char **));
    new_list->next = NULL;
    return new_list;

}






/* ------------------------------------ */

/* 
	Function: ;
    Param: ;

    Return: .
*/
