
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


// CDataAnalysisDlg 대화 상자



CDataAnalysisDlg::CDataAnalysisDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DataAnalysisDlg, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDataAnalysisDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_ReqSettingDlg, &CDataAnalysisDlg::OnBnClickedReqsettingdlg)
    ON_BN_CLICKED(IDC_TestButton, &CDataAnalysisDlg::OnBnClickedTestbutton)
END_MESSAGE_MAP()


// CDataAnalysisDlg 메시지 처리기

BOOL CDataAnalysisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
    GetDataFormat_fromReg();


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