//
//  NetworkManager.cpp
//  MAX
//
//  Created by Anton Katekov on 05.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "RequestManager.h"
#include "Request.h"
#include "Response.h"
#include <vector>

using namespace std;

static RequestManager* _staticRequestManager = NULL;


RequestManager::RequestManager()
{}

RequestManager::~RequestManager()
{
    std::list<Request*>::iterator it = _requests.begin();
    for (; it != _requests.end(); it++) {
        Request* request = *it;
        if (!request->_finished) {
            request->kill();
        }
    }
}

void RequestManager::ExecuteRequest(Request* request)
{
    _requests.push_back(request);
    request->Thread::start();
}

void RequestManager::Flush()
{
    std::vector<Request*>finishedRequests;
    
    std::list<Request*>::iterator it = _requests.begin();
    bool finded = true;
    while (finded)
    {
        if(_requests.empty())
            break;
        finded = false;
        int i = 0;
        for (; it != _requests.end(); it++) {
            Request* request = *it;
            if (request->_finished) {
                finded = true;
                finishedRequests.push_back(request);
                _requests.remove(request);
                break;
            }
            i++;
        }
    }
    
    std::vector<Request*>::iterator it1 = finishedRequests.begin();
    for (; it1 != finishedRequests.end(); it1++)
    {
        Request* r = *it1;
        if(r->delegate)
            r->delegate->RequestDidFinishLoadingWithResponce(r, r->_response);
        if (r->_needAutoDelete) 
            delete r;
        
    }
}

RequestManager* RequestManager::SharedRequestManager()
{
    if (!_staticRequestManager) {
        _staticRequestManager = new RequestManager();
    }
    return _staticRequestManager;
}
