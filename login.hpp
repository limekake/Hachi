#ifndef LOGIN_SERVER
#define LOGIN_SERVER

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "network.hpp"

typedef websocketpp::server<websocketpp::config::asio> websocketpp_server;

using websocketpp::connection_hdl;
using namespace std;

class HachiServer : public HachiNetwork
{
public:
    HachiServer();
    void on_open(connection_hdl hdl) override;
    void on_close(connection_hdl hdl) override;
    void on_message(connection_hdl hdl, websocketpp_server::message_ptr msg) override;
};

enum ACTION
{
    GET_USER = 1,
    CREATE_USER,
    DELETE_USER,
    UPDATE_USER,
};

#endif
