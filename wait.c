/*************************************************************************
	> File Name: wait.c
	> Author: liangmengdi
	> Mail: llmmdd1997130@126.com
	> Created Time: 2016年07月26日 星期二 15时11分24秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
int main(int argc,char *argv[])
{
    pid_t pid;
    int exit_code;
    char *msg;
    int k;
    printf(" study how to get exit code\n");
    pid=fork();
    switch(pid)
    {
    case 0:
    msg="child process is running";
    k=5;
    exit_code = 37;
    break;
    case -1;
    perror("process creation failed");
    exit(1);
    default:
    exit_code = 0;
    break;
    }
    /*父子进程都会执行以下代码*/
    if(pid!=0) //若父进程在运行则进行挂起
    {
        int stat_val;
        pid_t child_pid;
        child_pid=wait(&stat_val);
        printf("child process has exited,pid=%d\n",child_pid);
        if(WIFEXITED(stat_val))
        {
            printf("child exited code %d\n",WEXITSTATUS(stat_val));
        }
        else
        {
            printf("exit abnormally\n");
        }
    }
    else
    {
        while(k-->0)
        {
            puts(msg);
            sleep(1);//另子进程挂起，检测父进程会不会在此期间执行
        }
    }
    exit(exit_code);
}
