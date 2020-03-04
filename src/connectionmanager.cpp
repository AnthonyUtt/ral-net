#include "connectionmanager.hpp"
#include <iostream>

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
                        return 1;
                }
            }

            m_list[m_activeConns] = s;
            m_activeConns++;
            return 0;
        }
        else
        {
            // TODO: set net_errno
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
                    return 0;
                }
            }
        }
        return 1;
    }

    Socket* ConnectionList::operator[](unsigned int i)
    {
        if (i < m_activeConns)
            return m_list[i];
        return nullptr;
    }

    ConnectionManager *ConnectionManager::m_pInstance = nullptr;

    ConnectionManager *ConnectionManager::Instance()
    {
        if (!m_pInstance)
        {
            m_pInstance = new ConnectionManager;
        }
        return m_pInstance;
    }
}  // namespace net