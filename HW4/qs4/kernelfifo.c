#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/kthread.h>	
#include <linux/delay.h>
#include <linux/kfifo.h>
#include <linux/pid.h>
#include <linux/errno.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anay Gondhalekar");
MODULE_DESCRIPTION("Kthread demonstration between process");

#define fifo_size 16
static int Time= 7;
struct task_struct *thread1, *thread2;
static DECLARE_KFIFO(kernel_comm,struct task_struct*,fifo_size);	
module_param(Time, int, S_IRUGO | S_IWUSR);
static DEFINE_MUTEX(my_lock);	
int thread1_init(void *myptr)
{
	printk("From thread1 to thread2");

	while(!kthread_should_stop())                  //communication till kthread ends
	{	mutex_lock(&my_lock);	
		if(kfifo_put(&kernel_comm, current)==0)
		{
		printk("No space\n");
		}
                mutex_unlock(&my_lock);           //mutex lock and unlock for avoiding races
		ssleep(Time);
	}

	printk("%s end\n",__FUNCTION__);	

	return 0;
}

int thread2_init(void *myptr)
{
	struct task_struct *direct;
	printk("From thread2 to thread1");

	while(!kthread_should_stop())
	{
                mutex_lock(&my_lock);
		if(kfifo_get(&kernel_comm,&direct == 0))
		{
			printk("Empty FIFO\n");    //check if fifo is empty
		}
		else	
		{
                        
			printk("Previous PID: %d & vruntime: %llu\n",list_prev_entry(direct, tasks)->pid, list_prev_entry(direct, tasks)->se.vruntime);
			printk(KERN_INFO "Current PID: %d & vruntime: %llu\n",direct->pid, direct->se.vruntime);
			printk(KERN_INFO "Next PID: %d & vruntime: %llu\n",list_next_entry(direct, tasks)->pid, list_next_entry(direct, tasks)->se.vruntime);   //print the details rcvd
               mutex_unlock(&my_lock);	
		}
		
	}

	printk("%s end",__FUNCTION__);

	return 0;	
}

static int __init mykfifo_init(void)
{
	printk("Entering the kernelfifo kernel module");
	INIT_KFIFO(kernel_comm);                  //initialise and create threads
	kthread_run(thread1_init,NULL,"Thread1 create");       
	kthread_run(thread2_init,NULL,"Thread2 create");
	return 0;
}


static void __exit mykfifo_exit(void)
{
       int var =  kthread_stop(thread1);
	var = kthread_stop(thread2);
	printk("Exiting the kernelfifo kernel module");
}


module_init(mykfifo_init);
module_exit(mykfifo_exit);

