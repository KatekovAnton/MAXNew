//
//  DebugStackTrace.h
//  MAX
//
//  Created by Anton Katekov on 05.09.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__DebugStackTrace__
#define __MAX__DebugStackTrace__

#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define DEBUG_UTILS DebugStackTrace::DebugStackTraceInstance()

#ifdef DEBUG
#define DEBUG_FUNCTION_MESSAGE DEBUG_UTILS->enterFunction(__FILE__, __FUNCTION__, __LINE__)
#define DEBUG_FUNCTION_EXIT DEBUG_UTILS->exitFunction()
#else
#define DEBUG_FUNCTION_MESSAGE
#endif

class DebugStackTrace {
    
    vector<string> _stackTrace;
    
public:
    
    static DebugStackTrace *DebugStackTraceInstance();
    
    void enterFunction(string file, string function, int line);
    void exitFunction();
    string fullStackTrace();
    
    
};

#endif /* defined(__MAX__DebugStackTrace__) */
