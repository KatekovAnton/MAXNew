//
//  FileManger.cpp
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/3/12.
//
//

#include "FileManger.h"
#include "ios_fileManager.h"

FileManager * FileManager::CreateManager()
{
    return new iosFileManager();
}

const char * FileManager::GetFilePath(string filename)
{
    return NULL;
}

string * FileManager::GetContent(string filename)
{
    return new string("");
}

GLubyte * FileManager::CreateBitmapData(string filename, GLfloat *width, GLfloat *heigth)
{
    return NULL;
}

GLubyte * FileManager::CreateBitmapData(char* data, size_t size, GLfloat *width, GLfloat *heigth)
{
    return NULL;
}