// OpticalPen.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "UsbCameraDeviceList.h"
// CIWBApp:
// See OpticalPen.cpp for the implementation of this class




//@功能:返回实际的触控类型
EDeviceTouchType GetActualTouchType();

//@功  能:返回实际的屏幕控制区域
//@参  数:
//@返回值:
SIZE GetActualScreenControlSize();

class CIWBApp : public CWinApp
{
public:
    CIWBApp();

    ~CIWBApp();
// Overrides

    virtual BOOL InitInstance();
    
    BOOL IsStartFromAutoRun()const
    {
        return m_bAutoRunMode;
    }

    BOOL IsForAllUsers()const
    {
        return m_bForAllUser;
    }

    
    EDeviceTouchType  GetUSBKeyTouchType() const;
	EPalmTouchControlType GetPalmTouchType()const;


    //int GetDoubleScreenMerge() const
    //{
    //  return m_nDoubleScreenType ;
    //}

    EScreenMode GetScreenMode()const;
    EScreenMode GetScreenModeFromUSBKey()const;

    int GetScreenCount() const;


    BOOL IsHardwareKeyExist()const
    {
        return m_bFoundHardwareUSBKey;
    }

    BOOL IsOnlineRegistered()const
    {
        return m_bIsOnlineRegistered;
    }
    LPCTSTR GetLangeCode()const
    {
        return (LPCTSTR) m_strLanguageCode;
    }
// Implementation
    BOOL ParseCmdLine(LPCTSTR lpCmdLine);

    //@功能: 载入OEM资源。
    //@说明:OEM, Original Equipment Manufacture
    //OEM,  manufactures products or components that are purchased by a company 
    //      and retailed under that purchasing company's brand name.
    //
    HINSTANCE LoadOEMResource();


    //<<added by toxuke@gmail.com, 2013/10/10
    //@功能:初始化程序依赖的路径名称
    //      包括配置文件的目录路径和调试输出文件保存的目录路径。
    void  InitDirectoryInformation();


    CDispDevFinder& GetMonitorFinder(){return m_oDispMonitorFinder;}

    CString m_strSettingsDir         ;//配置文件存放目录
    CString m_strSystemDir           ; //安装程序下系统目录的位置
    CString m_strIntermediatOutputDir;//调试输出文件存放目录
    CString m_strFirmwareDirectory   ;//固件文件目录

    //@功能:从USBKey中读取信息
    //@参数:bFirstTime, 第一次检测UsbKey的存在
    //@说明:第一次检测UsbKey时允许弹出对话框, 并记录日志信息。
    //      第二次及以后则不再弹出兑换框。
    void ReadUSBKey(BOOL bFirstTime = FALSE);


protected:
    DECLARE_MESSAGE_MAP()


    HANDLE m_hMutex;
    BOOL   m_bAutoRunMode;
    BOOL   m_bForAllUser ;//所有用户标志

    //int    m_nUSBKeyTouchType;////0:为3DTouch电子白板,1:为手指触控电子白板，2:为高清手指触控，3:为手掌互动  
    EDeviceTouchType m_eUSBKeyTouchType;//
	EPalmTouchControlType   m_ePalmTouchControlType;

    
    //EScreenType    m_eScreenType; //0:为单屏，1：为双屏


    EScreenMode    m_eScreenModeFromUsbKey;//从机密狗注册信息中得到的屏幕模式
    
    
    BOOL           m_bFoundHardwareUSBKey;//发现硬件USB Key标志
    BOOL           m_bIsOnlineRegistered;//是否在线注册了。
    CString m_strLanguageCode;//语言编码

    CDispDevFinder m_oDispMonitorFinder;

	CUsbCameraDeviceList   m_oUSBCameraList;//视频设备列表

    //static const int m_nStartDelayTime = 5000;//10000ms,最大启动延迟时间

};

extern CIWBApp theApp;