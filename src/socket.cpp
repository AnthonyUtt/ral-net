#include "socket.hpp"

namespace net
{
    Socket::Socket(int fd) 
    {
        m_fd = fd;
    }

    int Socket::get_fd()
    {
        return m_fd;
    }

}  // namespace net