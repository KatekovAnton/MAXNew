//
//  Thread.cpp
//  MAX
//
//  Created by Anton Katekov on 05.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "Thread.h"

static void* runThread(void* arg)
{
    ((Thread*)arg)->run();
    return 0;
}

Thread::Thread()
: m_tid(0), m_running(0), m_detached(0)
{}

Thread::~Thread()
{
    if (m_running == 1) {
        pthread_cancel(m_tid);
    }
    if (m_running == 1 && m_detached == 0) {
        pthread_detach(m_tid);
    }
}

int Thread::start()
{
    int result = pthread_create(&m_tid, NULL, runThread, this);
    if (result == 0) {
        m_running = 1;
    }
    return result;
}

int Thread::join()
{
    int result = -1;
    if (m_running == 1) {
        result = pthread_join(m_tid, NULL);
        if (result == 0) {
            m_detached = 0;
        }
    }
    return result;
}

int Thread::kill()
{
    int result = -1;
    if (m_running == 1 && m_detached == 0) {
        result = pthread_kill(m_tid, 0);
        if (result == 0) {
            m_detached = 1;
        }
    }
    return result;
}

int Thread::detach()
{
    int result = -1;
    if (m_running == 1 && m_detached == 0) {
        result = pthread_detach(m_tid);
        if (result == 0) {
            m_detached = 1;
        }
    }
    return result;
}

pthread_t Thread::self() {
    return m_tid;
}