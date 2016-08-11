#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>

/* name: Sarah Gaidi
 * date: 17.7.2016 
 * * * * * * * * * * 
 * assembler.h
 * all libraries and all constants for error checking needed for the entire program.
 * * * * * * * * * *
 * the project
 * 
 * */

enum {ERROR = -1, OK}; /* for handeling errors */
enum {NO = 0, YES};
enum {DATA = 0, EXT, ENT, INST, NO_TOKEN}; /* token types */
enum {NOT_DEFINED = 0, EXTERNAL, ENTRY, NORMAL}; /* label types */

#define INITIAL_ADDRESS 100
#define MAX_ADDRESS 1000
#define MAX_LINE_LEN 80 /* max length of line in assembly */

#define ADDRESS_BITS 9u /* number of bits needed for address */
#define ADDRESS_CHARS 5u /* number of chars (includes '\0') needed for address in special 8 base */
#define CONTENT_BITS 15u /* number of total bits needed for content at data or instruction */
#define CONTENT_CHARS 6u /* number of chars (includes '\0') needed for content in special 8 base */

#endif /* ASSEMBLER_H */
