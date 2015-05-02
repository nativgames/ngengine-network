#include <iostream>
#include <ngengine/network/network.h>
#include <pthread.h>

#define PORT 5154

using namespace std;

class MessageDescription : public nge::entity::EntityDefinition
{
public:
  MessageDescription(nge::entity::Context *context);
};

class Message : public nge::entity::Entity {
  public:
    
    Message(nge::entity::Context *context) : nge::entity::Entity(context) { }
    ~Message() { }
    
    uint32_t get_nb() { return _nb; }
    uint32_t *get_nb_ptr() { return &_nb; }
    void set_nb(uint32_t nb) { _nb = nb; }
    
    std::string get_str() { return _str; }
    std::string *get_str_ptr() { return &_str; }
    void set_str(std::string str) { _str = str; }
    
    nge::entity::EntityDefinition *__get_definition() const;
    
  private:
    uint32_t _nb;
    std::string _str;
};

nge::entity::Entity *MessageFactory(nge::entity::Context *context) {
  return new Message(context);
}

MessageDescription::MessageDescription(nge::entity::Context *context) 
{
  Message msg(context);

  this->define_attribute("nb", nge::entity::UINT32, &msg, msg.get_nb_ptr());
  this->define_attribute("str", nge::entity::STRING, &msg, msg.get_str_ptr());
  
  this->set_factory(MessageFactory);
}

nge::entity::EntityDefinition *Message::__get_definition()   const { return new MessageDescription(_context); }

// ---

void *server_thread(void *data)
{
  cout << "demarrage du serveur\n";
  nge::network::connection::tcp::server::Server *server = 
    new nge::network::connection::tcp::server::Server(PORT);
  
  server->listen(1);
  
  nge::network::connection::tcp::server::Client *client;
  
  
  // accept a client
  client = server->accept();
  
  cout << "server: client accepté\n";
  
  nge::network::serializer::raw::RawContext raw_context;
  nge::network::BlocSender bs(client, 128, &raw_context);
  
  // send him data
  Message msg(&raw_context);
  msg.set_nb(3);
  msg.set_str("Hey !");
  
  bs.write(msg);
  
  // --
  
  /*uint32_t *p = (uint32_t *) bs.get_mem();
  cout << "writing: " << to_string(p[0]) << endl;
  cout << "writing: " << to_string(p[1]) << endl;*/
  bs.dump();
  // --
  
  cout << "send: " << std::to_string(bs.send()) << endl;
  
  // get data
  
  delete server;
  
  cout << "fin du serveur\n";
  
  return NULL;
}

void *client_thread(void *data)
{
  cout << "demarrage du client\n";
  nge::network::connection::tcp::client::Client *client = 
    new nge::network::connection::tcp::client::Client("127.0.0.1", PORT);

  nge::network::serializer::raw::RawContext raw_context;
  nge::network::BlocSender bs(client, 128, &raw_context);
  
  cout << "client: connect -> " << std::to_string(client->connect()) << endl; // dont forget !
  
  //
  MessageDescription md(&raw_context);
  Message *msg = (Message *) bs.get(md);
  
  uint32_t w, v = msg->get_nb();
  uint8_t *p, *q;
  
  p = (uint8_t *) &v;
  q = (uint8_t *) &w;
  q += 3;
  
  *q = *p; q --; p++;
  *q = *p; q --; p++;
  *q = *p; q --; p++;
  *q = *p;
  
  cout << "client: received nb : " << std::to_string(msg->get_nb()) << endl;
  cout << "client: inversed received nb : " << std::to_string(w) << endl;
  cout << "client: received str: " << msg->get_str() << endl;
  
  delete client;
  
  cout << "fin du client" << endl;
  
  return NULL;
}

int main(int argc, const char *argv[])
{
  pthread_t ts, tc;
  
  pthread_create(&ts, NULL, server_thread, NULL);
  sleep(1);
  pthread_create(&tc, NULL, client_thread, NULL);
  
  pthread_join(ts, NULL);
  pthread_join(tc, NULL);
  
  return 0;
}

