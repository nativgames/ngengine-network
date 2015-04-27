#ifndef NGENGINE_NETWORK_BLOC_SENDER_H
#define	NGENGINE_NETWORK_BLOC_SENDER_H

#include <ngengine/network/client.h>
#include <ngengine/entity/entity.h>

/*
 * BlocSender:
 * prepare a message, then send it in 1 bloc 
 */

namespace nge {
namespace network {

  class BlocSender { // only resize if the size to send is > _max_size
  public:
    
    BlocSender(AbstractClient *client, uint32_t bloc_size, entity::Context *context);
    ~BlocSender();
    
    void clear();
    bool write(const void *p, uint32_t size);
    bool write(entity::Entity &entity);

    bool send();
    entity::Entity *get(entity::EntityDefinition &entity_def);
    
  private:
    AbstractClient *_client;
    uint32_t _bloc_size;
    entity::Context *_context;
    
    uint32_t _size;
    uint32_t _max_size;
    void *_mem;
  };

}
}


#include <ngengine/network/serializer/raw/serializer.h>

#endif	/* BLOC_SENDER_H */

