// SettingDlg.cpp: 구현 파일
//

#include "pch.h"
#include "DataAnalysis.h"
#include "SettingDlg.h"
#include "afxdialogex.h"


// CSettingDlg 대화 상자

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SettingDlg, pParent)
    , m_iDataItem(FALSE)
    , m_iSelectItem(-1)
    , m_iPreDataItem(-1)
    , m_IDC_Text(IDC_ItemText0)
    , m_IDC_Value(IDC_value0)
{
}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DataFormatList, m_DataList);
    DDX_Radio(pDX, IDC_DataItem0, m_iDataItem);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
    ON_BN_CLICKED(IDC_TestButton, &CSettingDlg::OnBnClickedTestbutton)
    ON_BN_CLICKED(IDC_DataItem0, &CSettingDlg::OnBnClickedDataitem0)
    ON_BN_CLICKED(IDC_DataItem1, &CSettingDlg::OnBnClickedDataitem1)
    ON_BN_CLICKED(IDC_DataItem2, &CSettingDlg::OnBnClickedDataitem2)
    ON_BN_CLICKED(IDC_DataItem3, &CSettingDlg::OnBnClickedDataitem3)
    ON_BN_CLICKED(IDC_DataItem4, &CSettingDlg::OnBnClickedDataitem4)
    ON_BN_CLICKED(IDC_Get_Default_DataSet, &CSettingDlg::OnBnClickedGetDefaultDataset)
    ON_BN_CLICKED(IDC_SaveSetting, &CSettingDlg::OnBnClickedSavesetting)
    ON_NOTIFY(NM_CLICK, IDC_DataFormatList, &CSettingDlg::OnClickDataformatlist)
    ON_WM_LBUTTONDOWN()
    ON_BN_CLICKED(IDC_InputDataset, &CSettingDlg::OnBnClickedInputdataset)
	ON_BN_CLICKED(IDC_UseHexa, &CSettingDlg::OnBnClickedUsehexa)
	ON_BN_CLICKED(IDC_SetDataSet, &CSettingDlg::OnBnClickedSetdataset)
END_MESSAGE_MAP()


// CSettingDlg 메시지 처리기


void CSettingDlg::OnBnClickedTestbutton()
{
}


BOOL CSettingDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  여기에 추가 초기화 작업을 추가합니다.
    AddColumn();
    Get_InitData();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSettingDlg::AddColumn() {
    LV_COLUMN lvcolumn;
    char *listColumn[4] = { "No.", "Item", "Value", "Remark",  };
    int width[4] = { 1, 1, 1, 1 };
    int i;

    for (i = 0; i < 4; i++) 
    {
        lvcolumn.fmt = LVCFMT_CENTER;
        lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
        lvcolumn.pszText = listColumn[i];
        lvcolumn.iSubItem = i;
        lvcolumn.cx = width[i];
        m_DataList.InsertColumn(i, &lvcolumn);
    }

    m_DataList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    for (i = 0; i < m_DataList.GetHeaderCtrl()->GetItemCount(); i++)
    {
        VERIFY(m_DataList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));
    }

    m_iDataItem = 0;
    ShowHeadItem();
}

void CSettingDlg::InsertDataListItem() {
    // Add Blank Row in DataList Congtrol 
    LV_ITEM lvitem;
    CString str;

    int nCount = m_DataList.GetItemCount();
    lvitem.mask = LVIF_TEXT;
    lvitem.iSubItem = 0;
    lvitem.iItem = nCount;
    str.Format("%d", nCount);
    lvitem.pszText = (LPSTR)(LPCSTR)str;
    m_DataList.InsertItem(&lvitem);
}


void CSettingDlg::OnBnClickedDataitem0() 
{
    m_iDataItem = 0;
    if (m_iPreDataItem != m_iDataItem)
    {
        ShowHeadItem();
    }
}


void CSettingDlg::OnBnClickedDataitem1() 
{
    m_iDataItem = 1;
    if (m_iPreDataItem != m_iDataItem)
    {
        ShowHeadItem();
    }
}


void CSettingDlg::OnBnClickedDataitem2() 
{
    m_iDataItem = 2;
    if (m_iPreDataItem != m_iDataItem)
    {
        ShowHeadItem();
    }
}


void CSettingDlg::OnBnClickedDataitem3() 
{
    m_iDataItem = 3;
    if (m_iPreDataItem != m_iDataItem)
    {
        ShowHeadItem();
    }
}


void CSettingDlg::OnBnClickedDataitem4() 
{
    m_iDataItem = 4;
    if (m_iPreDataItem != m_iDataItem)
    {
        ShowHeadItem();
    }
}

void CSettingDlg::OnBnClickedGetDefaultDataset()
{
    SetDlgItemInt(IDC_N_MIN_DATA, pMain->m_Dataset.nData);
    SetDlgItemInt(IDC_N_MIN_VALUE, pMain->m_Dataset.nValue);
    SetDlgItemInt(IDC_N_MIN_MATH, pMain->m_Dataset.nMath);
}


void CSettingDlg::Get_InitData(void)
{
    OnBnClickedGetDefaultDataset();
}

void CSettingDlg::OnBnClickedSavesetting()
{
	OnBnClickedSetdataset();
}

int CSettingDlg::CheckInputData()
{
    int errCode = 0;
    // Dataset 
    if (GetDlgItemInt(IDC_N_MIN_DATA) < N_MIN_DATA) errCode = 1;
    if (GetDlgItemInt(IDC_N_MIN_VALUE) < N_MIN_VALUE) errCode = 2;
    if (GetDlgItemInt(IDC_N_MIN_MATH) < N_MIN_MATH) errCode = 3;

    if (GetDlgItemInt(IDC_N_MIN_DATA) > N_MAX_DATA) errCode = 4;
    if (GetDlgItemInt(IDC_N_MIN_VALUE) > N_MAX_VALUE) errCode = 5;
    if (GetDlgItemInt(IDC_N_MIN_MATH) > N_MAX_MATH) errCode = 6;

    int totalSize = GetDlgItemInt(IDC_N_MIN_DATA) * 8 * (GetDlgItemInt(IDC_N_MIN_VALUE) + GetDlgItemInt(IDC_N_MIN_MATH));
    if (totalSize > 500000000) errCode = 7;
	
    if (errCode > 0)
    {
        CString str;
        str.Format("errCode : %d", errCode);
        AfxMessageBox(str);
        return false;
    }
    return true;
}

void CSettingDlg::OnClickDataformatlist(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    m_iSelectItem = pNMItemActivate->iItem;
    
    CString str;
    if (m_iSelectItem >= 0)
    {
        str.Format("select no.: %d", m_iSelectItem);
    }
    else 
    {
        str.Format("");
    }
    SetDlgItemText(IDC_CurSelectItem, str);
	ShowInputControl();
    *pResult = 0;
}


void CSettingDlg::ShowInputControl() {
    // Hide all control
    //GetDligItem
    GetDlgItem(m_IDC_Text)->ShowWindow(FALSE);
    GetDlgItem(m_IDC_Value)->ShowWindow(FALSE);
    GetDlgItem(IDC_InputDataset)->ShowWindow(FALSE);
    GetDlgItem(IDC_Datatype)->ShowWindow(FALSE);
    GetDlgItem(IDC_Checksum)->ShowWindow(FALSE);
	GetDlgItem(IDC_UseHexa)->ShowWindow(FALSE);

    if (m_iSelectItem > -1)
    {
        switch (m_iDataItem)
        {
        case 0: // Header
            ShowHeaderControl();
            break;

        case 1: // Use Group
            ShowUseGroupControl();
            break;

        case 2: // nValue,  Data Type 
            ShowValueControl();
            break;

        case 3: // Header
            ShowChecksumControl();
            break;

        case 4: // Header
            ShowTailControl();
            break;
        }
    }
	
	if (m_iSelectItem == -1)
	{
		GetDlgItem(IDC_DataFormatList)->MoveWindow(235, 41, 687 - 235, 181 - 41, 1);
	}
	else
	{
		GetDlgItem(IDC_DataFormatList)->MoveWindow(235, 41, 687 - 235, 154 - 41, 1);
	}
}

void CSettingDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    CString str;
    str.Format("Px: %d   Py: %d", point.x, point.y);
    SetDlgItemText(IDC_SettingMessage, str);
    CDialogEx::OnLButtonDown(nFlags, point);
}

void CSettingDlg::ShowHeaderControl() {
    CString str;
    int px = DRAW_START_X;
    int py = DRAW_START_Y;
    int gx = 5;
  
    int sx;
    int sy = 17;
    int nShow = 0;	

    if (m_iSelectItem == 0)
    {
        nShow = 1;
        sx = 75;
        str.Format("nHeader");
        SetDlgItemText(m_IDC_Text, str);
        GetDlgItem(m_IDC_Text)->MoveWindow(px, py, sx, sy, 1);

        px += (sx + gx);
        py -= 2;
        sx = 50;
        str.Format("%d", pMain->m_Dataset.nHeader);
        SetDlgItemText(m_IDC_Value, str);
        GetDlgItem(m_IDC_Value)->MoveWindow(px, py, sx, sy, 1);
        px += sx + gx;
    }
    else {
        nShow = 1;
        sx = 75;
        int idx = m_iSelectItem - 1;
        str.Format("Header[%d]", idx);
        SetDlgItemText(m_IDC_Text, str);
        GetDlgItem(m_IDC_Text)->MoveWindow(px, py, sx, sy, 1);

		px += (sx + gx + 20);
		sx = 50;
		py -= 2;

		GetDlgItem(IDC_UseHexa)->MoveWindow(px, py, sx, sy, 1);
		GetDlgItem(IDC_UseHexa)->ShowWindow(TRUE);

        px += (sx + gx);        
        sx = 50; 
		if (IsDlgButtonChecked(IDC_UseHexa))
			str.Format("0x%X", pMain->m_Dataset.pHeader[idx]);
		else 
			str.Format("%d", pMain->m_Dataset.pHeader[idx]);
        SetDlgItemText(m_IDC_Value, str);
        GetDlgItem(m_IDC_Value)->MoveWindow(px, py, sx, sy, 1);
        px += sx + gx;
    }

    // Input Button
    GetDlgItem(IDC_InputDataset)->MoveWindow(px+10, py, sx, sy, 1);

    // Show Controls
    for (int i = 0; i < nShow; i++)
    {
        GetDlgItem(m_IDC_Text)->ShowWindow(TRUE);
        GetDlgItem(m_IDC_Value)->ShowWindow(TRUE);
    }
    GetDlgItem(IDC_InputDataset)->ShowWindow(TRUE);
}


void CSettingDlg::ShowUseGroupControl() {
    CString str;
    int px = DRAW_START_X;
    int py = DRAW_START_Y;
    int gx = 5;

    int sx;
    int sy = 17;

    if (m_iSelectItem == 0)
    {
        sx = 75;
        str.Format("UseGroup");
        SetDlgItemText(m_IDC_Text, str);
        GetDlgItem(m_IDC_Text)->MoveWindow(px, py, sx, sy, 1);

        px += (sx + gx);

        sx = 50; py -= 2;
        str.Format("%d", pMain->m_Dataset.bUseGroup);
        SetDlgItemText(m_IDC_Value, str);
        GetDlgItem(m_IDC_Value)->MoveWindow(px, py, sx, sy, 1);
        px += sx + gx;

        // Input Button
        GetDlgItem(IDC_InputDataset)->MoveWindow(px + 10, py, sx, sy, 1);

        // Show Controls
        GetDlgItem(m_IDC_Text)->ShowWindow(TRUE);
        GetDlgItem(m_IDC_Value)->ShowWindow(TRUE);
        GetDlgItem(IDC_InputDataset)->ShowWindow(TRUE);
    }
}


void CSettingDlg::ShowValueControl() {
    CString str;
    int px = DRAW_START_X;
    int py = DRAW_START_Y;
    int gx = 5;

    int sx;
    int sy = 17;

    if (m_iSelectItem == 0)
    {
        sx = 75;
        str.Format("nValue");
        SetDlgItemText(m_IDC_Text, str);
        GetDlgItem(m_IDC_Text)->MoveWindow(px, py, sx, sy, 1);

        px += (sx + gx);

        sx = 50; py -= 2;
        str.Format("%d", pMain->m_Dataset.nValue);
        SetDlgItemText(m_IDC_Value, str);
        GetDlgItem(m_IDC_Value)->MoveWindow(px, py, sx, sy, 1);
        px += sx + gx;
        GetDlgItem(m_IDC_Value)->ShowWindow(TRUE);
    }
    else if (m_iSelectItem == 1)
    {
        sx = 75;
        str.Format("PacketType");
        SetDlgItemText(m_IDC_Text, str);
        GetDlgItem(m_IDC_Text)->MoveWindow(px, py, sx, sy, 1);

        px += (sx + gx);

        sx = 50; py -= 2;
        str.Format("%d", pMain->m_Dataset.PacketValueType);
        SetDlgItemText(m_IDC_Value, str);
        GetDlgItem(m_IDC_Value)->MoveWindow(px, py, sx, sy, 1);
        px += sx + gx;
        GetDlgItem(m_IDC_Value)->ShowWindow(TRUE);
    }
    else
    {
        sx = 100;
        int idx = m_iSelectItem - 2;
        str.Format("ValueType[%d]", idx);
        SetDlgItemText(m_IDC_Text, str);
        GetDlgItem(m_IDC_Text)->MoveWindow(px, py, sx, sy, 1);

        px += (sx + gx);

        sx = 80; py -= 2;
        GetDlgItem(IDC_Datatype)->MoveWindow(px, py, sx, sy, 1);
        px += sx + gx; sx = 50;
        ((CComboBox *)GetDlgItem(IDC_Datatype))->SetCurSel(pMain->m_Dataset.pValueType[idx]);
        GetDlgItem(IDC_Datatype)->ShowWindow(TRUE);
    }

    // Input Button
    GetDlgItem(IDC_InputDataset)->MoveWindow(px + 10, py, sx, sy, 1);

    // Show Default  Controls
    GetDlgItem(m_IDC_Text)->ShowWindow(TRUE);
    
    GetDlgItem(IDC_InputDataset)->ShowWindow(TRUE);
}


void CSettingDlg::ShowChecksumControl() {
    CString str;
    int px = DRAW_START_X;
    int py = DRAW_START_Y;
    int gx = 5;

    int sx;
    int sy = 17;

    if (m_iSelectItem == 0)
    {
        sx = 100;
        int idx = m_iSelectItem - 2;
        str.Format("Checksum Type", idx);
        SetDlgItemText(m_IDC_Text, str);
        GetDlgItem(m_IDC_Text)->MoveWindow(px, py, sx, sy, 1);

        px += (sx + gx);

        sx = 80; py -= 2;
        GetDlgItem(IDC_Checksum)->MoveWindow(px, py, sx, sy, 1);
        px += sx + gx; sx = 50;
        ((CComboBox *)GetDlgItem(IDC_Checksum))->SetCurSel(pMain->m_Dataset.ChecksumType);
        GetDlgItem(IDC_Checksum)->ShowWindow(TRUE);
    }

    // Input Button
    GetDlgItem(IDC_InputDataset)->MoveWindow(px + 10, py, sx, sy, 1);

    // Show Default  Controls
    GetDlgItem(m_IDC_Text)->ShowWindow(TRUE);

    GetDlgItem(IDC_InputDataset)->ShowWindow(TRUE);
}


void CSettingDlg::ShowTailControl() {
	CString str;
	int px = DRAW_START_X;
	int py = DRAW_START_Y;
	int gx = 5;

	int sx;
	int sy = 17;
	int nShow = 0;

	if (m_iSelectItem == 0)
	{
		nShow = 1;
		sx = 75;
		str.Format("nTail");
		SetDlgItemText(m_IDC_Text, str);
		GetDlgItem(m_IDC_Text)->MoveWindow(px, py, sx, sy, 1);

		px += (sx + gx);
		py -= 2;
		sx = 50;
		str.Format("%d", pMain->m_Dataset.nTail);
		SetDlgItemText(m_IDC_Value, str);
		GetDlgItem(m_IDC_Value)->MoveWindow(px, py, sx, sy, 1);
		px += sx + gx;
	}
	else {
		nShow = 1;
		sx = 75;
		int idx = m_iSelectItem - 1;
		str.Format("Tail[%d]", idx);
		SetDlgItemText(m_IDC_Text, str);
		GetDlgItem(m_IDC_Text)->MoveWindow(px, py, sx, sy, 1);

		px += (sx + gx + 20);
		sx = 50;
		py -= 2;

		GetDlgItem(IDC_UseHexa)->MoveWindow(px, py, sx, sy, 1);
		GetDlgItem(IDC_UseHexa)->ShowWindow(TRUE);

		px += (sx + gx);
		sx = 50;
		if (IsDlgButtonChecked(IDC_UseHexa))
			str.Format("0x%X", pMain->m_Dataset.pTail[idx]);
		else
			str.Format("%d", pMain->m_Dataset.pTail[idx]);
		SetDlgItemText(m_IDC_Value, str);
		GetDlgItem(m_IDC_Value)->MoveWindow(px, py, sx, sy, 1);
		px += sx + gx;
	}

	// Input Button
	GetDlgItem(IDC_InputDataset)->MoveWindow(px + 10, py, sx, sy, 1);

	// Show Controls
	for (int i = 0; i < nShow; i++)
	{
		GetDlgItem(m_IDC_Text)->ShowWindow(TRUE);
		GetDlgItem(m_IDC_Value)->ShowWindow(TRUE);
	}
	GetDlgItem(IDC_InputDataset)->ShowWindow(TRUE);
}


void CSettingDlg::OnBnClickedInputdataset()
{
    CString strValue, strItem;
    int value = -1;
    switch (m_iDataItem) 
    {
    case 0:  //Header
        if (m_iSelectItem == 0) {
            // Check Param. Validity
            value = GetDlgItemInt(m_IDC_Value);
            if (value >= 0 && value <= 255 && value  != pMain->m_Dataset.nHeader) {
                if (pMain->m_Dataset.pHeader != nullptr)
                {
                    BYTE* pre = pMain->m_Dataset.pHeader;
                    if (value > 0)
                    {
                        pMain->m_Dataset.pHeader = new BYTE[value];

                        int nCopy = min(pMain->m_Dataset.nHeader, value);
                        for (int i = 0; i < nCopy; i++)
                        {
                            pMain->m_Dataset.pHeader[i] = pre[i];
                        }
                        for (int i = nCopy; i < value; i++)
                        {
                            pMain->m_Dataset.pHeader[i] = 0;
                            strItem.Format("Header_%d", i);
                            pMain->SetReg_RegistryData("Data\\Header\\Data", strItem, "0");
                        }
                    }
                    delete[] pre;
                }
                else 
                {
                    pMain->m_Dataset.pHeader = new BYTE[value];
                }
                if (value == 0) pMain->m_Dataset.pHeader = nullptr;

                pMain->m_Dataset.nHeader = value;
                // Reg update
                strValue.Format("%d", value);
                pMain->SetReg_RegistryData("Data\\Header", "nHeader", strValue);
            }
        }
        else {
            int idx = m_iSelectItem - 1;
			int value;
			if (IsDlgButtonChecked(IDC_UseHexa))
			{
				CString str;
				GetDlgItemText(m_IDC_Value, str);
				value = pMain->Hex2Dec(str);
			}
			else
			{
				value = GetDlgItemInt(m_IDC_Value);
			}

            if (value >= 0 && value <= 255 && value != pMain->m_Dataset.pHeader[idx])
            {
                CString strValue, strItem;
                pMain->m_Dataset.pHeader[idx] = value;
                strItem.Format("Header_%d", idx);
                strValue.Format("%d", value);
                pMain->SetReg_RegistryData("Data\\Header\\Data", strItem, strValue);
            }
        }
        break;

    case 1: // Use Data Group
        value = GetDlgItemInt(m_IDC_Value);
        if (value >= 0 && value <= 1 && value) {
            strValue.Format("%d", value);
            pMain->SetReg_RegistryData("Data\\Group", "UseGroup", strValue);
            pMain->m_Dataset.bUseGroup = value;
        }
        break;

    case 2: // nValue, Type
        if (m_iSelectItem == 0) {
            // Check Param. Validity
            value = GetDlgItemInt(m_IDC_Value);
            if (value >= N_MIN_VALUE && value <= N_MAX_VALUE && value != pMain->m_Dataset.nValue) 
            {
                if (pMain->m_Dataset.PacketValueType == 1)
                {
                    if (pMain->m_Dataset.pValueType != nullptr)
                    {
                        BYTE* pre = pMain->m_Dataset.pValueType;
                        pMain->m_Dataset.pValueType = new BYTE[value];

                        int nCopy = min(pMain->m_Dataset.nValue, value);
                        for (int i = 0; i < nCopy; i++)
                        {
                            pMain->m_Dataset.pValueType[i] = pre[i];
                        }
                        for (int i = nCopy; i < value; i++)
                        {
                            pMain->m_Dataset.pValueType[i] = 0;
                            strItem.Format("ValueType_%d", i);
                            pMain->SetReg_RegistryData("Data\\Value\\Type", strItem, "0");
                        }                       
                        delete[] pre;
                    }
                }

                pMain->m_Dataset.nValue = value;
                // Reg update
                strValue.Format("%d", value);
                pMain->SetReg_RegistryData("Data\\Header", "nValue", strValue);
            }
        }
        else if (m_iSelectItem == 1)
        {
            value = GetDlgItemInt(m_IDC_Value);
            if (value >= 0 && value <= 1 && value != pMain->m_Dataset.PacketValueType) {
                strValue.Format("%d", value);
                pMain->SetReg_RegistryData("Data", "PacketDataType", strValue);
                pMain->m_Dataset.PacketValueType = value;
            }
        }
        else 
        {
            int idx = m_iSelectItem - 2;
            value = ((CComboBox *)GetDlgItem(IDC_Datatype))->GetCurSel();
            if (value >= 0 && value < TYPE_LAST && value != pMain->m_Dataset.pValueType[idx])
            {
                pMain->m_Dataset.pValueType[idx] = value;
                strItem.Format("ValueType_%d", idx);
                strValue.Format("%d", value);
                pMain->SetReg_RegistryData("Data\\Value\\Type", strItem, strValue);

            }
        }
        break;

    case 3:
        value = ((CComboBox *)GetDlgItem(IDC_Checksum))->GetCurSel();
        if (value >= 0)
        {
            pMain->m_Dataset.ChecksumType = value;
            strValue.Format("%d", value);
            pMain->SetReg_RegistryData("Data\\Checksum", "ChecksumType", strValue);
        }
        break;

    case 4:  //Tail
        if (m_iSelectItem == 0) {
            // Check Param. Validity
            value = GetDlgItemInt(m_IDC_Value);
            if (value >= 0 && value <= 255 && value != pMain->m_Dataset.nTail) {
                if (pMain->m_Dataset.pTail != nullptr)
                {
                    BYTE* pre = pMain->m_Dataset.pTail;
                    if (value > 0)
                    {
                        pMain->m_Dataset.pTail = new BYTE[value];

                        int nCopy = min(pMain->m_Dataset.nTail, value);
                        for (int i = 0; i < nCopy; i++)
                        {
                            pMain->m_Dataset.pTail[i] = pre[i];
                        }
                        for (int i = nCopy; i < value; i++)
                        {
                            pMain->m_Dataset.pTail[i] = 0;
                            strItem.Format("Tail_%d", i);
                            pMain->SetReg_RegistryData("Data\\Tail\\Data", strItem, "0");
                        }
                    }
                    delete[] pre;
                }
                else
                {
                    pMain->m_Dataset.pTail = new BYTE[value];
                }
                if (value == 0) pMain->m_Dataset.pTail = nullptr;

                pMain->m_Dataset.nTail = value;
                // Reg update
                strValue.Format("%d", value);
                pMain->SetReg_RegistryData("Data\\Tail", "nTail", strValue);
            }
        }
		else {
			int idx = m_iSelectItem - 1;
			int value;
			if (IsDlgButtonChecked(IDC_UseHexa))
			{
				CString str;
				GetDlgItemText(m_IDC_Value, str);
				value = pMain->Hex2Dec(str);
			}
			else
			{
				value = GetDlgItemInt(m_IDC_Value);
			}
			if (value >= 0 && value <= 255 && value != pMain->m_Dataset.pTail[idx])
			{
				CString strValue, strItem;
				pMain->m_Dataset.pTail[idx] = value;
				strItem.Format("Tail_%d", idx);
				strValue.Format("%d", value);
				pMain->SetReg_RegistryData("Data\\Tail\\Data", strItem, strValue);
			}
		}
        break;
    }    
    ShowHeadItem();
}



void CSettingDlg::ShowHeadItem(void)
{
    m_iPreDataItem = m_iDataItem;
    m_iSelectItem = -1;
    SetDlgItemText(IDC_CurSelectItem, "");

    CString str;
    m_DataList.DeleteAllItems();

    switch (m_iDataItem)
    {
    case 0: // Header;
        InsertDataListItem();

        m_DataList.SetItemText(0, 1, "nHeader");
        if (pMain->m_Dataset.nHeader > 0)
        {
            str.Format("%d", pMain->m_Dataset.nHeader);
            m_DataList.SetItemText(0, 2, str);
            m_DataList.SetItemText(0, 3, "Header is consist of nHeader bytes as below");

            for (int i = 0; i < pMain->m_Dataset.nHeader; i++)
            {
                InsertDataListItem();
                str.Format("Header[%d]", i);
                m_DataList.SetItemText(i + 1, 1, str);

                int value = pMain->m_Dataset.pHeader[i];
                if (value < 0 || value >255) value = 0;

                str.Format("0x%02X (%3d)", value, value);
                m_DataList.SetItemText(i + 1, 2, str);
            }
        }
        else
        {
            m_DataList.SetItemText(0, 2, "0");
            m_DataList.SetItemText(0, 3, "Header is not uesd.");
        }
        break;

    case 1: // Data Group;
        InsertDataListItem();
        m_DataList.SetItemText(0, 1, "UseGroup");
        str.Format("%d", pMain->m_Dataset.bUseGroup);
        m_DataList.SetItemText(0, 2, str);
        m_DataList.SetItemText(0, 3, "1: use Data Group / 0: Not used Data Group");
        break;

    case 2: // Value;
        InsertDataListItem();
        m_DataList.SetItemText(0, 1, "nValue");
        str.Format("%d", pMain->m_Dataset.nValue);
        m_DataList.SetItemText(0, 2, str);
        m_DataList.SetItemText(0, 3, "No of Data Value in Packet");

        InsertDataListItem();
        m_DataList.SetItemText(1, 1, "PacketType");
        str.Format("%d", pMain->m_Dataset.PacketValueType);
        m_DataList.SetItemText(1, 2, str);
        m_DataList.SetItemText(1, 3, "0:Text / 1:Binary (needs data type)");

        if (pMain->m_Dataset.PacketValueType == 1)
        {
            for (int i = 0; i < pMain->m_Dataset.nValue; i++)
            {
                InsertDataListItem();
                str.Format("DataType[%d]", i);
                m_DataList.SetItemText(i + 2, 1, str);

                int value = pMain->m_Dataset.pValueType[i];
                switch (value)
                {
                case TYPE_INT32:  // INT32
                    str.Format(" int32_t  (%d) ", value);
                    break;

                case TYPE_UINT32:
                    str.Format(" uint32_t (%d) ", value);
                    break;

                case TYPE_INT8:
                    str.Format(" int8_t   (%d) ", value);
                    break;
                case TYPE_UINT8:
                    str.Format(" uint8_t  (%d) ", value);
                    break;

                case TYPE_INT16:
                    str.Format(" int16_t  (%d) ", value);
                    break;
                case TYPE_UINT16:

                    str.Format(" uint16_t (%d) ", value);
                    break;
                case TYPE_FLOAT:
                    str.Format(" float    (%d) ", value);
                    break;

                case TYPE_DOUBLE:
                    str.Format(" double   (%d) ", value);
                    break;
                default :

                    break;
                }

                m_DataList.SetItemText(i + 2, 2, str);
            }
        }
        break;

    case 3: // Checksum;
        InsertDataListItem();
        m_DataList.SetItemText(0, 1, "Checksum");
        switch (pMain->m_Dataset.ChecksumType)
        {
        case 0:
            str.Format("  NONE  ");
            break;
        case 1:
            str.Format("Checksum");
            break;
        case 2:
            str.Format(" CRC-16 ");
            break;
        case 3:
            str.Format(" CRC-32 ");
            break;
        case 4:
            str.Format(" CRC-64 ");
            break;
        }
        m_DataList.SetItemText(0, 2, str);
        break;

    case 4: // Tail;
        InsertDataListItem();

        m_DataList.SetItemText(0, 1, "nTail");
        if (pMain->m_Dataset.nTail > 0)
        {
            str.Format("%d", pMain->m_Dataset.nTail);
            m_DataList.SetItemText(0, 2, str);
            m_DataList.SetItemText(0, 3, "Tail is consist of nTail bytes as below");

            for (int i = 0; i < pMain->m_Dataset.nTail; i++)
            {
                InsertDataListItem();
                str.Format("Tail[%d]", i);
                m_DataList.SetItemText(i + 1, 1, str);

                int value = pMain->m_Dataset.pTail[i];
                if (value < 0 || value >255) value = 0;

                str.Format("0x%02X (%3d)", value, value);
                m_DataList.SetItemText(i + 1, 2, str);
            }
        }
        else
        {
            m_DataList.SetItemText(0, 2, "0");
            m_DataList.SetItemText(0, 3, "Tail is not uesd.");
        }
        break;
    }

    for (int i = 0; i < m_DataList.GetHeaderCtrl()->GetItemCount(); i++)
    {
        VERIFY(m_DataList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));
    }

	ShowInputControl();
}

void CSettingDlg::OnBnClickedUsehexa()
{
	// Value Change
	
	CString strNew;
	int value;
	if (IsDlgButtonChecked(IDC_UseHexa))
	{
		value = GetDlgItemInt(IDC_value0);
		strNew.Format("0x%X", value);
	}
	else
	{		
		CString strPre;
		GetDlgItemText(IDC_value0, strPre);
		value = pMain->Hex2Dec(strPre);
		strNew.Format("%d", value);
	}
	SetDlgItemText(IDC_value0, strNew);
}


void CSettingDlg::OnBnClickedSetdataset()
{
	if (CheckInputData())
	{
		CString strValue;
		// Dataset 
		strValue.Format("%d", GetDlgItemInt(IDC_N_MIN_DATA));
		pMain->SetReg_RegistryData("Data", "Default_N_Data", strValue);

		strValue.Format("%d", GetDlgItemInt(IDC_N_MIN_VALUE));
		pMain->SetReg_RegistryData("Data\\Value", "nValue", strValue);

		strValue.Format("%d", GetDlgItemInt(IDC_N_MIN_MATH));
		pMain->SetReg_RegistryData("Data", "Default_N_Math", strValue);
	}
}
