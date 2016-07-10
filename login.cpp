#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <rapidjson/document.h>
#include <iostream>
#include <map>
#include <exception>
#include "network.hpp"
#include "login.hpp"

typedef websocketpp::server<websocketpp::config::asio> websocketpp_server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using namespace std;

HachiServer::HachiServer()
{
    _server.set_open_handler(bind(&HachiServer::on_open, this, _1));
    _server.set_close_handler(bind(&HachiServer::on_close, this, _1));
    _server.set_message_handler(bind(&HachiServer::on_message, this, _1, _2));
}

void HachiServer::on_open(connection_hdl hdl)
{
    connection_data con;
    con.sessionid = _next_sessionid++;

    _connections[hdl] = con;
}

void HachiServer::on_close(connection_hdl hdl)
{
    connection_data& data = get_connection(hdl);

    cout << "Closing connection " << data.name << " with sessionid " << data.sessionid << endl;

    _connections.erase(hdl);
}

void HachiServer::on_message(connection_hdl hdl, websocketpp_server::message_ptr msg)
{
    connection_data& con = get_connection(hdl);

    rapidjson::Document message;
    message.Parse(msg->get_payload().c_str());
    switch (message["action"].GetInt())
    {
        case GET_USER:
            cout << "Get user" << endl;
            break;
    }
}
