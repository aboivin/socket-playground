#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>

int main(){


char content[4096] = "\
<!DOCTYPE html>\r\n\
<html>\r\n\
<head>\r\n\
<title>Hello !</title>\r\n\
</head>\r\n\
<body>\r\n\
<h1>C http server up and running</h1>\r\n\
</body>\r\n\
</html>\r\n";

char header[4096] = "\
HTTP/1.1 200 OK\r\n\
Date: Sun, 28 Jan 2018 23:11:04 GMT\r\n\
Server: Apache/2.4.6 (CentOS) PHP/5.4.16\r\n\
Last-Modified: Sun, 28 Jan 2018 20:10:37 GMT\r\n\
Accept-Ranges: bytes\r\n\
Content-Length: ";

char length[12];
sprintf(length, "%d", sizeof(content));

strcat(header, length);
strcat(header, "\r\n\
Content-Type: text/html; charset=UTF-8\r\n\
\r\n");


    printf("%s", header);
    return 0;
}