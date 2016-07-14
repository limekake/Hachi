CC = g++
CFLAGS = -std=c++11 -luv -lssl -lcrypto -lz -lpthread -luWS

all: HachiDispatch HachiLogin

HachiDispatch: core.cpp dispatch.cpp ; $(CC) -o HachiDispatch core.cpp dispatch.cpp $(CFLAGS) -DDISPATCH_SERVER

HachiLogin: core.cpp login.cpp ; $(CC) -o HachiLogin core.cpp login.cpp $(CFLAGS) -DLOGIN_SERVER

HachiMap: core.cpp map.cpp ; $(CC) -o HachiMap core.cpp map.cpp $(CFLAGS) -DMAP_SERVER
