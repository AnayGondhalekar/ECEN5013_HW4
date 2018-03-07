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
#define IP "127.0.0.1"

typedef struct{
char message[50];
int USRLED;
int length;
}mystruct;

int main()
{
  struct sockaddr_in addr, serv_addr;
  char *client_msg = "I am your Client";
  char *ptr;
  int count;
  
  mystruct traveller;
  ptr = (char*)&traveller;

  memcpy(traveller.message,client_msg,strlen(client_msg)+1);
  traveller.length = strlen(traveller.message);
  traveller.USRLED = 0;
  int client_connect = socket(AF_INET,SOCK_STREAM,0);

  if(client_connect < 0)
	{
	printf("Error in initialising the socket");
	exit(1);
	}
 
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  inet_pton(AF_INET,IP,&serv_addr.sin_addr);
  int val = connect(client_connect, (struct sockaddr *)&serv_addr,sizeof (serv_addr));
  if(val < 0);
  {
   printf("Connection Error");
   exit(1);
  }

  size_t length_to_send = sizeof(traveller);
  send(client_connect,&length_to_send,sizeof(size_t),0);
  count = send(client_connect , (char*)&traveller, sizeof(traveller),0);
  
  printf("String:%s\nString Length:%d USR LED Status: %d\n",traveller.message,traveller.length,traveller.USRLED); 
  
  close(client_connect);
  exit(0);
}

