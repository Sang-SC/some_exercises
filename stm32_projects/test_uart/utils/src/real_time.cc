#include "utils/real_time.h"

bool SetProcessHighPriority(unsigned int priority) {
    struct sched_param param;
    param.sched_priority = priority;
    int result = sched_setscheduler(0, SCHED_FIFO, &param);
    if (result != 0) {
        return false;
    }
    return true;
}