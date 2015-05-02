#include <ngengine/network/bloc_sender.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>

using namespace nge;
using namespace nge::entity;
using namespace nge::network;

BlocSender::BlocSender(AbstractClient *client, uint32_t bloc_size, entity::Context *context)
{
  _client = client;
  _bloc_size = bloc_size;
  _size = 0;
  _max_size = bloc_size;
  _mem = malloc(bloc_size);
  _context = context;
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
  
  if(_size + size >= _max_size) {
    uint32_t n = 0;
    uint32_t size_min = _size + size;
    
    n = size_min / _bloc_size;
    if(size_min % _bloc_size != 0)
      n ++;
    else;
    
    _mem = realloc(_mem, n * _bloc_size);
    _max_size = n * _bloc_size;
  }
  
  // copy the data
  
  uint8_t *q = (uint8_t *) _mem;
  q += _size;
  std::memcpy(q, p, size);
  _size += size;

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
      // get the serializer for this attribute
      serializer = serializers->at(attr->type);
      
      ::nge::entity::serializer::raw::RawBuffer *buf = new 
        ::nge::entity::serializer::raw::RawBuffer();
      // serialize
      serializer->serialize(entity.__get(attr->name), buf);
      // add the size only if needed
      //if(buf->variable_size == true) {
      std::cout << "buf size " << std::to_string(buf->size) << std::endl;
      this->write(&buf->size, sizeof(uint32_t));
      //}
      // write the data
      this->write(buf->data, buf->size);
      
      
      std::cout << "==============================\nDUMP ENTITY\n==============================\n";

      delete buf;
      
      this->dump();
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
  if(entity_def.get_factory() == NULL) {
    std::cout << "entity factory is NULL !" << std::endl;
    return NULL;
  }
  
  Entity *entity = (entity_def.get_factory())(_context);
  std::map<uint32_t, entity::AbstractSerializer *> * serializers = _context->get_serializers();
  entity::AbstractSerializer *serializer;
  
  for(auto attr : *(entity_def.get_attributes()) ) {
    if(serializers->find(attr->type) != serializers->end()) {
      // get the serializer for this attribute
      serializer = serializers->at(attr->type);
      
      ::nge::entity::serializer::raw::RawBuffer *buf = new 
        ::nge::entity::serializer::raw::RawBuffer();
      
      _client->read(&(buf->size), sizeof(uint32_t));
      std::cout << "size read: " << std::to_string(buf->size) << std::endl;
      buf->data = malloc(buf->size);
      _client->read(buf->data, buf->size);
      
      serializer->decode(buf, entity->__get(attr->name));
    }
  }
  
  return entity;
}

void *BlocSender::get_mem()
{
  return _mem;
}

uint32_t BlocSender::get_size()
{
  return _size;
}

uint32_t BlocSender::get_max_size()
{
  return _max_size;
}

uint32_t BlocSender::get_bloc_size()
{
  return _bloc_size;
}

void BlocSender::dump()
{
  std::cout << "========= Dump of Bloc Sender ========= " << std::endl;
  uint8_t *p = (uint8_t *) _mem;
  
  std::cout << "size: " << _size << std::endl;

  for(uint32_t i = 0; i < _size; i++) {
    std::cout << std::to_string(*p) << std::endl;
    p++;
  }
}
