#include "streamsocket.hpp"

namespace net
{    
    StreamSocket::StreamSocket(SocketType type, const char* node, const char* port)
    {
        switch (type)
        {
            case StreamListener:
            case StreamSender:
                m_type = type;
                break;
            
            default:
                net_errno = INVALID_SOCKET_TYPE;
                throw net_errno;
        }

        if (port == NULL && node == NULL)
        {
            net_errno = HOST_OR_PORT_UNDEFINED;
            throw net_errno;
        }

        m_node = node;
        m_port = port;

        struct addrinfo hints, *pai, *p;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        if (type == SocketType::StreamListener)
            hints.ai_flags = AI_PASSIVE;
        else
            hints.ai_flags = AI_ADDRCONFIG;
        
        int rv;
        if ((rv = getaddrinfo(m_node, m_port, &hints, &pai)) != 0)
        {
            net_errno = GETADDRINFO_FAIL;
            throw net_errno;
        }

        int yes = 1;
        for (p = pai; p != NULL; p = p->ai_next)
        {
            m_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if (m_fd < 0)
                continue;

            setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

            if (m_type == SocketType::StreamListener)
            {
                if (bind(m_fd, p->ai_addr, p->ai_addrlen) != 0)
                    continue;
            }

            break;
        }

        // If we got here, we didn't create the socket
        if (m_fd == -1)
        {
            net_errno = SOCKET_FAIL;
            throw net_errno;
        }

        freeaddrinfo(pai);

        if (m_type == SocketType::StreamListener)
        {
            if (listen(m_fd, 10) == -1)
            {
                net_errno = LISTEN_FAIL;
                throw net_errno;
            }
        }
    }

    int StreamSocket::disconnect()
    {
        return close(m_fd);
    }
}