// CameraFmtDialog.cpp : implementation file
//

#include "stdafx.h"
#include "IWB.h"
#include "CameraFmtDialog.h"
#include "afxdialogex.h"

// CameraFmtDialog dialog

IMPLEMENT_DYNAMIC(CameraFmtDialog, CDialog)

CameraFmtDialog::CameraFmtDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_ADANCEDSETTINGS, pParent)
	, m_sCurrentCameraResution(_T(""))
	, m_SelectedCameraFmtValue(_T(""))
	, m_bHIDMode(TRUE)
	, m_bTUIOMode(FALSE)
	, m_IPAddress(0)
	, m_strPort(_T(""))
{

}

CameraFmtDialog::~CameraFmtDialog()
{
}

void CameraFmtDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAMERAFMT_COMBO, m_CCameraFmt);
	DDX_CBString(pDX, IDC_CAMERAFMT_COMBO, m_SelectedCameraFmtValue);
	DDX_Check(pDX, IDC_CHECK_HID, m_bHIDMode);
	DDX_Check(pDX, IDC_CHECK_TUIO, m_bTUIOMode);
	DDX_IPAddress(pDX, IDC_IPADDRESS_IP, m_IPAddress);
	DDX_Text(pDX, IDC_EDIT_PORT, m_strPort);
	DDX_Control(pDX, IDC_IPADDRESS_IP, CIPAddress);
	DDX_Control(pDX, IDC_EDIT_PORT, CPortEdit);
}


BEGIN_MESSAGE_MAP(CameraFmtDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CameraFmtDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CameraFmtDialog::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_CAMERAFMT_COMBO, &CameraFmtDialog::OnCbnSelchangeCamerafmtCombo)
	ON_BN_CLICKED(IDC_CHECK_HID, &CameraFmtDialog::OnBnClickedCheckHid)
	ON_BN_CLICKED(IDC_CHECK_TUIO, &CameraFmtDialog::OnBnClickedCheckTuio)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_IP, &CameraFmtDialog::OnIpnFieldchangedIpaddressIp)
	ON_EN_CHANGE(IDC_EDIT_PORT, &CameraFmtDialog::OnEnChangeEditPort)
END_MESSAGE_MAP()


BOOL CameraFmtDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here
	if (CameraAllResution.empty())
	{
		m_CCameraFmt.AddString(_T("--------"));
		m_CCameraFmt.SetCurSel(0);
		m_SelectedCameraFmtValue = "";
	}
	else
	{
		for (int i = 0 ; i <CameraAllResution.size();i++ )
		{
			m_CCameraFmt.AddString((LPCTSTR)CameraAllResution[i]);
	    }
	    int index = m_CCameraFmt.SelectString(0, (LPCTSTR)m_sCurrentCameraResution);
	    m_CCameraFmt.SetCurSel(index);
	    m_SelectedCameraFmtValue = m_sCurrentCameraResution;
	}

	if (theApp.GetUSBKeyTouchType() == E_DEVICE_FINGER_TOUCH_WHITEBOARD || theApp.GetUSBKeyTouchType() == E_DEVICE_PEN_TOUCH_WHITEBOARD)
	{
		GetDlgItem(IDC_CHECK_TUIO)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_HID)->EnableWindow(false);
		GetDlgItem(IDC_IPADDRESS_IP)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(false);
	}
	else
	{
	    ((CButton*)GetDlgItem(IDC_CHECK_HID))->SetCheck(g_tSysCfgData.globalSettings.bTouchHIDMode);
	    ((CButton*)GetDlgItem(IDC_CHECK_TUIO))->SetCheck(g_tSysCfgData.globalSettings.bTouchTUIOMode);
	
	    unsigned   char   *pIP = (unsigned   char*)&m_IPAddress;
	    CIPAddress.SetAddress(*pIP, *(pIP + 1), *(pIP + 2), *(pIP + 3));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CameraFmtDialog::SetCameraResolution(std::vector<CAtlString>& CameraInfo, CAtlString  CurrentCameraInfo)
{
	CameraAllResution = CameraInfo;
	m_sCurrentCameraResution = CurrentCameraInfo;
}

void CameraFmtDialog::SetIPadressAndPort(DWORD IP, int nPort)
{
	m_IPAddress = IP;
	m_strPort.Format(_T("%d"), nPort);
}

void CameraFmtDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDialog::OnOK();
}


void CameraFmtDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}


void CameraFmtDialog::OnCbnSelchangeCamerafmtCombo()
{
	// TODO: Add your control notification handler code here
	m_SelectedCameraFmtValue =_T("");
	m_CCameraFmt.GetLBText(m_CCameraFmt.GetCurSel(), m_SelectedCameraFmtValue);
	m_CCameraFmt.SetWindowTextW(m_SelectedCameraFmtValue);
}

CAtlString  CameraFmtDialog::GetSelectComboxvalue()
{
	return  m_SelectedCameraFmtValue;
}

DWORD CameraFmtDialog::GetIPAddress()
{
	return m_IPAddress;
}

int CameraFmtDialog::GetPort()
{
	return  _ttoi(m_strPort);
}

void CameraFmtDialog::OnBnClickedCheckHid()
{
	// TODO: Add your control notification handler code here
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_HID))->GetCheck())
	{
     	((CButton*)GetDlgItem(IDC_CHECK_HID))->SetCheck(true);
		m_bHIDMode = TRUE;
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_HID))->SetCheck(false);
		m_bHIDMode = FALSE;
	}
}


void CameraFmtDialog::OnBnClickedCheckTuio()
{
	// TODO: Add your control notification handler code here
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_TUIO))->GetCheck())
	{
	     ((CButton*)GetDlgItem(IDC_CHECK_TUIO))->SetCheck(true);
		 m_bTUIOMode = TRUE;
		 CIPAddress.EnableWindow(true);
		 CPortEdit.EnableWindow(true);
	}
	else {
		((CButton*)GetDlgItem(IDC_CHECK_TUIO))->SetCheck(false);
		m_bTUIOMode = FALSE;

		//////配置IP和端口都要灰掉
		CIPAddress.EnableWindow(false);
		CPortEdit.EnableWindow(false);

	}
}

bool CameraFmtDialog::GetTouchHIDMode()
{
	return m_bHIDMode;
}

bool CameraFmtDialog::GetTouchTUIOMode()
{
	return m_bTUIOMode;
}

void CameraFmtDialog::OnIpnFieldchangedIpaddressIp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(true);
	*pResult = 0;
}


void CameraFmtDialog::OnEnChangeEditPort()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(true);
}
