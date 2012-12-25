//
//  ios_fileManager.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/3/12.
//
//

#ifndef __TerminalExtraction__ios_fileManager__
#define __TerminalExtraction__ios_fileManager__

#include <iostream>

#include "FileManger.h"

class iosFileManager : public FileManager {
    
    
    
public:
    virtual const char *  GetFilePath(string filename);
    virtual string* GetContent(string filename);
    virtual GLubyte * CreateBitmapData(string filename, GLfloat *width, GLfloat *heigth);
    virtual GLubyte * CreateBitmapData(char* data, size_t size, GLfloat *width, GLfloat *heigth);
};

#endif /* defined(__TerminalExtraction__ios_fileManager__) */
