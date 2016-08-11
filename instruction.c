#include "instruction.h"

/* global-like variable, get_commands returns all the commands and their
 * function, valid destination and source methods and group (respectively) */
struct command *get_commands(void) {
	static struct command cmds[] = { 
	{"mov",	{DIRECT,REGISTER},					{IMMEDIATE,DIRECT,DINAMIC,REGISTER},2u},
	{"cmp",	{IMMEDIATE,DIRECT,DINAMIC,REGISTER},{IMMEDIATE,DIRECT,DINAMIC,REGISTER},2u},
	{"add",	{DIRECT,REGISTER},					{IMMEDIATE,DIRECT,DINAMIC,REGISTER},2u},
	{"sub",	{DIRECT,REGISTER},					{IMMEDIATE,DIRECT,DINAMIC,REGISTER},2u},
	{"not",	{DIRECT,REGISTER},					{NO_METHODS},						1u},
	{"clr",	{DIRECT,REGISTER},					{NO_METHODS},						1u},
	{"lea",	{DIRECT,REGISTER},					{DIRECT},							2u},
	{"inc",	{DIRECT,REGISTER},					{NO_METHODS},						1u},
	{"dec",	{DIRECT,REGISTER},					{NO_METHODS},						1u},
	{"jmp",	{DIRECT,REGISTER},					{NO_METHODS},						1u},
	{"bne",	{DIRECT,REGISTER},					{NO_METHODS},						1u},
	{"red",	{DIRECT,REGISTER},					{NO_METHODS},						1u},
	{"prn",	{IMMEDIATE,DIRECT,DINAMIC,REGISTER},{NO_METHODS},						1u},
	{"jsr",	{DIRECT,REGISTER},					{NO_METHODS},						1u},
	{"rts",	{NO_METHODS},						{NO_METHODS},						0u},
	{"stop",{NO_METHODS},						{NO_METHODS},						0u},
	{"error",{NO_METHODS},						{NO_METHODS},						0u} };
		
	return cmds;
}

/* global-like variable for counting instructions
 * instruction_convert(0) returns current number of instructions
 * instruction_convert(-instruction_convert(0)) sets it to 0 */
int instruction_counter(int add) {
	static int counter = 0;
	counter += add;
	return counter;
}

char is_command(char *token) {
	struct command *cmds = get_commands();
	int i;
	
	for(i = 0; strcmp(cmds[i].name, "error") ; i++) /* goes over all commands until the last dummy command */
		if(strcmp(cmds[i].name, token) == 0)
			return YES;
			
	return NO;
}

/* root_instruction() and curr_instruction() are global-like variables:
 * root_instruction(NULL) or ROOT returns the root
 * root_instruction(new) (new != NULL) is root = new assigment
 * the same as above with curr_instruction()
 * both can be accessed throughout the entire data.c file */
 
instruction *root_instruction(instruction *new, int set) {
	static instruction *root = NULL;
	if(set) /* set == YES */
		root = new;
	return root;
}

instruction *curr_instruction(instruction *new, int set) {
	static instruction *curr = NULL;
	if(set)
		curr = new;
	return curr;
}

/* frees all the space list has taken */
void reset_instruction_list(void) {
	instruction *temp;
    
	while(ROOT != NULL && ROOT->next != NULL) { 
		temp = ROOT;
		SET_ROOT(ROOT->next);
		free(temp);
	}
	
	SET_CURR(NULL);
	SET_ROOT(NULL);
	instruction_counter(-instruction_counter(0)); /* resets counter */
}

char is_instruction_empty(void) {
	if(ROOT == NULL)
		return YES;
	return NO;
}

/* new_instruction clears space and returns a pointer */
instruction *new_instruction(void) {
	instruction *p;
	
	if((p = (instruction*)malloc(sizeof(instruction))) == NULL) {
		fprintf(stderr, "At new_instruction: Not enough space.\n");
		exit(1);
	}
		
	p->next = NULL;
	return p;
}

void add_instruction(unsigned int address, unsigned int whole_content) {
	instruction *new_instruction(void);
	
	if(is_instruction_empty()) { /* if it's the first instruction on list */
		if(SET_ROOT(new_instruction()) != NULL) {
			SET_INSTRUCTION_VALUES_OF(ROOT);
			SET_CURR(ROOT);
		}
	} else if((CURR->next = new_instruction()) != NULL) {
			SET_CURR(CURR->next);
			SET_INSTRUCTION_VALUES_OF(CURR);
		}
		
	instruction_counter(1);
}

char update_instruction(unsigned int address, unsigned int whole_content) {
	instruction *temp = ROOT;
	
	while(temp != NULL) {
		if(temp->address == address) {
			SET_INSTRUCTION_VALUES_OF(temp);
			return OK;
		}
		temp = temp->next;
	}
		
	fprintf(stderr, "Critical error at update_instruction: the instruction added at first "
						"is not found to be updated.\n");
	return ERROR;
}

unsigned int get_instruction_content(unsigned int address) {
	instruction *temp = ROOT;
	
	while(temp != NULL) {
		if(temp->address == address) {
			return temp->contents.whole;
		}
		temp = temp->next;
	}
		
	return NO;
}

int get_params(char **line, char params[MAX_PARAMS][MAX_PARAM_LEN], int line_counter) {
	void skip_white_space(char **line);
	int i,j, commas = 0;
	char comma_flag = NO;
	
	skip_white_space(line);
	
	for(i = 0; **line != '\0' && i < MAX_PARAMS; i++) {
		comma_flag = NO;
		
		/* copies the token */
		for(j = 0; **line != ' ' && **line != '\t' && **line != ',' && 
				  **line != '\n' && **line != '\0' && j < MAX_PARAM_LEN; j++)
			params[i][j] = *((*line)++);
		params[i][j] = '\0';
		
		skip_white_space(line);
		
		if(j == MAX_PARAM_LEN) { /* checks param length */
			fprintf(stderr, "Line %d: param %s is too long. more than %d characters.\n",
					line_counter, params[i], MAX_PARAM_LEN - 1);
			return ERROR;
		}
		
		if(**line == ',') { /* checks number of commas and their placement */
			(*line)++;
			commas++;
			comma_flag = YES;
		}
		
		skip_white_space(line);
		
		if(**line != '\0' && comma_flag == NO) {
			fprintf(stderr, "Line %d: missing comma after param %s.\n", line_counter, params[i]);
			return ERROR;
		}
	}
	
	if(commas != i - 1 && i != 0) {
		fprintf(stderr, "Line %d: invalid commas or too many parameters, only up to %d parameters allowed.\n",
						 line_counter, MAX_PARAMS);
		return ERROR;
	}
	
	return i;
}

char is_dinamic_addressing(char param[MAX_PARAM_LEN]) {
	int is_label(char **token);
	int search_type_label(char *name, int type);
	char temp[MAX_PARAM_LEN];
	char *p;
	
	if((p = (char*)malloc(sizeof(char) * (strlen(param) + 1))) == NULL) {
		fprintf(stderr, "At is_dinamic_addressing: Not enough space.\n");
		exit(1);
	}
	strcpy(temp, param);
	
	if((p = strtok(temp, "[")) == NULL)
		return NO;

	if(is_label(&p) == NO || search_type_label(p, EXTERNAL) == YES) /* you can't get content of external unknown address */
		return NO;

	if((p = strtok(NULL, "-")) == NULL)
		return NO;
		
	if(atoi(p) <= 0 || atoi(p) > (CONTENT_BITS-ERA_BITS)) /* not a valid number */
		return NO;
	
	if((p = strtok(NULL, "]")) == NULL)
		return NO;	
	
	if(atoi(p) <= 0 || atoi(p) > (CONTENT_BITS-ERA_BITS)) /* not a valid number */
		return NO;
		
	if((p = strtok(NULL, "")) != NULL && *p == '\0')
		return NO;
	
	return YES;
}

/* we assume there is no white space */
char get_addressing_method(char param[MAX_PARAM_LEN], unsigned int address, int line_counter) {
	void add_label(char *name, unsigned int address, unsigned char type);
	int get_label_type(char *name);
	int search_ext_label(char *name, unsigned int address);
	int is_label(char **token);
	
	if(*param == '\0')
		return NO_METHODS;
		
	if(*param == 'r' && (*(param + 1) >= '0' && *(param + 1) <= '7')) /* registers r1,r2, ... , r7 */
		return REGISTER;

	if(*param == '#') { /* might be IMMEDIATE addressing method */
		param++;
		if(strcmp("0", param) == 0 || atoi(param) != 0)
			return IMMEDIATE;
		else {
			fprintf(stderr, "Line %d: at immediate addressing, after # there must be an integer.\n",
							line_counter);
			return ERROR;
		}
	}
	
	if(is_label(&param) == YES) { /* puts appearance of external label in list for printing it later */
		if(get_label_type(param) == EXTERNAL && search_ext_label(param, address) == NO)
			add_label(param, address, EXTERNAL);
		return DIRECT;
	}
		
	if(is_dinamic_addressing(param) == YES) 
		return DINAMIC;
	
	fprintf(stderr, "Line %d: invalid addressing method.\n", line_counter);
	
	return ERROR;
}

char check_addressing_method(int i, int line_counter, unsigned int source, unsigned int target) {
	int j;
	struct command *cmds = get_commands();
	char flag = NO;
	
	for(j = 0; j < MAX_ADDRESSING_METHODS; j++) /* checks source addresing methods */
		if(source == cmds[i].source_method[j])
			flag = YES;
			
	if(flag == NO) {
		fprintf(stderr, "Line %d: invalid source addressing method for command %s.\n",
						 line_counter, cmds[i].name);
		return ERROR;
	}
	
	for(j = 0; j < MAX_ADDRESSING_METHODS; j++) /* checks target addresing methods */
		if(target == cmds[i].dest_method[j])
			flag = YES;
			
	if(flag == NO) {
		fprintf(stderr, "Line %d: invalid target addressing method for command %s.\n",
						 line_counter, cmds[i].name);
		return ERROR;
	}
	
	return OK;
}

unsigned int get_dinamic(char param[MAX_PARAM_LEN]) {
	unsigned int get_address(char *name);
	unsigned int get_data_content(unsigned int address);
	char temp[MAX_PARAM_LEN];
	char *p;
	unsigned int result = ~(0u), value = 0u;
	int start = 0, end = 0;
	
	if((p = (char*)malloc(sizeof(char) * (strlen(param) + 1))) == NULL) {
		fprintf(stderr, "At get_dinamic: Not enough space.\n");
		exit(1);
	}
	strcpy(temp, param);
	p = strtok(temp, "[");
	
	/* get the content needed to cut bits from */
	if(get_instruction_content(get_address(p)) != NO)
		value = get_instruction_content(get_address(p));
	else if(get_data_content(get_address(p)) != NO)
			value = get_data_content(get_address(p));
		 else
			fprintf(stderr, "Critical error at get_dinamic: address in label %s is not found in either"
							" data or instruction lists.\n", p);
	
	p = strtok(NULL, "-"); /* starting digit */
	start = atoi(p);
	value = value >> start;
	
	p = strtok(NULL, "]"); /* ending digit */
	end = atoi(p);
	result = result >> (end-start+1);
	result = result << (end-start+1);
	
	return (result | value);
}

char add_params(char *line, char *token, int line_counter, unsigned int *address) {
	instruction *temp = new_instruction();
	int search_type_label(char *name, int type);
	unsigned int get_address(char *name);
	char params[MAX_PARAMS][MAX_PARAM_LEN];
	signed int param_count;
	unsigned int source, target;
	
	line = (token + strlen(token) + 1); 
	if((param_count = get_params(&line, params, line_counter)) == ERROR)
		return ERROR;
		
	source = get_addressing_method(params[0], *address, line_counter);
	target = get_addressing_method(params[1], *address+1, line_counter);
		
	if(source == REGISTER && target == REGISTER) /* if both are registers, there will be used only one line */
		param_count--;
		
	/* sets the contents according to addressing method */
	switch(source) {
		case REGISTER:
			temp->contents.value_bits.era = ABSOLUTE;
			temp->contents.value_bits.content = (unsigned int)atoi((params[0]+1)) << ((CONTENT_BITS-ERA_BITS)/2); /* one after character 'r' */
			if(source == REGISTER && target == REGISTER)
				temp->contents.value_bits.content += (unsigned int)atoi((params[1]+1));
			break;
		case IMMEDIATE:
			temp->contents.value_bits.era = ABSOLUTE;
			temp->contents.value_bits.content = (unsigned int)atoi((params[0]+1)); /* one after character '#' */
			break;
		case DINAMIC:
			temp->contents.value_bits.era = ABSOLUTE;
			temp->contents.value_bits.content = get_dinamic(params[0]);
			break;
		case DIRECT:
			if(search_type_label(params[0], EXTERNAL) == YES) {
				temp->contents.value_bits.era = EXTERNAL;
				temp->contents.value_bits.content = 0u; /* external labels' address will be added with linker-loader */
			}
			else {
				temp->contents.value_bits.era = RELOCATABLE;
				temp->contents.value_bits.content = get_address(params[0]);
			}
			break;
		default:
			break;
	}	
	
	if(source != NO_METHODS && source != ERROR)
		update_instruction((*address)++, temp->contents.whole);
		
	/* sets the contents according to addressing method */
	if(param_count == 2) {
		switch(target) {
			case REGISTER:
			case IMMEDIATE:
				temp->contents.value_bits.era = ABSOLUTE;
				temp->contents.value_bits.content = (unsigned int)atoi((params[1]+1)); /* one after character 'r' or '#' */
				break;
			case DINAMIC:
				temp->contents.value_bits.era = ABSOLUTE;
				temp->contents.value_bits.content = get_dinamic(params[1]);
				break;
			case DIRECT:
				if(search_type_label(params[1], EXTERNAL) == YES) {
					temp->contents.value_bits.era = EXTERNAL;
					temp->contents.value_bits.content = 0u; /* external labels' address will be added with linker-loader */
				}
				else {
					temp->contents.value_bits.era = RELOCATABLE;
					temp->contents.value_bits.content = get_address(params[1]);
				}
				break;
			default:
				break;
		}	
		
		if(target != NO_METHODS && target != ERROR)
			update_instruction((*address)++, temp->contents.whole);
	}
	return OK;
}

char handle_instruction(char *line, char *token, int line_counter, unsigned int *address) {
	char *convert_address(unsigned int address);
	void skip_white_space(char **line);
	struct command *cmds = get_commands();
	unsigned int i, j, source, target, k = 0;
	signed int param_count;
	char params[MAX_PARAMS][MAX_PARAM_LEN];
	instruction *temp = new_instruction();
	
	line = (token + strlen(token) + 1); 
	if((param_count = get_params(&line, params, line_counter)) == ERROR)
		return ERROR;
	
	for(i = 0; strcmp(cmds[i].name, "error") != 0; i++)
		if(strcmp(cmds[i].name, token) == 0) { /* if this is the command */
			
			if(cmds[i].group != param_count) { /* checks number of parameters */
				fprintf(stderr, "Line %d: invalid amount of parameters, should be %d.\n",
								 line_counter, cmds[i].group);
				return ERROR;
			}
			if(param_count == 1) { /* if there is only one parameter, the second will be the source */
				strcpy(params[1], params[0]);
				params[0][0] = '\0';
			}
			if(param_count == 2) /* helps determine the address to keep target/source */
				k = 1;
				
			temp->address = (*address)++;
			temp->contents.command_bits.empty = EMPTY_CONTENT; /* the first 3 digits of any command are 101 */
			temp->contents.command_bits.group = cmds[i].group;
			temp->contents.command_bits.opcode = i;
			temp->contents.command_bits.source = source = get_addressing_method(params[0], *address, line_counter);
			temp->contents.command_bits.target = target = get_addressing_method(params[1], *address+k, line_counter);
			temp->contents.command_bits.era = ABSOLUTE; /* all commands' era is asbolute */
			
			if(check_addressing_method(i, line_counter, source, target) == ERROR)
				return ERROR;
			if(source == REGISTER && target == REGISTER) /* if both are registers, there will be used only one line */
				param_count--;
				
			add_instruction(temp->address, temp->contents.whole);
			for(j = 0; j < param_count; j++)
				add_instruction((*address)++, 0u); /* adds empty instructions */
			
			return OK;
		}
	
	fprintf(stderr, "Critical error at handle_instruction, shouldn't happen.\n"
					"Assembler should have found that token isn't a valid command or label before, at token_type.\n");
	return ERROR;
}

void print_instruction_to_file(FILE *ofp) {
	char *convert_address(unsigned int address);
	char *convert_instruction_content(unsigned int content);
	
	instruction *temp = ROOT;
	
	while(temp != NULL) {
		fprintf(ofp, "%s\t%s\n", convert_address(temp->address), convert_instruction_content(temp->contents.whole));
		temp = temp->next;
	}
}
