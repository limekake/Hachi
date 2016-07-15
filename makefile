CC = g++
CFLAGS = -std=c++11 -luv -lssl -lcrypto -lz -lpthread -luWS

all: HachiDispatch HachiLogin

HachiDispatch: dispatch.cpp ; $(CC) -o HachiDispatch dispatch.cpp $(CFLAGS)

HachiLogin: login.cpp ; $(CC) -o HachiLogin login.cpp $(CFLAGS)

HachiMap: map.cpp ; $(CC) -o HachiMap map.cpp $(CFLAGS)
