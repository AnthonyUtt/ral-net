#pragma once

#include "macros.h"
#include "net_errno.h"
#include "pch.h"

namespace net
{
    inline void *get_in_addr(struct sockaddr *sa)
    {
        if (sa->sa_family == AF_INET)
            return &((reinterpret_cast<struct sockaddr_in *>(sa))->sin_addr);
        return &((reinterpret_cast<struct sockaddr_in6 *>(sa))->sin6_addr);
    }

    template<typename T>
    struct Property
    {
      public:
        virtual ~Property() { }
        virtual T &operator=(const T &f) { return m_prop = f; }
        virtual const T &operator()() const { return m_prop; }
        virtual explicit operator const T &() const { return m_prop; }
        virtual T *operator->() { return &m_prop; }

      protected:
        T m_prop;
    };

    template<typename T>
    struct ReadOnlyProperty
    {
      public:
        virtual ~ReadOnlyProperty() { }
        virtual operator T const &() const { return m_prop; }

      protected:
        T m_prop;
    };

    template<typename T>
    struct Collection
    {
      public:
        virtual ~Collection() { }
        virtual int operator+=(T t)
        {
            if (Count == Size)
            {
                Size = Size * 2;

                m_list = static_cast<T*>(realloc(m_list, sizeof(T) * Size));
            }
            
            for (int i = 0; i < Count; i++)
            {
                if (m_list[i] == t)
                    return -1;
            }
            m_list[Count++] = t;
            return 0;
        }

        virtual int operator-=(T t)
        {
            for (int i = 0; i < Count; i++)
            {
                if (m_list[i] == t)
                {
                    m_list[i] = m_list[--Count];
                    return 0;
                }
            }
        }

        virtual T operator[](int i)
        {
            if (i < Count && i >= 0)
                return m_list[i];
        }

        int Count;
        int Size;
        T *m_list;
    };

    inline const char *net_strerror()
    {
        const char *msg;
        switch (net_errno)
        {
            case WSA_STARTUP_FAIL:
                msg = "Failure in WSAStartup()";
                break;
            case INVALID_SOCKET_TYPE:
                msg = "Invalid socket type";
                break;
            case GETADDRINFO_FAIL:
                msg = "Failure in getaddrinfo()";
                break;
            case SOCKET_FAIL:
                msg = "Failed to create socket";
                break;
            case LISTEN_FAIL:
                msg = "Failed to listen on socket";
                break;
            case HOST_OR_PORT_UNDEFINED:
                msg = "Both host and port were undefined";
                break;

            default:
                break;
        }
        return msg;
    }
}  // namespace net