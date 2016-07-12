CC = g++
CFLAGS = -std=c++11 -luv -lssl -lcrypto -lz -lpthread -luWS
MACROS = -DSERVER_HANDLER_LOGIN

Hachi: core.cpp login.cpp ; $(CC) -o Hachi core.cpp login.cpp $(CFLAGS) $(MACROS)
