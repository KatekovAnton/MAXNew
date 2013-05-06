//
//  StringUtils.cpp
//  MAX
//
//  Created by  Developer on 16.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#include "StringUtils.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    string::size_type pos = 0;
    while (true)
    {
        string::size_type startPos = s.find_first_of(delim, pos);
        if (startPos == std::string::npos)
        {
            if (pos != s.length())
                elems.push_back(s.substr(pos, s.length() - pos));
            
            break;
        }
        elems.push_back(s.substr(pos, startPos - pos));
        pos = startPos + 1;
    }
    
    //    while(std::getline(ss, item, delim))
    //    {
    //        elems.push_back(item);
    //    }
    return elems;
}

void split1(const string& str, const string& delimiters , vector<string>& tokens)
{
    string::size_type pos = 0;
    while (true)
    {
        string::size_type startPos = str.find(delimiters.c_str(), pos);
        if (startPos == std::string::npos)
            break;
        startPos += delimiters.length();
        string::size_type endPos = str.find(delimiters.c_str(), startPos+2);
        if (endPos == std::string::npos)
        {
            tokens.push_back(str.substr(0, startPos));
            tokens.push_back(str.substr(startPos, str.length() - startPos));
            break;
        }
        
        tokens.push_back(str.substr(startPos, endPos - startPos));
        pos = endPos;
    }
}

std::vector<std::string> splitString(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

std::vector<std::string> splitString(const std::string &s, string delim)
{
    std::vector<std::string> elems;
    split1(s, delim, elems);
    return elems;
}

std::vector<std::string> getElements(string value)
{
    size_t index = 0;
    while (true) {
        index = value.find("  ", 0);
        if (index == string::npos)
            break;
        value.replace(index, 2, " ");
    }
    vector<string> elements = splitString(value, ' ');
    return elements;
}

const string commentSign = "//";

vector<string> removeCommentsAndEmptyStrings(vector<string> lines)
{
    vector<string> result;
    
    for (int i = 0; i < lines.size(); i++)
    {
        string str = lines[i];
        if (str.length() == 0) {
            continue;
        }
        string::size_type startPos = str.find(commentSign.c_str(), 0);
        if (startPos != std::string::npos)
        {
            if (startPos == 0)
                continue;
            
            string result1 = str.substr(0, startPos);
            result.push_back(result1);
        }
        else
            result.push_back(str);
    }
    
    return result;
}


string intToString(int value)
{
    std::stringstream ss;
    ss<<value;
    std::string str;
    ss>>str;
    return str;
}

void removeBadCharacters(std::string &param)
{
	remove(param.begin(), param.end(), '\r');
}

string toLower(string value)
{
    string result = value;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}
