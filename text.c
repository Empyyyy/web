#include"wrapper.h"


#define PORT 8080


void con(int client_socket);
int main(){
  
	int server_socket = open_listen_sock(1024);
	int client_socket= -1;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	char buf[1024],method[1024],uri[1024],verison[1024];
	rio_t rio;
/*             创建服务器套接字
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
 	server_addr.sin_port = htons(1024);
	bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr));
	listen(server_socket,5); */

	
	int len = sizeof(client_addr);
	while(1){
	client_socket = accept(server_socket,(struct sockaddr*)&client_addr,&len);
/*     rio_readinitb(&rio,client_socket);
	rio_readlineb(&rio,buf,1024);
	
	sscanf(buf,"%s%s%s",method,uri,verison);
	printf("%s %s %s\n",method,uri,verison);

		char status[] = "HTTP/1.0 200 OK\r\n";
        char header[] = "Server: DWBServer\r\nContent-Type: text/html;charset=utf-8\r\n\r\n";
        char body[] = "<html><head><title>C语言构建小型Web服务器</title></head><body><h2>欢迎</h2><p>Hello，World</p></body></html>";
   	
	send(client_socket,status,sizeof(status),0);
    send(client_socket, header, sizeof(header),0);
    send(client_socket, body, sizeof(body),0); */
	con(client_socket);
    close(client_socket);
       }
   
	 close(server_socket);
    return 0;

}
void con(int client_socket)
{
	char buf[1024],method[1024],uri[1024],verison[1024];
	rio_t rio;

	rio_readinitb(&rio,client_socket);
	rio_readlineb(&rio,buf,1024);
	
	sscanf(buf,"%s%s%s",method,uri,verison);
	printf("%s %s %s\n",method,uri,verison);

}
