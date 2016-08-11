#include "assembler.h"

#define MAX_LABEL_LEN 30 /* max length of label in assembly */

/* getters & setters for our global-like variables */
#define ROOT root_label(NULL, 0)
#define CURR curr_label(NULL, 0)
#define SET_ROOT(new) root_label(new, 1)
#define SET_CURR(new) curr_label(new, 1)

#define SET_LABEL_VALUES_OF(node) {			\
			node->name = new_name(name);	\
			node->address = address;		\
			node->type = type;				\
		}

typedef struct labelnode {
	struct labelnode *next;
	/* content */
	char *name;
	unsigned int address: ADDRESS_BITS;
	unsigned int type: 2; /* no decleration, error 00, entry 01, external 10, normal 11 */
} label;


