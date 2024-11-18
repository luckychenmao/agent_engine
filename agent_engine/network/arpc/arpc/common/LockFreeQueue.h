#include "agent_engine/util/lock_free_queue.h"

namespace arpc::common {

template <typename T>
using LockFreeQueue = ::util::LockFreeQueue<T>;

}
