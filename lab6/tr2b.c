#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	char *first = argv[1];
	char *second = argv[2];
	//printf("The first string is : %s", argv[1]);
	int i, j, counter, flag;
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

	int curr = getchar();
	while(curr != EOF){     // as long as the file doesn't end
		flag = 0;
		for(i = 0; i < counter; i++){
			if(curr == first[i]){    // if any char in the file matches that of first arg.
				putchar(second[i]);    // output the equivalent char of the second arg.
				flag = 1;            // flag to no longer output the orig. char from the file
			}
		}
		if(flag == 0){
			putchar(curr); 
		}
		curr = getchar();
	}
}