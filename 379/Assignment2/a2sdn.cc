// C++
#include <iostream>
#include <fstream>
#include <string>
// C
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

using namespace std; 
  
// macro definition 
#define MAX_SWITCH 7
#define PRI 4
#define S_LOW 0
#define S_HIGH 1000
#define MAXWORD 10

typedef enum {ADD, QUERY, OPEN, ACK } KIND;	  // Message kinds
char KINDNAME[][MAXWORD]= { "ADD", "QUERY", "OPEN", "ACK" };
typedef struct {int port1; int port2; char port3[10]; char switch_no[5]; } MSG;

typedef struct { KIND kind; MSG msg; } FRAME;



// fifo declare
int fifo_0_1, fifo_1_1;




//function declare
char* RemoveDigits(char* input);
void controller();
void switches(char **arg, const string &input);
MSG composeMSTR (const string &a,  int port1,  int port2, char *port3);
void sendFrame (int fd, KIND kind, MSG *msg);
FRAME rcvFrame (int fd);










void controller(){






	// variable for select()
	// Ref: Youtube channels (keyword: select toturial)
	int fd;
	char buf[11];
	int ret, sret;
	fd = 0;
	fd_set readfds;
	// variable for select()




	

	MSG    msg;
	FRAME  frame;

	while(1){
		

		// reciving fifo file
		// controller always recive fifo_0_1 file 
		
		
		frame = rcvFrame(fifo_0_1);
		cout << "frame_port:" << frame.msg.ports.port1 << endl;
		sendFrame(fifo_1_1, ACK, &msg);     
		
		

		/////////////////////////////////////
		FD_ZERO(&readfds);
		FD_SET(fd,&readfds);
		select(8,&readfds,NULL,NULL,NULL);
		memset((void *) buf, 0, 11);
		ret = read(fd, (void*)buf, 10);
		/////////////////////////////////////

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
	char  *port3;
	port3= new char[10]; // allocate memory for pointer *port3;
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


	// composePacket and send
	port1 = 1;
	port2 = 2;
	strcpy(port3,"100-110");

	

	FRAME frame;
	MSG send_msg;

	// send_msg = composeMSTR(input,port1,port2,port3); // seg fault 
	// sendFrame(fifo_0_1,OPEN,&send_msg);

	// if (input.compare("sw1")==0){ 
	// 	frame = rcvFrame(fifo_1_1); 
	// 	cout << frame.msg.ports.port1 << endl;
	// }
	// reciving frame
	send_msg = composeMSTR(input,port1,port2,port3); // seg fault 
	// cout<< " port1: " <<send_msg.ports.port1 << endl;
	sendFrame(fifo_0_1,OPEN,&send_msg);


	while(1){


	
		if (input.compare("sw1")==0){ 
			frame = rcvFrame(fifo_1_1); 
			// cout << frame.msg.ports.port1 << endl;
		}




		/////////////////////////////////////
		FD_ZERO(&readfds);
		FD_SET(fd,&readfds);
		select(8,&readfds,NULL,NULL,NULL);
		memset((void *) buf, 0, 11);
		ret = read(fd, (void*)buf, 10);
		/////////////////////////////////////
		if(ret != -1){

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

// Ref: eclass

MSG composeMSTR (const string &a,  int port1,  int port2,  char *port3)
{
    MSG  msg;

    memset( (char *) &msg, 0, sizeof(msg) );
    
    msg.ports.port1 = port1;
    msg.ports.port2 = port2;
    // cout << port1 << endl;
    // cout << msg.ports.port1 << endl; // at this point; the port1 get the correct value;
    strcpy(msg.ports.port3,port3);
    // cout << msg.ports.port1 << endl;
    strncpy(msg.switch_no,a.c_str(),sizeof(msg.switch_no)); // port1 value changed at this point;
    // cout << msg.ports.port1 << endl;
    return msg;
} 

void sendFrame (int fd, KIND kind, MSG *msg)
{
    FRAME  frame;
    memset( (char *) &frame, 0, sizeof(frame) );
    frame.kind= kind;
    frame.msg=  *msg;
    write (fd, (char *) &frame, sizeof(frame));
}
       
FRAME rcvFrame (int fd)
{ 
    FRAME  frame;

    memset( (char *) &frame, 0, sizeof(frame) );
    read (fd, (char *) &frame, sizeof(frame));
    return frame;		  
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


	//open fifo
	if ( (fifo_0_1= open("fifo_0_1", O_RDWR)) < 0){
        cout << "NO fifo_0_1 EXITS" << endl;
        return 0;
	}


    if ( (fifo_1_1 = open("fifo_1_1", O_RDWR)) < 0){
        cout << "NO fifo_1_1 EXITS" << endl;
        return 0;
    }

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



































