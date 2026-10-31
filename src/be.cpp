#include <cstring>
#include <cstdio>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MYPORT_BE "8081" // the port on which user will connect
#define BACKLOG 10    // number of pending connections queue can hold

int main(int argc, char *argv[]) { 
  struct sockaddr_storage their_addr;
  struct addrinfo hints, *res;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;     // either IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP conn

  int status = getaddrinfo(NULL, MYPORT_BE, &hints, &res); 
  if (status != 0) {
    std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
    return 2;
  }
  int sockfd = -1;
  for (auto p = res; p != nullptr; p = p->ai_next) {
    int candidate = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

    if (candidate == -1) {
      std::perror("sockfd");
      continue;
    }

    if (bind(candidate, p->ai_addr, p->ai_addrlen) == -1) {
      std::perror("bind");
      close(candidate);
      continue;
    }

    sockfd = candidate;
    break;
  }
  freeaddrinfo(res);

  if (sockfd == -1) {
    std::cerr << "Could not bind to port " << MYPORT_BE << std::endl;
    return 3;
  }

  int listenfd = listen(sockfd, BACKLOG);
  if (listenfd == -1) {
    std::perror("listen");
    close(sockfd);
    return 5;
  }

  while (true) {
    socklen_t addr_size = sizeof(their_addr);
    int clientfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

    if (clientfd == -1) {
      std::perror("clientfd");
      continue;
    }

    std::cout << "Client connected!\n";

    // use the info here

    close(clientfd);
  }
}
