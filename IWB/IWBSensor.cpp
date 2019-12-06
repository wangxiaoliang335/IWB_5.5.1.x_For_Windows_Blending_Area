#include "stdafx.h"

#include "../inc/Log.h"
#include "..\MorphologyAlgo\inc\MorphologyAlgo.h"

//#pragma comment(lib, "..\lib\MorphologyAlgo.lib")
//#include "headers.h"

//静态对象
//CAutoCalibrator CIWBSensor::m_oAutoCalibrator;
//CDispMonitorFinder CIWBSensor::m_oDispMonitorFinder;


//光斑采集对象
//CCollectSpotSize   CIWBSensor::m_wndLightSpotSampling;

//@功能:静态屏蔽开始回调处理函数。
//@参数:lpCtx, 回调的上下文数据。    
BOOL  CALLBACK CIWBSensor::OnPreStaticMasking(LPVOID lpCtx)
{
    CIWBSensor* lpSensor = reinterpret_cast<CIWBSensor*>(lpCtx);

    //合上滤光片。
    IRCUTSwtich(lpSensor->m_oVideoPlayer.GetCaptureFilter(), FALSE, lpSensor->m_tDeviceInfo.m_nPID, lpSensor->m_tDeviceInfo.m_nVID);

    //载入正常使用时的相机参数
    EDeviceTouchType eTouchType;
    TSensorModeConfig* TSensorModeConfig = NULL;

	EProjectionMode eProjectionMode = g_tSysCfgData.globalSettings.eProjectionMode;
    TSensorModeConfig = &lpSensor->m_tCfgData.vecSensorModeConfig[eProjectionMode];

    eTouchType = TSensorModeConfig->advanceSettings.m_eTouchType;
    TLensConfig lensCfg = TSensorModeConfig->lensConfigs[lpSensor->m_eCameraType][lpSensor->m_tCfgData.eSelectedLensType];
	switch(eTouchType)
	{
		case E_DEVICE_PEN_TOUCH_WHITEBOARD:

			lpSensor->m_oVideoPlayer.SetCameraParams(lensCfg.normalUsageSettings_PenTouchWhiteBoard.cameraParams);
			break;
		case E_DEVICE_FINGER_TOUCH_WHITEBOARD:
			lpSensor->m_oVideoPlayer.SetCameraParams(lensCfg.normalUsageSettings_FingerTouchWhiteBoard.cameraParams);
			break;
		case E_DEVICE_FINGER_TOUCH_CONTROL:
			lpSensor->m_oVideoPlayer.SetCameraParams(lensCfg.normalUsageSettings_FingerTouchControl.cameraParams);
			break;
		case E_DEVICE_PALM_TOUCH_CONTROL:
			lpSensor->m_oVideoPlayer.SetCameraParams(lensCfg.normalUsageSettings_PalmTouchControl.cameraParams);
			break;
		default:
			break;

	}

	return TRUE;
}


//静态函数
//自动校正过程中变更摄像头参数的回调函数
BOOL  CIWBSensor::OnAutoCalibChangeCameraParams(EChangeCalibCameraParams eCtrlMode, LPVOID lpCtx, BYTE param1, BYTE param2)
{

    CIWBSensor* lpThis = reinterpret_cast<CIWBSensor*>(lpCtx);

    TSensorModeConfig* TSensorModeConfig = NULL;
	EProjectionMode eProjectionMode  = g_tSysCfgData.globalSettings.eProjectionMode;

    TSensorModeConfig = &lpThis->m_tCfgData.vecSensorModeConfig[eProjectionMode];

    const TLensConfig& lensCfg = TSensorModeConfig->lensConfigs[lpThis->m_eCameraType][lpThis->m_tCfgData.eSelectedLensType];

    TVideoProcAmpProperty cameraParams;

    switch (eCtrlMode)
    {
      case E_CAMERA_SHARPNESS_MAX://设置最大锐利度
          lpThis->m_oVideoPlayer.GetCameraParams(cameraParams);

           IRCUTSwtich(lpThis->m_oVideoPlayer.GetCaptureFilter(), FALSE, lpThis->m_tDeviceInfo.m_nPID, lpThis->m_tDeviceInfo.m_nVID);

        //        cameraParams.Prop_VideoProcMap_Gain  = IRCUT_OFF;//保持滤光片打开
        return lpThis->m_oVideoPlayer.SetCameraParams(cameraParams);

        break;


    case E_CAMERA_SHARPNESS_MIN://显示校正图案阶段
        //PROFILE::CameraParametersForAutoCalibration(cameraParams);

        cameraParams = lensCfg.autoCalibrateSettingsList[param2].cameraParams;

		IRCUTSwtich(lpThis->m_oVideoPlayer.GetCaptureFilter(), FALSE, lpThis->m_tDeviceInfo.m_nPID, lpThis->m_tDeviceInfo.m_nVID);

        return lpThis->m_oVideoPlayer.SetCameraParams(cameraParams);
        break;

    case E_CAMERA_BRIGHTNESS_INCREASE://增加亮度
    {
        LONG lBrightness, lMax, lMin, lSteppingDelta, lDefault;
        BYTE absDiff = param1;

        //按照亮度差的绝对值大小,按彼列调节
        BYTE K = 2;//比例调节系数Kp = 1/K
        if (!lpThis->m_oVideoPlayer.GetCameraBrightness(lBrightness, lMax, lMin, lSteppingDelta, lDefault))
        {
            return FALSE;
        }
        if (lSteppingDelta < absDiff / K)
        {
            lSteppingDelta = absDiff / K;
        }

        if (lBrightness > lMax - lSteppingDelta)
        {
            lBrightness = lMax;
        }
        else
        {
            lBrightness += lSteppingDelta;
        }

        //lpThis->m_oFilterGraphBuilder.GetCameraParams(cameraParams);
        /*
        lpThis->m_oVideoPlayer.GetCameraParams(cameraParams);
        cameraParams.Prop_VideoProcAmp_Brightness = lBrightness;
        cameraParams.Prop_VideoProcMap_Gain       = IRCUT_OFF  ;//保持滤光片打开
        */

        //AtlTrace(_T("[Brightness]delta =%d, coef=%d, increase\n"), lSteppingDelta, lBrightness);
        return lpThis->m_oVideoPlayer.SetCameraParams(lBrightness, VideoProcAmp_Brightness);
    }


    break;

    case E_CAMERA_BRIGHTNESS_DECREASE://减少亮度
    {
        LONG lBrightness, lMax, lMin, lSteppingDelta, lDefault;
        BYTE absDiff = param1;

        //按照亮度差的绝对值大小,按彼列调节
        BYTE K = 2;//比例调节系数Kp = 1/K
        if (!lpThis->m_oVideoPlayer.GetCameraBrightness(lBrightness, lMax, lMin, lSteppingDelta, lDefault))
        {
            return FALSE;
        }


        if (lSteppingDelta < absDiff / K)
        {
            lSteppingDelta = absDiff / K;
        }

        if (lBrightness < lSteppingDelta)
        {
            lBrightness = lMin;
        }
        else
        {
            lBrightness -= lSteppingDelta;
        }


        //AtlTrace(_T("[Brightness]delta =%d, coef=%d,decrease\n"), lSteppingDelta, lBrightness);

        /*
        lpThis->m_oVideoPlayer.GetCameraParams(cameraParams);
        cameraParams.Prop_VideoProcAmp_Brightness = lBrightness;
        cameraParams.Prop_VideoProcMap_Gain       = IRCUT_OFF  ;//保持滤光片打开
        */


        return lpThis->m_oVideoPlayer.SetCameraParams(lBrightness, VideoProcAmp_Brightness);
    }
    break;


    case E_CAMERA_CONTRAST_MAX:
    {
        if (!lpThis->m_oVideoPlayer.GetCameraParams(cameraParams)) return FALSE;

        LONG lValue, lMax, lMin, lSteppingDelta, lDefault;
        lMax = 255;
        lpThis->m_oVideoPlayer.GetCameraContrast(lValue, lMax, lMin, lSteppingDelta, lDefault);
        cameraParams.Prop_VideoProcAmp_Contrast = lMax;
        return lpThis->m_oVideoPlayer.SetCameraParams(cameraParams);
    }
    break;


    case E_CAMERA_CONTRAST_DEFAULT:
    {
        if (!lpThis->m_oVideoPlayer.GetCameraParams(cameraParams))return FALSE;

        LONG lValue, lMax, lMin, lSteppingDelta, lDefault;
        if (!lpThis->m_oVideoPlayer.GetCameraContrast(lValue, lMax, lMin, lSteppingDelta, lDefault))
        {
            return FALSE;
        }
        cameraParams.Prop_VideoProcAmp_Contrast = lDefault;
        return lpThis->m_oVideoPlayer.SetCameraParams(cameraParams);
    }

    case E_CAMERA_CONTRAST:
    {
        if (!lpThis->m_oVideoPlayer.GetCameraParams(cameraParams)) return FALSE;
        cameraParams.Prop_VideoProcAmp_Contrast = param1;
        return lpThis->m_oVideoPlayer.SetCameraParams(cameraParams);
    }


    case E_CAMERA_AUTO_CALIBRATE:

        cameraParams = lensCfg.autoCalibrateSettingsList[param2].cameraParams;

        IRCUTSwtich(lpThis->m_oVideoPlayer.GetCaptureFilter(), FALSE, lpThis->m_tDeviceInfo.m_nPID, lpThis->m_tDeviceInfo.m_nVID);

        //        cameraParams.Prop_VideoProcMap_Gain     = IRCUT_OFF  ;//保持滤光片打开
        return lpThis->m_oVideoPlayer.SetCameraParams(cameraParams);
        break;

    case E_CAMERA_AUTO_MASK:

        EDeviceTouchType eTouchType;
        eTouchType = TSensorModeConfig->advanceSettings.m_eTouchType;
		switch (eTouchType)
		{
		   case E_DEVICE_PEN_TOUCH_WHITEBOARD:

			  cameraParams = lensCfg.normalUsageSettings_PenTouchWhiteBoard.cameraParams;
			  break;
		   case E_DEVICE_FINGER_TOUCH_WHITEBOARD:

			   cameraParams = lensCfg.normalUsageSettings_FingerTouchWhiteBoard.cameraParams;
			   break;
		   case E_DEVICE_FINGER_TOUCH_CONTROL:
			   cameraParams = lensCfg.normalUsageSettings_FingerTouchControl.cameraParams;
			   break;
		   case E_DEVICE_PALM_TOUCH_CONTROL:
			   cameraParams = lensCfg.normalUsageSettings_PalmTouchControl.cameraParams;
			   break;
		   default:
			   break;
		}

//        if(eTouchType == E_DEVICE_PEN_TOUCH_WHITEBOARD)
//        {
//            cameraParams = lensCfg.normalUsageSettings_PenTouchWhiteBoard.cameraParams;
//        }
//        else
//        {
//            cameraParams = lensCfg.normalUsageSettings_FingerTouchWhiteBoard.cameraParams;
//        }

		//合上滤光片 
		IRCUTSwtich(lpThis->m_oVideoPlayer.GetCaptureFilter(), TRUE, lpThis->m_tDeviceInfo.m_nPID, lpThis->m_tDeviceInfo.m_nVID);

        //        cameraParams.Prop_VideoProcMap_Gain     = IRCUT_ON ;//拉上滤光片
        return lpThis->m_oVideoPlayer.SetCameraParams(cameraParams);

        break;


    }//switch

    return TRUE;
}


CIWBSensor::CIWBSensor(int nID)
	:
	m_tFavoriteMediaType(DEFAULT_VIDEO_FORMAT),
	m_nID(nID),
	m_oPenPosDetector(nID),
	m_oVideoPlayer(nID),
	m_oTimerActionExecuter(*this),//,
	m_eCameraType(E_CAMERA_MODEL_0)
    //m_hVideoDispWnd(NULL),
    //m_hNotifyWnd(NULL)
{
    //memset(&m_rcVideoDispArea, 0, sizeof(RECT));

    //m_pInterceptFilter = new CInterceptFilter(&this->m_oPenPosDetector, &m_oVideoPlayer);
    m_pInterceptFilter = new CInterceptFilter(this);
    m_pInterceptFilter->AddRef();

    this->m_oVideoPlayer.SetInterceptFilter(m_pInterceptFilter);

    m_tDeviceInfo.m_nPID = 0x9186;
    m_tDeviceInfo.m_nVID = 0x18ec;
    m_tDeviceInfo.m_strDevPath = "";
    m_tDeviceInfo.m_strName = "";

    m_rcMonintorArea.left   = 0;
    m_rcMonintorArea.top    = 0;
    m_rcMonintorArea.right  = ::GetSystemMetrics(SM_CXSCREEN);
    m_rcMonintorArea.bottom = ::GetSystemMetrics(SM_CYSCREEN);

}


CIWBSensor::~CIWBSensor()
{
    m_oVideoPlayer.StopDetect();
    this->m_oVideoPlayer.SetInterceptFilter(NULL);
    m_pInterceptFilter->Release();
}

void CIWBSensor::SetFavoriateMediaType(const VideoMediaType& vmt)
{
    m_tFavoriteMediaType = vmt;
}


//@功能:设置视频显显示相关信息
//@参数:hDispWnd, 视频播放窗体的句柄
//      rcDispArea, 视频绘制的区域  
//      hNotifyWnd, 接收消息接收窗体句柄
void CIWBSensor::SetVideoDispParams(HWND hDispWnd, RECT& rcDispArea, HWND hNotifyWnd)
{
    //m_hVideoDispWnd   = hDispWnd;
    //m_rcVideoDispArea = rcDispArea;
    //m_hNotifyWnd      = hNotifyWnd;

    m_oVideoPlayer.SetDisplayWnd(hDispWnd);
    m_oVideoPlayer.SetDisplayArea(rcDispArea);
    m_oVideoPlayer.SetNotifyWnd(hNotifyWnd);


}


//@功能:运行传感器
//@参数:
BOOL CIWBSensor::Run()
{
    //<<added by toxuke@gmail.com,2014/12/14
    //如果设备路径为空则不播放。
    if (m_tDeviceInfo.m_strDevPath.IsEmpty()) return FALSE;
    //>>

    BOOL bRet = m_oVideoPlayer.StartDetect(m_tDeviceInfo.m_strDevPath, &m_tFavoriteMediaType.videoInfoHeader);

    //<<added by toxuke@gmail.com, 2014/12/12    
    //说明:为何延迟500ms?
    //测试发现，不延迟500ms，则SwitchLensMode函数中设置相机参数虽然返回TRUE但实际并未起作用。
    //延时500ms
    Sleep(500);
    //end>


    //来回推拉IRCut一次确保IRCut正常工作
    //拉开滤光片禁用光笔控制，避免光斑乱跳
    BOOL bIsPenControlling = this->m_oPenPosDetector.IsOpticalPenControlling();

    if (bIsPenControlling)
    {//
        m_oPenPosDetector.EnableOpticalPenControl(FALSE);//
    }

    //拉开虑光片
    IRCUTSwtich(m_oVideoPlayer.GetCaptureFilter(), FALSE, m_tDeviceInfo.m_nPID, m_tDeviceInfo.m_nVID);

    //等待300ms
	//1280*720sleep(300)的时候会出现设置参数卡死的情况出现
    Sleep(300);

    //合上滤光片 
    IRCUTSwtich(m_oVideoPlayer.GetCaptureFilter(), TRUE, m_tDeviceInfo.m_nPID, m_tDeviceInfo.m_nVID);

    //等待300ms
    Sleep(300);

    //使能光笔。
    m_oPenPosDetector.EnableOpticalPenControl(bIsPenControlling);

    RECT rcMonitor;
    BOOL bGet = this->GetAttachedScreenArea(rcMonitor);
    if (bGet)
    {
        m_oPenPosDetector.SetAttachedMonitorSize(rcMonitor);
    }

    //切换到已设置的镜头模式
    SwitchLensMode(this->m_eLensMode);

    return bRet;
}

BOOL CIWBSensor::Stop()
{
    return m_oVideoPlayer.StopDetect();
}

void CIWBSensor::ShowStatusInfo()
{
    m_oVideoPlayer.MissStatusInfo();
}
//@功能:进入指定的工作模式
//@参数:eMode, 模式枚举值
void CIWBSensor::SwitchLensMode(ESensorLensMode eMode)
{
    ////////////判断是墙面模式还是桌面模式
    TSensorModeConfig* TSensorModeConfig = NULL;
	EProjectionMode eProjectionMode = g_tSysCfgData.globalSettings.eProjectionMode;
    TSensorModeConfig = &m_tCfgData.vecSensorModeConfig[eProjectionMode];

    BOOL bRet = FALSE;
    const TLensConfig& lensCfg = TSensorModeConfig->lensConfigs[this->m_eCameraType][m_tCfgData.eSelectedLensType];

    switch (eMode)
    {
    case E_VIDEO_TUNING_MODE:

        //禁用光笔
        //g_oMouseEventGen.EnableOpticalPenControl(FALSE);
        EnableOpticalPen(FALSE);

        //打开滤光片
        IRCUTSwtich(m_oVideoPlayer.GetCaptureFilter(), FALSE, m_tDeviceInfo.m_nPID, m_tDeviceInfo.m_nVID);

        //载入安装调试时的相机参数
        bRet = m_oVideoPlayer.SetCameraParams(lensCfg.installTunningSettings.cameraParams);
        if (!bRet)
        {
            AtlTrace(_T("Set CameraParam Failed!\n"));
        }

        {
            SIZE videoSize;
            bRet = m_oVideoPlayer.GetVideoSize(videoSize);

            if (bRet && videoSize.cx > 0 && videoSize.cy > 0)
            {
                //TOSDText::RectF rcText;
                RectF rcText;
                rcText.left = (float)lensCfg.rcGuideRectangle.left / (float)videoSize.cx;
                rcText.top = (float)lensCfg.rcGuideRectangle.left / (float)videoSize.cy;

                rcText.right = (float)lensCfg.rcGuideRectangle.left / (float)videoSize.cx;
                rcText.bottom = (float)lensCfg.rcGuideRectangle.left / (float)videoSize.cy;

                m_oVideoPlayer.AddOSDText(
                    E_OSDTEXT_TYPE_GUIDE_BOX,
                    g_oResStr[IDS_STRING465],
                    rcText,
                    DT_BOTTOM | DT_CENTER | DT_SINGLELINE,
                    8,
                    _T("Times New Roman"),
                    -1);
            }
        }

        //禁用"静态屏蔽"操作
        m_oPenPosDetector.EnableStaticMasking(FALSE);

        //停止"动态屏蔽"操作
        m_oPenPosDetector.EnableDynamicMasking(FALSE);

		//停止“抗干扰”操作
		m_oPenPosDetector.EnableAntiJamming(FALSE);

        //不显示光斑外接矩形
        m_oPenPosDetector.ShowSpotRect(FALSE);

        //不显示光斑尺寸信息
        m_oPenPosDetector.ShowSpotSizeInfo(FALSE);

        //使能画面亮度自动调节
        this->m_pInterceptFilter->EnableBrightnessAutoRegulating(TRUE);
        //
        break;

    case E_LASER_TUNING_MODE:
        //载入激光调试时的相机参数
		switch (GetActualTouchType())
		{
		   case E_DEVICE_FINGER_TOUCH_CONTROL:
			   m_oVideoPlayer.SetCameraParams(lensCfg.laserTunningSettings_FingerTouchControl.cameraParams);
			    break;
		  case E_DEVICE_PALM_TOUCH_CONTROL:
			   m_oVideoPlayer.SetCameraParams(lensCfg.laserTunningSettings_PalmTouchControl.cameraParams);
			   break;
		  default:
              m_oVideoPlayer.SetCameraParams(lensCfg.laserTunningSettings_WhiteBoard.cameraParams);
			  break;
		}

		m_oVideoPlayer.ClearOSDText(E_OSDTEXT_TYPE_GUIDE_BOX);
        //使能光笔
        //g_oMouseEventGen.EnableOpticalPenControl(FALSE);
        EnableOpticalPen(FALSE);

        //合上滤光片
        IRCUTSwtich(m_oVideoPlayer.GetCaptureFilter(), TRUE, m_tDeviceInfo.m_nPID, m_tDeviceInfo.m_nVID);

        //开启"静态屏蔽"操作
        m_oPenPosDetector.EnableStaticMasking(FALSE);

        //停止动态屏蔽操作
        m_oPenPosDetector.EnableDynamicMasking(FALSE);

		//停止“抗干扰”操作
		m_oPenPosDetector.EnableAntiJamming(FALSE);

        //显示光斑外接矩形
        m_oPenPosDetector.ShowSpotRect(TRUE);

        //不显示光斑尺寸信息
        m_oPenPosDetector.ShowSpotSizeInfo(TRUE);


        //禁用画面亮度自动调节
        this->m_pInterceptFilter->EnableBrightnessAutoRegulating(FALSE);

        break;

    case E_NORMAL_USAGE_MODE:
        //载入正常使用时的相机参数

        EDeviceTouchType eTouchType;
        eTouchType = TSensorModeConfig->advanceSettings.m_eTouchType;

		switch (eTouchType)
		{
		   case E_DEVICE_PEN_TOUCH_WHITEBOARD:

			   m_oVideoPlayer.SetCameraParams(lensCfg.normalUsageSettings_PenTouchWhiteBoard.cameraParams);
			   break;
		   case E_DEVICE_FINGER_TOUCH_WHITEBOARD:

			   m_oVideoPlayer.SetCameraParams(lensCfg.normalUsageSettings_FingerTouchWhiteBoard.cameraParams);
			   break;
		   case E_DEVICE_FINGER_TOUCH_CONTROL:

			   m_oVideoPlayer.SetCameraParams(lensCfg.normalUsageSettings_FingerTouchControl.cameraParams);
			   break;
		   case E_DEVICE_PALM_TOUCH_CONTROL:
			   m_oVideoPlayer.SetCameraParams(lensCfg.normalUsageSettings_PalmTouchControl.cameraParams);
			   break;
		   default:
			   break;
		}
//      if(eTouchType == E_DEVICE_PEN_TOUCH_WHITEBOARD)
//      {
//          m_oVideoPlayer.SetCameraParams(lensCfg.normalUsageSettings_PenTouchWhiteBoard.cameraParams);
//      }
//      else
//      {
//          m_oVideoPlayer.SetCameraParams(lensCfg.normalUsageSettings_FingerTouchWhiteBoard.cameraParams);
//      }

		m_oVideoPlayer.ClearOSDText(E_OSDTEXT_TYPE_GUIDE_BOX);
        //合上滤光片
        IRCUTSwtich(m_oVideoPlayer.GetCaptureFilter(), TRUE, m_tDeviceInfo.m_nPID, m_tDeviceInfo.m_nVID);
        //使能光笔
        //g_oMouseEventGen.EnableOpticalPenControl(TRUE);
        EnableOpticalPen(TRUE);

        //开启"静态屏蔽"操作
        m_oPenPosDetector.EnableStaticMasking(TRUE);

		///如果是正常模式的话那么就根据设置来判断是否需要开启动态屏蔽
		/////////MODIFY BY  VERA_ZHAO 2019.10.24
		if (TSensorModeConfig->advanceSettings.bIsDynamicMaskFrame)
		{
			//开启动态屏蔽操作
			m_oPenPosDetector.EnableDynamicMasking(TRUE);
		}
		else
		{
            //停止动态屏蔽操作
            m_oPenPosDetector.EnableDynamicMasking(FALSE);
		}

        //显示光斑外接矩形
        m_oPenPosDetector.ShowSpotRect(TRUE);

        //不显示光斑尺寸信息
        m_oPenPosDetector.ShowSpotSizeInfo(TRUE);

        this->m_pInterceptFilter->SetFrameSkipCount(10);
        //禁用画面亮度自动调节
        this->m_pInterceptFilter->EnableBrightnessAutoRegulating(FALSE);

        break;
    }

    m_eLensMode = eMode;
}

//@功能:设置视频捕获设备信息
void CIWBSensor::SetDeviceInfo(const TCaptureDeviceInstance& devInfo)
{
    m_tDeviceInfo = devInfo;

    /////如果是高清摄像头的话，如果设置中不是1080*720或者1920*1080的话就默认为1080*720
    if (m_tDeviceInfo.m_nPID == SONIX_PID && m_tDeviceInfo.m_nVID == SONIX_VID)
    {
        if (m_tCfgData.strFavoriteMediaType != "1280 X 720 MJPG" && m_tCfgData.strFavoriteMediaType != "1920 X 1080 MJPG")
        {
            m_tCfgData.strFavoriteMediaType = "1280 X 720 MJPG";
        }
    }

    //选取最合适的视频格式
    for (size_t i = 0; i < m_tDeviceInfo.m_vecVideoFmt.size(); i++)
    {
        CAtlString strFormatName = GetVideoFormatName(m_tDeviceInfo.m_vecVideoFmt[i]);

        if (strFormatName == m_tCfgData.strFavoriteMediaType)
        {
            m_tFavoriteMediaType = m_tDeviceInfo.m_vecVideoFmt[i];
            break;
        }
    }

    m_tCfgData.strFavoriteDevicePath = m_tDeviceInfo.m_strDevPath;
    m_tCfgData.strFavoriteMediaType = GetVideoFormatName(m_tFavoriteMediaType);


	m_eCameraType = ::GetCameraType(m_tDeviceInfo.m_nPID, m_tDeviceInfo.m_nVID);

}

//@功能:返回视频捕获设备信息
const TCaptureDeviceInstance& CIWBSensor::GetDeviceInfo()const
{
    return m_tDeviceInfo;
}

//@功能:载入配置数据
//@参数:cfgData, 图像传感器的配置信息
//      pGlobalSettings, 输入参数, 指向全局配置信息的指针
void CIWBSensor::SetCfgData(const TSensorConfig& cfgData, const GlobalSettings* pGlobalSettings)
{
	m_tCfgData = cfgData;

	m_tDeviceInfo.m_strDevPath = cfgData.strFavoriteDevicePath;

	int nPID = 0, nVID = 0;
	int ret = _stscanf_s(m_tDeviceInfo.m_strDevPath, _T("\\\\?\\usb#vid_%04x&pid_%04x"), &nVID, &nPID);
	if (2 == ret)
	{
		m_tDeviceInfo.m_nPID = nPID;
		m_tDeviceInfo.m_nVID = nVID;

		m_eCameraType = ::GetCameraType(m_tDeviceInfo.m_nPID, m_tDeviceInfo.m_nVID);
	}

	//选取最合适的视频格式
	for (size_t i = 0; i < m_tDeviceInfo.m_vecVideoFmt.size(); i++)
	{
		CAtlString strFormatName = GetVideoFormatName(m_tDeviceInfo.m_vecVideoFmt[i]);

		if (strFormatName == m_tCfgData.strFavoriteMediaType)
		{
			m_tFavoriteMediaType = m_tDeviceInfo.m_vecVideoFmt[i];
			break;
		}
	}
	SetGlobalCfgData( pGlobalSettings);

}

void CIWBSensor::SetGlobalCfgData(const GlobalSettings* pGlobalSettings)
{
    TSensorModeConfig* TSensorModeConfig = NULL;
	EProjectionMode eProjectionMode = g_tSysCfgData.globalSettings.eProjectionMode;

    TSensorModeConfig = &m_tCfgData.vecSensorModeConfig[eProjectionMode];

    const TLensConfig& lensCfg = TSensorModeConfig->lensConfigs[this->m_eCameraType][m_tCfgData.eSelectedLensType];
    //设置画面自动调节时的平均亮度 == 自动校正时的第一组画面的平均亮度
    this->m_pInterceptFilter->SetImageAverageBrightness(lensCfg.autoCalibrateSettingsList[0].calibrateImageParams.autoCalibrateExpectedBrightness);

    const NormalUsageSettings* pNormalUsageSettings = NULL;
    //全局配置信息
    if (pGlobalSettings)
    {
		switch (theApp.GetUSBKeyTouchType())
		{
		   case E_DEVICE_PEN_TOUCH_WHITEBOARD:

			   //加密狗为笔触模式, 强制工作模式为笔触模式
			   m_oPenPosDetector.SetTouchType(E_DEVICE_PEN_TOUCH_WHITEBOARD);
			   pNormalUsageSettings = &lensCfg.normalUsageSettings_PenTouchWhiteBoard;

			    break;
		   case E_DEVICE_FINGER_TOUCH_WHITEBOARD:

			   //加密狗为手触模式, 选用用户选择的触控模式
			   m_oPenPosDetector.SetTouchType(TSensorModeConfig->advanceSettings.m_eTouchType);

			   if (TSensorModeConfig->advanceSettings.m_eTouchType == E_DEVICE_PEN_TOUCH_WHITEBOARD)
			   {
				   pNormalUsageSettings = &lensCfg.normalUsageSettings_PenTouchWhiteBoard;
			   }
			   else
			   {
				   pNormalUsageSettings = &lensCfg.normalUsageSettings_FingerTouchWhiteBoard;
			   }
			   break;
		   case E_DEVICE_FINGER_TOUCH_CONTROL:

			   //加密狗为手指触控模式, 强制工作模式为手指触控模式
			   m_oPenPosDetector.SetTouchType(E_DEVICE_FINGER_TOUCH_CONTROL);
			   pNormalUsageSettings = &lensCfg.normalUsageSettings_FingerTouchControl;

			    break;
		   case E_DEVICE_PALM_TOUCH_CONTROL:

			   //加密狗为手掌互动模式, 强制工作模式为手掌互动模式
			   m_oPenPosDetector.SetTouchType(E_DEVICE_PALM_TOUCH_CONTROL);
			   pNormalUsageSettings = &lensCfg.normalUsageSettings_FingerTouchControl;
			    break;
		   default:
			   break;

		}

        //设置光斑检测门限
        m_oPenPosDetector.SetYThreshold(pNormalUsageSettings->cYThreshold);

        //设置动态屏蔽门限
        int nPercenttage = pNormalUsageSettings->nDynamicMaskThresholdPercentage;
        BYTE cDynamicMaskThreshold = BYTE(int(pNormalUsageSettings->cYThreshold) * nPercenttage / 100);
        m_oPenPosDetector.SetDynamicMaskThreshold(cDynamicMaskThreshold);


        //设置最小光斑尺寸
        SIZE minimumLightSpotSize;
        minimumLightSpotSize.cx = pNormalUsageSettings->nLightSpotMinimumWidth;
        minimumLightSpotSize.cy = pNormalUsageSettings->nLightSpotMinimumHeight;
        m_oPenPosDetector.SetMinimumLightSpotSize(minimumLightSpotSize);

        m_oPenPosDetector.SetGuideRectangle(
            lensCfg.rcGuideRectangle, lensCfg.dwGuideRectangleColor);

        m_oPenPosDetector.ShowGuideRectangle(lensCfg.bRectangleVisible);

        //设置镜头焦距类型
        //m_oPenPosDetector.SetLensFocalType(pGlobalSettings->eLensFocalType);
        m_oPenPosDetector.SetLensFocalType(m_tCfgData.eSelectedLensType);

        //设置是否是背投模式
        m_oPenPosDetector.GetVideoToScreenMap().SetRearProjectMode(TSensorModeConfig->advanceSettings.bIsRearProjection);

        //设置"自动校正补偿系数"
        m_oPenPosDetector.GetVideoToScreenMap().GetCalibAlog().SetAutoCalibCompCoefs(lensCfg.autoCalibCompCoefs);

        //设置CMOS芯片规格数据
        if (pGlobalSettings)
        {
            m_oPenPosDetector.GetVideoToScreenMap().GetCalibAlog().SetCMOSChipSpecification(pGlobalSettings->CMOSChipSpecification);
        }

        //设置镜头规格数据
        m_oPenPosDetector.GetVideoToScreenMap().GetCalibAlog().SetLensSpecification(lensCfg.lensSpecification);

        //设置镜头的内部初始参数
        m_oPenPosDetector.GetVideoToScreenMap().GetCalibAlog().SetLensInternalAndSymmmetricDistortParams(
            lensCfg.bInternalAndSymmetricDistortParamsIsValid ?
            &lensCfg.lensInternalAndSymmetricDistortParams :
            NULL
        );
        //设置校正参数
        m_oPenPosDetector.GetVideoToScreenMap().SetCalibParams(TSensorModeConfig->calibParam);

    }

    //设置Sensor实际关联的屏幕区域
    RECT rcArea;
    BOOL bRet = GetAttachedScreenArea(rcArea);
    if (bRet)
    {
        this->m_oPenPosDetector.GetVideoToScreenMap().SetActualMonitorResolution(rcArea);
    }


    //根据工作模式立即生效摄像头参数
    switch (m_eLensMode)
    {
    case E_VIDEO_TUNING_MODE:
        m_oVideoPlayer.SetCameraParams(lensCfg.installTunningSettings.cameraParams);
        break;

    case E_LASER_TUNING_MODE:

		switch (GetActualTouchType())
		{
		case E_DEVICE_FINGER_TOUCH_CONTROL:
			m_oVideoPlayer.SetCameraParams(lensCfg.laserTunningSettings_FingerTouchControl.cameraParams);
			break;
		case E_DEVICE_PALM_TOUCH_CONTROL:
			m_oVideoPlayer.SetCameraParams(lensCfg.laserTunningSettings_PalmTouchControl.cameraParams);
			break;
		default:
			m_oVideoPlayer.SetCameraParams(lensCfg.laserTunningSettings_WhiteBoard.cameraParams);
			break;
		}
        break;

    case E_NORMAL_USAGE_MODE:
        //m_oVideoPlayer.SetCameraParams(this->m_tCfgData.normalUsageSettings.cameraParams);
        m_oVideoPlayer.SetCameraParams(pNormalUsageSettings->cameraParams);
        break;
    }

}

//@功能:获取配置数据
//@参数:cfgData, 图像传感器的配置信息
const TSensorConfig& CIWBSensor::GetCfgData()const
{
    return m_tCfgData;
}

//@功能:判断传感器是否正在工作
BOOL CIWBSensor::IsDetecting()
{
    return this->m_oVideoPlayer.IsDetecting();

}


//@功能:使能光笔
//@参数:bEnable,使能标志
void CIWBSensor::EnableOpticalPen(BOOL bEnable)
{
    //this->m_oPenPosDetector.GetMouseEventGenerator().EnableOpticalPenControl(bEnable);
    this->m_oPenPosDetector.EnableOpticalPenControl(bEnable);
}

//@功能:判断光笔是否在控制中
//@说明:只要有一支光笔在控制中,则返回TRUE
BOOL CIWBSensor::IsOpticalPenControlling()
{
    //return this->m_oPenPosDetector.GetMouseEventGenerator().IsOpticalPenControlling();
    return this->m_oPenPosDetector.IsOpticalPenControlling();
}

//@功能:开始图像传感器的自动校正流程
//@参数:ePattern, 校正图案模式
//      hNotifyWnd, 校正结束后的同志消息的接收窗体
void  CIWBSensor::StartAutoCalibrate(E_AutoCalibratePattern ePattern, HWND hNotifyWnd)
{
    if (!this->IsDetecting()) return;//未在运行则立即退出

    TSensorModeConfig* TSensorModeConfig = NULL;
	EProjectionMode eProjectionMode = g_tSysCfgData.globalSettings.eProjectionMode;
    TSensorModeConfig = &m_tCfgData.vecSensorModeConfig[eProjectionMode];

    const TLensConfig& lensCfg = TSensorModeConfig->lensConfigs[this->m_eCameraType][m_tCfgData.eSelectedLensType];

    //设置自动校正时的参数
	//刚开始摄像头参数是第一个参数
    m_oVideoPlayer.SetCameraParams(lensCfg.autoCalibrateSettingsList[0].cameraParams);


    //<<added by xuke, 2016/11/12
    //禁用画面亮度自动调节
    this->m_pInterceptFilter->EnableBrightnessAutoRegulating(FALSE);
    //>>


    //打开滤光片
    IRCUTSwtich(m_oVideoPlayer.GetCaptureFilter(), FALSE, m_tDeviceInfo.m_nPID, m_tDeviceInfo.m_nVID);


    //停止"静态屏蔽"操作
    m_oPenPosDetector.EnableStaticMasking(FALSE);

    //停止"动态屏蔽"操作
    m_oPenPosDetector.EnableDynamicMasking(FALSE);

	//停止“抗干扰”操作
	m_oPenPosDetector.EnableAntiJamming(FALSE);

    //不显示光斑外接矩形
    m_oPenPosDetector.ShowSpotRect(FALSE);

    //不显示光斑尺寸信息
    m_oPenPosDetector.ShowSpotSizeInfo(FALSE);


    TAutoCalibrateParams autoCalibrateParams;

    autoCalibrateParams.hNotifyWnd = hNotifyWnd;
    autoCalibrateParams.ePattern = ePattern;
    //autoCalibrateParams.cAvgBrightness         = m_tCfgData.autoCalibrateSettings.autoCalibrateExpectedBrightness;
    autoCalibrateParams.ChangeCameraParamsProc = OnAutoCalibChangeCameraParams;
    autoCalibrateParams.lpCtx = (LPVOID)this;
    autoCalibrateParams.eDebugLevel = (ECalibDebugLevel)g_tSysCfgData.globalSettings.nDebugLevel;

    //高亮块对应的灰度值
    //BYTE  cHilightGray = m_tCfgData.autoCalibrateSettings.autoCalibrateHilightGray;
    //autoCalibrateParams.clrGridHighlight       = RGB(cHilightGray,cHilightGray,cHilightGray);
	///delete by vera_zhao
//    autoCalibrateParams.imageParamsList        = lensCfg.autoCalibrateSettings.calibrateImageParamsList;

    autoCalibrateParams.bSaveInermediatFile    = g_tSysCfgData.globalSettings.bSaveIntermediateFile;
    autoCalibrateParams.bRecordVideo           = g_tSysCfgData.globalSettings.bRecordVideo;



	//在线手动设置的屏蔽区域
     autoCalibrateParams.bEnableOnlineScreenArea = this->m_oPenPosDetector.IsEnableOnlineScreenArea();
    std::vector<CPoint> vecOnlineVertices;
    this->m_oPenPosDetector.GetCurrentOnLineScreenAreaPt(vecOnlineVertices);
    int nOnlineVerticesCount = vecOnlineVertices.size();

    autoCalibrateParams.vecOnlineAreaVertices.resize(nOnlineVerticesCount);
    for (int i = 0; i < nOnlineVerticesCount; i++)
    {
        autoCalibrateParams.vecOnlineAreaVertices[i].x = vecOnlineVertices[i].x;
        autoCalibrateParams.vecOnlineAreaVertices[i].y = vecOnlineVertices[i].y;

    }
    
	autoCalibrateParams.autocalibrateparamslist.clear();
	int nCount = lensCfg.autoCalibrateSettingsList.size();


	for (int i = 0 ; i < nCount; i++ )
	{
		AutoCalibrateParams  params;
		params.autoCalibrateImageParams  = lensCfg.autoCalibrateSettingsList[i].calibrateImageParams;
		params.videoprocampproperty = lensCfg.autoCalibrateSettingsList[i].cameraParams;

		autoCalibrateParams.autocalibrateparamslist.push_back(params);
	}



    TScreenInfo tScreenInfo;
    BOOL bRet = this->GetAttachedScreenArea(tScreenInfo.rcArea);

    if (!bRet) return;
    autoCalibrateParams.vecScreenInfos.push_back(tScreenInfo);

    //准备静态屏蔽参数
    TStaticMaskingParams staticMaskingParams;
    staticMaskingParams.fpPreStaticMaskingProc = CIWBSensor::OnPreStaticMasking;
    staticMaskingParams.lpPreStaticMaskingCtx = this;
    staticMaskingParams.cStaticMaskThreshold = lensCfg.autoMaskSettings.cAutoMaskDetectThreshold;
    staticMaskingParams.nMaskEroseSize = lensCfg.autoMaskSettings.nMaskAreaEroseSize;

    m_oAutoCalibrator.StartCalibrating(autoCalibrateParams, staticMaskingParams);
     

    BOOL bRecordVideoTemp;

    bRecordVideoTemp = g_tSysCfgData.globalSettings.bRecordVideo;


    if (bRecordVideoTemp)
    {
        this->m_pInterceptFilter->StartRecording(m_oAutoCalibrator.GetDebugVideoFilePath());
    }

    m_oPenPosDetector.EnterCalibrateMode(m_oAutoCalibrator.GetCalibrateHWnd(), CALIBRATE_MODE_AUTO);

}


//@功能:开始自动屏蔽
//@参数:hNotifyWnd, 校正结束后的同志消息的接收窗体
void  CIWBSensor::StartAutoMasking(HWND hNotifyWnd)
{
    if (!this->IsDetecting()) return;//未在运行则立即退出

    TSensorModeConfig* TSensorModeConfig = NULL;

	EProjectionMode eProjectionMode = g_tSysCfgData.globalSettings.eProjectionMode;
    TSensorModeConfig = &m_tCfgData.vecSensorModeConfig[eProjectionMode];

    const TLensConfig& lensCfg = TSensorModeConfig->lensConfigs[this->m_eCameraType][m_tCfgData.eSelectedLensType];

    //设置自动校正时的参数
    //m_oVideoPlayer.SetCameraParams(lensCfg.autoCalibrateSettings.cameraParams);
    //>>

    //合上滤光片
    IRCUTSwtich(m_oVideoPlayer.GetCaptureFilter(), TRUE, m_tDeviceInfo.m_nPID, m_tDeviceInfo.m_nVID);

    //<<added by toxuke, 2016/11/12
    //禁用画面亮度自动调节
    this->m_pInterceptFilter->EnableBrightnessAutoRegulating(FALSE);
    //>>

    //停止"静态屏蔽"操作
    m_oPenPosDetector.EnableStaticMasking(FALSE);

    //停止"动态屏蔽"操作
    m_oPenPosDetector.EnableDynamicMasking(FALSE);

	//停止“抗干扰”操作
	m_oPenPosDetector.EnableAntiJamming(FALSE);

    //不显示光斑外接矩形
    m_oPenPosDetector.ShowSpotRect(FALSE);

    //不显示光斑尺寸信息
    m_oPenPosDetector.ShowSpotSizeInfo(FALSE);



    TAutoMaskingParams autoMaskingParams;

    autoMaskingParams.hNotifyWnd = hNotifyWnd;
    autoMaskingParams.ChangeCameraParamsProc = OnAutoCalibChangeCameraParams;
    autoMaskingParams.lpCtx = (LPVOID)this;
    autoMaskingParams.eDebugLevel = (ECalibDebugLevel)g_tSysCfgData.globalSettings.nDebugLevel;

    //高亮块对应的灰度值
    //BYTE  cHilightGray = m_tCfgData.autoCalibrateSettings.autoCalibrateHilightGray;
    //autoCalibrateParams.clrGridHighlight       = RGB(cHilightGray,cHilightGray,cHilightGray);
  //  autoMaskingParams.imageParamsList        = lensCfg.autoCalibrateSettings.calibrateImageParamsList;

    //delete by vera_zhao
//	autoMaskingParams.imageParamsList = calibrateImageParamsList;

	autoMaskingParams.bSaveInermediatFile = g_tSysCfgData.globalSettings.bSaveIntermediateFile;
	autoMaskingParams.bRecordVideo = g_tSysCfgData.globalSettings.bRecordVideo;//m_tCfgData.advanceSettings.bRecordVideo;


    autoMaskingParams.bDoStaticMaskingOnly = TRUE;//仅作静态屏蔽

	int nCount = lensCfg.autoCalibrateSettingsList.size();
	for (int i = 0; i < nCount; i++)
	{
		AutoCalibrateParams params;
		params.autoCalibrateImageParams = lensCfg.autoCalibrateSettingsList[i].calibrateImageParams;
		params.videoprocampproperty = lensCfg.autoCalibrateSettingsList[i].cameraParams;
		autoMaskingParams.autocalibrateparamslist.push_back(params);
	}

    //搜索系统屏幕个数
    //theApp.GetMonitorFinder().SearchDisplayDev();
    //if(this->m_nID >= theApp.GetMonitorFinder().GetDisplayDevCount()) return;
    //const DisplayDevInfo* pDisplayDevInfo = theApp.GetMonitorFinder().GetDisplayDevInfo(this->m_nID);
    //if(pDisplayDevInfo)
    //{
    //    autoMaskingParams.monitors.push_back(*pDisplayDevInfo);
    //}
    TScreenInfo tScreenInfo;
    BOOL bRet = this->GetAttachedScreenArea(tScreenInfo.rcArea);
    if (!bRet) return;
    autoMaskingParams.vecScreenInfos.push_back(tScreenInfo);


    //准备静态屏蔽参数
    TStaticMaskingParams staticMaskingParams;
    staticMaskingParams.fpPreStaticMaskingProc = CIWBSensor::OnPreStaticMasking;
    staticMaskingParams.lpPreStaticMaskingCtx = this;
    staticMaskingParams.cStaticMaskThreshold = lensCfg.autoMaskSettings.cAutoMaskDetectThreshold;
    staticMaskingParams.nMaskEroseSize = lensCfg.autoMaskSettings.nMaskAreaEroseSize;

    int Width = m_oPenPosDetector.GetSrcImageWidth();
    int Height = m_oPenPosDetector.GetSrcImageHeight();
    m_oAutoCalibrator.StartMasking(autoMaskingParams, staticMaskingParams, Width, Height);

    //m_oPenPosDetector.GetMouseEventGenerator().EnterCalibrateMode(m_oAutoCalibrator.GetCalibrateHWnd(), CALIBRATE_MODE_AUTO);
    m_oPenPosDetector.EnterCalibrateMode(m_oAutoCalibrator.GetCalibrateHWnd(), CALIBRATE_MODE_AUTO);


}



//@功能:在视频画面中显示错误信息
void CIWBSensor::ShowErrInfo(LPCTSTR lpszInfo)
{
    m_oVideoPlayer.SetDisplayInfo(lpszInfo);
}


//@功能:传感器自动校正完成后的事件响应函数
//@参数:bSuccess, 成功/失败标志
void CIWBSensor::OnAutoCalibrateDone(BOOL bSuccess)
{

    if (!bSuccess)//失败
    {
        //如果调试输出目录不为空则弹出该文件夹
        if (!PathIsDirectoryEmpty(m_oAutoCalibrator.GetCalibrateIntermediataDirName()))
        {
            ShellExecute(NULL, NULL, m_oAutoCalibrator.GetCalibrateIntermediataDirName(), NULL, NULL, SW_SHOW);
        }

        //m_oVideoPlayer.SetDisplayInfo(_T("自动校正失败！"));
        //m_oVideoPlayer.SetDisplayInfo(_T("Auto Calibration Failed!"));
        m_oVideoPlayer.SetDisplayInfo(g_oResStr[IDS_STRING480]);
        m_oPenPosDetector.SetCalibrateFailed(TRUE);
        SwitchLensMode(E_VIDEO_TUNING_MODE);
    }
    else//成功
    {
        //m_oVideoPlayer.SetDisplayInfo(_T("Auto Calibrate Succeeded."));
        m_oVideoPlayer.SetDisplayInfo(g_oResStr[IDS_STRING481]);
        m_oPenPosDetector.SetCalibrateFailed(FALSE);


        m_oPenPosDetector.SetStaticMaskFrame(m_oAutoCalibrator.GetMaskFrame());

        //保存静态屏蔽图
        m_oPenPosDetector.SaveStaticMaskFrame();

        CVideToScreenMap& vtsm = m_oPenPosDetector.GetVideoToScreenMap();

        const TCalibData* pCalibData = m_oAutoCalibrator.GetCalibrateData();

        if (pCalibData && pCalibData->allMonitorCalibData.size() > 0)
        {
            TCalibData calibData = *pCalibData;
            //vtsm.SetCalibrateData(*pCalibData, m_tCfgData.advanceSettings.bIsRearProjection);
            calibData.eCalibType = E_CALIBRATE_TYPE_AUTO;
            calibData.lpCtx = const_cast<LPVOID>((const LPVOID)m_oAutoCalibrator.GetMaskFrame().GetData());

            vtsm.SetCalibrateData(calibData);

            TSensorModeConfig* TSensorModeConfig = NULL;

			EProjectionMode eProjectionMode = g_tSysCfgData.globalSettings.eProjectionMode;
            TSensorModeConfig = &m_tCfgData.vecSensorModeConfig[eProjectionMode];
     
            if (vtsm.DoCalibrate())
            {
                TSensorModeConfig->calibParam = *vtsm.GetCalibParams();
            }
        }


        //设置光点到屏幕编号的映射信息
        m_oPenPosDetector.GetVideoToScreenMap().SetScreenMap(m_oAutoCalibrator.GetScreenMap());

        //保存屏幕映射图
        m_oPenPosDetector.GetVideoToScreenMap().SaveScreenMap();

        //进入正常使用模式
        SwitchLensMode(E_NORMAL_USAGE_MODE);

    }
    BOOL bRecordVideoTemp = g_tSysCfgData.globalSettings.bRecordVideo;
    if (bRecordVideoTemp)
    {
        if (m_pInterceptFilter->IsRecording())
        {
            this->m_pInterceptFilter->StopRecording();
        }
    }


    //注意一定要等校正完成后再离开校正模式，避免"光斑
    //离开校正模式
    //this->m_oPenPosDetector.GetMouseEventGenerator().LeaveCalibrateMode();
    m_oPenPosDetector.LeaveCalibrateMode();
}



//@功能:开始图像传感器的手动校正流程
//@参数:
//      nPtsInRow, 每行的校正点个数
//      nPtsInCol, 每列的校正点个数
//      hNotifyWnd, 校正结束后的同志消息的接收窗体
void  CIWBSensor::StartManualCalibrate(HWND hNotifyWnd, int nPtsInRow, int nPtsInCol)
{
    TManualCalibrateParameters parameters;
    //搜索系统屏幕个数
    //theApp.GetMonitorFinder().SearchDisplayDev();
    //if(this->m_nID >= theApp.GetMonitorFinder().GetDisplayDevCount()) return;
    //const DisplayDevInfo* pDisplayDevInfo = theApp.GetMonitorFinder().GetDisplayDevInfo(this->m_nID);
    //if(pDisplayDevInfo)
    //{
    //    parameters.monitors.push_back(*pDisplayDevInfo);
    //}

    TScreenInfo tScreenInfo;
    BOOL bRet = this->GetAttachedScreenArea(tScreenInfo.rcArea);
    if (!bRet)
    {
        return;
    }
    parameters.vecScreenInfos.push_back(tScreenInfo);


    //合上滤光片
    IRCUTSwtich(m_oVideoPlayer.GetCaptureFilter(), TRUE, m_tDeviceInfo.m_nPID, m_tDeviceInfo.m_nVID);

    TSensorModeConfig* TSensorModeConfig = NULL;

	EProjectionMode eProjectionMode = g_tSysCfgData.globalSettings.eProjectionMode;
    TSensorModeConfig = &m_tCfgData.vecSensorModeConfig[eProjectionMode];

    parameters.nCalibratePointsInRow = (nPtsInRow == -1) ? TSensorModeConfig->manualCalibrateSetting.nPtNumInEachRow : nPtsInRow;
    parameters.nCalibratePointsInCol = (nPtsInCol == -1) ? TSensorModeConfig->manualCalibrateSetting.nPtNumInEachCol : nPtsInCol;
    parameters.hNotifyWnd = hNotifyWnd;
    //把图像的高和宽传递进手动校正中
    //add by vera_zhao 2018.11.30
    int ImageWidth = this->m_oPenPosDetector.GetSrcImageWidth();
    int ImageHeight = this->m_oPenPosDetector.GetSrcImageHeight();

    this->m_oManualCalibrator.StartCalibrate(parameters, ImageWidth, ImageHeight);

    m_oPenPosDetector.EnterCalibrateMode(this->m_oManualCalibrator, CALIBRATE_MODE_MANUAL);


}


//@功能:传感器手动校正完成后的事件响应函数
//@参数:bSuccess, 成功/失败标志
void CIWBSensor::OnManualCalibrateDone(BOOL bSuccess)
{
    if (bSuccess)
    {
        CVideToScreenMap& vtsm = m_oPenPosDetector.GetVideoToScreenMap();

        TCalibData calibData;
        calibData.allMonitorCalibData = m_oManualCalibrator.GetCalibrateData();
        calibData.szImage.cx = m_oManualCalibrator.GetImageWidth();
        calibData.szImage.cy = m_oManualCalibrator.GetImageHeight();

        calibData.eCalibType = E_CALIBRATE_TYPE_MANUAL;
        vtsm.SetCalibrateData(calibData);

        TSensorModeConfig* TSensorModeConfig = NULL;
		EProjectionMode eProjectionMode = g_tSysCfgData.globalSettings.eProjectionMode;
        TSensorModeConfig = &m_tCfgData.vecSensorModeConfig[eProjectionMode];
 
        if (vtsm.DoCalibrate())
        {
            TSensorModeConfig->calibParam = *vtsm.GetCalibParams();

            CImageFrame maskFrame = m_oManualCalibrator.GetScreenAreaMask();

            LOG_INF("maskFrameWidth=%d，maskFrameHeight=%d", maskFrame.Width(), maskFrame.Height());

            const TLensConfig& lensCfg = TSensorModeConfig->lensConfigs[this->m_eCameraType][m_tCfgData.eSelectedLensType];

            //适当腐蚀屏蔽区，扩到屏幕区域
            for (int r = 0; r < lensCfg.autoMaskSettings.nMaskAreaEroseSize; r++)
            {
                Morph_Dilate8(
                    maskFrame.GetData(),
                    maskFrame.GetData(),
                    maskFrame.Width(),
                    maskFrame.Height());
            }

            //将手动校正找到的屏幕区域屏蔽图合并到静态屏蔽图中去
            m_oPenPosDetector.MergeManualCalibScreenMask(maskFrame);

            //保存静态屏蔽图
            m_oPenPosDetector.SaveStaticMaskFrame();

        }

    }

    //无论成功失败都进入正常使用模式
    //如果失败了,则自动使用原有的校正数据
    SwitchLensMode(E_NORMAL_USAGE_MODE);

    m_oPenPosDetector.LeaveCalibrateMode();
}


//@功能:传感器自动搜索屏蔽区域结束事件响应函数
//@参数:bSuccess, 成功/失败标志
void CIWBSensor::OnAutoSearchMaskAreaDone(BOOL bSuccess)
{
    if (!bSuccess)//失败
    {
        //显示自动屏蔽失败信息
        m_oVideoPlayer.SetDisplayInfo(_T("Auto Mask Failed！"));

        //打开调试输出目录
        ShellExecute(NULL, NULL, m_oAutoCalibrator.GetCalibrateIntermediataDirName(), NULL, NULL, SW_SHOW);

        SwitchLensMode(E_VIDEO_TUNING_MODE);
    }
    else//成功
    {
        //设置静态屏蔽图
        m_oPenPosDetector.SetStaticMaskFrame(m_oAutoCalibrator.GetMaskFrame());

        //设置光点到屏幕编号的映射信息
        m_oPenPosDetector.GetVideoToScreenMap().SetScreenMap(m_oAutoCalibrator.GetScreenMap());

        //进入正常使用模式
        SwitchLensMode(E_NORMAL_USAGE_MODE);

    }

    //注意一定要等校正完成后再离开校正模式，避免"光斑
    //离开校正模式
    this->m_oPenPosDetector.LeaveCalibrateMode();
}

//@功能:开启光斑采集功能
//@参数: hSampleWnd, 采样窗体句柄
BOOL CIWBSensor::StartLightSpotSampling(HWND hSampleWnd)
{

    ////搜索系统屏幕个数
    //theApp.GetMonitorFinder().SearchDisplayDev();
    // std::vector<DisplayDevInfo> vecMonitorInfo;
    //if(this->m_nID >= theApp.GetMonitorFinder().GetDisplayDevCount()) return FALSE;

    //const DisplayDevInfo* pDisplayDevInfo = theApp.GetMonitorFinder().GetMointorInfo(this->m_nID);
    //if(pDisplayDevInfo)
    //{
    //    vecMonitorInfo.push_back(*pDisplayDevInfo);
    //}
    //  //for(size_t i =0; i < m_tCfgData.attachedMonitorIds.size(); i++)
    //  //{
    //  //      unsigned int uMonitorID = m_tCfgData.attachedMonitorIds[i];
    //  //      
    //  //      //const DisplayDevInfo* pDisplayDevInfo = m_oDispMonitorFinder.GetMointorInfo(uMonitorID);
    //  //      const DisplayDevInfo* pDisplayDevInfo = theApp.GetMonitorFinder().GetMointorInfo(uMonitorID);
    //  //      if(pDisplayDevInfo)
    //  //      {
    //  //          vecMonitorInfo.push_back(*pDisplayDevInfo);
    //  //      }
    //  //}
    //
    //  
    //BOOL bRet =  m_wndLightSpotSampling.StartCollectSpotSize(&vecMonitorInfo[0], vecMonitorInfo.size(), hNotifyWindow);

    //if(bRet)
    //{
    //    //this->m_oPenPosDetector.GetMouseEventGenerator().EnterSpotSamplingMode(m_wndLightSpotSampling.m_hWnd, COLLECTSPOT_MODE_COLLECT);
    this->m_oPenPosDetector.EnterSpotSamplingMode(hSampleWnd);
    //}

    return TRUE;
}


//@功能:光斑采集结束事件的响应函数
//@参数:bSuccess, 成功失败标志
void CIWBSensor::OnLightSpotSamplingDone(const ALL_LIGHTSPOT_SAMPLE_SIZE& allSampleSize, BOOL bSuccess)
{

    this->m_oPenPosDetector.LeaveSpotSamplingMode();

    if (bSuccess)
    {
        this->m_oPenPosDetector.SetLightSpotSampleSize(allSampleSize);
    }

}


//@功能:屏幕分辨率发生变化时的事件响应函数
//@参数:rcNewMonitorResolution, 关联的屏幕分辨率信息

//void CIWBSensor::OnMonitorResolutionChange(const RECT&  rcNewMonitorResolution)
void CIWBSensor::OnMonitorResolutionChange()
{
    RECT rcArea;
    BOOL bRet = GetAttachedScreenArea(rcArea);
    if (bRet)
    {
        this->m_oPenPosDetector.GetVideoToScreenMap().SetActualMonitorResolution(rcArea);
        //		this->m_oPenPosDetector.SetScreenSize((rcArea.right - rcArea.left),(rcArea.bottom- rcArea.top));
    }
}

/*
@功能：开启智能检测挡板是否消失
*/
void CIWBSensor::OnStartDetectBackSplashVanished()
{
    this->m_oPenPosDetector.SetHasDetectSVStarted(true);
}

/*
@功能：停止智能检测挡板是否消失
*/
void CIWBSensor::OnStopDetectBackSplashVanished()
{
    this->m_oPenPosDetector.SetHasDetectSVStarted(true);
}


//@功能:设备丢失事件响应函数
void CIWBSensor::OnDeviceIsMissing()
{
    if (this->m_oPenPosDetector.IsManualCalibrating())
    {
        this->m_oManualCalibrator.OnDeviceMissing();
    }
    if (this->m_oPenPosDetector.IsAutoCalibrating())
    {
        this->m_oAutoCalibrator.OnDeviceMissing();
    }
}



//@功能:查看校正符号位置
//@参数:bShow,TRUE,显示符号
//            FALSE, 隐藏符号
//@Author:15077727@qq.com, 2015/09/09
BOOL CIWBSensor::CheckCalibrateSymbolPos(BOOL bShow)
{
    if (bShow)
    {
        if (this->m_nID >= theApp.GetMonitorFinder().GetDisplayDevCount()) return FALSE;
        const DisplayDevInfo* pDisplayDevInfo = theApp.GetMonitorFinder().GetDisplayDevInfo(this->m_nID);
        if (NULL == pDisplayDevInfo) return FALSE;

        //const TCalibData* pCalibData = m_oAutoCalibrator.GetCalibrateData();
        const TCalibData& calibData = m_oPenPosDetector.GetVideoToScreenMap().GetCalibrateData();

        if (calibData.allMonitorCalibData.size() == 0)
        {
            MessageBox(NULL, _T("No Calibrate Data found"), _T("Error"), MB_OK | MB_ICONERROR);
            return FALSE;
        }


        int nSymbolCount = calibData.allMonitorCalibData[0].calibData.size();

        std::vector<POINT> symbolPos;
        symbolPos.resize(nSymbolCount);

        for (int i = 0; i < nSymbolCount; i++)
        {
            symbolPos[i] = calibData.allMonitorCalibData[0].calibData[i].ptScreenCoord;
        }

        std::vector<POINT> projectPos;
        projectPos.resize(nSymbolCount);

        for (int i = 0; i < nSymbolCount; i++)
        {
            const TPoint2D& pt2dImage = calibData.allMonitorCalibData[0].calibData[i].pt2DImageCoord;
            bool bIsOutside = FALSE;

            TPoint2D ptScreen;
            m_oPenPosDetector.GetVideoToScreenMap().GetPt(pt2dImage, &ptScreen, &bIsOutside, TRUE/*不做自动校正补偿*/);

            projectPos[i].x = long(ptScreen.d[0] + .5);
            projectPos[i].y = long(ptScreen.d[1] + .5);
        }

        m_oCalibSymbolChecker.ShowCalibSymbols(
            pDisplayDevInfo->rcMonitor,
            &symbolPos[0],
            nSymbolCount,
            RGB(255, 0, 0),
            &projectPos[0]
        );

        m_oPenPosDetector.ShowCalibratePoints(TRUE);

    }
    else
    {
        m_oCalibSymbolChecker.Hide();
        m_oPenPosDetector.ShowCalibratePoints(FALSE);
    }

    return TRUE;

}

//判断是否校正符号是否已经显示
BOOL CIWBSensor::IsCalibrateSymbolVisible()const
{
    return m_oCalibSymbolChecker.IsVisible();
}


void _stdcall CIWBSensor::OnManualSampleDoneCallBackProc(LPVOID lpCtx, BOOL bSuccess)
{
    CIWBSensor* lpSensor = reinterpret_cast<CIWBSensor*>(lpCtx);

    if (bSuccess)
    {
        SYSTEMTIME sysTime;
        GetLocalTime(&sysTime);
        char szFileName[256];
        sprintf_s(
            szFileName,
            _countof(szFileName),
            "ReSampleData_%04d%02d%02d_%02d%02d%02d%03d.txt",
            sysTime.wYear,
            sysTime.wMonth,
            sysTime.wDay,
            sysTime.wHour,
            sysTime.wMinute,
            sysTime.wSecond,
            sysTime.wMilliseconds);

        FILE* file;

        errno_t err = fopen_s(&file, szFileName, "w");
        if (err == 0)
        {
            char szData[1256];

            const std::vector<TPoint2D> & vecSymbolsInVideo = lpSensor->GetCalibrateSymbolManualSampler().GetSymbolsCoordInVideo();
            const std::vector<TPoint2D> & vecLightSpotsInVideo = lpSensor->GetCalibrateSymbolManualSampler().GetLightSpotsCoordInVideo();

            const std::vector<POINT>& vecActualTouchPosInScreen = lpSensor->GetCalibrateSymbolManualSampler().GetActualTouchPosInScreen();
            const std::vector<POINT>& vecSymbolPosInScreen = lpSensor->GetCalibrateSymbolManualSampler().GetSymbolPosInScreen();
            int nCount = vecSymbolsInVideo.size();
            for (int i = 0; i < nCount; i++)
            {
                const TPoint2D& ptSymbol = vecSymbolsInVideo[i];
                const TPoint2D& ptLightSpot = vecLightSpotsInVideo[i];

                const POINT&  symbolPos = vecSymbolPosInScreen[i];
                const POINT&  actualTouch = vecActualTouchPosInScreen[i];
                sprintf_s(
                    szData,
                    _countof(szData),
                    "%f,%f,%f,%f,%d,%d,%d,%d\n",
                    (double)ptSymbol.d[0],
                    (double)ptSymbol.d[1],
                    (double)ptLightSpot.d[0],
                    (double)ptLightSpot.d[1],
                    symbolPos.x,
                    symbolPos.y,
                    actualTouch.x,
                    actualTouch.y
                );

                fwrite(szData, 1, strlen(szData), file);
            }

            fclose(file);
        }

    }

    lpSensor->GetPenPosDetector()->LeaveManualResample();

}


//@功能:对校正符号进行手动再采样
BOOL CIWBSensor::CalibrateSymbolManualResample()
{
    if (this->m_nID >= theApp.GetMonitorFinder().GetDisplayDevCount()) return FALSE;
    const DisplayDevInfo* pDisplayDevInfo = theApp.GetMonitorFinder().GetDisplayDevInfo(this->m_nID);
    if (NULL == pDisplayDevInfo) return FALSE;


    const TCalibData& calibData = m_oPenPosDetector.GetVideoToScreenMap().GetCalibrateData();

    if (calibData.allMonitorCalibData.size() == 0)
    {
        MessageBox(NULL, _T("No Calibrate Data found"), _T("Error"), MB_OK | MB_ICONERROR);
        return FALSE;
    }


    int nSymbolCount = calibData.allMonitorCalibData[0].calibData.size();

    std::vector<POINT> symbolPosOnScreen;
    symbolPosOnScreen.resize(nSymbolCount);

    std::vector<TPoint2D> symbolPosOnImage;
    symbolPosOnImage.resize(nSymbolCount);

    for (int i = 0; i < nSymbolCount; i++)
    {
        symbolPosOnScreen[i] = calibData.allMonitorCalibData[0].calibData[i].ptScreenCoord;
        symbolPosOnImage[i] = calibData.allMonitorCalibData[0].calibData[i].pt2DImageCoord;
    }

    m_oCalibSymbolManualSampler.DoManualResample(
        pDisplayDevInfo->rcMonitor,
        &symbolPosOnScreen[0],
        &symbolPosOnImage[0],
        nSymbolCount,
        RGB(255, 0, 0),
        &CIWBSensor::OnManualSampleDoneCallBackProc,
        this
    );

    m_oPenPosDetector.EnterManualResample(m_oCalibSymbolManualSampler.GetHwnd());

    return TRUE;
    return FALSE;}




//功能;判断传感器是否有校正数据
BOOL CIWBSensor::IsCalibrated()const
{
    const TCalibParams*  pCalibParams = m_oPenPosDetector.GetVideoToScreenMap().GetCalibParams();
    if (pCalibParams)
    {
        if (pCalibParams->allCalibCoefs.size() > 0)
        {
            if (pCalibParams->allCalibCoefs[0].calibCoefs.size() > 0)
                return TRUE;
        }
    }
    return FALSE;
}


/*
//@功能:获取关联的屏幕区域尺寸
BOOL CIWBSensor::GetAttachedScreenArea(RECT& rcMonitor)const
{
    theApp.GetMonitorFinder().SearchDisplayDev();

    int nMonitorCount = theApp.GetMonitorFinder().GetDisplayDevCount();
    if (nMonitorCount == 0) return FALSE;

    if (m_tCfgData.bAutoAttachMonitor)
    {//自动关联屏幕
        EScreenMode screenMode = theApp.GetScreenMode();

        if (screenMode == EScreenModeSingle)
        {//
            if (this->m_nID >= nMonitorCount) return FALSE;
            {
                const DisplayDevInfo* pDisplayDevInfo = theApp.GetMonitorFinder().GetPrimaryMonitorInfo();
                rcMonitor = pDisplayDevInfo->rcMonitor;
            }
            
        }
        else if (screenMode >= EScreenModeDouble)
        {//多屏拼接模式
            //EScreenModeDouble://双屏模式
            //EScreenModeTriple://三屏模式
            //EScreenModeQuad  ://四屏模式
            //EScreenModeQuint ://五屏模式
            //EScreenModeHexa  ://六屏模式
            if (nMonitorCount == 1)
            {//只有一个屏幕
                //将屏幕等比例均分
                const DisplayDevInfo* pDisplayDevInfo = theApp.GetMonitorFinder().GetDisplayDevInfo(0);

                LONG nWidth = pDisplayDevInfo->rcMonitor.right - pDisplayDevInfo->rcMonitor.left;
                rcMonitor = pDisplayDevInfo->rcMonitor;
                LONG left = rcMonitor.left;

                //每个屏幕的宽度
                LONG perWidth = nWidth / (int(screenMode) + 1);

                rcMonitor.left = left + this->m_nID*perWidth;

                rcMonitor.right = rcMonitor.left + perWidth;


            }
            else//多个屏幕
            {
                

            }
        }//else if

    }
    else
    {//手动指定屏幕
        if (this->m_tCfgData.nAttachedMonitorId >= theApp.GetMonitorFinder().GetDisplayDevCount())
        {
            return FALSE;
        }
        const DisplayDevInfo* pDisplayDevInfo = theApp.GetMonitorFinder().GetDisplayDevInfo(this->m_tCfgData.nAttachedMonitorId);
        switch (this->m_tCfgData.eMonitorAreaType)
        {
        case E_MONITOR_AREA_TYPE_FULLSCREEN://全屏
            rcMonitor = pDisplayDevInfo->rcMonitor;
            break;

        case  E_MONITOR_AREA_TYPE_LEFT_HALF://左半屏
            rcMonitor = pDisplayDevInfo->rcMonitor;
            {
                int nWidth = pDisplayDevInfo->rcMonitor.right - pDisplayDevInfo->rcMonitor.left;
                rcMonitor.right = rcMonitor.left + (nWidth >> 1);
            }
            break;

        case E_MONITOR_AREA_TYPE_RIGHT_HALF://右半部
            rcMonitor = pDisplayDevInfo->rcMonitor;
            {
                int nWidth = pDisplayDevInfo->rcMonitor.right - pDisplayDevInfo->rcMonitor.left;
                rcMonitor.left = rcMonitor.right - (nWidth >> 1);
            }
            break;

        default:
            return FALSE;
        }
    }//else

    return TRUE;
}
*/

void CIWBSensor::SetAttachedScreenArea(const RECT& rcMonitor)
{
    m_rcMonintorArea = rcMonitor;
    this->m_oPenPosDetector.GetVideoToScreenMap().SetActualMonitorResolution(rcMonitor);
}

//@功能:获取关联的屏幕区域尺寸
BOOL CIWBSensor::GetAttachedScreenArea(RECT& rcMonitor)const
{
    rcMonitor = m_rcMonintorArea;
    return TRUE;
}

void CIWBSensor::OnTimer(LPVOID lpCtxData)
{
    m_oTimerActionExecuter.Run();
}

void CIWBSensor::SetlenCfgData(const TLensConfig& lencfgData)
{
    TSensorModeConfig* TSensorModeConfig = NULL;
	EProjectionMode eProjectionMode = g_tSysCfgData.globalSettings.eProjectionMode;
    TSensorModeConfig = &m_tCfgData.vecSensorModeConfig[eProjectionMode];

    TLensConfig& lensCfg = TSensorModeConfig->lensConfigs[this->m_eCameraType][m_tCfgData.eSelectedLensType];
    lensCfg = lencfgData;
}

//@功能：把设置插值的值保存下来
void CIWBSensor::SetStrokeInterpolate(bool bEnableStrokeInterpolate)
{
	TSensorModeConfig* TSensorModeConfig = NULL;

	EProjectionMode eProjectionMode = g_tSysCfgData.globalSettings.eProjectionMode;
    TSensorModeConfig = &m_tCfgData.vecSensorModeConfig[eProjectionMode];

	TSensorModeConfig->advanceSettings.bEnableStrokeInterpolate = bEnableStrokeInterpolate;
}

//@功能：把设置的勾勒外部屏幕区域的值保存下来
void CIWBSensor::SetOnlineScreenArea(bool bEnableOnlineScreenArea) 
{
	EProjectionMode eProjectionMode = g_tSysCfgData.globalSettings.eProjectionMode;

	m_tCfgData.vecSensorModeConfig[eProjectionMode].advanceSettings.bIsOnLineScreenArea = bEnableOnlineScreenArea;

	this->m_oPenPosDetector.EnableOnlineScreenArea(bEnableOnlineScreenArea);
	
}

void CIWBSensor::GetPidVid(INT* pPID, INT* pVID)const
{
	if (pPID) *pPID = m_tDeviceInfo.m_nPID;
	if (pVID) *pVID = m_tDeviceInfo.m_nVID;
}