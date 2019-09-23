#pragma once
#include "afxdlgs.h"
#include "AdvancedSettingsDlg.h"
#include "GestureSettingDlg.h"
#include "CBPropertySheet.h"
#include "DlgItemResizer.h"

class CParamsSettingPropertySheet :
    public CCBPropertySheet
{
    DECLARE_DYNAMIC(CParamsSettingPropertySheet)
protected:
    BOOL OnInitDialog();

    afx_msg void OnApply();
    afx_msg void OnOK();
    afx_msg void OnCancel();

    afx_msg void OnDestroy();

    DECLARE_MESSAGE_MAP()

public:
    CParamsSettingPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
    CParamsSettingPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
    ~CParamsSettingPropertySheet(void);  

public:
    void SetIWBSensorInfo(CIWBSensor* pSensor, int nSenorCount = 1, bool isDoubleScreen = false);
    void SetGlobalSettingInfo(const GlobalSettings& globalSettings);

    const TSensorConfig&   GetSensorConfig()const;
    const GlobalSettings& GetGlobalSettings()const;

protected:
	//计算每个标签页面相对于PropertySheet左上角的矩形可视区
    BOOL CalcPageRect(RECT *pRect);

	//计算SysTabControl32空间相对于PropertySheet左上角的矩形位置
	BOOL CalcTabCtrlRect(RECT* pRect);
    CDlgItemResizer m_Resizer;


	//SysTabControl32四边与对话框四边之间的距离
	int m_nTabControlMarginLeft  ;
	int m_nTabControlMarginRight ;
	int m_nTabControlMarginTop   ;
	int m_nTabControlMarginBottom;

public:
    CAdvancedSettingsDlg        m_advSettingPage;
    CGestureSettingDlg          m_gesSettingPage;

    afx_msg void OnSize(UINT nType, int cx, int cy);


	virtual INT_PTR DoModal();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};
