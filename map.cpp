#include <iostream>
#include <uWS/uWS.h>
#include <rapidjson/document.h>
#include "network.hpp"
#include "map.hpp"

using namespace std;

HachiServer::HachiServer()
{
    _server.onConnection(bind(&HachiServer::on_connect, this, placeholders::_1));
    _server.onDisconnection(bind(&HachiServer::on_disconnect, this, placeholders::_1));
    _server.onMessage(bind(&HachiServer::on_message, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));
}

void HachiServer::on_connect(uWS::WebSocket socket)
{
    switch (socket.getAddress().port)
    {
    case DISPATCH_SERVER_PORT:
        _dispatch_server = socket;
        cout << "[MAP] Dispatch server connected" << endl;
        break;
    }
}

void HachiServer::on_disconnect(uWS::WebSocket socket)
{
    cout << "[MAP] Disconnect: " << socket.getAddress().address << endl;
}

void HachiServer::on_message(uWS::WebSocket socket, char *message, size_t length, uWS::OpCode opCode)
{
    rapidjson::Document json_message;
    json_message.Parse(message);

    process_message(json_message["PAYLOAD"].GetString());
}

void HachiServer::process_message(const char *message)
{
    cout << "[MAP] Message: " << message << endl;
}