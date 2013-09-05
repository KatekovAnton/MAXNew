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
#define DEBUG_FUNCTION_ENTER(isNullPointer) DEBUG_UTILS->enterFunction(__FILE__, __FUNCTION__, __LINE__, isNullPointer)
#define DEBUG_FUNCTION_EXIT DEBUG_UTILS->exitFunction()
#else
#define DEBUG_FUNCTION_ENTER(isNullPointer)
#endif

#ifdef DEBUG
#define DEBUG_LOG(message) DEBUG_UTILS->log(__FILE__, __FUNCTION__, __LINE__, message)
#else
#define DEBUG_LOG(message)
#endif

class DebugStackTrace {
    
    vector<string> _stackTrace;
    
    
    vector<string> _log;
    
public:
    
    static DebugStackTrace *DebugStackTraceInstance();
    
    void enterFunction(string file, string function, int line, bool isNullPointer);
    void exitFunction();
    string fullStackTrace();
    
    
    void log(string file, string function, int line, string message);
    string fullLog();
    
    
};

#endif /* defined(__MAX__DebugStackTrace__) */
