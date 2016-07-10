#ifndef NETWORKING
#define NETWORKING
#define PORT 8081

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <map>
#include <exception>

typedef websocketpp::server<websocketpp::config::asio> websocketpp_server;

using websocketpp::connection_hdl;
using namespace std;

struct connection_data {
    int sessionid;
    string name;
};

class HachiNetwork
{
public:
    HachiNetwork() : _next_sessionid(1)
    {
        _server.init_asio();
    }

    void run()
    {
        cout << "Server started on port " << PORT << endl;
        _server.listen(PORT);
        _server.start_accept();
        _server.run();
    }

    virtual void on_open(connection_hdl hdl) = 0;
    virtual void on_close(connection_hdl hdl) = 0;
    virtual void on_message(connection_hdl hdl, websocketpp_server::message_ptr msg) = 0;

    connection_data& get_connection(connection_hdl hdl)
    {
        auto it = _connections.find(hdl);
        if (it == _connections.end()) {
            throw invalid_argument("No data avaliable for session");
        }
        return it->second;
    }

protected:
    int _next_sessionid;
    websocketpp_server _server;
    map<connection_hdl, connection_data, std::owner_less<connection_hdl>> _connections;
};

#endif
