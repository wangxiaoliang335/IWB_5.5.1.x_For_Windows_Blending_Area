#pragma once
#include "afxwin.h"


// CameraFmtDialog dialog

class CameraFmtDialog : public CDialog
{
	DECLARE_DYNAMIC(CameraFmtDialog)

public:
	CameraFmtDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CameraFmtDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERA_FMT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeCamerafmtCombo();

	void    SetCameraResolution(std::vector<CAtlString>& CameraInfo, CAtlString  CurrentCameraInfo);
	CAtlString  GetSelectComboxvalue();
	DECLARE_MESSAGE_MAP()

public:
	CComboBox  m_CCameraFmt;
	std::vector<CAtlString> CameraAllResution;
	CAtlString   m_sCurrentCameraResution;
	CString m_SelectedCameraFmtValue;
};
