
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <string.h>

using namespace std; 
  
// macro definition 
#define MAX_SWITCH 7
#define PRI 4
#define S_LOW 0
#define S_HIGH 1000





void controller(){




}




void switches(){





}




// Ref: https://stackoverflow.com/questions/28353173/trying-to-remove-all-numbers-from-a-string-in-c
char* RemoveDigits(char* input)
{
    char* dest = input;
    char* src = input;

    while(*src)
    {
        if (isdigit(*src)) { src++; continue; }
        *dest++ = *src++;
    }
    *dest = '\0';
    return input;
}





int main(int argc, char** argv) 
{ 

	if (argc < 3){
		cout << "Too little arguments " << endl;
		return 0;
	}
	// cout << argv[1] << endl;
	char *argv_1 = RemoveDigits(argv[1]);
	// cout << argv_1 << endl;
	if (strcmp(argv_1,"cont ")){
		if (argc > 3){
			cout << "Too many inputs for controller" << endl;
			return 0;
		}
		// cout << argv[2] << endl;
		int n_swithes = atoi(argv[2]);
		if (n_swithes > MAX_SWITCH){
			cout << "Too many switches (MAX: 7)!" << endl;
			return 0;
		}
		
		controller();
	}

    // cout <<"this line" << endl;


  
    return 0; 
} 



































