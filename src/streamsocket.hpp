#pragma once

#include "init/macros.h"
#include "init/pch.h"
#include "init/socket.hpp"
#include "init/net_errno.h"

namespace net
{
    class API StreamSocket : Socket
    {
        public:
            StreamSocket(SocketType type, const char* node = NULL, const char* port = NULL);
            int disconnect();

        private:
            struct sockaddr_storage* m_remoteHost;
            const char* m_node;
            const char* m_port;
            SocketType m_type;
            bool m_active;
    };
}