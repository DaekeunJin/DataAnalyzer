#pragma once

#include "Colorlistctrl.h"
#include "DataAnalysisDlg.h"

// CSettingDlg 대화 상자

#define DRAW_START_X 233
#define DRAW_START_Y 162


class CSettingDlg : public CDialogEx
{
private:
    int m_iDataItem;
    int m_iSelectItem;
    int m_iPreDataItem;

	DECLARE_DYNAMIC(CSettingDlg)

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    DECLARE_MESSAGE_MAP()

public:
	CSettingDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSettingDlg();
    CDataAnalysisDlg* pMain;
    void Get_InitData(void);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SettingDlg };
#endif


    afx_msg void OnBnClickedTestbutton();
    virtual BOOL OnInitDialog();
    CColorListCtrl m_DataList;
    void AddColumn();
    void ShowHeadItem(void);
    void InsertDataListItem();
    afx_msg void OnBnClickedDataitem0();
    afx_msg void OnBnClickedDataitem1();
    afx_msg void OnBnClickedDataitem2();
    afx_msg void OnBnClickedDataitem3();
    afx_msg void OnBnClickedDataitem4();
    afx_msg void OnBnClickedGetDefaultDataset();
    afx_msg void OnBnClickedSavesetting();

    int CheckInputData();
    afx_msg void OnClickDataformatlist(NMHDR *pNMHDR, LRESULT *pResult);
    void ShowInputControl();

    int m_IDC_Text;
    int m_IDC_Value;

    void ShowHeaderControl();
    void ShowUseGroupControl();
    void ShowValueControl();
    void ShowChecksumControl();
    void ShowTailControl();

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedInputdataset();
	afx_msg void OnBnClickedUsehexa();
	afx_msg void OnBnClickedSetdataset();
};

