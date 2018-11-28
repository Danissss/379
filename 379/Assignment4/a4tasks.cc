// example command:
// ./a4tasks inputFile 100 20

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
#define MAXLINE     132
#define MAX_NTOKEN  MAXLINE
#define MAXWORD     32



//function declartion
char* RemoveDigits(char* input);
int split(char inStr[],  char token[][MAXWORD], char fs[]);
void set_cpu_time();
string convert_int_to_string(int input);
void error(const char *msg);
void RemoveSpaces(char* source);
void simulator(int argc, char** argv, int time_start_program);
void split_name_value(char *string, int *value);
void *monitor_thread(void * arg);  //thread function
void *task_thread(void *argu);     //thread function 
// define struct
typedef struct { 
    char resource_type[10][32]; 
    int resource_unit[10]; 
} resource;

struct resources2{ 
    char * resource_type;
    int resource_unit;
    struct resources2 *next;
};

// monitor the task
// for each task; task_name = tasks.task_name[i] if tasks.status[i] is run; append to run list ...; 
// print monitor: wait list, run list, idle list
typedef struct tasks {
    char task_name[25][32];
    char status[25][10];    // WAIT; RUN; IDLE
} tasks;

struct task_args {
    int num_jobs;
    int iter;
    char *task_name;
    int busyTime;
    int idleTime;
    char jobs[10][32];

};

// struct args {
//     char* name;
//     int age;
// };
// void *hello(void *input) {
//     printf("name: %s\n", ((struct args*)input)->name);
//     printf("age: %d\n", ((struct args*)input)->age);
// }
// int main() {
//     struct args *Allen = (struct args *)malloc(sizeof(struct args));
//     char allen[] = "Allen";
//     Allen->name = allen;
//     Allen->age = 20;

//     pthread_t tid;
//     pthread_create(&tid, NULL, hello, (void *)Allen);
//     pthread_join(tid, NULL);
//     return 0;
// }


//global variable declartion
pthread_t ntid;
time_t   program_start;
int time_start_program;
resource g_resource;
tasks task_list;





    // tid is the thread’s ID (of type pthread_t)
    // pthread_t thread_id; 
    // printf("Before Thread\n"); 
    // pthread_create(&thread_id, NULL, myThreadFun, NULL); 
    // pthread_join(thread_id, NULL); 
void *monitor_thread(void *arg){
    int monitorTime = *(int*)arg;
    pid_t       pid;
    pthread_t   tid;
    int tid_int;
    pid = getpid();
    tid = pthread_self();           // return thread_id
    tid_int = (unsigned long)tid;
    //most 25 task
    while(1){
        char wait_list[25][32];
        char run_list[25][32];
        char idle_list[25][32];
        // task_list.task_name
        // task_list.status
        int wait_inds = 0;
        int run_inds  = 0;
        int idle_inds = 0;
        for (int i = 0; i<25; i++){
            if (task_list.status[i] != NULL){
                if (strcmp(task_list.status[i],"WAIT")==0){
                    strcpy(wait_list[wait_inds],task_list.task_name[i]);
                    wait_inds++;
                }
                else if (strcmp(task_list.status[i],"RUN")==0){
                    strcpy(run_list[run_inds],task_list.task_name[i]);
                    run_inds++;
                }
                else if (strcmp(task_list.status[i],"IDLE")==0){
                    strcpy(idle_list[idle_inds],task_list.task_name[i]);
                    idle_inds++;
                }
            }
        }
        // print the current status:
        char *wait_string = new char[100];
        strcpy(wait_string,"[WAIT] ");
        char *run_string = new char[100];
        strcpy(run_string,"[RUN] ");
        char *idle_string = new char[100];
        strcpy(idle_string,"[IDLE] ");

        for (int i = 0; i<wait_inds; i++){
            strcat(wait_string,wait_list[wait_inds]);
            strcat(wait_string," ");
        }
        for (int i = 0; i<run_inds; i++){
            strcat(run_string,run_list[run_inds]);
            strcat(run_string," ");
        }
        for (int i = 0; i<idle_inds; i++){
            strcat(idle_string,idle_list[idle_inds]);
            strcat(idle_string," ");
        }
        cout << "monitor:" << wait_string << endl;
        cout << "        " << run_string  << endl;
        cout << "        " << idle_string  << endl;
        cout << "..."                      << endl;

        // The sleep command suspends execution for a minimum of seconds.
        sleep(monitorTime/1000);
    }
    return NULL;
}




void *task_thread(void *argu){
    pid_t       pid;
    pthread_t   tid;

    // time
    time_t   current_time;
    current_time = time(NULL);
	int current_time_int = (long)current_time;

    int tid_int;
    pid = getpid();
    tid = pthread_self();           // return thread_id
    tid_int = (unsigned long)tid;
    struct task_args *coming_task = (task_args*) argu; // this only for g++
    
    int busyTime = coming_task->busyTime;
    int idleTime = coming_task->idleTime;

    cout << busyTime << "and idleTime: " << idleTime << endl;
    int time_gap = (time_start_program - current_time_int) * 1000;
    cout << "task " << coming_task->task_name << " (tid= " << tid_int << ", iter= " << coming_task->iter << ", time= " << time_gap << "msec)" << endl;
    
    pthread_exit(NULL);
    return NULL;
}

void simulator(int argc, char** argv,int time_start_program){
    // open file
    ifstream inputFile;
	inputFile.open(argv[1]);

    int monitorTime = atoi(argv[2]);
    int NITER = atoi(argv[3]); // n iteration

    string STRING;
    memset( (char *) &task_list, 0, sizeof(task_list) );

    // start monitor thread
    int err;
    int *monitorTime_p = new int[5];
    *monitorTime_p = monitorTime;
    err = pthread_create(&ntid, NULL, monitor_thread,  monitorTime_p);
    if (err != 0) { cout << "Can't create the monitor thread." << endl; exit(0); }
    else          { cout << "Moniter thread created!" << endl; }
    // pthread_join(ntid, NULL);
    
	while (!inputFile.eof()){
		getline(inputFile,STRING); 
        // cout << STRING << endl;
        // split the string 
        
        

        
        if (STRING.substr(0,1).compare("#")!=0 && STRING.substr(0,1).compare("")!=0 ){
            // if the line is task line, create the task thread
            // each task thread will execute for NITER iterations?
            // if the line is resource, then create what?
            char strings[50];
	        char delimiter[1];
            int num_words;
	        strcpy(delimiter," ");
	        char * tab = new char [STRING.length()+1];
	        strcpy (tab, STRING.c_str());
	        char splited_str[MAXLINE][MAXWORD];
	        num_words = split(tab,splited_str,delimiter);
            // cout<< num_words << endl;
            if(strcmp(splited_str[0],"resources") == 0){
                memset( (char *) &g_resource, 0, sizeof(g_resource) );
                // cout << STRING << endl;
                // create the resources array:
                for(int i=1; i < num_words; i++){
                    int value;
                    char *name_type = new char[32];
                    char delimiter_resource[1];
                    strcpy(delimiter_resource,":");
                    char splited_resource[MAXLINE][MAXWORD];
                    split(splited_str[i],splited_resource,delimiter_resource);
                    
                    // value = atoi(splited_resource[1]);
                    // strcpy(name_type, splited_resource[0]);
                    strcpy(g_resource.resource_type[i-1],splited_resource[0]);
                    g_resource.resource_unit[i-1] = atoi(splited_resource[1]);
                    // cout << g_resource.resource_type[i-1] << endl;
                    // cout << g_resource.resource_unit[i-1] << endl;
                }
                // for(int i = 0; i<10; i++){
                //     cout << "g_resource.resource_type: " << g_resource.resource_type[i] << endl;
                //     cout << "g_resource.resource_unit: " << g_resource.resource_unit[i] << endl;
                // }
                // exit(0);
            }
            
            if (strcmp(splited_str[0],"task") == 0){
                // cout << STRING << endl;
                struct task_args new_task;
                new_task.busyTime = atoi(splited_str[2]);
                new_task.idleTime = atoi(splited_str[3]);;
                // cout << "busyTime " << new_task.busyTime << endl;
                // cout << "idleTime " << new_task.idleTime << endl;
                // cout << "task_name " << splited_str[1]<< endl;
                new_task.task_name = new char[32];
                strcpy(new_task.task_name,splited_str[1]);
                // cout << "busyTime " << busyTime << endl;
                // cout << "idleTime " << idleTime << endl;
                // cout << "task_name: " << new_task.task_name<< endl;
                int task_args_inds = 0;
                for (int i=4; i < num_words; i++){
                    // cout << splited_str[i] << endl;
                    int value;
                    strcpy(new_task.jobs[task_args_inds],splited_str[i]);
                    task_args_inds++;
                }
                new_task.num_jobs = task_args_inds;
                // for (int i = 0; i <task_args_inds; i++){
                //     if (new_task.jobs[i] != NULL){
                //         cout << new_task.jobs[i] << endl;
                //     }
                //     cout << "new_task.jobs[i]: " <<new_task.jobs[i] << endl;
                    
                // }
                // exit(0);
                // create the NITER thread here
                // for 0 -> NITER
                //    create thread
                //    join thread
                // ref: https://stackoverflow.com/questions/16230542/passing-multiple-arguments-to-threaded-function-from-pthread-create
                // exit(0);
                pthread_t task_tid;
                int errs;
                for(int iter = 0; iter < NITER; iter++){
                    new_task.iter = iter;
                    errs = pthread_create(&task_tid, NULL, task_thread, &new_task);
                    if (errs < 0) { cout << "Thread creation failed. Exiting..." << endl; exit(0); }
                    else          { pthread_join(task_tid,NULL); }
                    exit(0);
                }

                
            }

            // start a monitor thread? 
        }
    }
    pthread_join(ntid, NULL);






}







void split_name_value(char *string, int *value){
    char delimiter_resource[1];
    char splited_resource[MAXLINE][MAXWORD];
    char * name = new char[32];
    strcpy(delimiter_resource,":");
    split(string,splited_resource,delimiter_resource);
    *value = atoi(splited_resource[1]);
    // strcpy(name,"h");
    // return name;

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
    
    program_start = time(NULL);
	time_start_program = (long)program_start;

	if (argc < 4){ cout << "Too little arguments " << endl; return 0;}
	if (argc > 4){ cout << "Too many arguments "   << endl; return 0;}
    set_cpu_time();
    simulator(argc, argv,time_start_program);
  
    return 0; 
} 
