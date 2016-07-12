#ifndef NETWORKING
#define NETWORKING
#define PORT 8081
#define THREADS 4

#include <iostream>
#include <uWS/uWS.h>
#include <map>

using namespace uWS;
using namespace std;

struct connection_session {
    int sessionid;
    WebSocket socket;
    string name;
};

class HachiNetwork
{
public:
	virtual ~HachiNetwork() {}

	HachiNetwork() : _next_sessionid(1), _server(PORT)
    {
    }

    void run()
    {
        cout << "Server started on port " << PORT << endl;
        _server.run();
    }

    virtual void on_connect(WebSocket socket) = 0;
    virtual void on_disconnect(WebSocket socket) = 0;
    virtual void on_message(WebSocket socket, char *message, size_t length, OpCode opCode) = 0;

    connection_session* get_session(WebSocket socket)
    {
        auto session = _connection_pool.find(reinterpret_cast<uint32_t>(&socket));

        if (session == _connection_pool.end())
        {
            cout << "No session found!" << endl;
            return nullptr;
        }
        return &(session->second);
    }

protected:
    int _next_sessionid;
    map<uint32_t, connection_session> _connection_pool;
    Server _server;
};

#endif