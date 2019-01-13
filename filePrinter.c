#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>

int main(){

    printf("Hello World\n");
    FILE *fptr = fopen("/home/aboivin/workspace/socket-tests/a.txt","r");
    char ch;
    while((ch = fgetc(fptr)) != EOF)
      printf("%c", ch);

    sleep(10);
    fclose(fptr);
    return 0;
}