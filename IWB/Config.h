#pragma once
#include "../inc/CameraSpecs.h"

//传感器镜头模式
enum ESensorLensMode
{
    E_VIDEO_TUNING_MODE,//图像调试模式
    E_LASER_TUNING_MODE,//激光器调试模式
    E_NORMAL_USAGE_MODE,//正常使用模式
};

//触控类型
enum EDeviceTouchType
{
    E_DEVICE_PEN_TOUCH_WHITEBOARD   , //笔触控电子白板（精准触控）
    E_DEVICE_FINGER_TOUCH_WHITEBOARD, //手触控电子白板（精准触控）
	E_DEVICE_FINGER_TOUCH_CONTROL   , //手指触控(大画面精准触控)
	E_DEVICE_PALM_TOUCH_CONTROL     , //手掌互动(非精准触控并保证某一区域只能大光斑触发，激光器装在中间)
	E_DEVICE_NOFIND                 , //没发现加密狗
};

/////手掌互动触控的类型
enum EPalmTouchControlType
{
	E_PLAM_TOUCHCONTROL_P0,        //手掌互动触控的类型，激光器是放在中间位置的
	E_PLAM_TOUCHCONTROL_P1,        //手掌互动触控的类型，激光器是放在左上角的位置的。
	E_PLAM_TOUCHCONTROL_P2,        //保留
	E_PLAM_TOUCHCONTROL_P3,        //保留
	E_PLAM_TOUCHCONTROL_P4,        //保留
	E_PLAM_TOUCHCONTROL_P5,        //保留
};

enum EHIDDeviceMode
{
	E_DEV_MODE_MOUSE,   //鼠标
	E_DEV_MODE_TOUCHSCREEN,//触屏
};

enum EProjectionMode
{
	E_PROJECTION_WALL,    //墙面模式
	E_PROJECTION_DESKTOP, //桌面模式
};

enum ECameraType
{
	E_CAMERA_TR_0 = 0,   //PID=0x9186,VID=0x18ec  //正常摄像头
	E_CAMERA_TR_1 = 1,   //PID=0x3390,VID=0x18ec  //1/6摄像头
	E_CAMERA_TR_2 = 2,   //PID=0x9230,VID= 0x05a3;//高清摄像头
};

enum ETouchScreenAspectRatio
{
    E_TOUCH_SCREEN_ASPECT_RATIO_AUTO  ,//自动决定宽高比
    E_TOUCH_SCREEN_ASPECT_RATIO_16_9 ,//宽高比16:9
    E_TOUCH_SCREEN_ASPECT_RATIO_16_10 ,//宽高比16:10
    E_TOUCH_SCREEN_ASPECT_RATIO_4_3   ,//宽高比4:3
};
#ifdef PICO_PROJECTOR
#define DEFAULT_SCREEN_DIAGONAL_LENGTH (20.0f*25.4f)
#else
#define DEFAULT_SCREEN_DIAGONAL_LENGTH (80.0f*25.4f)
#endif


//<added by Jiqw 201505191446
#define DEFAULT_SCREEN_DIAGONAL_LOGICAL_LENGTH (1270) //1024 * 768
//>

//全局设定
struct GlobalSettings
{
    CAtlString              langCode             ;   //语言ID
	EProjectionMode         eProjectionMode      ;   //墙面还是桌面
    BOOL                    bDebugMode           ;   //调试模式
    int                     nDebugLevel          ;   //调试级别
    BOOL                    bSaveIntermediateFile;   //保存中间文件标志
    ESensorLensMode         eLensMode            ;   //镜头工作模式
	BOOL                    bTouchTUIOMode       ;   //TUIO触控方式
	BOOL                    bTouchHIDMode        ;   //HID触控方式
    EHIDDeviceMode          eHIDDeviceMode       ;   //鼠标/触屏模式
    ETouchScreenAspectRatio eTouchScreenAspectRatio; //触屏宽高比
    BOOL                    bEnableOnlineRegister;   //使能在线注册功能

    BOOL                    bRecordVideo          ; //是否在自动校正时保存画面录像
    BOOL                    bDoubleScreenTouchMerge;//双屏是否融合标志
//  ELensFocalType          eLensFocalType      ;//镜头焦距类型

    float                   fScreenDiagonalPhysicalLength;//触屏对角线物理尺寸,单位:毫米(mm);
    int                     nMaxTimeInSearchDevice;//程序启动时,搜索设备时最大允许花费的时间， 单位:毫秒。
    TCMOSChipSpecification  CMOSChipSpecification;//CMOS芯片规格信息

	ECameraType             eCameraType;         //攝像頭的類型

    GlobalSettings()
    {
        langCode                 = _T("");
		eProjectionMode          = E_PROJECTION_DESKTOP;
        nDebugLevel              = 0;
        bDebugMode               = FALSE;
        bSaveIntermediateFile    = FALSE;
        eLensMode                = E_VIDEO_TUNING_MODE;
        eHIDDeviceMode           = E_DEV_MODE_TOUCHSCREEN;
        eTouchScreenAspectRatio  = E_TOUCH_SCREEN_ASPECT_RATIO_AUTO;
        bEnableOnlineRegister    = TRUE;

        bRecordVideo             = FALSE;
        bDoubleScreenTouchMerge  = TRUE;
        //eLensFocalType         = E_LENS_FOCAL_NEAR;//缺省为"短焦"镜头。

        //fScreenDiagonalPhysicalLength = 80.0f*25.4f;//缺省为80英寸
        fScreenDiagonalPhysicalLength = DEFAULT_SCREEN_DIAGONAL_LENGTH;//缺省为80英寸
        //2017/03/14>>
        nMaxTimeInSearchDevice = 8000;

        //<<2017/03/14
        //缺省为OV7225的规格
        CMOSChipSpecification.pixel_size      = 0.006;//6um
        CMOSChipSpecification.width_in_mm     = 3.984;//3984um
        CMOSChipSpecification.height_in_mm    = 2.952;//2952um
        CMOSChipSpecification.width_in_pixel  = 656  ;//像素
        CMOSChipSpecification.height_in_pixel = 488  ;//像素

		eCameraType = E_CAMERA_TR_0;
    }
};

//手指触控设备正常使用时的设定值
#define FINGER_TOUCH_NORMAL_USAGE_BRIGHTNESS  150

//笔触设备正常使用时的设定值
#define PEN_TOUCH_NORMAL_USAGE_BRIGHTNESS     25

struct NormalUsageSettings
{
    BYTE           cYThreshold            ;//亮度门限,范围(0~255)
    int            nDynamicMaskThresholdPercentage;//动态屏蔽门限占触发门限的百分比(50~100)
    int            nLightSpotMinimumWidth ;//最小光斑的宽度
    int            nLightSpotMinimumHeight;//最小光斑的高度

    //相机参数
    TVideoProcAmpProperty   cameraParams;

    //<<added by 15077726@qq.com
    //缺省值
    BYTE            cYThresholdDefault            ;//缺省亮度门限
    int             nDynamicMaskThresholdPercentageDefault;//动态屏蔽门限占触发门限的百分比(50~100)
    BYTE            nLightSpotMinimumWidthDefault ;//缺省的最小光斑的宽度
    BYTE            nLightSpotMinimumHeightDefault;//缺省的最小光斑的高度

    TVideoProcAmpProperty defaultParams;
    //>>

    NormalUsageSettings()
    {
        cYThreshold = 205;
        nDynamicMaskThresholdPercentage = 80;//动态屏蔽门限占触发门限的百分比(50~100)
        nLightSpotMinimumWidth = 2;
        nLightSpotMinimumHeight = 2;

        cameraParams.Prop_VideoProcAmp_Brightness           = FINGER_TOUCH_NORMAL_USAGE_BRIGHTNESS;//亮度
        cameraParams.Prop_VideoProcAmp_Contrast             = 255    ;//对比度
        cameraParams.Prop_VideoProcAmp_Hue                  = 0      ;//色调
        cameraParams.Prop_VideoProcAmp_Satuation            = 64     ;//饱和度
        cameraParams.Prop_VideoProcAmp_Sharpness            = 1      ;//锐利度
        cameraParams.Prop_VideoProcAmp_Gamma                = 1      ;//Gamma
        cameraParams.Prop_VideoProcAmp_ColorEnable          = 0      ;//颜色
        cameraParams.Prop_VideoProcAmp_WhiteBalance         = 5000   ;//白平衡
        cameraParams.Prop_VideoProcMap_BacklightCompensation= 2      ;//背光补偿
        cameraParams.Prop_VideoProcMap_Gain                 = 0x44AA ;//增益
		cameraParams.Prop_CameraControl_Exposure = -7                ;//曝光时间


        //缺省值初始化
        cYThresholdDefault = 205;
        nDynamicMaskThresholdPercentageDefault = 80;
        nLightSpotMinimumWidthDefault = 2;
        nLightSpotMinimumHeightDefault = 2;

        defaultParams.Prop_VideoProcAmp_Brightness           = FINGER_TOUCH_NORMAL_USAGE_BRIGHTNESS;//亮度
        defaultParams.Prop_VideoProcAmp_Contrast             = 255    ;//对比度
        defaultParams.Prop_VideoProcAmp_Hue                  = 0      ;//色调
        defaultParams.Prop_VideoProcAmp_Satuation            = 64     ;//饱和度
        defaultParams.Prop_VideoProcAmp_Sharpness            = 1      ;//锐利度
        defaultParams.Prop_VideoProcAmp_Gamma                = 1      ;//Gamma
        defaultParams.Prop_VideoProcAmp_ColorEnable          = 0      ;//颜色
        defaultParams.Prop_VideoProcAmp_WhiteBalance         = 5000   ;//白平衡
        defaultParams.Prop_VideoProcMap_BacklightCompensation= 2      ;//背光补偿
        defaultParams.Prop_VideoProcMap_Gain                 = 0x44AA ;//增益
		defaultParams.Prop_CameraControl_Exposure = -7;//曝光时间

    }
};

//安装调试时的摄像头参数
struct InstallTunningSettings
{
    //相机参数
    TVideoProcAmpProperty   cameraParams;

    //相机参数
    //<<added by 15077726@qq.com, 2015/11/6
    //<<
    TVideoProcAmpProperty   defaultParams;
    //>>


    InstallTunningSettings()
    {
        cameraParams.Prop_VideoProcAmp_Brightness           = 150    ;//亮度
        cameraParams.Prop_VideoProcAmp_Contrast             = 150    ;//对比度
        cameraParams.Prop_VideoProcAmp_Hue                  = 0      ;//色调
        cameraParams.Prop_VideoProcAmp_Satuation            = 64     ;//饱和度
        cameraParams.Prop_VideoProcAmp_Sharpness            = 1      ;//锐利度
        cameraParams.Prop_VideoProcAmp_Gamma                = 1      ;//Gamma
        cameraParams.Prop_VideoProcAmp_ColorEnable          = 0      ;//颜色
        cameraParams.Prop_VideoProcAmp_WhiteBalance         = 5000   ;//白平衡
        cameraParams.Prop_VideoProcMap_BacklightCompensation= 2      ;//背光补偿
        cameraParams.Prop_VideoProcMap_Gain                 = 0x33AA ;//增益
		cameraParams.Prop_CameraControl_Exposure = -7;

        //缺省值初始化
        defaultParams.Prop_VideoProcAmp_Brightness           = 150    ;//亮度
        defaultParams.Prop_VideoProcAmp_Contrast             = 150    ;//对比度
        defaultParams.Prop_VideoProcAmp_Hue                  = 0      ;//色调
        defaultParams.Prop_VideoProcAmp_Satuation            = 64     ;//饱和度
        defaultParams.Prop_VideoProcAmp_Sharpness            = 1      ;//锐利度
        defaultParams.Prop_VideoProcAmp_Gamma                = 1      ;//Gamma
        defaultParams.Prop_VideoProcAmp_ColorEnable          = 0      ;//颜色
        defaultParams.Prop_VideoProcAmp_WhiteBalance         = 5000   ;//白平衡
        defaultParams.Prop_VideoProcMap_BacklightCompensation= 2      ;//背光补
        defaultParams.Prop_VideoProcMap_Gain                 = 0x33AA ;//增益
		defaultParams.Prop_CameraControl_Exposure = -7;
    }

};


//激光器调试模式下的设定值
struct LaserTunningSettings
{
    //相机参数
    TVideoProcAmpProperty   cameraParams;

    //缺省参数
    //相机参数
    //<<added by 15077726@qq.com, 2015/11/6
    //<<
    TVideoProcAmpProperty   defaultParams;
    //>>

    LaserTunningSettings()
    {
        cameraParams.Prop_VideoProcAmp_Brightness           = 120    ;//亮度
        cameraParams.Prop_VideoProcAmp_Contrast             = 150    ;//对比度
        cameraParams.Prop_VideoProcAmp_Hue                  = 0      ;//色调
        cameraParams.Prop_VideoProcAmp_Satuation            = 64     ;//饱和度
        cameraParams.Prop_VideoProcAmp_Sharpness            = 1      ;//锐利度
        cameraParams.Prop_VideoProcAmp_Gamma                = 1      ;//Gamma
        cameraParams.Prop_VideoProcAmp_ColorEnable          = 0      ;//颜色
        cameraParams.Prop_VideoProcAmp_WhiteBalance         = 5000   ;//白平衡
        cameraParams.Prop_VideoProcMap_BacklightCompensation= 2      ;//背光补偿
        cameraParams.Prop_VideoProcMap_Gain                 = 0x44AA ;//增益
		cameraParams.Prop_CameraControl_Exposure = -7;

        defaultParams.Prop_VideoProcAmp_Brightness           = 120    ;//亮度
        defaultParams.Prop_VideoProcAmp_Contrast             = 150    ;//对比度
        defaultParams.Prop_VideoProcAmp_Hue                  = 0      ;//色调
        defaultParams.Prop_VideoProcAmp_Satuation            = 64     ;//饱和度
        defaultParams.Prop_VideoProcAmp_Sharpness            = 1      ;//锐利度
        defaultParams.Prop_VideoProcAmp_Gamma                = 1      ;//Gamma
        defaultParams.Prop_VideoProcAmp_ColorEnable          = 0      ;//颜色
        defaultParams.Prop_VideoProcAmp_WhiteBalance         = 5000   ;//白平衡
        defaultParams.Prop_VideoProcMap_BacklightCompensation= 2      ;//背光补偿 偿
        defaultParams.Prop_VideoProcMap_Gain                 = 0x44AA ;//增益
		defaultParams.Prop_CameraControl_Exposure = -7;

    }
};



//自动校正时的设定参数值
struct AutoCalibrateSettings
{
    //BYTE autoCalibrateExpectedBrightness; //自动校正亮度自动控制时期望的平均画面亮度(范围0~255)
    //BYTE autoCalibrateHilightGray; //自动校正图案中高亮块的灰度值(0~255)
    //相机参数
    TVideoProcAmpProperty   cameraParams;
    //自动校正时的画面参数,每一组参数对应一次校正尝试的画面参数
	AutoCalibrateImageParams   calibrateImageParams;

    //缺省参数
    //相机参数
    //<<added by 15077726@qq.com, 2015/11/6
    //<<
    TVideoProcAmpProperty     defaultParams;
	AutoCalibrateImageParams  defaultCalibrateImageParams;
    //>>


    AutoCalibrateSettings()
    {

        cameraParams.Prop_VideoProcAmp_Brightness           = 120    ;//亮度
        cameraParams.Prop_VideoProcAmp_Contrast             = 50     ;//对比度
        cameraParams.Prop_VideoProcAmp_Hue                  = 0      ;//色调
        cameraParams.Prop_VideoProcAmp_Satuation            = 64     ;//饱和度
        cameraParams.Prop_VideoProcAmp_Sharpness            = 1      ;//锐利度
        cameraParams.Prop_VideoProcAmp_Gamma                = 1      ;//Gamma
        cameraParams.Prop_VideoProcAmp_ColorEnable          = 0      ;//颜色
        cameraParams.Prop_VideoProcAmp_WhiteBalance         = 5000   ;//白平衡
        cameraParams.Prop_VideoProcMap_BacklightCompensation= 2      ;//背光补偿
        cameraParams.Prop_VideoProcMap_Gain                 = 0x33AA ;//增益
		cameraParams.Prop_CameraControl_Exposure = -7;

		calibrateImageParams.autoCalibrateExpectedBrightness = 100;
		calibrateImageParams.autoCalibrateHilightGray        = 255;//940 nm 激光器

        //缺省参数
        defaultParams.Prop_VideoProcAmp_Brightness           = 120    ;//亮度
        defaultParams.Prop_VideoProcAmp_Contrast             = 50     ;//对比度
        defaultParams.Prop_VideoProcAmp_Hue                  = 0      ;//色调
        defaultParams.Prop_VideoProcAmp_Satuation            = 64     ;//饱和度
        defaultParams.Prop_VideoProcAmp_Sharpness            = 1      ;//锐利度
        defaultParams.Prop_VideoProcAmp_Gamma                = 1      ;//Gamma
        defaultParams.Prop_VideoProcAmp_ColorEnable          = 0      ;//颜色
        defaultParams.Prop_VideoProcAmp_WhiteBalance         = 5000   ;//白平衡
        defaultParams.Prop_VideoProcMap_BacklightCompensation= 2      ;//背光补偿
        defaultParams.Prop_VideoProcMap_Gain                 = 0x33AA ;//增益
		defaultParams.Prop_CameraControl_Exposure = -7;

		defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
		defaultCalibrateImageParams.autoCalibrateHilightGray        = 255;//940 nm 激光?
    }

};
/////自动校正参数列表
typedef std::vector<AutoCalibrateSettings> AutoCalibrateSettingsList;



//自动屏蔽时的设定参数值
#define DEFAULT_MASK_AREA_EROSIE_SIZE 12
struct AutoMaskSettings
{
    BYTE cAutoMaskDetectThreshold;//自动屏蔽目标检测门限0~255, 值越小则屏蔽的区域越多
    int  nMaskAreaEroseSize      ;//自动屏蔽区域腐蚀半径

    //<<添加缺省值
    //added by 15077726@qq.com, 2015/11/6
    BYTE  cAutoMaskDetectThresholdDefault;
    int   nMaskAreaEroseSizeDefault;
    //>>

    AutoMaskSettings()
    {
        cAutoMaskDetectThreshold = 180;
        nMaskAreaEroseSize = DEFAULT_MASK_AREA_EROSIE_SIZE;

        cAutoMaskDetectThresholdDefault = 180;
        nMaskAreaEroseSizeDefault = DEFAULT_MASK_AREA_EROSIE_SIZE;

    }
};

//手动校正时的设定参数值
struct ManualCalibrateSettings
{
    int nPtNumInEachRow;//手动校正每行的校正点个数(=列数),最小值=2
    int nPtNumInEachCol;//手动校正每列的校正点个数(=行数),最小值=2
    ManualCalibrateSettings()
    {
        nPtNumInEachRow = 5;
        nPtNumInEachCol = 5;
    }
};


//高级设定
struct TAdvancedSettings
{
    int nAutoMaskDilateRadius  ;//自动屏蔽膨胀半径
    int nXDeviationCompensationValue;//X方向偏差修正值
    int nYDeviationCompensationValue;//Y方向偏差修正值

    int nSpotProportion            ;//光斑响应的比例
    int nMultEraser                ;//板擦的响应倍数
    int nFixedBlobSetTime          ;//定义为固定光斑的设置时间

	BOOL  bEnableStrokeInterpolate; //使能插值标志
	EDeviceTouchType m_eTouchType;  //触控类型
   //BOOL bGuestureRecognition     ;//手势识别检测功能使能标志
	BOOL bIsRearProjection;         //是否开启背投模式
	BOOL bIsDynamicMaskFrame;      //是否开启动态屏蔽功能
	BOOL bIsAntiJamming;           //是否开启抗干扰功能
	BOOL bIsOnLineScreenArea; //是否开启手动绘制的静态屏蔽图


    TAdvancedSettings()
    {
        nAutoMaskDilateRadius        = 1             ;
        nXDeviationCompensationValue = 0             ;
        nYDeviationCompensationValue = 0             ;
        nSpotProportion              = 30            ;
        nMultEraser                  = 3             ;
        nFixedBlobSetTime            = 5             ;
		bEnableStrokeInterpolate     = FALSE         ;
		m_eTouchType = E_DEVICE_PEN_TOUCH_WHITEBOARD ;
		bIsRearProjection = FALSE                    ;
		bIsDynamicMaskFrame = FALSE;
		bIsAntiJamming = FALSE;
		bIsOnLineScreenArea = FALSE;
    }
};



//镜头配置参数, 每个镜头关联以下参数
//1.安装调试时的参数
//2.激光器打板调试时.参数
//3.自动校正时的参数设置
//4.自动屏蔽时的参数设置
//5.手动校正时的参数
//6.手指触控模式下正常使用时的参数
//7.笔触控模式下的正常使用时的参数设定。

struct TLensConfig
{
    TInternalAndSymmetricDistortParams   lensInternalAndSymmetricDistortParams;//镜头内部和对称畸变参数
    BOOL                    bInternalAndSymmetricDistortParamsIsValid;         //镜头内部和对称畸变参数有效标志
    TLensSpecification      lensSpecification                        ;         //镜头规格参数
    TAutoCalibCompCoefs     autoCalibCompCoefs                       ;         //自动校正补偿系数
    NormalUsageSettings     normalUsageSettings_FingerTouchWhiteBoard;         //手指触控电子白板模式下的正常使用时的参数设定
    NormalUsageSettings     normalUsageSettings_PenTouchWhiteBoard   ;         //笔触控电子白板模式下的正常使用时的参数设定
	NormalUsageSettings     normalUsageSettings_FingerTouchControl   ;         //手指触控模式下的正常使用时的参数设定
	NormalUsageSettings     normalUsageSettings_PalmTouchControl     ;         //手掌触控模式下的正常使用时的参数设定

    InstallTunningSettings  installTunningSettings                   ;         //安装调试时的参数设定

    LaserTunningSettings    laserTunningSettings_WhiteBoard          ;         //电子白板激光器打板调试时的参数设定
	LaserTunningSettings    laserTunningSettings_FingerTouchControl  ;         //手指触控激光器打板调试时的参数设定
	LaserTunningSettings    laserTunningSettings_PalmTouchControl    ;         //手掌互动激光器打板调试时的参数设定

	AutoCalibrateSettingsList   autoCalibrateSettingsList            ;         //自动校正时的参数设置
    AutoMaskSettings            autoMaskSettings                     ;         //自动屏蔽时的参数设置


    //引导框
    RECT  rcGuideRectangle     ;//引导框位置
	int   Referwidth           ;//设置引导框所参考的分辨率
	int   ReferHeight;
    DWORD dwGuideRectangleColor;//引导框颜色
    BOOL  bRectangleVisible    ;//引导框可见标志

	TLensConfig()
	{
		bInternalAndSymmetricDistortParamsIsValid = FALSE;

		//2017/11/16, 新增引导框信息
		//<<
		int image_Default_Width = 640;
		int image_Default_Height = 480;
		rcGuideRectangle.left = image_Default_Width / 8;
		rcGuideRectangle.right = image_Default_Width - image_Default_Width / 8;
		rcGuideRectangle.top = image_Default_Height / 8;
		rcGuideRectangle.bottom = image_Default_Height - image_Default_Height / 8;
		Referwidth = image_Default_Width;
		ReferHeight = image_Default_Height;
		dwGuideRectangleColor = 0xFF0000;//RGB
		bRectangleVisible = TRUE;
		//>>
		this->autoCalibrateSettingsList.resize(3);
		
		{
			AutoCalibrateSettings &autocalibratesettings = autoCalibrateSettingsList[0];

			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
			autocalibratesettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
			autocalibratesettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
			autocalibratesettings.cameraParams.Prop_CameraControl_Exposure = -7;

			autocalibratesettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
			autocalibratesettings.calibrateImageParams.autoCalibrateHilightGray = 255;//940 nm 激光器
		}

		{
			AutoCalibrateSettings &autocalibratesettings = autoCalibrateSettingsList[1];

			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
			autocalibratesettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
			autocalibratesettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
			autocalibratesettings.cameraParams.Prop_CameraControl_Exposure = -7;

			autocalibratesettings.calibrateImageParams.autoCalibrateExpectedBrightness = 70;
			autocalibratesettings.calibrateImageParams.autoCalibrateHilightGray = 255;//940 nm 激光器
		}
		{
			AutoCalibrateSettings &autocalibratesettings = autoCalibrateSettingsList[2];

			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
			autocalibratesettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
			autocalibratesettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
			autocalibratesettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
			autocalibratesettings.cameraParams.Prop_CameraControl_Exposure = -7;

			autocalibratesettings.calibrateImageParams.autoCalibrateExpectedBrightness = 30;
			autocalibratesettings.calibrateImageParams.autoCalibrateHilightGray = 255;//940 nm 激光器
		}
    }
};

//镜头类型
enum ELensType
{
    E_LENS_TR_0_DOT_15   = 0,//投射比0.15
    E_LENS_TR_0_DOT_19   = 1,//投射比0.19
    E_LENS_TR_0_DOT_21   = 2,//投射比0.21
    E_LENS_TR_0_DOT_25   = 3,//投射比0.25
    E_LENS_TR_0_DOT_28   = 4,//投射比0.28
    E_LENS_TR_0_DOT_34   = 5,//投射比0.34
    E_LENS_TR_0_DOT_55   = 6,//投射比0.55
    E_LENS_TR_0_DOT_70   = 7,//投射比0.70
    E_LENS_TR_0_DOT_86   = 8,//投射比0.86
    E_LENS_TR_1_DOT_34   = 9,//投射比1.34
    E_LENS_TYPE_COUNT    = 10
};

//投射比列表

_declspec(selectany) extern const double TRHOW_RATIO_LIST[] =
{
    0.15,
    0.19,
    0.21,
    0.25,
    0.28,
    0.34,
    0.55,
    0.70,
    0.86,
    1.34,
};

//屏幕区域类型
enum EMonitorAreaType
{
    E_MONITOR_AREA_TYPE_FULLSCREEN = 0,//全屏
    E_MONITOR_AREA_TYPE_LEFT_HALF  = 1,//左半屏
    E_MONITOR_AREA_TYPE_RIGHT_HALF = 2 //右半部
};

struct TSensorModeConfig
{
	TAdvancedSettings       advanceSettings;//高级设置
	TLensConfig             lensConfigs[E_LENS_TYPE_COUNT];
	ManualCalibrateSettings manualCalibrateSetting;   //手动校正时的参数设置												   //std::vector<int>         attachedMonitorIds   ;//关联的显示器编号
	TCalibParams            calibParam;        //校正参数

	TSensorModeConfig()
	{
	    calibParam.szImage.cx = 640;
	    calibParam.szImage.cy = 480;
		//投射比1.34镜头的参数配置
		//==================================
		{
			TLensConfig&  lens = lensConfigs[E_LENS_TR_1_DOT_34];
			//正常电子白板手指触控模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;  //曝光时间

				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间
			}

			//正常电子白板笔触触控模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PenTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间
				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光时间
			}
			////正常手指触控模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness =0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 100;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间
																				  //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 100;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光时间
			}
			////正常手掌互动模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PalmTouchControl   ;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间
																					  //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光时间
			}

			//安装调试模式下的参数
			{
				InstallTunningSettings& installTunningSettings = lens.installTunningSettings;
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				installTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																					//缺省值初始化
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补
				installTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光时间
			}
			//电子白板激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_WhiteBoard;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 180;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光时间

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 180;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间
			}
			//手指触控激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_FingerTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光时间

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间
			}
			//手掌互动激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_PalmTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光时间

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间
			}

			//自动校正参数
			{
				AutoCalibrateSettingsList& autoCalibrateSettingsList = lens.autoCalibrateSettingsList;
				{
					AutoCalibrateSettings &autocalibratesettings = autoCalibrateSettingsList[0];
                    //第一组自动校正参数
				    autocalibratesettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autocalibratesettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autocalibratesettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autocalibratesettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

				    //第一组,对应较亮的光线环境
					autocalibratesettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autocalibratesettings.calibrateImageParams.autoCalibrateHilightGray = 255;//

				    //缺省参数
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autocalibratesettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autocalibratesettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autocalibratesettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

					//第一组,对应较亮的光线环境
					autocalibratesettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autocalibratesettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 255;//
				}
				{
					AutoCalibrateSettings &autocalibratesettings = autoCalibrateSettingsList[1];

					//第二组自动校正参数
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autocalibratesettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autocalibratesettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autocalibratesettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第二组,对应适当的光线环境
					autocalibratesettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autocalibratesettings.calibrateImageParams.autoCalibrateHilightGray = 180;

					//缺省参数
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autocalibratesettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autocalibratesettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autocalibratesettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

					autocalibratesettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autocalibratesettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 180;
				}
				{
					AutoCalibrateSettings &autocalibratesettings = autoCalibrateSettingsList[2];

					//第三组自动校正参数
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autocalibratesettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autocalibratesettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autocalibratesettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autocalibratesettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第三组, 对应较暗的光线环境
					autocalibratesettings.calibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autocalibratesettings.calibrateImageParams.autoCalibrateHilightGray = 120;

					//缺省参数
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autocalibratesettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autocalibratesettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autocalibratesettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autocalibratesettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

					//第三组, 对应较暗的光线环境
					autocalibratesettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autocalibratesettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 120;
				}
			}
			
			//自动屏蔽参数
			{
				AutoMaskSettings& autoMaskSettings = lens.autoMaskSettings;
				autoMaskSettings.cAutoMaskDetectThreshold = 180;
				autoMaskSettings.nMaskAreaEroseSize = DEFAULT_MASK_AREA_EROSIE_SIZE;

				autoMaskSettings.cAutoMaskDetectThresholdDefault = 180;
				autoMaskSettings.nMaskAreaEroseSizeDefault = DEFAULT_MASK_AREA_EROSIE_SIZE;
			}

			//镜头规格
			{
				TLensSpecification& lensSpecification = lens.lensSpecification;
				lensSpecification.backFocalLength = 3.5;//像方焦距,   单位:mm
				lensSpecification.FOV_horz = 36;//水平视角，  单位:度
				lensSpecification.FOV_vert = 26;//垂直视角，  单位:度
				lensSpecification.FOV_diagonal = 54;//对角线视角，单位:度
				lensSpecification.throwRatio = 1.34;//投射比
			}

			//自动校正补偿系数
			{
				TAutoCalibCompCoefs& autoCalibCompCoefs = lens.autoCalibCompCoefs;

				//<<2017/05/29, 用公司仓库采集到的数据的计算得到的模型参数
				//u0=320.161206,v0=238.086918,k1=10.077846,k2=-98.414409,k3=313.647717,u0_normalized=5.002519e-01, v0_normalized=4.960144e-01
				autoCalibCompCoefs.u0 = 320.161206 / 640;
				autoCalibCompCoefs.v0 = 238.086918 / 480;
				autoCalibCompCoefs.k[0] = 10.077846;
				autoCalibCompCoefs.k[1] = -98.414409;
				autoCalibCompCoefs.k[2] = 313.647717;
				//2017/05/29>>
			}

		}//投射比1.34镜头的参数配置

		 //投射比0.86镜头的参数配置
		 //==================================
		{
			TLensConfig&  lens = lensConfigs[E_LENS_TR_0_DOT_86];

			//电子白板正常手指触控模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光时间

				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间
			}

			//电子白板正常笔触触控模式下的使用参数
			{

				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PenTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

                //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间
			}

			//手指触控正常模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																				  //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间
			}

			//手掌互动正常模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PalmTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																				  //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间
			}

			//安装调试模式下的参数
			{
				InstallTunningSettings& installTunningSettings = lens.installTunningSettings;
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				installTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光时间

																					//缺省值初始化
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补
				installTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光時間
			}

			//电子白板激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_WhiteBoard;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 180;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光時間

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 180;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間
			}

			//手指触控激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_FingerTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光時間

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間
			}

			//手掌互动激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_PalmTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光時間

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間
			}

			//自动校正参数
			{
				AutoCalibrateSettingsList& autoCalibrateSettingsList = lens.autoCalibrateSettingsList;
				{
				    //第一组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[0];

				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				    autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				    autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				    autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																					//第一组,对应较亮的光线环境
				    autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
				    autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 255;//

																						  //缺省参数
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				    autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				    autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				    autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

																					  //第一组,对应较亮的光线环境
				    autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
				    //calibrateImageParamsList[0].autoCalibrateHilightGray        = 180;
				    autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 255;//
				}
				{
					//第二组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[1];
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				    autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				    autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				    autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																					//第二组,对应适当的光线环境
				    autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
				    autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 180;

				    //缺省参数
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				    autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				    autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				    autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

				    autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
				    autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 180;
				}
				{
					//第三组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[2];

				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				    autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				    autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				    autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																					//第三组, 对应较暗的光线环境
				    autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 70;
				    autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 120;

				    //缺省参数
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				    autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				    autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				    autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

																					  //第三组, 对应较暗的光线环境
				    autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 70;
				    autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 120;
				}

			}
		
			//自动屏蔽参数
			{
				AutoMaskSettings& autoMaskSettings = lens.autoMaskSettings;
				autoMaskSettings.cAutoMaskDetectThreshold = 180;
				autoMaskSettings.nMaskAreaEroseSize = DEFAULT_MASK_AREA_EROSIE_SIZE;

				autoMaskSettings.cAutoMaskDetectThresholdDefault = 180;
				autoMaskSettings.nMaskAreaEroseSizeDefault = DEFAULT_MASK_AREA_EROSIE_SIZE;
			}

			//镜头规格
			{
				TLensSpecification& lensSpecification = lens.lensSpecification;
				lensSpecification.backFocalLength = 3.6;//像方焦距,   单位:mm
				lensSpecification.FOV_horz = 0;//水平视角，  单位:度
				lensSpecification.FOV_vert = 0;//垂直视角，  单位:度
				lensSpecification.FOV_diagonal = 70;//对角线视角，单位:度
				lensSpecification.throwRatio = 0.86;//投射比
			}

			//自动校正补偿系数
			{
				TAutoCalibCompCoefs& autoCalibCompCoefs = lens.autoCalibCompCoefs;
				//u0=486.876093,v0=229.531703,k1=5.849763,k2=-21.908832,k3=39.819524
				//autoCalibCompCoefs.u0 = 486.876093/640.0;
				//autoCalibCompCoefs.v0 = 229.531703/480.0;

				//autoCalibCompCoefs.k[0] = 5.849763;
				//autoCalibCompCoefs.k[1] = -21.908832;
				//autoCalibCompCoefs.k[2] = 39.819524;



				//<2017/05/29, 用公司仓库采集到的数据的计算得到的模型参数
				//u0=335.187651,v0=245.791172,k1=10.713114,k2=-122.109087,k3=428.738565
				autoCalibCompCoefs.u0 = 335.187651 / 640.0;
				autoCalibCompCoefs.v0 = 245.791172 / 480.0;

				autoCalibCompCoefs.k[0] = 10.713114;
				autoCalibCompCoefs.k[1] = -122.109087;
				autoCalibCompCoefs.k[2] = 39.819524;
				//2017/05/29>>

			}

		}//投射比0.86镜头

		 //投射比0.55镜头的参数配置
		 //==================================
		{
			TLensConfig&  lens = lensConfigs[E_LENS_TR_0_DOT_55];

			//电子白板正常手指触控模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 100;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光時間


																				 //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 100;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光時間
			}

			//电子白板正常笔触触控模式下的使用参数
			{

				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PenTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光時間
				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間

			}

			//手指触控正常模式下的使用参数
			{

				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光時間
																				  //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間

			}

			//手掌互动正常模式下的使用参数
			{

				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PalmTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光時間
																				  //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 46;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間

			}

			//安装调试模式下的参数
			{
				InstallTunningSettings& installTunningSettings = lens.installTunningSettings;
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				installTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光時間

																					//缺省值初始化
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补
				installTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光時間
			}

			//电子白板激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_WhiteBoard;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 180;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光時間

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 180;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光時間
			}
			//手指触控激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_FingerTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光時間

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光時間
			}
			//手掌互动激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_PalmTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光時間

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光時間
			}

			//自动校正参数
			{
				AutoCalibrateSettingsList& autoCalibrateSettingsList = lens.autoCalibrateSettingsList;
				{
				   //第一组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[0];

				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				    autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				    autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				    autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				    autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																					//第一组,对应较亮的光线环境
				    autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
				    autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 255;//

				    //缺省参数
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				    autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				    autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				    autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				    autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

				    //第一组,对应较亮的光线环境
				    autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
				    autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 255;//
				}
				{
					//第二组自动校正参数	
				    AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[1];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第二组,对应适当的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 180;

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 180;
				}
				{
					//第三组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[2];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第三组, 对应较暗的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 120;

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间
																							 //第三组, 对应较暗的光线环境
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 120;
				}	
			}

			//自动屏蔽参数
			{
				AutoMaskSettings& autoMaskSettings = lens.autoMaskSettings;
				autoMaskSettings.cAutoMaskDetectThreshold = 180;
				autoMaskSettings.nMaskAreaEroseSize = DEFAULT_MASK_AREA_EROSIE_SIZE;

				autoMaskSettings.cAutoMaskDetectThresholdDefault = 180;
				autoMaskSettings.nMaskAreaEroseSizeDefault = DEFAULT_MASK_AREA_EROSIE_SIZE;
			}

			//镜头规格
			{
				TLensSpecification& lensSpecification = lens.lensSpecification;
				lensSpecification.backFocalLength = 3.4;//像方焦距,   单位:mm
				lensSpecification.FOV_horz = 0;//水平视角，  单位:度
				lensSpecification.FOV_vert = 0;//垂直视角，  单位:度
				lensSpecification.FOV_diagonal = 70;//对角线视角，单位:度
				lensSpecification.throwRatio = 0.55;//投射比
			}

			//自动校正补偿系数
			{
				TAutoCalibCompCoefs& autoCalibCompCoefs = lens.autoCalibCompCoefs;

				//{{2017/12/30
				//u0 = 385.100857, v0=183.523701, k1=4.909888, k2=-8.764274, k3=16.234063
				autoCalibCompCoefs.u0 = 385.100857 / 640.0;
				autoCalibCompCoefs.v0 = 183.523701 / 480.0;

				autoCalibCompCoefs.k[0] = 4.909888;
				autoCalibCompCoefs.k[1] = -8.764274;
				autoCalibCompCoefs.k[2] = 16.234063;
				//2017/12/30>>
			}

		}//投射比0.55镜头


		 //投射比0.7镜头的参数配置
		 //==================================
		{
			TLensConfig&  lens = lensConfigs[E_LENS_TR_0_DOT_70];

			//电子白板正常手指触控模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光時間

				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光時間
			}


			//电子白板正常笔触触控模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PenTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光時間

				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間

			}

			//手指触控正常模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光時間

				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間

			}
			//手掌互动正常模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PalmTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光時間

				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間
			}


			//安装调试模式下的参数
			{
				InstallTunningSettings& installTunningSettings = lens.installTunningSettings;
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				installTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光時間

																					//缺省值初始化
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补
				installTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間
			}

			//电子白板激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_WhiteBoard;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 180;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光時間

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 180;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光時間
			}

			//手指触控激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_FingerTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光時間

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光時間
			}

			//手掌触控激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_PalmTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光時間

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光時間
			}

			//自动校正参数
			{
				AutoCalibrateSettingsList& autoCalibrateSettingsList = lens.autoCalibrateSettingsList;
				{
					//第一组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[0];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第一组,对应较亮的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 255;//

																								 //缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

																							 //第一组,对应较亮的光线环境
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					//calibrateImageParamsList[0].autoCalibrateHilightGray        = 180;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 255;//
				}
				{

					//第二组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[1];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第二组,对应适当的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 180;

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 180;
				}
				{
					//第三组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[2];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第三组, 对应较暗的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 120;

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

																							 //第三组, 对应较暗的光线环境
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 120;
				}
			}
			//自动屏蔽参数
			{
				AutoMaskSettings& autoMaskSettings = lens.autoMaskSettings;
				autoMaskSettings.cAutoMaskDetectThreshold = 180;
				autoMaskSettings.nMaskAreaEroseSize = DEFAULT_MASK_AREA_EROSIE_SIZE;

				autoMaskSettings.cAutoMaskDetectThresholdDefault = 180;
				autoMaskSettings.nMaskAreaEroseSizeDefault = DEFAULT_MASK_AREA_EROSIE_SIZE;
			}

			//镜头规格
			{
				TLensSpecification& lensSpecification = lens.lensSpecification;
				lensSpecification.backFocalLength = 2.7;//像方焦距,   单位:mm
				lensSpecification.FOV_horz = 146;//水平视角，  单位:度
				lensSpecification.FOV_vert = 105;//垂直视角，  单位:度
				lensSpecification.FOV_diagonal = 175;//对角线视角，单位:度
				lensSpecification.throwRatio = 0.7;//投射比
			}

			//自动校正补偿系数
			{
				TAutoCalibCompCoefs& autoCalibCompCoefs = lens.autoCalibCompCoefs;
				autoCalibCompCoefs.u0 = 320.0 / 640.0;
				autoCalibCompCoefs.v0 = 240.0 / 480.0;

				autoCalibCompCoefs.k[0] = 0.0;
				autoCalibCompCoefs.k[1] = 0.0;
				autoCalibCompCoefs.k[2] = 0.0;
			}

		}//投射比0.7镜头


		 //投射比0.34镜头参数
		 //==================================
		{
			TLensConfig&  lens = lensConfigs[E_LENS_TR_0_DOT_34];

			//电子白板正常手指触控模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 100;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光時間


																				 //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 100;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間
			}


			//电子白板正常笔触触控模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PenTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光時間

				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間
			}

			//手指触控正常模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光時間

																				  //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間
			}

			//手掌互动正常模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PalmTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光時間

																				  //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間
			}

			//安装调试模式下的参数
			{
				InstallTunningSettings& installTunningSettings = lens.installTunningSettings;
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				installTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光時間

																					//缺省值初始化
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补
				installTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間
			}

			//电子白板激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_WhiteBoard;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 130;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光時間

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 130;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間
			}

			//手指触控激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_FingerTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光時間

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間
			}

			//手掌互动激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_PalmTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7; //曝光時間

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;//曝光時間
			}

			//自动校正参数
			{
				AutoCalibrateSettingsList& autoCalibrateSettingsList = lens.autoCalibrateSettingsList;
				{
				    //第一组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[0];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第一组,对应较亮的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 255;//

																								 //缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

																							 //第一组,对应较亮的光线环境
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					//calibrateImageParamsList[0].autoCalibrateHilightGray        = 180;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 255;//
				}
				{
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[1];

	               //第二组自动校正参数
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第二组,对应适当的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 180;

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 180;

				}
				{
				   //第三组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[2];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第三组, 对应较暗的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 120;

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

																							 //第三组, 对应较暗的光线环境
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 120;
				}
			}
		
			//自动屏蔽参数
			{
				AutoMaskSettings& autoMaskSettings = lens.autoMaskSettings;
				autoMaskSettings.cAutoMaskDetectThreshold = 180;
				autoMaskSettings.nMaskAreaEroseSize = DEFAULT_MASK_AREA_EROSIE_SIZE;

				autoMaskSettings.cAutoMaskDetectThresholdDefault = 180;
				autoMaskSettings.nMaskAreaEroseSizeDefault = DEFAULT_MASK_AREA_EROSIE_SIZE;
			}

			//镜头规格
			{
				TLensSpecification& lensSpecification = lens.lensSpecification;
				lensSpecification.backFocalLength = 2.9;//像方焦距,   单位:mm
				lensSpecification.FOV_horz = 146;//水平视角，  单位:度 (X)
				lensSpecification.FOV_vert = 105;//垂直视角，  单位:度 (X)
				lensSpecification.FOV_diagonal = 160;//对角线视角，单位:度
				lensSpecification.throwRatio = 0.34;//投射比
			}

			//自动校正补偿系数
			{
				TAutoCalibCompCoefs& autoCalibCompCoefs = lens.autoCalibCompCoefs;

				//u0=265.664339,v0=280.949538,k1=7.626064,k2=6.223058,k3=-6.596766
				//u0=273.591450,v0=277.587269,k1=7.590502,k2=6.671639,k3=-6.028019
				//u0=277.466887,v0=276.712032,k1=7.605442,k2=6.669648,k3=-5.912612
				//autoCalibCompCoefs.u0 = 277.466887/640.0;
				//autoCalibCompCoefs.v0 = 276.712032/480.0;

				//autoCalibCompCoefs.k[0] = 7.605442;
				//autoCalibCompCoefs.k[1] = 6.669648;
				//autoCalibCompCoefs.k[2] =-5.912612;


				//2017/05/25,仓库测试数据
				//u0=238.687308,v0=211.182170,k1=8.360741,k2=-12.416823,k3=40.916424,u0_normalized=3.729489e-01, v0_normalized=4.399629e-01

				//2017/05/25,公司测试得到的数据
				//u0=220.894547,v0=304.831558,k1=7.769802,k2=10.034001,k3=-20.034688,u0_normalized=3.451477e-01, v0_normalized=6.350657e-01

				/*
				//{{2017/05/25,公司测得的数据和仓库测试得到的数据混合计算出的拟合数据
				//u0=230.570401,v0=280.161057,k1=7.994895,k2=5.084500,k3=-6.767159,u0_normalized=3.602663e-01, v0_normalized=5.836689e-01
				autoCalibCompCoefs.u0 = 230.570401/640.0;
				autoCalibCompCoefs.v0 = 280.161057/480.0;

				autoCalibCompCoefs.k[0] =   7.994895;
				autoCalibCompCoefs.k[1] =   5.084500;
				autoCalibCompCoefs.k[2] = - 6.767159;
				//2017/05/25,}}
				*/
#ifdef USING_OLD_IRCUT
				//{{2017/06/29
				//旧的IRCut,外贸客户
				//u0=308.268396,v0=254.378663,k1=8.573092,k2=4.075745,k3=-6.104614,u0_normalized=4.816694e-01, v0_normalized=5.299555e-01
				autoCalibCompCoefs.u0 = 308.268396 / 640.0;
				autoCalibCompCoefs.v0 = 254.378663 / 480.0;

				autoCalibCompCoefs.k[0] = 8.573092;
				autoCalibCompCoefs.k[1] = 4.075745;
				autoCalibCompCoefs.k[2] = -6.104614;
				//2017/06/29}}

#else
				//{{2017/06/29
				//u0=287.199525,v0=235.642519,k1=9.211478,k2=-7.315840,k3=23.628175,u0_normalized=4.487493e-01, v0_normalized=4.909219e-01
				/*
				autoCalibCompCoefs.u0 = 287.199525/640.0;
				autoCalibCompCoefs.v0 = 235.642519/480.0;

				autoCalibCompCoefs.k[0] =  9.211478;
				autoCalibCompCoefs.k[1] = -7.315840;
				autoCalibCompCoefs.k[2] = 23.628175;
				*/
				//2017/06/29}}

				//{{2017/12/30
				//u0 = 340.736850, v0 = 232.898638, k1 = 9.570193, k2 = -9.567076, k3 = 22.851639, u0_normalized = 5.324013e-01, v0_normalized = 4.852055e-01
				autoCalibCompCoefs.u0 = 340.736850 / 640.0;
				autoCalibCompCoefs.v0 = 232.898638 / 480.0;

				autoCalibCompCoefs.k[0] = 9.570193;
				autoCalibCompCoefs.k[1] = -9.567076;
				autoCalibCompCoefs.k[2] = 22.851639;
				//2017/12/30}}
#endif
			}

		}//投射比0.34镜头的参数配置

		 //投射比0.28镜头参数
		 //==================================
		{
			TLensConfig&  lens = lensConfigs[E_LENS_TR_0_DOT_28];

			//电子白板正常手指触控模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;

																				 //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//电子白板正常笔触触控模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PenTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;

																				 //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//手指触控正常模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//手掌互动正常模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PalmTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}


			//安装调试模式下的参数
			{
				InstallTunningSettings& installTunningSettings = lens.installTunningSettings;
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				installTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

																					//缺省值初始化
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补
				installTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//电子白板激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_WhiteBoard;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 130;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 130;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//手指触控激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_FingerTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//手掌互动激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_PalmTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//第一组自动校正参数
			{
				AutoCalibrateSettingsList& autoCalibrateSettingsList = lens.autoCalibrateSettingsList;
				{
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[0];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第一组,对应较亮的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 255;//

																								 //缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

																							 //第一组,对应较亮的光线环境
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					//calibrateImageParamsList[0].autoCalibrateHilightGray        = 180;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 255;//
				}
				{
					//第二组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[1];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第二组,对应适当的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 180;

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 180;

				}
				{
					//第三组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[2];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

					//第三组, 对应较暗的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 120;

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

					 //第三组, 对应较暗的光线环境
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 120;
				}
			}
			//自动屏蔽参数
			{
				AutoMaskSettings& autoMaskSettings = lens.autoMaskSettings;
				autoMaskSettings.cAutoMaskDetectThreshold = 180;
				autoMaskSettings.nMaskAreaEroseSize = DEFAULT_MASK_AREA_EROSIE_SIZE;

				autoMaskSettings.cAutoMaskDetectThresholdDefault = 180;
				autoMaskSettings.nMaskAreaEroseSizeDefault = DEFAULT_MASK_AREA_EROSIE_SIZE;
			}

			//镜头规格
			{
				TLensSpecification& lensSpecification = lens.lensSpecification;
				lensSpecification.backFocalLength = 2.3;//像方焦距,   单位:mm
				lensSpecification.FOV_horz = 146;//水平视角，  单位:度 (X)
				lensSpecification.FOV_vert = 105;//垂直视角，  单位:度 (X)
				lensSpecification.FOV_diagonal = 170;//对角线视角，单位:度
				lensSpecification.throwRatio = 0.28;//投射比
			}

			//自动校正补偿系数
			{
				TAutoCalibCompCoefs& autoCalibCompCoefs = lens.autoCalibCompCoefs;

#ifdef HUAN_SUO
				//u0=274.023758,v0=260.265300,k1=5.584781,k2=-5.433567,k3=10.572886,u0_normalized=4.281621e-01, v0_normalized=5.422194e-01
				autoCalibCompCoefs.u0 = 274.023758 / 640.0;
				autoCalibCompCoefs.v0 = 260.265300 / 480.0;

				autoCalibCompCoefs.k[0] = 5.584781;
				autoCalibCompCoefs.k[1] = -5.433567;
				autoCalibCompCoefs.k[2] = 10.572886;
#else           //EASI

				//u0=294.082407,v0=265.407011,k1=4.352066,k2=10.878342,k3=-7.466683
				//autoCalibCompCoefs.u0 = 294.082407/640.0;
				//autoCalibCompCoefs.v0 = 265.407011/480.0;

				//autoCalibCompCoefs.k[0] = 4.352066;
				//autoCalibCompCoefs.k[1] = 10.878342;
				//autoCalibCompCoefs.k[2] =-7.466683;


				//u0=299.983367,v0=282.278932,k1=5.116665,k2=6.163398,k3=-6.14878
				/*
				autoCalibCompCoefs.u0 = 299.983367/640.0;
				autoCalibCompCoefs.v0 = 282.278932/480.0;

				autoCalibCompCoefs.k[0] =   5.116665;
				autoCalibCompCoefs.k[1] =   6.163398;
				autoCalibCompCoefs.k[2] = - 6.14878 ;
				*/
#ifdef OEM_JINYANWEI
				//{{2017/07/25
				//u0 = 339.113301, v0 = 278.019209, k1 = 7.751777, k2 = -16.946824, k3 = 41.379675, u0_normalized = 5.298645e-01, v0_normalized = 5.792067e-01
				//autoCalibCompCoefs.u0 = 339.113301 / 640.0;
				//autoCalibCompCoefs.v0 = 278.019209 / 480.0;

				//autoCalibCompCoefs.k[0] = 7.751777  ;
				//autoCalibCompCoefs.k[1] = -16.946824;
				//autoCalibCompCoefs.k[2] = 41.379675 ;
				//2017/07/25}}



				//u0 = 342.214767, v0 = 276.107598, k1 = 7.122529, k2 = -7.919495, k3 = 16.457586, u0_normalized = 5.347106e-01, v0_normalized = 5.752242e-01
				//autoCalibCompCoefs.u0 = 342.214767 / 640.0;
				//autoCalibCompCoefs.v0 = 276.107598 / 480.0;

				//autoCalibCompCoefs.k[0] = 7.122529 ;
				//autoCalibCompCoefs.k[1] = -7.919495;
				//autoCalibCompCoefs.k[2] = 16.457586;


				//u0 = 342.389807, v0 = 274.348021, k1 = 7.150992, k2 = -8.307416, k3 = 17.769362, u0_normalized = 5.349841e-01, v0_normalized = 5.715584e-01
				//autoCalibCompCoefs.u0 = 342.389807 / 640.0;
				//autoCalibCompCoefs.v0 = 274.348021 / 480.0;

				//autoCalibCompCoefs.k[0] = 7.150992;
				//autoCalibCompCoefs.k[1] = -8.307416;
				//autoCalibCompCoefs.k[2] = 17.769362;

				//u0 = 357.436004, v0 = 249.212047, k1 = 7.703376, k2 = -15.953633, k3 = 35.270832, u0_normalized = 5.584938e-01, v0_normalized = 5.191918e-01
				autoCalibCompCoefs.u0 = 357.436004 / 640.0;
				autoCalibCompCoefs.v0 = 249.212047 / 480.0;

				autoCalibCompCoefs.k[0] = 7.703376;
				autoCalibCompCoefs.k[1] = -15.953633;
				autoCalibCompCoefs.k[2] = 35.270832;


#elif USING_OLD_IRCUT

				//{{2017/06/29
				//旧的IRCut, 外贸客户
				//u0=331.896164,v0=283.044131,k1=6.420449,k2=-1.347495,k3=4.920541,u0_normalized=5.185878e-01, v0_normalized=5.896753e-01
				autoCalibCompCoefs.u0 = 331.896164 / 640.0;
				autoCalibCompCoefs.v0 = 283.044131 / 480.0;

				autoCalibCompCoefs.k[0] = 6.420449;
				autoCalibCompCoefs.k[1] = -1.347495;
				autoCalibCompCoefs.k[2] = 4.920541;
				//2017/06/29}}

#else
				//{{2017/06/29
				//u0=317.041971,v0=241.752276,k1=5.897884,k2=-1.917680,k3=7.821563,u0_normalized=4.953781e-01, v0_normalized=5.036506e-01
				//autoCalibCompCoefs.u0 = 317.041971/640.0;
				//autoCalibCompCoefs.v0 = 241.752276/480.0;

				//autoCalibCompCoefs.k[0] =   5.897884;
				//autoCalibCompCoefs.k[1] =  -1.917680;
				//autoCalibCompCoefs.k[2] =  7.821563 ;
				//2017/06/29}}


				//{{2017/09/09
				//u0 = 284.109902, v0 = 220.914089, k1 = 6.112229, k2 = -4.134430, k3 = 16.971633, u0_normalized = 4.439217e-01, v0_normalized = 4.602377e-01
				//autoCalibCompCoefs.u0 = 284.109902 /640.0;
				//autoCalibCompCoefs.v0 = 220.914089 /480.0;

				//autoCalibCompCoefs.k[0] = 6.112229;
				//autoCalibCompCoefs.k[1] = -4.134430;
				//autoCalibCompCoefs.k[2] = 16.971633;
				//2017/09/09}}

				//{{2017/12/25
				//u0 = 303.702573, v0 = 218.858777, k1 = 7.062936, k2 = -17.169107, k3 = 42.405851, u0_normalized = 4.745353e-01, v0_normalized = 4.559558e-01
				autoCalibCompCoefs.u0 = 303.702573 / 640.0;
				autoCalibCompCoefs.v0 = 218.858777 / 480.0;

				autoCalibCompCoefs.k[0] = 7.062936;
				autoCalibCompCoefs.k[1] = -17.169107;
				autoCalibCompCoefs.k[2] = 42.405851;
				//2017/12/25}}
#endif

#endif
			}

		}//投射比0.28镜头的参数配置

		 //投射比0.25镜头参数
		 //==================================
		{
			TLensConfig&  lens = lensConfigs[E_LENS_TR_0_DOT_25];

			//电子白板正常手指触控模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;


																				 //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//电子白板正常笔触触控模式下的使用参数
			{

				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PenTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;


																				 //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;

			}

			//手指触控正常模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;


				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//手掌互动正常模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PalmTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;


				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//安装调试模式下的参数
			{
				InstallTunningSettings& installTunningSettings = lens.installTunningSettings;
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				installTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

																					//缺省值初始化
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补
				installTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//电子白板激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_WhiteBoard;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 130;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 130;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//手指触控激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_FingerTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//手掌互动激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_PalmTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//第一组自动校正参数
			{
				AutoCalibrateSettingsList& autoCalibrateSettingsList = lens.autoCalibrateSettingsList;
				{
					//第一组自动校正参数
				    AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[0];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第一组,对应较亮的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 255;//

																								 //缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

																							 //第一组,对应较亮的光线环境
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 255;//
				}
				{

					//第二组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[1];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第二组,对应适当的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 180;

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 180;
				}
				{
					//第三组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[2];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第三组, 对应较暗的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 120;

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

																							 //第三组, 对应较暗的光线环境
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 120;
				}
			}
			//自动屏蔽参数
			{
				AutoMaskSettings& autoMaskSettings = lens.autoMaskSettings;
				autoMaskSettings.cAutoMaskDetectThreshold = 180;
				autoMaskSettings.nMaskAreaEroseSize = DEFAULT_MASK_AREA_EROSIE_SIZE;

				autoMaskSettings.cAutoMaskDetectThresholdDefault = 180;
				autoMaskSettings.nMaskAreaEroseSizeDefault = DEFAULT_MASK_AREA_EROSIE_SIZE;
			}

			//镜头规格
			{
				TLensSpecification& lensSpecification = lens.lensSpecification;
				lensSpecification.backFocalLength = 1.46;//像方焦距,   单位:mm
				lensSpecification.FOV_horz = 126;//水平视角，  单位:度
				lensSpecification.FOV_vert = 100;//垂直视角，  单位:度
				lensSpecification.FOV_diagonal = 160;//对角线视角，单位:度
				lensSpecification.throwRatio = 0.25;//投射比
			}

			//自动校正补偿系数
			{
				TAutoCalibCompCoefs& autoCalibCompCoefs = lens.autoCalibCompCoefs;
				//u0=335.073503,v0=278.260420,k1=2.899607,k2=-2.526796,k3=2.967799
				//小镜头，无切换器版本

				autoCalibCompCoefs.u0 = 335.073503 / 640.0;
				autoCalibCompCoefs.v0 = 278.260420 / 480.0;

				autoCalibCompCoefs.k[0] = 2.899607;
				autoCalibCompCoefs.k[1] = -2.526796;
				autoCalibCompCoefs.k[2] = 2.967799;
			}

		}//投射比0.25镜头的参数配置

		 //投射比0.21镜头的参数配置
		 //==================================
		{
			TLensConfig&  lens = lensConfigs[E_LENS_TR_0_DOT_21];

			//电子白板正常手指触控模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;


																				 //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}


			//电子白板正常笔触触控模式下的使用参数
			{

				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PenTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;


				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;

			}

			//手指触控正常模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;


				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;

			}
			//手掌互动正常模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PalmTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;


				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;

			}

			//安装调试模式下的参数
			{
				InstallTunningSettings& installTunningSettings = lens.installTunningSettings;
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				installTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

																					//缺省值初始化
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补
				installTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//电子白板激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_WhiteBoard;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//手指触控激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_FingerTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//手掌互动激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_PalmTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}
			{
				AutoCalibrateSettingsList& autoCalibrateSettingsList = lens.autoCalibrateSettingsList;
				{
					//第一组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[0];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第一组,对应较亮的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 255;//

																								 //缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

																							 //第一组,对应较亮的光线环境
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 255;//
				}
				{
					//第二组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[1];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第二组,对应适当的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 180;

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 180;
				}
				{
					//第三组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[2];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第三组, 对应较暗的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 120;

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

																							 //第三组, 对应较暗的光线环境
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 120;
				}
			}
			//自动屏蔽参数
			{
				AutoMaskSettings& autoMaskSettings = lens.autoMaskSettings;
				autoMaskSettings.cAutoMaskDetectThreshold = 180;
				autoMaskSettings.nMaskAreaEroseSize = DEFAULT_MASK_AREA_EROSIE_SIZE;

				autoMaskSettings.cAutoMaskDetectThresholdDefault = 180;
				autoMaskSettings.nMaskAreaEroseSizeDefault = DEFAULT_MASK_AREA_EROSIE_SIZE;
			}

			//镜头规格
			{
				TLensSpecification& lensSpecification = lens.lensSpecification;
				lensSpecification.backFocalLength = 3.71;//像方焦距,   单位:mm
				lensSpecification.FOV_horz = 146;//水平视角，  单位:度
				lensSpecification.FOV_vert = 105;//垂直视角，  单位:度
				lensSpecification.FOV_diagonal = 170;//对角线视角，单位:度
				lensSpecification.throwRatio = 0.21;//投射比
			}

			//自动校正补偿系数
			{
				TAutoCalibCompCoefs& autoCalibCompCoefs = lens.autoCalibCompCoefs;

				//u0=319.663522,v0=255.926539,k1=6.781283,k2=-14.577119,k3=66.789126
				//u0=319.001013,v0=262.873086,k1=6.995382,k2=-15.779594,k3=68.493177
				//autoCalibCompCoefs.u0 = 319.001013/640.0;
				//autoCalibCompCoefs.v0 = 262.873086/480.0;

				//autoCalibCompCoefs.k[0] =   6.995382;
				//autoCalibCompCoefs.k[1] = -15.779594;
				//autoCalibCompCoefs.k[2] = 68.493177;


				//u0=274.873459,v0=282.960636,k1=8.286172,k2=-23.967977,k3=85.958584
				/*
				autoCalibCompCoefs.u0 = 274.873459/640.0;
				autoCalibCompCoefs.v0 = 282.960636/480.0;

				autoCalibCompCoefs.k[0] =    8.286172;
				autoCalibCompCoefs.k[1] = - 23.967977;
				autoCalibCompCoefs.k[2] =   85.958584;
				*/


				//u0=292.052986,v0=215.368079,k1=7.454406,k2=-23.591264,k3=85.094341,u0_normalized=4.563328e-01, v0_normalized=4.486835e-01
				//{{2017/06/29
				/*
				autoCalibCompCoefs.u0 = 292.052986/640.0;
				autoCalibCompCoefs.v0 = 215.368079/480.0;

				autoCalibCompCoefs.k[0] =    7.454406;
				autoCalibCompCoefs.k[1] = - 23.591264;
				autoCalibCompCoefs.k[2] =    5.094341;
				*/
				//2017/06/29}}
				//u0 = 352.294764, v0 = 188.394952, k1 = 8.864434, k2 = -29.905298, k3 = 82.983196, u0_normalized = 5.504606e-01, v0_normalized = 3.924895e-01
				//{{2017/12/30
				autoCalibCompCoefs.u0 = 352.294764 / 640.0;
				autoCalibCompCoefs.v0 = 188.394952 / 480.0;

				autoCalibCompCoefs.k[0] = 8.864434;
				autoCalibCompCoefs.k[1] = -29.905298;
				autoCalibCompCoefs.k[2] = 82.983196;
				//2017/12/30}}
			}

		}//投射比0.21镜头参数的配置

		 //投射比0.19镜头的参数配置
		 //==================================
		{
			TLensConfig&  lens = lensConfigs[E_LENS_TR_0_DOT_19];

			//电子白板正常手指触控模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;


																				 //缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//电子白板正常笔触触控模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PenTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;


				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;

			}

			//手指触控正常模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;


				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;

			}

			//手掌互动正常模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PalmTouchControl;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;


				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;

			}

			//安装调试模式下的参数
			{
				InstallTunningSettings& installTunningSettings = lens.installTunningSettings;
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				installTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

																					//缺省值初始化
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补
				installTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//电子白板激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_WhiteBoard;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;


				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//手指触控激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_FingerTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 0;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;


				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//手掌互动激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_PalmTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 0;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;


				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}
			{
				AutoCalibrateSettingsList& autoCalibrateSettingsList = lens.autoCalibrateSettingsList;
				{
			        //第一组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[0];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第一组,对应较亮的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 255;//

																								 //缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

																							 //第一组,对应较亮的光线环境
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 255;//
				} 
				{
					//第二组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[1];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第二组,对应适当的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 180;

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 180;
				}
				{
					//第三组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[2];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第三组, 对应较暗的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 120;

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

																							 //第三组, 对应较暗的光线环境
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 120;
				}
			}

			//自动屏蔽参数
			{
				AutoMaskSettings& autoMaskSettings = lens.autoMaskSettings;
				autoMaskSettings.cAutoMaskDetectThreshold = 180;
				autoMaskSettings.nMaskAreaEroseSize = DEFAULT_MASK_AREA_EROSIE_SIZE;

				autoMaskSettings.cAutoMaskDetectThresholdDefault = 180;
				autoMaskSettings.nMaskAreaEroseSizeDefault = DEFAULT_MASK_AREA_EROSIE_SIZE;
			}

			//镜头规格
			{
				TLensSpecification& lensSpecification = lens.lensSpecification;
				lensSpecification.backFocalLength = 2.2;//像方焦距,   单位:mm
				lensSpecification.FOV_horz = 146;//水平视角，  单位:度
				lensSpecification.FOV_vert = 105;//垂直视角，  单位:度
				lensSpecification.FOV_diagonal = 175;//对角线视角，单位:度
				lensSpecification.throwRatio = 0.19;//投射比
			}

			//自动校正补偿系数
			{
				TAutoCalibCompCoefs& autoCalibCompCoefs = lens.autoCalibCompCoefs;

				//u0 = 330.738453, v0 = 298.012115, k1 = 6.196995, k2 = -0.561616, k3 = 1.478723,
				autoCalibCompCoefs.u0 = 330.738453 / 640.0;
				autoCalibCompCoefs.v0 = 298.012115 / 480.0;

				autoCalibCompCoefs.k[0] = 6.196995;
				autoCalibCompCoefs.k[1] = -0.561616;
				autoCalibCompCoefs.k[2] = 1.478723;

			}

		}//投射比0.19镜头参数的配置

		 //投射比为0.15的超短焦镜头的参数配置
		 //==================================
		{
			TLensConfig&  lens = lensConfigs[E_LENS_TR_0_DOT_15];
			//电子白板正常手指触控模式下的使用参数
			{
				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 205;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma     ★
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 205;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma    ★
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}
			//电子白板正常笔触触控模式下的使用参数
			{

				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PenTouchWhiteBoard;
				normalUsageSettings.cYThreshold = 225;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 225;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 60;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 255;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}
			//手指触控正常模式下的使用参数
			{

				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_FingerTouchControl;
				normalUsageSettings.cYThreshold = 225;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 225;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 46;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}
			//手掌互动正常模式下的使用参数
			{

				NormalUsageSettings& normalUsageSettings = lens.normalUsageSettings_PalmTouchControl;
				normalUsageSettings.cYThreshold = 225;
				normalUsageSettings.nLightSpotMinimumWidth = 2;
				normalUsageSettings.nLightSpotMinimumHeight = 2;

				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				//缺省值初始化
				normalUsageSettings.cYThresholdDefault = 225;
				normalUsageSettings.nLightSpotMinimumWidthDefault = 2;
				normalUsageSettings.nLightSpotMinimumHeightDefault = 2;

				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Contrast = 46;//对比度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma    ★
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				normalUsageSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				normalUsageSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				normalUsageSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				normalUsageSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}


			//安装调试模式下的参数
			{
				InstallTunningSettings& installTunningSettings = lens.installTunningSettings;
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				installTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

																					//缺省值初始化
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				installTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
				installTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				installTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				installTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补
				installTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
				installTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//电子白板激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_WhiteBoard;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//手指触控激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_FingerTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 0;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 38;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 4600;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//手掌互动激光器调试模式参数
			{
				LaserTunningSettings& laserTunningSettings = lens.laserTunningSettings_PalmTouchControl;

				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
				laserTunningSettings.cameraParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.cameraParams.Prop_CameraControl_Exposure = -7;

				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Brightness = 150;//亮度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Contrast = 150;//对比度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_Gamma = 2;//Gamma
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
				laserTunningSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
				laserTunningSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿 偿
				laserTunningSettings.defaultParams.Prop_VideoProcMap_Gain = 0x44AA;//增益
				laserTunningSettings.defaultParams.Prop_CameraControl_Exposure = -7;
			}

			//自动校正参数列表
			{
				AutoCalibrateSettingsList& autoCalibrateSettingsList = lens.autoCalibrateSettingsList;
				{
					//第一组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[0];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

																						   //第一组,对应较亮的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 255;//

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

					//第一组,对应较亮的光线环境
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 255;//
				}
				{
					//第二组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[1];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

					//第二组,对应适当的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 180;

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 100;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 180;
				}
				{
					//第三组自动校正参数
					AutoCalibrateSettings &autoCalibrateSettings = autoCalibrateSettingsList[2];

					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.cameraParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.cameraParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.cameraParams.Prop_CameraControl_Exposure = -7;//曝光时间

					//第三组, 对应较暗的光线环境
					autoCalibrateSettings.calibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autoCalibrateSettings.calibrateImageParams.autoCalibrateHilightGray = 120;

					//缺省参数
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Brightness = 120;//亮度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Contrast = 50;//对比度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Hue = 0;//色调
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Satuation = 64;//饱和度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Sharpness = 1;//锐利度
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_Gamma = 1;//Gamma
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_ColorEnable = 0;//颜色
					autoCalibrateSettings.defaultParams.Prop_VideoProcAmp_WhiteBalance = 5000;//白平衡
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_BacklightCompensation = 2;//背光补偿
					autoCalibrateSettings.defaultParams.Prop_VideoProcMap_Gain = 0x33AA;//增益
					autoCalibrateSettings.defaultParams.Prop_CameraControl_Exposure = -7; //曝光时间

					//第三组, 对应较暗的光线环境
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateExpectedBrightness = 70;
					autoCalibrateSettings.defaultCalibrateImageParams.autoCalibrateHilightGray = 120;
				}
			}
			//自动屏蔽参数
			{
				AutoMaskSettings& autoMaskSettings = lens.autoMaskSettings;
				autoMaskSettings.cAutoMaskDetectThreshold = 180;
				autoMaskSettings.nMaskAreaEroseSize = DEFAULT_MASK_AREA_EROSIE_SIZE;

				autoMaskSettings.cAutoMaskDetectThresholdDefault = 180;
				autoMaskSettings.nMaskAreaEroseSizeDefault = DEFAULT_MASK_AREA_EROSIE_SIZE;
			}
			//镜头规格
			{
				TLensSpecification& lensSpecification = lens.lensSpecification;
				lensSpecification.backFocalLength = 2.7;//像方焦距,   单位:mm
				lensSpecification.FOV_horz = 146;//水平视角，  单位:度
				lensSpecification.FOV_vert = 105;//垂直视角，  单位:度
				lensSpecification.FOV_diagonal = 175;//对角线视角，单位:度
				lensSpecification.throwRatio = 0.15;//投射比
			}

			//自动校正补偿系数
			{
				TAutoCalibCompCoefs& autoCalibCompCoefs = lens.autoCalibCompCoefs;

				//易视
				//u0=282.490377,v0=268.399291,k1=4.298340,k2=-5.262811,k3=15.972059
				/*
				autoCalibCompCoefs.u0   = 282.490377/640.0;
				autoCalibCompCoefs.v0   = 268.399291/480.0;

				autoCalibCompCoefs.k[0] =  4.298340;
				autoCalibCompCoefs.k[1] = -5.262811;
				autoCalibCompCoefs.k[2] =  15.972059;
				*/



				//0.15_easi_圣普豪威IRCUT
				//u0=307.497247,v0=250.872375,k1=7.026544,k2=-12.256803,k3=26.368668,u0/640=0.480464, v0/480=0.522651
				/*
				autoCalibCompCoefs.u0   = 307.497247/640.0;
				autoCalibCompCoefs.v0   = 250.872375/480.0;

				autoCalibCompCoefs.k[0] =   7.026544;
				autoCalibCompCoefs.k[1] = -12.256803;
				autoCalibCompCoefs.k[2] =  26.368668;
				*/



				//{{2017/06/26
				/*
				//u0=304.335983,v0=222.927900,k1=8.057596,k2=-23.869677,k3=55.731360
				autoCalibCompCoefs.u0   = 304.335983/640.0;
				autoCalibCompCoefs.v0   = 222.927900/480.0;

				autoCalibCompCoefs.k[0] =   8.057596;
				autoCalibCompCoefs.k[1] = -23.869677;
				autoCalibCompCoefs.k[2] =  55.731360;
				*/
				//2017/06/26}}

				//u0=310.135680,v0=222.126442,k1=7.458371,k2=-14.359894,k3=26.969461
				//{{2017/07/03
				/*
				autoCalibCompCoefs.u0   = 310.135680/640.0;
				autoCalibCompCoefs.v0   = 222.126442/480.0;

				autoCalibCompCoefs.k[0] =   7.458371;
				autoCalibCompCoefs.k[1] = -14.359894;
				autoCalibCompCoefs.k[2] =  26.969461;
				*/
				//2017/07/03}}

#ifdef CCD_ROTATE_90
				//u0 = 323.727170, v0 = 221.434748, k1 = 7.331750, k2 = -10.172566, k3 = 20.331750
				//autoCalibCompCoefs.u0 = 323.727170 / 640.0;
				//autoCalibCompCoefs.v0 = 221.434748 / 480.0;

				//autoCalibCompCoefs.k[0] = 7.331750;
				//autoCalibCompCoefs.k[1] = -10.172566;
				//autoCalibCompCoefs.k[2] = 20.331750;


				//u0 = 326.159754, v0 = 212.770324, k1 = 8.035979, k2 = -18.892648, k3 = 44.544014
				autoCalibCompCoefs.u0 = 326.159754 / 640.0;
				autoCalibCompCoefs.v0 = 212.770324 / 480.0;

				autoCalibCompCoefs.k[0] = 8.035979;
				autoCalibCompCoefs.k[1] = -18.892648;
				autoCalibCompCoefs.k[2] = 44.544014;
#else
				//{{2017/07/04
				//u0=312.583863,v0=220.238080,k1=7.706045,k2=-16.430888,k3=35.777384
				//autoCalibCompCoefs.u0 = 312.583863 / 640.0;
				//autoCalibCompCoefs.v0 = 220.238080 / 480.0;

				//autoCalibCompCoefs.k[0] = 7.706045;
				//autoCalibCompCoefs.k[1] = -16.430888;
				//autoCalibCompCoefs.k[2] = 35.777384;
				//20170704}}

				//IR_u0 = 318.520569, IR_v0 = 247.994945, IR_f = [8.272932, -25.641896, 48.363140
				/*
				autoCalibCompCoefs.u0 = 318.520569 / 640.0;
				autoCalibCompCoefs.v0 = 247.994945 / 480.0;

				autoCalibCompCoefs.k[0] = 8.272932;
				autoCalibCompCoefs.k[1] = -25.641896;
				autoCalibCompCoefs.k[2] = 48.363140;
				*/


				//2017/11/10
				//IR_u0 = 320.082542, IR_v0 = 247.716911, IR_f = [10.846422, -45.338132, 94.552169
				/*
				autoCalibCompCoefs.u0 = 320.082542 / 640.0;
				autoCalibCompCoefs.v0 = 247.716911 / 480.0;

				autoCalibCompCoefs.k[0] = 10.846422;
				autoCalibCompCoefs.k[1] = -45.338132;
				autoCalibCompCoefs.k[2] = 94.552169;
				*/

				//2017/11/11
				//IR_u0 = 341.632854, IR_v0 = 198.620807, IR_f = [7.319401, -15.724932, 27.581669
				/*
				autoCalibCompCoefs.u0 = 341.632854 / 640.0;
				autoCalibCompCoefs.v0 = 198.620807 / 480.0;

				autoCalibCompCoefs.k[0] = 7.319401;
				autoCalibCompCoefs.k[1] = -15.724932;
				autoCalibCompCoefs.k[2] = 27.581669;
				*/

				/*
				autoCalibCompCoefs.u0 = 323.629534 / 640.0;
				autoCalibCompCoefs.v0 = 214.344065 / 480.0;

				autoCalibCompCoefs.k[0] = 5.796761;
				autoCalibCompCoefs.k[1] = -3.625549;
				autoCalibCompCoefs.k[2] = 11.213195;
				*/


				//2017/11/22
				//u0 = 342.382245, v0 = 191.224036, u0 / 640 = 0.534972, v0 / 480 = 0.398383, k1 = 6.732897, k2 = -9.634419, k3 = 23.062782
				/*
				autoCalibCompCoefs.u0 = 342.382245 / 640.0;
				autoCalibCompCoefs.v0 = 191.224036 / 480.0;

				autoCalibCompCoefs.k[0] =  6.732897;
				autoCalibCompCoefs.k[1] = -9.634419;
				autoCalibCompCoefs.k[2] = 23.062782;

				//IR_f = [3.94102, -2.28788, 1.1788]
				autoCalibCompCoefs.k[0] =  3.94102;
				autoCalibCompCoefs.k[1] = -2.28788;
				autoCalibCompCoefs.k[2] =  1.1788;
				*/
				//2017/11/22>>
				//2018/01/11
				//IR_u0 = 321.239992, IR_v0 = 224.913703, IR_f = [5.012811, 12.998592, -18.748065]
				autoCalibCompCoefs.u0 = 321.239992 / 640.0;
				autoCalibCompCoefs.v0 = 224.913703 / 480.0;

				autoCalibCompCoefs.k[0] = 5.012811;
				autoCalibCompCoefs.k[1] = 12.998592;
				autoCalibCompCoefs.k[2] = -18.748065;
			  //2018/01/11
#endif

			}
			//
			lens.bInternalAndSymmetricDistortParamsIsValid = TRUE;

			//相机内部参数
			//mu = 1.22754, mv = 1.22628
			//u0 = 318.521, v0 = 247.995
			//k(1) = 202.660577, k(2) = -1.567474, k(3) = 4.009577, k(4) = -3.274776, k(5) = 0.924704
			lens.lensInternalAndSymmetricDistortParams.mu = 1.22754;
			lens.lensInternalAndSymmetricDistortParams.mv = 1.22628;
			lens.lensInternalAndSymmetricDistortParams.u0 = 318.521;
			lens.lensInternalAndSymmetricDistortParams.v0 = 247.995;

			//相机对称畸变参数
			lens.lensInternalAndSymmetricDistortParams.radialDistrt[0] = 202.660577;
			lens.lensInternalAndSymmetricDistortParams.radialDistrt[1] = -1.567474;
			lens.lensInternalAndSymmetricDistortParams.radialDistrt[2] = 4.009577;
			lens.lensInternalAndSymmetricDistortParams.radialDistrt[3] = -3.274776;
			lens.lensInternalAndSymmetricDistortParams.radialDistrt[4] = 0.924704;

		}//投射比为0.15镜头参数
    }
};

//传感器配置信息
struct TSensorConfig
{
    CAtlString       strFavoriteDevicePath  ; //优先选择的视频设备路径
    CAtlString       strFavoriteMediaType   ; //选择的视频格式名称
    ELensType        eSelectedLensType      ; //当前选中的镜头类型

    //关联屏幕信息
    BOOL             bAutoAttachMonitor  ;   //自动关联屏幕标志
    int              nAttachedMonitorId  ;   //Sensor手动设置的关联屏幕id
    EMonitorAreaType eMonitorAreaType    ;   //手动设置的关联屏幕区域类型
 
	std::vector<TSensorModeConfig>    vecSensorModeConfig ; //传感器的模式

    TSensorConfig()
    {
        //attachedMonitorIds.resize(1);
        //attachedMonitorIds[0] = 0;//缺省关联0号屏幕。
        strFavoriteDevicePath = _T("");
        strFavoriteMediaType  = _T("640 X 480 MJPG");

        //手触和笔触模式下的摄像头亮度系数是不同的.
       // normalUsageSettings_FingerTouchWhiteBoard.cameraParams.Prop_VideoProcAmp_Brightness = 150;
        //normalUsageSettings_PenTouchWhiteBoard   .cameraParams.Prop_VideoProcAmp_Brightness = 30 ;
        //当前选中的镜头类型
        eSelectedLensType = E_LENS_TR_0_DOT_34;
        //自动关联屏幕标志
        bAutoAttachMonitor = true;
        //Sensor手动设置的关联屏幕id
        nAttachedMonitorId = 0;//Sensor
        //手动设置的关联屏幕区域类型
        eMonitorAreaType = E_MONITOR_AREA_TYPE_FULLSCREEN;
		//////////用来存放墙面和桌面的参数Add by zhaown 2019.7.17
		vecSensorModeConfig.resize(2);

    }//构造函数

};


//系统配置信息
struct TSysConfigData
{
	GlobalSettings               globalSettings;//全局配置信息
    std::vector<TSensorConfig>  vecSensorConfig; //投影模式配置
	TSysConfigData()
	{
		vecSensorConfig.resize(2);
	}
};

//typedef std::vector<TSensorConfig> AllSensorsConfig;

_declspec(selectany) extern const VideoMediaType DEFAULT_VIDEO_FORMAT
=
{
    MEDIATYPE_Video,
    MEDIASUBTYPE_MJPG,
    FORMAT_VideoInfo,

    //union
    {
        {//
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            147456000u,
            0u,
            333333,
            {
                40,
                    640,
                    480,
                    1,
                    16,
                    844715353,
                    614400,
                    0,
                    0,
                    0,
                    0
            }
        }

    }
};

// \\?\USB#Vid_a088&Pid_082
#define DEV_ID_LENGTH 25
_declspec(selectany) extern const TCHAR* DEFAULT_DEV_IDS[] = {
    _T("\\\\?\\USB#Vid_a088&Pid_0820"),
    _T("\\\\?\\USB#Vid_0ac8&Pid_3450")

};

//@功能:载入配置文件
//@参数:lpszConfigFilePath, 配置文件的完整路路径
BOOL LoadConfig(LPCTSTR lpszConfigFilePath, TSysConfigData& sysCfgData,int PID=37254, int VID = 6380);

BOOL UpDateConfig(LPCTSTR lpszConfigFilePath, TSysConfigData& sysCfgData, int PID = 37254, int VID = 6380);
//@功能:保存配置文件
//@参数:lpszConfigFilePath, 配置文件的完成路径
BOOL SaveConfig(LPCTSTR lpszConfigFilePath, const TSysConfigData& sysCfgData);


extern TSysConfigData g_tSysCfgData;

#define SENSOR_NUMBER 2

