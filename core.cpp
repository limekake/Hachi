#ifdef DISPATCH_SERVER
#include "dispatch.hpp"
#endif
#ifdef LOGIN_SERVER
#include "login.hpp"
#endif
#ifdef MAP_SERVER
#include "map.hpp"
#endif

int main(int argc, char *argv[])
{
    HachiServer _server;
    _server.run();

    return 0;
}
