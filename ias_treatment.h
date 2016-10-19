/*  Trabalho 1 - MC404 : IAS Assembler
    Nome: Ruy Castilho Barrichelo RA: 177012 */


// HEADER GUARD
#ifndef IAS_TREATMENT_H
#define IAS_TREATMENT_H

// INCLUDES
#include "ias_treatment.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/* 
	Function: Treats symbol arguments;
    Param: String containing sym argument;

    Return: Flase if sym is invalid. True, if valid.
*/
bool sym_treatment(char* macro);


/* 
	Function: Treats hexadecimal arguments;
    Param: String containing hexadecimal argument;

    Return: Negative integer if number is invalid. Converted number, if is valid.
*/
int hex_treatment(char* macro);


/* 
	Function: Treats decimal arguments;
    Param: String containing decimal argument;

    Return: Negative integer if number is invalid. Positive, if is valid.
*/
int dec_treatment(char* macro);


/* 
	Function: Treats line, checking if elements are in valid order;
    Param: String pointing to the end of a instruction/directive, pointers to 
           integer input text line, boolean error flag, and output_file;

    Return: True, if line is valid, false, if not.
*/
bool line_treatment(char* end_line, int* line, bool* error_ocurred, FILE* output_file);


/* 
	Function: Treats errors, prints message accordingly.
    Param:  Pointer to line integer, strings containing element with error and
    		corresponding error message, pointer to output_file;

    Return: None.
*/
void error_treatment(int* line, char* element, char* event, bool* error_ocurred, FILE* output_file);


/* 
	Function: Treats words, converting values to hexadecimal algarisms in a string;
    Param: Integer to be converted, string to store the converted number;

    Return: None.
*/
void word_treatment(long int integer, char* hex_string);

#endif
