#pragma once

#ifdef WIN32
    #include <winsock.h>
#else
    #include <arpa/inet.h>
    #include <errno.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <poll.h>
    #include <sys/socket.h>
    #include <sys/types.h>
#endif

#include <cstring>
#include <iostream>
#include <unistd.h>
