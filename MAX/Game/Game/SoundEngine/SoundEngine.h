//
//  SoundEngine.h
//  MAX
//
//  Created by Anton Katekov on 08.05.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__SoundEngine__
#define __MAX__SoundEngine__

#include <iostream>
#include "miniPrefix.h"

class SoundEngine {
    
    //map<string, ALUn> playedSound;
    
public:
    
    void PlaySound(string filename, bool looped);
    
};

#endif /* defined(__MAX__SoundEngine__) */
