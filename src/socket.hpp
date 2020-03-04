#pragma once

#include "init/macros.h"
#include "init/pch.h"

namespace net
{
    class API Socket {
      public:
        Socket(int fd);
        int get_fd();

      private:
        void test();
        int m_fd;
    };
}  // namespace net