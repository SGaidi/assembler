#include "assembler.h"

/* getters & setters for our global-like variables */
#define ROOT root_data(NULL, 0)
#define CURR curr_data(NULL, 0)
#define SET_ROOT(new) root_data(new, 1)
#define SET_CURR(new) curr_data(new, 1)

#define SET_DATA_VALUES_OF(node) {		\
			node->content = content;	\
			node->address = address;	\
		}

typedef struct datanode {
	struct datanode *next;
	/* content */
	unsigned int content: CONTENT_BITS;
	signed int address: ADDRESS_BITS; 
} data;
