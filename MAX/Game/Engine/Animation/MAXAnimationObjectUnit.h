//
//  MAXAnimationObjectUnit.h
//  MAX
//
//  Created by Anton Katekov on 30.01.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__MAXAnimationObjectUnit__
#define __MAX__MAXAnimationObjectUnit__

#include <iostream>
#include "MAXAnimationObjectBase.h"

class MAXAnimationObjectUnit:MAXAnimationObjectBase {
    
public:
    
    virtual bool IsFinished();
    virtual void Update(double time);
};

#endif /* defined(__MAX__MAXAnimationObjectUnit__) */
