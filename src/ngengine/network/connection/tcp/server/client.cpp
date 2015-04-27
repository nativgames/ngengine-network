#include <ngengine/network/connection/tcp/server/client.h>

using namespace nge::network; 
using namespace nge::network::connection::tcp::server;


Client::Client(int socket)
{
  _socket = socket;
}

Client::~Client()
{
  close(_socket);
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