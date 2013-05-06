//
//  StringUtils.h
//  MAX
//
//  Created by  Developer on 16.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef __MAX__StringUtils__
#define __MAX__StringUtils__

#include <iostream>
#include "miniPrefix.h"


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

void split1(const string& str, const string& delimiters , vector<string>& tokens);

std::vector<std::string> splitString(const std::string &s, char delim);

std::vector<std::string> splitString(const std::string &s, string delim);

std::vector<std::string> getElements(string value);

vector<string> removeCommentsAndEmptyStrings(vector<string> lines);

string intToString(int value);

void removeBadCharacters(std::string &param);

string toLower(string value);

#endif /* defined(__MAX__StringUtils__) */
