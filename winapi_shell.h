#ifndef WINAPI_SHELL_H
#define WINAPI_SHELL_H

#include <Windows.h>
#include <Shlobj.h>

class Shell
{
public:

	Shell();
	~Shell();

	// use a shell dialog to browse for a folder
    BOOL GetFolderSelection(HWND hWnd, LPCTSTR szTitle, BFFCALLBACK browseCallbackProc, LPCTSTR szInitialDirectory);

	char* const SelectedFolder() const { return (char * const)selectedFolder; }
	
private:
	static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg, LPARAM lParam, LPARAM lpData);
	TCHAR selectedFolder[MAX_PATH];
};



// shell programming for resolving short cuts/networks/disk drivers/etc..

// Macros for interface casts 
//#ifdef __cplusplus
//#define IID_PPV_ARG(IType, ppType) IID_##IType, reinterpret_cast(static_cast(ppType))
//#else
//#define IID_PPV_ARG(IType, ppType) &IID_##IType, (void**)(ppType)
//#endif


//static HRESULT SHGetUIObjectFromFullPIDL(LPCITEMIDLIST pidl, HWND hwnd, REFIID riid, void **ppv)
//{
//    LPCITEMIDLIST pidlChild;
//    IShellFolder* psf;
//    *ppv = NULL;
//    HRESULT hr = SHBindToParent(pidl, IID_IShellFolder, (LPVOID*)&psf, &pidlChild);
//    if (SUCCEEDED(hr))
//    {
//        hr = psf->GetUIObjectOf(hwnd, 1, &pidlChild, riid, NULL, ppv);
//        psf->Release();
//    }
//    return hr;
//}
//static HRESULT SHILClone(LPCITEMIDLIST pidl, LPITEMIDLIST *ppidl)
//{
//    DWORD cbTotal = 0;
//    if (pidl)
//    {
//        LPCITEMIDLIST pidl_temp = pidl;
//        cbTotal += pidl_temp->mkid.cb;
//        while (pidl_temp->mkid.cb) 
//        {
//            cbTotal += pidl_temp->mkid.cb;
//            pidl_temp = ILNext(pidl_temp);
//        }
//    }
//
//    *ppidl = (LPITEMIDLIST)CoTaskMemAlloc(cbTotal);
//
//    if (*ppidl)
//        CopyMemory(*ppidl, pidl, cbTotal);
//
//    return  *ppidl ? S_OK: E_OUTOFMEMORY;
//}
//
//// Get the target PIDL for a folder PIDL. This also deals with cases of a folder  
//// shortcut or an alias to a real folder.
//static HRESULT SHGetTargetFolderIDList(LPCITEMIDLIST pidlFolder, LPITEMIDLIST *ppidl)
//{
//    IShellLink *psl;
//
//    *ppidl = NULL;
//
//    HRESULT hr = SHGetUIObjectFromFullPIDL(pidlFolder, NULL, IID_IShellLink, (LPVOID*)&psl);
//
//    if (SUCCEEDED(hr))
//    {
//        hr = psl->GetIDList(ppidl);
//        psl->Release();
//    }
//
//    // It's not a folder shortcut so get the PIDL normally.
//    if (FAILED(hr))
//        hr = SHILClone(pidlFolder, ppidl);
//
//    return hr;
//}
//// Get the target folder for a folder PIDL. This deals with cases where a folder
//// is an alias to a real folder, folder shortcuts, the My Documents folder, etc.
//STDAPI SHGetTargetFolderPath(LPCITEMIDLIST pidlFolder, LPWSTR pszPath, UINT cchPath)
//{
//    LPITEMIDLIST pidlTarget;
//
//    *pszPath = 0;
//    HRESULT hr = SHGetTargetFolderIDList(pidlFolder, &pidlTarget);
//
//    if (SUCCEEDED(hr))
//    {
//        SHGetPathFromIDListW(pidlTarget, pszPath);   // Make sure it is a path
//        CoTaskMemFree(pidlTarget);
//    }
//
//    return *pszPath ? S_OK : E_FAIL;
//}

// my misc shell code
//static LPITEMIDLIST SHGetFilePathPIDL(char* szPath)
//{
//	LPITEMIDLIST  pidl;
//	LPSHELLFOLDER pDesktopFolder;
//	
//	OLECHAR       olePath[MAX_PATH];
//	ULONG         chEaten;
//	ULONG         dwAttributes;
//	HRESULT       hr;
//
//	// 
//	// Get a pointer to the Desktop's IShellFolder interface.
//	// 
//	if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
//	{
//		// 
//		// IShellFolder::ParseDisplayName requires the file name be in
//		// Unicode.
//		// 
//		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1,
//							olePath, MAX_PATH);
//
//		// 
//		// Convert the path to an ITEMIDLIST.
//		// 
//		hr = pDesktopFolder->ParseDisplayName(NULL,NULL,olePath,&chEaten,&pidl,&dwAttributes);
//		if (FAILED(hr))
//		{
//			// Handle error.
//		}
//
//		// 
//		// pidl now contains a pointer to an ITEMIDLIST for .\readme.txt.
//		// This ITEMIDLIST needs to be freed using the IMalloc allocator
//		// returned from SHGetMalloc().
//		// 
//
//		//release the desktop folder object
//			pDesktopFolder->Release();
//	}
//
//	return pidl;
//}

#endif 