#include "gsc_socket.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#if COMPILE_SOCKET == 1

void gsc_socket_init()
{
	printf("gsc_socket_init()\n");

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
		printf("gsc_socket_init() failed to create socket\n");
        stackError("gsc_socket_init() failed to create socket");
        stackPushInt(-1);
        return;
    }
    stackPushInt(sockfd);
}

void gsc_socket_bind()
{
	printf("gsc_socket_bind()\n");

    int sockfd, port;
    char *ip;

    if (!stackGetParams("isi", &sockfd, &ip, &port))
    {
		printf("gsc_socket_bind() one or more arguments is undefined or has a wrong type\n");
        stackError("gsc_socket_bind() one or more arguments is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_aton(ip, &addr.sin_addr) == 0)
    {
		printf("gsc_socket_bind() invalid IP address\n");
        stackError("gsc_socket_bind() invalid IP address");
        stackPushInt(0);
        return;
    }

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
		printf("gsc_socket_bind() failed to bind\n");
        stackError("gsc_socket_bind() failed to bind");
        stackPushInt(0);
    }
    else
    {
        stackPushInt(1);
    }
}

void gsc_socket_connect()
{
	printf("gsc_socket_connect()\n");

    int sockfd;
    char *ip;
    int port;

    if (!stackGetParams("isi", &sockfd, &ip, &port))
    {
		printf("gsc_socket_connect() one or more arguments is undefined or has a wrong type\n");
        stackError("gsc_socket_connect() one or more arguments is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    struct sockaddr_in remote_address;
    memset(&remote_address, 0, sizeof(remote_address));

    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &remote_address.sin_addr) <= 0)
    {
		printf("gsc_socket_connect() invalid address or address not supported\n");
        stackError("gsc_socket_connect() invalid address or address not supported");
        stackPushUndefined();
        return;
    }

    stackPushInt(connect(sockfd, (struct sockaddr *)&remote_address, sizeof(remote_address)) < 0);
}

void gsc_socket_send()
{
	printf("gsc_socket_send()\n");

    int sockfd;
    char *buffer;

    if (!stackGetParams("is", &sockfd, &buffer))
    {
		printf("gsc_socket_send() one or more arguments is undefined or has a wrong type\n");
        stackError("gsc_socket_send() one or more arguments is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    ssize_t bytesSent = send(sockfd, buffer, strlen(buffer), 0);
    if (bytesSent < 0)
    {
		printf("gsc_socket_send() failed to send data\n");
        stackError("gsc_socket_send() failed to send data");
        stackPushInt(0);
    }
    else
    {
        stackPushInt((int)bytesSent);
    }
}

void gsc_socket_receive()
{
	printf("gsc_socket_receive()\n");

    int sockfd, bufsize;

    if (!stackGetParams("ii", &sockfd, &bufsize))
    {
		printf("gsc_socket_receive() one or more arguments is undefined or has a wrong type\n");
        stackError("gsc_socket_receive() one or more arguments is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    char *buffer = new char[bufsize + 1];
    ssize_t bytesReceived = recv(sockfd, buffer, bufsize, 0);

    if (bytesReceived < 0)
    {
		printf("gsc_socket_receive() failed to receive data\n");
        stackError("gsc_socket_receive() failed to receive data");
        delete[] buffer;
        stackPushUndefined();
    }
    else
    {
        buffer[bytesReceived] = '\0';  // Null terminate
        stackPushString(buffer);
        delete[] buffer;
    }
}

void gsc_socket_close()
{
	printf("gsc_socket_close()\n");

    int sockfd;
    if (!stackGetParams("i", &sockfd))
    {
		printf("gsc_socket_close() argument is undefined or has a wrong type\n");
        stackError("gsc_socket_close() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if (close(sockfd) == -1)
    {
		printf("gsc_socket_close() failed to close socket\n");
        stackError("gsc_socket_close() failed to close socket");
        stackPushInt(0);
    }
    else
    {
        stackPushInt(1);
    }
}

#endif