/*************************************************************************
	> File Name: chmod.c
	> Author: liangmengdi
	> Mail: llmmdd1997130@126.com
	> Created Time: 2016年07月18日 星期一 11时51分02秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int argc,char *argv[])
{
    int mode; //chmod（char *path,mode),mode指的是所输入八进制数的十进制形式
    int mode_u;//所有者权限
    int mode_g;//同组用户权限
    int mode_o;//其他用户权限
    char *path;//存放所修改文件权限的文件名

    /*检查参数个数的合法性*/
    if(argc<3)
    {
        printf("%s <mode num><target file>\n",argv[0]);
        exit(0);
    }
    mode=atoi(argv[1]);//atoi函数将字符串转化为整型数
    if(mode>777||mode<0)//用于判断所输入数字是否超过范围
    {
      printf("mode num error\n");
      exit(0);
    }
    mode_u=mode/100;
    mode_g=mode/10%10;
    mode_o=mode%10;
    mode=(mode_u*8*8)+(mode_g*8)+mode_o;//将所输入的十进制转化为此十进制数字所对应八进制数的十进制
    path=argv[2];//将所修改的文件名赋给path
    if(chmod(path,mode)==-1)
    {
    perror("error!\n");//将上一函数发生错误原因输出
    exit(1);//非正常退出返回1
    }

    return 0;
}
