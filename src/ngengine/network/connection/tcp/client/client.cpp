#include <ngengine/network/connection/tcp/client/client.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>

using namespace nge::network; 
using namespace nge::network::connection::tcp::client;

Client::Client(const char *server_name, unsigned short port)
{
  long hostAddr;

  _server_name = strdup(server_name);
  _port = port;
  _socket = 0;

  memset(&_server_sockaddr, 0,  sizeof(_server_sockaddr));
  memset(&_serverHostEnt, 0,  sizeof(_serverHostEnt));

  hostAddr = ::inet_addr(server_name); // pour une ip

  bcopy(&hostAddr, &_server_sockaddr.sin_addr, sizeof(hostAddr));
  
  _server_sockaddr.sin_port = htons(port);
  _server_sockaddr.sin_family = AF_INET;

  if((_socket = ::socket(AF_INET,SOCK_STREAM,0)) < 0) {
    printf("Impossible de se connecter au serveur.\n");
    exit(42);
  }
  else;
}

Client::~Client()
{
  close(_socket);
}

bool Client::connect()
{
  return !(::connect(_socket,
             (struct sockaddr *)&_server_sockaddr,
             sizeof(_server_sockaddr)) < 0);
}

bool Client::is_connected()
{
  return true;
}

int32_t Client::write(const void *p, unsigned int maxsize)
{
  return ::send(_socket, p, maxsize, MSG_NOSIGNAL);
}

int32_t Client::read(void *p, unsigned int maxsize)
{
  return ::recv(_socket, p, maxsize, MSG_WAITALL);  
}

int Client::get_socket()
{
  return _socket;
}
