// dspGui2.h : Hauptheaderdatei für die dspGui2-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error 'stdafx.h' muss vor dieser Datei in PCH eingeschlossen werden.
#endif

#include "resource.h"		// Hauptsymbole


// CdspGui2App:
// Siehe dspGui2.cpp für die Implementierung dieser Klasse
//

class CdspGui2App : public CWinApp
{
public:
	CdspGui2App();

// Überschreibungen
	public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CdspGui2App theApp;
