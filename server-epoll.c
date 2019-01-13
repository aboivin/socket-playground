#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include <unistd.h>

#define MAX_EVENTS 5

int waitForTCPConnection(int portNumber) {
    //create the server socket
  int socketDescriptor = socket(AF_INET,SOCK_STREAM,0);
  
  //define the server address
  //creating the address as same way we have created for TCPclient
  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(portNumber);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  //calling bind function to oir specified IP and port
  bind(socketDescriptor,(struct sockaddr*) &serverAddress,sizeof(serverAddress));

  listen(socketDescriptor,5);

  //starting the accepting 
  //accept(socketWeAreAccepting,structuresClientIsConnectingFrom,)
  int client_socket = accept(socketDescriptor, NULL, NULL);

  printf("\r\n%d", client_socket);
  return client_socket;
}

int registerEpollFD(int epoll_fd, int fd) {
  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = fd;
 
  if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event))
  {
    fprintf(stderr, "Failed to add file descriptor to epoll\n");
    close(epoll_fd);
    return 1;
  }
}

int main(){

  int running = 1, event_count, i;
  struct epoll_event events[MAX_EVENTS];
  int epoll_fd = epoll_create1(0);
 
  if(epoll_fd == -1)
  {
    fprintf(stderr, "Failed to create epoll file descriptor\n");
    return 1;
  }
 
  int client_socket = waitForTCPConnection(9002);
  registerEpollFD(epoll_fd, client_socket);

  int client_socket2 = waitForTCPConnection(9003);
  registerEpollFD(epoll_fd, client_socket2);

  while(running)
  {
    printf("\nPolling for input...\n");
    sleep(3);
    event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, 30000);
    printf("%d ready events\n", event_count);
    for(i = 0; i < event_count; i++)
    {
      char receivedData[256];
      recv(events[i].data.fd, receivedData, 256, 0);
      printf("\r\n%s", receivedData);
 
      if(!strncmp(receivedData, "stop\n", 5)) {
        running = 0;
      }
    }
  }
 
  if(close(epoll_fd))
  {
    fprintf(stderr, "Failed to close epoll file descriptor\n");
    return 1;
  }

   close(client_socket);
  //close the socket
  //sleep(10);
  //close(socketDescriptor);
 
}
