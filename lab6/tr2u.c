#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	char *first = argv[1];
	char *second = argv[2];
	//printf("The first string is : %s", argv[1]);
	int i, j, counter, offset, flag;
	for(i = 0; first[i] != '\0'; i++){           // checking for duplicates
		for(j = i+1; first[j] != '\0'; j++){
			if(first[i]==first[j]){
				fprintf(stderr, "Duplicates error");
				exit(1);
			}
		}
	}

	i = 0;
	j = 0;

	while(first[i] != '\0' || second[j] != '\0'){     // checking for diff lengths
		if(first[i] == '\0' || second[j] == '\0'){    // for the two args.
			fprintf(stderr, "Different lengths error");
			exit(1);
		}
		i++;
		j++;
	}

	counter = i;      // get the length of arg. to transliterate
	offset = 1;
	int end;
	char *curr = malloc(sizeof(char));
	end = read(0, curr, 1);
	while(end != 0){     // as long as the file doesn't end
		flag = 0;
		for(i = 0; i < counter; i++){
			if(*curr == first[i]){    // if any char in the file matches that of first arg.
				write(1, &second[i], 1);
				flag = 1;            // flag to no longer output the orig. char from the file
			}
		}
			
		if(flag == 0){
			write(1, curr, 1);
			curr = NULL;
			curr = realloc(curr, sizeof(char)); 
		}
		end = read(0, curr, 1);
	}
}