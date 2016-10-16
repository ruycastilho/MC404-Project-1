/*  Trabalho 1 - MC404 : Montador de IAS
    Nome: Ruy Castilho Barrichelo RA: 177012 */


// HEADER GUARD
#ifndef IAS_ANALYSIS_H
#define IAS_ANALYSIS_H

// INCLUDE
#include "ias_analysis.h"
#include "list.h"
#include "ias_treatment.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// DEFINES
#define COMMAND_SIZE 65 // 64 characters + eventual '\0'
#define MAX_MEMORY 1023

// Enum used to identify sides
typedef enum {

    LEFT,
    RIGHT

} sides;


/* 
	Function: Performs first run through the commands, thus creates the label list;
    Param: Pointers to command list, label list and memory map (NULL in this case, 
           used in other functions) and pointer to output file, if the latter exists;

    Return: True if error ocurred during analysis. False, if not.
*/
bool first_analysis(List command_list, List label_list, List memory_map, FILE* output_file);


/* 
	Function: Stores a new label or set constant and its address;
    Param: String containing label/constant, label list, 'address' and 
            'side' integers, pointers to both 'line' integer, boolean error flag,
            and output file;

    Return: None.
*/
void label_analysis(char* label, List list, int address, int side, int* line, bool* error_ocurred, FILE* output_file);


/* 
	Function: Identifies instructions;
    Param: String containing instruction, pointer to both memory map and label lists, pointers to 'address',
            'side' and 'line' integers and to boolean error flag, pointer to output file;

    Return: None.
*/
void instruction_analysis(char* instruction, List memory_map, List label_list, int* address, int* side, int* line, bool* error_ocurred, FILE* output_file);


/* 
	Function: Identifies and treats directives;
    Param: String containing directives, pointers to memory map(null if not used)
           and label lists, pointers to 'address' and 'side' integer and to 
           boolean error flag, pointer to output_file;

    Return: None.
*/
void directive_analysis(char* directive, List memory_map, List label_list, int *address, int *side, int *line, bool* error_ocurred, FILE* output_file);


/* 
	Function: Performs second run through the code, thus creates the label list;
    Param: Pointers to command list, label list, memory map list and pointer to 
           output file, if the latter exists;

    Return: True if error ocurred during analysis. False, if not.
*/
bool second_analysis(List command_list, List label_list, List memory_map, FILE* output_file);

/* 
	Function: Performs second run through the commands;
    Param: Command, label and memory map lists, pointer to output file;

    Return: True if error ocurred during analysis. False, if not.
*/
void print_map(List memory_map, FILE* output_file);

#endif
