//
//  MAXAnimationManager.h
//  MAX
//
//  Created by Anton Katekov on 30.01.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__AnimationManager__
#define __MAX__AnimationManager__

#include <iostream>
#include "MAXAnimationBase.h"
#include "miniPrefix.h"

class MAXAnimationManager {
    
    list<MAXAnimationBase*> _objects;
    
public:
    MAXAnimationManager();
    ~MAXAnimationManager();
    
    void AddAnimatedObject(MAXAnimationBase* object);
    void RemoveAnimatedObject(MAXAnimationBase* object);
    
    void Update();
};

#endif /* defined(__MAX__AnimationManager__) */
