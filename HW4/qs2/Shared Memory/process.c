#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>        
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <string.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>           
sem_t semaphore;
typedef struct              //structure to be sent
{             
char message[50];     
int USRLED;
int length;
}mystruct;

void  process2(void)
{
	printf("Initializing Process 2 \n");
	mystruct get_mes;
	char *name = "Memory";
	int sm_fd;
	void *ptr;
	sm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);//open shared mem and give permission
	if(sm_fd < 0 )
       {
	printf("Error in opening");
	}

	ptr = mmap(0, sizeof(get_mes), PROT_READ | PROT_WRITE, MAP_SHARED, sm_fd, 0);                                         //use in shared memory
	sem_wait(&semaphore);

	memcpy((char *)&get_mes, ptr, sizeof(get_mes));   
	msync(ptr,sizeof(get_mes),MS_SYNC);
	sem_post(&semaphore);
	

	char obuffer[50];
	strncpy(obuffer, get_mes.message, get_mes.length);
	printf("String from process1:%s,String length:%d,USR LED:%d\n",obuffer,get_mes.length,get_mes.USRLED);    //print rcvd details
	
	mystruct sent_mes;
	char buffer[50]; 
	
	sprintf(buffer,"Hi there from Process 2");
	strncpy(sent_mes.message , buffer, strlen(buffer));
	sent_mes.length = strlen(buffer);
	sent_mes.USRLED = get_mes.USRLED;

	sem_wait(&semaphore);
	memcpy(ptr, (char *)&sent_mes, sizeof(sent_mes));
	msync(ptr,sizeof(sent_mes),MS_SYNC);
	sem_post(&semaphore);

	printf("Terminating process 2\n");	
}

void  process1(void)
{
    printf("Initializing process 1 \n"); 
	
	mystruct smessage;
	char *sm_name = "Memory";
	int sm_fd;
	void *ptr;
	sm_fd = shm_open(sm_name, O_CREAT | O_RDWR, 0666);
	ptr = mmap(0, sizeof(smessage), PROT_READ | PROT_WRITE, MAP_SHARED, sm_fd, 0);
	
	char buffer[50];
	strncpy(buffer, "Hi from Process 1@@@",20);

	strncpy(smessage.message , buffer, strlen(buffer));
	smessage.length = strlen(buffer);
	smessage.USRLED = 0;	
	
	sem_wait(&semaphore);

	memcpy(ptr, (char *)&smessage, sizeof(smessage));
	msync(ptr,sizeof(smessage),MS_SYNC);
	sem_post(&semaphore);
	sleep(0.5);
	mystruct get_mes;

	sem_wait(&semaphore);

	memcpy((char *)&get_mes,ptr, sizeof(get_mes));
	msync(ptr,sizeof(get_mes),MS_SYNC);
	sem_post(&semaphore);
	
	char obuffer[50] ;
	strncpy(obuffer, get_mes.message, get_mes.length);
	printf("String from process 2:%s,String length :%d\n, USRLED:%d\n",obuffer,get_mes.length,get_mes.USRLED);

	printf("Terminating process 1 \n");
}

int  main(void)
{
	pid_t  mythread;
	sem_init(&semaphore,1,1);    //semaphore init
	mythread = fork();            //forking to create threads
	if (mythread > 0) 
	  process2();
	else if(mythread == 0)
	  process1();
        else
	printf("Error in forking");
	sem_destroy(&semaphore);
	return 0;
}
