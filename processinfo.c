#include <linux/kernel.h>
#include <linux/init.h>
#include<linux/file.h>
#include<linux/fs.h>
#include<linux/fcntl.h>
#include<asm/uaccess.h>
#include<linux/sched.h>
#include<linux/syscalls.h>
#include<uapi/asm-generic/errno-base.h>
#include "processinfo.h"
asmlinkage long sys_processinfo(int pid, char* f)
{
struct task_struct *task;
struct file *file;
char data[500];
char temp[500];
int fd;
if(pid>32768 || pid<=0)
return -EINVAL;
mm_segment_t old_fs= get_fs();
set_fs(KERNEL_DS);
fd= sys_open(f, O_WRONLY|O_CREAT, 0644);
for_each_process(task)
{
if((int)task->pid==pid)
{
printk("Process: %s\n PID NUMBER: %ld\n Process State: %ld\n Priority: %ld\n RT_Priority: %ld\n \
Static Priority: %ld\n Normal Priority: %ld\n",task->comm,(long)task->pid,(long)task->state,
(long)task->prio,(long)task->rt_priority,(long)task->static_prio,(long)task->normal_prio);

strcpy(data,"Process: ");
strcat(data,task->comm);
strcat(data,"\n");
strcat(data,"PID NUMBER: ");
sprintf(temp,"%ld\n",(long)task->pid);
strcat(data,temp);
strcat(data,"Process state: ");
sprintf(temp,"%ld\n",(long)task->state);
strcat(data,temp);
strcat(data,"Priority: ");
sprintf(temp,"%ld\n",(long)task->prio);
strcat(data,temp);
strcat(data,"RT_Priority: ");
sprintf(temp,"%ld\n",(long)task->rt_priority);
strcat(data,temp);
strcat(data,"Static_Priority: ");
sprintf(temp,"%ld\n",(long)task->static_prio);
strcat(data,temp);
strcat(data,"Normal Priority: ");
sprintf(temp,"%ld\n",(long)task->normal_prio);
strcat(data,temp);
if(task->parent)
{
printk("Parent Process: %s ",task->parent->comm);
strcat(data,"Parent Process: ");
strcat(data,task->parent->comm);
strcat(data,"\n");
}

if(fd<0)
return -EISDIR;
file= fget(fd);
file->f_op->write(file,data,strlen(data), &file->f_pos);
}
}
set_fs(old_fs);
return 0;
}
