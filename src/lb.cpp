#include <cstring>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MYPORT "8080" // the port on which user will connect
#define BACKLOG 10    // number of pending connections queue can hold

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "usage: connect to the correct port" << std::endl;
    return 1;
  }
  struct sockaddr_storage their_addr;
  struct addrinfo hints, *res;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;     // either IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP conn

  int status = getaddrinfo(argv[1], MYPORT, &hints,
                           &res); // for now we assuming first result is
                                  // correct, we will fix it later
  if (status != 0) {
    std::cerr << "getaddrinfo: " << status << std::endl;
    return 2;
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  if (sockfd == -1) {
    std::cerr << "socket: " << sockfd << std::endl;
    return 3;
  }

  int bindfd = bind(sockfd, res->ai_addr, res->ai_addrlen);
  if (bindfd == -1) {
    std::cerr << "bind: " << bindfd << std::endl;
    return 4;
  }

  freeaddrinfo(res);

  int listenfd = listen(sockfd, BACKLOG);
  if (listenfd == -1) {
    std::cerr << "listen: " << listenfd << std::endl;
    return 5;
  }

  while (true) {
    socklen_t addr_size = sizeof(their_addr);
    int clientfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

    if (clientfd == -1) {
      std::cerr << "accept: " << clientfd << std::endl;
      continue;
    }

    std::cout << "Client connected!\n";

    // use the info here

    close(clientfd);
  }
}
