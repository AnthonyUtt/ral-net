#pragma once

#include "macros.h"

#define WSA_STARTUP_FAIL 0x0001
#define INVALID_SOCKET_TYPE 0x0002
#define GETADDRINFO_FAIL 0x0003
#define SOCKET_FAIL 0x0004
#define LISTEN_FAIL 0x0005
#define HOST_OR_PORT_UNDEFINED 0x0006
#define CONNECT_FAIL 0x0007
#define SEND_FAIL 0x0008
#define RECV_FAIL 0x0009
#define REMOTE_CONN_CLOSED 0x000A
#define ACCEPT_FAIL 0x000B
#define CONN_ALREADY_OPEN 0x000C
#define BAD_FILE_DESC 0x000D
#define NULL_ADDRINFO 0x000E
#define DUPLICATE_SOCKET 0x000F
#define MAX_CONNS_REACHED 0x0010
#define CONN_NOT_FOUND 0x0011
#define NO_CONNS_PRESENT 0x0012
#define INDEX_OUT_OF_BOUNDS 0x0013
#define POLL_FAIL 0x0014
#define GETSOCKOPT_FAIL 0x0015
#define CALLBACK_FAILURE 0x0016

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