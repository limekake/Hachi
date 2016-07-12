CC = g++
CFLAGS = -std=c++11 -luv -lssl -lcrypto -lz -lpthread -luWS

HachiDispatch: core.cpp dispatch.cpp ; $(CC) -o HachiDispatch core.cpp dispatch.cpp $(CFLAGS) -DDISPATCH_SERVER

HachiLogin: core.cpp login.cpp ; $(CC) -o HachiLogin core.cpp login.cpp $(CFLAGS) -DLOGIN_SERVER
