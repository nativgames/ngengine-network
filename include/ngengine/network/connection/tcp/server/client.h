#ifndef NGENGINE_NETWORK_CONNECTION_TCP_SERVER_CLIENT_H
#define	NGENGINE_NETWORK_CONNECTION_TCP_SERVER_CLIENT_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <ngengine/network/client.h>

namespace nge {
namespace network {
namespace connection {
namespace tcp {
namespace server {

  class Client : public AbstractClient {

    public:

      Client(int socket);
      ~Client();
      
      bool is_connected();

      virtual int32_t write(const void *p, unsigned int maxsize);
      virtual int32_t read(void *p, unsigned int maxsize);
      
      int get_socket();

    private:

      int _socket;
      struct sockaddr_in _server_sockaddr;
      struct hostent *_serverHostEnt;
  };
  
}
}
}
}
}

#endif
