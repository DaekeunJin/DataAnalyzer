
// DataAnalysisDlg.h: 헤더 파일
//

#pragma once

#include "config.h"
#include "DataFormat.h"


// CDataAnalysisDlg 대화 상자
class CDataAnalysisDlg : public CDialogEx
{
public:
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
};
