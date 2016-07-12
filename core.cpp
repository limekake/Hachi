#ifdef SERVER_HANDLER_DISPATCH
#include "dispatch.hpp"
#endif
#ifdef SERVER_HANDLER_LOGIN
#include "login.hpp"
#endif

int main(int argc, char *argv[])
{
    HachiServer _server;
    _server.run();

    return 0;
}
