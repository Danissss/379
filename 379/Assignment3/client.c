#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    // struct sockaddr_in {
    //     sa_family_t    sin_family;
    //     in_port_t      sin_port;
    //     struct in_addr sin_addr;
    // }

    struct sockaddr_in serv_addr;

    // struct hostent {
    //     char   *h_name;
    //     char  **h_aliases;
    //     int     h_addrtype;
    //     int     h_length;
    //     char  **h_addr_list; 
    // }


    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }


    portno = atoi(argv[2]);
    // AF_INET: IPv4 Internet domain
    // SOCK_STREAM: sequenced, reliable, bidirectional, connection-oriented byte streams
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // define the socket 

    if (sockfd < 0) 
        error("ERROR opening socket");  // error checking the socket file 
 
    server = gethostbyname(argv[1]);   // get server by its ip address 

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n"); // error check for server
        exit(0);
    }

    // bzero -- write zeroes to a byte string
    bzero((char *) &serv_addr, sizeof(serv_addr));



    serv_addr.sin_family = AF_INET; //define sin_family as AF_INET/IPv4


    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);         // copy the ip address to struct in_addr sin_addr in serv_addr;


    serv_addr.sin_port = htons(portno); // define sin_port by given port use function htons()

    // connect to the host using socket 
    // connection establishment
    // int connect(int sockfd, const struct sockaddr *addr, socklen_t len);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");


    printf("Please enter the message: ");

    bzero(buffer,256);
    fgets(buffer,255,stdin); // get user input 

    n = write(sockfd,buffer,strlen(buffer)); // write to the socket sockfd and send 
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);


    n = read(sockfd,buffer,255);  // read back from socket sockfd 

    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);

    close(sockfd);  // close socket
    return 0;
}