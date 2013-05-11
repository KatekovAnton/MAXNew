//
//  SoundEngineDelegate.h
//  MAX
//
//  Created by Anton Katekov on 11.05.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_SoundEngineDelegate_h
#define MAX_SoundEngineDelegate_h

class SoundEngineDelegate {
    
public:
    
    virtual void SoundDidFinishPlaying(int finishedSound) = 0;
    
};

#endif
