CC = clang++
CFLAGS = -std=c++11 -luv -lssl -lcrypto -lz -lpthread -luWs

Hachi: core.cpp login.cpp ; $(CC) -o Hachi core.cpp login.cpp $(CFLAGS)
