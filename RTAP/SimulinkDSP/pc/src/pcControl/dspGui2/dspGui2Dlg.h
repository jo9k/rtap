// dspGui2Dlg.h : Headerdatei
//

#pragma once
#include "afxwin.h"

#include <string>
#include <vector>
#include "afxcmn.h"

#include "serialConnectionUseRef.h"
#include "serialConnection.h"

// CdspGui2Dlg Dialogfeld
class CdspGui2Dlg : public CDialog, public serialConnectionUseRef
{
// Konstruktion
public:
	CdspGui2Dlg(CWnd* pParent = NULL);	// Standardkonstruktor
	void printMessageControlBox(const std::string& txt);
	void printMessage(const std::string& txt, int opcodeOS, int opcodeUser, const std::vector<int>& lstParameters);
	bool transformBinaryToInt(const char* newField, unsigned amBytes, std::vector<int>& listParameters);
	bool initSend(char msgPurpose, char opcodeMsg, std::vector<int> lstParameters);


	void messageComplete(unsigned portID, const char* field, unsigned amBytes, long sendID);
	void messageTimeout(unsigned portID, const char* field, unsigned amBytes, long sendID);
	void messageReceived(unsigned portID, long receiveID, const char* newField, unsigned amBytes);
	void messageDiscarded(unsigned portID, const char* field, unsigned amBytes, long sendID);
	void error(unsigned portID, errorEnum reason);
	void awakeTimer(timerWin32* timerPointer);
	void indicateRing(unsigned portID);
	void indicateRLSDChanged(unsigned portID);
	void indicateCTSChanged(unsigned portID);
	void indicateEvent(unsigned portID, DWORD evMask);

// Dialogfelddaten
	enum { IDD = IDD_DSPGUI2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
private:
	int selectOpcode_field1;
	int selectOpcode_field2;
	int selectOpcode_field3;
	int opcodeOSGlobal;
	int idSelectedComPort;
	CRITICAL_SECTION sectionAccessGUI;
	CRITICAL_SECTION sectionChannel;
	serialConnection connectionRS232;
	bool ctsSet;
	typedef enum 
	{
		PORT_CLOSED,
		PORT_OPENED,
		PORT_ACTIVE,
		PORT_DEVICE,
		PORT_DEVICE_DSP
	} CONNECTIONSTATE;
	CONNECTIONSTATE connectionState;

// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck1();
	int sliderVolumeParam;
	afx_msg void OnNMReleasedcaptureSliderVolume(NMHDR *pNMHDR, LRESULT *pResult);
	CListBox outputBox;
	afx_msg void OnCbnSelchangeCombo4();
	CComboBox comboBoxRS232;
	CButton checkBoxDSP;
	afx_msg void OnBnClickedButtonTestDSP();
	afx_msg void OnCbnEditchangeComboOpcode1();
	CSliderCtrl sliderVolume;
	CEdit textConnectionState;
	CEdit volumeDisplay;
	afx_msg void OnBnClickedButton1();
	CEdit loadDisplay;
};
