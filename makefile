CC = g++
CFLAGS = -std=c++11 -luv -lssl -lcrypto -lz -lpthread -luWS

all: HachiDispatch HachiLogin

HachiDispatch: core.cpp dispatch.cpp easywsclient/easywsclient.cpp ; $(CC) -o HachiDispatch core.cpp dispatch.cpp easywsclient/easywsclient.cpp $(CFLAGS) -DDISPATCH_SERVER

HachiLogin: core.cpp login.cpp easywsclient/easywsclient.cpp ; $(CC) -o HachiLogin core.cpp login.cpp easywsclient/easywsclient.cpp $(CFLAGS) -DLOGIN_SERVER

HachiMap: core.cpp map.cpp easywsclient/easywsclient.cpp ; $(CC) -o HachiLogin core.cpp map.cpp easywsclient/easywsclient.cpp $(CFLAGS) -DMAP_SERVER