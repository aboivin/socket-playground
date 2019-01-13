#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include <unistd.h>

#define MAX_EVENTS 5
#define READ_SIZE 10

int main(){

  int running = 1, event_count, i;
  size_t bytes_read;
  char read_buffer[READ_SIZE + 1];
  struct epoll_event event, events[MAX_EVENTS];
  int epoll_fd = epoll_create1(0);
 
  if(epoll_fd == -1)
  {
    fprintf(stderr, "Failed to create epoll file descriptor\n");
    return 1;
  }
 
  event.events = EPOLLIN;
  event.data.fd = 0;
 
  if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event))
  {
    fprintf(stderr, "Failed to add file descriptor to epoll\n");
    close(epoll_fd);
    return 1;
  }
 
  while(running)
  {
    printf("\nPolling for input...\n");
    event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, 30000);
    printf("%d ready events\n", event_count);
    for(i = 0; i < event_count; i++)
    {
      printf("Reading file descriptor '%d' -- ", events[i].data.fd);
      bytes_read = read(events[i].data.fd, read_buffer, READ_SIZE);
      printf("%zd bytes read.\n", bytes_read);
      read_buffer[bytes_read] = '\0';
      printf("Read '%s'\n", read_buffer);
 
      if(!strncmp(read_buffer, "stop\n", 5))
        running = 0;
    }
  }
 
  if(close(epoll_fd))
  {
    fprintf(stderr, "Failed to close epoll file descriptor\n");
    return 1;
  }
  

  char serverMessage[4096] = "\
HTTP/1.1 200 OK\r\n\
Date: Sun, 28 Jan 2018 23:11:04 GMT\r\n\
Server: Apache/2.4.6 (CentOS) PHP/5.4.16\r\n\
Last-Modified: Sun, 28 Jan 2018 20:10:37 GMT\r\n\
Content-Length: 3890\r\n\
Content-Type: text/html; charset=UTF-8\r\n\
\r\n\
<!DOCTYPE html>\r\n\
<html>\r\n\
<head>\r\n\
<title>Hello !</title>\r\n\
</head>\r\n\
<body>\r\n\
<h1>C http server up and running</h1>\r\n\
</body>\r\n\
</html>\r\n";

  //create the server socket
  int socketDescriptor = socket(AF_INET,SOCK_STREAM,0);
  
  //define the server address
  //creating the address as same way we have created for TCPclient
  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(9002);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  //calling bind function to oir specified IP and port
  bind(socketDescriptor,(struct sockaddr*) &serverAddress,sizeof(serverAddress));

  listen(socketDescriptor,5);

  //starting the accepting 
  //accept(socketWeAreAccepting,structuresClientIsConnectingFrom,)
  int client_socket = accept(socketDescriptor, NULL, NULL);
  
  // receive the data
  char receivedData[256];
  recv(client_socket, receivedData, 256, 0);
  printf("\r\n%s", receivedData);

  //sending data
  //send(toWhom,Message,SizeOfMessage,FLAG);
  send(client_socket,serverMessage,sizeof(serverMessage),0);
  sleep(1);
  shutdown(client_socket, SHUT_RDWR);
  close(client_socket);
  //close the socket
  sleep(10);
  close(socketDescriptor);
    return 0;
}
