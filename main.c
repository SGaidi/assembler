#include "main.h"
#include "data.h"

/* tries compiling each file given by user (if given any) */
int main(int argc, char *argv[]) {
	void compile(char *filename);
	int i;
	char res = OK;
	
	if(argc <= 1) {
		fprintf(stderr, "No files.\n");
		res = ERROR;
	} else 
		for(i = 1; i < argc; i++)
			compile(argv[i]);
	
	return res;
}

/* tries opening file with this extension and mode */
FILE *open_file(char *filename, char *extension, char *mode) {
	FILE *fp = NULL;
	char temp[MAX_FILENAME_LEN];
	
	if(strlen(filename) + strlen(extension) + 1 > MAX_FILENAME_LEN) { /* checks filename's length */
		fprintf(stderr, "File %s name is longer than allowed: %d characters.\n",
						filename, MAX_FILENAME_LEN - strlen(extension) - 1);
		return NULL;
	}
	
	/* converts file name to filename.extension format */
	strcpy(temp, filename);
	strcat(temp, ".");
	strcat(temp, extension);
			
	if((fp = fopen(temp, mode)) == NULL) /* tries opening it */
		fprintf(stderr, "Can't open %s file.\n", temp);
	
	return fp;
}

/* tries creating object file (if needed) and writes content */
void write_object_file(char *filename) {
	char *convert_address(int address); /* temporary */
	char is_instruction_empty(void);
	int instruction_counter(int add);
	char is_data_empty(void);
	int data_counter(int add);
	void print_data_to_file(FILE *ofp);
	void print_instruction_to_file(FILE *ofp);
	FILE *ofp;
	
	if(is_instruction_empty() && is_data_empty()) { 
		fprintf(stderr, "No instructions or data entries in %s.as file.\n", filename);
		return;
	}
	
	if((ofp = open_file(filename, "ob", "w")) != NULL) {
				
		/* prints number of instructions, datas and then lists them */
		fprintf(ofp,"%s\t%s\n", convert_address(instruction_counter(0)), convert_address(data_counter(0)));
		print_instruction_to_file(ofp);
		print_data_to_file(ofp);
		fclose(ofp);
	}
}

void write_entry_file(char *filename) {
	void print_labels_to_file(FILE *ofp, int type);
	char is_label_type_empty(int type);
	FILE *ofp;
	
	if(is_label_type_empty(ENTRY) == NO) /* if there are entries */
		if((ofp = open_file(filename, "ent", "w")) != NULL) {
		print_labels_to_file(ofp, ENTRY);	
		fclose(ofp);
	}
}

void write_external_file(char *filename) {
	void print_labels_to_file(FILE *ofp, int type);
	char is_label_type_empty(int type);
	FILE *ofp;
	
	if(is_label_type_empty(EXTERNAL) == NO) /* if there are externals */
		if((ofp = open_file(filename, "ext", "w")) != NULL) {
		print_labels_to_file(ofp, EXTERNAL);	
		fclose(ofp);
	}
}

/* goes overs this file twice, collects all data and instructions */
void compile(char *filename) {
	signed char first(FILE *ifp);
	signed char second(FILE *ifp);
	FILE *ifp;
	
	/* opens file */
	if((ifp = open_file(filename, "as", "r")) == NULL)
		return;
		
	printf("FILE: %s.as\n"
		   "*************************\n", filename);
	
	/* compiles, if there were errors, no files will be created */
	if(first(ifp) == ERROR) return;
	if(second(ifp) == ERROR) return;
	printf("No compilation errors. Files will be created.\n");
	
	/* write all neccesary files */
	write_object_file(filename);
	write_entry_file(filename);
	write_external_file(filename);
	fclose(ifp);
}
