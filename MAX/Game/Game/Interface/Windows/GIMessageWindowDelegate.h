//
//  GIMessageWindowDelegate.h
//  MAX
//
//  Created by Anton Katekov on 04.09.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_GIMessageWindowDelegate_h
#define MAX_GIMessageWindowDelegate_h

class GIMessageWindow;

class GIMessageWindowDelegate {
    
public:
    
    virtual void OnMessageWindowButtonClicked(int buttonNumber, GIMessageWindow *sender) = 0;
    
};

#endif
