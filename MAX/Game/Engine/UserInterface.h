//
//  UserInterface.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/11/12.
//
//

#ifndef __TerminalExtraction__UserInterface__
#define __TerminalExtraction__UserInterface__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class UserInterface {
    
    CCArray           *_guiNodes;
    
    CCScene           *_guiScene;
    
public:
    
    UserInterface();
    ~UserInterface();
    
    void Prepare();
    
    CCScene * GetGUISession();
};

#endif /* defined(__TerminalExtraction__UserInterface__) */
