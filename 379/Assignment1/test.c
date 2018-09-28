


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


#define MAXARGC     50  /* max number of arguments in buf */
#define WHITE   " \t\n" /* white space for tokenizing arguments */



int main(){




	// char a[] = "A,B,C,D,E";
	// char * separator = ",";
	// char * b = strtok(a, separator);
	// printf("b: %s\n", b);
	// char * c = strtok(NULL, ",");
	// printf("c: %s\n", c);
	// char * d = strtok(NULL, "");
	// printf("d: %s\n", d);

	/*
	 * buf[] contains white-space-separated arguments.  We convert it to an
	 * argv-style array of pointers, and call the user’s function (optfunc)
	 * to process the array.  We return -1 if there’s a problem parsing buf,
	 * else we return whatever optfunc() returns.  Note that user’s buf[]
	 * array is modified (nulls placed after each token).
	 */
	char    *ptr, *argv[MAXARGC];
	int     argc;
	char *buf = malloc(sizeof(char)*100);
	strcpy(buf,"run pgm i ii");
		                                    /* an argv[0] is required */
	while ((ptr = strtok(NULL, WHITE)) != NULL) 
	{
		if (strtok(buf, WHITE) == NULL){
		    return(-1);
			argv[argc = 0] = buf;
		}
		if (++argc >= MAXARGC-1){
			return(-1);
			argv[argc] = ptr;
		}
		argv[++argc] = NULL;

		printf("%s\n",*argv);
	}
	return 0;
}


