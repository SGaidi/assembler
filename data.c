#include "data.h"

/* global-like variable for counting data
 * data_convert(0) returns current number of datas
 * data_convert(-data_convert(0)) sets it to 0 */
int data_counter(int add) {
	static int counter = 0;
	counter += add;
	return counter;
}

/* root_data() and curr_data() are global-like variables:
 * root_data(NULL, YES) or ROOT returns the root
 * root_data(new, NO) is root = new assigment
 * the same as above with curr_data()
 * both can be accessed throughout the entire data.c file */
 
data *root_data(data *new, int set) {
	static data *root = NULL;
	if(set) /* set == YES */
		root = new;
	return root;
}

data *curr_data(data *new, int set) {
	static data *curr = NULL;
	if(set)
		curr = new;
	return curr;
}

/* frees all the space list has taken */
void reset_data_list(void) {
	data *temp;
	
	while(ROOT != NULL && ROOT->next != NULL) { 
		temp = ROOT;
		SET_ROOT(ROOT->next);
		free(temp);
	}
	
	SET_CURR(NULL);
	SET_ROOT(NULL);
	data_counter(-data_counter(0)); /* resets counter */
}

char is_data_empty(void) {
	if(ROOT == NULL)
		return YES;
	return NO;
}

/* new_data clears space and returns a pointer */
data *new_data(void) {
	data *p;
	
	if((p = (data*)malloc(sizeof(data))) == NULL) {
		fprintf(stderr, "At new_data: Not enough space.\n");
		exit(1);
	}
		
	p->next = NULL; 
	return p;
}

/* add a data to a list that starts at *root */
void add_data(unsigned int address, signed int content) {
	
	if(is_data_empty()) { /* if it's the first data on list */
		if(SET_ROOT(new_data()) != NULL) {
			SET_DATA_VALUES_OF(ROOT);
			SET_CURR(ROOT);
		}
	} else if((CURR->next = new_data()) != NULL) {
			SET_CURR(CURR->next);
			SET_DATA_VALUES_OF(CURR);
		}
		
	data_counter(1);
}

unsigned int get_data_content(unsigned int address) {
	data *temp = ROOT;
	
	while(temp != NULL) {
		if(temp->address == address) {
			return temp->content;
		}
		temp = temp->next;
	}
		
	return NO;
}

/* checks input and gets string
 * doesn't check escape sequences and assumes the text inside two double quotes is valid */
char get_string(char *line, int line_counter, unsigned int *address) {
	char *ptr, *temp;
	
	if((temp = (char*)malloc(strlen(line) + 1)) == NULL) {
		fprintf(stderr, "At get_string: Not enough space.\n");
		return ERROR;
	}
	strcpy(temp,line);
		
	if(*temp != '\"' || *(temp + strlen(temp) - 2) != '\"') { /* checks double quotes */
		fprintf(stderr, "Line %d: no valid starting and closing double quotes at .string command.\n",
						line_counter);
		return ERROR;
	}
	
	temp++; /* skips the starting double quote */

	if((ptr = (char*)malloc(strlen(temp) + 1)) == NULL) {
		fprintf(stderr, "At get_string: Not enough space.\n");
		return ERROR;
	}
	else
		strcpy(ptr,temp); /* copies without the starting double quote */
	
	while(*temp != '\0')
		temp++;
	
	*(ptr + strlen(ptr) - 2) = '\0'; /* deletes the ending double quote */
	
	do { /* add datas */
		add_data((*address)++, *(ptr++));
		} while(*(ptr-1) != '\0');
		
			
	return OK;
}

char get_data(char *line, int line_counter, unsigned int *address) {
	void skip_white_space(char **line);
	char *ptr;
	int i,j, commas = 0;
	
	line = strtok(NULL, "");
	
	if((ptr = (char*)malloc(strlen(line) + 1)) == NULL) {
		fprintf(stderr, "At get_data: Not enough space.\n");
		return ERROR;
	}
	
	skip_white_space(&line);
	
	for(i = 1; *line != '\0'; i++) {

		for(j = 0; *line != ' ' && *line != '\t' && *line != ',' && *line != '\n' && *line != '\0'; j++)
			*(ptr+j) = *(line++);
		*(ptr+j) = '\0'; /* saving the argument in ptr */
		
		skip_white_space(&line);
		
		if(strcmp("0", ptr) == 0 || atoi(ptr) != 0)
			add_data((*address)++, atoi(ptr));
		else {
			fprintf(stderr, "Line %d: argument number %d - %s isn't a valid number.\n", line_counter, i, ptr);
			return ERROR;
		}
		
		if(*line == ',') {
			line++;
			commas++;
		}
			
		skip_white_space(&line);
	}
	
	if(commas != i - 2) {
		fprintf(stderr, "Line %d: invalid commas.\n", line_counter);
		return ERROR;
	}
	
	return OK;
}

/* handles .data and .string commands. saves in token the values (characters or numbers) */
char handle_data(char *line, char *token, int line_counter, unsigned int *address) {
	
	if(strcmp(".string", token) == 0) { /* .string */
		/* puts the line pointer starting from the next character after ".string" */
		if(get_string((token + strlen(token) + 1), line_counter, address) == ERROR)
			return ERROR;
	} else if(get_data(line, line_counter, address) == ERROR) /* .data */
				return ERROR;
				
	return OK;
}

void print_data_to_file(FILE *ofp) {
	char *convert_address(int address);
	char *convert_content(int content);
	data *temp = ROOT;
	
	while(temp != NULL) {
		fprintf(ofp, "%s\t%s\n", convert_address(temp->address), convert_content(temp->content));
		temp = temp->next;
	}
}
