#include <iostream>
#include <cstring>
#include <uWS/uWS.h>
#include "network.hpp"
#include "login.hpp"
#include "packet.hpp"

using namespace std;

HachiServer::HachiServer() : HachiNetwork(LOGIN_SERVER_PORT)
{
    _server.onConnection(bind(&HachiServer::on_connect, this, placeholders::_1));
    _server.onDisconnection(bind(&HachiServer::on_disconnect, this, placeholders::_1));
    _server.onMessage(bind(&HachiServer::on_message, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));
}

void HachiServer::run()
{
    cout << "LOGIN SERVER STARTED ON PORT " << LOGIN_SERVER_PORT << endl;
    _server.run();
}

void HachiServer::on_connect(uWS::WebSocket socket)
{
    switch (socket.getAddress().port)
    {
    case DISPATCH_SERVER_PORT:
        _dispatch_server = socket;
        cout << "[LOGIN] Dispatch server connected" << endl;
        break;
    }
}

void HachiServer::on_disconnect(uWS::WebSocket socket)
{
    cout << "[LOGIN] Disconnect: " << socket.getAddress().address << endl;
}

void HachiServer::on_message(uWS::WebSocket socket, char *message, size_t length, uWS::OpCode opCode)
{
    auto s = string(message, length).c_str();
    cout << s << endl;
    process_message(s);
}

void HachiServer::process_message(const char *message)
{
    REQUEST_LOGIN login_request;
    memcpy(&login_request, message, sizeof(REQUEST_LOGIN));

    cout << "[LOGIN] User: " << string(login_request.username) <<  " " << login_request.session_id << endl;

    RESPONSE_LOGIN login_response;
    login_response.session_id = login_request.session_id;
    auto response_message = new char[sizeof(RESPONSE_LOGIN)];
    memcpy(static_cast<void*>(&login_response), static_cast<void*>(response_message), sizeof(RESPONSE_LOGIN));

    _dispatch_server.send(response_message, sizeof(response_message), uWS::OpCode::BINARY);
}