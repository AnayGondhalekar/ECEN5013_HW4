#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>       
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>          


typedef struct              //structure to be sent
{             
char message[50];     
int USRLED;
int length;
}mystruct;

int process2()
{
	printf("Initialising Process 2\n");
	
	char buffer1[50]; 
	mqd_t queue2;     //making a queue
	
	queue2 = mq_open("/my_queue",O_RDWR | O_CREAT, 0666, NULL);
	if(queue2 < 0) {
		printf("Can't open\n");      //opening the queue
		return -1;
	}
	
	mystruct given,deliver;
	struct mq_attr *pact;
	pact = malloc(sizeof(struct mq_attr));
	mq_getattr(queue2,pact);

        mq_receive(queue2,(char *)&given,pact->mq_msgsize,NULL) ;
		char buffer[50];
		strncpy(buffer, given.message, given.length);
		printf("String received from process1:%s,String length:%d,USR LED Sts:%d\n",buffer,given.length,given.USRLED);
	
	sprintf(buffer1,"Hi there from process 2");
	strncpy(deliver.message , buffer1, strlen(buffer1));
	deliver.length = strlen(buffer1);
	deliver.USRLED = given.USRLED;

	mq_send(queue2,(char *)&deliver,sizeof(deliver),1);//sending to process1

	mq_close(queue2);
	mq_unlink("/my_queue");

	printf("Terminating process 2\n");
	return 0;
}

int  process1()
{
    printf("Initialising Process 1 \n"); 
	
	mqd_t mq1;
	
	mystruct sample_message;
	char buffer1[50] ={0};
	strncpy(buffer1, "Hi I am Process 1",17);
	strncpy(sample_message.message , buffer1, strlen(buffer1));
	sample_message.length = strlen(buffer1);
	sample_message.USRLED = 1;	
	
	mq1 = mq_open("/my_queue",O_RDWR | O_CREAT, 0666, NULL);
	if(mq1 == -1) {
		
		printf("Can't open\n"); //opening queue 1
		return -1;
	}

	if( mq_send(mq1,(char *)&sample_message,sizeof(sample_message),1)== -1){
		printf("Sending failed\n");
	}

	mystruct given;
	struct mq_attr *pact;
	pact = malloc(sizeof(struct mq_attr));
	mq_getattr(mq1,pact);

	mq_receive(mq1,(char *)&given,pact->mq_msgsize,NULL);
		char buffer[50] = {0};
		strncpy(buffer, given.message, given.length);
		printf("String received from process2:%s,String length:%d,USR LED  Sts:%d\n",buffer,given.length,given.USRLED); //reading recvd struct
	

	mq_close(mq1);
	mq_unlink("/my_queue");
	
	printf("Terminating process 1\n");
        return 0;
}

int  main()
{
	pid_t mythread;       //creating 2 processes by forking
        mythread= fork();
	if (mythread > 0) 
	  process2();
	else if(mythread == 0)
	  process1();
        else
        printf("Error in forking");
	return 0;
}


