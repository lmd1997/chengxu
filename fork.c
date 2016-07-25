/*************************************************************************
	> File Name: fork.c
	> Author: liangmengdi
	> Mail: llmmdd1997130@126.com
	> Created Time: 2016年07月25日 星期一 14时05分21秒
 ************************************************************************/

/*此函数用来判断fork的返回值有两个*/

#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>

int main(int argc,char *argv[])
{
    pid_t pid;
    char *ptr;
    int k;
    pid=fork();
    switch(pid)
    {
        case 0:
        ptr="children";
        k=3;
        break;
        case -1:
        ptr="fail";
        break;
        default:
        ptr="parent";
        k=5;
        break;

    }
    while(k>0)
    {
        puts(ptr);
       // sleep(1);
        k--;
    }

    return 0;
}
