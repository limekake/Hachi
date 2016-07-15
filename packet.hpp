#ifndef PACKETS
#define PACKETS

struct SERVER_CONNECT
{
    int server_type;
};

struct RESPONSE_LOGIN
{
    int session_id;
};

struct REQUEST_LOGIN_INTERNAL
{
    int session_id;
    char username[20];
};

struct REQUEST_LOGIN_EXTERNAL
{
    char username[20];
};

struct REQUEST_MOVE
{
    int x;
    int y;
};

#endif
