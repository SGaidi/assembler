#include "assembler.h"
#include "label.h"

/* root() and curr() are global-like variables:
 * root(NULL, YES) or ROOT returns the root
 * root(new, NO) is root = new assigment
 * the same as above with curr()
 * both can be accessed throughout the entire label.c file */
 
label *root_label(label *new, int set) {
	static label *root = NULL;
	if(set) /* set == YES */
		root = new;
	return root;
}

label *curr_label(label *new, int set) {
	static label *curr = NULL;
	if(set)
		curr = new;
	return curr;
}

/* frees all the space list has taken */
void reset_label_list(void) {
	label *temp;
	
	while(ROOT != NULL && ROOT->next != NULL) { 
		temp = ROOT;
		SET_ROOT(ROOT->next);
		free(temp);
	}
	
	SET_CURR(NULL);
	SET_ROOT(NULL);
}

char is_label_empty(void) {
	if(ROOT == NULL)
		return YES;
	return NO;
}

char is_label_type_empty(unsigned int type) {
	label *temp = ROOT;
	
	while(temp != NULL) {
		if(temp->type == type)
			return NO;
		temp = temp->next;
	}
	return YES;
}

/* new_name and new_label clear space and return a pointer */
char *new_name(char *name) {
	char *p;
	int len = (strlen(name) + 1);
	
	if((p = (char*)malloc(len * sizeof(char))) == NULL) {
		fprintf(stderr, "At new_name: Not enough space.\n");
		exit(1);
	}
		
	strcpy(p,name);
	return p;
}

label *new_label(void) {
	label *p;
	
	if((p = (label*)malloc(sizeof(label))) == NULL) {
		fprintf(stderr, "At new_label: Not enough space.\n");
		exit(1);
	}
		
	p->next = NULL;	
	return p;
}

/* add a label to a list that starts at ROOT */
void add_label(char *name, unsigned int address, unsigned char type) {
	if(is_label_empty()) { /* if it's the first label on list */
		if(SET_ROOT(new_label()) != NULL) {
			SET_LABEL_VALUES_OF(ROOT);
			SET_CURR(ROOT);
		}
	} else if((CURR->next = new_label()) != NULL) {
			SET_CURR(CURR->next);
			SET_LABEL_VALUES_OF(CURR);
		}
}

/* checks if *token is a label */
int is_label(char **token) {
	char is_command(char *token);
	char *ptr = "";
	
	if(token == NULL || *token == NULL || **token == '\0')
		return NO; 

	if(strlen(*token) > MAX_LABEL_LEN) {
		fprintf(stderr, "The length of token %s is longer than allowed for a label or assembly word (%d).\n",
						*token, MAX_LABEL_LEN);
		return NO;
	}
	
	/* checks it's not an assembly word */
	if(strcmp(*token, ".data") == 0 || strcmp(*token, ".string") == 0 ||
	   strcmp(*token, ".extern") == 0 || strcmp(*token, ".entry") == 0)
		return NO;
	if(is_command(*token) == YES)
		return NO;
	if(*ptr == 'r' && (*(ptr + 1) >= '0' && *(ptr + 1) <= '7')) /* it's a register name */
		return NO;

	if((ptr = (char*)malloc(strlen(*token) + 1)) == NULL) {
		fprintf(stderr, "At is_label: Not enough space.\n");
		exit(1);
	}
	strcpy(ptr,*token);
	
	if(!isalpha(*ptr)) /* label must start with a-z or A-Z */
		return NO;
		
	while(*ptr) {
		if(!isdigit(*ptr) && !isalpha(*ptr))
			return NO;
		ptr++;
	}
		
	return YES;
}

/* takes a valid label and searches it in the label table, if not found, adds it.
 * returns whether it was added or if there is an error.
 * below more searches by different queries and results. */
int search_label(char *name) {
	label *temp = ROOT;
	
	while(temp != NULL) {
		if(strcmp(temp->name, name) == 0)
			return YES;
		temp = temp->next;
	}
	
	return NO;
}

int search_ext_label(char *name, unsigned int address) {
	label *temp = ROOT;
	
	while(temp != NULL) {
		if(strcmp(temp->name, name) == 0 && temp->type == EXTERNAL && temp->address == address)
			return YES;
		temp = temp->next;
	}
	
	return NO;
}

unsigned int get_address(char *name) {
	label *temp = ROOT;
	
	while(temp != NULL) {
		if(strcmp(temp->name, name) == 0)
			return temp->address;
		temp = temp->next;
	}
	
	return ERROR;
}

int update_label_type(char *name, int type) {
	label *temp = ROOT;
	
	while(temp != NULL) {
		if(strcmp(temp->name, name) == 0) {
			temp->type = type;
			return OK;
		}
		temp = temp->next;
	}
		
	fprintf(stderr, "Critical error at update_label: the instruction added at first "
					"is not found to be updated.\n");
	return ERROR;
}

int search_type_label(char *name, int type) {
	label *temp = ROOT;
	
	while(temp != NULL) {
		if(strcmp(temp->name, name) == 0 && temp->type == type)
			return YES;
		temp = temp->next;
	}
	
	return NO;
}

int get_label_type(char *name) {
	label *temp = ROOT;
	
	while(temp != NULL) {
		if(strcmp(temp->name, name) == 0)
			return temp->type;
		temp = temp->next;
	}
	
	return ERROR;
}

/* handels the first token, identifies wheter it is a label. and if it is, adds it. */
char handle_first_token(char *label_flag, char *line, char **token, unsigned int address) {
	char label_found = NO;
	*label_flag = NO;
	
	/* checks if there is any white space before token, and if it's valid */
	if(*(*token + strlen(*token) - 1) == ':' && *line != ' ' && *line != '\t') {
		*(*token + strlen(*token) - 1) = '\0';
		if(is_label(token) == YES) {
			if(search_label(*token) == YES)
				label_found = YES;
			else
				add_label(*token, address, NORMAL);
			*label_flag = YES;
			*token = strtok(NULL," \t\n"); /* gets next token */
		}
	}
	
	return label_found;
}

char add_ext_label(char **token, int line_counter) {
	*token = strtok(NULL, " \t\n");
	
	if(is_label(token) == YES) {
		if(search_label(*token) == NO) {
			add_label(*token, INITIAL_ADDRESS-1, EXT);
			return OK;
		} else
			fprintf(stderr, "Line %d: label \"%s\" is already declared in the file.\n",
							line_counter, *token);
	} else if(*token != '\0')
				fprintf(stderr, "Line %d: token \"%s\" is is not a valid label.\n",
								line_counter, *token);
		   else
				fprintf(stderr, "Line %d: missing label after .extern.\n", line_counter);
		
	return ERROR;
}

char add_ent_label(char **token, unsigned int address, int line_counter) {
	*token = strtok(NULL, " \t\n");
	
	if(is_label(token) == YES) {
		if(search_label(*token) == YES) {
			update_label_type(*token, ENTRY);
			return OK;
		}
		else
			fprintf(stderr, "Line %d: label \"%s\" is already declared in the file.\n",
							line_counter, *token);
	} else if(*token != '\0')
				fprintf(stderr, "Line %d: token \"%s\" is is not a valid label.\n",
								line_counter, *token);
		   else
				fprintf(stderr, "Line %d: missing label after .entry.\n", line_counter);
	
	return ERROR;
}

void print_labels_to_file(FILE *ofp, int type) {
	char *convert_address(int address);
	label *temp = ROOT;
	
	while(temp != NULL) {
		if(temp->type == type && temp->address != INITIAL_ADDRESS-1)
			fprintf(ofp, "%s\t%s\n", temp->name, convert_address(temp->address));
		temp = temp->next;
	}
}
