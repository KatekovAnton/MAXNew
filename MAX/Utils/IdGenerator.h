//
//  IdGenerator.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/1/12.
//
//

#ifndef __TerminalExtraction__IdGenerator__
#define __TerminalExtraction__IdGenerator__

#include <iostream>

class IdGenerator {
    static IdGenerator *_instance;
    
    unsigned int        _idPointer;
    
public:
    
    static IdGenerator * DefaultGenerator();
    
    unsigned int GenerateId();
    
    void Reset();
    
};

#endif /* defined(__TerminalExtraction__IdGenerator__) */
