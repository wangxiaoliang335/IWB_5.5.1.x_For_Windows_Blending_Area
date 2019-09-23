// OpticalPen.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
//#include "headers.h"

#include "../inc/EasiSDKReg_Interface.h"
#pragma comment(lib,"../lib/EasiSDKReg_Interface.lib")

//#ifdef NDEBUG
#include "../bitanswer/BitAnswerCpp.h"
#include "OnlineRegisterDlg.h"
//<<added by xuke, 2015/10/27
CBitAnswer  g_bitanswer;//比特安索在线注册对象。
//>>
//#endif

#define new DEBUG_NEW

//@功能:返回实际的触控类型
EDeviceTouchType GetActualTouchType()
{
    if(theApp.GetUSBKeyTouchType() == E_DEVICE_PEN_TOUCH)
    {//加密狗为笔触模式, 强制工作模式为笔触模式
        return E_DEVICE_PEN_TOUCH;
    }
    else
    { 
		//加密狗为手触模式, 选用用户选择的触控模式
		if (g_tSysCfgData.globalSettings.eProjectionMode == E_PROJECTION_DESKTOP)
		{
           return g_tSysCfgData.vecSensorConfig[0].vecSensorModeConfig[0].advanceSettings.m_eTouchType;
		}
		return g_tSysCfgData.vecSensorConfig[0].vecSensorModeConfig[1].advanceSettings.m_eTouchType;
    }
}

//@功  能:返回实际的屏幕控制区域
//@参  数:
//@返回值:
SIZE GetActualScreenControlSize()
{
    SIZE szScreen;

    if(theApp.GetScreenType() == EDoubleScreenMode)
    {
		//<<temp, 2017/08/22
        //szScreen.cx = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
        //szScreen.cy = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);

		szScreen.cx = ::GetSystemMetrics(SM_CXSCREEN);
		szScreen.cy = ::GetSystemMetrics(SM_CYSCREEN);
		//temp, 2017/08/22>>
    }
    else
    {
        szScreen.cx = ::GetSystemMetrics(SM_CXSCREEN);
        szScreen.cy = ::GetSystemMetrics(SM_CYSCREEN);
    }

    return szScreen;
}




// CIWBApp
BEGIN_MESSAGE_MAP(CIWBApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CIWBApp construction

CIWBApp::CIWBApp()
:
m_hMutex(NULL),
m_bAutoRunMode(FALSE),
m_bForAllUser(FALSE),
m_eUSBKeyTouchType(E_DEVICE_PEN_TOUCH),
m_eScreenType(ESingleScreenMode),
m_bFoundHardwareUSBKey(FALSE),
m_bIsOnlineRegistered(FALSE)
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance

    //提升程序进程优先级为实时
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
}

CIWBApp::~CIWBApp()
{
    if(m_hMutex)
    {
        //Use the CloseHandle function to close the handle. The system closes the handle automatically when the process terminates. 
        //The mutex object is destroyed when its last handle has been closed.
        //主动关闭，不依赖进程的终止。
        CloseHandle(m_hMutex);//2014/04/01
    }
}

// The one and only CIWBApp object
CIWBApp theApp;
//#ifdef EVALUATION_VERSION
//试用时间单位:ms
#define EVALUATION_USE_TIME (30*60*1000)

DWORD g_dwBeginTime;
bool  g_bAlertDlgShowing = false;
VOID CALLBACK  timerProc( 
                         _In_ HWND     hwnd,
                         _In_ UINT     uMsg,
                         _In_ UINT_PTR idEvent,
                         _In_ DWORD    dwTime)
{


    DWORD dwElapseTime = dwTime - g_dwBeginTime;

    if(dwElapseTime > EVALUATION_USE_TIME)
    {

        if(!g_bAlertDlgShowing)
        {
            g_bAlertDlgShowing = true;
            //MessageBox(NULL, _T("很抱歉,软件试用时间到, 请重新启动应用程序"), _T("通知"), MB_OK);
            MessageBox(NULL, g_oResStr[IDS_STRING478], g_oResStr[IDS_STRING479], MB_OK);
			PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_CLOSE, 0, 0);
        }
    }




}
//#endif

// CIWBApp initialization

BOOL CIWBApp::InitInstance()
{
	SKDREG_Init();

    //初始化目录信息
    InitDirectoryInformation();

    //载入配置信息
    ::LoadConfig(PROFILE::CONFIG_FILE_NAME, g_tSysCfgData);

    m_strLanguageCode =  g_tSysCfgData.globalSettings.langCode;

    //事先载入语言资源，因为如果有多个实例时需要给出提示信息
    //查找OEM_RES.dll文件, 如果有则载入, 否则载入
    HINSTANCE hResource = NULL;
    hResource = LoadOEMResource();


    if(!hResource)//载入OEM资源失败,则依照
    {

        //从配置文件中读取配置的语言
        //若未发现配置信息则缺省为系统的默认语言。
        //LCID lcid = PROFILE::GetCfg_LCID();

        //lcid = MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT);
        hResource = LoadResDll(this->m_hInstance, m_strLanguageCode);

        //<added by jiqw 2015/05/11
        g_tSysCfgData.globalSettings.langCode = m_strLanguageCode; //当使用本地默认语言时，把为""的g_tSysCfgData.globalSettings.langCode设置为本地语言编码
        //>
    }

    if(hResource == NULL)
    {
        //载入失败则缺省载入英文资源
        m_strLanguageCode = _T("EN");
        g_oResStr.SetResInst(AfxGetResourceHandle());

    }
    else
    {
        AfxSetResourceHandle(hResource);
        _AtlBaseModule.SetResourceInstance(hResource);
        g_oResStr.SetResInst(hResource);

    }
    g_oResStr.Initialize();


    m_hMutex = CreateMutex(NULL, TRUE, _T("Global\\OpticalPen_20140630"));
    if(m_hMutex != NULL)
    {

        if(GetLastError() == ERROR_ALREADY_EXISTS)
        {
            MessageBox(NULL,g_oResStr[IDS_STRING106], g_oResStr[IDS_STRING107], MB_ICONINFORMATION);
            return FALSE;
        }
    }
    else
    {
        DWORD dwError = GetLastError();
        CAtlString strError = GetErrorMessage(dwError);
        LOG_ERR("CreateMutex Failed(0x%x):%s", dwError, (const char*)CT2CA(strError));
    }

    //AsyncLogInit(_T("OpticalPen.log"));
    //#ifdef _DEBUG
    //	static CMemLeakDetect oMemLeakDetector;
    //#endif 

    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    AfxEnableControlContainer();

    //
    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	
	//Sleep(5000);//延迟启动5秒中
	m_oUSBCameraList.UpdateDeviceList();

	int nDeviceCount = m_oUSBCameraList.GetDeviceInstanceCount();

    BOOL bDoubleScreenTouchMerge = FALSE;//双屏拼接功能检测
    //#ifndef _DEBUG

    DWORD dwStartTick = GetTickCount();
    BOOL bFoundUSBKey = FALSE;

//#ifdef NDEBUG

    do
    {
        UINT uKeyNum = SDKREG_GetUSBKeyCount();

		LOG_DBG("uKeyNum=%d", uKeyNum);

        int nAppType = 0;
        float fVersion = 0.0f;

        for(UINT uKeyIndex = 0; uKeyIndex < uKeyNum; uKeyIndex++)
        {

            if(SDKREG_GetVersion(&fVersion,uKeyIndex) != S_OK)
            {
                continue;
            }

            if(fVersion < 0.20111018f)
            {
                continue;
            }

            //AppType各位描述
            //
            //bit9 bit8 bit7 bit6 bit5 bit4 bit3 bit2 bit1 bit0
            //      │                                      │
            //      │                                      └─0:3D Touch；1:手指触控
            //      └─1:使能双屏拼接功能                                                 
            //
            //

            if(SDKREG_GetAppType(&nAppType, uKeyIndex) != S_OK)
            {
                if((nAppType & 0x00000FF) != 0  && (nAppType & 0x0000FF) != 1)
                {
                    //nAppType
                    //1:为手指触控
                    //0:为3DTouch
                    //既不为手指触控也不为3D-Touch,则继续搜索下一个加密狗。
                    continue;
                }

            }

            bDoubleScreenTouchMerge = (nAppType >> 8) & 0x00000001;
            bFoundUSBKey = TRUE;//找到加密狗退出
            break;
        }//for


        if(!bFoundUSBKey  || SDKREG_IsBasicFunctionEnabled(14) != S_OK)
        {

			LOG_DBG("bFoundUSBKey=%s,SDKREG_IsBasicFunctionEnabled=%d\n", 
				bFoundUSBKey ? "TRUE" : "FALSE",
				SDKREG_IsBasicFunctionEnabled(14)
			);
            DWORD dwNow   = GetTickCount();
            DWORD dwElapse =  dwNow - dwStartTick;

            if(dwElapse <g_tSysCfgData.globalSettings.nMaxTimeInSearchDevice)
            {
				LOG_INF("Not find USBKey then delay 1 second, time has elapsed %fs\n", (float)dwElapse/1000.0);
                Sleep(1000);//延迟等待1秒钟

                continue;
            }
			else if(g_tSysCfgData.globalSettings.bEnableOnlineRegister)
            {   
				//如果使能了在线注册
                //判断是否已经在线注册过了。
				if (nDeviceCount > 0)///说明发现摄像头设备
				{					
                    BIT_STATUS status = g_bitanswer.Login("", BIT_MODE_AUTO);
                    if(status != BIT_SUCCESS)
                    {
				    	LOG_ERR("bitAnswer login in returns 0x%x\n", status);
                        COnlineRegisterDlg onlineRegisterDlg;
                        onlineRegisterDlg.DoModal();

                        if(onlineRegisterDlg.IsRegisteredOk())
                        {
                            m_eUSBKeyTouchType = onlineRegisterDlg.GetTouchType();
                            bDoubleScreenTouchMerge =onlineRegisterDlg.GetScreenType() == EDoubleScreenMode?TRUE:FALSE;
                            break;
                        }
                     }
                     else
                     {    //成功了
                          //Read Features
                          BIT_UINT32 value;
                          BIT_STATUS status = g_bitanswer.ReadFeature(FEATURE_TOUCH_TYPE,&value);
                          if(status == BIT_SUCCESS)
                          {
                              m_eUSBKeyTouchType = (value == 0)?E_DEVICE_PEN_TOUCH:E_DEVICE_FINGER_TOUCH;
                          }

                          status = g_bitanswer.ReadFeature(FEATURE_SCREEN_TYPE,&value);
                          if(status == BIT_SUCCESS)
                          {
                               bDoubleScreenTouchMerge = (value == 1)?TRUE:FALSE;
                          }

					      m_bIsOnlineRegistered = TRUE;
                          break;
                    }					
				}

                //开启试用版超时检测器。
                SetTimer(NULL, 0, 1000, timerProc);
                g_dwBeginTime = GetTickCount();
                m_eUSBKeyTouchType = E_DEVICE_FINGER_TOUCH;

				LOG_INF("Start Evaluation Timer\n");
                break;

                
            }
			else
			{//
				MessageBox(NULL, g_oResStr[IDS_STRING125], g_oResStr[IDS_STRING103], MB_ICONERROR | MB_OK);
				return FALSE;
			}
            
        }
        else
        {
			LOG_INF("find USBKey\n");
            m_eUSBKeyTouchType = (nAppType & 0x00000001)?E_DEVICE_FINGER_TOUCH:E_DEVICE_PEN_TOUCH;
            m_bFoundHardwareUSBKey = TRUE;

        }


    }while(!bFoundUSBKey);


//#endif


    //#endif
    m_eScreenType = bDoubleScreenTouchMerge?EDoubleScreenMode:ESingleScreenMode;

    //m_eScreenType = EDoubleScreenMode;




    LOG_INF("Usb Key Support Screen Merge:%s\n", bDoubleScreenTouchMerge?"Yes":"No");

    //<<del
    ////说明加密狗是双屏拼接，  再就是验证分辨率的长宽比例，如果长宽的比例小于16:10的时候，说明是单屏的，只是双屏的加密狗而已
    //del>>

    int nCxScreen   = GetSystemMetrics(SM_CXSCREEN       )  ;
    int nCyScreen   = GetSystemMetrics(SM_CYSCREEN       )  ;


    int nCxVScreen   = GetSystemMetrics(SM_CXVIRTUALSCREEN) ;
    int nCyVScreen   = GetSystemMetrics(SM_CYVIRTUALSCREEN) ;
    int nVScreenLeft = GetSystemMetrics(SM_XVIRTUALSCREEN ) ;
    int nVScreenTop  = GetSystemMetrics(SM_YVIRTUALSCREEN ) ;

    LOG_INF("Screen Size = %d X %d, Virtual Screen Size = %d X %d, <Left,Top>=<%d,%d>,  Double Screen Merge Eabled:%s\n", 
        nCxScreen, 
        nCyScreen, 
        nCxVScreen,
        nCyVScreen,
        nVScreenLeft,
        nVScreenTop,
        (EDoubleScreenMode==m_eScreenType)?"Yes":"No");

    ParseCmdLine(this->m_lpCmdLine);
    CIWBDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }

    //AsyncLogUninit();

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    LOG_INF("Exit CIWBApp::InitInstance()");

	SKDREG_Uninit();
    return FALSE;
}




BOOL CIWBApp::ParseCmdLine(LPCTSTR lpCmdLine)
{
    //将空格或者Tab符看作分隔符,解析命令行参数中的参数,每个参数放在字符串列表中
    std::list<CAtlString> lstParam;
    //
    if(lpCmdLine == NULL) return FALSE;

    //Step1:分割参数
    const TCHAR* pChar    = lpCmdLine;//字符串中指向某位置字符的指针
    const TCHAR* pFirstPos = lpCmdLine;//解析的参数字串的第一个字符

    do
    {
        //过滤空格和TAB分隔符
        while(*pChar != _T('\0') && (*pChar ==  _T(' ')|| *pChar == _T('\t')))
        {
            pChar++;
            pFirstPos++;
        }
        if(*pChar != _T('\0'))
        {

            while(*pChar != _T('\0'))
            {
                if(*pChar == _T(' ') || *pChar == _T('\t') )//出现了分隔符
                {
                    if(*pFirstPos != _T(' ') && *pFirstPos != _T('\t'))
                    {
                        //std::string str;
                        //str.resize(pChar - pFirstPos + 1/*结尾NULL*/);
                        CAtlString str;
                        int strLength = pChar - pFirstPos ;
                        TCHAR* pBuf = str.GetBuffer(strLength + 1/*结尾NULL*/);
                        _tcsncpy_s(pBuf, strLength + 1, pFirstPos, strLength);

                        pBuf[strLength] = _T('\0');

                        str.ReleaseBuffer();

                        //找到一个参数
                        lstParam.push_back(str);

                        //
                        pFirstPos = pChar;
                        break;
                    }//if

                }//if

                pChar++;
            }//while
        }//if


    }while(*pChar != _T('\0'));

    if((pChar-pFirstPos)>0)
    {

        CAtlString str;
        int strLength = pChar - pFirstPos ;
        TCHAR* pBuf = str.GetBuffer(strLength + 1/*结尾NULL*/);
        _tcsncpy_s(pBuf, strLength + 1, pFirstPos, strLength);

        pBuf[strLength] = _T('\0');

        str.ReleaseBuffer();

        //找到一个参数
        lstParam.push_back(str);

    }

    for(std::list<CAtlString>::const_iterator it = lstParam.begin(); it != lstParam.end(); it++)
    {
        if(_tcsicmp(*it,_T("-autorun")) == 0)
        {
            m_bAutoRunMode = TRUE;
        }
        if(_tcsicmp(*it, _T("-allusers")) == 0)
        {
            m_bForAllUser = TRUE;
        }

    }

    return TRUE;
}


//@功能: 载入OEM资源。
//@说明:OEM, Original Equipment Manufacture
//OEM,  manufactures products or components that are purchased by a company 
//      and retailed under that purchasing company's brand name.
//
HINSTANCE CIWBApp::LoadOEMResource()
{
    HINSTANCE hInstance = NULL;
    TCHAR szFullFileName[MAX_PATH];
    LPTSTR pszFileName = NULL;

    int ret = ::GetModuleFileName(this->m_hInstance, szFullFileName, MAX_PATH);
    if(ret == 0 || ret == MAX_PATH)
    {
        ASSERT(FALSE);
        return NULL;
    }
    pszFileName = :: PathFindFileName(szFullFileName);

    if(pszFileName)
    {
        *pszFileName = _T('\0');
        _tcscat_s(szFullFileName, _countof(szFullFileName), _T("OEM_RES.dll"));
    }

    hInstance = ::LoadLibrary(szFullFileName);

    return hInstance;
}


//<<added by toxuke@gmail.com, 2013/10/10
//@功能:初始化程序依赖的路径名称
//      包括配置文件的目录路径和调试输出文件保存的目录路径。
void CIWBApp::InitDirectoryInformation()
{
    //获取当前工作路径
    TCHAR szPath[MAX_PATH];
    TCHAR* pcwd = _tgetcwd(szPath, _countof(szPath));
    if(!pcwd)
    {
        //致命错误
        assert(FALSE);
        return;
    }

    //判断是否在U盘或者在固定介质上上运行
    int iDrive = PathGetDriveNumber(pcwd);


    BOOL bIntermediatOutputInSystemTempDir = FALSE;//调试目录在系统临时目录下标志
    TCHAR szDrive[4];

    if(iDrive !=-1)
    {
        memset(szDrive, 0, sizeof(szDrive));
        PathBuildRoot(szDrive, iDrive);
        UINT uType = GetDriveType(szDrive);

        switch(uType)
        {
        case DRIVE_UNKNOWN:
            //The drive type cannot be determined.
            bIntermediatOutputInSystemTempDir = TRUE;

            break;

        case DRIVE_NO_ROOT_DIR:
            //The root szPath is invalid; for example, there is no volume is mounted at the szPath.
            bIntermediatOutputInSystemTempDir = TRUE;
            break;

        case DRIVE_REMOVABLE:
            //The drive has removable media; for example, a floppy drive, thumb drive, or flash card reader.
            bIntermediatOutputInSystemTempDir = TRUE;    
            break;

        case DRIVE_FIXED:
            //The drive has fixed media; for example, a hard drive or flash drive.
            bIntermediatOutputInSystemTempDir = FALSE;
            break;

        case DRIVE_REMOTE:
            //The drive is a remote (network) drive.
            bIntermediatOutputInSystemTempDir = TRUE;
            break;

        case DRIVE_CDROM:
            //The drive is a CD-ROM drive.
            bIntermediatOutputInSystemTempDir = TRUE;
            break;

        case DRIVE_RAMDISK:
            //The drive is a RAM disk
            bIntermediatOutputInSystemTempDir = TRUE;

            break;
        }//switch

    }

    //
    if(bIntermediatOutputInSystemTempDir)
    {
        TCHAR szTmpPath[MAX_PATH];
        memset(szTmpPath, 0, _countof(szTmpPath));

        //在系统临时目录中创建输出目录
        GetTempPath(_countof(szTmpPath),szTmpPath);//D:\Users\k.xu\AppData\Local\Temp\

        m_strIntermediatOutputDir = szTmpPath;
        m_strIntermediatOutputDir += _T("Intermediate(EASI Gloview)");

    }
    else
    {
        //在当前目录下临时输出目录
        m_strIntermediatOutputDir = pcwd;
        m_strIntermediatOutputDir += _T("\\Intermediate");
    }
    //
    BOOL bConfigUSBDiskFound = FALSE;//查看存放配置文件的U盘是否存在


    //易视
    //const TCHAR* szConfigUSBDiskVolumeName = _T("ISAESY@9168");
    //新加坡
    //const TCHAR* szConfigUSBDiskVolumeName = _T("V-ID2");
    //{{OEM}}
    const TCHAR* szConfigUSBDiskVolumeName = _T("ISAESY@9168");
    //{{OEM}}


    //枚举所有逻辑盘
    DWORD dwMaskBits = GetLogicalDrives();
    iDrive = 0;
    while(dwMaskBits)
    {
        if(dwMaskBits & 0x00000001)
        {
            memset(szDrive, 0, sizeof(szDrive));
            PathBuildRoot(szDrive, iDrive);
            UINT uType = GetDriveType(szDrive);
            if(uType == DRIVE_REMOVABLE)
            {
                TCHAR szVolumeName[MAX_PATH+1];
                memset(szVolumeName, 0, sizeof(szVolumeName));

                TCHAR szFileSystemNameBuffer[MAX_PATH + 1];
                memset(szFileSystemNameBuffer, 0, sizeof(szFileSystemNameBuffer));

                DWORD dwMaximumComponentLength = 0;
                DWORD dwFileSystemFlags = 0;


                BOOL bRet = 
                    GetVolumeInformation(
                    szDrive,                  //lpRootPathName
                    szVolumeName,             //lpVolumeNameBuffer
                    _countof(szVolumeName),   //nVoumeNameSize
                    NULL,                     //lpVolumeSerialNumber
                    &dwMaximumComponentLength,//lpMaximumComponentLength 
                    &dwFileSystemFlags       ,//lpFileSystemFlags
                    szFileSystemNameBuffer   ,//lpFileSystemNameBuffer,
                    _countof(szFileSystemNameBuffer)//
                    );


                if(bRet)
                {

                    if(_tcsicmp(szConfigUSBDiskVolumeName, szVolumeName) == 0)
                    {
                        bConfigUSBDiskFound = TRUE;
                        break;
                    }
                }
            }

        }
        iDrive ++;
        dwMaskBits >>= 1;
    }//while


    if(bConfigUSBDiskFound)
    {
        m_strSettingsDir = szDrive;
    }
    else
    {
        m_strSettingsDir = pcwd;
    }

    m_strSystemDir = pcwd ;
    //设置
    ALGOAPI_SetIntermediateDir(m_strIntermediatOutputDir);  
    //设置配置路径
    PROFILE::CONFIG_FILE_NAME       = m_strSettingsDir+_T("\\Video.dll");
    //配置信息保存的根目录
    PROFILE::SETTINGS_BASE_DIRECTORY  = m_strSettingsDir;

	m_strFirmwareDirectory = m_strSettingsDir + _T("\\firmware");
}

EDeviceTouchType  CIWBApp::GetUSBKeyTouchType()const
{
    return this->m_eUSBKeyTouchType;
}



//从USBKey中读取信息
void CIWBApp::ReadUSBKey()
{
    BOOL bDoubleScreenTouchMerge = FALSE;//双屏拼接功能检测
    BOOL bFoundUSBKey = FALSE;

    UINT uKeyNum = SDKREG_GetUSBKeyCount();

    int nAppType = 0;
    float fVersion = 0.0f;

    for(UINT uKeyIndex = 0; uKeyIndex < uKeyNum; uKeyIndex++)
    {

        if(SDKREG_GetVersion(&fVersion,uKeyIndex) != S_OK)
        {
            continue;
        }

        if(fVersion < 0.20111018f)
        {
            continue;
        }

        //AppType各位描述
        //
        //bit9 bit8 bit7 bit6 bit5 bit4 bit3 bit2 bit1 bit0
        //      │                                      │
        //      │                                      └─0:3D Touch；1:手指触控
        //      └─1:使能双屏拼接功能                                                 
        //
        //

        if(SDKREG_GetAppType(&nAppType, uKeyIndex) != S_OK)
        {
            if((nAppType & 0x00000FF) != 0  && (nAppType & 0x0000FF) != 1)
            {
                //nAppType
                //1:为手指触控
                //0:为3DTouch
                //既不为手指触控也不为3D-Touch,则继续搜索下一个加密狗。
                continue;
            }

        }

        bDoubleScreenTouchMerge = (nAppType >> 8) & 0x00000001;
        bFoundUSBKey = TRUE;//找到加密狗退出
        break;
    }//for


    //if(bFoundUSBKey && m_eScreenType != EDoubleScreenMode)//双屏模式使用外置加密锁
	if (bFoundUSBKey)
    {
        m_eUSBKeyTouchType = (nAppType & 0x00000001)?E_DEVICE_FINGER_TOUCH:E_DEVICE_PEN_TOUCH;       
        m_eScreenType = bDoubleScreenTouchMerge?EDoubleScreenMode:ESingleScreenMode;

    }
	else
	{
		BIT_STATUS status = g_bitanswer.Login("", BIT_MODE_AUTO);
		if (status == BIT_SUCCESS)
		{//成功了
			bFoundUSBKey = TRUE;
		   //Read Features
			BIT_UINT32 value;
			BIT_STATUS status = g_bitanswer.ReadFeature(FEATURE_TOUCH_TYPE, &value);
			if (status == BIT_SUCCESS)
			{
				m_eUSBKeyTouchType = (value == 0) ? E_DEVICE_PEN_TOUCH : E_DEVICE_FINGER_TOUCH;
			}

			status = g_bitanswer.ReadFeature(FEATURE_SCREEN_TYPE, &value);
			if (status == BIT_SUCCESS)
			{
				bDoubleScreenTouchMerge = (value == 1) ? TRUE : FALSE;
			}

		}

	}

	m_bFoundHardwareUSBKey = bFoundUSBKey;
}