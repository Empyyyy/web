#include"wrapper.h"
#define MAXSIZE 2048
void err_request(int fd,char *cause,char *errnum,char *shortmsg,char *longmsg); //错误http事务
void static_html(int fd, char *filename); //打开静态页面
int main(int argc,char **argv)
{
    int server_sockert,client_socket,clientlen;
	struct sockaddr_in client_addr;
	rio_t rio;
	int port = 1024 ;
	char buf[1024],method[1024],uri[1024],verison[1024];
	server_sockert=open_listen_sock(port);
	
	while(1)
	{
		clientlen = sizeof(client_addr);
		client_socket = accept(server_sockert,(struct sockaddr*)&client_addr,&clientlen);
		if(client_socket < 0)
			perror("accept");
		static_html(client_socket,"./index.html");
		
		close(client_socket);
	}
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

void static_html(int fd,char *filename)
{
	  FILE *resource = NULL;  
    char buf[MAXSIZE];  
  
	/*丢弃原来的header*/
	read(fd,buf,MAXSIZE);
  
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

