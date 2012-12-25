//
//  FileManger.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/3/12.
//
//

#ifndef __TerminalExtraction__FileManger__
#define __TerminalExtraction__FileManger__

#include <iostream>
#include "Geometry.h"

using namespace std;

class FileManager {
    
protected:

    
public:
    
    static FileManager *CreateManager();
    
    virtual const char *  GetFilePath(string filename);
    virtual string * GetContent(string filename);
    virtual GLubyte * CreateBitmapData(string filename, GLfloat *width, GLfloat *heigth);
    virtual GLubyte * CreateBitmapData(char* data, size_t size, GLfloat *width, GLfloat *heigth);
};

#endif /* defined(__TerminalExtraction__FileManger__) */
