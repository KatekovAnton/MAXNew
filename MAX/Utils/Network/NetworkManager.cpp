//
//  NetworkManager.cpp
//  MAX
//
//  Created by Anton Katekov on 05.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "NetworkManager.h"
#include "Request.h"
#include "Response.h"
#include <vector>

using namespace std;

void NetworkManager::ExecuteRequest(Request* request)
{
    _requests.push_back(request);
}

void NetworkManager::Flush()
{
    std::vector<Request*>finishedRequests;
    
    std::list<Request*>::iterator it = _requests.begin();
    bool finded = true;
    while (finded)
    {
        finded = false;
    
        for (; it != _requests.end(); it++) {
            Request* request = *it;
            if (request->_finished) {
                finded = true;
                finishedRequests.push_back(request);
                _requests.remove(request);
                break;
            }
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

