#include"wrapper.h"
#define PORT 8080
#define MAXSIZE 2048

void process_run(int fd);//服务器运行的主要函数
void static_uri(char *uri, char *fileName);//获取静态页面的uri上的文件名
void static_html(int fd,char *filename);
int get_line(int sock, char *buf, int size);
void test(int fd);
int main()
{
	int listen_sock,conn_sock,clientlen;
	struct sockaddr_in clientaddr;
	listen_sock = open_listen_sock(PORT);
	while(1)
	{
		clientlen = sizeof(clientaddr);
		conn_sock = accept(listen_sock,(struct sockaddr *)&clientaddr,&clientlen);
		if(conn_sock < 0)
		{
			perror("accept");
			exit(1);
		}
		/* test(conn_sock); */
		process_run(conn_sock);
		close(conn_sock);
	}
}

void test(int fd)
{
	char *file = "./index.html";
	printf("%s\n",file);
	static_html(fd,file);
}


void process_run(int fd)
{
	int dynamic_flag = 0;//动态态页面的标志
	struct stat sbuf;//用来描述一个linux系统文件系统中的文件属性的结构。
	/*可以有两种方法来获取一个文件的属性：
		1、通过路径：
		int stat(const char *path, struct stat *struct_stat);
		int lstat(const char *path,struct stat *struct_stat);
		两个函数的第一个参数都是文件的路径，第二个参数是struct stat的指针。返回值为0，表示成功执行。
	*/
	//缓冲区，方法，uri，版本，文件名
	char buf[MAXSIZE], method[MAXSIZE], uri[MAXSIZE], version[MAXSIZE];
	char fileName[MAXSIZE];
	rio_t rio;
	char *query_string;
	
	rio_readinitb(&rio,fd);
	rio_readlineb(&rio,buf,MAXSIZE);
	sscanf(buf,"%s%s%s",method,uri,version);
	
    rio_readlineb(&rio, buf, MAXSIZE);
    while(strcmp(buf, "\r\n")) {
	    printf("%s", buf);
	    rio_readlineb(&rio, buf, MAXSIZE);
    }
	
	//判断方法，不是GET也不是POST则直接跳出
	if(strcasecmp(method, "GET") && strcasecmp(method, "	POST"))
	{
		//错误处理，方法错误
		printf("12\n");
		return;
	}
	if(strcasecmp(method, "POST") == 0)
		dynamic_flag = 1;
	if(strcasecmp(method, "GET") == 0)
	{
	
		query_string = uri;
		//遍历uri，找出是否有?后的条目
		while((*query_string != '?') && (*query_string != '\0'))
			query_string++;
		//uri如果有?
		
		if(*query_string == '?')
		{
			//动态网页
			dynamic_flag = 1;
			*query_string = '\0';
			query_string++;
		}
		else//静态页面
		{
 			static_uri(uri,fileName);
				printf("fileName:%s\n",fileName); 
			 static_html(fd,fileName);
			
		}
	}
	

	
	
	
}
void static_html(int fd,char *filename)
{
	  FILE *resource = NULL;  
    char buf[MAXSIZE]; 
	int numchars = 1;

    /*打开 filename 的文件*/  
    resource = fopen(filename,"r"); 	
    if (resource == NULL)  
       printf("cuowu\n");  
    else  
    {  
        /*写 HTTP header */  
    strcpy(buf, "HTTP/1.0 200 OK\r\n");  
    send(fd, buf, strlen(buf), 0);  
    /*服务器信息*/  
    strcpy(buf,"sadasdsasaddsa");  
    send(fd, buf, strlen(buf), 0);  
    sprintf(buf, "Content-Type: text/html\r\n");  
    send(fd, buf, strlen(buf), 0);  
    strcpy(buf, "\r\n");  
    send(fd, buf, strlen(buf), 0);  
	
	
        /*复制文件*/  
     fgets(buf, sizeof(buf), resource);  
    while (!feof(resource))  
    {  
        send(fd, buf, strlen(buf), 0);  
        fgets(buf, sizeof(buf), resource);  
    }  
    }  
    fclose(resource);  
}


void static_uri(char *uri, char *fileName)
{
	char *p;
	strcpy(fileName,".");
	strcat(fileName,uri);
	if(uri[strlen(uri)-1] == '/')
		strcat(fileName,"index.html");
}