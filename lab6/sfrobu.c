#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int frobcmp (const char *a, const char *b);
int decode_and_cmp (const char *a, const char *b);
int wrapper (const void *a, const void *b);
void check_mem_error(char *ptr);

static int num_comparisons = 0;

//returns an int result that is negative, zero, or positive depending 
//on whether a is less than, equal to, or greater than b.
int frobcmp (const char *a, const char *b){
	num_comparisons++;
	int result;	
	if(*a == ' ' && *b == ' ')
		return 0;
	while(*a != ' ' || *b != ' '){	
		if(*a == ' '){
			return -1;
		}
		else if(*b == ' '){
			return 1;
		}
		result = decode_and_cmp(a, b);
		if(result != 0){
			return result;
		}
		a++;
		b++;
	}
	return 0;
}

int decode_and_cmp (const char *a, const char *b){
	char first = *a^42;
	//printf("%s, %c", "\n", first);
	char second = *b^42;
	//printf("%s, %c", "\n", second);
	if(first < second){
		return -1;
	}
	else if (first == second){
		return 0;
	}
	return 1;
}

int wrapper (const void *a, const void *b){
	return frobcmp(*(const char **)a, *(const char **) b);
}

void check_read_error(int result){
	if(result < 0)
	{
		fprintf(stderr, "I/O Error");
		exit(1);
	}
}

void check_mem_error(char *ptr){
	if(ptr == NULL){
		free(ptr);
		fprintf(stderr, "Memory Allocation Error");
		exit(1);
	}
}

int main(int argc, char *argv[]){
	char * list;  // long string that gets all of stdin
	char ** word_separator = malloc(sizeof(char*));
	// array of pointers that mark where new words begin
	int separator_counter = 0;
	int list_counter = 0;    // respective counters
	int i, end;    
	char padding = ' '; 	// to append space at end
	struct stat fileStat;
	// purpose of fileStat is to find out the size
	if(fstat(0, &fileStat) < 0){   
		printf("Error running fstat");
		exit(1);
	}
	// if regular file
	if(S_ISREG(fileStat.st_mode)) 
	{ 	// allocate entire size of file
		list = malloc((fileStat.st_size+1) * sizeof(char)); 
		check_mem_error(list);

		char *curr = malloc(sizeof(char));     // char used to read in bytes
		check_mem_error(curr);
		char temp;

		end = read(0, curr, 1);
		check_read_error(end);  
		temp = *curr;         	// temp trails curr with each iteration

		while(end != 0){		// as long as the file hasn't ended
			if(temp == ' ' && *curr == ' '){     // skipping over white space
				end = read(0, curr, 1);
				check_read_error(end);
				continue;
			}
			list[list_counter] = *curr;    // place characters into the list
			list_counter++;
		
			temp = *curr;
			end = read(0, curr, 1);
			check_read_error(end);         // check if there is an error
		}

    	// appending a space if there is no space at the end
		if(temp != ' ' && list_counter > 0){  
			list[list_counter] = ' ';
			list_counter++;
		}
	}
    
    else{
    	list = malloc(sizeof(char));    // will grow with every additional character
		check_mem_error(list);

		char *curr = malloc(sizeof(char));     // char used to read in bytes
		check_mem_error(curr);
		char temp;

		end = read(0, curr, 1);
		check_read_error(end);  
		temp = *curr;         	// temp trails curr with each iteration

		while(end != 0){		// as long as the file hasn't ended
			if(temp == ' ' && *curr == ' '){     // skipping over white space
				end = read(0, curr, 1);
				check_read_error(end);
				continue;
			}
			list[list_counter] = *curr;    // place characters into the list
			list_counter++;
			list = realloc(list, (list_counter)*sizeof(char*));
		
			temp = *curr;
			end = read(0, curr, 1);
			check_read_error(end);         // check if there is an error
		}

    	// appending a space if there is no space at the end
		if(temp != ' ' && list_counter > 0){  
			list[list_counter] = ' ';
			list_counter++;
		}  
    }

    if(list_counter > 0){   // only if there are non-space characters
		word_separator[separator_counter] = &list[0];     // add a pointer to the beginning of array
		separator_counter++;                   
		word_separator = realloc(word_separator, (separator_counter)*sizeof(char*));
	}
	for(i = 0; i < list_counter-1; i++){	
		if(list[i]==' '){
			word_separator[separator_counter] = &(list[i+1]);     
			separator_counter++;
			word_separator = realloc(word_separator, (separator_counter+1)*sizeof(char*));
		}
	}

	qsort(word_separator, separator_counter, sizeof(char*), wrapper);   // sort

	for(i = 0; i < separator_counter; i++){
		list_counter = 0;	// output all the chars
		while(word_separator[i][list_counter] != ' '){
			write(1, &word_separator[i][list_counter], 1);
			list_counter++;
		}
		write(1, &padding, 1);
	}

	free(word_separator);
	free(list);
	fprintf(stderr, "\nComparisons: %d\n", num_comparisons);
}