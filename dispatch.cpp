#define xstr(a) str(a)
#define str(a) #a

#include <iostream>
#include <cstring>
#include <thread>
#include <netdb.h>
#include <arpa/inet.h>
#include <uWS/uWS.h>
#include "dispatch.hpp"
#include "network.hpp"
#include "packet.hpp"

using namespace std;

HachiServer::HachiServer() : _outside_server(OUTSIDE_SERVER_PORT), _next_sessionid(1)
{
    _outside_server.onConnection(bind(&HachiServer::on_connect, this, placeholders::_1));
    _outside_server.onDisconnection(bind(&HachiServer::on_disconnect, this, placeholders::_1));
    _outside_server.onMessage(bind(&HachiServer::on_message, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));

    _dispatch_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_dispatch_server_socket < 0)
    {
        cout << "[DISPATCH] Error creating socket" << endl;
        exit(1);
    }
    _dispatch_server.sin_family = AF_INET;
    _dispatch_server.sin_addr.s_addr = inet_addr("127.0.0.1");
    _dispatch_server.sin_port = htons(DISPATCH_SERVER_PORT);
}

void HachiServer::run()
{
    if (bind(_dispatch_server_socket, (struct sockaddr *) &_dispatch_server, sizeof(_dispatch_server)) < 0) {
        cout << "[DISPATCH] Error binding on port " << DISPATCH_SERVER_PORT << endl;
        exit(1);
    }
    listen(_dispatch_server_socket, 2);
    cout << "DISPATCH SERVER STARTED ON PORT " << DISPATCH_SERVER_PORT << endl;

    int c = sizeof(_login_server);
    if ((_login_server_socket = accept(_dispatch_server_socket, (struct sockaddr *) &_login_server, (socklen_t *)&c)) < 0)
    {
        cout << "[DISPATCH] Error accepting connection from login" << endl;
        exit(1);
    }
    thread _login_thread(&HachiServer::login_message, this);

    _outside_server.run();
    _login_thread.join();
}

void HachiServer::login_message()
{
    int recv_size;
    char buffer[64];
    string message;

    while ((recv_size = recv(_login_server_socket, buffer, sizeof(buffer), 0)) > 0)
    {
        cout << buffer << endl;
        memset(buffer, 0, 64);
    }
}

void HachiServer::login_send(const char* message)
{
    send(_login_server_socket, message, sizeof(message), 0);
}

void HachiServer::login_process_message(const char* message)
{
    RESPONSE_LOGIN login_request;
    memcpy(&login_request, message, sizeof(RESPONSE_LOGIN));

    auto session = get_session(login_request.session_id);
    session->auth = true;

    cout << "[DISPATCH] Login authenticated session " << session->session_id << endl;
}


void HachiServer::on_connect(uWS::WebSocket socket)
{
    connection_session new_session;
    new_session.session_id = _next_sessionid++;
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
    auto session = get_session(socket);
    auto message_data = string(message, length);

    cout << "[DISPATCH] Address: " << socket.getAddress().address << endl;

    if (!session->auth)
    {
        auto pass_message = new char[sizeof(REQUEST_LOGIN)];
        REQUEST_LOGIN login_packet;
        login_packet.session_id = session->session_id;
        strncpy(login_packet.username, "administrator", sizeof(login_packet.username));
        memcpy(static_cast<void*>(pass_message), static_cast<void*>(&login_packet), sizeof(REQUEST_LOGIN));

        login_send(pass_message);
    }
    else
    {
        cout << "[DISPATCH] Message: " << message_data << endl;
    }
}
