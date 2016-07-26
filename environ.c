/*************************************************************************
	> File Name: environ.c
	> Author: liangmengdi
	> Mail: llmmdd1997130@126.com
	> Created Time: 2016年07月26日 星期二 09时55分46秒
 ************************************************************************/

#include<stdio.h>
#include<malloc.h>
  extern char **environ;
int main(int argc,char *argv[])
{
    int i;
    printf("Argument:\n");
    for(i=0;i<argc;i++)
    {
        printf("argv[%d] is %s\n",i,argv[i]);
    }
    printf("Enviroment:\n");
    for(i=0;environ!=NULL;i++)
    printf("%s\n",environ[i]);

    return 0;
}
