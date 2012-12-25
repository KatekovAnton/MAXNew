//
//  ios_fileManager.cpp
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 11/3/12.
//
//

#include "ios_fileManager.h"

const char * iosFileManager::GetFilePath(string filename) {
    size_t dotPosition = filename.find(".");
    string extension = filename.substr(dotPosition + 1);
    NSString* pureName = [[NSString stringWithUTF8String:filename.c_str()] substringToIndex:dotPosition];
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:pureName ofType:[NSString stringWithUTF8String:extension.c_str()]];
    
    return [filePath UTF8String];
}


string* iosFileManager::GetContent(string filename) {
    
    size_t dotPosition = filename.find(".");
    string extension = filename.substr(dotPosition + 1);
    NSString* pureName = [[NSString stringWithUTF8String:filename.c_str()] substringToIndex:dotPosition];
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:pureName ofType:[NSString stringWithUTF8String:extension.c_str()]];
    
    NSString *content = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:NULL];
    
    const char * cContent = [content UTF8String];
    int len = [content length];
 
    return new string(cContent, len);
    
}

GLubyte * iosFileManager::CreateBitmapData(string filename, GLfloat *width, GLfloat *heigth) {
    
    CGDataProviderRef dataProvider = CGDataProviderCreateWithFilename(GetFilePath(filename));
    
    CGImageRef image = CGImageCreateWithJPEGDataProvider(dataProvider, NULL, false, kCGRenderingIntentDefault);
    
    size_t i_width = CGImageGetWidth(image);
    size_t i_height = CGImageGetHeight(image);
    
    GLubyte * spriteData = new GLubyte[i_width * i_height * 4];
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData,
                                                       i_width,
                                                       i_height,
                                                       8,
                                                       i_width * 4,
                                                       CGImageGetColorSpace(image),
                                                       kCGImageAlphaPremultipliedLast);
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, i_width, i_height), image);
    
    CGDataProviderRelease(dataProvider);
    CGImageRelease(image);
    
    (* width) = i_width;
    (* heigth) = i_height;
    
    return spriteData;
}

GLubyte * iosFileManager::CreateBitmapData(char* data, size_t size, GLfloat *width, GLfloat *heigth) {
    
    CGDataProviderRef dataProvider = CGDataProviderCreateWithData(NULL, data, size, NULL); //CGDataProviderCreateWithFilename(GetFilePath(filename));
   
    CGImageRef image = CGImageCreateWithPNGDataProvider(dataProvider, NULL, false, kCGRenderingIntentDefault);
    
    size_t i_width = CGImageGetWidth(image);
    size_t i_height = CGImageGetHeight(image);
    
    GLubyte * spriteData = new GLubyte[i_width * i_height * 4];
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData,
                                                       i_width,
                                                       i_height,
                                                       8,
                                                       i_width * 4,
                                                       CGImageGetColorSpace(image),
                                                       kCGImageAlphaPremultipliedLast);
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, i_width, i_height), image);
    
    CGDataProviderRelease(dataProvider);
    CGImageRelease(image);
    
    (* width) = i_width;
    (* heigth) = i_height;
    
    return spriteData;
}
