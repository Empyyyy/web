#include"wrapper.h"
#define PORT 8080
#define MAXSIZE 2048

void * process_run(void *v);//服务器运行的主要函数
void static_uri(char *uri, char *fileName);//获取静态页面的uri上的文件名
void static_html(int fd,char *filename);//静态页面处理
void err_request(int fd,char *cause,char *errnum,char *shortmsg,char *longmsg); //错误http事务
void dynamic_html(int fd, char *filename, char *cgiargs);//动态处理页面
void test(int fd);//测试



int main()
{
	int listen_sock,*conn_sock,clientlen;
	struct sockaddr_in clientaddr;
	listen_sock = open_listen_sock(PORT);
	pthread_t pid;
	clientlen = sizeof(clientaddr);
	while(1)
	{
		conn_sock = malloc(sizeof(int));
		*conn_sock = accept(listen_sock,(struct sockaddr *)&clientaddr,&clientlen);
		if(conn_sock < 0)
		{
			perror("accept");
			exit(1);
		}
		/* test(conn_sock); */
		pthread_create(&pid,NULL,process_run,conn_sock);
	}
}

void test(int fd)
{
	char *file = "./index.html";
	printf("%s\n",file);
	static_html(fd,file);
}


void err_request(int fd,char *cause,char *errnum,char *shortmsg,char *longmsg)
{
		char buf[MAXLINE],body[MAXBUF];
		
		    /* Build the HTTP response body */
    sprintf(body, "<html><title>error request</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em> small Web server</em>\r\n", body);

    /* send the HTTP response */ 
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html \r\n");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    rio_writen(fd, buf, strlen(buf));
    rio_writen(fd, body, strlen(body));	
}

void * process_run(void *v)
{
	int fd =  *((int *)v);
	pthread_detach(pthread_self());
	free(v);
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
	char cgiargs[MAXSIZE];
	
	rio_readinitb(&rio,fd);
	rio_readlineb(&rio,buf,MAXSIZE);
	sscanf(buf,"%s%s%s",method,uri,version);
	
    rio_readlineb(&rio, buf, MAXSIZE);
    while(strcmp(buf, "\r\n")) {
	    printf("%s", buf);
	    rio_readlineb(&rio, buf, MAXSIZE);
    }
	
	//判断方法，不是GET也不是POST则直接跳出
	if(strcasecmp(method, "GET") && strcasecmp(method, "POST"))
	{
		//错误处理，方法错误
	
		return NULL;
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
			static_uri(uri,fileName);
			dynamic_html(fd,fileName,query_string);
		}
		else//静态页面
		{
 			static_uri(uri,fileName);
			 static_html(fd,fileName);
			
		}
	}
		close(fd);
		return NULL;
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
    strcpy(buf,"small web ");  
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




void dynamic_html(int fd, char *filename, char *cgiargs) 
{
    char buf[MAXLINE], *emptylist[] = { NULL };
    int pfd[2];

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: weblet Web Server\r\n");
    rio_writen(fd, buf, strlen(buf));
 
    pipe(pfd);
    if (fork() == 0) {             /* child */
	close(pfd[1]);
        dup2(pfd[0],STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);         /* Redirect stdout to client */
	execve(filename, emptylist, environ);    /* Run CGI program */
    }

    close(pfd[0]);
    write(pfd[1], cgiargs, strlen(cgiargs)+1);
    wait(NULL);                          /* Parent waits for and reaps child */
    close(pfd[1]);
}