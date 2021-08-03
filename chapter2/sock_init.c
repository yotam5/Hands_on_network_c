
#if defined(_WIN32) //windows
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#else    //linux
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#endif


#include <stdio.h>

int main(void)
{
	#if defined(_WIN32)
		WSADATA d;
		if (WSAStartup(MAKEWORD(2,2),&d)){
			fprintf(stderr,"failed to initialize\n");
			return -1;
		}
	#endif
		printf("ready to use socket API\n");
	#if defined(_WIN32)
		WSACleanup(d);
	#endif
		return 0;
}