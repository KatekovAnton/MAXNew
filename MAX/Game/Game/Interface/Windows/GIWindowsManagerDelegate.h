//
//  GIWindowsManagerDelegate.h
//  MAX
//
//  Created by Anton Katekov on 04.09.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_GIWindowsManagerDelegate_h
#define MAX_GIWindowsManagerDelegate_h

class GIWindowsManagerDelegate {
    
public:
    virtual void WindowManagerDidCloseLastWindow() = 0;
    virtual bool WindowManagerShouldMoveToFinishState() = 0;
};

#endif
