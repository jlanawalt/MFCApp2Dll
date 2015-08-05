
// MFCindll.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// Export MFC application from DLL
class __declspec( dllexport ) CMFCindllApp;
extern "C" __declspec( dllexport ) CMFCindllApp* RunMainframe();
class __declspec( dllexport ) CMFCindllDlg;


// CMFCindllApp:
// See MFCindll.cpp for the implementation of this class
//

class CMFCindllApp : public CWinApp
{
public:
	CMFCindllApp();
	bool m_bReady;

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCindllApp theApp;