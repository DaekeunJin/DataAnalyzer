
// DataAnalysisDlg.h: 헤더 파일
//

#pragma once

#include "config.h"
#include "DataFormat.h"
#include "CommThread.h"
#include "resource.h"
#include <vector>


#define DATA_ACQUISITION_MODE	1
#define DATA_ANALYZE_MODE		0

// CDataAnalysisDlg 대화 상자
class CDataAnalysisDlg : public CDialogEx
{
private:
	CCommThread m_ComuPort;
	CComboBox m_cSerialPort;
	int m_iSerialPort;
	CString m_strSerial;
	CComboBox m_cBaudRate;
	int m_iBaudRate;

	bool m_bUpdateList;
	int m_iCurrentMode;

	int m_iConnectedPortCount;
	int m_iConnectComPort[MAX_COM_PORT_SIZE];

	std::vector<CEdit*> m_editControls;  // CEdit 객체를 관리할 벡터
	void CreateEditControls(int count);  // CEdit 컨트롤을 생성하는 함수
		
	afx_msg LRESULT OnPostInit(WPARAM wParam, LPARAM lParam);  // 사용자 정의 메시지 핸들러
	static const UINT WM_POST_INIT = WM_USER + 2;  // 사용자 정의 메시지

public:
	virtual ~CDataAnalysisDlg();


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    DataFormat_t m_Dataset;
    Data_t* m_pData = nullptr;
    int m_index;

    CString m_strModel;
    CDataAnalysisDlg(CWnd* pParent = nullptr);
	enum { IDD = IDD_DataAnalysisDlg };
    afx_msg void OnBnClickedReqsettingdlg();
    afx_msg void OnBnClickedTestbutton();
    void SetRegRoot_RegistryData(const CString& strAddress, const CString& strItem, const CString& stData);
    CString GetRegRoot_RegistryData(const CString& strAddress, const CString& strItem);
    void SetReg_RegistryData(const CString& strAddress, const CString& strItem, const CString& stData);
    CString GetReg_RegistryData(const CString& strAddress, const CString& strItem);


    void GetDataFormat_fromReg();

    void SetData(int nData, int nValue, int nMath, bool bUpdate = TRUE);
    void GetReg_HeaderData();
    virtual BOOL DestroyWindow();

	// Common function
	int Hex2Dec(CString str);
	afx_msg void OnBnClickedBtnPortOpen();
	
	afx_msg void OnBnClickedBtnPortClose();
	void Wait(DWORD dwMillisecond);
	CString RemoveSerialInfo(CString str);
	CString byNameComPort(CString str);
	DWORD byIndexBaud(int xBaud);
	int  String2Num(CString str);
	void SetLastComPort(CString strComPort);
	afx_msg void OnCbnSelchangeSerialPort();
	afx_msg void OnCbnSelchangeBaudRate();
	void InitSerialPort();
	void OnSetfocusSerialPort();
	CString GetLastComPort(void);

	afx_msg LRESULT OnCommunication(WPARAM wParam, LPARAM lParam);
	void GetNewDataPacket();
	CListBox m_LogList;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

};

