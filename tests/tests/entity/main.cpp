#include <iostream>
#include <ngengine/network/network.h>
#include <pthread.h>

#define PORT 5155

using namespace std;

class Message : public nge::entity::Entity {
  public:
    
    Message(nge::entity::Context *context) : nge::entity::Entity(context) { }
    ~Message() { }
    
    uint32_t get_nb() { return _nb; }
    uint32_t *get_nb_ptr() { return &_nb; }
    void set_nb(uint32_t nb) { _nb = nb; }
    
    
  private:
    uint32_t _nb;
    std::string _str;
};

void *server_thread(void *data)
{
  cout << "demarrage du serveur" << endl;
  nge::network::connection::tcp::server::Server *server = 
    new nge::network::connection::tcp::server::Server(PORT);
  
  server->listen(1);
  
  nge::network::connection::tcp::server::Client *client;
  
  
  // accept a client
  client = server->accept();
  
  cout << "server: client accepté" << endl;
  
  nge::network::serializer::raw::RawContext raw_context;
  nge::network::BlocSender bs(client, 128, &raw_context);
  
  // send him data
  
  
  // get data
  
  delete server;
  
  cout << "fin du serveur" << endl;
  
  return NULL;
}

void *client_thread(void *data)
{
  cout << "demarrage du client" << endl;
  nge::network::connection::tcp::client::Client *client = 
    new nge::network::connection::tcp::client::Client("127.0.0.1", PORT);

  nge::network::serializer::raw::RawContext raw_context;
  nge::network::BlocSender bs(client, 128, &raw_context);
  int t = 0;
  
  cout << "client: connect -> " << std::to_string(client->connect()) << endl; // dont forget !
  client->write(&t, sizeof(t));
  
  delete client;
  
  cout << "fin du client" << endl;
  
  return NULL;
}

int main(int argc, const char *argv[])
{
  pthread_t ts, tc;
  
  pthread_create(&ts, NULL, server_thread, NULL);
  sleep(2);
  pthread_create(&tc, NULL, client_thread, NULL);
  
  pthread_join(ts, NULL);
  pthread_join(tc, NULL);
  
  return 0;
}

