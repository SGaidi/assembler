#include "instruction.h"

signed char first(FILE *ifp) {
	void reset_label_list(void);
	void reset_data_list(void);
	void reset_instruction_list(void);
	char handle_first_token(char *label_flag, char *line, char **token, unsigned int address);
	char handle_data(char *line, char *token, int line_counter, unsigned int *address);
	char handle_instruction(char *line, char *token, int line_counter, unsigned int *address);
	char add_ext_label(char **token, int line_counter);
	int token_type(char *token);
							
	char label_flag, error_flag = OK;
	char *line = NULL, *token = NULL;
    int line_counter = 1, type = ERROR;
    unsigned int address = INITIAL_ADDRESS;
    size_t len_dummy; /* makes iso compilers happy */
    
    /* reset all lists for this file */
    reset_data_list();
    reset_label_list();
    reset_instruction_list();
    rewind(ifp);
    
     while(getline(&line, &len_dummy, ifp) != -1 && address <= MAX_ADDRESS) { /* for every line */
        if(strlen(line) > MAX_LINE_LEN) { /* checks line length */
			fprintf(stderr, "Line %d has more than %d characters.\n", line_counter, MAX_LINE_LEN);
			error_flag = ERROR;
		}
		
		label_flag = NO;
		
		/* check if this line has any tokens to compile. we assume "empty" lines are't an error */
        if((token = strtok(line," \t\n")) != NULL && *token != ';') { /* if it's not an empty or comment line */
			
			/* if first token is a label - handles it */
			if(handle_first_token(&label_flag, line, &token, address) == YES) {
				fprintf(stderr, "Line %d: the label is already declared in the file.\n", line_counter);
				error_flag = ERROR;
			}

			type = token_type(token);
			switch(type) {
				case DATA:
					if(handle_data(line, token, line_counter, &address) == ERROR)
						error_flag = ERROR;
					break;
				case ENT:
				case EXT:
					if(label_flag == YES) {
						fprintf(stderr, "Line %d: label is not useable with .external or .entry.\n", line_counter);
						error_flag = ERROR;
					}
					if(type == EXT)
						if(add_ext_label(&token, line_counter) == ERROR)
							error_flag = ERROR;
					break;
				case INST:
					if(handle_instruction(line, token, line_counter, &address) == ERROR)
						error_flag = ERROR;
					break;
				case ERROR:
					fprintf(stderr, "Line %d: token \"%s\" is not a valid command%s.\n", line_counter, token, (!label_flag) ? " or label" : "");
					error_flag = ERROR;
					break;
				case NO_TOKEN:
					fprintf(stderr, "Line %d: label has no command.\n", line_counter);
					error_flag = ERROR;
					break;
				default:
					fprintf(stderr, "Critical error at first.c: not supposed to happen.\n"
									"token_type should only return the above values (at the switch).\n");
					error_flag = ERROR;
					break;
				}
		}
        line_counter++;
    }
    
    if(address > MAX_ADDRESS) {
		fprintf(stderr, "No more memory in virtual computer, max address is: %d.\n", MAX_ADDRESS);
		error_flag = ERROR;
	}
	
	return error_flag;
}
