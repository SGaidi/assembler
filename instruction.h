#include "assembler.h"

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#define CMD_MAX_LEN 6 /* longest: "error" */
#define MAX_ADDRESSING_METHODS 4 /* 0,1,2,3 */
#define MAX_PARAMS 2
#define MAX_PARAM_LEN 80

/* bitwise sizes constants */
#define EMPTY_BITS 3
#define GROUP_BITS 2
#define OP_CODE_BITS 4
#define SOURCE_BITS 2
#define TARGET_BITS 2
#define ERA_BITS 2

/* content of bitwises
 * they are declared with define and not enums to ensure they're unsigned when printing to file */
#define EMPTY_CONTENT 5u /* = 101 */

/* era encoding */
#define ABSOLUTE 0u
#define EXTERNAL 1u
#define RELOCATABLE 2u

/* source and destination addressing methods */
#define IMMEDIATE 0u /* number with # */
#define DIRECT 1u /* label */
#define DINAMIC 2u /* with index */
#define REGISTER 3u
#define NO_METHODS 4u

/* getters & setters for our global-like variables */
#define ROOT root_instruction(NULL, 0)
#define CURR curr_instruction(NULL, 0)
#define SET_ROOT(new) root_instruction(new, 1)
#define SET_CURR(new) curr_instruction(new, 1)

#define SET_INSTRUCTION_VALUES_OF(node) {			\
			node->address = address;				\
			node->contents.whole = whole_content;	\
		}

/* data types */
struct command {
	char name[CMD_MAX_LEN];
	unsigned int dest_method[MAX_ADDRESSING_METHODS]; /* valid destination addressing methods */
	unsigned int source_method[MAX_ADDRESSING_METHODS]; /* valid source addressing methods */
	unsigned int group; /* 0,1,2 - number of parameters exactly needed */
	};
	
typedef struct commandnode {
	unsigned int era:ERA_BITS; /* absolute, external, relocatale */
	unsigned int target:TARGET_BITS; /* addressing of target as above */
	unsigned int source:SOURCE_BITS; /* immediate, direct, direct-offset and register addressing of source */
	unsigned int opcode:OP_CODE_BITS; /* mov, cmp, ... ,stop */
	unsigned int group:GROUP_BITS; /* 0,1,2 operands. 3 for an error */
	unsigned int empty:EMPTY_BITS; /* for commands: always = 101 */
} commandbits;

typedef struct valuenode {
	unsigned int era:ERA_BITS;
	unsigned int content:(CONTENT_BITS-ERA_BITS);
} valuebits;
							
typedef struct instructnode {
	unsigned int address: ADDRESS_BITS;
	struct instructnode *next;
	/* content */
	union contentunion { /* in total, all have exactly CONTENT_BITS bits */
	commandbits command_bits;
	valuebits value_bits;
	unsigned int whole:CONTENT_BITS; /* we use this in convert_content to print to file */
	} contents;
} instruction;

#endif /* INSTRUCTION_H */
