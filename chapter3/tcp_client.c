#include "chap3.h"
#include <netdb.h>
#include <stdio.h>

#if defined(_win32)
#include <conio.h>
#endif

int main(int argc, char *argv[]) {
#ifdef _WIN32
  WSADATA d;
  if (WSAStartup(MAKEWORD(2, 2), &d)) {
    fprintf(stderr, "Failed to initialize\n");
    return 1;
  }
#endif

  if (argc < 3) {
    fprintf(stderr, "usage:tcp_client hostname port\n");
    return 1;
  }
  printf("configuring remote address...\n");
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  struct addrinfo *peer_address;
  if (getaddrinfo(argv[1], argv[2], &hints, &peer_address)) {
    fprintf(stderr, "getaddrinfo() failed. %d\n", GETSOCKETERRNO());
    return 1;
  }

  printf("Remote address is:");
  char address_buffer[100];
  char service_buffer[100];
  getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen, address_buffer,
              sizeof(address_buffer), service_buffer, sizeof(service_buffer),
              NI_NUMERICHOST);
  printf("%s %s\n", address_buffer, service_buffer);

  puts("Creating socket...");
  SOCKET socket_peer;
  socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype,
                       peer_address->ai_protocol);
  if (!ISVALIDSOCKET(socket_peer)) {
    fprintf(stderr, "socket() failed. %d\n", GETSOCKETERRNO());
    return 1;
  }

  printf("Connecting..\n");
  if (connect(socket_peer, peer_address->ai_addr, peer_address->ai_addrlen)) {
    fprintf(stderr, "connect() failed. %d\n", GETSOCKETERRNO());
    return 1;
  }

  freeaddrinfo(peer_address);
}