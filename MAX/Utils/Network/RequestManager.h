//
//  RequestManager.h
//  MAX
//
//  Created by Anton Katekov on 05.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__RequestManager__
#define __MAX__RequestManager__

#include <iostream>
#include <list>
#include "curl.h"

using namespace std;

class Request;
class Responce;

class RequestManager {
    
    friend class Request;
    list<Request*> _requests;
    
public:
    
    void ExecuteRequest(Request* request);
    void Flush();
    
};

#endif /* defined(__MAX__NetworkManager__) */
