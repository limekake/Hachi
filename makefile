CC = clang++
CFLAGS = -lboost_system -std=c++11 -D_WEBSOCKETPP_CPP11_STL_ -DSERVER_HANDLER_LOGIN -lpq

Hachi: core.cpp login.cpp ; $(CC) -o Hachi core.cpp login.cpp $(CFLAGS)
