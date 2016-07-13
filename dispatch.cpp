#define xstr(a) str(a)
#define str(a) #a

#include <iostream>
#include <uWS/uWS.h>
#include <rapidjson/document.h>
#include "easywsclient/easywsclient.hpp"
#include "network.hpp"
#include "dispatch.hpp"

using namespace std;

HachiServer::HachiServer()
{
    _login_server_ws = easywsclient::WebSocket::from_url("ws://localhost:" xstr(LOGIN_SERVER_PORT));
    cout << "LOGIN SERVER CONNECTED" << endl;

    _map_server_ws = easywsclient::WebSocket::from_url("ws://localhost:" xstr(MAP_SERVER_PORT));
    cout << "MAP SERVER CONNECTED" << endl;

    _server.onConnection(bind(&HachiServer::on_connect, this, placeholders::_1));
    _server.onDisconnection(bind(&HachiServer::on_disconnect, this, placeholders::_1));
    _server.onMessage(bind(&HachiServer::on_message, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));
}

void HachiServer::on_connect(uWS::WebSocket socket)
{
    connection_session new_session;
    new_session.sessionid = _next_sessionid++;
    _connection_pool[socket] = new_session;
    cout << "[DISPATCH] Connect: " << new_session.sessionid << endl;
}

void HachiServer::on_disconnect(uWS::WebSocket socket)
{
    auto session = get_session(socket);

    cout << "[DISPATCH] Disconnect: " << session->sessionid << endl;

    _connection_pool.erase(socket);
}

void HachiServer::on_message(uWS::WebSocket socket, char *message, size_t length, uWS::OpCode opCode)
{
    rapidjson::Document json_message;
    json_message.Parse(message);

    auto endpoint = json_message["ENDPOINT"].GetInt();
    switch (endpoint)
    {
    case SERVER_TYPE::DISPATCH:
        process_message(json_message["PAYLOAD"].GetString());
        break;
    case SERVER_TYPE::LOGIN:
        _login_server_ws->send(message);
        _login_server_ws->poll();
        break;
    }
}

void HachiServer::process_message(const char *message)
{
    cout << "[DISPATCH] Message: " << message << endl;
}
