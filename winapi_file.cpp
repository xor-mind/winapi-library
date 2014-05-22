#include "winapi_file.h"
#include "Shlobj.h"
#include "Shlwapi.h"
#include "winapi_error.h"

// class MemoryMap implementation ------------------------------------------------------------------------------ //
// TODO: Add a conditional branch for dwCreationDisposition, based on READ and or WRITE..
MemoryMap::MemoryMap()
{
	m_hFile = NULL;
	m_hFileMappingObject = NULL;
	m_FileMapPtr = NULL;
	m_MapState = NoMap;
}

MemoryMap::MemoryMap(LPCTSTR name, DWORD mapType, DWORD mapFlags, size_t mapSize)
{
	m_hFile = NULL;
	m_hFileMappingObject = NULL;
	m_FileMapPtr = NULL;
	m_MapState = NoMap;

	if (mapType == FileMap)
	{
		MemoryMapFile(name, mapFlags, mapSize);
	}
	else
	{
		MemoryMapSharedMem(name, mapFlags, mapSize);
	}
}

MemoryMap::~MemoryMap()
{
	CleanUpMemory();
}

void MemoryMap::CleanUpMemory()
{
	if (m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}

	if (m_hFileMappingObject)
	{
		CloseHandle(m_hFileMappingObject);
		m_hFileMappingObject = NULL;
	}

	if (m_FileMapPtr)
	{
		UnmapViewOfFile(m_FileMapPtr);
		m_FileMapPtr = NULL;
	}

	//m_FileAccess = 0;
	//m_SharedAccess = 0;
	//m_MapObjProtection = 0;
	//m_MapAccess = 0;

	m_MapState = NoMap;
}

bool MemoryMap::MemoryMapFile(LPCTSTR filePath, DWORD mapFlags, size_t mapSize)
{ 
	CleanUpMemory(); // make sure to close any handles possibly in use.

	m_FilePath = filePath;
	if (m_FilePath.find(_T("\\")) != tstring::npos)
	{
		m_Name = m_FilePath.substr(m_FilePath.rfind(_T('\\'), m_FilePath.size()-1)+1);
	}
	else
	{
		m_Name = m_FilePath;
	}

	m_MapFlags = mapFlags;

	m_FileAccess = ((mapFlags & Read) ? GENERIC_READ:GENERIC_WRITE) | 
					   ((mapFlags & Write)?GENERIC_WRITE:GENERIC_READ);

	m_SharedAccess = ((mapFlags & Read)?FILE_SHARE_READ:FILE_SHARE_WRITE) |
						 ((mapFlags & Write)?FILE_SHARE_WRITE:FILE_SHARE_READ);

	if ((m_hFile = CreateFile(filePath,
							  m_FileAccess , 
							  m_SharedAccess,
							  NULL, 
							  OPEN_ALWAYS,
							  FILE_ATTRIBUTE_NORMAL,
							  NULL)) == INVALID_HANDLE_VALUE)
	{		  
		return false;
	}

	m_MapObjProtection = (mapFlags & Write)?PAGE_READWRITE:PAGE_READONLY;

	if (!(m_hFileMappingObject = CreateFileMapping(m_hFile,
											       NULL,
												   m_MapObjProtection,
												   0,
												   (DWORD)mapSize,
												   NULL)))
	{
		PrintLastWinAPIError(_T("CreateFileMapping"));
    	return false;
	}	

	m_MapAccess = ((mapFlags & Read)?FILE_MAP_READ:FILE_MAP_WRITE) | 
					  ((mapFlags & Write)?FILE_MAP_WRITE:FILE_MAP_READ);

	if (!(m_FileMapPtr = (BYTE *)MapViewOfFile(m_hFileMappingObject,
									 m_MapAccess,
									 0,
									 0,
									 mapSize)))
	{
		return false;
	}

	m_MapSize = (DWORD)mapSize;
	m_MapState = FileMap;


	if (m_MapSize == 0)
		GetFileMapSize();

	return true;
}

bool MemoryMap::MemoryMapSharedMem(LPCTSTR sharedMemoryName, DWORD mapFlags, size_t mapSize)
{
	CleanUpMemory(); // make sure to close any handles possibly in use.

	m_FilePath = m_Name = sharedMemoryName;

	m_MapFlags = mapFlags;

	m_MapObjProtection = (mapFlags & Write)?PAGE_READWRITE:PAGE_READONLY;

	if (!(m_hFileMappingObject = CreateFileMapping(m_hFile,
											    NULL,
												m_MapObjProtection,
												(DWORD)mapSize,
												0,
												sharedMemoryName)))
	{
		return false;
	}	


	m_MapAccess = ((mapFlags & Read)?FILE_MAP_READ:FILE_MAP_WRITE) | 
			      ((mapFlags & Write)?FILE_MAP_WRITE:FILE_MAP_READ);

	if (!(m_FileMapPtr = (BYTE *)MapViewOfFile(m_hFileMappingObject,
											 m_MapAccess,
											 0,
											 0,
											 mapSize)))
	{
		return false;
	}

	m_MapSize = (DWORD)mapSize;
	m_MapState = SharedMemoryMap;
	return true;
}

bool MemoryMap::ResizeMap(DWORD newMapSize)
{
	if (!m_MapState)
		return false;

	if (m_MapState == FileMap)
	{
		if (!MemoryMapFile(m_FilePath.c_str(), m_MapFlags, newMapSize))
			return false;
	}
	else
	{
		if (!MemoryMapSharedMem(m_Name.c_str(), m_MapFlags, newMapSize))
			return false;
	}

	return true;
}

DWORD MemoryMap::GetFileMapSize()
{
	if (m_MapSize) // was a map size specificed with a file mem map?
		return (DWORD)m_MapSize;

	// if not, get the size of the file that was completely mmaped.
	__int64 fileSize;
	GetFileSizeEx(m_hFile, (PLARGE_INTEGER)(&fileSize));
	m_MapSize = (DWORD)fileSize;
	return (DWORD)fileSize;
}

// class OpenSaveFileDialog ------------------------------------------------------------------------------------ //

OpenSaveFileDialog::OpenSaveFileDialog()
{
	// initialize states
	RtlZeroMemory(&m_ofn, sizeof(m_ofn));
	std::fill(m_FileNameBuffer, m_FileNameBuffer+0x400, 0);
	std::fill(m_Filtertstring, m_Filtertstring+0x400, 0);
	std::fill(m_InitialDirectory, m_InitialDirectory+0x400, 0);
	m_ofn.lStructSize = sizeof(m_ofn);
	memcpy(m_Filtertstring, "*.*\0*.*\0", 9);

	// start search at module directory
	_tcscpy_s(m_InitialDirectory, _countof(m_InitialDirectory), File::GetModuleDirectoryPath().c_str());
}

OpenSaveFileDialog::~OpenSaveFileDialog()
{
}

// power functions
bool OpenSaveFileDialog::OpenFile()
{
	//m_ofn.hwndOwner = NULL;
	//m_ofn.hInstance = GetModuleHandle(NULL);
	//m_ofn.lpstrFilter = m_Filtertstring;
	//m_ofn.lpstrFile   = m_FileNameBuffer;
	//m_ofn.nMaxFile = 1023;
	//m_ofn.Flags =   OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST 
	//| OFN_LONGNAMES    | OFN_EXPLORER;
	//m_ofn.lpstrInitialDir = m_InitialDirectory;

	m_ofn.hwndOwner = NULL;
	m_ofn.hInstance = GetModuleHandle(NULL);
	m_ofn.lpstrFilter = NULL;// m_Filtertstring;
	m_ofn.lpstrFile   = m_FileNameBuffer;
	m_ofn.nMaxFile = 1023;
	m_ofn.nFilterIndex = NULL;
	m_ofn.Flags =   OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST 
	| OFN_LONGNAMES    | OFN_EXPLORER;
	m_ofn.lpstrInitialDir = m_InitialDirectory;

	if (!GetOpenFileName(&m_ofn))
	{
		if (CommDlgExtendedError() == CDERR_GENERALCODES)
		{
			return false;
		}		
		return false;
	}

	return true;
}

bool OpenSaveFileDialog::SaveFile()
{
	m_ofn.hwndOwner = NULL;
	m_ofn.hInstance = GetModuleHandle(NULL);
	m_ofn.lpstrFilter = m_Filtertstring;
	m_ofn.lpstrFile   = m_FileNameBuffer;
	m_ofn.nMaxFile = 1023;
	m_ofn.Flags =   OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST 
	| OFN_LONGNAMES    | OFN_EXPLORER;
	m_ofn.lpstrInitialDir = m_InitialDirectory;

	if (!GetSaveFileName(&m_ofn))
	{
		if (CommDlgExtendedError() == CDERR_GENERALCODES)
		{
			return false;
		}		
		return false;
	}

	return true;
}

void File::GenerateFileList(const tstring& directoryPath)
{
	SmartFileHandle hFile;
	WIN32_FIND_DATA fd;
	// go through the directory and make a list of files
	tstring s = directoryPath;
	s += _T("\\*.*");

	hFile.set( FindFirstFile(s.c_str(), &fd) );
	
	while ( FindNextFile(hFile.get(), &fd) )
	{	
		fileList.push_back(directoryPath + _T("\\") + tstring(fd.cFileName) );
	}

}

void File::GenerateFileListRecusively(const tstring& directoryPath)
{
	SmartFileHandle hFile;
	WIN32_FIND_DATA fd;
	// go through the directory and make a list of files
	tstring s = directoryPath;
	s += _T("\\*.*");

	hFile.set( FindFirstFile(s.c_str(), &fd) );
	
	while ( FindNextFile(hFile.get(), &fd) )
	{	
		tstring s = (directoryPath + _T("\\") + tstring(fd.cFileName));
		
		if (s.rfind(_T(".."), s.size(), 2) != tstring::npos) // skip double dot directories, as they cause infinite loop
			continue;

		if (PathIsDirectory(s.c_str())) // if we find a directory, generate a file list from it
			GenerateFileListRecusively(s);

		fileList.push_back( s );
	}

}

void File::PruneFileList(std::vector<tstring>& fileTypes)
{
	std::vector<tstring> prunedList;

	// prune all files that don't end with a the passed in extensions
	for ( size_t i = 0; i < List().size(); ++i )
	{
		for (size_t j = 0; j < fileTypes.size(); ++j )
		{
			if ( List()[i].size() >= fileTypes[j].size() + 1) // make sure the file name is large enough for the file extension and identifer
			{
				if ( tstring(PathFindExtension(List()[i].c_str())) == fileTypes[j] )
				{
					prunedList.push_back(List()[i]);
					break;
				}
			}
		}
	}

	List().clear();
	List().insert(List().begin(), prunedList.begin(), prunedList.end());
}

tstring File::GetModuleDirectoryPath()
{
	HMODULE hModule = GetModuleHandle(0);
	tstring serverPath; 
	TCHAR buff[0x400];
	GetModuleFileName(hModule, buff, 0x400);
	serverPath = buff;
	serverPath = serverPath.substr(0, serverPath.rfind('\\', serverPath.size()-1)+1);
	return serverPath;
};