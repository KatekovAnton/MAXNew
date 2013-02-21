//
//  FileManger.cpp
//  TerminalExtraction
//
//  Created by Katekov Anton on 11/3/12.
//
//

#include "FileManger.h"
#ifdef TARGET_OS_IPHONE
#include "ios_fileManager.h"
FileManager * FileManager::CreateManager()
{
    return new iosFileManager();
}
#endif
#ifdef TARGET_OS_WIN
#include "windows_fileManager.h"
FileManager * FileManager::CreateManager()
{
    return new windows_fileManager();
}
#endif


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