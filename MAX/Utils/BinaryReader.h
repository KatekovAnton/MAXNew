//
//  BinaryReader.h
//  TerminalExtraction
//
//  Created by Mister Pattern on 11/19/12.
//
//

#ifndef __TerminalExtraction__BinaryReader__
#define __TerminalExtraction__BinaryReader__

#include <iostream>
#include <istream>
#include <fstream>
#include <iosfwd>
#include <stdio.h>

class BinaryReader {
    
    FILE            *_file;
    char            *_currentBuffer;
    char            *_internalBuffer;
    
    long            _filesize;
    long            _position;
    
public:
    
    BinaryReader(std::string filename);
    BinaryReader(char *buffer, unsigned int length);
    ~ BinaryReader();
	bool isValid() const { return !(!_file); };
    long GetPosition();
    int ReadInt();
    short ReadInt16();
    unsigned short ReadUInt16();
    float ReadSingle();
    char ReadChar();
    unsigned char ReadUChar();
    std::string ReadBadString();
    std::string ReadString();
	void ReadBuffer(int length, char *buf);
    void SetPosition(int position);
    
	long GetLength() { return _filesize; };
    
    std::string ReadFullAsString();
};

#endif /* defined(__TerminalExtraction__BinaryReader__) */
