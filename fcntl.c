/*************************************************************************
	> File Name: fcntl.c
	> Author: liangmengdi
	> Mail: llmmdd1997130@126.com
	> Created Time: 2016年07月19日 星期二 10时57分35秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
void error(char*errstring,int line)
{
    fprintf(stderr,"line:%d",line);//将显示屏看作是文件，将数据显示到显示屏相当于将数据写入文件，故用fprintf
    perror(errstring);//将错误语句输出且后面会跟上line
    exit(1);//非正常退出
}
int main(int argc,char *argv[])
{
    int line;
    int fd;
    int ret;
    int mode;
    if((fd=open("ceshi.c",O_CREAT|O_EXCL|O_RDONLY,S_IRUSR)) == -1)//创建文件
    {
        error("creat",line);//_LINE_预编译器内置宏，记录出错语句所在位置
    }
    //设置文件打开方式
    if((ret=fcntl(fd,F_SETFL,O_APPEND)) == -1)
    {
        error("open",line);
    }
    //获取文件打开方式
    if((ret=fcntl(fd,F_GETFL,0)) == -1)
    {
        error("get",line);
    }
    mode=ret&O_ACCMODE; //进行或位运算，得出ret的后两位；（00：可读，01：可写，10：可读可写）
    if(mode==O_RDONLY)
    {
        printf("you can only read\n");
    }
    else if(mode == O_WRONLY)
    {
        printf("you can only write\n");
    }
    else if(mode == O_RDWR)
    {
        printf("you can read and write\n");
    }
    return 0;
}

