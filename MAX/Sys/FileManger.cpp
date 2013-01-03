//
//  FileManger.cpp
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/3/12.
//
//

#include "FileManger.h"
#ifdef TARGET_OS_IPHONE
#include "ios_fileManager.h"
#endif
#ifdef TARGET_OS_WIN
#include "windows_fileManager.h"
#endif
FileManager * FileManager::CreateManager()
{
    return new windows_fileManager();
}

string FileManager::GetFilePath(string filename)
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