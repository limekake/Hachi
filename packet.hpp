#ifndef PACKETS
#define PACKETS

struct RESPONSE_LOGIN
{
    int session_id;
};

struct REQUEST_LOGIN
{
    int session_id;
    char username[20] = {0};
};

struct REQUEST_MOVE
{
    int x;
    int y;
};

#endif