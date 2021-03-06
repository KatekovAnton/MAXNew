//
//  IdGenerator.cpp
//  TerminalExtraction
//
//  Created by Katekov Anton on 11/1/12.
//
//

#include "IdGenerator.h"

static IdGenerator *_instance;

IdGenerator * IdGenerator::DefaultGenerator() {
    if (!_instance) {
        _instance = new IdGenerator();
    }
    return _instance;
}

unsigned int IdGenerator::GenerateId() {
    return _idPointer++;
}

void IdGenerator::Reset() {
    _idPointer = 0;
}

