// wraps memory maps and using the opensavefiledialog
#ifndef WINAPI_FILE_H
#define WINAPI_FILE_H

#include <windows.h>
#include <string>
#include <commdlg.h>
#include <cderr.h>
#include <vector>
#include "winapi_RAII.h"
#include "winapi_DataConversion.h"

struct MemoryMap 
{
public:
	// this maps a file to memory or creates a shared memory. The goal here is simplifying and compartmentalizing
	// the common and ugly process of memory maping at the expense of control and customizability.
	// This currently only supports file and shared memory maps under 2^32-1 bytes in size, i.e. 4 gigabytes..
	enum MapType  { NoMap=0, FileMap=1, SharedMemoryMap=2};
	enum MapFlags { Read=1, Write=2};

	HANDLE m_hFile;
	HANDLE m_hFileMappingObject;
	BYTE*  m_FileMapPtr; 
	DWORD  m_MapSize; 

	DWORD m_FileAccess;       // for file maps
	DWORD m_SharedAccess;     // for file maps
	DWORD m_MapObjProtection; // for file and shared maps
	DWORD m_MapAccess;        // for file and shared maps
	
	DWORD m_MapFlags;         // for file and shared maps

	tstring m_FilePath;   // used by both file and shared maps
	tstring m_Name;       // used by both file and shared maps
	int m_MapState;

	MemoryMap();
	// mapType - a mapType designates either a file map or a shared memory map
	// name - is either the the file path or the shared memory name based on the mapType.
	// 
	// return type - no return type, if m_FileMapPtr is NULL, the mapping failed.
	MemoryMap(LPCTSTR name, DWORD mapType, DWORD mapFlags, size_t mapSize = 0);
	~MemoryMap();

	// filePath - file path of the file to map
	// mapFlags		 - This can either be Read, Write or Read and Write
	// mapSize        - size in bytes of how much of the file to map. A value of NULL will map the whole file
	// 
	// return value          - This function returns false if it fails. If the function fails a message box will appear giving some information as to why the operation failed. No further information about failure can be retrieved.
	bool MemoryMapFile(LPCTSTR filePath, DWORD mapFlags, size_t mapSize = 0);

	// sharedMemoryName - is the name of the shared memory, this can be NULL. 
	// mapFlags		 - This can either be Read, Write or Read and Write
	// mapSize        - size in bytes of how much memory to map for the shared memory, this can't be NULL.
	//
	// return value          - This function returns false if it fails. If the function fails a message box will appear giving some information as to why the operation failed. No further information about failure can be retrieved.
	bool MemoryMapSharedMem(LPCTSTR sharedMemoryName, DWORD mapFlags, size_t mapSize);

	// CleanUpMemory checks to see if any of the handles or the filemap is in use, and closes them if they are.
	void CleanUpMemory();

	DWORD GetFileMapSize(); // returns the size of a file map
	bool ResizeMap(DWORD newMapSize); // resizes the map
}; 

class OpenSaveFileDialog
{
public:
	OPENFILENAME m_ofn;
	TCHAR m_Filtertstring[0x400];
	TCHAR m_FileNameBuffer[0x400];
	TCHAR m_InitialDirectory[0x400];

	// Ctors/Dtors/Etc..
	OpenSaveFileDialog();
	~OpenSaveFileDialog();

	// open and save functions
	bool OpenFile();
	bool SaveFile();
};

class File
{
public:
	File() {} 
	~File() {}

	void GenerateFileList(const tstring& directoryPath);  // generate a list of files from a directory path
	void GenerateFileListRecusively(const tstring& directoryPath);
	void PruneFileList(std::vector<tstring>& filesTypes); // prune the list of files of all types except those declared in fileTypes

	static tstring GetModuleDirectoryPath();

	std::vector<tstring> & List() { return fileList; }

private:
	std::vector<tstring> fileList;

};

class NewLine
{
public:
	NewLine() {}
	~NewLine() {}
	std::string GetFormat() { return Format(); }
	void SetFormat(std::string& s) { Format() = s; }
	void SetFormat(const char * const s) { Format() = s; }

	bool IsNewLine(BYTE* p);
	bool IsNewLine(char* p);
	bool SkipNewLine(BYTE*& p);
	bool SkipNewLine(char*& p);

private:
	
	//static NewLine*& MyNewLine() { static NewLine* nl = 0; return nl; }
	static std::string& Format() { static std::string s = "\r\n"; return s; }
};

inline bool NewLine::IsNewLine(BYTE* p)
{ // let p := file pointer
	std::string s(p, p + GetFormat().size()); 
	
	if ( s == GetFormat())
		return true;
	else
		return false;
}

inline bool NewLine::IsNewLine(char* p)
{ // let p := file pointer
	return IsNewLine((BYTE*)p);
}

inline bool NewLine::SkipNewLine(BYTE*& p)
{ // let p := file pointer
	if ( IsNewLine(p) ) {
		p += GetFormat().size(); // skip the line
		return true;
	}

	return false;
}

inline bool NewLine::SkipNewLine(char*& p)
{ // let p := file pointer
	return SkipNewLine((BYTE*&)p);
}

#endif

