#include "instruction.h"

/* list.c includes some helpful functions for manipulating and converting information
 * needed throughout the project */

/* returns token type */
int token_type(char *token) {
	char is_command(char *token);
	
	if(token == NULL)
		return NO_TOKEN;

	if(strcmp(".string", token) == 0 || strcmp(".data", token) == 0)
		return DATA;

	if(strcmp(".entry", token) == 0)
		return ENT;
	
	if(strcmp(".extern", token) == 0)
		return EXT;
		
	if(is_command(token) == YES)
		return INST;

	return ERROR;
}

/* converion needed for output files */
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

/* converion needed for instrcution content */
char *convert_instruction_content(unsigned int content) {
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
