#include "socket.hpp"

namespace net
{
    int Socket::get_fd()
    {
        return m_fd;
    }
}  // namespace net