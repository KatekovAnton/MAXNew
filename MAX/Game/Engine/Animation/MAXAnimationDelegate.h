//
//  MAXAnimationDelegate.h
//  MAX
//
//  Created by Anton Katekov on 01.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_MAXAnimationDelegate_h
#define MAX_MAXAnimationDelegate_h

class MAXAnimationBase;

class MAXAnimationDelegate {
    
public:
    
    virtual void OnAnimationStart(MAXAnimationBase* animation) = 0;
    virtual void OnAnimationUpdate(MAXAnimationBase* animation) = 0;
    virtual void OnAnimationFinish(MAXAnimationBase* animation) = 0;
    
};

#endif
