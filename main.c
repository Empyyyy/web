#include"wrapper.h"
void accept_request(int client); //处理http事务

int main(int argc,char **argv)
{
    int server_sockert,client_socket,clientlen;
	struct sockaddr_in client_addr;
	rio_t rio;
	port = 1024 ;
	
	server_sockert=open_listen_sock(port);
	
	while(1)
	{
		clientlen = sizeof(client_addr);
		client_socket = accept(server_sockert,(struct sockaddr*)&client_addr,&clientlen);
		
		close(client_socket);
	}
}

void accept_request(int client)
{
     int flag ;//判断是动态还是静态
     	 
}