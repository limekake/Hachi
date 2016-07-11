#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <rapidjson/document.h>
#include <iostream>
#include <map>
#include <postgresql/libpq-fe.h>
#include "network.hpp"
#include "login.hpp"

typedef websocketpp::server<websocketpp::config::asio> websocketpp_server;

using websocketpp::connection_hdl;
using placeholders::_1;
using placeholders::_2;
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
    connection_session session;
    session.sessionid = _next_sessionid++;

    _connections[hdl] = session;
}

void HachiServer::on_close(connection_hdl hdl)
{
	auto& session = get_connection(hdl);

    cout << "Closing connection " << session.name << " with sessionid " << session.sessionid << endl;

    _connections.erase(hdl);
}

void HachiServer::on_message(connection_hdl hdl, websocketpp_server::message_ptr msg)
{
    rapidjson::Document message;
    message.Parse(msg->get_payload().c_str());
    switch (message["action"].GetInt())
    {
        case GET_USER:
            auth_user(hdl, message["username"].GetString(), message["password"].GetString());
            break;
        case CHAT_MESSAGE:
            chat_message(hdl, msg, message["chat_message"].GetString());
            break;
    }
}

void HachiServer::auth_user(connection_hdl hdl, const char* u, const char* p)
{
    auto& session = get_connection(hdl);

    string query = "SELECT username, password from hachi_login WHERE username='" + string(u) + "' AND password='" + string(p) + "'";
    PGresult *result = PQexec(conn, query.c_str());

    string username = PQgetvalue(result, 0, 0);
    cout << PQgetvalue(result, 0, 0) << " has been authenticated" << endl;
    session.auth = true;
    session.name = username;
}

void HachiServer::chat_message(connection_hdl hdl, websocketpp_server::message_ptr msg, const char* c_m)
{
    auto& session = get_connection(hdl);

    if (!session.auth)
    {
        string not_auth_message = "Connection id " + to_string(session.sessionid) + " is not authenticated";
        for (auto iter = _connections.begin(); iter != _connections.end(); ++iter)
        {
            auto handler = iter->first;
            msg->set_payload(not_auth_message);
            _server.send(handler, msg);
        }
    }

    string chat_message = "[" + session.name + "]: " + c_m;
    for (auto iter = _connections.begin(); iter != _connections.end(); ++iter)
    {
        auto handler = iter->first;
        msg->set_payload(chat_message);
        _server.send(handler, msg);
    }
}
