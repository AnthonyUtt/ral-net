#pragma once

#include "init/macros.h"
#include "init/pch.h"
#include "init/net_errno.h"
#include "socket.hpp"

#ifndef MAX_CONNECTIONS
    #define MAX_CONNECTIONS 10
#endif

namespace net
{
    struct API ConnectionList
    {
      public:
        ConnectionList() {}
        int operator+=(Socket *s);
        int operator-=(Socket *s);
        Socket *operator[](unsigned int i);
        int find(Socket *s);
        int count();

        struct pollfd *pfds;

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

        int setPollFlags(Socket* s, short flags);
        int PollConnections();

      private:
        ConnectionManager() { Connections.pfds = static_cast<struct pollfd*>(malloc(sizeof(struct pollfd*) * MAX_CONNECTIONS)); }
        ConnectionManager(ConnectionManager const &);
        ConnectionManager &operator=(ConnectionManager const &);

        static ConnectionManager *m_pInstance;
    };
}  // namespace net
