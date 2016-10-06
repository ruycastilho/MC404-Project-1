/*  Trabalho 1 - MC404 : Montador de IAS
    Nome: Ruy Castilho Barrichelo RA: 177012 */


// HEADER GUARD
#ifndef AS_ROUTINES_H
#define AS_ROUTINES_H

// INCLUDE
#include "as_routines.h"
#include "list.h"
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// DEFINES
#define COMMAND_SIZE 65 // 64 characters + eventual '\0'
#define MAX_MEMORY 1024

// Enum used to identify sides
typedef enum {

    LEFT,
    RIGHT

} sides;


/* 
	Function: Performs first run through the code, thus creates the label list;
    Param: Command and label list;

    Return: True if error ocurred during analysis. False, if not.
*/
bool first_analysis(List command_list, List label_list);

/* 
	Function: Stores a new label and its address;
    Param: String containing label, label list, 'address' and 
            'side' integers, pointer to both 'line' integer and boolean error flag;

    Return: None.
*/
void label_treatment(char* label, List list, int address, int side, int* line, bool* error_ocurred);


/* 
	Function: Identifies instructions;
    Param: String containing instruction, pointer to both memory map and label lists, pointers to 'address',
            'side' and 'line' integers and to boolean error flag;

    Return: None.
*/
void instruction_treatment(char* instruction, List memory_map, List label_list, int* address, int* side, int* line, bool* error_ocurred) ;


/* 
	Function: Treats symbol arguments;
    Param: String containing sym argument;

    Return: Flase if sym is invalid. True, if valid.
*/
bool sym_treatment(char* macro);


/* 
	Function: Treats hexadecimal arguments;
    Param: ;

    Return: Negative integer if number is invalid. Converted number, if is valid.
*/
int hex_treatment(char* macro);


/* 
	Function: Treats decimal arguments;
    Param: ;

    Return: Negative integer if number is invalid. Positive, if is valid.
*/
int dec_treatment(char* macro);


/* 
	Function: Identifies and treats directives;
    Param: String containing directives, pointers to 'address'
            'side' integer and to boolean error flag;

    Return: None.
*/
void directive_treatment(char* directive, List label_list, int *address, int *side, int *line, bool* error_ocurred);


/* 
	Function: Performs second run through the code, thus creates the label list;
    Param: Command list;

    Return: True if error ocurred during analysis. False, if not.
*/
bool second_analysis(List command_list, List label_list, List memory_map);

/* 
	Function: Prints memory map either in stdout or in a output file;
    Param: Pointer to memory map list, string containing name of the output
           file or NUll if map should be printed in stdout;
           
    Return: None.
*/
void print_map(List memory_map, char* output_file_name);

#endif
