#ifndef NETWORKING
#define NETWORKING
#define THREADS 4
#define DISPATCH_SERVER_PORT 8081
#define LOGIN_SERVER_PORT 8082

#ifdef DISPATCH_SERVER
#define CURRENT_PORT DISPATCH_SERVER_PORT
#endif
#ifdef LOGIN_SERVER
#define CURRENT_PORT LOGIN_SERVER_PORT
#endif

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

	HachiNetwork() : _server(CURRENT_PORT)
    {
    }

    void run()
    {
        cout << "Server started on port " << CURRENT_PORT << endl;
        _server.run();
    }

    virtual void on_connect(WebSocket socket) = 0;
    virtual void on_disconnect(WebSocket socket) = 0;
    virtual void on_message(WebSocket socket, char *message, size_t length, OpCode opCode) = 0;

protected:
    Server _server;
};

enum SERVER_TYPE
{
    DISPATCH = 1,
    LOGIN,
};

#endif