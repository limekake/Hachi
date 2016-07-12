#include <uWS/uWS.h>
#include "network.hpp"

using namespace uWS;
using namespace std;

class HachiServer : public HachiNetwork
{
public:
    HachiServer();
    void on_connect(WebSocket socket) override;
    void on_disconnect(WebSocket socket) override;
    void on_message(WebSocket socket, char *message, size_t length, OpCode opCode) override;

private:
    static void process_message(const char *message);

    WebSocket _dispatch_server;
};

enum ACTION
{
    GET_USER = 1,
    CREATE_USER,
    DELETE_USER,
    UPDATE_USER,
    MESSAGE_CHAT,
};