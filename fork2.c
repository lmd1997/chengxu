/*************************************************************************
	> File Name: fork2.c
	> Author: liangmengdi
	> Mail: llmmdd1997130@126.com
	> Created Time: 2016年07月25日 星期一 15时54分12秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main(int argc,char *argv[])
{
    pid_t pid;
    pid=fork();
    switch(pid)
    {
        case 0:
        while (1)
        {
            printf("子进程 background process,PID:%d parent PID:%d\n",getpid(),getppid());
            sleep(3);
        }
        case -1:
        printf("fail\n");
        exit(-1);
        default:
        printf("父进程 parent process,parent PID :%d\n",getpid());
        exit(0);
    }

    return 0;
}
