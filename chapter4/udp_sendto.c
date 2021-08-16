#include "chap04.h"

int main() {
#ifdef _WIN32
  WSADATA d;
  if (WSAStartup(MAKEWORD(2, 2), &d)) {
    fprintf(stderr, "WSAStartup() failed\n");
    return 1;
  }
#endif

  puts("configuring remote address");
  struct addrinfo hints;

  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_DGRAM;
  struct addrinfo *peer_address;
  if (getaddrinfo("127.0.0.1", "8080", &hints, &peer_address)) {
    fprintf(stderr, "getaddrinfo() failed. %d\n", GETSOCKETERRNO());
    return 1;
  }

  puts("remote address is: ");
  char address_buffer[100];
  char service_buffer[100];
  getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen, address_buffer,
              sizeof(address_buffer), service_buffer, sizeof(service_buffer),
              NI_NUMERICHOST | NI_NUMERICSERV);
  printf("%s %s\n", address_buffer, service_buffer);

  puts("creating socket");
  SOCKET socket_peer;
  socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype,
                       peer_address->ai_protocol);
  if (!ISVALIDSOCKET(socket_peer)) {
    fprintf(stderr, "socket() failed. %d\n", GETSOCKETERRNO());
    return 1;
  }
  const char *message = "Hello World";
  printf("sending: %s\n", message);
  int bytes_sent = sendto(socket_peer, message, strlen(message), 0,
                          peer_address->ai_addr, peer_address->ai_addrlen);
  printf("send %d bytes\n", bytes_sent);
  freeaddrinfo(peer_address) : CLOSESOCKET(socket_peer);

#ifdef _WIN32
  WSACleanup();
#endif

  puts("finished");

  return 0;
}