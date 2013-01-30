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
#include "Utils.h"
#include "MAXAnimationObjectBase.h"

using namespace Utils;

class MAXAnimationManager {
    
    UContainer<MAXAnimationObjectBase> *_objects;
    
public:
    MAXAnimationManager();
    ~MAXAnimationManager();
    
    void AddAnimatedObject(shared_ptr<MAXAnimationObjectBase>& object);
    void RemoveAnimatedObject(shared_ptr<MAXAnimationObjectBase>& object);
    
    void Update();
};

#endif /* defined(__MAX__AnimationManager__) */
