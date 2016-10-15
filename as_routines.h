/*  Trabalho 1 - MC404 : Montador de IAS
    Nome: Ruy Castilho Barrichelo RA: 177012 */


// HEADER GUARD
#ifndef AS_ROUTINES_H
#define AS_ROUTINES_H

// INCLUDE
#include "as_routines.h"
#include "list.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

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
    Param: Pointers to command list, label list and memory map (NULL in this case, 
           used in other functions) and pointer to output file, if the latter exists;

    Return: True if error ocurred during analysis. False, if not.
*/
bool first_analysis(List command_list, List label_list, List memory_map, FILE* output_file);

/* 
	Function: Stores a new label and its address;
    Param: String containing label, label list, 'address' and 
            'side' integers, pointers to both 'line' integer, boolean error flag,
            and output file;

    Return: None.
*/
void label_treatment(char* label, List list, int address, int side, int* line, bool* error_ocurred, FILE* output_file);


/* 
	Function: Identifies instructions;
    Param: String containing instruction, pointer to both memory map and label lists, pointers to 'address',
            'side' and 'line' integers and to boolean error flag, pointer to output file;

    Return: None.
*/
void instruction_treatment(char* instruction, List memory_map, List label_list, int* address, int* side, int* line, bool* error_ocurred, FILE* output_file);


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
    Param: String containing directives, pointers to memory map(null if not used)
           and label lists, pointers to 'address' and 'side' integer and to 
           boolean error flag, pointer to output_file;

    Return: None.
*/
void directive_treatment(char* directive, List memory_map, List label_list, int *address, int *side, int *line, bool* error_ocurred, FILE* output_file);


/* 
	Function: Performs second run through the code, thus creates the label list;
    Param: Pointers to command list, label list, memory map list and pointer to 
           output file, if the latter exists;

    Return: True if error ocurred during analysis. False, if not.
*/
bool second_analysis(List command_list, List label_list, List memory_map, FILE* output_file);

/* 
	Function: Prints memory map either in stdout or in a output file;
    Param: Pointer to memory map list, output file or NUll if map should 
    be printed in stdout;
           
    Return: None.
*/
void print_map(List memory_map, FILE* output_file);

#endif
