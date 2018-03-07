#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#define PORT 4000 

typedef struct{
char message[50];
int USRLED;
int length;
}mystruct;


int main()
{
  struct sockaddr_in addr,socket_addr;
  int rec_len = sizeof(socket_addr);
  int server_connect,temp,var,j=0;
  int cond;
  char Buffer[500];
  server_connect= socket(AF_INET,SOCK_STREAM,0);    //connect to server
  if (server_connect == 0)
  {
   printf("Error in Socket initialization");
   exit(1);
  }
  setsockopt(server_connect, SOL_SOCKET, SO_REUSEADDR, &(cond), sizeof(cond));
  addr.sin_family = AF_INET;                        //details to suitable port and ip
  addr.sin_addr.s_addr = INADDR_ANY;	
  addr.sin_port = htons(PORT);     

  int check= bind(server_connect, (struct sockaddr*)&addr, sizeof(addr));
  //check if binded
  if (check < 0)
  {
   printf("Socket is not binded");
   }
  
  listen(server_connect,5);

  temp = accept(server_connect, (struct sockaddr*)&socket_addr,(socklen_t*)&rec_len);                          //accept the package given by client
  if( temp < 0)
  {
    printf("Error in receiving");
    exit(1);
  }
  size_t bored;
  var = read(temp,&bored,sizeof(size_t));
  while(var = read(temp, Buffer +j,500) < bored)
   {
    j= j+ var;
    }
  mystruct *ptr =  (mystruct*)Buffer;
  printf("String:%s String Length: %d USR LED Status: %d\n",ptr->message,ptr->length,ptr->USRLED);   //print the recvd details
close(temp);
exit(0);
}



