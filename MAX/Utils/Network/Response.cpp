//
//  Responce.cpp
//  MAX
//
//  Created by Anton Katekov on 05.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "Response.h"

Response::Response()
:_size(0), _dataSize(1024)
{
    _data = (char*)malloc(1024);
}

Response::~Response()
{
    free(_data);
    printf("!!!!");
}

void Response::AppendData(char *buffer,
                          size_t size,
                          size_t nitems)
{
    size_t rembuff = 0;
    size *= nitems;
    
    rembuff= _dataSize - _size; /* remaining space in buffer */
    
    
    if(size > rembuff)
    {
        
        int times = size / 1024;
        times ++;
        size_t add = times * 1024;
        
        char *newbuff = (char*)realloc((void*)_data, _size + add);
        if(newbuff==NULL)
        {
            throw "cant allocate memory for responce!";
        }
        else
        {
            _dataSize = _size + add;
            _data = newbuff;
        }
    }
    memcpy(_data + _size, buffer, size);
    _size = size + _size;
}

std::string Response::ToString()
{
    std::string result = std::string(_data);
    return result;
}
