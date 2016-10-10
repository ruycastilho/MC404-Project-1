/*  Trabalho 1 - MC404 : Montador de IAS
    Nome: Ruy Castilho Barrichelo RA: 177012 */

// HEADER FILE
// Implementation of a linked list with head-node


// HEADER GUARD
#ifndef LIST_H
#define LIST_H

// INCLUDES
#include "list.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> 

// DEFINES
#define STRING_SIZE 64
#define STRING_NUMBER 3
#define INTEGER_NUMBER 3

// List struct with head-node containing strings as data variables
typedef struct node {
    char **strings;
    int *integers;

	struct node *next;

} *List, node;


/* 
	Function: Inserts a node in end of the list;
    Param: Three string and three integer variables to be stored in the new node;

    Return: void.
*/
void list_insert(List list, char *string1, char *string2, char *string3, int value1, int value2, int value3);


/* 
	Function: Inserts a node in a list, sorted by two integers;
    Param: Three string and three integer variables to be stored in the new node;

    Return: void.
*/
void list_insert_sorted(List list, char *string1, char *string2, char *string3, int value1, int value2, int value3);



/* 
	Function: Searches a key label in a list;
    Param: Pointer to the head-node, String used as key;

    Return: Pointer to node if key was found. NULL, if not.
*/
node* list_search_string1(List list, char* key);


/* 
	Function: Creates a list;
    Param: None;

    Return: Pointer to a new list.
*/
List createList();


/* 
	Function: Releases allocated memory occupied by a list;
    Param: Pointer to head node of a list;

    Return: None.
*/
void list_free(List list);
 
#endif
