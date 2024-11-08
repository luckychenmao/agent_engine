
#include "LoggingEvent.h"

namespace alog {
int32_t sysPid = getpid();
thread_local int64_t sysTid = (long)syscall(SYS_gettid);
} // namespace alog
