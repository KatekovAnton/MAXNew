//
//  Responce.h
//  MAX
//
//  Created by Anton Katekov on 05.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__Responce__
#define __MAX__Responce__

#include <iostream>
#include "curl.h"

class Request;

class Response {
    
    friend class Request;
    
    size_t _size;
    CURLcode _status;
    
    char *_data;
    size_t _dataSize;
    
public:
    
    size_t GetResponseSize() const { return _size; }
    CURLcode GetStatus() const { return _status; }
    
    Response();
    ~Response();

    void AppendData(char *buffer,
                    size_t size,
                    size_t nitems);
    
    std::string ToString();

};

#endif /* defined(__MAX__Responce__) */
