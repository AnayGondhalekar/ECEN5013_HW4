#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/errno.h>
#include <linux/stat.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anay Gondhalekar");
MODULE_DESCRIPTION("Evaluate the process tree and travel backwards in process lineage till any process is not found and printing the metrics");//module units

static int pid = -2;                  //set a pid

module_param(pid,int,S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(pid,"Process ID of the process"); 

static int __init process_tree_init(void)
{
        struct task_struct *ptr;
        int count;
	struct list_head *mylist;
	printk("Entering the process tree.");        //entered the module
        if(pid < 0)                                 //to enter the current process
        {
                printk("Process Tree of Current process.\n");
                ptr = current;
        }
        else 
        {       
                struct pid *anay = find_get_pid(pid);       //to enter a give pid process
		printk("Process Tree of %d",pid);
                ptr = pid_task(anay, PIDTYPE_PID);
        }
    /*int state = ptr->state;
    char c[20];
    if(state == 0)
	{
	c="Running";
	}
    else if(state == 0)
	{
	c="Stopped";
	}
    else { c = "Unknown";}*/
        count =0;
	list_for_each(mylist,&ptr->children)    //count number of children of the process     
        {
	count++;                                                  
        }     

    printk("Thread name: %s\nProcess ID: %d\nProcess Status: %ld\nNumber of Children: %u\n Nice Value: %d\n", ptr->comm, ptr->pid, ptr->state, count,  task_nice(ptr));                  //print required stats
    while(ptr->pid != 0)          //to continuosly check for a parent till it reaches a deadend
    {
        ptr = ptr->parent;
        
        count =0;
	list_for_each(mylist,&ptr->children) //count children
        {
	count++;                                                  
        } 
        printk("Thread name: %s\n Process ID: %d\n Process Status: %ld\n Children: %u\n Nice Value: %d\n", ptr->comm, ptr->pid,ptr->state, count,  task_nice(ptr));                  //print required stats

    }
    return 0;
}


static void __exit process_tree_exit(void)
{
        printk("Exiting the process tree\n"); //exit the kernel module
}


module_init(process_tree_init);
module_exit(process_tree_exit);


