#pragma once

#include "macros.h"

#define WSA_STARTUP_FAIL 0x0001
#define INVALID_SOCKET_TYPE 0x0002
#define GETADDRINFO_FAIL 0x0003
#define SOCKET_FAIL 0x0004
#define LISTEN_FAIL 0x0005
#define HOST_OR_PORT_UNDEFINED 0x0006

namespace net
{
    struct API __net_errno
    {
      public:
        static __net_errno *ins;
        void operator=(int i) { err = i; }
        operator unsigned int const &() const { return err; }
        static __net_errno *instance();

      private:
        unsigned int err;
    };
}  // namespace net

#define net_errno *net::__net_errno::instance()