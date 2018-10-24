// C++
#include <iostream>
#include <fstream>
// C
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






	// variable for select()
	// Ref: Youtube channels (keyword: select toturial)
	int fd;
	char buf[11];
	int ret, sret;
	fd = 0;
	fd_set readfds;
	// variable for select()


	while(1){

		FD_ZERO(&readfds);
		FD_SET(fd,&readfds);
		sret = select(8,&readfds,NULL,NULL,NULL);
		if(sret==0){
			printf("sret = %d\n", sret);
		}
		memset((void *) buf, 0, 11);
		ret = read(fd, (void*)buf, 10);
		// string s; s.push_back(buf); 

		if(ret != -1){
			// cout << strcmp(buf,"list") << endl;
			if(strcmp(buf,"list")==10){


			}
			else if (strcmp(buf,"exit")==10){
				break;
			}
			else{
				cout << "unknown command! [list/exit]" << endl;
			}
		}
	}



}




void switches(char **arg, const string &input){
	
	int    port1;
	int    port2;
	string port3;
	int dest_ip_low;
	int dest_ip_high;

	// cout << arg[1] << endl; // sw
	// cout << arg[2] << endl; // file name
	// cout << arg[3] << endl; // port 1
	// cout << arg[4] << endl; // port 2
	// cout << arg[5] << endl; // port 3 100-110
	ofstream myfile;
	if(strcmp(arg[3],"null") == 0){ port1 = -1; }
	if(strcmp(arg[4],"null") == 0){ port2 = -1; }



	// variable for select()
	int fd;
	char buf[11];
	int ret, sret;
	fd = 0;
	fd_set readfds;
	// variable for select()

	while(1){

		FD_ZERO(&readfds);
		FD_SET(fd,&readfds);
		sret = select(8,&readfds,NULL,NULL,NULL);
		if(sret==0){
			printf("sret = %d\n", sret);
		}
		memset((void *) buf, 0, 11);
		ret = read(fd, (void*)buf, 10);
		if(ret != -1){

			printf("buf = %s\n", buf);
		}
	}



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
	string argv_1_rep = "";
	argv_1_rep = argv[1]; 
	char *argv_1 = RemoveDigits(argv[1]);
	// cout << argv_1 << endl;
	if (strcmp(argv_1,"cont") == 0){
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
	else if(strcmp(argv_1, "sw") == 0){
		if(argc != 6){
			cout << "Required 6 arguments (e.g. ./a2sdn sw1 t1.dat null null 100-110)!" << endl;
			return 0;
		}
		// try to open the dat.file
		
		switches(argv,argv_1_rep);


	}
	else{

		cout << "Invalid Command!" << endl;
		return 0;
	}

    // cout <<"this line" << endl;


  
    return 0; 
} 



































