//
// Created by gaojian on 2019/11/30.
//

#include "ThreadPool.hpp"

thread_local WorkStealingQueue *ThreadPool::threadWorkQueue;
thread_local unsigned ThreadPool::threadId;