/*************************************************************************
	> File Name: 文件创建.c
	> Author: liangmengdi
	> Mail: llmmdd1997130@126.com
	> Created Time: 2016年07月18日 星期一 17时08分00秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
int main()
{
    int fd;
    if((fd=open("kkk.c",O_CREAT|O_EXCL,S_IRUSR|S_IWUSR))==-1)
    {
       exit(1);
    }
    else
    printf("create file success!\n");
    close(fd);
    return 0;
}
