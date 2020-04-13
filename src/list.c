#include "instruction.h"

/* returns token type */
int token_type(char *token) {
	struct command *get_commands(void);
	struct command *cmds = get_commands();
	int i;
	
	if(token == NULL)
		return NO_TOKEN;

	if(strcmp(".string", token) == 0 || strcmp(".data", token) == 0)
		return DATA;

	if(strcmp(".entry", token) == 0)
		return ENT;
	
	if(strcmp(".extern", token) == 0)
		return EXT;
		
	for(i = 0; strcmp(cmds[i].name, "error") ; i++) /* goes over all commands until the last dummy command */
		if(strcmp(cmds[i].name, token) == 0)
			return INST;

	return ERROR;
}

/* convretion needed for output files */
char *convert_content(unsigned int content) {
	static char *special = "!@#$%^&*"; /* the special 8 base */
	char *ptr, *temp; 
	unsigned int i, k;
	
	if((ptr = (char*)malloc(CONTENT_CHARS)) == NULL)
		return NULL;
	if((temp = (char*)malloc(CONTENT_CHARS)) == NULL)
		return NULL;
	
	*ptr = '!'; /* deals with case of content = 0 */
	
	/* converting every 3 digits of 0 or 1 to a special 8 base */
	for(i = 0; i < CONTENT_CHARS; i++, content = content / 8)
		*(ptr + i) = *(special + (content % 8));
		
	*(ptr + CONTENT_CHARS - 1) = '\0';
	
	/* reverses the string */
	for(i = 0,k = strlen(ptr)-1 ; i < k; i++,k--) { 
        *temp = *(ptr+k);
        *(ptr+k) = *(ptr+i);
        *(ptr+i) = *temp;
	}
	
	return ptr;
}

char *convert_instruction_content(unsigned int content) {
	static char *special = "!@#$%^&*"; /* the special 8 base */
	char *ptr, *temp; 
	unsigned int i, k;
	
	/*printf("content: %s\n", get_bits(content, CONTENT_BITS));*/
	
	if((ptr = (char*)malloc(CONTENT_CHARS)) == NULL)
		return NULL;
		if((temp = (char*)malloc(CONTENT_CHARS)) == NULL)
		return NULL;
	
	*ptr = '!'; /* deals with case of content = 0 */
	
	/* converting every 3 digits of 0 or 1 to a special 8 base */
	for(i = 0; i < CONTENT_CHARS; i++, content = content / 8)
		*(ptr + i) = *(special + (content % 8));
		
	*(ptr + CONTENT_CHARS - 1) = '\0';
	
	/* reverses the string */
	for(i = 0,k = strlen(ptr)-1 ; i < k; i++,k--) { 
        *temp = *(ptr+k);
        *(ptr+k) = *(ptr+i);
        *(ptr+i) = *temp;
	}
	
	/*printf("ptr: %s\n\n", ptr);*/
	
	return ptr;
}

/* converts address to special 8 base: we suppose address is described with 9 bits */
char *convert_address(unsigned int address) {
	static char *special = "!@#$%^&*"; /* the special 8 base */
	char *ptr, *temp; 
	unsigned int i, k;
	
	if((ptr = (char*)malloc(ADDRESS_CHARS)) == NULL)
		return NULL;
	if((temp = (char*)malloc(ADDRESS_CHARS)) == NULL)
		return NULL;
	
	*ptr = '!'; /* deals with case of address = 0 */
	
	/* converting every 3 digits of 0 or 1 to a special 8 base */
	for(i = 0; address != 0; i++, address = address / 8)
		*(ptr + i) = *(special + (address % 8));
		
	*(ptr + ADDRESS_CHARS - 1) = '\0';
	
	/* reverses the string */
	for(i = 0,k = strlen(ptr)-1 ; i < k; i++,k--) { 
        *temp = *(ptr+k);
        *(ptr+k) = *(ptr+i);
        *(ptr+i) = *temp;
	}
	
	return ptr;
}

void skip_white_space(char **line) {
	while(**line == ' ' || **line == '\t' || **line == '\n')
		(*line)++;
}

char *get_components(unsigned int content) {
	static char *special = "01"; /* the special 8 base */
	char *ptr, *temp; 
	unsigned int i, k = 0;
	
	if((ptr = (char*)malloc(CONTENT_CHARS + 2)) == NULL)
		return NULL;
	if((temp = (char*)malloc(CONTENT_CHARS + 2)) == NULL)
		return NULL;
	
	*ptr = '0'; /* deals with case of address = 0 */
	
	/* converting every 3 digits of 0 or 1 to a special 8 base */
	for(i = 0; i < CONTENT_BITS; i++, content = content / 2, k++)
		*(ptr + k) = *(special + (content % 2));
		
	*(ptr+k) = '-';
	k++;
	for(i = 0; i < ERA_BITS; i++, content = content / 2, k++)
		*(ptr + k) = *(special + (content % 2));
		
	*(ptr + CONTENT_BITS + 5) = '\0';
	
	return ptr;
}
