#include "windows_fileManager.h"
#include <string>
#include <algorithm>

void string_to_wstring(const std::string& src, std::wstring& dest)
{
    std::wstring tmp;
    tmp.resize(src.size());
    std::transform(src.begin(),src.end(),tmp.begin(),btowc);
    tmp.swap(dest);
}
    
// convert wstring_to_string
void wstring_to_string(const std::wstring& src, std::string& dest)
{
    std::string tmp;
    tmp.resize(src.size());
    std::transform(src.begin(),src.end(),tmp.begin(),wctob);
    tmp.swap(dest);
}

windows_fileManager::windows_fileManager(void)
{
	const unsigned len = GetCurrentDirectory(0, 0);
	string dir(len, '\0');
	wstring str(len,'\0');
	GetCurrentDirectory(len, &str[0]);
	wstring_to_string(str,dir);
	appPath = dir;
}


windows_fileManager::~windows_fileManager(void)
{
}

string* windows_fileManager::GetContent(string filename)
{
	string path = GetFilePath(filename);
	FILE* f = fopen(filename.c_str(), "r");

	// Determine file size
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);

	char* where1 = new char[size];

    rewind(f);
    fread(where1, sizeof(char), size, f);

   
	string * result = new string(where1);
	delete[] where1;
    fclose(f);
    return result;
}

string  windows_fileManager::GetFilePath(string filename)
{
	string delemiter = string("\\");
	delemiter.append(filename);
	string pdf = string(appPath.c_str());
	pdf.append(delemiter);
	return pdf;
}