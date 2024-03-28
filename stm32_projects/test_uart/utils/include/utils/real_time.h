#pragma once
#include <sched.h>  // 用于设置实时性调度策略的头文件

bool SetProcessHighPriority(unsigned int priority);