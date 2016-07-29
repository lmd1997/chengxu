/*************************************************************************
	> File Name: shell.c
	> Author: liangmengdi
	> Mail: llmmdd1997130@126.com
	> Created Time: 2016年07月27日 星期三 10时54分30秒
 ************************************************************************/

#include<pwd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<dirent.h>

#define normal 0                              //含有一般命令
#define out_redirect 1                        //含有重定向输出
#define in_redirect 2                         //含有重定向输入
#define have_pipe 3                           //含有管道

void printf_prompt(char*);                         //输出shell提示符
void get_input(char*);                        //获取所输入的命令,将所获取命令存入buf中
void explain_input(char *,int *,char a[100][256]);  //对输入的命令进行解析
void do_cmd(int, char a[100][256]);                 //执行所输入的命令
int find_command(char*);

struct passwd *pw;




int main(int argc,char *argv[])
{
    char a[100];
    char *buf=NULL;
    int argcount=0;                           //记录命令行参数的个数
    char arglist [100][256];                  //记录每个参数的具体内容
    int i;
    buf=(char*)malloc(256);                   //为buf申请存储空间
    if(buf ==NULL){
    perror("malloc failed");
    exit(-1);
    }
    while(1)                                  //开始申请空间
    {
        memset(buf,0,256);                    //将buf中的全部字节设为0,初始化内存
        printf_prompt(a);                     //开始先输出shell提示符
        get_input(buf);                       //获取命令至buf中
        while(buf[0] == '\n'){
            printf_prompt(a);
            get_input(buf);
        }
        
        if(strcmp(buf,"exit\n") == 0 || strcmp(buf,"logout\n") == 0) //若输入exit,logout则退出
        break;
        for(i=0;i<100;i++)                    //清空上一次操作中 arglist中的数据，另第一个字符结束
        {
            arglist[i][0] = '\0';
        }
        argcount=0;                           //运行前先将上一次操作数据清空
        explain_input(buf,&argcount,arglist); //解析所输入的命令
        do_cmd(argcount,arglist);             //执行相应的命令    
    }
    while(buf!=NULL)                          //每次运行完命令，都将buf清空
    {
        free(buf);
        buf=NULL;
    }        
     exit(0);
}
void printf_prompt(char*a)
{
    uid_t uid;
    uid  = getuid();
    pw = getpwuid(uid);                       //获取当前用户id

    getcwd(a,100);                            //获取当前路径信息
    printf("\033[;32m%s\033[0m\033[;32m@\033[0m\033[;32m%s\033[0m:\033[;34m~\033[0m$ ",pw->pw_name,a);
}

/*获取命令保存至buf中*/
void get_input(char*buf)
{
    int len=0;
    int ch;
    ch=getchar();                             //获取命令的第一个字符
    while(len<256 && ch!='\n')                //将命令全部存入buf[]中
    {
        buf[len++] = ch;
        ch=getchar();                         //不断往后获取命令字节
    }
    if(len == 256)                //若输入的命令过长退出
    {
        printf("您输入的命令过长\n");
        exit(-1);
    }
    buf[len] ='\n';                           //确保命令的结尾为回车
    len++;
    buf[len]='\0';                            //结束命令
}

/*解析命令行,将结果保存至arglist与argcount中*/
void explain_input(char *buf,int *argcount,char arglist[100][256])
{
   char *p=buf;                               //将buf中保存的命令存到p,q中
   char *q=buf;
   int number = 0;                            //一个arglist[i]中（一个字符串）字符的个数
    while(1)
    {
        if(p[0] == '\n')                       // 若命令输入完毕则跳出此循环
        break;
        if(p[0] == ' ')                        //若前面一直是空格，则一直跳过空格取到后面的命令
        p++; 
        else
        {
            q = p;                            //将除过空格的命令赋给q
            number = 0;
            while(q[0]!='\n' && q[0]!=' ')    //若指到了命令间的空格或末尾则跳出
            {
                number++;
                q++;
            }
            strncpy(arglist[*argcount],p,number+1); //p指向的是命令中一个字符串的首字符,number+1将空格赋给arglist以来存放'\0'
            arglist[*argcount][number] = '\0';
            *argcount = *argcount+1;
            p=q;                                    //若p指到了命令中的空格或命令的末尾，则将值赋给q，让q跳过空格或跳出循环  
        }
    }     
}


void do_cmd(int argcount,char arglist[100][256])
{
    int flag = 0;
    int how = 0;
    int background = 0;
    int status;
    int i;
    int fd;
    char* arg[argcount + 1];
    char* argnext[argcount + 1];
    char *file;
    char home[100];
    pid_t pid;

    /*将所有命令取出放作用至agr[]中*/
    for(i=0;i<argcount;i++)
    {
        arg[i] = (char*) arglist[i];
    }
    arg[argcount] = NULL;                     
     
    /*判断是否为后台运行命令*/
    for(i=0;i<argcount;i++)
    {
       if(strncmp(arg[i],"&",1) == 0)
      {
          if(i=argcount - 1)
          {
              background = 1;
              arg[argcount - 1] = NULL;
              break;
          }
       else
        {
          printf("wrong command\n");
          return;
        }
      }
    }
    for(i=0;arg[i] != NULL;i++)
    {
        if(strcmp(arg[i],">") == 0)
        {
            flag++;
            how=out_redirect;
            if(arg[i+1] == NULL)                       //若重定向输出符后面为空，则输入命令有误，此时令flag不为一，在下面跳出循环
            flag++;
        }
        
        if(strcmp(arg[i],"<") == 0)
        {
            flag++;
            how=in_redirect;
            if(i == 0)                                 //若重定向输入符前面为空，则命令有误
            flag++;
        }
        if(strcmp(arg[i],"|") == 0)
        {
            flag++;
            how=have_pipe;
            if(arg[i+1] == NULL)                       //管道命令符在最后面，命令有误
            flag++;
            if(i == 0)                                 //管道命令符在最前面，命令有误
            flag++;
        }
    }
    if(flag > 1)
    {
        printf("wrong command\n");
        return;
    }
    if(how == out_redirect)
    {
        for(i=0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],">") == 0)
            {
                file = arg[i+1];                                //将>后的文件名保存至file中
                arg[i] = NULL;                                  //>后面的文件名不需要作为参数
            }
        }
    }
    if(how == in_redirect)
    {
        for(i=0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],"<") == 0)
            {
                file = arg[i+1];                                //将<后的文件名保存至file中          
                arg[i] =NULL;                                   //<后面的文件名不需要作为参数
            }
        }
    }
    if(how == have_pipe)
    {
        for(i=0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],"|") == 0)
            {
                arg[i] = NULL;
                int j;
                for(j=i+1;arg[j]!=NULL;j++)                     // 将管道符后面的命令存入argnext中
                {
                    argnext[j-i-1] = arg[j];
                }
                argnext[j-i-1] = arg[j];
                break;
                }
            }
        }
        

        if(strcmp(arg[0],"cd")  == 0){
            sprintf(home,"/home/%s/",pw->pw_name);              //若cd后面没有目录则返回目录
            if(argcount == 1){
                chdir(home);
            }
            else if(strcmp(arg[1],"~") == 0)                    //若cd后面为～则返回主目录
            {
                chdir(home);
            }
            else
                chdir(arg[1]);
            goto loop;
        }



        if((pid=fork()) < 0)                                        //创建进程
        {
            printf("fork error\n");
            return;
        }
        switch(how)
        {
            case 0:
            if(pid == 0)
            {
                
                if(!(find_command(arg[0])))
                {
                    printf("%s:command not foud",arg[i]);
                    exit(0);
                }
                execvp(arg[0],arg);
                exit(0);
            }
            break;
            case 1:
            if(pid == 0)
            {
                if(!(find_command(arg[0])))
                {
                    printf("%s: command not found",arg[i]);
                    exit(0);
                }
                fd=open(file,O_RDWR|O_CREAT|O_TRUNC,0644);
                dup2(fd,1);                                         //将输出显示屏替换为文件fp
                execvp(arg[0],arg);
                exit(0);
            }
            break;
            case 2:
            if(pid == 0)
            {
                if(!(find_command(arg[0])))
                {
                    printf("%s:command not found",arg[i]);
                    exit(0);
                }
                fd=open(file,O_RDONLY);
                dup2(fd,0);                                        // 将输入键盘替换为文件fp  
                execvp(arg[0],arg);
                exit(0);
            }
            break;
            case 3:
            if(pid == 0)
            {
                int pid2;
                int status2;
                int fd2;

                if((pid2 =fork()) < 0)
                {
                    printf("fork2 error\n");
                    return;
                }
                else if(pid2 == 0)
                {
                    if(!(find_command(arg[0])))
                    {
                        printf("%s :command not found\n",arg[0]);
                        
                        exit(0);
                    }
                    fd2 = open("/tmp/youdonotknowfile",O_WRONLY|O_CREAT|O_TRUNC,0644);  //创建临时文件保存管道符前面的操作
                    dup2(fd2,1);              //将输出显示屏替换为fd2
                    
                    execvp(arg[0],arg);                                                 
                    exit(0);
                }

                if(waitpid(pid2,&status2,0) == -1)                                      //确保管道符前的操作先执行完成
                printf("please wait process error\n");

                if(!(find_command(arg[0])))
                {
                    printf("%s :command not found\n",arg[0]);
                    exit(0);
                }
                fd2 = open("/tmp/youdonotknowfile",O_RDONLY,0644);      //将键盘输入替换为fd2
                dup2(fd2,0);
                execvp(argnext[0],argnext);
                if(remove("/tmp/youdonotknowfile"))
                printf("remove error\n");
                exit(0);
        }
        break;
        default:
            break;
        }
        
        /*后台运行*/
        if(background == 1)
        {
            printf(" process id :%d",pid);                                 //对父进程进行操作，退出其父进程使其子进程独立，成为后台运行进程
            return;
        }
        if(waitpid(pid, &status, 0) == -1)
        printf("wait for child process error\n");

        loop : ;

}

int find_command (char* command)
{
    DIR* dp;
    struct dirent* dirp;
    char* path[]={"./", "/bin", "/usr/bin",NULL};
    if(strncmp(command,"./",2) == 0)
    command = command+2;
    int i = 0;
    while(path[i] != NULL)
    {
        if((dp=opendir(path[i])) == NULL)
        printf("can not open /bin\n");
        while((dirp = readdir(dp)) !=NULL)
        {
            if(strcmp(dirp->d_name,command) == 0)
            {
                closedir(dp);
                return 1;
            }
        }
        closedir(dp);
        i++;
    }
    return 0;
}

























