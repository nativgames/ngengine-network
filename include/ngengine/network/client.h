#ifndef NGENGINE_NETWORK_CLIENT_H
#define	NGENGINE_NETWORK_CLIENT_H

#include <cstdint>

namespace nge {
namespace network {

  class AbstractClient {

    public:

      virtual ~AbstractClient();

      virtual int32_t write(const void *p, unsigned int maxsize) = 0;
      virtual int32_t read(void *p, unsigned int maxsize) = 0;
      
      virtual bool is_connected() = 0;
      
      virtual int get_socket() = 0;
  };

}
}

#endif	
