#ifndef NETWORKING
#define NETWORKING
#define THREADS 4
#define DISPATCH_SERVER_PORT 8081
#define LOGIN_SERVER_PORT 8082
#define MAP_SERVER_PORT 8083

enum SERVER_TYPE
{
    DISPATCH = 1,
    LOGIN,
    MAP,
};

#endif