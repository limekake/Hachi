#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#ifdef SERVER_HANDLER_MAP
#include "map.hpp"
#endif
#ifdef SERVER_HANDLER_CHAR
#include "char.hpp"
#endif
#ifdef SERVER_HANDLER_LOGIN
#include "login.hpp"
#endif

using namespace std;

int main(int argc, char *argv[])
{
    HachiServer _server;
    _server.run();

    return 0;
}
