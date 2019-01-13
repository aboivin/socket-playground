#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>

int main(){

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