#pragma once

#include "init/macros.h"
#include "init/pch.h"
#include "socket.hpp"

#ifndef MAX_CONNECTIONS
    #define MAX_CONNECTIONS 10
#endif

#ifdef WIN32
    #define CLOSE(...) CloseSocket(...)
#else
    #define CLOSE(...) close(...)
#endif

namespace net
{
    struct API ConnectionList
    {
      public:
        ConnectionList() {};
        int operator+=(Socket *s);
        int operator-=(Socket *s);
        Socket* operator[](unsigned int i);

      private:
        ConnectionList(ConnectionList const &);

        unsigned int m_activeConns;
        Socket *m_list[MAX_CONNECTIONS];
    };

    class API ConnectionManager {
      public:
        /// Creates and maintains the singleton status
        /// of the ConnectionManager
        static ConnectionManager *Instance();

        ConnectionList Connections;

      private:
        ConnectionManager() {};
        ConnectionManager(ConnectionManager const &);
        ConnectionManager &operator=(ConnectionManager const &);
        static ConnectionManager *m_pInstance;

        int *net_err_code;

#ifdef WIN32
        WSADATA wsaData;
#endif
    };
}  // namespace net
