#include "windows_fileManager.h"
#include <string>
#include <algorithm>

//#define COCOS2D_DEBUG 1

#include "cocos2d.h"

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
	FILE* f = fopen(path.c_str(), "r");

	// Determine file size
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f) + 1;

	char* where1 = new char[size];
	memset(where1, 0, size);
    rewind(f);
    fread(where1, sizeof(char), size - 1, f);
	where1[size - 1] = 0;
   
	string * result = new string(where1);
	delete[] where1;
    fclose(f);
    return result;
}

wstring windows_fileManager::GetFilePathFromDir(wstring filename, wstring dir)
{
	WIN32_FIND_DATA fd; 
	HANDLE handle;
	wstring result = L"";   
	wstring delemiter = L"\\";
	
	wstring strSpec = dir + delemiter + L"*.*";
    handle = FindFirstFile(strSpec.c_str(), &fd);
    if (handle != INVALID_HANDLE_VALUE)
	{
		do 
		{
			strSpec = fd.cFileName;
			if((strSpec != L".") && (strSpec != L".."))
			{
				if( !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) )
				{
					if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
					{
						strSpec = GetFilePathFromDir(filename, dir + delemiter + strSpec);
						if (strSpec.size() > 0)
						{
							result = strSpec; // found on sub dir
							break;
						}
					}
					else if (strSpec == filename)
					{
						if((fd.nFileSizeLow != 0) || (fd.nFileSizeHigh != 0)) 
						{
							result = dir + delemiter + strSpec;
							break;
						}
					}
				}
			}
		} while(FindNextFile(handle, &fd));
		FindClose(handle);
	}
	return result;
}


string windows_fileManager::GetFilePath(string filename)
{
	wstring path (appPath.begin(), appPath.end());
	path = L".";
	wstring fName (filename.begin(), filename.end());
	wstring result = GetFilePathFromDir(fName, path);
	string results;
	wstring_to_string(result, results);
	CCLOG("file: %s\npath: %s\n\n", filename.c_str(), results.c_str());
	return results;
}