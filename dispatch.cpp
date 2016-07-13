#define xstr(a) str(a)
#define str(a) #a

#include <iostream>
#include <cstring>
#include <uWS/uWS.h>
#include "easywsclient/easywsclient.hpp"
#include "dispatch.hpp"
#include "network.hpp"
#include "packet.hpp"

using namespace std;

HachiServer::HachiServer() : HachiNetwork(DISPATCH_SERVER_PORT)
{
    _login_server_ws = easywsclient::WebSocket::from_url("ws://localhost:" xstr(LOGIN_SERVER_PORT));
    cout << "LOGIN SERVER CONNECTED" << endl;

    _map_server_ws = easywsclient::WebSocket::from_url("ws://localhost:" xstr(MAP_SERVER_PORT));
    cout << "MAP SERVER CONNECTED" << endl;

    _server.onConnection(bind(&HachiServer::on_connect, this, placeholders::_1));
    _server.onDisconnection(bind(&HachiServer::on_disconnect, this, placeholders::_1));
    _server.onMessage(bind(&HachiServer::on_message, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));
}

void HachiServer::run()
{
    cout << "DISPATCH SERVER STARTED ON PORT " << DISPATCH_SERVER_PORT << endl;
    _server.run();
}

void HachiServer::on_connect(uWS::WebSocket socket)
{
    connection_session new_session;
    _connection_pool[socket] = new_session;
    cout << "[DISPATCH] Connect: " << socket.getAddress().address << ":" << socket.getAddress().port << endl;
}

void HachiServer::on_disconnect(uWS::WebSocket socket)
{
    auto session = get_session(socket);

    cout << "[DISPATCH] Disconnect: " << socket.getAddress().address << ":" << socket.getAddress().port << endl;

    _connection_pool.erase(socket);
}

void HachiServer::on_message(uWS::WebSocket socket, char *message, size_t length, uWS::OpCode opCode)
{
    cout << socket.getAddress().address << endl;

    auto session = get_session(socket);
    auto pass_message = new char[sizeof(REQUEST_LOGIN)];
    if (!session->auth)
    {
        REQUEST_LOGIN login_packet;
        strncpy(login_packet.username, "administrator", sizeof(login_packet.username));
        memcpy(pass_message, &login_packet, sizeof(REQUEST_LOGIN));

        _login_server_ws->send(pass_message);
        _login_server_ws->poll();
    }
    else
    {
        _map_server_ws->send(message);
        _map_server_ws->poll();
    }
}