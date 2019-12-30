#include"wrapper.h"
void accept_request(int client); //处理http事务

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

		close(client_socket);
	}
}

