#pragma once

#include "init/macros.h"
#include "init/pch.h"
#include "init/net_errno.h"
#include "init/util.h"

namespace net
{    
    enum API SocketType
    {
        Any,
        StreamSocket,
        DatagramSocket
    };

    class API Socket {
      public:
        Socket(SocketType type, const char *node = NULL, const char *port = NULL, int flags = 0);
        Socket(int fd);

        int Open(bool bind = false);
        int Connect();
        int Connect(struct addrinfo *dest);
        int BeginListen();
        Socket AcceptConnection(struct sockaddr_storage *remote_addr);
        int Send(const void *buf, ssize_t *len, int flags = 0);
        int Receive(void *buf, ssize_t *len, int flags = 0);
        int Close();

        Property<bool> Active;

        int get_fd();
        const char *get_node();
        const char *get_port();
        SocketType get_type();
        int get_flags();

      private:
        struct addrinfo *m_addr;
        int m_fd = -1;
        const char *m_node;
        const char *m_port;
        SocketType m_type;
        int m_flags;
    };
}  // namespace net