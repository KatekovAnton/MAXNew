//
//  EngineTiledDynamicMeshDelegate.h
//  MAX
//
//  Created by  Developer on 30.03.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_EngineTiledDynamicMeshDelegate_h
#define MAX_EngineTiledDynamicMeshDelegate_h

class EngineTiledDynamicMeshDelegate {
    
public:
    virtual void ElementDidChangePosition(int oldIndex, int newIndex, int singleArrayIndex) = 0;
};

#endif
