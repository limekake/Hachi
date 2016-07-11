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
    connection_session con;
    con.sessionid = _next_sessionid++;

    _connections[hdl] = con;
}

void HachiServer::on_close(connection_hdl hdl)
{
	auto& data = get_connection(hdl);

    cout << "Closing connection " << data.name << " with sessionid " << data.sessionid << endl;

    _connections.erase(hdl);
}

void HachiServer::on_message(connection_hdl hdl, websocketpp_server::message_ptr msg)
{
	auto& con = get_connection(hdl);

    rapidjson::Document message;
    message.Parse(msg->get_payload().c_str());
    switch (message["action"].GetInt())
    {
        case GET_USER:
            auth_user();
            break;
        case CREATE_USER:
            cout << "Create user" << endl;
            break;
    }
}
void HachiServer::auth_user()
{
    PGconn *conn = PQconnectdb("dbname=hachi_db user=hachi_db_worker password=hachi.9308");
    if (PQstatus(conn) != CONNECTION_OK)
    {
        cout << "ERROR POSTGRES CONNECT" << endl;
    }

    PGresult *result = PQexec(conn, "SELECT * from hachi_login");
    int nfields = PQnfields(result);
    int ntuples = PQntuples(result);

    for(int i = 0; i < ntuples; i++)
    {
        for(int j = 0; j < nfields; j++)
        {
            cout << "[" << i << "," << j << "] " << PQgetvalue(result, i, j) << endl;
        }
    }
}
