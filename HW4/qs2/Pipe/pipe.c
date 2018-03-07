// C program to illustrate IPC mechanism by pipes.
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MSGSIZE 16

typedef struct              //structure to be sent
{             
char message[50];     
int USRLED;
int length;
}mystruct;
 
int main()
{
    int p[2],q[2];
    pid_t pid;
    if ((pipe(p) < 0) || (pipe(q) < 0 ))        //pipe initialisation
        {
        printf("Error in pipe initialization."); 
	exit(1);
	}
    pid = fork();
    if (pid>0)
	{
	char *content="I am your parent\n",*ptr;
        mystruct traveller;
        ptr = (char*) &traveller;
	memcpy(traveller.message,content,strlen(content)+1);
	traveller.length=strlen(traveller.message);
	traveller.USRLED = 1;

	close(p[0]);
	write(p[1],ptr,sizeof(traveller));       //send structure to child in pipe 1
        printf("Message sent to child\n");     
	close(p[1]);
	wait(NULL);
        
	close(q[1]);

	mystruct *ptr1;
	char buffer[sizeof (mystruct)]={0};
	read(q[0],buffer,sizeof(mystruct));
	ptr1 = (mystruct*)buffer;
	printf("Message received by parent from child: %s, String Length: %d, USR LED status is %d.\n",ptr1->message,ptr1->length,ptr1->USRLED);
	close(q[0]);                              //read recvd structure at parent
	exit(0);
	}
    else if(pid==0)
	{
	
	close(p[1]);
	char buffer[sizeof (mystruct)]={0};
	mystruct *ptr1;
	read(p[0],buffer,sizeof(mystruct));
	ptr1 = (mystruct*)buffer;
	printf("Message received by child from parent: %s, String Length:%d, USR LED status is %d\n",ptr1->message,ptr1->length,ptr1->USRLED);
	close(p[0]);                          //read rcvd structure at child
	close(q[0]);
	char *content="I am your obedient child\n",*ptr;
        mystruct traveller={0};
        ptr = (char*) &traveller;
	memcpy(traveller.message,content,strlen(content)+1);
	traveller.length = strlen(traveller.message);
	traveller.USRLED = 0;
        write(q[1],ptr,sizeof(traveller));    //send structure to parent in pipe 2
        printf("Message sent to parent\n");
        close(q[1]);
	exit(0);
	}
    else
	{
         printf("Error in creating child process\n");
	 exit(1);
	}
    return 0;
}
