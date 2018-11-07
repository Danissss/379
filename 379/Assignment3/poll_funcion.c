#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <poll.h>
 
 
#define POLL_SIZE 32
#define LISTEN_QUEUE 5
#define SOCKET_PATH "web/server_socket.sock"
 
 
#define TRUE 1
#define FALSE 0
 
 
 
 
int main()
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_un server_address;
    struct sockaddr_un client_address;
    int result;
 
 
    struct pollfd poll_set[POLL_SIZE];
    int numfds = 0;
 
 
    unlink(SOCKET_PATH);
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
 
 
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, SOCKET_PATH);
    server_len = sizeof(server_address);
 
 
    bind(server_sockfd, (struct sockaddr *)&server_address, (socklen_t)server_len);
    listen(server_sockfd, 5);
    memset(poll_set, '\0', sizeof(poll_set));
    poll_set[0].fd = server_sockfd;
    poll_set[0].events = POLLIN;
    numfds++;
    //signal(SIGPIPE, SIG_IGN);
 
 
 
 
    
    // where the poll start to poll the incoming msg/socket
 
    while (1) {
        char ch;
        int fd_index;
        int nread;
 
 
        printf("Waiting for client (%d total)...\n", numfds);
  
        // int poll(struct pollfd fdarray[], nfds_t nfds, int timeout);
        // Returns: count of ready descriptors, 0 on timeout, −1 on error
        // timeout == -1: wait forever
        // timeout == 0 : don't wait
        // timeout > 0  : wait for timeout time
        poll(poll_set, numfds, -1);
        for(fd_index = 0; fd_index < numfds; fd_index++)
        {
            if( poll_set[fd_index].revents & POLLIN ) {
                if(poll_set[fd_index].fd == server_sockfd) {
                    client_len = sizeof(client_address);
                    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t *)&client_len);
 
 
                    poll_set[numfds].fd = client_sockfd;
                    poll_set[numfds].events = POLLIN;
                    numfds++;
 
 
                    printf("Adding client on fd %d\n", client_sockfd);
                }
                else {
                    ioctl(poll_set[fd_index].fd, FIONREAD, &nread);
 
 
                    if( nread == 0 )
                    {
                        close(poll_set[fd_index].fd);
                        poll_set[fd_index].events = 0;
                        printf("Removing client on fd %d\n", poll_set[fd_index].fd);
                        //int i = fd_index;
                        //for( i = fd_index; i<numfds; i++)
                        //{
                        poll_set[fd_index] = poll_set[fd_index + 1];
                        //}
                        numfds--;
                    }
 
 
                    else {
                        read(poll_set[fd_index].fd, &ch, 1);
                        printf("Serving client on fd %d\n", poll_set[fd_index].fd);
                        ch++;
                        write(poll_set[fd_index].fd, &ch, 1);
                    }
                }
            }
 
 
 
 
        }
 
 
 
 
 
 
    }
}

// client
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>
 
 
#define SOCKET_PATH "web/server_socket.sock"
 
 
int main()
{
    int sockfd;
    int len;
    struct sockaddr_un address;
    int result;
    char ch = 'A';
 
 
    /* -AF_UNIX is a UNIX internal socket, created on the filesystem.
     * -SOCK_STREAM is a reliable, sequenced, connection-based two-way byte stream
     *  (INET equivalent is TCP) */
      
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
 
 
    /* -sun_family - specifies type (domain) of socket
     * -sun_path - socket path location (on filesystem) */
 
 
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);
    len = sizeof(address);
 
 
    /* request connection from the server, connecting socket to server with specified address
     * 'len' is the length of the address */
    result = connect(sockfd, (struct sockaddr *)&address, len);
 
 
    /* exit with error if connection is not successful */
    if(result == -1) {
        perror("oops: client1");
        exit(1);
    }
 
 
    /* otherwise write data to server and obtain response */
    write(sockfd, &ch, 1);
    read(sockfd, &ch, 1);
    printf("char from server: %c\n", ch);
    close(sockfd);
 
 
    exit(0);
}