// example command:
// with one switches
// ./a2sdn cont 1 8080
// ./a2sdn sw1 t2.dat null null 100-110 127.0.0.1 8080


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

using namespace std; 
  
// macro definition 
#define MAX_SWITCH 7
#define PRI 4
#define S_LOW 0
#define S_HIGH 1000

#define MAXLINE     132
#define MAX_NTOKEN  MAXLINE
#define MAXWORD     32

typedef enum {ADD, QUERY, OPEN, ACK } KIND;	  // Message kinds
typedef struct {char kind[10]; int port1; int port2; char port3[10]; char switch_no[5]; } MSG;


// fifo declare
int fifo_1_0;
// fifo declare as array:
int *fifo;


//function declartion
char* RemoveDigits(char* input);
void controller(int n_swithes, int portNumber);
void switches(char **arg, const string &input, char *serverAddress, int portNumber);
MSG composeMSTR (const string &a,  int port1,  int port2, char *port3, char *kind);
void sendFrame (int fd, MSG *msg);
string rcvFrame (int fd);
int split(char inStr[],  char token[][MAXWORD], char fs[]);
char * format_swi(const string &a);
void set_cpu_time();
string convert_int_to_string(int input);
void error(const char *msg);





// no need to use fifo;
// implment socket with number of switch and portNumber (address is system address: 127.0.0.1)
// binding to socket and start to accept connection; 
// in select(), listen the multiple incoming msg;
void controller(int n_swithes, int portNumber){

	// socket setup
	int sockfd, newsockfd;							 // socket (file decriptor) declaration:
	socklen_t clilen; 								 // length of bytes for store address
	char *buffer = new char[256];		
    struct sockaddr_in serv_addr, cli_addr;		     // define two scokaddr_in (struct) serv_addr and cli_addr; 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 		 // define socket
    												 // error checking 
    if (sockfd < 0) { error("ERROR opening socket");} 
    serv_addr.sin_family = AF_INET;					 // set the server family as IPv4
    serv_addr.sin_addr.s_addr = INADDR_ANY;			 // receive msg from everywhere
    serv_addr.sin_port = htons(portNumber);		     // define the port number 
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {error("ERROR on binding");}
    listen(sockfd,n_swithes);						 // start to listen incoming connection




	int OPEN = 0;
	int ACK  = 0;
	int QUERY= 0;
	int ADD  = 0;
	char switches[n_swithes][50];



	// variable for select()
	// Ref: Youtube channels (keyword: select toturial)
	// int fd;
	// char buf[11];
	// int ret, sret;
	// fd = 0;
	// fd_set readfds;
	// // variable for select()

	// MSG    msg;
	// string rvc_msg;

	// char kind[10] = "OPEN";
	// string ab_string = "null";
	// char ab_char[5] = "null";
	// msg = composeMSTR(ab_string,0,0,ab_char,kind);




	// for(int i = 0; i < n_swithes; i++){
	// 	rvc_msg = rcvFrame(fifo_1_0);
	// 	cout << "before send msg controller" << endl;
	// 	int fifo_nm = fifo[i];
	// 	cout << fifo_nm << endl;
	// 	sendFrame(fifo_nm, &msg);
	// 	cout << "after send msg controller" << endl;
	// 	// cout << "recieved msg from switches: " << rvc_msg << endl;
	// 	char *switch_1 = format_swi(rvc_msg);
	// 	strcpy(switches[i],switch_1);
	// 	OPEN = OPEN+1;
	// 	ACK = ACK+1;

	// }



    string OPEN_s = convert_int_to_string(OPEN);
	string ACK_s = convert_int_to_string(ACK);
	string QUERY_s = convert_int_to_string(QUERY);
	string ADD_s = convert_int_to_string(ADD);
	string general_info_1 = "Packet Stats:\n \t Recived:  OPEN:"+ OPEN_s +" QUERY:" +QUERY_s+"\n";
	string general_info_2 = "\t Transmitted:  ACK:"+ACK_s+" ADD:"+ADD_s;
	string general_info = general_info_1 + general_info_2;




	// poll setup
	struct pollfd polls[7];
	int    timeout;
	int    current_msg = 0;
	timeout = (10* 1000);			// set timeout for 10 sec;
	int rc_msg;
	
	polls[0].fd = 0;
	polls[0].events = POLLIN;
	polls[1].fd = sockfd;
	polls[1].events = POLLIN;
	// polls[2].fd = sockfd_2;

	while(1){
		

		// poll setup
		// int poll(struct pollfd fdarray[], nfds_t nfds, int timeout);

		rc_msg = poll(polls, 7, 0);
		// if (rc_msg < 0) {error("poll() failed...\n");}
		// if (rc_msg == 0){cout<<"poll() timeout..."<<endl;}
		// if (rc_msg > 0 ) { cout<< "poll() msg" << endl;}
		if (rc_msg < 0) {
			error("poll() failed...\n");
		}
		else if (rc_msg == 0){
			continue;
		}
		else{

			for (int i = 0; i< current_msg; i++){
				if( polls[i].revents & POLLIN ) {
					cout << "revents: " << polls[i].fd << endl;
					if (polls[i].fd == 0){
						char *buffer = new char[10];
						read(polls[i].fd,buffer,10);
						cout << "stdin: " << buffer << endl;

					}else if(polls[i].fd == sockfd){

						cout  << "ACCEPTING CONNECTION ... " << endl;

					}

				}
				else{
					continue;
				}
				// end of for loop
			}

		}
		// should I start to listen right away like this?
		// https://linux.die.net/man/3/poll
		// struct pollfd {
  //    		int    fd;       /* file descriptor to check, or <0 to ignore */
  //    		short  events;   /* events of interest on fd */
  //    		short  revents;  /* events that occurred on fd */
		// };
	}



}



// use fifo to send query to nearby switches
// use socket to establish connection with controllers 
// implement delay
// when the switch is down, the connection lost signal needs to be sent to controller 
void switches(char **arg, const string &input, char *serverAddress, int portNumber){
	
	int sockfd,n;
	struct sockaddr_in server_obj;
	struct hostent *server;
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 					// define the socket
	if (sockfd < 0) { error("ERROR opening socket"); }
	server = gethostbyname(serverAddress);
	// struct hostent {
    //     char   *h_name;
    //     char  **h_aliases;
    //     int     h_addrtype;
    //     int     h_length;
    //     char  **h_addr_list; 
    // }
	if (server == NULL) { 
		cout << "Can't read the server address! " << endl;		// error check the server
        exit(0);
    }

	bzero((char *) &server_obj, sizeof(server_obj));
	server_obj.sin_family = AF_INET;						    //define sin_family as AF_INET/IPv4
	bcopy((char *)server->h_addr, (char *)&server_obj.sin_addr.s_addr, server->h_length);
	server_obj.sin_port = htons(portNumber); 					// define sin_port by given port use function htons()

	if (connect(sockfd,(struct sockaddr *) &server_obj,sizeof(server_obj)) < 0) 
	{ 
		error("ERROR connecting");
	}else{
		cout << "Succesfully connect to server!... " << endl;
	}

	char *buffer = new char[256];   // set all buffer byte to zero; initialize buffer 
    // fgets(buffer,255,stdin); // get user input 


	// should write to server in specific conditions and read from server aswellas other switches in while loop
    // n = write(sockfd,buffer,strlen(buffer)); // write to the socket sockfd and send 
	// if (n < 0) { error("ERROR writing to socket");}

	// n = read(sockfd,buffer,255);  // read back from socket sockfd 
	// if (n < 0) { error("ERROR reading from socket") ;}



	// parse the packet file
	// and follow the rules to prepare the msg send to controller;
	// controller will either tell switch to drop the packet for send the packet to nearby switches
	int    port1;
	int    port2;
	char  *port3;
	port3= new char[20]; // allocate memory for pointer *port3;
	int dest_ip_low;
	int dest_ip_high;

	int DELIVER  = 0;
	int pri      = PRI;
	int pkgCount = 0;

	int ADMIT    = 0;
	int OPEN     = 0;
	int ACK      = 0;
	int QUERY    = 0;   // send to controller the query
	int ADDRULE  = 0;   // 
	int RELAYOUT = 0;
	int RELAYIN  = 0;

	int num_of_rules = 0;

	//determine the fifo number
	int fifo_n;
	char const * tmp_input = input.c_str();
	
	sscanf(tmp_input, "sw%d", &fifo_n);
	//cout << tmp_input << " and " << fifo_n << endl;
	
	int fifo_number = fifo[fifo_n-1];
	
	cout << fifo_number << endl;
	
	// cout << arg[1] << endl; // sw
	// cout << arg[2] << endl; // file name
	// cout << arg[3] << endl; // port 1
	// cout << arg[4] << endl; // port 2
	// cout << arg[5] << endl; // port 3 100-110
	

	if(strcmp(arg[3],"null") == 0){ port1 = -1;} else {int n;sscanf(arg[3], "sw%d", &n);port1 = n;}
	if(strcmp(arg[4],"null") == 0){ port2 = -1;} else {int n;sscanf(arg[4], "sw%d", &n);port2 = n;}
	
	strcpy(port3,"100-110");
	int DEST_PORT_LOW, DEST_PORT_HIGH;
	sscanf(port3,"%d-%d",&DEST_PORT_LOW,&DEST_PORT_HIGH);
	// cout << SCR_PORT << endl;
	//parse String
	ifstream myfile;
	string STRING;
	myfile.open(arg[2]);
	
	while(!myfile.eof()) // To get you all the lines.
	{
        getline(myfile,STRING); // Saves the line in STRING.
        int src_port, dest_port;
        if (STRING.substr(0,1).compare("#")!=0 && STRING.substr(0,1).compare("")!=0 ){
			// split string here:
			char delimiter[1];
			strcpy(delimiter," ");
			char * tab2 = new char [STRING.length()+1];
			strcpy (tab2, STRING.c_str());
			char splited_str[MAXLINE][MAXWORD];
			split(tab2,splited_str,delimiter);
			// cout << splited_str[0] << endl;
			// cout << splited_str[1] << endl;
			// cout << splited_str[2] << endl;
			string first_arg(splited_str[0]);
			// cout << first_arg << "and " << input<< endl;
        	if(first_arg.compare(input)==0){

        		string src_port_s = STRING.substr(5,5);
        		string dest_port_s = STRING.substr(10,10);
        		src_port  = atoi(src_port_s.c_str());
        		dest_port = atoi(dest_port_s.c_str());
				// cout << src_port << ":" << dest_port << endl;
        		if ((src_port >= S_LOW && src_port <= S_HIGH) && (dest_port >= DEST_PORT_LOW && dest_port <= DEST_PORT_HIGH)){
        			ADMIT++;
        			DELIVER++;
        			pkgCount++;
					// cout << "destination port: " << dest_port << endl;
        		}else{
					// cout << "destination port (else): " << dest_port << endl;
        			DELIVER++;
        			ADDRULE++;
        			QUERY++;
        		}
        	}
        }

    }
	
	myfile.close();

	// exit(0);
	// variable for select()
	int fd;
	char buf[11];
	int ret, sret;
	fd = 0;
	fd_set readfds;
	// variable for select()



	MSG send_msg;
	char kind[10] = "ACK";
	string rvc_msg;
	// send msg to controller;
	// cout << fifo_number << endl;	
	send_msg = composeMSTR(input,port1,port2,port3,kind);

	sendFrame(fifo_1_0,&send_msg);
	cout << "before get msg switches" << endl;
	rvc_msg = rcvFrame(fifo_number); 
	cout << "recived from controller: " << rvc_msg << endl;
	OPEN++;
	ACK++;


	// prepare the print for list command 
	// string srcIP = "0-1000";
	// predefine num_of_rules as 1
	string str(port3);
	num_of_rules = 1;
	string list_command[num_of_rules];
	// switch information
	for (int i=0; i< num_of_rules; i++){
		string i_s = convert_int_to_string(i);
		string pkgCount_s = convert_int_to_string(pkgCount);
		string DELIVER_s  = convert_int_to_string(DELIVER);
		string single_command = "["+i_s+"]" + "(srcIP= 0-1000, destIP= "+ port3 +", action= "+" DELIVER: "+DELIVER_s+ " pri= 4, pkgCount= " + pkgCount_s + ")";
		list_command[i] = single_command;
	}

	// general information (total)
	string ADMIT_s    = convert_int_to_string(ADMIT);
	string ACK_s      = convert_int_to_string(ACK);
	string ADDRULE_s  = convert_int_to_string(ADDRULE);
	string RELAYIN_s  = convert_int_to_string(RELAYIN);
	string OPEN_s     = convert_int_to_string(OPEN);
	string QUERY_s    = convert_int_to_string(QUERY);
	string RELAYOUT_s = convert_int_to_string(RELAYOUT);
	string general_info_1 = "Packet Stats: \n \t Recived: ADMIT:" + ADMIT_s + ", ACK: " + ACK_s + ", ADDRULE: " + ADDRULE_s + ", RELAYIN: "+ RELAYIN_s +"\n";
	string general_info_2 = "\t Recived: OPEN:" + OPEN_s + ", QUERY: " + QUERY_s + ", RELAYOUT: " + RELAYOUT_s + "\n";
	string general_info = general_info_1 + general_info_2;


	// poll setup
	// what will be the size of polls?

	struct pollfd polls[2];
	int    timeout;
	int    current_msg = 0;
	timeout = (10 * 1000);			// set timeout for 10 seconds
	int rc_msg;

	while(1){

		rc_msg = poll(polls, 2, timeout);
		if (rc_msg < 0) {error("poll() failed...\n");}
		if (rc_msg == 0){cout<<"poll() timeout..."<<endl; break;}
		// https://linux.die.net/man/3/poll
		// struct pollfd {
  //    		int    fd;       /* file descriptor to check, or <0 to ignore */
  //    		short  events;   /* events of interest on fd */
  //    		short  revents;  /* events that occurred on fd */
		// };

		current_msg = 2;
		// Loop through the descriptors for POLLIN and determine whether
		// it is the listening or active connection

		for (int i = 0; i< current_msg;i++){
			// stdin?
			if(polls[i].events == 0){
        		continue;
			}
			if(polls[i].events != POLLIN){
				cout << "Error revents: " << polls[i].revents << endl;
				break;
				// break;
			}

			
			if(polls[i].events == sockfd){
				// socket portion
				char *buffer = new char[255];
				n = read(sockfd,buffer,255);  // read back from socket sockfd 
				// clilen = sizeof(cli_addr);
				// // store msg in clilen
				// newsockfd = accept(sockfd,  (struct sockaddr *) &cli_addr, &clilen);
				// if (newsockfd < 0){
				// 	error("accept() failed! ");
				// }
			}
		}
	}
	// while(1){


	// 	/////////////////////////////////////
	// 	FD_ZERO(&readfds);
	// 	FD_SET(fd,&readfds);
	// 	select(8,&readfds,NULL,NULL,NULL);
	// 	memset((void *) buf, 0, 11);
	// 	ret = read(fd, (void*)buf, 10);
	// 	/////////////////////////////////////
	// 	// select will minitor file descriptor 1024 (poll doesn't have this limitation)
	// 	// stdin 0; stdout 1; stderr 2; incrementing with open
	// 	// poll() or select() detail for read msg


	// 	if(ret != -1){

	// 		if(strcmp(buf,"list")==10){
	// 			for(int i=0; i<num_of_rules; i++){
	// 				cout << list_command[i] << endl;
	// 			}
	// 			cout << general_info << endl;

	// 		}
	// 		else if (strcmp(buf,"exit")==10){
	// 			break;
	// 		}
	// 		else{
	// 			cout << "unknown command! [list/exit]" << endl;
	// 		}
	// 	}
	// }


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
MSG composeMSTR (const string &a,  int port1,  int port2,  char *port3, char *kind )
{
    MSG  msg;

    memset( (char *) &msg, 0, sizeof(msg) );
   
    msg.port1 = port1;
    msg.port2 = port2;
    strcpy(msg.port3,port3);
    strcpy(msg.kind,kind);
   
    strncpy(msg.switch_no,a.c_str(),sizeof(msg.switch_no)); // port1 value changed at this point;
    return msg;
} 


// recive the msg struct, but send the string object;
void sendFrame (int fd, MSG *msg)
{

	char *MESSAGE_P = (char *) malloc(8192);

	
	string port1 = convert_int_to_string(msg->port1);
	string port2 = convert_int_to_string(msg->port2);
	string port3 = msg->port3;
	string s_no  = msg->switch_no;
	string kind  = msg->kind;

	string MESSAGE = port1 + ";" + port2 + ";" + port3 + ";" + s_no + ";" + kind;
	char const * MESSAGE_P_P = MESSAGE.c_str();
	// cout << "sending msg: " << MESSAGE_P << endl;
	// cout << MESSAGE_P << endl;
	write (fd, MESSAGE_P_P, 8192); // write the message_p into fifo file with constraint 8192

}

       
string rcvFrame (int fd)
{ 
	int len; 
	char * MESSAGE_P = (char *) malloc(8192);

    len = read (fd, MESSAGE_P, 8192);


    string str(MESSAGE_P);

    return MESSAGE_P;	  
}


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


char * format_swi(const string &a){
	// char const * msg = a.c_str();
	char strings[100];
	char delimiter[1];
	strcpy(delimiter,";");
	//////////////////////////////////
	char * tab2 = new char [a.length()+1];
	strcpy (tab2, a.c_str());
	char splited_str[MAXLINE][MAXWORD];
	// int** splited_str = new int*[MAXWORD];
	// char string_ing[100] = "-1;-1;100-110;sw1;ACK"; // this will pass successfully
	split(tab2,splited_str,delimiter);

	// cout << splited_str[0] << endl;
	// cout << splited_str[1] << endl;
	// cout << splited_str[2] << endl;
	// cout << splited_str[3] << endl;
	// cout << splited_str[4] << endl;
	char *MESSAGE_P = (char *) malloc(8192);
	string switch_no(splited_str[3]);
	string port1(splited_str[0]);
	string port2(splited_str[1]);
	string port3(splited_str[2]);
	string message = "["+ switch_no +"] port1= " +port1+ ", port2= " +port2+", port3= "+port3+"\n";
 	// cout << message << endl;
 	char const *cstr = message.c_str();
 	strcpy(MESSAGE_P,cstr);
	return MESSAGE_P;
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
	if (argc < 4){ cout << "Too little arguments " << endl; return 0;}
	if (argc > 8){ cout << "Too many arguments "   << endl; return 0;}

	string argv_1_rep = "";
	argv_1_rep = argv[1]; 
	char *argv_1 = RemoveDigits(argv[1]);


	
	string fifo_name = "fifo_1_";
	fifo = new int[7];
        for (int i = 0; i<7; i++){
		int tmp_i = i;
		tmp_i++;
		string num = convert_int_to_string(tmp_i);
		string fifo_name_ = fifo_name + num;
		char const * fifo_name_char = fifo_name_.c_str();
		if ((fifo[i] = open(fifo_name_char,O_RDWR))< 0){
			cout << "NO" << fifo_name_char << "EXIST!" << endl;
			return 0;
		}
	
	}
	// work with correct input value first 
	// controller argument check miss check the validity of portNumber 
	// switches argument check miss check the validity of portNumber and serverAddress
	if (strcmp(argv_1,"cont") == 0){
		if (argc != 4){ cout << "Incorrected argument (e.g. a3sdn cont nSwitch portNumber)!" << endl; return 0; }
		// if (argc < 3){ cout << "Identify number of switches and the portNumber" << endl; return 0; }
		
		int n_swithes = atoi(argv[2]);
		if (n_swithes > MAX_SWITCH){
			cout << "Too many switches (MAX: 7)!" << endl;
			return 0;
		}
		int portNumber = atoi(argv[3]);

			
		set_cpu_time();
		cout << "num_of_switches: " << n_swithes << "; port number: " << portNumber << endl;
		controller(n_swithes,portNumber);
	}
	
	else if(strcmp(argv_1, "sw") == 0){
		if(argc != 8){
			cout << "Required 8 arguments (e.g. a3sdn swi trafficFile (null|swj) (null|swk) IPlow-IPhigh serverAddress portNumber)!" << endl;
			return 0;
		}
		// try to open the dat.file
		char * serverAddress = new char [20];
		strcpy(serverAddress,argv[6]);
		int portNumber = atoi(argv[7]);

		set_cpu_time();
		switches(argv,argv_1_rep,serverAddress,portNumber);

	}
	else{

		cout << "Invalid Command!" << endl;
		return 0;
	}

    // cout <<"this line" << endl;


  
    return 0; 
} 



































