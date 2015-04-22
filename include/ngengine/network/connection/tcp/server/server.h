#ifndef NGENGINE_NETWORK_CONNECTION_TCP_SERVER_SERVER_H
#define NGENGINE_NETWORK_CONNECTION_TCP_SERVER_SERVER_H

#include <ngengine/network/connection/tcp/server/client.h>

#include <netinet/in.h>
#include <sys/socket.h>

namespace nge {
namespace network {
namespace connection {
namespace tcp {
namespace server {

  class Server {

    public:

      Server(unsigned short port);
      ~Server();

      void listen(unsigned int nb_connections);

      Client *accept(); // appel bloquant


    private:

      unsigned short _port;

      int _socket;
      struct sockaddr_in _sockaddr;
  };

}
}
}
}
}

#endif