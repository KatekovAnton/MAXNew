//
//  Request.h
//  MAX
//
//  Created by Anton Katekov on 05.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__Request__
#define __MAX__Request__

#include <iostream>
#include "Thread.h"

#include<string>
#include "curl.h"
#include "RequestDelegate.h"

class RequestMethod
{
public:
    static const std::string GET;
    static const std::string POST;
};

class Response;
class RequestManager;

class Request : public Thread {
    
    friend class RequestManager;
    
public:
    
    RequestDelegate* delegate;
    
    bool _needAutoDelete;
    
    Request();
    ~Request();
    
    void SetUrl(const std::string &url);
    void SetMethod(const std::string &method);
    
    std::string GetMethod () const { return _method; }
    std::string GetUrl () const { return _url; }
    //Responce* GetResponce () const { return _responce; }
    
    void AddHeader(const char *header);
    bool Send();
    
    //thread method
    void* run();
    
    static size_t ReceiveFunc(char *ptr, size_t size, size_t nmemb, void *userdata);//receive data  callback function. you can handle recv data in this function.
    static void RecvHeaderFunc(char *ptr, size_t size, size_t nmemb, void *userdata);
    
private:
    
    bool _finished;
    
    bool GetMethod();
    bool PostMethod();
    
    std::string _method;
    std::string _url;
    
    CURL* easy_handle;
    curl_slist *headers;
    bool  hasHeader;
    
    
    Response* _response;
};



#endif /* defined(__MAX__Request__) */
