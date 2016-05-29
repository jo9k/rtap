// dspGui2Dlg.cpp : Implementierungsdatei
//

#include <string>

#include "stdafx.h"
#include "dspGui2.h"
#include "dspGui2Dlg.h"
#include ".\dspgui2dlg.h"

#include <string>
#define NUMBER_DISPLAYED_OPCODES 4

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define OPCODE_OS_INITIAL 2
#define OPCODE_OS_LIMIT 4
#define OPCODE_VOLUME 1

#define SYNCWORD 0x19061975
#define DATARATE 115200

enum syncWord
{
	SYNCWORD_DEFAULT1 = 0x19,
	SYNCWORD_DEFAULT2 = 0x06,
	SYNCWORD_DEFAULT3 = 0x19,
	SYNCWORD_DEFAULT4 = 0x75
};

enum purposeMessage
{
	POLLING = 0,
	INITIAL_HANDSHAKE = 1,
	TERMINATE_HANDSHAKE = 2,
	INITIAL_SYNCHRONIZATION = 3,
	TERMINATE_SYNCHRONIZATION = 4,
	SWITCH_MODE_STREAMING,
	SWITCH_MODE_DEFAULT,
	EXCHANGE_GENERIC_CONFIGURATION_REQUEST,
	EXCHANGE_GENERIC_CONFIGURATION_RESPONSE,
	USER_MODE_MESSAGE,
	NEXT_PART_MESSAGE,
	GENERIC_MESSAGE,
	NOT_SUPPORTED,
	PURPOSE_INVALID = 0xFF
};

std::string
int2String(int val)
{
	char txt[128];
	sprintf(txt, "%d", val);
	return(txt);
}

std::string
int2HexString(int val)
{
	char txt[128];
	sprintf(txt, "0x%x", val);
	return(txt);
}

// CAboutDlg-Dialogfeld für Anwendungsbefehl 'Info'

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CdspGui2Dlg Dialogfeld



CdspGui2Dlg::CdspGui2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CdspGui2Dlg::IDD, pParent)
	, sliderVolumeParam(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdspGui2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER3, sliderVolumeParam);
	DDX_Control(pDX, IDC_LIST1, outputBox);
	DDX_Control(pDX, IDC_COMBO4, comboBoxRS232);
	DDX_Control(pDX, IDC_CHECK1, checkBoxDSP);
	DDX_Control(pDX, IDC_SLIDER3, sliderVolume);
	DDX_Control(pDX, IDC_EDIT4, textConnectionState);
	DDX_Control(pDX, IDC_EDIT5, volumeDisplay);
}

BEGIN_MESSAGE_MAP(CdspGui2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER3, OnNMReleasedcaptureSliderVolume)
	ON_CBN_SELCHANGE(IDC_COMBO4, OnCbnSelchangeCombo4)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButtonTestDSP)
	ON_CBN_EDITCHANGE(IDC_COMBO1, OnCbnEditchangeComboOpcode1)
END_MESSAGE_MAP()


// CdspGui2Dlg Meldungshandler

BOOL CdspGui2Dlg::OnInitDialog()
{
	unsigned i;
	CDialog::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	// TODO: Hier zusätzliche Initialisierung einfügen
	
	std::string addItem;
	addItem = "NONE";
//	this->ComboxBoxOpcode1.AddString(addItem.c_str());
//	this->ComboxBoxOpcode2.AddString(addItem.c_str());
//	this->ComboxBoxOpcode3.AddString(addItem.c_str());
//	for(i = 0; i < NUMBER_DISPLAYED_OPCODES; i++)
//	{
//		addItem = "Opcode" + int2String(i);
//		this->ComboxBoxOpcode1.AddString(addItem.c_str());
//		this->ComboxBoxOpcode2.AddString(addItem.c_str());
//		this->ComboxBoxOpcode3.AddString(addItem.c_str());
//	}
//	this->ComboxBoxOpcode1.SetCurSel(0);
//	this->ComboxBoxOpcode2.SetCurSel(0);
//	this->ComboxBoxOpcode3.SetCurSel(0);

	selectOpcode_field1 = -1;
	selectOpcode_field2 = -1;
	selectOpcode_field3 = -1;

	sliderVolume.SetRange(0, 999);

	std::string txt = int2String(0);
//	fieldOpcode1.SetWindowText(txt.c_str());
//	fieldOpcode2.SetWindowText(txt.c_str());
//	fieldOpcode3.SetWindowText(txt.c_str());

	this->connectionRS232.initialize(100, this);
	int numberPorts = connectionRS232.getAmountAvailableComPorts();
	idSelectedComPort = -1;
	comboBoxRS232.AddString("NONE");
	for(i = 0; i < numberPorts; i++)
	{
		comboBoxRS232.AddString(connectionRS232.getShortcutComPort(i));
	}
	comboBoxRS232.SetCurSel(0);
	connectionState = PORT_CLOSED;
	textConnectionState.SetWindowText("NONE");

/*
	for(i = 0; i< OPCODE_OS_LIMIT; i++)
	{
		addItem = "Opcode" + int2String(i);
		comboBoxOpcodeOS.AddString(addItem.c_str());
	}
	opcodeOSGlobal = OPCODE_OS_INITIAL;
	comboBoxOpcodeOS.SetCurSel(opcodeOSGlobal);
*/
	this->volumeDisplay.SetWindowText(int2String(0).c_str());

	ctsSet = false;

	InitializeCriticalSection(&sectionAccessGUI);
	InitializeCriticalSection(&sectionChannel);
	printMessageControlBox("Starting host for DSP COntrol, IND!");

	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

void CdspGui2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CdspGui2Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CdspGui2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CdspGui2Dlg::OnBnClickedCheck1()
{
	if(ctsSet)
	{
		this->checkBoxDSP.SetCheck(1);
	}
	else
	{
		this->checkBoxDSP.SetCheck(0);
	}

	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
}

void CdspGui2Dlg::OnNMReleasedcaptureSliderVolume(NMHDR *pNMHDR, LRESULT *pResult)
{
	int value = sliderVolume.GetPos();
	int opcodeUser = OPCODE_VOLUME;
	float level;
	std::vector<int> lstParameters;

	this->volumeDisplay.SetWindowText(int2String(value).c_str());
	if((opcodeUser >= 0)&&ctsSet)
	{
		level = (float)value/1000.0;
		value = *((int*)&level);
		lstParameters.push_back(value);
		this->printMessage("Sending Message", GENERIC_MESSAGE, 1, lstParameters);
		this->initSend(GENERIC_MESSAGE, 1, lstParameters);
	}
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
	*pResult = 0;
}

void CdspGui2Dlg::OnCbnSelchangeCombo4()
{
	if(idSelectedComPort >= 0)
	{
		if(!connectionRS232.deactivateComPort(idSelectedComPort))
		{
			printMessageControlBox((std::string)"Failed to deactivate port " + connectionRS232.getShortcutComPort(idSelectedComPort));
		}
		if(!connectionRS232.closePort(idSelectedComPort))
		{
			printMessageControlBox((std::string)"Failed to close port " + connectionRS232.getShortcutComPort(idSelectedComPort));
		}
		Sleep(500);
	}
	connectionState = PORT_CLOSED;
	textConnectionState.SetWindowText("NONE");
	ctsSet = false;
	this->checkBoxDSP.SetCheck(0);
	this->idSelectedComPort = comboBoxRS232.GetCurSel()-1;
	if(idSelectedComPort >= 0)
	{
		if(!connectionRS232.openPort(idSelectedComPort))
		{
			printMessageControlBox((std::string)"Failed to open port " + connectionRS232.getShortcutComPort(idSelectedComPort));
			idSelectedComPort = -1;
			comboBoxRS232.SetCurSel(0);
		}
		else
		{
			connectionState = PORT_OPENED;
			textConnectionState.SetWindowText("RS232 ON");
			if(!connectionRS232.activateComPort(idSelectedComPort, DATARATE, 8, TWOSTOPBITS,
			ODDPARITY, serialElementDescriptor::NO_FLOWCONTROL))
			{
				printMessageControlBox((std::string)"Failed to activate port " + connectionRS232.getShortcutComPort(idSelectedComPort));
				connectionRS232.closePort(idSelectedComPort);
				idSelectedComPort = -1;
				comboBoxRS232.SetCurSel(0);
			}
			else
			{
				connectionState = PORT_ACTIVE;
				textConnectionState.SetWindowText("Port Active");

				ctsSet = connectionRS232.verifyCTSSet(idSelectedComPort);
				if(ctsSet)
				{
					this->checkBoxDSP.SetCheck(1);
					connectionState = PORT_DEVICE;
					textConnectionState.SetWindowText("Device detected");
					char field[12];
					short stmp;

					field[0] = SYNCWORD_DEFAULT1;
					field[1] = SYNCWORD_DEFAULT2;
					field[2] = SYNCWORD_DEFAULT3;
					field[3] = SYNCWORD_DEFAULT4;
					field[4] = INITIAL_HANDSHAKE; // Initial handshake
					field[5] = 0; // Opcode
					stmp = 0; // NUMBER_ELEMENTS_FOLLOW
					memcpy(&field[6], &stmp, sizeof(short));
					Sleep(200);
					connectionRS232.initSend(idSelectedComPort, field, 8, 0, 500);
				}
			}
		}
	}
	
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
}

void CdspGui2Dlg::OnBnClickedButtonTestDSP()
{
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
}

void CdspGui2Dlg::OnCbnEditchangeComboOpcode1()
{
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
}

void 
CdspGui2Dlg::printMessageControlBox(const std::string& txt)
{
	EnterCriticalSection(&sectionAccessGUI);
	outputBox.AddString(txt.c_str());
	LeaveCriticalSection(&sectionAccessGUI);
}

void 
CdspGui2Dlg::printMessage(const std::string& purposeCallback, int opcodeOS, int opcodeUser, const std::vector<int>& lstParameters)
{
	std::string txtOut = purposeCallback + ": OS: " + int2String(opcodeOS) + ", USER: " + int2String(opcodeUser);
	printMessageControlBox(txtOut);
	for(unsigned i = 0; i < lstParameters.size();i++)
	{
		txtOut = "--- Param#" + int2String(i) + " = " + int2HexString(lstParameters[i]);
		printMessageControlBox(txtOut);
	}
}

void 
CdspGui2Dlg::messageComplete(unsigned portID, const char* newField, unsigned amBytes, long sendID)
{
	char msgPurpose = 0;
	char opcodeMsg = 0;
	short numValues;
	std::vector<int> listParameters;

	if(amBytes < 8)
	{
		printMessageControlBox("COMM ERROR OUT: Too few bytes received for a valid message");
		return;
	}
	if(!((newField[0] == SYNCWORD_DEFAULT1)&&(newField[1] == SYNCWORD_DEFAULT2)&&
		(newField[2] == SYNCWORD_DEFAULT3)&&(newField[3] == SYNCWORD_DEFAULT4)))
	{
		printMessageControlBox("COMM ERROR OUT: Header syncword is incorrect");
		return;
	}
	
	msgPurpose = newField[4];
	opcodeMsg = newField[5];
	memcpy(&numValues, &newField[6], sizeof(short));

	transformBinaryToInt(&newField[8], amBytes-8, listParameters);
	printMessage("Message Complete", msgPurpose, opcodeMsg, listParameters);
}

bool
CdspGui2Dlg::transformBinaryToInt(const char* newField, unsigned amBytes, std::vector<int>& listParameters)
{
	unsigned numMin;
	int val = 0;
	listParameters.clear();

	while(amBytes > 0)
	{
		numMin = min(amBytes, 4);
		memcpy(&val, newField, sizeof(char)*numMin);
		listParameters.push_back(val);
		newField+=numMin;
		amBytes -= numMin;
	}
	return(true);
}
	
void 
CdspGui2Dlg::messageTimeout(unsigned portID, const char* field, unsigned amBytes, long sendID)
{
	printMessageControlBox("MESSAGE TIMEOUT!");
}
	
void 
CdspGui2Dlg::messageReceived(unsigned portID, long receiveID, const char* newField, unsigned amBytes)
{
	unsigned i, ii;

	char msgPurpose;
	char opcodeMsg;
	short numValues;

	std::vector<int> listParameters;
	std::string txt;
	char field[2] = {0};
	char* fPtr = NULL;
	char* fldTxt = NULL;

	if(amBytes < 8)
	{
		printMessageControlBox("COMM ERROR: Too few bytes received for a valid message");
		return;
	}
	if(!((newField[0] == SYNCWORD_DEFAULT1)&&(newField[1] == SYNCWORD_DEFAULT2)&&
		(newField[2] == SYNCWORD_DEFAULT3)&&(newField[3] == SYNCWORD_DEFAULT4)))
	{
		printMessageControlBox("COMM ERROR: Header syncword is incorrect");
		return;
	}
	
	msgPurpose = newField[4];
	opcodeMsg = newField[5];
	memcpy(&numValues, &newField[6], sizeof(short));
	
	switch(msgPurpose)
	{
	case TERMINATE_HANDSHAKE:
		fldTxt = new char[amBytes-8+1];
		memset(fldTxt, 0, sizeof(char)*(amBytes-8+1));
		memcpy(fldTxt, &newField[8], sizeof(char)*(amBytes-8));
		txt = fldTxt;
		textConnectionState.SetWindowText(txt.c_str());
		break;
	case GENERIC_MESSAGE:
		switch(opcodeMsg)
		{
			// Place your code right here!
		default:
			printMessageControlBox("GENERIC COMM ERROR: Received generic message with unknown opcode");
		}
	}
	
	transformBinaryToInt(&newField[8], amBytes-8, listParameters);
	printMessage("Message Received", msgPurpose, opcodeMsg, listParameters);
}
	
void 
CdspGui2Dlg::messageDiscarded(unsigned portID, const char* field, unsigned amBytes, long sendID)
{
	printMessageControlBox("MESSAGE DISCARDED!");
}
	
void 
CdspGui2Dlg::error(unsigned portID, errorEnum reason)
{
	printMessageControlBox("MESSAGE ERROR!");
}
	
void 
CdspGui2Dlg::awakeTimer(timerWin32* timerPointer)
{
	printMessageControlBox("MESSAGE TIMER!");
}
	
void 
CdspGui2Dlg::indicateRing(unsigned portID)
{
	printMessageControlBox("MESSAGE RING!");
}
	
void 
CdspGui2Dlg::indicateRLSDChanged(unsigned portID)
{
	printMessageControlBox("MESSAGE RLSD!");
}
	
void 
CdspGui2Dlg::indicateCTSChanged(unsigned portID)
{
	printMessageControlBox("MESSAGE CTS!");
}
	
void 
CdspGui2Dlg::indicateEvent(unsigned portID, DWORD evMask)
{
	if(evMask & EV_CTS)
	{
		if(connectionState == PORT_ACTIVE)
		{
			ctsSet = connectionRS232.verifyCTSSet(idSelectedComPort);
			if(ctsSet)
			{
				this->checkBoxDSP.SetCheck(1);
				connectionState = PORT_DEVICE;
				textConnectionState.SetWindowText("Device detected");
				char field[12];
				int value = SYNCWORD;
				memcpy(field, &value, 4);
				value = 1;
				memcpy(&field[4], &value, 4);
				value = 3;
				memcpy(&field[8], &value, 4);
				Sleep(500);
				connectionRS232.initSend(idSelectedComPort, field, 12, 0, 500);
			}
		}
		if((connectionState == PORT_DEVICE)||(connectionState == PORT_DEVICE_DSP))
		{
			ctsSet = connectionRS232.verifyCTSSet(idSelectedComPort);
			if(!ctsSet)
			{
				this->checkBoxDSP.SetCheck(0);
				connectionState = PORT_ACTIVE;
				textConnectionState.SetWindowText("Port Active");
			}
		}
	}
	//	printMessageControlBox("MESSAGE EVENT!");
}

bool
CdspGui2Dlg::initSend(char purposeMsg, char opcodeUser, std::vector<int> lstParameters)
{
	int value;
	short stmp;
	int idx;
	int numBytes;
	if(idSelectedComPort >= 0)
	{
		EnterCriticalSection(&sectionChannel);
		numBytes = 8+lstParameters.size()*sizeof(int);
		char * field = new char[numBytes];
	    field[0] = SYNCWORD_DEFAULT1;
		field[1] = SYNCWORD_DEFAULT2;
		field[2] = SYNCWORD_DEFAULT3;
		field[3] = SYNCWORD_DEFAULT4;
		field[4] = purposeMsg; 
		field[5] = opcodeUser; // Opcode
		stmp = lstParameters.size(); // NUMBER_ELEMENTS_FOLLOW
		memcpy(&field[6], &stmp, sizeof(short));

		for(unsigned i = 0; i < lstParameters.size(); i++)
		{
			idx = 8+sizeof(char)*i;
			value = lstParameters[i];
			memcpy(&field[idx], &value, sizeof(int));
		}
		this->connectionRS232.initSend(idSelectedComPort, field, 8+lstParameters.size()*sizeof(int), 0, 500);
		delete[](field);
		LeaveCriticalSection(&sectionChannel);
		return(true);
	}
	return(false);
}

