#include<sys/socket.h>
#include<sys/un.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>

#define PORT 8080


int main(){
  
	int server_socket = socket(AF_INET,SOCK_STREAM,0);
	int client_socket= -1;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
             /*创建服务器套接字*/
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
 	server_addr.sin_port = htons(PORT);
	bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr));
	listen(server_socket,5);


	int len = sizeof(client_addr);
	while(1){
	client_socket = accept(server_socket,(struct sockaddr*)&client_addr,&len);
	char buf[1024];
	read(client_socket,buf,1024);
	printf("%s",buf);

	char status[] = "HTTP/1.0 200 OK\r\n";
        char header[] = "Server: DWBServer\r\nContent-Type: text/html;charset=utf-8\r\n\r\n";
        char body[] = "<html><head><title>C语言构建小型Web服务器</title></head><body><h2>欢迎</h2><p>Hello，World</p></body></html>";
   	
	send(client_socket,status,sizeof(status),0);
            send(client_socket, header, sizeof(header),0);
            send(client_socket, body, sizeof(body),0);
       }
    close(client_socket);
    close(server_socket);

    return 0;

}
