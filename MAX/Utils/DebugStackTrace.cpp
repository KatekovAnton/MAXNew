//
//  DebugStackTrace.cpp
//  MAX
//
//  Created by Anton Katekov on 05.09.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "DebugStackTrace.h"
#include "StringUtils.h"

using namespace std;

DebugStackTrace* statuicStackTrace = NULL;


DebugStackTrace *DebugStackTrace::DebugStackTraceInstance()
{
    if (!statuicStackTrace)
        statuicStackTrace = new DebugStackTrace();
    return statuicStackTrace;
}

void DebugStackTrace::enterFunction(string file, string function, int line, bool isNullPointer)
{
    vector<string> path = splitString(file, '/');
    string pureFile = path[path.size() - 1];
    string result = pureFile + "       " + function + "     line:" + std::to_string(line);
    if (isNullPointer) 
        result = result + "   Null Pointer Exception";
    
    _stackTrace.push_back(result);
}

void DebugStackTrace::exitFunction()
{
    _stackTrace.pop_back();
}

string DebugStackTrace::fullStackTrace()
{
    string result = "Stack trace:\n";
    for (int i = 0; i < _stackTrace.size(); i++) {
        result = result + _stackTrace[i];
        result = result + "\n";
    }
    return result;
}

void DebugStackTrace::log(string file, string function, int line, string message)
{
    vector<string> path = splitString(file, '/');
    string pureFile = path[path.size() - 1];
    string result = pureFile + " " + function + " line:" + std::to_string(line) + " message:" + message;
    
    _log.push_back(result);
}

string DebugStackTrace::fullLog()
{
    string result = "Application log:\n";
    for (int i = 0; i < _log.size(); i++) {
        result = result + _log[i];
        result = result + "\n";
    }
    return result;
}
