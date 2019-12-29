#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int socket(int *listen,struct sockaddr_in *server_add)
{
	if((*listen = socket(AF_INET, SOCK_STREAM, 0)) == -1)    
    {
       printf("创建套接字失败\n");
        return -1;
    }
	
	int opt = SO_REUSEADDR;
    if(setsockopt(*listen, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
         printf("设置套接口选项值\n");
        return -1;
    }
	/*初始化套接字的参数*/
	server_add->sin_family = AF_INET;
    server_add->sin_port = htons(8080);
    server_add->sin_addr.s_addr = htonl(INADDR_ANY);
	
	
    if(bind(*listen, (struct sockaddr *)server_addr, sizeof(struct sockaddr_in)) == -1)
    {
         printf("绑定套接字失败\n");
        return -1;
    }


    if(listen(*listen, 25) == -1)
    {
        perror("监听失败\n");
        return -1;
    }
	return 0;

}