#include "chap04.h"
#include <netdb.h>

int main() {
#ifdef _WIN32
  WSADATA d;
  if (WSAStartup(MAKEWORD(2, 2), &d)) {
    fprintf(stderr, "Failed to initialize\n");
    return 1;
  }
#endif

  puts("configuring local address");
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo *bind_address;
  getaddrinfo(0, "8080", &hints, &bind_address);

  puts("creating soket...");
  SOCKET socket_listen;
  socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype,
                         bind_address->ai_protocol);
  if (!ISVALIDSOCKET(socket_listen)) {
    fprintf(stderr, "socket() failed. %d\n", GETSOCKETERRNO());
    return 1;
  }

  puts("binding socket to local address..");

  if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
    fprintf(stderr, "bind() failed. %d\n", GETSOCKETERRNO());
    return 1;
  }
  freeaddrinfo(bind_address);

  struct sockaddr_storage client_address;
  socklen_t client_len = sizeof(client_address);
  char read[1024];
  int bytes_received =
      recvfrom(socket_listen, read, 1024, 0, (struct sockaddr *)&client_address,
               &client_len);
  printf("recevied %d bytes: %.s\n", bytes_received, read);

  puts("remote address is: ");
  char address_buffer[100];
  char service_buffer[100];
  getnameinfo(((struct sockaddr *)&client_address), client_len, address_buffer,
              sizeof(address_buffer), service_buffer, sizeof(service_buffer),
              NI_NUMERICHOST | NI_NUMERICSERV);
  printf("%s %s\n",address_buffer, service_buffer);

  CLOSESOCKET(socket_listen);

  #ifdef _WIN32
  	WSACleanup();
  #endif

  puts("finished");


  return 0;
}