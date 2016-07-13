#ifndef PACKETS
#define PACKETS

struct REQUEST_LOGIN
{
    char username[20];
};

struct REQUEST_MOVE
{
    int x;
    int y;
};

#endif