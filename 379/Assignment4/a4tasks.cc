// example command:
// ./a4tasks inputFile monitorTime NITER

// Other reference:
// https://linux.die.net/man/3/poll

// C++
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
// C
#include <sys/socket.h> 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <poll.h>
#include <time.h>
#include <pthread.h> 


using namespace std; 
  
// macro definition 
#define MAX_NRES_TYPES = 10
#define MAX_NTASKS     = 25

//function declartion
char* RemoveDigits(char* input);
int split(char inStr[],  char token[][MAXWORD], char fs[]);
void set_cpu_time();
string convert_int_to_string(int input);
void error(const char *msg);
void RemoveSpaces(char* source);
void simulator(int argc, char** argv, int time_start_program);

//global variable declartion
pthread_t ntid;





    // tid is the thread’s ID (of type pthread_t)
    // pthread_t thread_id; 
    // printf("Before Thread\n"); 
    // pthread_create(&thread_id, NULL, myThreadFun, NULL); 
    // pthread_join(thread_id, NULL); 
void monitor_thread(int monitorTime){

}




void task_thread(int argc, char** argu){
    pid_t       pid;
    pthread_t   tid;
    int tid_int;
    pid = getpid();
    tid = pthread_self();           // return thread_id
    tid_int = (unsigned long)tid;
}

void simulator(int argc, char** argv,int time_start_program){
    // open file
    ifstream inputFile;
	if(inputFile.open(argv[1])<0){ error("Failed to open file. ")};

    int monitorTime = atoi(argv[2]);
    int NITER = atoi(argv[3]); // n iteration

    string STRING;
	if (!myfile.eof() || myfile.eof()){
		getline(myfile,STRING); 
        

        if (STRING.substr(0,1).compare("#")!=0 && STRING.substr(0,1).compare("")!=0 ){
            // if the line is task line, create the task thread
            // each task thread will execute for NITER iterations?
            // if the line is resource, then create what?

            int err;

        }
    }





}













// Ref: https://stackoverflow.com/questions/1726302/removing-spaces-from-a-string-in-c
void RemoveSpaces(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;
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

// how to use split:
// char * tab2 = new char [a.length()+1];
// 	strcpy (tab2, a.c_str());
// 	char splited_str[MAXLINE][MAXWORD];
// 	split(tab2,splited_str,delimiter);

// 	// cout << splited_str[0] << endl;
// 	// cout << splited_str[1] << endl;
// 	// cout << splited_str[2] << endl;
// 	// cout << splited_str[3] << endl;
// 	// cout << splited_str[4] << endl;
int split(char inStr[],  char token[][MAXWORD], char fs[])
{
    int    i, count;
    char   *tokenp, inStrCopy[MAXLINE];
	// cout << "print from split" << inStr << endl; // this prints nothing
    count= 0;
    memset (inStrCopy, 0, sizeof(inStrCopy));

    for (i=0; i < MAX_NTOKEN; i++) memset (token[i], 0, sizeof(token[i]));

    strcpy (inStrCopy, inStr);
    if ( (tokenp= strtok(inStr, fs)) == NULL) return(0);

    strcpy(token[count],tokenp); count++;

    while ( (tokenp= strtok(NULL, fs)) != NULL) {
        strcpy(token[count],tokenp); count++;
    }
    strcpy (inStr, inStrCopy);
    return(count);
}

string convert_int_to_string(int input){
	stringstream ss;
    ss << input;
    string s=ss.str();
	return s;
}

void set_cpu_time(){
	struct rlimit limit;
	getrlimit(RLIMIT_CPU,&limit);
	limit.rlim_cur = 60*100;
	setrlimit(RLIMIT_CPU,&limit);
}

// ref: http://www.linuxhowtos.org/C_C++/socket.htm
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char** argv) 
{ 
	//
    time_t   program_start;
    program_start = time(NULL);
	int time_start_program = (long)program_start;

	if (argc < 4){ cout << "Too little arguments " << endl; return 0;}
	if (argc > 4){ cout << "Too many arguments "   << endl; return 0;}
    set_cpu_time();
    simulator(argc, argv,time_start_program);
  
    return 0; 
} 
