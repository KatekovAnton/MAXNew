//
//  Request.cpp
//  MAX
//
//  Created by Anton Katekov on 05.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "Request.h"
#include "curl.h"
#include "Response.h"

void Request::run()
{
    Send();
}

const std::string RequestMethod::GET = "Get";
const std::string RequestMethod::POST = "Post";

Request::Request()
:_finished(false), _needAutoDelete(true)
{
    easy_handle = curl_easy_init();
    headers=NULL;
    hasHeader = false;
    _url = "";
    _response = new Response();
}

Request::~Request()
{
    if(headers)
        curl_slist_free_all(headers);
    curl_easy_cleanup(easy_handle);
    delete _response;
}

void Request::SetUrl(const std::string &url)
{
    _url = url;
}

void Request::SetMethod(const std::string &method)
{
    _method = method;
}

void Request::AddHeader(const char *header)
{
    hasHeader = true;
    headers = curl_slist_append(headers, header);
}

void Request::Send()
{
    if(_url == "")
        return;
    if(_method.compare(RequestMethod::GET)==0)
        GetMethod();
    else if(_method.compare(RequestMethod::POST)==0)
        PostMethod();
    else
        return;
    _finished = true;
}

bool Request::GetMethod()
{
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE,1L);
    curl_easy_setopt(easy_handle, CURLOPT_URL, _url.c_str());
    if(hasHeader)
        curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER,headers);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION,(void*)ReceiveFunc);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, this);
    CURLcode code = curl_easy_perform(easy_handle);
    _response->_status = code;
    return code == CURLE_OK;
}

bool Request::PostMethod()//not works yet
{
    
    if(hasHeader)
        curl_easy_setopt(easy_handle,CURLOPT_HTTPHEADER,headers);
    char *encodedURL = curl_easy_escape(easy_handle, _url.c_str(), _url.length());
    
  //  curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, RecvHeaderFunc);
    //curl_easy_setopt(easy_handle, CURLOPT_HEADERDATA,&head);
    
    curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS,encodedURL);
    curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE,strlen(encodedURL));
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION,(void*)ReceiveFunc);//receive callback function
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(easy_handle, CURLOPT_POST,1);
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE,1); /* open comment when debug mode.*/
    
    CURLcode code = curl_easy_perform(easy_handle);
    curl_free(encodedURL);
    _response->_status = code;
    return code == CURLE_OK;
}

size_t Request::ReceiveFunc(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    ((Request*)userdata)->_response->AppendData(ptr, size, nmemb);
    return size*nmemb;
}




