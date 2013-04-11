#pragma once
#include "filemanger.h"
class windows_fileManager :
	public FileManager
{
	string appPath;
	wstring GetFilePathFromDir(wstring filename, wstring dir);
public:
	windows_fileManager(void);
	~windows_fileManager(void);

	virtual string GetFilePath(string filename);
	virtual string* GetContent(string filename);
};

