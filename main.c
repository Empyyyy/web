#include"wrapper.h"
void err_request(int fd,char *cause,char *errnum,char *shortmsg,char *longmsg); //错误http事务

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
		
		err_request(client_socket,"11","404","not find","meiyou faxian");
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

