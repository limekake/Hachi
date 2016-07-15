#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>
#include "map.hpp"
#include "packet.hpp"
#include "network.hpp"

using namespace std;

HachiServer::HachiServer()
{
    _dispatch_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_dispatch_server_socket < 0)
    {
        cout << "[MAP] Error creating socket" << endl;
        exit(1);
    }

    _dispatch_server.sin_addr.s_addr = inet_addr("127.0.0.1");
    _dispatch_server.sin_family = AF_INET;
    _dispatch_server.sin_port = htons(DISPATCH_SERVER_PORT);
}

void HachiServer::run()
{
    if (connect(_dispatch_server_socket, (struct sockaddr *)&_dispatch_server, sizeof(_dispatch_server)) < 0)
    {
        cout << "[MAP] Error connecting to dispatch" << endl;
        exit(1);
    }
    cout << "[MAP] Connected to dispatch" << endl;

    thread _dispatch_thread(&HachiServer::dispatch_message, this);

    _dispatch_thread.join();
}

void HachiServer::dispatch_message()
{
    int recv_size;
    char buffer[64];
    string message;

    while ((recv_size = recv(_dispatch_server_socket, buffer, 64, 0)) > 0)
    {
        process_message(buffer);
        memset(buffer, 0, 64);
    }
}

void HachiServer::dispatch_send(const char* message, size_t size)
{
    send(_dispatch_server_socket, message, size, 0);
}

void HachiServer::process_message(const char *message)
{
}

int main(int argc, char *argv[])
{
    HachiServer _server;
    _server.run();

    return 0;
}