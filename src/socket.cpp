#include "socket.hpp"

namespace net
{
    Socket::Socket(SocketType type, const char *node, const char *port, int flags)
    {
        switch (type)
        {
            case StreamSocket:
            case DatagramSocket:
                m_type = type;
                break;
            
            default:
                net_errno = INVALID_SOCKET_TYPE;
                throw net_errno;
        }

        if (m_node == NULL && m_port == NULL)
        {
            net_errno = HOST_OR_PORT_UNDEFINED;
            throw net_errno;
        }

        m_node = node;
        m_port = port;

        m_flags = flags;

        Children.Count = 0;
        Children.Size = 10;
        Children.m_list = static_cast<Socket**>(malloc(sizeof(Socket*) * Children.Size));
    }

    Socket::Socket(int fd)
    {
        if (fd == -1)
        {
            net_errno = BAD_FILE_DESC;
            throw net_errno;
        }

        m_fd = fd;
    }

    int Socket::Open(bool _bind)
    {
        if (m_fd != -1)
        {
            net_errno = CONN_ALREADY_OPEN;
            return -1;
        }

        int gai_rv;
        int yes = 1;
        struct addrinfo hints, *pai, *p;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = m_type;
        hints.ai_flags = m_flags;
        
        if ((gai_rv = getaddrinfo(m_node, m_port, &hints, &pai)) != 0)
        {
            net_errno = GETADDRINFO_FAIL;
            return -1;
        }

        for (p = pai; p != NULL; p = p->ai_next)
        {
            m_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if (m_fd < 0)
                continue;

            setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

            if (_bind)
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
            return -1;
        }

        freeaddrinfo(pai);

        return 0;
    }

    int Socket::Connect()
    {
        if (m_fd == -1)
        {
            net_errno = BAD_FILE_DESC;
            return -1;
        }

        if (!m_addr)
        {
            net_errno = NULL_ADDRINFO;
            return -1;
        }

        int rv = connect(m_fd, m_addr->ai_addr, m_addr->ai_addrlen);
        if (rv != 0)
            net_errno = CONNECT_FAIL;
        return rv;
    }

    int Socket::Connect(struct addrinfo *dest)
    {
        if (m_fd == -1)
        {
            net_errno = BAD_FILE_DESC;
            return -1;
        }

        int rv = connect(m_fd, dest->ai_addr, dest->ai_addrlen);
        if (rv != 0)
            net_errno = CONNECT_FAIL;
        return rv;
    }

    int Socket::BeginListen()
    {
        if (m_fd == -1)
        {
            net_errno = BAD_FILE_DESC;
            return -1;
        }

        int rv = listen(m_fd, 10);
        if (rv != 0)
            net_errno = LISTEN_FAIL;
        return rv;
    }

    Socket *Socket::AcceptConnection(struct sockaddr_storage *remote_addr)
    {
        if (m_fd == -1)
        {
            net_errno = BAD_FILE_DESC;
            throw net_errno;
        }

        socklen_t addr_len = sizeof(*remote_addr);
        int newfd = accept(m_fd, reinterpret_cast<struct sockaddr*>(remote_addr), &addr_len);
        if (newfd == -1)
        {
            net_errno = ACCEPT_FAIL;
            throw net_errno;
        }

        return new Socket(newfd);
    }

    int Socket::Send(const void *buf, ssize_t *len, int flags)
    {
        if (m_fd == -1)
        {
            net_errno = BAD_FILE_DESC;
            return -1;
        }

        ssize_t total_sent = 0;
        ssize_t bytes_left = *len;
        ssize_t n;
        while (total_sent < bytes_left)
        {
            n = send(m_fd, static_cast<const void*>(&static_cast<const char*>(buf)[total_sent]), bytes_left, flags);
            if (n == -1)
                {
                    net_errno = SEND_FAIL;
                    break;
                }
            total_sent += n;
            bytes_left -= n;
        }

        *len = total_sent;

        return n == -1 ? -1 : 0;
    }

    int Socket::Receive(void *buf, ssize_t *len, int flags)
    {
        if (m_fd == -1)
        {
            net_errno = BAD_FILE_DESC;
            return -1;
        }

        ssize_t num_bytes = recv(m_fd, buf, *len, flags);
        if (num_bytes == -1)
            net_errno = RECV_FAIL;
        else if (num_bytes == 0)
            net_errno = REMOTE_CONN_CLOSED;
        
        *len = num_bytes;
        
        return num_bytes == -1 ? -1 : 0;
    }

    int Socket::Close()
    {
        if (m_fd == -1)
        {
            net_errno = BAD_FILE_DESC;
            return -1;
        }

        int rv = close(m_fd);
        m_fd = -1;
        return rv;
    }

    int Socket::get_fd()
    {
        return m_fd;
    }

    const char *Socket::get_node()
    {
        return m_node;
    }

    const char *Socket::get_port()
    {
        return m_port;
    }

    SocketType Socket::get_type()
    {
        return m_type;
    }

    int Socket::get_flags()
    {
        return m_flags;
    }
}  // namespace net