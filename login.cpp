#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "login.hpp"
#include "packet.hpp"
#include "network.hpp"

using namespace std;

HachiServer::HachiServer()
{
}

void HachiServer::run()
{
}

//void HachiServer::process_message(const char *message)
//{
//    REQUEST_LOGIN login_request;
//    memcpy(&login_request, message, sizeof(REQUEST_LOGIN));
//
//    //cout << "[LOGIN] User: " << string(login_request.username) <<  " " << login_request.session_id << endl;
//
//    RESPONSE_LOGIN login_response;
//    login_response.session_id = login_request.session_id;
//    auto response_message = new char[sizeof(RESPONSE_LOGIN)];
//    memcpy(static_cast<void*>(&login_response), static_cast<void*>(response_message), sizeof(RESPONSE_LOGIN));
//}