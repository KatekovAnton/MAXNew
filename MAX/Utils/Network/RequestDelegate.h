//
//  RequestDelegate.h
//  MAX
//
//  Created by Anton Katekov on 05.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_RequestDelegate_h
#define MAX_RequestDelegate_h

using namespace std;

class Request;
class Response;

class RequestDelegate {
    
public:
    
    virtual void RequestDidFinishLoadingWithResponce(Request* request, Response* response) = 0;
};

#endif
