#include <ngengine/network/bloc_sender.h>
#include <stdlib.h>
#include <cstring>

using namespace nge;
using namespace nge::entity;
using namespace nge::network;

BlocSender::BlocSender(AbstractClient *client, uint32_t bloc_size)
{
  _client = client;
  _bloc_size = bloc_size;
  _size = bloc_size;
  _max_size = bloc_size;
  _mem = malloc(bloc_size);
  
}

BlocSender::~BlocSender()
{
  free(_mem);
}

void BlocSender::clear()
{
  _size = 0;
}

bool BlocSender::write(const void *p, uint32_t size)
{
  bool ret = false;
  
  if(_size + _bloc_size <= _max_size) {
    uint8_t *q = (uint8_t *) _mem;
    q += _size;
    std::memcpy(q, p, size);
    _size += size;
  }
  
  return ret;
}

bool BlocSender::write(Entity &entity)
{
  bool ret = false;
  std::map<uint32_t, entity::AbstractSerializer *> * serializers = entity.__get_context()->get_serializers();
  entity::AbstractSerializer *serializer;
  
  for(auto attr : *(entity.__get_definition()->get_attributes()) ) {
    ret = true;

    if(serializers->find(attr->type) != serializers->end()) {
      serializer = serializers->at(attr->type);
      
      ::nge::entity::serializer::raw::RawBuffer *buf = new 
        ::nge::entity::serializer::raw::RawBuffer();
      
      serializer->serialize(entity.__get(attr->name), buf);
      
      delete buf;
    }
    else {
      ret = false;
      break;
    }
  }
  
  return ret;
}

bool BlocSender::send()
{
  bool ret = false;
  uint32_t r = _client->write(_mem, _size);

  if(r == _size)
    ret = true;
  else ret = false;
  
  _size = 0;
    
  return ret;
}

Entity *BlocSender::get(entity::EntityDefinition &entity_def)
{
  return nullptr;
}
