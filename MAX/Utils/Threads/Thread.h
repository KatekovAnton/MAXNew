//
//  Thread.h
//  MAX
//
//  Created by Anton Katekov on 05.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__Thread__
#define __MAX__Thread__

#include <cstdlib>
#include <iostream>
#include <memory>

#include <pthread.h>

class Thread
{
public:
    Thread();
    virtual ~Thread();
    
    int start();
    int join();
    int detach();
    int kill();
    pthread_t self();
    
    virtual void run() = 0;
    
private:
    pthread_t  m_tid;
    int        m_running;
    int        m_detached;
};

#endif /* defined(__MAX__Thread__) */
