#include <iostream>
#include <uWS/uWS.h>
#include <rapidjson/document.h>
#include "network.hpp"
#include "login.hpp"

using namespace uWS;
using namespace std;

HachiServer::HachiServer()
{
    _server.onConnection(bind(&HachiServer::on_connect, this, placeholders::_1));
    _server.onDisconnection(bind(&HachiServer::on_disconnect, this, placeholders::_1));
    _server.onMessage(bind(&HachiServer::on_message, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));
}

void HachiServer::on_connect(WebSocket socket)
{
    connection_session new_session;
    new_session.sessionid = _next_sessionid++;
    _connection_pool[socket] = new_session;
    cout << "[Connection] New Client: " << new_session.sessionid << endl;
}

void HachiServer::on_disconnect(WebSocket socket)
{
    auto session = get_session(socket);

    cout << "[Connection] Disconnecting Client: " << session->sessionid << endl;

    _connection_pool.erase(socket);
}

void HachiServer::on_message(WebSocket socket, char *message, size_t length, OpCode opCode)
{
    rapidjson::Document message;
    //message.Parse(msg->get_payload().c_str());
}