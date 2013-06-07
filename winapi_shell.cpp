#include "winapi_shell.h"

Shell::Shell()
{
	CoInitialize(NULL);
}

Shell::~Shell()
{
	CoUninitialize();
}

BOOL Shell::GetFolderSelection(HWND hWnd, LPCTSTR szTitle, BFFCALLBACK browseCallbackProc, LPCTSTR szInitialDirectory)
{
	LPITEMIDLIST pidl     = NULL;
	BROWSEINFO   bi       = { 0 };
	BOOL         bResult  = FALSE;

	bi.hwndOwner      = hWnd;
	bi.pszDisplayName = selectedFolder;
	bi.pidlRoot       = NULL;//SHGetFilePathPIDL(szPath);
	bi.lpszTitle      = szTitle;
	bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	bi.lpfn			  = (browseCallbackProc != NULL) ? browseCallbackProc : BrowseCallbackProc;
	bi.lParam         = (LPARAM)szInitialDirectory;

	if ((pidl = SHBrowseForFolder(&bi)) != NULL)
	{
		bResult = SHGetPathFromIDList(pidl, selectedFolder);
		CoTaskMemFree(pidl);
	}

	return bResult;
}

int CALLBACK Shell::BrowseCallbackProc(HWND hwnd,UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    // If the BFFM_INITIALIZED message is received
    // set the path to the start path.
    switch (uMsg)
    {
        case BFFM_INITIALIZED:
        {
            if (NULL != lpData)
            {
                SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
            }
        }
    }
 
    return 0; // The function should always return 0.
}

//
//STDAPI SHGetUIObjectFromFullPIDL(LPCITEMIDLIST pidl, HWND hwnd, REFIID riid, void **ppv)
//{
//    LPCITEMIDLIST pidlChild;
//    IShellFolder* psf;
//
//    *ppv = NULL;
//
//    HRESULT hr = SHBindToParent(pidl, IID_PPV_ARG(IShellFolder, &psf), &pidlChild);
//    if (SUCCEEDED(hr))
//    {
//        hr = psf->GetUIObjectOf(hwnd, 1, &pidlChild, riid, NULL, ppv);
//        psf->Release();
//    }
//    return hr;
//}
//
//HRESULT SHILClone(LPCITEMIDLIST pidl, LPITEMIDLIST *ppidl)
//{
//    DWORD cbTotal = 0;
//
//    if (pidl)
//    {
//        LPCITEMIDLIST pidl_temp = pidl;
//        cbTotal += sizeof (pidl_temp->mkid.cb);
//
//        while (pidl_temp->mkid.cb) 
//        {
//            cbTotal += pidl_temp->mkid.cb;
//            pidl_temp += ILNext (pidl_temp);
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
//STDAPI SHGetTargetFolderIDList(LPCITEMIDLIST pidlFolder, LPITEMIDLIST *ppidl)
//{
//    IShellLink *psl;
//	
//    *ppidl = NULL;
//    
//    HRESULT hr = SHGetUIObjectFromFullPIDL(pidlFolder, NULL, IID_PPV_ARG(IShellLink, &psl));
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
//
//STDAPI SHGetTargetFolderPath(LPCITEMIDLIST pidlFolder, LPWSTR pszPath, UINT cchPath)
//{
//    LPITEMIDLIST pidlTarget;
//	
//    *pszPath = 0;
//
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
