#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define SERVPORT 3333
#define MAXDATASIZE 100
int main(int argc,char *argv[])
{
	int sockfd,recvbytes;//声明客户端套接字标识，接收消息标识
	char buf[MAXDATASIZE];//建立消息缓冲区
	struct hostent *host;
	struct sockaddr_in serv_addr;//声明一个套接字
	while(1){
		if(argc<2)
		{
			fprintf(stderr,"Please enter the ip address:\n");
			exit(1);
		}

		if((host=gethostbyname(argv[1]))==NULL)//由主机名称取得网络数据
		{
			herror("error in gethostbyname");//打印网络错误原因信息字符串
			exit(1);
		}
		//代码1-------start---------创建客户端socket，将返回值赋值给sockfd，并判断是否创建成功
		if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
		{
			perror("error in create socket");
			exit(1);
		}
		//代码1--------end------------
		//初始化客户端套接字结构体中的变量
		serv_addr.sin_family=AF_INET;
		serv_addr.sin_port=htons(SERVPORT);
		serv_addr.sin_addr=*((struct in_addr *)host->h_addr);
		bzero(&(serv_addr.sin_zero),8);
		//代码2-------start---------客户端向服务端发起一次连接,并判断是否连接成功
		if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr))==-1)
		{
			perror("error in connect");
			exit(1);
		}
		//代码2--------end------------
		//代码3-------start---------从服务器端接收连接消息，将结果存于接收消息标识recvbytes，并是否接收成功
		if((recvbytes=recv(sockfd,buf,MAXDATASIZE,0))==-1)
		{
			perror("error in recive");
			exit(1);
		}
		//代码3--------end------------
		//字符转换
		buf[recvbytes]='\0';
		printf("Recive:%s",buf);
		scanf("%s",buf);
		buf[strlen(buf)]='\0';
		//代码4-------start---------客户端发送计算类型和参数，并判断是否发送成功
		if(send(sockfd,buf,MAXDATASIZE,0)==-1)
		{
			perror("error in send");
			exit(1);
		}
		//代码4--------end------------
		//接收计算结果
		int result=0;
		if((recvbytes=recv(sockfd,&result,sizeof(result),0))==-1)
		{
			perror("error in recive");
			exit(1);
		}
		if(buf[0]=='q') break;
		printf("The answer is :%d\n",result);
		close(sockfd);//关闭客户端套接字
	}
}
