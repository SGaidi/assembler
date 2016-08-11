#include "assembler.h"

signed char second(FILE *ifp) {
	int token_type(char *token);
	char add_ent_label(char **token, unsigned int address, int line_counter);
	char add_params(char *line, char *token, int line_counter, unsigned int *address);
	char handle_first_token(char *label_flag, char *line, char **token, unsigned int address);
	
	char error_flag = OK, label_flag;
	char *line = NULL, *token = NULL;
	int line_counter = 1, type = ERROR;
	unsigned int address = INITIAL_ADDRESS;
	size_t len_dummy; /* makes iso compilers happy */
	
	rewind(ifp);
	
	while(getline(&line, &len_dummy, ifp) != -1) { /* for every line */
		/* check if this line has any tokens to compile. we assume "empty" lines are't an error */
        if((token = strtok(line," \t\n")) != NULL && *token != ';') { /* if it's not an empty or comment line */
		
			/* if first token is a label - handles it */
			if(handle_first_token(&label_flag, line, &token, address) == ERROR)
				error_flag = ERROR;

			type = token_type(token);
			switch(type) {
				case ENT:
				case EXT:
					if(type == ENT)
						if(add_ent_label(&token, address, line_counter) == ERROR)
							error_flag = ERROR;
					break;
				case INST:
					address++;
					if(add_params(line, token, line_counter, &address) == ERROR)
						error_flag = ERROR;
					break;
				case DATA:
					address++;
					break;
				case ERROR:
				case NO_TOKEN:
					break;
				default:
					fprintf(stderr, "Critical error at second.c: not supposed to happen.\n"
									"token_type should only return the above values (at the switch).\n");
					error_flag = ERROR;
					break;
				}
		}
        line_counter++;
    }
	return error_flag;
}
