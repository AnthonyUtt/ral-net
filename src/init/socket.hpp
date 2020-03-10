#pragma once

#include "macros.h"
#include "pch.h"

namespace net
{    
    enum API SocketType
    {
        StreamListener,
        StreamSender,
        DatagramListener,
        DatagramSender,
        ConnectedDatagramListener,
        ConnectedDatagramSender
    };

    class API Socket {
      public:
        int get_fd();

      protected:
        int m_fd = -1;
    };
}  // namespace net