// AdvancedSettingsDlg.cpp : implementation file
//
#include "stdafx.h"
//#include "headers.h"
#include "AdvancedSettingsDlg.h"
#include "AttachToScreenDlg.h"
//#include "MonitorSelectDlg.h"

// CAdvancedSettingsDlg dialog

IMPLEMENT_DYNAMIC(CAdvancedSettingsDlg, CScrollablePropertyPage)


//CAdvancedSettingsDlg::CAdvancedSettingsDlg(const TAdvancedSettings* pSettings, CWnd* pParent /*= NULL*/)
//CAdvancedSettingsDlg::CAdvancedSettingsDlg(const TSensorConfig& sensorConfig, const GlobalSettings& globalSettings, CWnd* pParent /*= NULL*/, LPCTSTR lpszAdditionalCaption)

CAdvancedSettingsDlg::CAdvancedSettingsDlg()
: CScrollablePropertyPage(CAdvancedSettingsDlg::IDD)
 ,m_bInitDone(FALSE)
{

}

CAdvancedSettingsDlg::~CAdvancedSettingsDlg()
{
}

void CAdvancedSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

	TSensorModeConfig* TSensorModeConfig = NULL;
	if (m_tGlobalSettings.eProjectionMode == E_PROJECTION_DESKTOP)
	{
		TSensorModeConfig = &m_tSensorConfig.vecSensorModeConfig[0];
	}
	else
	{
		TSensorModeConfig = &m_tSensorConfig.vecSensorModeConfig[1];
	}

    TLensConfig& lensCfg = TSensorModeConfig->lensConfigs[m_tSensorConfig.eSelectedLensType];

    DDX_Text(pDX, IDC_EDIT_SPOTPROPORTION, TSensorModeConfig->advanceSettings.nSpotProportion);
    DDX_Control(pDX, IDC_SPIN_SPOTPROPORTION, m_ctlSpotProportion);

    DDX_Control(pDX, IDC_SPIN_SET_AUTOCALIBRATION_BRIGHTNESS, m_ctlNormalUserBrightness);
    DDX_Text(pDX, IDC_EDIT_SET_AUTOCALIBRATE_AVERAGE_BRIGHTNESS, lensCfg.autoCalibrateSettings.calibrateImageParamsList[0].autoCalibrateExpectedBrightness);
    DDX_Control(pDX, IDC_SPIN_AUTOCALIBRATE_AVERAGE_BRIGHTNESS, m_ctlAutoCalibrationAveBrightness);
    DDX_Text(pDX, IDC_EDIT_AUTOCALIBRATE_HILIGHT_GRAY, lensCfg.autoCalibrateSettings.calibrateImageParamsList[0].autoCalibrateHilightGray);
    DDX_Control(pDX, IDC_SPINAUTOCALIBRATE_HILIGHT_GRAY, m_ctlAutoCalibrateHiLightGray);
    DDX_Radio(pDX, IDC_RADIO_PEN_TOUCH,  (int&)TSensorModeConfig->advanceSettings.m_eTouchType);

	DDX_Radio(pDX, IDC_WALLMODE,(int&)m_tGlobalSettings.eProjectionMode);

    //根据手触和笔触的不同设置不同的摄像头亮度参数。
    DDX_Text(pDX, IDC_EDIT_SET_NORMALUSAGE_BRIGHTNESS_COEFFICIENT,
        (TSensorModeConfig->advanceSettings.m_eTouchType == E_DEVICE_FINGER_TOUCH)?
        lensCfg.normalUsageSettings_FingerTouch.cameraParams.Prop_VideoProcAmp_Brightness
        :
        lensCfg.normalUsageSettings_PenTouch.cameraParams.Prop_VideoProcAmp_Brightness
        );

    DDX_Check(pDX, IDC_CHECK_RECORD_VIDEO, m_tGlobalSettings.bRecordVideo);
    DDX_Check(pDX, IDC_CHECK_REAR_PROJECTOR,TSensorModeConfig->advanceSettings.bIsRearProjection);

}


BEGIN_MESSAGE_MAP(CAdvancedSettingsDlg, CScrollablePropertyPage)
	ON_BN_CLICKED(IDOK, &CAdvancedSettingsDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_AUTO_MASK_INFLATE_RADIUS, &CAdvancedSettingsDlg::OnEnChangeEditAutoMaskInflateRadius)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_SETTINGS, &CAdvancedSettingsDlg::OnBnClickedButtonDefaultSettings)

	ON_EN_CHANGE(IDC_EDIT_SPOTPROPORTION, &CAdvancedSettingsDlg::OnEnChangeEditSpotProportion)
	ON_EN_CHANGE(IDC_EDIT_MULT_ERASER, &CAdvancedSettingsDlg::OnEnChangeEditMultEraser)
	ON_EN_CHANGE(IDC_EDIT_FIXED_BLOB_SET_TIME_VALUE, &CAdvancedSettingsDlg::OnEnChangeEditFixedBlobSetTimeValue)

	ON_BN_CLICKED(IDC_BUTTON_MANUAL_COLLECT_SPOT, &CAdvancedSettingsDlg::OnBnClickedButtonManualCollectSpot)
 //  ON_BN_CLICKED(IDC_CHECK_GESTURE_RECOGNITION, &CAdvancedSettingsDlg::OnBnClickedCheckGestureRecognition)

	ON_EN_CHANGE(IDC_EDIT_SET_NORMALUSAGE_BRIGHTNESS_COEFFICIENT, &CAdvancedSettingsDlg::OnEnChangeEditSetNormalUserBrightness)
	ON_BN_CLICKED(IDC_RADIO_PEN_TOUCH, &CAdvancedSettingsDlg::OnBnClickedRadioPenTouch)
	ON_BN_CLICKED(IDC_RADIO_FINGER_TOUCH, &CAdvancedSettingsDlg::OnBnClickedRadioFingerTouch)
	ON_EN_CHANGE(IDC_EDIT_AUTOMASKDETECTTHRESHOLD, &CAdvancedSettingsDlg::OnEnChangeEditAutomaskdetectthreshold)

	ON_BN_CLICKED(IDC_CHECK_RECORD_VIDEO, &CAdvancedSettingsDlg::OnBnClickedCheckRecordVideo)
	ON_EN_CHANGE(IDC_EDIT_SET_AUTOCALIBRATE_AVERAGE_BRIGHTNESS, &CAdvancedSettingsDlg::OnEnChangeEditSetAutocalibrateAverageBrightness)
	ON_EN_CHANGE(IDC_EDIT_AUTOCALIBRATE_HILIGHT_GRAY, &CAdvancedSettingsDlg::OnEnChangeEditAutocalibrateHilightGray)
	ON_BN_CLICKED(IDC_CHECK_REAR_PROJECTOR, &CAdvancedSettingsDlg::OnBnClickedCheckRearProjector)
    ON_CBN_SELCHANGE(IDC_COMBO_MONITOR_LIST, &CAdvancedSettingsDlg::OnCbnSelchangeComboMonitorList)
 
    ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_THROW_RATIO_015, IDC_RADIO_THROW_RATIO_134, &CAdvancedSettingsDlg::OnBnClickRadioLensType)

	ON_BN_CLICKED(IDC_WALLMODE, &CAdvancedSettingsDlg::OnBnClickedRadioWallMode)
	ON_BN_CLICKED(IDC_DESKTOPMODE, &CAdvancedSettingsDlg::OnBnClickedRadioDeskTopMode)
    
	ON_BN_CLICKED(IDC_BUTTON_ATTACH_TO_A_SCREEN, &CAdvancedSettingsDlg::OnBnClickedButtonAttachToAScreen)
END_MESSAGE_MAP()


// CAdvancedSettingsDlg message handlers

BOOL CAdvancedSettingsDlg::OnInitDialog()
{
	/*CDialog::OnInitDialog();*/
	/*
	if (m_strAdditionalCaption.GetLength() > 0)
	{
		CString strCaption;
		GetWindowText(strCaption);
		strCaption += _T("-");
		strCaption += m_strAdditionalCaption;

		SetWindowText(strCaption);

	}*/

	CScrollablePropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_ctlSpotProportion.SetRange(20, 80);
	m_ctlNormalUserBrightness.SetRange(-255,255);
	m_ctlAutoCalibrationAveBrightness.SetRange(-255,255) ;
	m_ctlAutoCalibrateHiLightGray.SetRange(-255,255);
	m_bInitDone = TRUE;


    //如果加密狗为笔触模式, 则禁用"笔触/手触"单选框
     if(theApp.GetUSBKeyTouchType() == E_DEVICE_PEN_TOUCH)
     {
        GetDlgItem(IDC_RADIO_FINGER_TOUCH)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_PEN_TOUCH   )->EnableWindow(FALSE);

        //光斑相应比例调节禁止。
        GetDlgItem(IDC_EDIT_SPOTPROPORTION)->EnableWindow(FALSE);
     }
     else
     {  
        //如果加密狗支持手触模式，则使能"笔触/手触"单选框
        GetDlgItem(IDC_RADIO_FINGER_TOUCH)->EnableWindow(TRUE);
        GetDlgItem(IDC_RADIO_PEN_TOUCH   )->EnableWindow(TRUE);


        //如果使用类型为笔触,则禁用"相应光斑大小比例输入框"和spin控件。
		TSensorModeConfig* TSensorModeConfig = NULL;
		if (m_tGlobalSettings.eProjectionMode == E_PROJECTION_DESKTOP)
		{
			TSensorModeConfig = &this->m_tSensorConfig.vecSensorModeConfig[0];
		}
		else 
		{
			TSensorModeConfig = &this->m_tSensorConfig.vecSensorModeConfig[1];
		}
        if(E_DEVICE_PEN_TOUCH == TSensorModeConfig->advanceSettings.m_eTouchType)
        {
            GetDlgItem(IDC_EDIT_SPOTPROPORTION)->EnableWindow(FALSE);
        }
     }

	 //////////////
	 if (E_PROJECTION_DESKTOP == m_tGlobalSettings.eProjectionMode)
	 {
		 GetDlgItem(IDC_DESKTOPMODE)->EnableWindow(TRUE);
	 }

     //只读信息, 双屏拼接
     GetDlgItem(IDC_CHECK_DOUBLE_SCREEN)->EnableWindow(FALSE);
     //CheckDlgButton(IDC_CHECK_DOUBLE_SCREEN, theApp.GetScreenType() == EDoubleScreenMode?BST_CHECKED:BST_UNCHECKED);
     CheckDlgButton(IDC_CHECK_DOUBLE_SCREEN, theApp.GetScreenMode() >= EScreenModeDouble ? BST_CHECKED : BST_UNCHECKED);

     //勾选实际选择的镜头类型
      CheckDlgButton(IDC_RADIO_THROW_RATIO_015 + int(this->m_tSensorConfig.eSelectedLensType), BST_CHECKED);
 

     //如果是双屏则显示"屏幕选择"按钮
     if(theApp.GetScreenMode() >= EScreenModeDouble)
     {
        GetDlgItem(IDC_BUTTON_ATTACH_TO_A_SCREEN)->ShowWindow(SW_SHOW);
     }
     else
     {//否则隐藏"屏幕选择"按钮
        GetDlgItem(IDC_BUTTON_ATTACH_TO_A_SCREEN)->ShowWindow(SW_HIDE);
     }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAdvancedSettingsDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	OnOK();
}

void CAdvancedSettingsDlg::OnEnChangeEditAutoMaskInflateRadius()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	CString strText;
	GetDlgItem(IDC_EDIT_AUTO_MASK_INFLATE_RADIUS)->GetWindowText(strText);

	int nDilateRadius = atoi(CT2A(strText));

	if(nDilateRadius > AUTO_MASK_AREA_INFLATE_RADIUS_MAX)
	{
		nDilateRadius = AUTO_MASK_AREA_INFLATE_RADIUS_MAX;

		strText.Format(_T("%d"), nDilateRadius);
		GetDlgItem(IDC_EDIT_AUTO_MASK_INFLATE_RADIUS)->SetWindowText(strText);
	}	
	else  if(nDilateRadius < AUTO_MASK_AREA_INFLATE_RADIUS_MIN)
	{
		nDilateRadius = AUTO_MASK_AREA_INFLATE_RADIUS_MIN;
		strText.Format(_T("%d"), nDilateRadius);
		GetDlgItem(IDC_EDIT_AUTO_MASK_INFLATE_RADIUS)->SetWindowText(strText);
	}	
    SetModified(TRUE);
}


void CAdvancedSettingsDlg::OnBnClickedButtonDefaultSettings()     //缺省值设置
{
	///////////////桌面墙面进行判断

	TSensorModeConfig* TSensorModeConfig = NULL;
	if (m_tGlobalSettings.eProjectionMode == E_PROJECTION_DESKTOP)
	{
		TSensorModeConfig = &this->m_tSensorConfig.vecSensorModeConfig[0];
	}
	else
	{
		TSensorModeConfig = &this->m_tSensorConfig.vecSensorModeConfig[1];
	}

    TLensConfig& lensCfg = TSensorModeConfig->lensConfigs[m_tSensorConfig.eSelectedLensType];

	TSensorModeConfig->advanceSettings.nSpotProportion = SPOT_DEFAULT_POS;


    //根据手触和笔触动态更改
    if(TSensorModeConfig->advanceSettings.m_eTouchType == E_DEVICE_PEN_TOUCH)
    {
        lensCfg.normalUsageSettings_PenTouch.cameraParams.Prop_VideoProcAmp_Brightness = lensCfg.normalUsageSettings_PenTouch.defaultParams.Prop_VideoProcAmp_Brightness;
    }
    else if(TSensorModeConfig->advanceSettings.m_eTouchType == E_DEVICE_FINGER_TOUCH)
    {
        lensCfg.normalUsageSettings_FingerTouch.cameraParams.Prop_VideoProcAmp_Brightness = lensCfg.normalUsageSettings_FingerTouch.defaultParams.Prop_VideoProcAmp_Brightness;
    }

    lensCfg.autoMaskSettings.cAutoMaskDetectThreshold =  lensCfg.autoMaskSettings.cAutoMaskDetectThresholdDefault;

	//m_nAutoCalibrationAveBrightness = AUTOCALIBRATEAVERAGEBRIGHTNESS_DEFAULT;
    //lensCfg.autoCalibrateSettings.calibrateImageParamsList[0].autoCalibrateExpectedBrightness = AUTOCALIBRATEAVERAGEBRIGHTNESS_DEFAULT;
    lensCfg.autoCalibrateSettings.calibrateImageParamsList[0].autoCalibrateExpectedBrightness =  lensCfg.autoCalibrateSettings.defaultCalibrateImageParamsList[0].autoCalibrateExpectedBrightness;
    
	//m_nAutoCalibrateHiLightGray =         AUTOCALIBRATELIGHTGRAY;
    //lensCfg.autoCalibrateSettings.calibrateImageParamsList[0].autoCalibrateHilightGray  = AUTOCALIBRATELIGHTGRAY;
    lensCfg.autoCalibrateSettings.calibrateImageParamsList[0].autoCalibrateHilightGray  = lensCfg.autoCalibrateSettings.defaultCalibrateImageParamsList[0].autoCalibrateHilightGray;

    if(TSensorModeConfig->advanceSettings.m_eTouchType == E_DEVICE_PEN_TOUCH)
    {
        if(lensCfg.autoMaskSettings.cAutoMaskDetectThreshold > lensCfg.normalUsageSettings_PenTouch.cYThreshold)
        {
            //m_nAutoMaskDetectThreshold = m_nThreshold;   //如果默认的值大于最高能设置的值的话。就置最大值为默认的值。
            lensCfg.autoMaskSettings.cAutoMaskDetectThreshold = lensCfg.normalUsageSettings_PenTouch.cYThreshold;
        }
    }
    else if(TSensorModeConfig->advanceSettings.m_eTouchType == E_DEVICE_FINGER_TOUCH)
    {
        if(lensCfg.autoMaskSettings.cAutoMaskDetectThreshold > lensCfg.normalUsageSettings_FingerTouch.cYThreshold)
        {
            //m_nAutoMaskDetectThreshold = m_nThreshold;   //如果默认的值大于最高能设置的值的话。就置最大值为默认的值。
            lensCfg.autoMaskSettings.cAutoMaskDetectThreshold = lensCfg.normalUsageSettings_FingerTouch.cYThreshold;
        }
    }
	
    //缺省不启用录像
    this->m_tGlobalSettings.bRecordVideo = FALSE;

    //缺省不使用背投模式
	TSensorModeConfig->advanceSettings.bIsRearProjection = FALSE;

    //更新数据到控件中去
	UpdateData(FALSE);

    SetModified(TRUE);
}

void CAdvancedSettingsDlg::OnEnChangeEditSpotProportion()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (!m_bInitDone)
	{
		return;
	}
	CString strText;
	GetDlgItem(IDC_EDIT_SPOTPROPORTION)->GetWindowText(strText);
	int npos = _ttoi(strText);
	int bForceValidate = FALSE;

	if (npos < SPOT_MIN)
	{
		npos = SPOT_MIN;
		bForceValidate = TRUE;
	}

	if(npos > SPOT_MAX)
	{
		npos = SPOT_MAX;
		bForceValidate = TRUE;
	}

	if(bForceValidate)
	{
		strText.Format(_T("%d"),npos);
		GetDlgItem(IDC_EDIT_SPOTPROPORTION)->SetWindowText(strText);
	}
	else
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CHANGE_SPOTPROPORTION,(WPARAM)0,(LPARAM)npos);
	}
    SetModified(TRUE);
}

void CAdvancedSettingsDlg::OnEnChangeEditMultEraser()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	if (!m_bInitDone)
	{
		return;
	}
	CString strText;
	GetDlgItem(IDC_EDIT_MULT_ERASER)->GetWindowText(strText);

	int npos = _ttoi(strText);
	int bForceValidate = FALSE;

	if (npos < MULT_ERASER_MIN)
	{
		npos = MULT_ERASER_MIN;
		bForceValidate = TRUE;
	}

	if (npos > MULT_ERASER_MAX)
	{
		npos = MULT_ERASER_MAX;
		bForceValidate = TRUE;
	}

	if (bForceValidate)
	{
		strText.Format(_T("%d"),npos);
		GetDlgItem(IDC_EDIT_MULT_ERASER)->SetWindowText(strText);
	}
	else
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_CHANGE_MULTERASER, (WPARAM)0, (LPARAM)npos);
	}
    SetModified(TRUE);
}


void CAdvancedSettingsDlg::OnBnClickedButtonManualCollectSpot()
{
	// TODO: Add your control notification handler code here
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_MANUAL_COLLECT_SPOT,0,0);
}

void CAdvancedSettingsDlg::OnEnChangeEditFixedBlobSetTimeValue()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	if(!m_bInitDone) return ;
	CString strText;
	GetDlgItem(IDC_EDIT_FIXED_BLOB_SET_TIME_VALUE)->GetWindowText(strText);
	int npos = _ttoi(strText);
    int bForceValidate = FALSE;

	if (npos < FIXED_BLOB_SET_TIME_MIN)
	{
		npos = FIXED_BLOB_SET_TIME_MIN;
		bForceValidate = TRUE         ;
	}

	if (npos > FIXED_BLOB_SET_TIME_MAX)
	{
		npos = FIXED_BLOB_SET_TIME_MAX;
		bForceValidate = TRUE ;
	}

	if (bForceValidate)
	{
		strText.Format(_T("%d"),npos);
		GetDlgItem(IDC_EDIT_FIXED_BLOB_SET_TIME_VALUE)->SetWindowText(strText);
	}
	else
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CHANGE_FIXEDBOLBSETTIME,(WPARAM)0,(LPARAM)npos);
	}

    SetModified(TRUE);
}

void CAdvancedSettingsDlg::OnEnChangeEditSetNormalUserBrightness()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	if (!m_bInitDone)
	{
		return;
	}
	CString strText;
	GetDlgItem(IDC_EDIT_SET_NORMALUSAGE_BRIGHTNESS_COEFFICIENT)->GetWindowText(strText);

	int npos = _ttoi(strText);
	int bForceValidate = FALSE;

	if (npos < AUTOCALIBRATION_BRIGHTNESS_MIX)
	{
		npos = AUTOCALIBRATION_BRIGHTNESS_MIX;
		bForceValidate = TRUE;
	}
	if (npos > AUTOCALIBRATION_BRIGHTNESS_MAX)
	{
		npos = AUTOCALIBRATION_BRIGHTNESS_MAX;
		bForceValidate = TRUE;
	}

	if (bForceValidate)
	{
		strText.Format(_T("%d"),npos);
		GetDlgItem(IDC_EDIT_SET_NORMALUSAGE_BRIGHTNESS_COEFFICIENT)->SetWindowText(strText);
	}
	else
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CHANGE_NORMALUSER_BRIGHTNESS,(WPARAM)0, (LPARAM)npos);
	}
    SetModified(TRUE);
}

void CAdvancedSettingsDlg::OnBnClickedRadioFingerTouch()
{
	// TODO: Add your control notification handler code here

	 TSensorModeConfig* TSensorModeConfig = NULL;
	 if (m_tGlobalSettings.eProjectionMode == E_PROJECTION_DESKTOP)
	 {
		TSensorModeConfig = &m_tSensorConfig.vecSensorModeConfig[0];
	 }
	 else {
		TSensorModeConfig = &m_tSensorConfig.vecSensorModeConfig[1];
	 }

     TLensConfig& lensCfg = TSensorModeConfig->lensConfigs[m_tSensorConfig.eSelectedLensType];

	 if (IsDlgButtonChecked(IDC_RADIO_FINGER_TOUCH) == BST_CHECKED)
	 {
		 GetDlgItem(IDC_EDIT_SPOTPROPORTION)->EnableWindow(TRUE);

		 //更新正常使用时的亮度系数
		 CString strBrightnessCoefficient;//亮度系数
		 strBrightnessCoefficient.Format(_T("%d"), lensCfg.normalUsageSettings_FingerTouch.cameraParams.Prop_VideoProcAmp_Brightness);
		 GetDlgItem(IDC_EDIT_SET_NORMALUSAGE_BRIGHTNESS_COEFFICIENT)->SetWindowText(strBrightnessCoefficient);

		 //更新到变量中去
		 UpdateData(TRUE);
		 SetModified(TRUE);
	 }
}

void CAdvancedSettingsDlg::OnBnClickedRadioWallMode()
{
	if (IsDlgButtonChecked(IDC_WALLMODE) == BST_CHECKED)
	{
		m_tGlobalSettings.eProjectionMode = E_PROJECTION_WALL;

		CString strText;
		strText.Format(_T("%d"), m_tSensorConfig.vecSensorModeConfig[1].advanceSettings.nSpotProportion);
		GetDlgItem(IDC_EDIT_SPOTPROPORTION)->SetWindowText(strText);

		//更新正常使用时的亮度系数
		TLensConfig& lensCfg = m_tSensorConfig.vecSensorModeConfig[1].lensConfigs[m_tSensorConfig.eSelectedLensType];

		CString strBrightnessCoefficient;//亮度系数

		if (m_tSensorConfig.vecSensorModeConfig[1].advanceSettings.m_eTouchType == E_DEVICE_FINGER_TOUCH) {
		     strBrightnessCoefficient.Format(_T("%d"), lensCfg.normalUsageSettings_FingerTouch.cameraParams.Prop_VideoProcAmp_Brightness);
		}
		else {
			strBrightnessCoefficient.Format(_T("%d"), lensCfg.normalUsageSettings_PenTouch.cameraParams.Prop_VideoProcAmp_Brightness);			
		}
		GetDlgItem(IDC_EDIT_SET_NORMALUSAGE_BRIGHTNESS_COEFFICIENT)->SetWindowText(strBrightnessCoefficient);

		//更新触控方式
		if (m_tSensorConfig.vecSensorModeConfig[1].advanceSettings.m_eTouchType == E_DEVICE_PEN_TOUCH)
		{
			((CButton*)GetDlgItem(IDC_RADIO_PEN_TOUCH))->SetCheck(true);
			((CButton*)GetDlgItem(IDC_RADIO_FINGER_TOUCH))->SetCheck(false);
		}
		else {
			((CButton*)GetDlgItem(IDC_RADIO_PEN_TOUCH))->SetCheck(false);
			((CButton*)GetDlgItem(IDC_RADIO_FINGER_TOUCH))->SetCheck(true);
		}

		CString  autoCalibrateExpectedBrightness;
		autoCalibrateExpectedBrightness.Format(_T("%d"), lensCfg.autoCalibrateSettings.calibrateImageParamsList[0].autoCalibrateExpectedBrightness);
		GetDlgItem(IDC_EDIT_SET_AUTOCALIBRATE_AVERAGE_BRIGHTNESS)->SetWindowText(autoCalibrateExpectedBrightness);

		CString autoCalibrateHilightGray;
		autoCalibrateHilightGray.Format(_T("%d"), lensCfg.autoCalibrateSettings.calibrateImageParamsList[0].autoCalibrateHilightGray);
		GetDlgItem(IDC_SPINAUTOCALIBRATE_HILIGHT_GRAY)->SetWindowText(autoCalibrateHilightGray);


		//更新到变量中去
		UpdateData(TRUE);
		SetModified(TRUE);
	}
}
void CAdvancedSettingsDlg::OnBnClickedRadioDeskTopMode()
{
	if (IsDlgButtonChecked(IDC_DESKTOPMODE) == BST_CHECKED)
	{
		m_tGlobalSettings.eProjectionMode = E_PROJECTION_DESKTOP;

		CString strText;
		strText.Format(_T("%d"), m_tSensorConfig.vecSensorModeConfig[0].advanceSettings.nSpotProportion);
		GetDlgItem(IDC_EDIT_SPOTPROPORTION)->SetWindowText(strText);

		//更新正常使用时的亮度系数
		TLensConfig& lensCfg = m_tSensorConfig.vecSensorModeConfig[0].lensConfigs[m_tSensorConfig.eSelectedLensType];

		CString strBrightnessCoefficient;//亮度系数
		if (m_tSensorConfig.vecSensorModeConfig[0].advanceSettings.m_eTouchType == E_DEVICE_FINGER_TOUCH) {
			strBrightnessCoefficient.Format(_T("%d"), lensCfg.normalUsageSettings_FingerTouch.cameraParams.Prop_VideoProcAmp_Brightness);
		}
		else {
			strBrightnessCoefficient.Format(_T("%d"), lensCfg.normalUsageSettings_PenTouch.cameraParams.Prop_VideoProcAmp_Brightness);
		}
		GetDlgItem(IDC_EDIT_SET_NORMALUSAGE_BRIGHTNESS_COEFFICIENT)->SetWindowText(strBrightnessCoefficient);
		/////更新触控方式
		if (m_tSensorConfig.vecSensorModeConfig[0].advanceSettings.m_eTouchType == E_DEVICE_PEN_TOUCH)
		{
			((CButton*)GetDlgItem(IDC_RADIO_PEN_TOUCH))->SetCheck(true);
			((CButton*)GetDlgItem(IDC_RADIO_FINGER_TOUCH))->SetCheck(false);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_RADIO_PEN_TOUCH))->SetCheck(false);
			((CButton*)GetDlgItem(IDC_RADIO_FINGER_TOUCH))->SetCheck(true);
		}

		CString  autoCalibrateExpectedBrightness;
		autoCalibrateExpectedBrightness.Format(_T("%d"), lensCfg.autoCalibrateSettings.calibrateImageParamsList[0].autoCalibrateExpectedBrightness);
		GetDlgItem(IDC_EDIT_SET_AUTOCALIBRATE_AVERAGE_BRIGHTNESS)->SetWindowText(autoCalibrateExpectedBrightness);

		CString autoCalibrateHilightGray;
		autoCalibrateHilightGray.Format(_T("%d"), lensCfg.autoCalibrateSettings.calibrateImageParamsList[0].autoCalibrateHilightGray);
		GetDlgItem(IDC_SPINAUTOCALIBRATE_HILIGHT_GRAY)->SetWindowText(autoCalibrateHilightGray);

		//更新到变量中去
		UpdateData(TRUE);
		SetModified(TRUE);
	}
}

void CAdvancedSettingsDlg::OnBnClickedRadioPenTouch()
{
	// TODO: Add your control notification handler code here
	//////判断是墙面还是桌面
	TSensorModeConfig* TSensorModeConfig = NULL;
	if (m_tGlobalSettings.eProjectionMode == E_PROJECTION_DESKTOP)
	{
		TSensorModeConfig = &m_tSensorConfig.vecSensorModeConfig[0];
	}
	else {
		TSensorModeConfig = &m_tSensorConfig.vecSensorModeConfig[1];
	}

    TLensConfig& lensCfg = TSensorModeConfig->lensConfigs[m_tSensorConfig.eSelectedLensType];
	if (IsDlgButtonChecked(IDC_RADIO_PEN_TOUCH) == BST_CHECKED)
	{
        //笔触模式下,不能够设置光斑比例
		GetDlgItem(IDC_EDIT_SPOTPROPORTION)->EnableWindow(FALSE);
		
        //GetDlgItem(IDC_EDIT_MULT_ERASER)->EnableWindow(FALSE);


        //更新正常使用时的亮度系数
        CString strBrightnessCoefficient;//亮度系数
        strBrightnessCoefficient.Format(_T("%d"), lensCfg.normalUsageSettings_PenTouch.cameraParams.Prop_VideoProcAmp_Brightness);
        GetDlgItem(IDC_EDIT_SET_NORMALUSAGE_BRIGHTNESS_COEFFICIENT)->SetWindowText(strBrightnessCoefficient);


        //更新到变量中去
		UpdateData(TRUE) ;
        SetModified(TRUE);
	}
}

void CAdvancedSettingsDlg::OnEnChangeEditAutomaskdetectthreshold()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (!m_bInitDone)
	{
		return;
	}
	TSensorModeConfig* TSensorModeConfig = NULL;
	if (m_tGlobalSettings.eProjectionMode == E_PROJECTION_DESKTOP)
	{
		TSensorModeConfig = & m_tSensorConfig.vecSensorModeConfig[0];
	}
	else 
	{
		TSensorModeConfig = & m_tSensorConfig.vecSensorModeConfig[1];
	}

    TLensConfig& lensCfg = TSensorModeConfig->lensConfigs[m_tSensorConfig.eSelectedLensType];

	CString StrText;

	GetDlgItem(IDC_EDIT_AUTOMASKDETECTTHRESHOLD)->GetWindowText(StrText);

	int npos = _ttoi(StrText);
	BOOL  bForceValidate = FALSE;

	if (npos < AUTOMASKDETECTTHRESHOLD_MIX)
	{
		npos = AUTOMASKDETECTTHRESHOLD_MIX;
		bForceValidate = TRUE;
	}
	//if (npos > m_nThreshold)

    if(TSensorModeConfig->advanceSettings.m_eTouchType == E_DEVICE_PEN_TOUCH)
    {
        if (npos > lensCfg.normalUsageSettings_PenTouch.cYThreshold)
        {
            npos = lensCfg.normalUsageSettings_PenTouch.cYThreshold;
            bForceValidate = TRUE;
        }
    }
    else if(TSensorModeConfig->advanceSettings.m_eTouchType == E_DEVICE_FINGER_TOUCH)
    {
        if (npos > lensCfg.normalUsageSettings_FingerTouch.cYThreshold)
        {
            npos = lensCfg.normalUsageSettings_FingerTouch.cYThreshold;
            bForceValidate = TRUE;
        }
    }

	if (bForceValidate)
	{
		StrText.Format(_T("%d"),npos);
		GetDlgItem(IDC_EDIT_AUTOMASKDETECTTHRESHOLD)->SetWindowText(StrText);
	}
	else
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CHANGE_AUTOMASKDETECTTHRESHOLD,(WPARAM)0,(LPARAM)npos);

	}
    SetModified(TRUE);
}

void CAdvancedSettingsDlg::OnBnClickedCheckRecordVideo()
{
	// TODO: Add your control notification handler code here
	if (IsDlgButtonChecked(IDC_CHECK_RECORD_VIDEO) == BST_CHECKED)
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_RECORD_VIDEO,(WPARAM)0, (LPARAM)TRUE);
	}
	else
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_RECORD_VIDEO,(WPARAM)0, (LPARAM)FALSE);
	}
    SetModified(TRUE);
}


void CAdvancedSettingsDlg::OnEnChangeEditSetAutocalibrateAverageBrightness()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (!m_bInitDone)
	{
		return;
	}
	CString strText;
	GetDlgItem(IDC_EDIT_SET_AUTOCALIBRATE_AVERAGE_BRIGHTNESS)->GetWindowText(strText);
	int npos = _ttoi(strText);
	int bForceValidate = FALSE;

	if (npos < AUTOCALIBRATION_BRIGHTNESS_MIX)
	{
		npos = AUTOCALIBRATION_BRIGHTNESS_MIX;
		bForceValidate = TRUE;
	}
	if (npos > AUTOCALIBRATION_BRIGHTNESS_MAX)
	{
		npos = AUTOCALIBRATION_BRIGHTNESS_MAX;
		bForceValidate = TRUE;
	}

	if ((bForceValidate))
	{
		strText.Format(_T("%d"),npos);
		GetDlgItem(IDC_EDIT_SET_AUTOCALIBRATE_AVERAGE_BRIGHTNESS)->GetWindowText(strText);
	}
	else
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CHANGE_AUTOCALIBRIATION_AVERAGE_BRIGHTNESS,(WPARAM)0,(LPARAM)npos);
	}

    SetModified(TRUE);
}

void CAdvancedSettingsDlg::OnEnChangeEditAutocalibrateHilightGray()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	if (!m_bInitDone)
	{
		return ;
	}

	CString strText;
	GetDlgItem(IDC_EDIT_AUTOCALIBRATE_HILIGHT_GRAY)->GetWindowText(strText);

	int npos = _ttoi(strText);
	int bForceValidate = FALSE ;

	if (npos < AUTOCALIBRATION_BRIGHTNESS_MIX)
	{
		npos = AUTOCALIBRATION_BRIGHTNESS_MIX ;
		bForceValidate = TRUE ;
	}
	if (npos > AUTOCALIBRATION_BRIGHTNESS_MAX)
	{
		npos = AUTOCALIBRATION_BRIGHTNESS_MAX ;
		bForceValidate = TRUE ;
	}

	if (bForceValidate)
	{
		strText.Format(_T("%d"), npos) ;
		GetDlgItem(IDC_EDIT_AUTOCALIBRATE_HILIGHT_GRAY)->GetWindowText(strText) ;
	}
	else
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd() , WM_CHANGE_AUTOCALIBRIATION_LIGHTGRAY , (WPARAM)0 ,(LPARAM)npos) ;
	}
    SetModified(TRUE);
}

void CAdvancedSettingsDlg::OnBnClickedCheckRearProjector()
{
	// TODO: Add your control notification handler code here
	if (IsDlgButtonChecked(IDC_CHECK_REAR_PROJECTOR) ==BST_CHECKED)
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_REAR_PROJECTION ,(WPARAM)0,(LPARAM)TRUE);
	}
	else
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_REAR_PROJECTION ,(WPARAM)0,(LPARAM)FALSE);
	}
	
    SetModified(TRUE);
}

void CAdvancedSettingsDlg::OnCbnSelchangeComboMonitorList()
{
    // TODO: Add your control notification handler code here
}


const TSensorConfig&   CAdvancedSettingsDlg::GetSensorConfig()const
{
    return this->m_tSensorConfig;
}

const GlobalSettings& CAdvancedSettingsDlg::GetGlobalSettings()const
{
    return this->m_tGlobalSettings;
}


//void CAdvancedSettingsDlg::OnBnClickedScreenSelect()
//{
//    // TODO: Add your control notification handler code here
//    const int WAIT_TIME = 1000;//单位:毫秒
//    
//    //在各个屏幕上分别显示编号
//    this->m_oScreenRecognition.DoRecoginition();
//
//    ESensorLensMode OldMode = m_pSensor->GetLensMode();
//
//    //进入图像调试模式
//    m_pSensor->SwitchLensMode(E_VIDEO_TUNING_MODE);
//    
//    m_pSensor->GetPenPosDetector()->StartImageCapture(WAIT_TIME);
//
//    
//    DWORD dwStart = GetTickCount();
//    BOOL bCaptureSuccess = FALSE;
//    do
//    {
//        Sleep(WAIT_TIME);
//
//        BOOL bCaptureStatus = m_pSensor->GetPenPosDetector()->IsImageCaputredDone();
//
//        if(!bCaptureStatus)
//        {
//            DWORD dwNow = GetTickCount();
//            DWORD dwDelta = dwNow - dwStart;
//
//            if(dwDelta > 3*WAIT_TIME)
//            {
//                break;
//            }
//
//        }
//        else
//        {
//            bCaptureSuccess = TRUE;
//            break;
//        }
//
//        //MSG msg;
//        //GetMessage(&msg, NULL, 0, 0);
//        //TranslateMessage(&msg);
//        //DispatchMessage(&msg);
//
//    }while(1);
//
//
//     //进入原来的镜头模式
//     m_pSensor->SwitchLensMode(OldMode);
//
//    if(bCaptureSuccess)
//    {
//        //CMonitorSelectDlg monitorSelectDlg(m_pSensor->GetPenPosDetector()->GetCapturedFrame(), this);
//
//        //if(monitorSelectDlg.DoModal() == IDOK)
//        //{
//        //    
//
//        //}
//
//    }
//    
//    
//
//    //结束各个屏幕上的编号显示
//    this->m_oScreenRecognition.StopRecognition();
//
//}

//void CAdvancedSettingsDlg::OnBnClickedRadioFar()
//{
//    // TODO: Add your control notification handler code here
//    if (IsDlgButtonChecked(IDC_RADIO_FAR) == BST_CHECKED)
//    {        
//
//        //将界面输入数据保存到变量中
//        UpdateData(TRUE);
//
//
//        if( E_LENS_TR_1_DOT_34 != m_tSensorConfig.eSelectedLensType )
//        {
//
//            //变更镜头类型
//            m_tSensorConfig.eSelectedLensType  = E_LENS_TR_1_DOT_34;
//            
//            //更新控件显示数据
//            UpdateData(FALSE);
//
//
//            //使能应用按钮
//            SetModified(TRUE);
//        }
//
//    }
//}
//
//void CAdvancedSettingsDlg::OnBnClickedRadioNear()
//{
//    // TODO: Add your control notification handler code here
//    if (IsDlgButtonChecked(IDC_RADIO_NEAR) == BST_CHECKED)
//    {        
//
//        //将界面输入数据保存到变量中
//        UpdateData(TRUE);
//
//        if(E_LENS_TR_0_DOT_34 != m_tSensorConfig.eSelectedLensType)
//        {
//            //变更镜头类型
//            m_tSensorConfig.eSelectedLensType  = E_LENS_TR_0_DOT_34;
//
//           //更新控件显示数据
//            UpdateData(FALSE);
//           
//            //使能应用按钮
//            SetModified(TRUE);
//        }
//    }
//}
//
//void CAdvancedSettingsDlg::OnBnClickedRadioSupernear()
//{
//    // TODO: Add your control notification handler code here
//    if (IsDlgButtonChecked(IDC_RADIO_SUPERNEAR) == BST_CHECKED)
//    {
//        //将界面输入数据保存到变量中
//        UpdateData(TRUE);
//
//        if(E_LENS_TR_0_DOT_21 != m_tSensorConfig.eSelectedLensType)
//        {
//            //变更镜头类型
//            m_tSensorConfig.eSelectedLensType  = E_LENS_TR_0_DOT_21;
//
//            //更新控件显示数据
//            UpdateData(FALSE);
//
//            //使能应用按钮
//            SetModified(TRUE);
//        }
//    }
//}
//
//
//void CAdvancedSettingsDlg::OnBnClickedRadioSupernear019()
//{
//    if (IsDlgButtonChecked(IDC_RADIO_SUPERNEAR_0_19) == BST_CHECKED)
//    {
//        //将界面输入数据保存到变量中
//        UpdateData(TRUE);
//
//        if(E_LENS_TR_0_DOT_15 != m_tSensorConfig.eSelectedLensType)
//        {
//            //变更镜头类型
//            m_tSensorConfig.eSelectedLensType  = E_LENS_TR_0_DOT_15;
//
//            //更新控件显示数据
//            UpdateData(FALSE);
//
//            //使能应用按钮
//            SetModified(TRUE);
//        }
//    }
//
//}

void CAdvancedSettingsDlg::OnBnClickRadioLensType(UINT uID)
{
    if (IsDlgButtonChecked(uID) == BST_CHECKED)
    {
        ELensType lensType = (ELensType)(uID - IDC_RADIO_THROW_RATIO_015);

        if(lensType != m_tSensorConfig.eSelectedLensType)
        {
            //变更镜头类型
            m_tSensorConfig.eSelectedLensType  = lensType;

            ////更新控件显示数据
            UpdateData(FALSE);

            ////使能应用按钮
            SetModified(TRUE);
        }
    }

}

void CAdvancedSettingsDlg::OnBnClickedButtonAttachToAScreen()
{
	// TODO: Add your control notification handler code here

	CAttachToScreenDlg attachToScreenDlg(
		m_tSensorConfig.bAutoAttachMonitor,
		m_tGlobalSettings.bDoubleScreenTouchMerge,
		m_tSensorConfig.nAttachedMonitorId,
		m_tSensorConfig.eMonitorAreaType,

		this);


	if (IDOK == attachToScreenDlg.DoModal())
	{
		m_tSensorConfig.bAutoAttachMonitor = attachToScreenDlg.IsAutoAttachMode();
		m_tSensorConfig.nAttachedMonitorId = attachToScreenDlg.GetMonitorId();
		m_tSensorConfig.eMonitorAreaType = attachToScreenDlg.GetMonitorAreaType();
		m_tGlobalSettings.bDoubleScreenTouchMerge = attachToScreenDlg.IsDoubleScreenTouchMerge();

		//使能应用按钮
		SetModified(TRUE);
	}//if
}


BOOL CAdvancedSettingsDlg::OnApply()
{
	UpdateData(TRUE);
	CPropertyPage::OnApply();
	SetModified(FALSE);


	//通知主窗体应用设置
	::SendMessage(
		AfxGetMainWnd()->GetSafeHwnd(),
		WM_APPLY_SENSOR_CONFIG,
		(WPARAM)this,
		(LPARAM)m_pSensor);

	//

	return TRUE;
}
