
// DataAnalysisDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "DataAnalysis.h"
#include "DataAnalysisDlg.h"
#include "afxdialogex.h"
#include "SettingDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// variables for CommThread
HWND hCommWnd;
CString strPortname;

CDataAnalysisDlg::CDataAnalysisDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DataAnalysisDlg, pParent)
	, m_iSerialPort(-1)
	, m_strSerial(_T(""))
	, m_iBaudRate(7)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERIAL_PORT, m_cSerialPort);
	DDX_Control(pDX, IDC_BAUD_RATE, m_cBaudRate);
	DDX_Control(pDX, IDC_Message, m_LogList);
}

BEGIN_MESSAGE_MAP(CDataAnalysisDlg, CDialogEx)
	ON_WM_PAINT()
	ON_MESSAGE(WM_COMM_READ1, OnCommunication) //추가
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_ReqSettingDlg, &CDataAnalysisDlg::OnBnClickedReqsettingdlg)
    ON_BN_CLICKED(IDC_TestButton, &CDataAnalysisDlg::OnBnClickedTestbutton)
	ON_BN_CLICKED(IDC_BTN_PORT_OPEN, &CDataAnalysisDlg::OnBnClickedBtnPortOpen)
	ON_BN_CLICKED(IDC_BTN_PORT_CLOSE, &CDataAnalysisDlg::OnBnClickedBtnPortClose)
	ON_CBN_SELCHANGE(IDC_SERIAL_PORT, &CDataAnalysisDlg::OnCbnSelchangeSerialPort)
	ON_CBN_SELCHANGE(IDC_BAUD_RATE, &CDataAnalysisDlg::OnCbnSelchangeBaudRate)
END_MESSAGE_MAP()


// CDataAnalysisDlg 메시지 처리기
BOOL CDataAnalysisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	hCommWnd = m_hWnd;


	// TODO: 여기에 추가 초기화 작업을 추가합니다.
    GetDataFormat_fromReg();
	InitSerialPort();


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDataAnalysisDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDataAnalysisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDataAnalysisDlg::OnBnClickedReqsettingdlg() 
{
    CSettingDlg dlg;
    dlg.pMain = this;
    dlg.DoModal();
}

void CDataAnalysisDlg::SetReg_RegistryData(const CString& strAddress, const CString& strItem, const CString& stData) 
{
    HKEY hkey = NULL;
    DWORD dwDisp;

    CString strRegDirectory;
    strRegDirectory.Format("%sModel\\%s\\%s", DEFAULT_REGISTRY_SETTING_PATH, m_strModel, strAddress);

    const unsigned char *str;
    unsigned char buff[1024];
    memcpy(buff, (unsigned char *)(LPCTSTR)stData, strlen(stData));
    str = buff;

    LONG result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);
    while (result != ERROR_SUCCESS) 
    {
        // Key가 없다면
        ::RegCreateKeyEx(HKEY_CURRENT_USER, strRegDirectory, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dwDisp);
        result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);
    }

    ::RegSetValueEx(hkey, strItem, 0, REG_SZ, str, (DWORD)strlen(stData));
    ::RegCloseKey(hkey);
    ::RegFlushKey(hkey);
}


CString CDataAnalysisDlg::GetReg_RegistryData(const CString& strAddress, const CString& strItem) 
{
    HKEY hkey = NULL;
    DWORD dwDisp;

    CString strRegDirectory;
    strRegDirectory.Format("%sModel\\%s\\%s", DEFAULT_REGISTRY_SETTING_PATH, m_strModel, strAddress);

    LONG result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);

    if (result != ERROR_SUCCESS) 
    {
        // Key가 없다면
        ::RegCreateKeyEx(HKEY_CURRENT_USER, strRegDirectory, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dwDisp);
        result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);
    }

    CString str;
    DWORD dwType = REG_SZ;
    DWORD dwCount = _MAX_PATH;

    ::RegQueryValueEx(hkey, strItem, NULL, &dwType, (LPBYTE)str.GetBuffer(_MAX_PATH), &dwCount);
    ::RegCloseKey(hkey);
    ::RegFlushKey(hkey);
    return str;
}


void CDataAnalysisDlg::SetRegRoot_RegistryData(const CString& strAddress, const CString& strItem, const CString& stData) 
{
    HKEY hkey = NULL;
    DWORD dwDisp;

    CString strRegDirectory;
    strRegDirectory.Format("%scommon\\%s", DEFAULT_REGISTRY_SETTING_PATH, strAddress);

    const unsigned char *str;
    unsigned char buff[1024];
    memcpy(buff, (unsigned char *)(LPCTSTR)stData, strlen(stData));
    str = buff;

    LONG result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);
    while (result != ERROR_SUCCESS) 
    {
        // Key가 없다면
        ::RegCreateKeyEx(HKEY_CURRENT_USER, strRegDirectory, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dwDisp);
        result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);
    }

    ::RegSetValueEx(hkey, strItem, 0, REG_SZ, str, (DWORD)strlen(stData));
    ::RegCloseKey(hkey);
    ::RegFlushKey(hkey);
}


CString CDataAnalysisDlg::GetRegRoot_RegistryData(const CString& strAddress, const CString& strItem) 
{
    HKEY hkey = NULL;
    DWORD dwDisp;

    CString strRegDirectory;
    strRegDirectory.Format("%scommon\\%s", DEFAULT_REGISTRY_SETTING_PATH, strAddress);

    LONG result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);

    if (result != ERROR_SUCCESS) 
    {
        // Key가 없다면
        ::RegCreateKeyEx(HKEY_CURRENT_USER, strRegDirectory, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dwDisp);
        result = ::RegOpenKey(HKEY_CURRENT_USER, strRegDirectory, &hkey);
    }

    CString str;
    DWORD dwType = REG_SZ;
    DWORD dwCount = _MAX_PATH;

    ::RegQueryValueEx(hkey, strItem, NULL, &dwType, (LPBYTE)str.GetBuffer(_MAX_PATH), &dwCount);
    ::RegCloseKey(hkey);
    ::RegFlushKey(hkey);
    return str;
}


void CDataAnalysisDlg::GetDataFormat_fromReg() 
{
    CString strValue, strItem, strAddress;
    int iValue;
    
    // Header
    m_Dataset.nHeader = max(0, min(255, atoi(GetReg_RegistryData("Data\\Header", "nHeader"))));

    if (m_Dataset.pHeader != nullptr)
    {
        delete[]m_Dataset.pHeader;
        m_Dataset.pHeader = nullptr;
    }

    if (m_Dataset.nHeader > 0)
    {
        m_Dataset.pHeader = new BYTE[m_Dataset.nHeader];
        for (int i = 0; i < m_Dataset.nHeader; i++)
        {
            strItem.Format("Header_%d", i);
            strValue = GetReg_RegistryData("Data\\Header\\Data", strItem);
            m_Dataset.pHeader[i] = atoi(strValue);
        }
    }


    // Use Group
    iValue = atoi(GetReg_RegistryData("Data\\Group", "UseGroup"));
    m_Dataset.bUseGroup = iValue;


    // Value    
    m_Dataset.nValue = max(N_MIN_VALUE, min(N_MAX_VALUE, atoi(GetReg_RegistryData("Data\\Header", "nValue"))));

    if (m_Dataset.pValueType != nullptr)
    {
        delete[]m_Dataset.pValueType;
        m_Dataset.pValueType = nullptr;
    }

    m_Dataset.pValueType = new BYTE[m_Dataset.nValue];

    for (int i = 0; i < m_Dataset.nValue; i++)
    {
        strItem.Format("ValueType_%d", i);
        strValue = GetReg_RegistryData("Data\\Value\\Type", strItem);
        m_Dataset.pValueType[i] = atoi(strValue);
    }
           
    // Checksum Type
    iValue = atoi(GetReg_RegistryData("Data\\Checksum", "ChecksumType"));
    m_Dataset.ChecksumType = iValue;


    // Tail
    m_Dataset.nTail = max(0, min(255, atoi(GetReg_RegistryData("Data\\Tail", "nTail"))));

    if (m_Dataset.pTail != nullptr)
    {
        delete[]m_Dataset.pTail;
        m_Dataset.pTail = nullptr;
    }

    if (m_Dataset.nTail > 0)
    {
        m_Dataset.pTail = new BYTE[m_Dataset.nTail];
        for (int i = 0; i < m_Dataset.nTail; i++)
        {
            strItem.Format("Tail_%d", i);
            strValue = GetReg_RegistryData("Data\\Tail\\Data", strItem);
            m_Dataset.pTail[i] = atoi(strValue);
        }
    }

    // Packet Value Type
    m_Dataset.PacketValueType = atoi(GetReg_RegistryData("Data", "PacketDataType"));

    // Get Existing Data config    
    m_Dataset.nData = max(atoi(GetReg_RegistryData("Data", "Default_N_Data")), N_MIN_DATA);
    m_Dataset.nMath = max(atoi(GetReg_RegistryData("Data", "Default_N_Math")), N_MIN_MATH);

    SetData(m_Dataset.nData, m_Dataset.nValue, m_Dataset.nMath, FALSE);
}


void CDataAnalysisDlg::OnBnClickedTestbutton() 
{
}


void CDataAnalysisDlg::GetReg_HeaderData() 
{
    m_Dataset.nHeader = max(0, min(255, atoi(GetReg_RegistryData("Data\\Header", "nHeader"))));
    
    if (m_Dataset.pHeader != nullptr) 
    {
        delete[]m_Dataset.pHeader;
        m_Dataset.pHeader = nullptr;
    }

    if (m_Dataset.nHeader > 0) 
    {
        m_Dataset.pHeader = new BYTE [m_Dataset.nHeader];
        CString strItem, strValue;
        for (int i = 0; i < m_Dataset.nHeader; i++)
        {
            strItem.Format("Header_%d", i);
            strValue = GetReg_RegistryData("Data\\Header\\Data", strItem);
            m_Dataset.pHeader[i] = atoi(strValue);
        }
    }
}

void CDataAnalysisDlg::SetData(int nData, int nValue, int nMath, bool bUpdate) 
{
    Data_t *pre = m_pData;
    if (nData > 0)
    {
        nData = max(N_MIN_DATA, nData);
        nValue = max(N_MIN_VALUE, nValue);
        nMath = max(N_MIN_MATH, nMath);


        m_pData = new Data_t[nData];
        for (int i = 0; i < nData; i++)
        {
            m_pData[i].pValue = new double[nValue];
            m_pData[i].pMath = new double[nMath];
        }
    }
    else {
        bUpdate = FALSE;
    }

    if (pre != nullptr)
    {
        // copy & free heap memory
        int nCopyData = min(m_index, nData);
        int nCopyValue = min(m_Dataset.nValue, nValue);
        int nCopyMath = min(m_Dataset.nMath, nMath);

        for (int i = 0; i < nCopyData; i++) 
        {
            if (bUpdate) 
            {
                for (int j = 0; j < nCopyValue; j++)
                {
                    m_pData[i].pValue[j] = pre[i].pValue[j];
                }
                for (int j = 0; j < nCopyMath; j++)
                {
                    m_pData[i].pMath[j] = pre[i].pMath[j];
                }
                m_pData[i].group = pre[i].group;
            }
            delete [] pre[i].pValue;
            delete [] pre[i].pMath;
        }

        for (int i = nCopyData; i < m_Dataset.nValue; i++) {
            if (bUpdate)
            {
                for (int j = 0; j < nCopyValue; j++)
                {
                    m_pData[i].pValue[j] = 0;
                }
                for (int j = 0; j < nCopyMath; j++)
                {
                    m_pData[i].pMath[j] = 0;
                }
                m_pData[i].group = 0;
            }
            delete[] pre[i].pValue;
            delete[] pre[i].pMath;
        }

        delete[]pre;
    }
    if (!bUpdate) 
    {
        m_index = 0;
    }
    
    m_Dataset.nValue = nValue;
    m_Dataset.nMath = nMath;
}


BOOL CDataAnalysisDlg::DestroyWindow()
{
    CString str;
    str.Format("Delete Heap memory\n %p(%d) --> ", m_pData, sizeof(m_pData));
    
    if (m_pData != nullptr)
    {
        SetData(0, 0, 0);
        m_pData = nullptr;
    }
    
    // Data Packet 
    if (m_Dataset.pHeader != nullptr) 
    {
        delete [] m_Dataset.pHeader;
        m_Dataset.pHeader = nullptr;
    }

    if (m_Dataset.pValueType != nullptr)
    {
        delete[] m_Dataset.pValueType;
        m_Dataset.pValueType = nullptr;
    }

    if (m_Dataset.pTail != nullptr)
    {
        delete[] m_Dataset.pTail;
        m_Dataset.pTail = nullptr;
    }

    str.Format("%s %p", str, m_pData);
    //AfxMessageBox(str);
    return CDialogEx::DestroyWindow();
}


int CDataAnalysisDlg::Hex2Dec(CString str)
{
	int len = str.GetLength();
	int value = 0;
	for (int i = 0; i < len; i++) 
	{
		char ch = str.GetAt(i);
		if (ch >= '0' && ch <= '9') 
		{
			value <<= 4;
			value += ch - '0';
		}
		else if (ch >= 'a' && ch <= 'f') 
		{
			value <<= 4;
			value += ch - 'a' + 10;
		}
		else if (ch >= 'A' && ch <= 'F') 
		{
			value <<= 4;
			value += ch - 'A' + 10;
		}
	}
	return value;
}

void CDataAnalysisDlg::OnBnClickedBtnPortOpen()
{
	OnBnClickedBtnPortClose();

	CString PortName;
	
	if (m_ComuPort.m_bConnected == TRUE)
	{
		OnBnClickedBtnPortClose();
		Wait(100);
	}

	m_iSerialPort = m_cSerialPort.GetCurSel();
	if (m_iSerialPort == -1) 
	{
		PortName.Format("사용 가능한 Port(device)가 없습니다.");
		AfxMessageBox(PortName);
		return;
	}
	m_cSerialPort.GetLBText(m_iSerialPort, m_strSerial);
	m_strSerial.Format("%s", RemoveSerialInfo(m_strSerial));
	if (m_ComuPort.OpenPort(byNameComPort(m_strSerial), byIndexBaud(m_iBaudRate), 8, 0, 0) == TRUE) 
	{
		strPortname = m_ComuPort.m_sPortName;
		CString strCom;
		strCom.Format("COM%d", String2Num(strPortname));
		SetLastComPort(strCom);
		m_bUpdateList = TRUE;
		m_iCurrentMode = DATA_ACQUISITION_MODE;
	}
}


void CDataAnalysisDlg::OnBnClickedBtnPortClose()
{
	if (m_ComuPort.m_bConnected == TRUE) {
		m_ComuPort.ClosePort();
		strPortname.Empty();

		m_bUpdateList = FALSE;
		m_iCurrentMode = DATA_ANALYZE_MODE;
	}
}

void CDataAnalysisDlg::Wait(DWORD dwMillisecond) 
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();
	do {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	} while (GetTickCount() - dwStart < dwMillisecond);
}

CString CDataAnalysisDlg::RemoveSerialInfo(CString str) 
{
	CString strReturn;
	strReturn.Format("%s", str);

	int iSize = strReturn.GetLength();
	if (iSize != 0) 
	{
		int iPos = strReturn.Find('(', 0);
		if (iPos != -1) strReturn.Format("%s", strReturn.Left(iPos));
	}
	return strReturn;
}

CString CDataAnalysisDlg::byNameComPort(CString str) 
{
	CString PortName;
	int iPort = String2Num(str);

	if (iPort == 0)
	{
		PortName.Format("");
	}
	else if (iPort <= 4) 
	{
		PortName.Format("COM%d", iPort);
	}
	else
	{
		PortName.Format("\\\\.\\COM%d", iPort);
	}

	return PortName;
}

int CDataAnalysisDlg::String2Num(CString str)
{
	char *buf = LPSTR(LPCTSTR(str));
	int iSize = str.GetLength();
	int iNum = 0;

	if (iSize < 20) 
	{
		for (int i = 0; i < iSize; i++) 
		{
			if (buf[i] >= '0' && buf[i] <= '9')
				iNum = iNum * 10 + buf[i] - '0';
		}
	}
	return iNum;
}

DWORD CDataAnalysisDlg::byIndexBaud(int xBaud) 
{
	DWORD dwBaud = CBR_115200;
	switch (xBaud) 
	{
	case 0:		dwBaud = CBR_4800;		break;
	case 1:		dwBaud = CBR_9600;		break;
	case 2:		dwBaud = CBR_14400;		break;
	case 3:		dwBaud = CBR_19200;		break;
	case 4:		dwBaud = CBR_38400;		break;
	case 5:		dwBaud = CBR_56000;		break;
	case 6:		dwBaud = CBR_57600;		break;
	case 7:		dwBaud = CBR_115200;	break;
	case 8:     dwBaud = 230400;	    break;
	case 9:		dwBaud = 460800;        break;
	case 10:    dwBaud = GetDlgItemInt(IDC_ManualBaud);  break;
	}
	if (xBaud < 10)
	{
		SetDlgItemInt(IDC_ManualBaud, dwBaud);
		GetDlgItem(IDC_ManualBaud)->EnableWindow(FALSE);
	}
	else {
		GetDlgItem(IDC_ManualBaud)->EnableWindow(TRUE);
	}
	return dwBaud;
}

void CDataAnalysisDlg::SetLastComPort(CString strComPort) 
{
	if (strComPort == "")return;
	CString strAddress = "Common";
	CString strItem = "LastComPort";
	SetRegRoot_RegistryData(strAddress, strItem, strComPort);
}

void CDataAnalysisDlg::OnCbnSelchangeSerialPort()
{
	m_iSerialPort = m_cSerialPort.GetCurSel();
}


void CDataAnalysisDlg::OnCbnSelchangeBaudRate()
{
	m_iBaudRate = m_cBaudRate.GetCurSel();
	byIndexBaud(m_iBaudRate);

}

void CDataAnalysisDlg::InitSerialPort()
{
	OnSetfocusSerialPort();
	m_cBaudRate.SetCurSel(m_iBaudRate);
	OnCbnSelchangeBaudRate();
}

void CDataAnalysisDlg::OnSetfocusSerialPort() 
{
	//HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM 에  시리얼포트 번호들이 등록되어 있음.
	HKEY hKey;

	//오픈할 레지스터 키에 대한 기본키 이름   
	//오픈할 레지스터 서브키 이름   
	//레지스터키에 대한 핸들   

	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

	TCHAR szData[40], szName[200], szBT[200];

	DWORD index = 0, dwSize = 200, dwSize2 = 40, dwType = REG_SZ;
	CString strCurrentAvailPort;

	m_cSerialPort.ResetContent();
	CString strLastComPort = GetLastComPort();

	memset(szData, 0x00, sizeof(szData));
	memset(szName, 0x00, sizeof(szName));
	memset(szBT, 0x00, sizeof(szBT));

	//hKey - 레지스터키 핸들  
	//index - 값을 가져올 인덱스.. 다수의 값이 있을 경우 필요   
	//szName - 항목값이 저장될 배열   
	//dwSize - 배열의 크기   
	int iLastComPortIndex = -1;
	CString str = "";

	while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL))
	{

		//szName-레지터스터 항목의 이름   
		//dwType-항목의 타입, 여기에서는 널로 끝나는 문자열   
		//szData-항목값이 저장될 배열   
		//dwSize2-배열의 크기   

		RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2);
		str.Format("%s", szName);
		str.Format("(%s)", str.Right(str.GetLength() - 8));

		strCurrentAvailPort = CString(szData);
		if (strCurrentAvailPort == strLastComPort) 
		{
			iLastComPortIndex = index;
		}

		memset(szData, 0x00, sizeof(szData));
		memset(szName, 0x00, sizeof(szName));
		dwSize = 200;
		dwSize2 = 40;
		strCurrentAvailPort += str;
		m_cSerialPort.AddString(strCurrentAvailPort);
		index++;
	}
	RegCloseKey(hKey);

	m_iConnectedPortCount = 0;
	for (int i = 0; i < m_cSerialPort.GetCount(); i++)
	{
		CString strTemp;
		m_cSerialPort.GetLBText(i, strTemp);
		m_iConnectComPort[m_iConnectedPortCount++] = String2Num(strTemp);
	}

	if (iLastComPortIndex == -1)
	{
		if (m_cSerialPort.GetCount() > 0) 
		{
			m_iSerialPort = 0;
			m_cSerialPort.SetCurSel(m_iSerialPort);
			m_cSerialPort.GetLBText(m_iSerialPort, m_strSerial);
			m_strSerial.Format("%s", RemoveSerialInfo(m_strSerial));
		}
	}
	else {
		BOOL bFind = FALSE;
		for (int i = 0; i < m_cSerialPort.GetCount(); i++) 
		{
			CString strTemp;
			m_cSerialPort.GetLBText(i, strTemp);
			if (RemoveSerialInfo(strTemp) == strLastComPort)
			{
				m_iSerialPort = i;
				m_cSerialPort.SetCurSel(i);
				m_cSerialPort.GetLBText(i, m_strSerial);
				m_strSerial.Format("%s", RemoveSerialInfo(m_strSerial));
				bFind = TRUE;
			}
		}

		if (!bFind) 
		{
			m_iSerialPort = 0;
			m_cSerialPort.SetCurSel(0);
			m_cSerialPort.GetLBText(0, m_strSerial);
			m_strSerial.Format("%s", RemoveSerialInfo(m_strSerial));
		}
	}
}

CString CDataAnalysisDlg::GetLastComPort(void) 
{
	CString strAddress = "Common";
	CString strItem = "LastComPort";

	return GetRegRoot_RegistryData(strAddress, strItem);
}


LRESULT  CDataAnalysisDlg::OnCommunication(WPARAM wParam, LPARAM lParam)
{
	//UpdateData(TRUE);//받는 데이터 타입을 알기 위해
	BYTE aByte; //데이터를 저장할 변수 
	int iSize = (m_ComuPort.m_QueueRead).GetSize(); //포트로 들어온 데이터 갯수

	static int iSequence = 0; 
	static int iStep = 0;

	static char cRcvBuffer[300 + 1];
	static int iRcvBufferIndex = 0;

	static int isGetData = FALSE;
	static int value = 0;
	static int order = 1;
	static int findPeriod = FALSE;
	static int isMinus = FALSE;

	for (int i = 0; i < iSize; i++) 
	{	//들어온 갯수 만큼 데이터를 읽어 와 화면에 보여줌
		(m_ComuPort.m_QueueRead).GetByte(&aByte);//큐에서 데이터 한개를 읽어옴
		switch (iSequence)
		{
			case 	SEQ_HEADER_CHECK_FIRST:
				iStep = 0;
				isGetData = FALSE;
				value = 0;
				order = 1;
				findPeriod = FALSE;
				isMinus = FALSE;

				if (m_Dataset.nHeader > 0)
				{
					if (m_Dataset.pHeader[iStep++] == aByte)
					{
						// Goto next
						if (m_Dataset.nHeader == iStep)
						{
							if (m_Dataset.bUseGroup)
							{
								iSequence = SEQ_GET_GROUP;
								iStep = 0;
							}
							else {
								iSequence = SEQ_HEDER_CHECK_UNTIL_LAST;
							}
						}
					}
					break;
				}

			case 	SEQ_HEDER_CHECK_UNTIL_LAST:
				if (m_Dataset.nHeader > 0)
				{
					if (m_Dataset.pHeader[iStep++] == aByte)
					{
						// Goto next
						if (m_Dataset.nHeader == iStep)
						{
							if (m_Dataset.bUseGroup)
							{
								iSequence = SEQ_GET_GROUP;
							}
							else {
								iSequence = SEQ_HEDER_CHECK_UNTIL_LAST;
							}
							iStep = 0;
						}
					}
					else {
						iSequence = SEQ_HEADER_CHECK_FIRST;
					}
					break;
				}
			

			case 	SEQ_GET_GROUP:
				if (m_Dataset.bUseGroup == TRUE)
				{
					m_pData[m_index].group = aByte;

					if (m_Dataset.PacketValueType == 0) 
					{
						iSequence = SEQ_TEXT_VALUE_GET;
					}
					else
					{
						iSequence = SEQ_BIN_VALUE_GET;
					}
					break;
				}

			case	SEQ_TEXT_VALUE_GET:
				if (m_Dataset.PacketValueType == 0) // TEXT이면
				{					
					// FirstDataCheck
					if (isGetData) {
						if (aByte >= '0' && aByte <= '9') 
						{
							value *= 10;
							value += (aByte - '0');
							if (findPeriod) order *= 10;
						}
						else if(aByte == '.')
						{
							findPeriod = TRUE;
						}
						else {
							if (isMinus) value = -value;
							
							if (order == 1)
								m_pData[m_index].pValue[iStep] = value;
							else
								m_pData[m_index].pValue[iStep] = ((double)value) / order;


							isGetData = FALSE;
							value = 0;
							order = 1;
							findPeriod = FALSE;
							isMinus = FALSE;

							if (++iStep == m_Dataset.nValue) 
							{
								iSequence = SEQ_WAIT_END;
							}

							if (aByte == '\n') {
								GetNewDataPacket();
								iSequence = SEQ_HEADER_CHECK_FIRST;
							}
						}
					}
					else {
						if (aByte >= '0' && aByte <= '9')
						{
							isGetData = TRUE;
							 value = aByte - '0';
						}
						else if (aByte == '.')
						{
							findPeriod = TRUE;
							isGetData = TRUE;
							value = 0;
						}
						else if (aByte == '-')
						{
							isMinus = TRUE;
						}
					}
					break;
				}

			case	SEQ_BIN_VALUE_GET:
				if (m_Dataset.PacketValueType == 1) // Binary이면
				{

					break;
				}

			case	 SEQ_GET_TAIL_CHECK_FIRST:
				if (m_Dataset.nTail > 0) // Binary이면
				{

					break;
				}

			case  	SEQ_GET_TAIL_CHECK_UNTIL_LAST:
				if (m_Dataset.nTail > 0) // Binary이면
				{

					break;
				}


			case SEQ_WAIT_END:
				if (aByte == '\n') {
					GetNewDataPacket();
					iSequence = SEQ_HEADER_CHECK_FIRST;
				}
				break;
		}
	}
	//SetDlgItemInt(IDC_Debug, iSize);
	return 1;
}

void CDataAnalysisDlg::GetNewDataPacket()
{
	
	SetDlgItemInt(IDC_Debug, m_index);

	CString str, str1; 
	str.Format("%c ", m_pData[m_index].group);

	for (int i = 0; i < m_Dataset.nValue; i++) {
		str1.Format("%.0f ", m_pData[m_index].pValue[i]);
		str += str1;
	}
	
	int count = m_LogList.GetCount();
	m_LogList.InsertString(count, str);
	m_LogList.SetCurSel(count);
	m_index++;
}