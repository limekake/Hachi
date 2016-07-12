#include <iostream>
#include <uWS/uWS.h>
#include <rapidjson/document.h>
#include <easywsclient/easywsclient.hpp>
#include "network.hpp"
#include "dispatch.hpp"

using namespace uWS;
using namespace std;

HachiServer::HachiServer()
{
    _login_server_ws = easywsclient::WebSocket::from_url("ws://localhost:" + LOGIN_SERVER_PORT);
    cout << "LOGIN SERVER CONNECTED" << endl;

    _server.onConnection(bind(&HachiServer::on_connect, this, placeholders::_1));
    _server.onDisconnection(bind(&HachiServer::on_disconnect, this, placeholders::_1));
    _server.onMessage(bind(&HachiServer::on_message, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));
}

void HachiServer::on_connect(WebSocket socket)
{
    connection_session new_session;
    new_session.sessionid = _next_sessionid++;
    _connection_pool[socket] = new_session;
    cout << "[DISPATCH] Connect: " << new_session.sessionid << endl;
}

void HachiServer::on_disconnect(WebSocket socket)
{
    auto session = get_session(socket);

    cout << "[DISPATCH] Disconnect: " << session->sessionid << endl;

    _connection_pool.erase(socket);
}

void HachiServer::on_message(WebSocket socket, char *message, size_t length, OpCode opCode)
{
    auto session = get_session(socket);

    rapidjson::Document json_message;
    json_message.Parse(message);

    auto endpoint = json_message["ENDPOINT"].GetInt();
    switch (endpoint)
    {
    case SERVER_TYPE::DISPATCH:
        process_message(message);
        break;
    case SERVER_TYPE::LOGIN:
        _login_server_ws->send(message);
        _login_server_ws->poll();
        break;
    }
}

void HachiServer::process_message(char *message)
{
    cout << "[DISPATCH] Message: " << message << endl;
}
