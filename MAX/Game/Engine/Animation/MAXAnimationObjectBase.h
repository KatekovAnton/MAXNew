//
//  MAXAnimationObjectBase.h
//  MAX
//
//  Created by Anton Katekov on 30.01.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXAnimationObjectBase__
#define __MAX__MAXAnimationObjectBase__

#include <iostream>

class MAXAnimationObjectBase {
    
public:
    
    virtual bool IsFinished() = 0;
    virtual void Update(double time) = 0;
    
};

#endif /* defined(__MAX__MAXAnimationObjectBase__) */
