#ifndef WINAPI_DATA_CONVERSATION_H
#define WINAPI_DATA_CONVERSATION_H

#ifndef _INC_TCHAR
# include <tchar.h>
#endif
#ifndef _INC_CRTDBG
# include <crtdbg.h>
#endif
#ifndef _WINDOWS_
# include <windows.h>
#endif

/*#############################################################################
# TCONVERT.H
#
# SCA Software International S.A.
# http://www.scasoftware.com
# scaadmin@scasoftware.com
#
# Copyright (c) 2000 SCA Software International S.A.
#
# Date: 01.05.2000
# Author: Zoran M.Todorovic
#
# This software is provided "AS IS", without a warranty of any kind.
# You are free to use/modify this code but leave this header intact.
#
#############################################################################*/

//=============================================================================
// class _totchar
// This class converts a TCHAR string to a new TCHAR string.
// Memory is allocated/deallocated using new/delete
//=============================================================================

class _totchar {
private:
  BOOL m_bAutoDelete;
  LPTSTR m_tszBuffer;

public:
  _totchar(LPCSTR szText, BOOL bAutoDelete = TRUE)
  {
    m_bAutoDelete = bAutoDelete;
    _ASSERTE(szText);
    int nLen = strlen(szText) + 1;
    m_tszBuffer = new TCHAR [nLen];
    #if defined(UNICODE) || defined(_UNICODE)
    mbstowcs(m_tszBuffer, szText, nLen);
    #else
    strcpy(m_tszBuffer, szText);
    #endif
  }
  _totchar(LPCWSTR wszText, BOOL bAutoDelete = TRUE)
  {
    m_bAutoDelete = bAutoDelete;
    _ASSERTE(wszText);
    int nLen = wcslen(wszText) + 1;
    m_tszBuffer = new TCHAR [nLen];
    #if defined(UNICODE) || defined(_UNICODE)
    wcscpy(m_tszBuffer, wszText);
    #else
    wcstombs(m_tszBuffer, wszText, nLen);
    #endif
  }
  ~_totchar()
  {
    if (m_bAutoDelete) {
      _ASSERTE(m_tszBuffer);
      delete [] m_tszBuffer;
    }
  }
  operator LPTSTR()
  {
    _ASSERTE(m_tszBuffer);
    return (LPTSTR) m_tszBuffer;
  }
  operator LPCTSTR()
  {
    _ASSERTE(m_tszBuffer);
    return (LPCTSTR) m_tszBuffer;
  }
};

#endif