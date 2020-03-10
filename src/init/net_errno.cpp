#include "net_errno.h"

namespace net
{
    __net_errno *__net_errno::ins;

    __net_errno *__net_errno::instance()
    {
        if (!ins)
        {
            ins = new __net_errno;
            *ins = 0x0000;
        }
        return ins;
    }
}  // namespace net