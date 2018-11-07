/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
// establish the connection bewteen controller and swtiches
int main(int argc, char *argv[])
{   
     // sockfd and newsockfd will be socket file descriptor  as accept return file socket descriptor
     int sockfd, newsockfd, portno;

     socklen_t clilen;   //socklen_t         ai_addrlen;  length of bytes of address

     char buffer[256];

     // define two scokaddr_in struct  serv_addr and cli_addr; 
     struct sockaddr_in serv_addr, cli_addr;

     int n;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0); // define socket

     if (sockfd < 0) 
        error("ERROR opening socket"); // error check socket define error (if any)

     bzero((char *) &serv_addr, sizeof(serv_addr));

     portno = atoi(argv[1]);

     serv_addr.sin_family = AF_INET;
     //For the Internet domain, if we specify the special IP address INADDR_ANY (defined in <netinet/in.h>), 
     // the socket endpoint will be bound to all the system’s network interfaces. This means that we can 
     // receive packets from any of the network interface cards installed in the system. (i.e. 127.0.0.1)
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     // int bind(int sockfd, const struct sockaddr *addr, socklen_t len);
     // as client will be given ip address, so it will establish connection
     // server will needs to bind the socket with any address in the system (INADDR_ANY);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     // int listen(int sockfd, int backlog);
     // A server announces that it is willing to accept connect requests by calling the listen function.
     // The backlog argument provides a hint to the system regarding the number of outstanding connect 
          // requests that it should enqueue on behalf of the process. 
          // The actual value is determined by the system, but the upper limit is specified as SOMAXCONN in <sys/socket.h>.
     listen(sockfd,5); // listen any income connection; for my case backlog = num of swithes

     clilen = sizeof(cli_addr);

     // Once a server has called listen, the socket used can receive connect requests. 
     // We use the accept function to retrieve a connect request and convert it into a connection.
     // int accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict len);
     // Returns: file (socket) descriptor if OK, −1 on error
     newsockfd = accept(sockfd,  (struct sockaddr *) &cli_addr, &clilen);



     if (newsockfd < 0) 
          error("ERROR on accept"); // error checking for accept

     bzero(buffer,256);

     n = read(newsockfd,buffer,255); // read the newsockfd; not the sockfd

     if (n < 0) error("ERROR reading from socket");

     printf("Here is the message: %s\n",buffer);

     n = write(newsockfd,"I got your message",18); // after get the incoming msg, send back to the newsockfd (since receive from newsockfd )

     if (n < 0) error("ERROR writing to socket");


     // needs to close both socket
     close(newsockfd);

     close(sockfd);
     
     return 0; 
}
