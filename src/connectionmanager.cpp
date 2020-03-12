#include "connectionmanager.hpp"

namespace net
{
    int ConnectionList::operator+=(Socket *s)
    {
        if (m_activeConns < MAX_CONNECTIONS)
        {
            if (m_activeConns > 0)
            {
                for (unsigned int i = 0; i < m_activeConns; i++)
                {
                    if (m_list[i] == s)
                    {
                        net_errno = DUPLICATE_SOCKET;
                        return -1;
                    }
                }
            }
            m_list[m_activeConns] = s;
            pfds[m_activeConns].fd = s->get_fd();
            m_activeConns++;

            return 0;
        }
        else
        {
            net_errno = MAX_CONNS_REACHED;
            return -1;
        }
    }

    int ConnectionList::operator-=(Socket *s)
    {
        if (m_activeConns > 0)
        {
            for (unsigned int i = 0; i < m_activeConns; i++)
            {
                if (m_list[i] == s)
                {
                    m_list[i] = m_list[--m_activeConns];
                    pfds[i] = pfds[m_activeConns];
                    return 0;
                }
            }
            net_errno = CONN_NOT_FOUND;
            return -1;
        }
        net_errno = NO_CONNS_PRESENT;
        return -1;
    }

    Socket *ConnectionList::operator[](unsigned int i)
    {
        if (i < m_activeConns)
            return m_list[i];
        else
            net_errno = INDEX_OUT_OF_BOUNDS;
    }

    int ConnectionList::find(Socket *s)
    {
        for (unsigned int i = 0; i < m_activeConns; i++)
        {
            if (m_list[i] == s)
                return i;
        }
        net_errno = CONN_NOT_FOUND;
        return -1;
    }

    int ConnectionList::count()
    {
        return m_activeConns;
    }

    ConnectionManager *ConnectionManager::m_pInstance = nullptr;

    ConnectionManager *ConnectionManager::Instance()
    {
        if (!m_pInstance)
        {
            m_pInstance       = new ConnectionManager;
        }
        return m_pInstance;
    }

    int ConnectionManager::setPollFlags(Socket *s, short flags)
    {
        int i = Connections.find(s);
        if (i == -1)
        {
            return -1;
        }
        Connections.pfds[i].events = flags;
        return 0;
    }

    int ConnectionManager::PollConnections()
    {
        if (Connections.count() == 0)
        {
            net_errno = NO_CONNS_PRESENT;
            return -1;
        }

        int poll_ret = poll(Connections.pfds, Connections.count(), -1);
        if (poll_ret == -1)
        {
            net_errno = POLL_FAIL;
            return -1;
        }

        for (int i = 0; i < Connections.count(); i++)
        {
            if (Connections.pfds[i].revents & Connections.pfds[i].events)
            {
                int listener;
                socklen_t opt_len = sizeof(listener);
                int rv = getsockopt(Connections.pfds[i].fd, SOL_SOCKET, SO_ACCEPTCONN, &listener, &opt_len);
                if (rv == -1)
                {
                    net_errno = GETSOCKOPT_FAIL;
                    return -1;
                }

                if (listener)
                {
                    struct sockaddr_storage remoteAddr;
                    Socket *s = Connections[i]->AcceptConnection(&remoteAddr);
                    Connections += s;
                    setPollFlags(s, POLLIN);

                    // Manage parent-child link
                    Connections[i]->Children += s;
                    s->Parent = Connections[i];

                    // Parent callbacks are passed on to children
                    s->callback = Connections[i]->callback;
                }
                else
                {
                    // Callback function with data
                    char buf[MAX_BUF_LEN];
                    ssize_t bufLen = sizeof(buf);
                    memset(buf, 0, bufLen);

                    rv = Connections[i]->Receive(buf, &bufLen);
                    if (rv == -1)
                    {
                        return -1;
                    }

                    rv = (*Connections[i]->callback)(buf, &bufLen);
                    if (rv == -1)
                    {
                        net_errno = CALLBACK_FAILURE;
                        return -1;
                    }
                }
            }
        }
        return 0;
    }
}  // namespace net