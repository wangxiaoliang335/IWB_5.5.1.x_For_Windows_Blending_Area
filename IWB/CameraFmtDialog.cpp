// CameraFmtDialog.cpp : implementation file
//

#include "stdafx.h"
#include "IWB.h"
#include "CameraFmtDialog.h"
#include "afxdialogex.h"

// CameraFmtDialog dialog

IMPLEMENT_DYNAMIC(CameraFmtDialog, CDialog)

CameraFmtDialog::CameraFmtDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CAMERA_FMT_DIALOG, pParent)
	, m_sCurrentCameraResution(_T(""))
	, m_SelectedCameraFmtValue(_T(""))
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
}


BEGIN_MESSAGE_MAP(CameraFmtDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CameraFmtDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CameraFmtDialog::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_CAMERAFMT_COMBO, &CameraFmtDialog::OnCbnSelchangeCamerafmtCombo)
END_MESSAGE_MAP()


BOOL CameraFmtDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here
	if (CameraAllResution.empty())
	{
		m_CCameraFmt.AddString(_T("--------"));
	}
	else {
		for (int i = 0 ; i <CameraAllResution.size();i++ )
		{
			m_CCameraFmt.AddString((LPCTSTR)CameraAllResution[i]);
	    }
	}

	int index = m_CCameraFmt.SelectString(0, (LPCTSTR)m_sCurrentCameraResution);
	m_CCameraFmt.SetCurSel(index);

	m_SelectedCameraFmtValue = m_sCurrentCameraResution;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CameraFmtDialog::SetCameraResolution(std::vector<CAtlString>& CameraInfo, CAtlString  CurrentCameraInfo)
{
	CameraAllResution = CameraInfo;
	m_sCurrentCameraResution = CurrentCameraInfo;
}

void CameraFmtDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
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