#include "stdafx.h"
//#include "headers.h"
#include "..\inc\MyAPI.h"

TSysConfigData g_tSysCfgData;

//@功能:保存屏幕布局数据
BOOL SaveConfig(LPCTSTR lpszConfigFilePath, const std::vector<TScreenLayout>& allScreenLayouts);

//@功能:载入所有的视频布局
BOOL LoadConfig(LPCTSTR lpszConfigFilePath, std::vector<TScreenLayout>& allScreenLayouts);

//@功能:载入相机参数
//@参数:pNode, 指向配置文件中<CameraParams>节点的指针
//      cameraParams, 输出参数， 相机参数
BOOL LoadConfig(TiXmlNode *pNode, TVideoProcAmpProperty& cameraParams)
{
    TiXmlNode* pChild=NULL;
    do
    {
        pChild = pNode->IterateChildren(pChild);

        if(NULL == pChild) break;

        const char* lpszElementName = pChild->Value();
        if(_stricmp(lpszElementName, "Param") == 0)
        {
            const char* paramName  = ((TiXmlElement*)pChild)->Attribute("name");
            const char* paramValue = ((TiXmlElement*)pChild)->Attribute("value");


            if(paramName && paramValue && _stricmp(paramName, "Brightness") == 0)
            {//亮度
                cameraParams.Prop_VideoProcAmp_Brightness = atoi(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "Contrast") == 0)
            {//对比度
                cameraParams.Prop_VideoProcAmp_Contrast = atoi(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "Hue") == 0)
            {//色调
                cameraParams.Prop_VideoProcAmp_Hue = atoi(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "Saturation") == 0)
            {//饱和度
                cameraParams.Prop_VideoProcAmp_Satuation = atoi(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "Sharpness") == 0)
            {//清晰度
                cameraParams.Prop_VideoProcAmp_Sharpness = atoi(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "Gamma") == 0)
            {//伽玛值
                cameraParams.Prop_VideoProcAmp_Gamma = atoi(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "ColorEnable") == 0)
            {//颜色启动
                cameraParams.Prop_VideoProcAmp_ColorEnable = atoi(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "WhiteBalance") == 0)
            {//白平衡
                cameraParams.Prop_VideoProcAmp_WhiteBalance = atoi(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "BacklightCompensation") == 0)
            {//背光补偿
                cameraParams.Prop_VideoProcMap_BacklightCompensation = atoi(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "Gain") == 0)
            {//增益
                cameraParams.Prop_VideoProcMap_Gain = atoi(paramValue);
            }
        }

    }while(pChild);

    return TRUE;
}



//@功能:载入相机参数
//@参数:pNode, 指向配置文件中<CameraParams>节点的指针
//      cameraParams, 输出参数， 相机参数
//      defaultParams, 输出参数，缺省的相机参数
BOOL LoadConfig(TiXmlNode *pNode, TVideoProcAmpProperty& cameraParams,TVideoProcAmpProperty& defaultParams)
{
    TiXmlNode* pChild=NULL;
    do
    {
        pChild = pNode->IterateChildren(pChild);

        if(NULL == pChild) break;

        const char* lpszElementName = pChild->Value();
        if(_stricmp(lpszElementName, "Param") == 0)
        {
            const char* paramName    = ((TiXmlElement*)pChild)->Attribute("name"   );
            const char* paramValue   = ((TiXmlElement*)pChild)->Attribute("value"  );
            const char* paramDefault = ((TiXmlElement*)pChild)->Attribute("default");//缺省值


            if(paramName && paramValue && _stricmp(paramName, "Brightness") == 0)
            {//亮度
                cameraParams.Prop_VideoProcAmp_Brightness = atoi(paramValue);
                if(paramDefault) defaultParams.Prop_VideoProcAmp_Brightness = atoi(paramDefault);
            }
            else if(paramName && paramValue && _stricmp(paramName, "Contrast") == 0)
            {//对比度
                cameraParams.Prop_VideoProcAmp_Contrast = atoi(paramValue);
                if(paramDefault) defaultParams.Prop_VideoProcAmp_Contrast = atoi(paramDefault);
            }
            else if(paramName && paramValue && _stricmp(paramName, "Hue") == 0)
            {//色调
                cameraParams.Prop_VideoProcAmp_Hue = atoi(paramValue);
                if(paramDefault) defaultParams.Prop_VideoProcAmp_Hue = atoi(paramDefault);
            }
            else if(paramName && paramValue && _stricmp(paramName, "Saturation") == 0)
            {//饱和度
                cameraParams.Prop_VideoProcAmp_Satuation = atoi(paramValue);
                if(paramDefault) defaultParams.Prop_VideoProcAmp_Satuation = atoi(paramDefault);
            }
            else if(paramName && paramValue && _stricmp(paramName, "Sharpness") == 0)
            {//清晰度
                cameraParams.Prop_VideoProcAmp_Sharpness = atoi(paramValue);
                if(paramDefault) defaultParams.Prop_VideoProcAmp_Sharpness = atoi(paramDefault);
            }
            else if(paramName && paramValue && _stricmp(paramName, "Gamma") == 0)
            {//伽玛值
                cameraParams.Prop_VideoProcAmp_Gamma = atoi(paramValue);
                if(paramDefault) defaultParams.Prop_VideoProcAmp_Gamma = atoi(paramDefault);
            }
            else if(paramName && paramValue && _stricmp(paramName, "ColorEnable") == 0)
            {//颜色启动
                cameraParams.Prop_VideoProcAmp_ColorEnable = atoi(paramValue);
                if(paramDefault) defaultParams.Prop_VideoProcAmp_ColorEnable = atoi(paramDefault);
            }
            else if(paramName && paramValue && _stricmp(paramName, "WhiteBalance") == 0)
            {//白平衡
                cameraParams.Prop_VideoProcAmp_WhiteBalance = atoi(paramValue);
                if(paramDefault) defaultParams.Prop_VideoProcAmp_WhiteBalance = atoi(paramDefault);
            }
            else if(paramName && paramValue && _stricmp(paramName, "BacklightCompensation") == 0)
            {//背光补偿
                cameraParams.Prop_VideoProcMap_BacklightCompensation = atoi(paramValue);
                if(paramDefault) defaultParams.Prop_VideoProcMap_BacklightCompensation = atoi(paramDefault);
            }
            else if(paramName && paramValue && _stricmp(paramName, "Gain") == 0)
            {//增益
                cameraParams.Prop_VideoProcMap_Gain = atoi(paramValue);
                if(paramDefault) defaultParams.Prop_VideoProcMap_Gain = atoi(paramDefault);
            }
			else if (paramName && paramValue && _stricmp(paramName, "Exposure") == 0)
			{//曝光时间
				cameraParams.Prop_CameraControl_Exposure = atoi(paramValue);
				if(paramDefault) defaultParams.Prop_CameraControl_Exposure  = atoi(paramDefault);
			}
			else {

			}
        }

    }while(pChild);

    return TRUE;
}

//@功能:保存相机参数
//@参数:pNode, 指向配置文件中<CameraParams>节点的指针
//      cameraParams, 输如参数, 相机参数
BOOL SaveConfig(TiXmlNode *pNode, const TVideoProcAmpProperty& cameraParams)
{
    //亮度
    TiXmlComment* pXmlComment = new TiXmlComment("亮度");
    pNode->LinkEndChild(pXmlComment);

    TiXmlElement * pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "Brightness");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcAmp_Brightness);
    pNode->LinkEndChild(pElement);

    //对比度
    pXmlComment = new TiXmlComment("对比度");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "Contrast");
    pElement->SetAttribute("value",cameraParams.Prop_VideoProcAmp_Contrast);
    pNode->LinkEndChild(pElement);

    //色调
    pXmlComment = new TiXmlComment("色调");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "Hue");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcAmp_Hue);
    pNode->LinkEndChild(pElement);

    //饱和度
    pXmlComment = new TiXmlComment("饱和度");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "Saturation");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcAmp_Satuation);
    pNode->LinkEndChild(pElement);

    //清晰度
    pXmlComment = new TiXmlComment("清晰度");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "Sharpness");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcAmp_Sharpness);
    pNode->LinkEndChild(pElement);

    //伽玛值
    pXmlComment = new TiXmlComment("伽玛值");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "Gamma");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcAmp_Gamma);
    pNode->LinkEndChild(pElement);

    //颜色启动
    pXmlComment = new TiXmlComment("颜色启动");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "ColorEnable");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcAmp_ColorEnable);
    pNode->LinkEndChild(pElement);

    //白平衡
    pXmlComment = new TiXmlComment("白平衡");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "WhiteBalance");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcAmp_WhiteBalance);
    pNode->LinkEndChild(pElement);

    //背光补偿
    pXmlComment = new TiXmlComment("背光补偿");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "BacklightCompensation");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcMap_BacklightCompensation);
    pNode->LinkEndChild(pElement);

    //增益
    pXmlComment = new TiXmlComment("增益");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "Gain");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcMap_Gain);
    pNode->LinkEndChild(pElement);

	//曝光时间
	pXmlComment = new TiXmlComment("曝光时间");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "Exposure");
	pElement->SetAttribute("value", cameraParams.Prop_CameraControl_Exposure);
	pNode->LinkEndChild(pElement);
	

    return TRUE;
}



//@功能:保存相机参数
//@参数:pNode, 指向配置文件中<CameraParams>节点的指针
//      cameraParams, 输如参数, 相机参数
BOOL SaveConfig(TiXmlNode *pNode, const TVideoProcAmpProperty& cameraParams, const TVideoProcAmpProperty& defaultParams)
{
    //亮度
    TiXmlComment* pXmlComment = new TiXmlComment("亮度");
    pNode->LinkEndChild(pXmlComment);

    TiXmlElement * pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "Brightness");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcAmp_Brightness);
    pElement->SetAttribute("default", defaultParams.Prop_VideoProcAmp_Brightness);
    pNode->LinkEndChild(pElement);

    //对比度
    pXmlComment = new TiXmlComment("对比度");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "Contrast");
    pElement->SetAttribute("value",cameraParams.Prop_VideoProcAmp_Contrast);
    pElement->SetAttribute("default", defaultParams.Prop_VideoProcAmp_Contrast);
    pNode->LinkEndChild(pElement);

    //色调
    pXmlComment = new TiXmlComment("色调");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "Hue");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcAmp_Hue);
    pElement->SetAttribute("default", defaultParams.Prop_VideoProcAmp_Hue);
    pNode->LinkEndChild(pElement);

    //饱和度
    pXmlComment = new TiXmlComment("饱和度");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "Saturation");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcAmp_Satuation);
    pElement->SetAttribute("default", defaultParams.Prop_VideoProcAmp_Satuation);
    pNode->LinkEndChild(pElement);

    //清晰度
    pXmlComment = new TiXmlComment("清晰度");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "Sharpness");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcAmp_Sharpness);
    pElement->SetAttribute("default", defaultParams.Prop_VideoProcAmp_Sharpness);
    pNode->LinkEndChild(pElement);

    //伽玛值
    pXmlComment = new TiXmlComment("伽玛值");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "Gamma");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcAmp_Gamma);
    pElement->SetAttribute("default", defaultParams.Prop_VideoProcAmp_Gamma);
    pNode->LinkEndChild(pElement);

    //颜色启动
    pXmlComment = new TiXmlComment("颜色启动");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "ColorEnable");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcAmp_ColorEnable);
    pElement->SetAttribute("default", defaultParams.Prop_VideoProcAmp_ColorEnable);
    pNode->LinkEndChild(pElement);

    //白平衡
    pXmlComment = new TiXmlComment("白平衡");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "WhiteBalance");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcAmp_WhiteBalance);
    pElement->SetAttribute("default", defaultParams.Prop_VideoProcAmp_WhiteBalance);
    pNode->LinkEndChild(pElement);

    //背光补偿
    pXmlComment = new TiXmlComment("背光补偿");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "BacklightCompensation");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcMap_BacklightCompensation);
    pElement->SetAttribute("default", defaultParams.Prop_VideoProcMap_BacklightCompensation);
    pNode->LinkEndChild(pElement);

    //增益
    pXmlComment = new TiXmlComment("增益");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "Gain");
    pElement->SetAttribute("value", cameraParams.Prop_VideoProcMap_Gain);
    pElement->SetAttribute("default", defaultParams.Prop_VideoProcMap_Gain);
    pNode->LinkEndChild(pElement);

	//曝光时间
	pXmlComment = new TiXmlComment("曝光时间");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "Exposure");
	pElement->SetAttribute("value", cameraParams.Prop_CameraControl_Exposure);
	pElement->SetAttribute("default", defaultParams.Prop_CameraControl_Exposure);
	pNode->LinkEndChild(pElement);

    return TRUE;
}



//@功能:载入CMOS芯片规格信息
//@参数:pNode, 节点
//      CMOSChipSpec, CMOS芯片规格
BOOL LoadConfig(TiXmlNode *pNode, TCMOSChipSpecification &CMOSChipSpec)
{
    TiXmlNode* pChild=NULL;
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild) break;

        const char* lpszElementName = pChild->Value();
        if(_stricmp(lpszElementName, "Param") == 0)
        {
           const char* paramName    = ((TiXmlElement*)pChild)->Attribute("name"   );
           const char* paramValue   = ((TiXmlElement*)pChild)->Attribute("value"  );
 
            if(paramName && paramValue && _stricmp(paramName, "PixelSize") == 0)
            {//像素尺寸
                CMOSChipSpec.pixel_size = atof(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "WidthInMM") == 0)
            {//CMOS传感器图像区域物理宽度
                CMOSChipSpec.width_in_mm = atof(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "HeightInMM") == 0)
            {//CMOS传感器图像区域物理高度
                CMOSChipSpec.height_in_mm = atof(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "WidthInPixel") == 0)
            {//CMOS传感器图像区域像素宽度
                CMOSChipSpec.width_in_pixel = atoi(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "HeightInPixel") == 0)
            {//CMOS传感器图像区域像素高度
                CMOSChipSpec.height_in_pixel = atoi(paramValue);
            }
        }

    }while(pChild);

    return TRUE;
}

//@功能:保存CMOS芯片规格信息
//@参数:pNode, 节点
//      CMOSChipSpec, CMOS芯片规格
BOOL SaveConfig(TiXmlNode *pNode, const TCMOSChipSpecification &CMOSChipSpec)
{

    //像素尺寸
    TiXmlComment* pXmlComment = new TiXmlComment("像素尺寸，单位:mm");
    pNode->LinkEndChild(pXmlComment);

    TiXmlElement * pElement = new TiXmlElement("Param");
     pElement->SetAttribute("name", "PixelSize");
    pElement->SetDoubleAttribute("value", CMOSChipSpec.pixel_size);
    pNode->LinkEndChild(pElement);

    //CMOS传感器图像区域物理宽度
    pXmlComment = new TiXmlComment("CMOS传感器图像区域物理宽度, 单位:mm");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "WidthInMM");
    pElement->SetDoubleAttribute("value", CMOSChipSpec.width_in_mm);
    pNode->LinkEndChild(pElement);

    //CMOS传感器图像区域物理高度
    pXmlComment = new TiXmlComment("CMOS传感器图像区域物理高度, 单位:mm");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "HeightInMM");
    pElement->SetDoubleAttribute("value", CMOSChipSpec.height_in_mm);
    pNode->LinkEndChild(pElement);


    //CMOS传感器图像区域像素宽度
    pXmlComment = new TiXmlComment("CMOS传感器图像区域像素宽度, 单位:像素");
    pNode->LinkEndChild(pXmlComment);

     pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "WidthInPixel");
    pElement->SetDoubleAttribute("value", CMOSChipSpec.width_in_pixel);
    pNode->LinkEndChild(pElement);

    //CMOS传感器图像区域像素高度
    pXmlComment = new TiXmlComment("CMOS传感器图像区域像素高度, 单位:像素");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "HeightInPixel");
    pElement->SetDoubleAttribute("value", CMOSChipSpec.height_in_pixel);
    pNode->LinkEndChild(pElement);

    return TRUE;
}



//@功能:载入全局设置
//@参数:pNode, 指向配置文件中<Global>节点的指针
//      globalSettings,
BOOL LoadConfig(TiXmlNode *pNode, GlobalSettings& globalSettings)
{
    TiXmlNode* pChild=NULL;
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild) break;
        const char* lpszElementName = pChild->Value();
        if(_stricmp(lpszElementName, "Param") == 0)
        {
            const char* paramName  = ((TiXmlElement*)pChild)->Attribute("name");
            const char* paramValue = ((TiXmlElement*)pChild)->Attribute("value");

            if(paramName && paramValue && _stricmp(paramName, "Language") == 0)
            {//语言
                globalSettings.langCode = paramValue;
            }
			else if (paramName && paramValue && _stricmp(paramName, "ProjectionMode") == 0)
			{  //投影模式
				if (_stricmp(paramValue, "Desktop") == 0)
				{
					globalSettings.eProjectionMode = E_PROJECTION_DESKTOP;
				}
				else {
					globalSettings.eProjectionMode = E_PROJECTION_WALL;
				}
			}
            else if(paramName && paramValue && _stricmp(paramName, "DebugMode") == 0)
            {//调试模式
                if(_stricmp(paramValue, "Yes") == 0)
                {
                    globalSettings.bDebugMode = TRUE;
                }
                else
                {
                    globalSettings.bDebugMode = FALSE;
                }
            }
            else if(paramName && paramValue && _stricmp(paramName, "DebugLevel") == 0)
            {//调试级别
                globalSettings.nDebugLevel = atoi(paramValue);
            }

            else if(paramName && paramValue && _stricmp(paramName, "SaveIntermediateFile") == 0)
            {//是否保存自动校正中间文件标志
                if(_stricmp(paramValue, "Yes") == 0)
                {
                    globalSettings.bSaveIntermediateFile = TRUE;
                }
                else
                {
                    globalSettings.bSaveIntermediateFile = FALSE;
                }
            }
			else if (paramName && paramValue && _stricmp(paramName, "LensMode") == 0)
            { //启动时的工作模式
                globalSettings.eLensMode = (ESensorLensMode)atoi(paramValue);
            }
			else if (paramName && paramValue && _stricmp(paramName, "TouchTUIOMode") == 0)
			{
				if (_stricmp(paramValue, "Yes") == 0)
				{
					globalSettings.bTouchTUIOMode = TRUE;
				}
				else
				{
					globalSettings.bTouchTUIOMode = FALSE;
				}
			}
			else if (paramName && paramValue && _stricmp(paramName, "TouchHIDMode") == 0)
			{
				if (_stricmp(paramValue, "Yes") == 0)
				{
					globalSettings.bTouchHIDMode = TRUE;
				}
				else
				{
					globalSettings.bTouchHIDMode = FALSE;
				}
			}
            else if(paramName && paramValue && _stricmp(paramName, "HID Mode") == 0)
            {

                if(_stricmp(paramValue, "Touch Screen") == 0)
                {
                    globalSettings.eHIDDeviceMode = E_DEV_MODE_TOUCHSCREEN;
                }
                else
                {
                    globalSettings.eHIDDeviceMode = E_DEV_MODE_MOUSE;
                }
            }
			else if(paramName && paramValue && _stricmp(paramName, "EnableOnlineRegister") == 0)
			{
				if(_stricmp(paramValue, "Yes") == 0)
				{
					globalSettings.bEnableOnlineRegister = TRUE;
				}
				else
				{
					globalSettings.bEnableOnlineRegister = FALSE;
				}

			}
			else if(paramName && paramValue && _stricmp(paramName, "TouchScreenAspectRatio") == 0)
			{

				if(_stricmp(paramValue, "16:9") == 0)
				{
					globalSettings.eTouchScreenAspectRatio = E_TOUCH_SCREEN_ASPECT_RATIO_16_9;
				}
				else if(_stricmp(paramValue, "16:10") == 0)
				{
					globalSettings.eTouchScreenAspectRatio = E_TOUCH_SCREEN_ASPECT_RATIO_16_10;
				}
				else if(_stricmp(paramValue, "4:3") == 0)
				{
					globalSettings.eTouchScreenAspectRatio = E_TOUCH_SCREEN_ASPECT_RATIO_4_3;
				}
				else
				{
					globalSettings.eTouchScreenAspectRatio = E_TOUCH_SCREEN_ASPECT_RATIO_AUTO;
				}
			}
            else if(paramName && paramValue && _stricmp(paramName, "RecordVideo") == 0)
            {
                if(_stricmp(paramValue, "Yes") == 0)
                {
                    globalSettings.bRecordVideo = TRUE;
                }
                else
                {
                    globalSettings.bRecordVideo = FALSE;
                }
            }

			else if (paramName && paramValue && _stricmp(paramName, "DoubleScreenTouchMerge") == 0)
			{
				if (_stricmp(paramValue, "Yes") == 0)
				{
					globalSettings.bDoubleScreenTouchMerge = TRUE;
				}
				else
				{
					globalSettings.bDoubleScreenTouchMerge = FALSE;
				}
			}
            else if(paramName && paramValue && _stricmp(paramName, "ScreenDiagonalLength") == 0)
            {
                double fInches = atof(paramValue);
                globalSettings.fScreenDiagonalPhysicalLength = (float)(fInches * 25.4);//1 inches = 25.4 mm

            }
			else if (paramName && paramValue && _stricmp(paramName, "MaxTimeInSearchDevice") == 0)
			{
				globalSettings.nMaxTimeInSearchDevice = atoi(paramValue);
			}
            else if (paramName && paramValue && _stricmp(paramName, "ScreenCount") == 0)
            {
                int nScreenCount = atoi(paramValue);

                globalSettings.eScreenMode = EScreenModeUnknown;

                if ( 0 <  nScreenCount && nScreenCount <= EScreenModeNumber)
                {
                    globalSettings.eScreenMode = (EScreenMode)(nScreenCount + 1);
                }
            }

        }//if
        else if(_stricmp(lpszElementName, "CMOS_CHIP") == 0)
        {
            LoadConfig(pChild, globalSettings.CMOSChipSpecification);
        }
       
    }while(pChild);


    return TRUE;
}


//@功能:载入配置文件
//@参数:pNode,
//      globalSettings,
BOOL SaveConfig(TiXmlNode *pNode, const GlobalSettings& globalSettings)
{
    //语言
    TiXmlComment* pXmlComment = new TiXmlComment("语言\n\t\tEN:英语\n\t\tCHS:简体中文\n\t\tCHT:繁体中文\n\t\tESP:西班牙语\n\t\tKOR:韩语\n\t\tTHA:泰语\n\t\tVIT:越南语\n\t\tDEU:德语\n\t\tFRA:法语\n\t\tITA:意大利语\n\t\tRUS:俄语\n\t\tPTG:葡萄牙语");
    pNode->LinkEndChild(pXmlComment);

    TiXmlElement * pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "Language");
    pElement->SetAttribute("value",CT2A(globalSettings.langCode));
    pNode->LinkEndChild(pElement);

	//投影模式
	pXmlComment = new TiXmlComment("投影模式");
	pNode->LinkEndChild(pXmlComment);
	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "ProjectionMode");
	switch (globalSettings.eProjectionMode)
	{
	    case E_PROJECTION_DESKTOP:
			pElement->SetAttribute("value", "Desktop");
			break;
		case E_PROJECTION_WALL:
			pElement->SetAttribute("value", "Wall");
			break;
	}
	pNode->LinkEndChild(pElement);

    //调试模式
    pXmlComment = new TiXmlComment("调试模式");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "DebugMode");
    pElement->SetAttribute("value", globalSettings.bDebugMode?"Yes":"No");
    pNode->LinkEndChild(pElement);


    //调试级别
    pXmlComment = new TiXmlComment("调试级别(0~2)");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "DebugLevel");
    pElement->SetAttribute("value", globalSettings.nDebugLevel);
    pNode->LinkEndChild(pElement);


    //是否保存自动校正中间文件标志
    pXmlComment = new TiXmlComment("是否保存自动校正中间文件标志(Yes/No)");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "SaveIntermediateFile");
    pElement->SetAttribute("value", globalSettings.bSaveIntermediateFile?"Yes":"No");
    pNode->LinkEndChild(pElement);

    //启动时的工作模式
    pXmlComment = new TiXmlComment("启动时的工作模式。0:图像调试模式;1:激光器调试模式;2:正常使用模式");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "LensMode");
    pElement->SetAttribute("value", int(globalSettings.eLensMode));
    pNode->LinkEndChild(pElement);

	//TUIO触控的方式
	pXmlComment = new TiXmlComment("是否打开TUIO触控方式");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "TouchTUIOMode");
	pElement->SetAttribute("value", globalSettings.bTouchTUIOMode? "Yes" : "No");
	pNode->LinkEndChild(pElement);

	//HID触控的方式
	pXmlComment = new TiXmlComment("是否打开HID触控方式");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "TouchHIDMode");
	pElement->SetAttribute("value", globalSettings.bTouchHIDMode ? "Yes" : "No");
	pNode->LinkEndChild(pElement);


    //HID工作模式
    pXmlComment = new TiXmlComment("HID工作方式(Mouse; Touch Screen)");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "HID Mode");
    pElement->SetAttribute("value", globalSettings.eHIDDeviceMode == E_DEV_MODE_MOUSE?"Mouse":"Touch Screen");
    pNode->LinkEndChild(pElement);


	//在线注册使能标志
    pXmlComment = new TiXmlComment("使能在线注册功能(Yes/No)");
    pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "EnableOnlineRegister");
	pElement->SetAttribute("value", globalSettings.bEnableOnlineRegister?"Yes":"No");
    pNode->LinkEndChild(pElement);

	//触屏宽高比
	pXmlComment = new TiXmlComment("触屏宽高比:Auto, 16:9, 16:10, 4:3");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "TouchScreenAspectRatio");
    switch(globalSettings.eTouchScreenAspectRatio)
    {
    case E_TOUCH_SCREEN_ASPECT_RATIO_AUTO:
        pElement->SetAttribute("value", "Auto");
        break;

    case E_TOUCH_SCREEN_ASPECT_RATIO_16_9:
		pElement->SetAttribute("value", "16:9");
        break;

	case E_TOUCH_SCREEN_ASPECT_RATIO_16_10:
		pElement->SetAttribute("value", "16:10");
		break;

	case E_TOUCH_SCREEN_ASPECT_RATIO_4_3:
		pElement->SetAttribute("value", "4:3");
		break;

    }//switch

    pNode->LinkEndChild(pElement);

    //是否在自动校正过程中录像
    pXmlComment = new TiXmlComment("是否在自动校正过程中录像");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "RecordVideo");
    pElement->SetAttribute("value", globalSettings.bRecordVideo?"Yes":"No");
    pNode->LinkEndChild(pElement);


	//是否双屏融合
	pXmlComment = new TiXmlComment("是否双屏融合");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "DoubleScreenTouchMerge");
	pElement->SetAttribute("value", globalSettings.bDoubleScreenTouchMerge ? "Yes" : "No");
	pNode->LinkEndChild(pElement);

    //触摸屏的物理尺寸
    pXmlComment = new TiXmlComment("触摸屏对角线的物理尺寸,单位:英寸");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "ScreenDiagonalLength");

    char szValue[32];
    sprintf_s(szValue, _countof(szValue), "%.1f", globalSettings.fScreenDiagonalPhysicalLength/25.4f);
    pElement->SetAttribute("value",szValue);
    pNode->LinkEndChild(pElement);

	//程序启动时,搜索设备时最大允许花费的时间
	pXmlComment = new TiXmlComment("程序启动时,搜索设备时最大允许花费的时间, 单位:毫秒");
	pNode->LinkEndChild(pXmlComment);
	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "MaxTimeInSearchDevice");
	pElement->SetAttribute("value",globalSettings.nMaxTimeInSearchDevice);
	pNode->LinkEndChild(pElement);

    //CMOS芯片信息
    pXmlComment = new TiXmlComment("CMOS芯片信息");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("CMOS_CHIP");
    pNode->LinkEndChild(pElement);

    //保存CMOS芯片
    SaveConfig(pElement, globalSettings.CMOSChipSpecification);

    //屏接的屏幕数目
    pXmlComment = new TiXmlComment("屏接的屏幕数目");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "ScreenCount");

    if (EScreenModeUnknown == globalSettings.eScreenMode)
    {
        pElement->SetAttribute("value", -1);
    }
    else
    {
        pElement->SetAttribute("value", int(globalSettings.eScreenMode) + 1);
    }
    pNode->LinkEndChild(pElement);

    return TRUE;
}


//@功能:载入正常使用时的配置信息
//@参数:pNode, 指向配置文件中<NormalUsage>节点的指针
//      normalUsageSettings,输出参数
BOOL LoadConfig(TiXmlNode *pNode, NormalUsageSettings& normalUsageSettings)
{
    if(pNode == NULL) return FALSE;
    TiXmlNode* pChild = NULL;

    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild) break;
        const char* lpszElementName = pChild->Value();
        if(_stricmp(lpszElementName, "Param") == 0)
        {
            const char* paramName  = ((TiXmlElement*)pChild)->Attribute("name");
            const char* paramValue = ((TiXmlElement*)pChild)->Attribute("value");
            const char* paramDefault = ((TiXmlElement*)pChild)->Attribute("default");

            if(paramName && paramValue && _stricmp(paramName, "YThreshold") == 0)
            {//光斑触发门限
                normalUsageSettings.cYThreshold = atoi(paramValue);

                if(paramDefault) normalUsageSettings.cYThresholdDefault = atoi(paramDefault);
            }
			else if (paramName && paramValue && _stricmp(paramName, "DynmicMaskThresholdPercentage") == 0)
			{//自动屏蔽门限占光斑触发门限的百分比
				normalUsageSettings.nDynamicMaskThresholdPercentage = atoi(paramValue);
				if(paramDefault) normalUsageSettings.nDynamicMaskThresholdPercentageDefault = atoi(paramDefault);
			}
            else if(paramName && paramValue && _stricmp(paramName, "LightSpotMinimumWidth") == 0)
            {//最小光斑的宽度
                normalUsageSettings.nLightSpotMinimumWidth = atoi(paramValue);

                if(paramDefault) normalUsageSettings.nLightSpotMinimumHeightDefault = atoi(paramDefault);
            }
            else if(paramName && paramValue && _stricmp(paramName, "LightSpotMinimumHeight") == 0)
            {//最小光斑的高度
                normalUsageSettings.nLightSpotMinimumHeight = atoi(paramValue);
                //
                if(paramDefault) normalUsageSettings.nLightSpotMinimumHeightDefault = atoi(paramDefault);

            }
        }
        else if(_stricmp(lpszElementName, "CameraParams") == 0)
        {//正常使用时的相机参数
            LoadConfig(pChild, normalUsageSettings.cameraParams, normalUsageSettings.defaultParams);
        }
    }while(pChild);

    return TRUE;
}


//@功能:载入配置文件
//@参数:pNode, 指向配置文件中<NormalUsage>节点的指针
//      normalUsageSettings,
BOOL SaveConfig(TiXmlNode *pNode, const NormalUsageSettings& normalUsageSettings)
{
    //光斑触发门限
    TiXmlComment* pXmlComment = new TiXmlComment("光斑触发门限");
    pNode->LinkEndChild(pXmlComment);

    TiXmlElement * pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "YThreshold");
    pElement->SetAttribute("value", normalUsageSettings.cYThreshold);
    pElement->SetAttribute("default", normalUsageSettings.cYThresholdDefault);
    pNode->LinkEndChild(pElement);


	//自动屏蔽门限占光斑触发门限的百分比
	pXmlComment = new TiXmlComment("自动屏蔽门限占光斑触发门限的百分比(50~100)");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "DynmicMaskThresholdPercentage");
	pElement->SetAttribute("value", normalUsageSettings.nDynamicMaskThresholdPercentage);
	pElement->SetAttribute("default", normalUsageSettings.nDynamicMaskThresholdPercentageDefault);
	pNode->LinkEndChild(pElement);

    //最小光斑的宽度
    pXmlComment = new TiXmlComment("最小光斑的宽度");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "LightSpotMinimumWidth");
    pElement->SetAttribute("value",  normalUsageSettings.nLightSpotMinimumWidth);
    pElement->SetAttribute("default", normalUsageSettings.nLightSpotMinimumWidthDefault);
    pNode->LinkEndChild(pElement);

    //最小光斑的高度
    pXmlComment = new TiXmlComment("最小光斑的高度");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "LightSpotMinimumHeight");
    pElement->SetAttribute("value",  normalUsageSettings.nLightSpotMinimumHeight);
    pElement->SetAttribute("default", normalUsageSettings.nLightSpotMinimumHeightDefault);
    pNode->LinkEndChild(pElement);

    //正常使用时的相机参数
    pXmlComment = new TiXmlComment("正常使用时的相机参数");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("CameraParams");
    pNode->LinkEndChild(pElement);
    SaveConfig(pElement, normalUsageSettings.cameraParams, normalUsageSettings.defaultParams);
    return TRUE;
}


//@功能:载入安装调试模式下的参数
//@参数:pNode, 指向配置文件中<InstallTunning>节点的指针
//      installTunningSettings, 输出参数
BOOL LoadConfig(TiXmlNode *pNode, InstallTunningSettings& installTunningSettings)
{
    if(pNode == NULL) return FALSE;
    TiXmlNode* pChild = NULL;
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild) break;
        const char* lpszElementName = pChild->Value();
        //激光器调节模试模式相机参数
        if(_stricmp(lpszElementName, "CameraParams") == 0)
        {
            LoadConfig(pChild, installTunningSettings.cameraParams, installTunningSettings.defaultParams);
        }

    }while(pChild);
    return TRUE;
}

//@功能:保存安装调试模式下的参数
//@参数:pNode, 指向配置文件中<InstallTunning>节点的指针
//      installTunningSettings, 输入参数
BOOL SaveConfig(TiXmlNode *pNode, const InstallTunningSettings& installTunningSettings)
{
    //激光器调节模试模式相机参数
    TiXmlComment* pXmlComment = new TiXmlComment("安装调试模式下的相机参数");
    pNode->LinkEndChild(pXmlComment);

    TiXmlElement * pElement = new TiXmlElement("CameraParams");
    pNode->LinkEndChild(pElement);
    SaveConfig(pElement, installTunningSettings.cameraParams, installTunningSettings.defaultParams);
    return TRUE;
}



//@功能:载入正常使用时的配置信息
//@参数:pNode, 指向配置文件中<LaserTunning>节点的指针
//      laserTunningSettings, 输出参数
BOOL LoadConfig(TiXmlNode *pNode, LaserTunningSettings& laserTunningSettings)
{
    if(pNode == NULL) return FALSE;
    TiXmlNode* pChild = NULL;
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild) break;
        const char* lpszElementName = pChild->Value();
        //激光器调节模试模式相机参数
        if(_stricmp(lpszElementName, "CameraParams") == 0)
        {
            LoadConfig(pChild, laserTunningSettings.cameraParams, laserTunningSettings.defaultParams);
        }

    }while(pChild);
    return TRUE;
}

//@功能:保存激光器调试模式下的参数
//@参数:pNode, 指向配置文件中<LaserTunning>节点的指针
//      laserTunningSettings, 输入参数
BOOL SaveConfig(TiXmlNode *pNode, const LaserTunningSettings& laserTunningSettings)
{
    //激光器调节模试模式相机参数
    TiXmlComment* pXmlComment = new TiXmlComment("激光器调试模式下的相机参数");
    pNode->LinkEndChild(pXmlComment);

    TiXmlElement * pElement = new TiXmlElement("CameraParams");
    pNode->LinkEndChild(pElement);
    SaveConfig(pElement, laserTunningSettings.cameraParams, laserTunningSettings.defaultParams);
    return TRUE;
}

//@功能:载入自动校正时的图像参数列表
//@参数:pNode, 指向配置文件中<ImageParams>节点的指针
//      imageParam, 自动校正画面参数
BOOL LoadConfig(TiXmlNode * pNode, AutoCalibrateImageParams& imageParams)
{
    if(NULL == pNode) return FALSE;
    TiXmlNode* pChild = NULL;
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild) break;
        const char* lpszElementName = pChild->Value();

        if(_stricmp(lpszElementName, "Param") == 0)
        {
            const char* paramName  = ((TiXmlElement*)pChild)->Attribute("name");
            const char* paramValue = ((TiXmlElement*)pChild)->Attribute("value");
            if(paramName && paramValue && _stricmp(paramName, "autoCalibrateExpectedBrightness") == 0)
            {//自动校正亮度自动控制时期望的平均画面亮度(范围0~255)
                imageParams.autoCalibrateExpectedBrightness = atoi(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "AutoCalibrateHilightGray") == 0)
            {//自动校正图案中高亮块的灰度值(0~255)
                imageParams.autoCalibrateHilightGray = atoi(paramValue);
            }

        }

    }while(pChild);

    return TRUE;
}


//@功能:载入自动校正时的图像参数列表
//@参数:pNode, 指向配置文件中<ImageParams>节点的指针
//      imageParam, 自动校正画面参数
BOOL LoadConfig(TiXmlNode * pNode, AutoCalibrateImageParams& imageParams, AutoCalibrateImageParams& defaultParams)
{
    if(NULL == pNode) return FALSE;
    TiXmlNode* pChild = NULL;
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild) break;
        const char* lpszElementName = pChild->Value();

        if(_stricmp(lpszElementName, "Param") == 0)
        {
            const char* paramName  = ((TiXmlElement*)pChild)->Attribute("name");
            const char* paramValue = ((TiXmlElement*)pChild)->Attribute("value");
            const char* paramDefault = ((TiXmlElement*)pChild)->Attribute("default");

            if(paramName && paramValue && _stricmp(paramName, "autoCalibrateExpectedBrightness") == 0)
            {//自动校正亮度自动控制时期望的平均画面亮度(范围0~255)
                imageParams.autoCalibrateExpectedBrightness = atoi(paramValue);

                //缺省值
                if(paramDefault)defaultParams.autoCalibrateExpectedBrightness = atoi(paramDefault);

            }
            else if(paramName && paramValue && _stricmp(paramName, "AutoCalibrateHilightGray") == 0)
            {//自动校正图案中高亮块的灰度值(0~255)
                imageParams.autoCalibrateHilightGray = atoi(paramValue);

                //缺省值
                if(paramDefault)defaultParams.autoCalibrateHilightGray = atoi(paramDefault);
            }
        }
    }while(pChild);

    return TRUE;
}



//@功能:保存自动校正下的画面参数
//@参数:pNode, 指向配置文件中<ImageParams>节点的指针
//      imageParams, 输入参数, 自动校正画面参数
BOOL SaveConfig(TiXmlNode *pNode, const AutoCalibrateImageParams& imageParams)
{
    if(NULL == pNode) return FALSE;

    TiXmlComment* pXmlComment = new TiXmlComment("自动校正亮度自动控制时期望的平均画面亮度(范围0~255)");
    pNode->LinkEndChild(pXmlComment);

    TiXmlElement* pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "autoCalibrateExpectedBrightness");
    pElement->SetAttribute("value", imageParams.autoCalibrateExpectedBrightness);
    pNode->LinkEndChild(pElement);

    //自动校正图案中高亮块的灰度值(0~255)
    pXmlComment = new TiXmlComment("自动校正图案中高亮块的灰度值(0~255)");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "AutoCalibrateHilightGray");
    pElement->SetAttribute("value", imageParams.autoCalibrateHilightGray);
    pNode->LinkEndChild(pElement);

    return TRUE;
}


//@功能:保存自动校正下的画面参数
//@参数:pNode, 指向配置文件中<ImageParams>节点的指针
//      imageParams, 输入参数, 自动校正画面参数
//      defaultParams, 输入参数, 缺省的画面自动校正参数
BOOL SaveConfig(TiXmlNode *pNode, const AutoCalibrateImageParams& imageParams, const AutoCalibrateImageParams& defaultParams)
{
    if(NULL == pNode) return FALSE;

    TiXmlComment* pXmlComment = new TiXmlComment("自动校正亮度自动控制时期望的平均画面亮度(范围0~255)");
    pNode->LinkEndChild(pXmlComment);

    TiXmlElement* pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "autoCalibrateExpectedBrightness");
    pElement->SetAttribute("value", imageParams.autoCalibrateExpectedBrightness);
    pElement->SetAttribute("default", defaultParams.autoCalibrateExpectedBrightness);
    pNode->LinkEndChild(pElement);

    //自动校正图案中高亮块的灰度值(0~255)
    pXmlComment = new TiXmlComment("自动校正图案中高亮块的灰度值(0~255)");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "AutoCalibrateHilightGray");
    pElement->SetAttribute("value", imageParams.autoCalibrateHilightGray);
    pElement->SetAttribute("default", defaultParams.autoCalibrateHilightGray);
    pNode->LinkEndChild(pElement);

    return TRUE;
}

//@功能:载入自动校正时的图像参数列表
//@参数:pNode, 指向配置文件中<ImageParamsList>节点的指针
//      imageParamList, 自动校正画面参数列表
BOOL LoadConfig(TiXmlNode * pNode, AutoCalibrateSettings& autoCalibrateSettings)
{
    if(NULL == pNode) return FALSE;
    TiXmlNode* pChild = NULL;
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild) break;
        const char* lpszElementName = pChild->Value();

        if(_stricmp(lpszElementName, "ImageParams") == 0)
        {
			//载入自动校正时的画面参数
			LoadConfig(pChild, autoCalibrateSettings.calibrateImageParams, autoCalibrateSettings.defaultCalibrateImageParams);
        }
		else if(_stricmp(lpszElementName, "CameraParams") == 0)
		{
			LoadConfig(pChild, autoCalibrateSettings.cameraParams, autoCalibrateSettings.defaultParams);
		}

    }while(pChild);

    return TRUE;
}



//@功能:保存自动校正下的参数
//@参数:pNode, 指向配置文件中<ImageParamsList>节点的指针
//      imageParamList, 输入参数, 自动校正画面参数列表
BOOL SaveConfig(TiXmlNode *pNode, const AutoCalibrateSettings& autoCalibrateSettings,int nIndex)
{
      //自动校正画面参数
      char text[1024];
      sprintf_s(text, _countof(text), "第(%d)组自动校正画面参数", nIndex);
      TiXmlComment* pXmlComment = new TiXmlComment(text);
      pNode->LinkEndChild(pXmlComment);

      TiXmlElement* pElement = new TiXmlElement("ImageParams");
      SaveConfig(pElement, autoCalibrateSettings.calibrateImageParams, autoCalibrateSettings.defaultCalibrateImageParams);
      pNode->LinkEndChild(pElement);

	  sprintf_s(text, _countof(text), "第(%d)组自动校正参数", nIndex);
	  pXmlComment = new TiXmlComment(text);
	  pNode->LinkEndChild(pXmlComment);

	  pElement = new TiXmlElement("CameraParams");
	  SaveConfig(pElement, autoCalibrateSettings.cameraParams, autoCalibrateSettings.defaultParams);
	  pNode->LinkEndChild(pElement);

    return TRUE;
}

//@功能:载入正常使用时的配置信息
//@参数:pNode, 指向配置文件中<AutoCalibrate>节点的指针
//      autoCalibrateSettings, 输出参数, 自动校正配置信息
BOOL LoadConfig(TiXmlNode *pNode, AutoCalibrateSettingsList& autoCalibrateSettingslist)
{
    if(pNode == NULL) return FALSE;
    TiXmlNode* pChild = NULL;

	AutoCalibrateSettings autocalibrateSettings;
	autoCalibrateSettingslist.clear();
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild) break;
        const char* lpszElementName = pChild->Value();

        if(_stricmp(lpszElementName, "AutoCalibrateParams") == 0)
        {  
			//载入自动校正时的画面参数

			if (LoadConfig(pChild, autocalibrateSettings))
			{
				autoCalibrateSettingslist.push_back(autocalibrateSettings);
			}
        }
      
    }while(pChild);

    return TRUE;
}



//@功能:保存自动校正下的参数
//@参数:pNode, 指向配置文件中<AutoCalibrate>节点的指针
//      autoCalibrateSettings, 输入参数, 自动校正配置信息
BOOL SaveConfig(TiXmlNode *pNode, const AutoCalibrateSettingsList& autoCalibrateSettingslist)
{
	int nCount = autoCalibrateSettingslist.size();

	for(int i = 0 ;i < nCount ; i++ )
	{
		char text[1024];
		sprintf_s(text, _countof(text), "第(%d)组自动校正参数", i + 1);
		TiXmlComment* pXmlComment = new TiXmlComment(text);
		pNode->LinkEndChild(pXmlComment);

		TiXmlElement* pElement = new TiXmlElement("AutoCalibrateParams");
		SaveConfig(pElement, autoCalibrateSettingslist[i],i+1);
		pNode->LinkEndChild(pElement);
	}
    return TRUE;
}


//@功能:载入正常使用时的配置信息
//@参数:pNode, 指向配置文件中<AutoMask>节点的指针
//      autoMasktSettings, 输出参数, 自动校正配置信息
BOOL LoadConfig(TiXmlNode *pNode, AutoMaskSettings& autoMasktSettings)
{
    if(pNode == NULL) return FALSE;
    TiXmlNode* pChild = NULL;
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild) break;
        const char* lpszElementName = pChild->Value();
        if(_stricmp(lpszElementName, "Param") == 0)
        {
            const char* paramName   = ((TiXmlElement*)pChild)->Attribute("name");
            const char* paramValue  = ((TiXmlElement*)pChild)->Attribute("value");
            const char* paramDefault=  ((TiXmlElement*)pChild)->Attribute("default");

            if(paramName && paramValue && _stricmp(paramName, "AutoMaskDetectThreshold") == 0)
            {//自动屏蔽目标检测门限0~255, 值越小则屏蔽的区域越多
                autoMasktSettings.cAutoMaskDetectThreshold = atoi(paramValue);
                if(paramDefault) autoMasktSettings.cAutoMaskDetectThresholdDefault = atoi(paramDefault);
            }
            else if(paramName && paramValue && _stricmp(paramName, "MaskAreaEroseSize") == 0)
            {//自动屏蔽区向外膨胀尺寸
                autoMasktSettings.nMaskAreaEroseSize = atoi(paramValue);
                if(paramDefault) autoMasktSettings.nMaskAreaEroseSizeDefault = atoi(paramDefault);
            }
        }

    }while(pChild);
    return TRUE;
}

//@功能:保存自动屏蔽下的参数
//@参数:pNode, 指向配置文件中<AutoMask>节点的指针
//      autoMasktSettings, 输入参数, 自动校正配置信息
BOOL SaveConfig(TiXmlNode *pNode, const AutoMaskSettings& autoMasktSettings)
{
    //自动屏蔽目标检测门限0~255, 值越小则屏蔽的区域越多
    TiXmlComment* pXmlComment = new TiXmlComment("自动屏蔽目标检测门限0~255, 值越小则屏蔽的区域越多");
    pNode->LinkEndChild(pXmlComment);

    TiXmlElement * pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "AutoMaskDetectThreshold");
    pElement->SetAttribute("value", autoMasktSettings.cAutoMaskDetectThreshold);
    pElement->SetAttribute("default", autoMasktSettings.cAutoMaskDetectThresholdDefault);
    pNode->LinkEndChild(pElement);

    //自动屏蔽区向外膨胀尺寸
    pXmlComment = new TiXmlComment("自动屏蔽区腐蚀尺寸");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "MaskAreaEroseSize");
    pElement->SetAttribute("value", autoMasktSettings.nMaskAreaEroseSize);
    pElement->SetAttribute("default", autoMasktSettings.nMaskAreaEroseSizeDefault);
    pNode->LinkEndChild(pElement);
    return TRUE;
}


//@功能:载入正常使用时的配置信息
//@参数:pNode, 指向配置文件中<ManualCalibrate>节点的指针
//      manualCalibrateSettings, 输出参数, 手动校正配置信息
BOOL LoadConfig(TiXmlNode *pNode, ManualCalibrateSettings& manualCalibrateSettings)
{
    if(pNode == NULL) return FALSE;
    TiXmlNode* pChild = NULL;
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild) break;
        const char* lpszElementName = pChild->Value();
        if(_stricmp(lpszElementName, "Param") == 0)
        {
            const char* paramName  = ((TiXmlElement*)pChild)->Attribute("name");
            const char* paramValue = ((TiXmlElement*)pChild)->Attribute("value");

            if(paramName && paramValue && _stricmp(paramName, "ManualCalibratePtNumInEachRow") == 0)
            { //手动校正每行的校正点个数(=列数),最小值=2
                manualCalibrateSettings.nPtNumInEachRow = atoi(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "ManualCalibratePtNumInEachCol") == 0)
            {//手动校正每列的校正点个数(=行数),最小值=2
                manualCalibrateSettings.nPtNumInEachCol = atoi(paramValue);
            }

        }

    }while(pChild);
    return TRUE;
}

//@功能:保存手动校正的参数
//@参数:pNode, 指向配置文件中<ManualCalibrate>节点的指针
//      manualCalibrateSettings, 输入参数, 手动校正配置信息
BOOL SaveConfig(TiXmlNode *pNode, const ManualCalibrateSettings& manualCalibrateSettings)
{
    //手动校正每行的校正点个数(=列数),最小值=2
    TiXmlComment* pXmlComment = new TiXmlComment("手动校正每行的校正点个数(=列数),最小值=2");
    pNode->LinkEndChild(pXmlComment);

    TiXmlElement * pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "ManualCalibratePtNumInEachRow");
    pElement->SetAttribute("value", manualCalibrateSettings.nPtNumInEachRow);
    pNode->LinkEndChild(pElement);

    //手动校正每行的校正点个数(=列数),最小值=2
    pXmlComment = new TiXmlComment("//手动校正每行的校正点个数(=列数),最小值=2");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "ManualCalibratePtNumInEachCol");
    pElement->SetAttribute("value", manualCalibrateSettings.nPtNumInEachCol);
    pNode->LinkEndChild(pElement);
    return TRUE;
}

//@功能:载入正常使用时的配置信息
//@参数:pNode, 指向配置文件中<AdvanceSettings>节点的指针
//      advanceSettings, 输出参数, 手动校正配置信息
BOOL LoadConfig(TiXmlNode *pNode, TAdvancedSettings& advanceSettings)
{
    if(pNode == NULL) return FALSE;
    TiXmlNode* pChild = NULL;
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild) break;
        const char* lpszElementName = pChild->Value();
        if(_stricmp(lpszElementName, "Param") == 0)
        {
            const char* paramName  = ((TiXmlElement*)pChild)->Attribute("name");
            const char* paramValue = ((TiXmlElement*)pChild)->Attribute("value");
            if(_stricmp(paramName, "ClutterMaskingAreaInflationRadius") == 0)
            {//干扰光斑屏蔽区膨胀半径
                advanceSettings.nAutoMaskDilateRadius = atoi(paramValue);
            }	
            else if(_stricmp(paramName, "SpotProportion") == 0)
            {//响应光斑大小比例
                advanceSettings.nSpotProportion = atoi(paramValue);
            }
            else if(_stricmp(paramName, "EraserMultiples") == 0)
            {//板擦与光斑的倍数
                advanceSettings.nMultEraser = atoi(paramValue);
            }
            else if(_stricmp(paramName, "SetFixedBlobTime") == 0)
            {//检测为固定光斑的时间
                advanceSettings.nFixedBlobSetTime = atoi(paramValue);
            }
			else if (_stricmp(paramName, "EnableStrokeInterpolate") == 0)
			{
				if (_stricmp(paramValue, "Yes") == 0)
				{
					advanceSettings.bEnableStrokeInterpolate = TRUE;
				}
				else
				{
					advanceSettings.bEnableStrokeInterpolate = FALSE;
				}
			}
			else if (_stricmp(paramName, "DeviceTouchType") == 0)
			{
				if (paramValue &&_stricmp(paramValue, "Finger_Touch_WhiteBoard") == 0)
				{
					advanceSettings.m_eTouchType = E_DEVICE_FINGER_TOUCH_WHITEBOARD;
				}
				else if(paramValue &&_stricmp(paramValue, "Pen_Touch_WhiteBoard") == 0)
				{
					advanceSettings.m_eTouchType = E_DEVICE_PEN_TOUCH_WHITEBOARD;
				}
				else if(paramValue &&_stricmp(paramValue, "Finger_Touch_Control") == 0)
				{
					advanceSettings.m_eTouchType = E_DEVICE_FINGER_TOUCH_CONTROL;
				}
				else if (paramValue &&_stricmp(paramValue, "Palm_Touch_Control") == 0)
				{
					advanceSettings.m_eTouchType = E_DEVICE_PALM_TOUCH_CONTROL;
				}
				else
				{
				}
			}
			else if (_stricmp(paramName, "RearProjectionDevice") == 0)
			{
				if (paramValue && _stricmp(paramValue, "Yes") == 0)
				{
					advanceSettings.bIsRearProjection = TRUE;
				}
				else
				{
					advanceSettings.bIsRearProjection = FALSE;
				}

			}
			else if(_stricmp(paramName, "DynamicMaskFrame") == 0)
			{
				if (paramValue && _stricmp(paramValue, "Yes")== 0)
				{
					advanceSettings.bIsDynamicMaskFrame = TRUE;
				}
				else {
					advanceSettings.bIsDynamicMaskFrame = FALSE;
				}
			}
			else if(_stricmp(paramName, "AntiJamming") == 0)
			{
				if (paramValue && _stricmp(paramValue, "Yes") == 0)
				{
					advanceSettings.bIsAntiJamming = TRUE;
				}
				else
				{
					advanceSettings.bIsAntiJamming = FALSE;
				}
			}
			else if (_stricmp(paramName, "OnLineScreenArea") == 0)
			{
				if (paramValue && _stricmp(paramValue, "Yes") == 0)
				{
					advanceSettings.bIsOnLineScreenArea = TRUE;
				}
				else
				{
					advanceSettings.bIsOnLineScreenArea = FALSE;
				}
			}
        }

    }while(pChild);
    return TRUE;
}

//@功能:保存高级设置参数
//@参数:pNode, 指向配置文件中<AdvanceSettings>节点的指针
//      advanceSettings, 输入参数, 手动校正配置信息
BOOL SaveConfig(TiXmlNode *pNode, const TAdvancedSettings& advanceSettings)
{

	//设备触控类型
	TiXmlComment* pXmlComment = new TiXmlComment("设备触控类型(\"Finger_Touch_WhiteBoard\":手触控电子白板; \"Pen_Touch_WhiteBoard\":笔触控电子白板; \"Finger_Touch_Control\":手指触控;\"Palm_Touch_Control\":手掌互动;");
	pNode->LinkEndChild(pXmlComment);

	TiXmlElement * pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "DeviceTouchType");
	switch (advanceSettings.m_eTouchType)
	{
	   case E_DEVICE_FINGER_TOUCH_WHITEBOARD:
		    pElement->SetAttribute("value", "Finger_Touch_WhiteBoard");
		    break;

	   case E_DEVICE_PEN_TOUCH_WHITEBOARD:
		   pElement->SetAttribute("value", "Pen_Touch_WhiteBoard");
		   break;
	   case E_DEVICE_FINGER_TOUCH_CONTROL:
		   pElement->SetAttribute("value", "Finger_Touch_Control");
		   break;
	   case E_DEVICE_PALM_TOUCH_CONTROL:
		   pElement->SetAttribute("value", "Palm_Touch_Control");
		   break;
	   default:
		   break;

	}//switch

	pNode->LinkEndChild(pElement);

	//是否是背投设备
	pXmlComment = new TiXmlComment("是否是背投设备");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "RearProjectionDevice");
	pElement->SetAttribute("value", advanceSettings.bIsRearProjection ? "Yes" : "No");
	pNode->LinkEndChild(pElement);

    //干扰光斑屏蔽区膨胀半径
    pXmlComment = new TiXmlComment("干扰光斑屏蔽区膨胀半径");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "ClutterMaskingAreaInflationRadius");
    pElement->SetAttribute("value", advanceSettings.nAutoMaskDilateRadius);
    pNode->LinkEndChild(pElement);

    //响应光斑大小比例
    pXmlComment = new TiXmlComment("响应光斑大小比例");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "SpotProportion");
    pElement->SetAttribute("value", advanceSettings.nSpotProportion);
    pNode->LinkEndChild(pElement);

    //板擦与光斑的倍数
    pXmlComment = new TiXmlComment("板擦与光斑的倍数");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "EraserMultiples");
    pElement->SetAttribute("value", advanceSettings.nMultEraser);
    pNode->LinkEndChild(pElement);

    //检测为固定光斑的时间
    pXmlComment = new TiXmlComment("检测为固定光斑的时间");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "SetFixedBlobTime");
    pElement->SetAttribute("value", advanceSettings.nFixedBlobSetTime);
    pNode->LinkEndChild(pElement);

	//使能笔迹插值
	pXmlComment = new TiXmlComment("使能笔迹插值(Yes/No)");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "EnableStrokeInterpolate");
	pElement->SetAttribute("value", advanceSettings.bEnableStrokeInterpolate ? "Yes" : "No");
	pNode->LinkEndChild(pElement);

	//是否打开动态屏蔽功能
	pXmlComment = new TiXmlComment("是否打开动态屏蔽功能");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "DynamicMaskFrame");
	pElement->SetAttribute("value", advanceSettings.bIsDynamicMaskFrame ? "Yes" : "No");
	pNode->LinkEndChild(pElement);

	//是否打开抗干扰功能
	pXmlComment = new TiXmlComment("是否打开抗干扰功能");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "AntiJamming");
	pElement->SetAttribute("value", advanceSettings.bIsAntiJamming ? "Yes" : "No");
	pNode->LinkEndChild(pElement);

	///是否启用手动绘制的静态屏蔽图
	pXmlComment = new TiXmlComment("是否启用手动绘制屏幕区域");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "OnLineScreenArea");
	pElement->SetAttribute("value", advanceSettings.bIsOnLineScreenArea ? "Yes" : "No");
	pNode->LinkEndChild(pElement);
	
    return TRUE;
}

//@功能:载入传感器关联监视器ID
//@参数:pNode, 指向配置文件中<AttachedMonitorIds>节点的指针
//      attachedMonitorIds, 输出参数, 监视器ID数组
BOOL LoadConfig(TiXmlNode *pNode, std::vector<int> & attachedMonitorIds)
{
    if(pNode == NULL) return FALSE;
    const char* szMonitorCount  = ((TiXmlElement*)pNode)->Attribute("count");
    int nMonitorCount = atoi(szMonitorCount);
    attachedMonitorIds.resize(nMonitorCount);

    TiXmlNode * pChild = NULL;

    int nMonitorIndex = 0;
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild)
        {
            break;
        }
        const char* lpszElementName = pChild->Value();
        if(_stricmp(lpszElementName, "Monitor") == 0)
        {
            const char* paramName  = ((TiXmlElement*)pChild)->Attribute("id");
            if(paramName)
            {
                attachedMonitorIds[nMonitorIndex] = atoi(paramName);
            }
            else
            {
                attachedMonitorIds[nMonitorIndex] = 0;
            }
            nMonitorIndex ++;
        }

    }while(pChild);

    return TRUE;
}


//@功能:保存传感器关联监视器ID
//@参数:pNode, 指向配置文件中<AttachedMonitorIds>节点的指针
//      attachedMonitorIds, 输入参数, 监视器ID数组
BOOL SaveConfig(TiXmlNode *pNode, const  std::vector<int> & attachedMonitorIds)
{
    for(size_t i=0; i< attachedMonitorIds.size(); i++)
    {
        TiXmlComment* pXmlComment = new TiXmlComment("监视器");
        pNode->LinkEndChild(pXmlComment);

        TiXmlElement * pMonitor = new TiXmlElement("Monitor");
        pMonitor->SetAttribute("id", attachedMonitorIds[i]);
        pNode->LinkEndChild(pMonitor);
    }//for

    return TRUE;
}

//@功能:载入一个屏幕的校正方程系数
//@参数:pNode, 指向配置文件中<MonitorCalibCoeffients>节点的指针
//      tMonitorCalibCoefs,输出参数, 监视器校正方程系数
BOOL LoadConfig(TiXmlNode *pNode, TMonitorCalibCoefs& tMonitorCalibCoefs)
{
    if(pNode == NULL) return FALSE;

    TiXmlNode * pChild = NULL;

    int nCoefIndex = 0;
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild)
        {
            break;
        }
        const char* lpszElementName = pChild->Value();
        if(_stricmp(lpszElementName, "RECT") == 0)
        {
            const char* attrName  =   ((TiXmlElement*)pChild)->Attribute("name" );
            const char* attrLeft    = ((TiXmlElement*)pChild)->Attribute("left"  );
            const char* attrRight   = ((TiXmlElement*)pChild)->Attribute("right" );
            const char* attrTop     = ((TiXmlElement*)pChild)->Attribute("top"   );
            const char* attrBottom  = ((TiXmlElement*)pChild)->Attribute("bottom");


            if(_stricmp(attrName, "Monitor") == 0)
            {
                if(attrLeft)
                {
                    tMonitorCalibCoefs.rcMonitor.left = atoi(attrLeft);
                }
                if(attrRight)
                {
                    tMonitorCalibCoefs.rcMonitor.right = atoi(attrRight);
                }

                if(attrTop)
                {
                    tMonitorCalibCoefs.rcMonitor.top = atoi(attrTop);
                }

                if(attrBottom)
                {
                    tMonitorCalibCoefs.rcMonitor.bottom = atoi(attrBottom);
                }

            }

        }
        else if(_stricmp(lpszElementName, "Coefficients") == 0)
        {

            const char* szCoefCount  = ((TiXmlElement*)pChild)->Attribute("count");
            int nCoefCount = atoi(szCoefCount);
            tMonitorCalibCoefs.calibCoefs.resize(nCoefCount);

            int coefIndex = 0;

            TiXmlNode * pCoefNode = NULL;
            while(coefIndex < nCoefCount)
            {

                pCoefNode = pChild->IterateChildren(pCoefNode);
                if(pCoefNode == NULL) break;
                const char* lpszCoefElementName = pCoefNode->Value();
                if(_stricmp(lpszCoefElementName, "Param") == 0)
                {
                    const char* szCoefName  = ((TiXmlElement*)pCoefNode)->Attribute("name");
                    const char* szCoefValue = ((TiXmlElement*)pCoefNode)->Attribute("value");
                    if(_stricmp(szCoefName, "Coefficient") == 0 && szCoefValue)
                    {
                        tMonitorCalibCoefs.calibCoefs[coefIndex] = atof(szCoefValue);
                        coefIndex ++;
                    }

                }
            }//while
        }

    }while(pChild);

    return TRUE;
}

//@功能:保存一个校正方程系数
//@参数:pNode, 指向配置文件中<MonitorCalibCoeffients>节点的指针
//      tMonitorCalibCoefs, 输入参数, 监视器校正方程系数
BOOL SaveConfig(TiXmlNode *pNode, const TMonitorCalibCoefs& tMonitorCalibCoefs)
{
    TiXmlComment* pXmlComment = new TiXmlComment("显示器尺寸");
    pNode->LinkEndChild(pXmlComment);

    TiXmlElement * pElement = new TiXmlElement("RECT");
    pElement->SetAttribute("name"  , "Monitor");
    pElement->SetAttribute("left"  , tMonitorCalibCoefs.rcMonitor.left  );
    pElement->SetAttribute("right" , tMonitorCalibCoefs.rcMonitor.right );
    pElement->SetAttribute("top"   , tMonitorCalibCoefs.rcMonitor.top   );
    pElement->SetAttribute("bottom", tMonitorCalibCoefs.rcMonitor.bottom);
    pNode->LinkEndChild(pElement);


    pXmlComment = new TiXmlComment("系数");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Coefficients");
    pElement->SetAttribute("count", tMonitorCalibCoefs.calibCoefs.size());
    pNode->LinkEndChild(pElement);

    TiXmlElement* pCoefs = pElement;

    for(size_t i=0; i< tMonitorCalibCoefs.calibCoefs.size(); i++)
    {
        TiXmlElement * pCoef = new TiXmlElement("Param");
        char szText[128];
        sprintf_s(szText, _countof(szText), "%.20e", tMonitorCalibCoefs.calibCoefs[i]);
        pCoef->SetAttribute("name", "Coefficient");
        pCoef->SetAttribute("value", szText);
        pCoefs->LinkEndChild(pCoef);
    }//for

    return TRUE;

}

//@功能:载入校正方程系数
//@参数:pNode, 指向配置文件中<AttachedMonitorIds>节点的指针
//      coefs,输出参数, 监视器ID数组
BOOL LoadConfig(TiXmlNode *pNode, TCalibParams& calibParams )
{
    if(pNode == NULL) return FALSE;

    TiXmlNode * pChild = NULL;

    int nCoefIndex = 0;
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild)
        {
            break;
        }
        const char* lpszElementName = pChild->Value();
        if(_stricmp(lpszElementName, "Param") == 0)
        {
            const char* paramName  = ((TiXmlElement*)pChild)->Attribute("name");
            const char* paramValue = ((TiXmlElement*)pChild)->Attribute("value");
            if(paramName && paramValue)
            {
                if(_stricmp(paramName, "ImageWidth") == 0)
                {
                    calibParams.szImage.cx = atoi(paramValue);
                }
                else if(_stricmp(paramName, "ImageHeight") == 0)
                {
                    calibParams.szImage.cy = atoi(paramValue);
                }
                else if(_stricmp(paramName, "CalibrateType") == 0)
                {
                    if(_stricmp(paramValue,"Auto") == 0)
                    {
                        calibParams.eCalibType = E_CALIBRATE_TYPE_AUTO;
                    }
                    else
                    {
                        calibParams.eCalibType = E_CALIBRATE_TYPE_MANUAL;
                    }

                }
            }
        }
        else if(_stricmp(lpszElementName, "AllMonitorCalibCoefficients") == 0)
        {

            const char* szMonitorCount  = ((TiXmlElement*)pChild)->Attribute("count");
            int nMonitorCount = atoi(szMonitorCount);
            calibParams.allCalibCoefs.resize(nMonitorCount);

            int nMonitorIndex = 0;


            TiXmlNode * pMonitorParamsNode = NULL;
            while(nMonitorIndex < nMonitorCount)
            {
                pMonitorParamsNode = pChild->IterateChildren(pMonitorParamsNode);
                if(pMonitorParamsNode == NULL) break;

                const char* lpszNodeName = pMonitorParamsNode->Value();
                if(_stricmp(lpszNodeName, "MonitorCalibCoefficients") == 0)
                {
                    LoadConfig(pMonitorParamsNode, calibParams.allCalibCoefs[nMonitorIndex]);
                    nMonitorIndex ++;
                }
            }//while
        }

    }while(pChild);

    return TRUE;
}


//@功能:保存校正方程系数
//@参数:pNode, 指向配置文件中<AttachedMonitorIds>节点的指针
//      attachedMonitorIds, 输入参数, 监视器ID数组
BOOL SaveConfig(TiXmlNode *pNode, const TCalibParams& calibParams)
{
    TiXmlComment* pXmlComment = new TiXmlComment("校正图片尺寸");
    pNode->LinkEndChild(pXmlComment);

    TiXmlElement * pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "ImageWidth");
    pElement->SetAttribute("value", calibParams.szImage.cx);
    pNode->LinkEndChild(pElement);


    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "ImageHeight");
    pElement->SetAttribute("value", calibParams.szImage.cy);
    pNode->LinkEndChild(pElement);

    pXmlComment = new TiXmlComment("校正类型(手动:Manual, 自动:Auto)");
    pNode->LinkEndChild(pXmlComment);
    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "CalibrateType");
    pElement->SetAttribute("value", calibParams.eCalibType == E_CALIBRATE_TYPE_AUTO ?"Auto":"Manual");
    pNode->LinkEndChild(pElement);

    pXmlComment = new TiXmlComment("所有监视器校正参数");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("AllMonitorCalibCoefficients");
    pElement->SetAttribute("count", calibParams.allCalibCoefs.size());
    pNode->LinkEndChild(pElement);

    TiXmlElement* pAllMonitorCoefNode = pElement;

    for(size_t i=0; i< calibParams.allCalibCoefs.size(); i++)
    {
        TiXmlElement * pMonitorCoef = new TiXmlElement("MonitorCalibCoefficients");
        pMonitorCoef->SetAttribute("id", i);
        pAllMonitorCoefNode->LinkEndChild(pMonitorCoef);
        SaveConfig(pMonitorCoef, calibParams.allCalibCoefs[i]);
    }//for

    return TRUE;
}


//@功能:载入镜头规格
//@参数:pNode, 节点
//      lensSpecification， 镜头规格数据结构
BOOL LoadConfig(TiXmlNode *pNode, TLensSpecification &lensSpecification)
{
     TiXmlNode* pChild=NULL;
    do
    {
        pChild = pNode->IterateChildren(pChild);

        if(NULL == pChild) break;

        const char* lpszElementName = pChild->Value();
        if(_stricmp(lpszElementName, "Param") == 0)
        {
           const char* paramName    = ((TiXmlElement*)pChild)->Attribute("name");
           const char* paramValue   = ((TiXmlElement*)pChild)->Attribute("value");

            if(paramName && paramValue && _stricmp(paramName, "backFocalLength") == 0)
            {//后方焦距(像方焦距)
                lensSpecification.backFocalLength = atof(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "FOV_horz") == 0)
            {//水平视角
                lensSpecification.FOV_horz = atof(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "FOV_vert") == 0)
            {//垂直视角
                lensSpecification.FOV_vert = atof(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "FOV_diagonal") == 0)
            {//对角线视角
                lensSpecification.FOV_diagonal = atof(paramValue);
            }
            else if(paramName && paramValue && _stricmp(paramName, "Throw Ratio") == 0)
            {//投射比
                lensSpecification.throwRatio = atof(paramValue);
            }
            
        }
    }while(pChild);

    return TRUE;
}



//@功能:保存镜头规格数据
//@参数:pNode, 节点
//      lensSpecification, 镜头规格数据结构
BOOL SaveConfig(TiXmlNode *pNode, const TLensSpecification &lensSpecification)
{
    //1.镜头后方焦距
    TiXmlComment* pXmlComment = new TiXmlComment("镜头后焦距, 单位:mm");
    pNode->LinkEndChild(pXmlComment);
    TiXmlElement * pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "backFocalLength");
    pElement->SetDoubleAttribute("value", lensSpecification.backFocalLength);
    pNode->LinkEndChild(pElement);

    //2.水平视角
    pXmlComment = new TiXmlComment("水平视角, 单位:度");
    pNode->LinkEndChild(pXmlComment);
    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "FOV_horz");
    pElement->SetDoubleAttribute("value", lensSpecification.FOV_horz);
    pNode->LinkEndChild(pElement);


    //3.垂直视角
    pXmlComment = new TiXmlComment("垂直视角, 单位:度");
    pNode->LinkEndChild(pXmlComment);
    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "FOV_vert");
    pElement->SetDoubleAttribute("value", lensSpecification.FOV_vert);
    pNode->LinkEndChild(pElement);


    //4.对角线视角
    pXmlComment = new TiXmlComment("对角线视角, 单位:度");
    pNode->LinkEndChild(pXmlComment);
    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "FOV_diagonal");
    pElement->SetDoubleAttribute("value", lensSpecification.FOV_diagonal);
    pNode->LinkEndChild(pElement);


    //5.投射比
    pXmlComment = new TiXmlComment("投射比");
    pNode->LinkEndChild(pXmlComment);
    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "Throw Ratio");
    pElement->SetDoubleAttribute("value", lensSpecification.throwRatio);
    pNode->LinkEndChild(pElement);

    return TRUE;
}



//@功能:载入自动校正补偿系数
//@参数:pNode, 节点
//      autoCalibCompCoefs, 自动校正补偿数据结构
BOOL LoadConfig(TiXmlNode *pNode, TAutoCalibCompCoefs &autoCalibCompCoefs)
{
     TiXmlNode* pChild=NULL;
     
     int nCoefficientCount = 0;
    do
    {
        pChild = pNode->IterateChildren(pChild);

        if(NULL == pChild) break;

        const char* lpszElementName = pChild->Value();
        
       
        if( _stricmp(lpszElementName, "Param") == 0 )
        {
            const char* paramName    = ((TiXmlElement*)pChild)->Attribute("name");
            const char* paramValue   = ((TiXmlElement*)pChild)->Attribute("value");
           if(_stricmp(paramName, "CompensateCoefficient") == 0)
           {
			   if(nCoefficientCount < _countof(autoCalibCompCoefs.k))
			   {
				 autoCalibCompCoefs.k[nCoefficientCount] = atof(paramValue);
				 nCoefficientCount ++;
			   }

           }
           else if( _stricmp(paramName, "u0") == 0 )
           {
                autoCalibCompCoefs.u0 = atof(paramValue);
           }
           else if( _stricmp(paramName, "v0") == 0 )
           {
                autoCalibCompCoefs.v0 = atof(paramValue);
           }
        }

    }while(pChild);

    return TRUE;
}



//@功能:保存自动校正补偿系数
//@参数:pNode, 节点
//      autoCalibCompCoefs, 自动校正补偿数据结构
BOOL SaveConfig(TiXmlNode *pNode, const TAutoCalibCompCoefs &autoCalibCompCoefs)
{
     TiXmlElement * pElement = NULL; 
     char szText[1024];
    for(int i=0; i < _countof(autoCalibCompCoefs.k); i++)
    {
        TiXmlElement * pElement = new TiXmlElement("Param");
        pElement->SetAttribute("name", "CompensateCoefficient");
        
        sprintf_s(szText, _countof(szText), "%.20e", autoCalibCompCoefs.k[i]);
        pElement->SetAttribute("value", szText);

        pNode->LinkEndChild(pElement);

    }//for

    TiXmlComment* pXmlComment = new TiXmlComment("归一化后的水平光心坐标");
    pNode->LinkEndChild(pXmlComment);
    pElement = new TiXmlElement("Param");
    
    pElement->SetAttribute("name", "u0");

    sprintf_s(szText, _countof(szText), "%.20e", autoCalibCompCoefs.u0);
    pElement->SetAttribute("value", szText);

    pNode->LinkEndChild(pElement);


    pXmlComment = new TiXmlComment("归一化后的垂直光心坐标");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");

    pElement->SetAttribute("name", "v0");


    sprintf_s(szText, _countof(szText), "%.20e", autoCalibCompCoefs.v0);
    pElement->SetAttribute("value", szText);

    pNode->LinkEndChild(pElement);
    return TRUE;
}


//@功能:载入相机镜头的内部参数和对称畸变参数
//@参数:pNode, 节点
//      autoCalibCompCoefs, 相机镜头的内部参数和对称畸变参数
BOOL LoadConfig(TiXmlNode *pNode, TInternalAndSymmetricDistortParams& internalAndSymmetricDistortParams)
{
	TiXmlNode* pChild = NULL;

	int nCoefficientCount = 0;
	do
	{
		pChild = pNode->IterateChildren(pChild);

		if (NULL == pChild) break;

		const char* lpszElementName = pChild->Value();


		if (_stricmp(lpszElementName, "Param") == 0)
		{
			const char* paramName = ((TiXmlElement*)pChild)->Attribute("name");
			const char* paramValue = ((TiXmlElement*)pChild)->Attribute("value");
			if (paramName && paramValue && _stricmp(paramName, "mu") == 0)
			{
				internalAndSymmetricDistortParams.mu = atof(paramValue);

			}
			else if (paramName && paramValue && _stricmp(paramName, "mv") == 0)
			{
				internalAndSymmetricDistortParams.mv = atof(paramValue);
			}
			else if (paramName && paramValue && _stricmp(paramName, "u0") == 0)
			{
				internalAndSymmetricDistortParams.u0 = atof(paramValue);
			}
			else if (paramName && paramValue && _stricmp(paramName, "v0") == 0)
			{
				internalAndSymmetricDistortParams.v0 = atof(paramValue);
			}
			else if (paramName && paramValue && _stricmp(paramName, "k1") == 0)
			{
				internalAndSymmetricDistortParams.radialDistrt[0] = atof(paramValue);
			}
			else if (paramName && paramValue && _stricmp(paramName, "k2") == 0)
			{
				internalAndSymmetricDistortParams.radialDistrt[1] = atof(paramValue);
			}
			else if (paramName && paramValue && _stricmp(paramName, "k3") == 0)
			{
				internalAndSymmetricDistortParams.radialDistrt[2] = atof(paramValue);
			}
			else if (paramName && paramValue && _stricmp(paramName, "k4") == 0)
			{
				internalAndSymmetricDistortParams.radialDistrt[3] = atof(paramValue);
			}
			else if (paramName && paramValue && _stricmp(paramName, "k5") == 0)
			{
				internalAndSymmetricDistortParams.radialDistrt[4] = atof(paramValue);
			}

		}

	} while (pChild);

	return TRUE;
}



//@功能:保存相机镜头的内部参数和对称畸变参数
//@参数:pNode, 节点
//      internalAndSymmetricDistortParams, 相机镜头的内部参数和对称畸变参数
BOOL SaveConfig(TiXmlNode *pNode, const TInternalAndSymmetricDistortParams &internalAndSymmetricDistortParams)
{
	TiXmlElement * pElement = NULL;
	TiXmlComment* pXmlComment = NULL;
	char szText[1024];
	//mu
	pXmlComment = new TiXmlComment("相机内部参数mu");
	pNode->LinkEndChild(pXmlComment);
	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "mu");
	sprintf_s(szText, _countof(szText), "%.20e", internalAndSymmetricDistortParams.mu);
	pElement->SetAttribute("value", szText);
	pNode->LinkEndChild(pElement);

	
	//mv
	pXmlComment = new TiXmlComment("相机内部参数mv");
	pNode->LinkEndChild(pXmlComment);
	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "mv");
	sprintf_s(szText, _countof(szText), "%.20e", internalAndSymmetricDistortParams.mv);
	pElement->SetAttribute("value", szText);
	pNode->LinkEndChild(pElement);


	//u0
	pXmlComment = new TiXmlComment("相机内部参数u0");
	pNode->LinkEndChild(pXmlComment);
	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "u0");
	sprintf_s(szText, _countof(szText), "%.20e", internalAndSymmetricDistortParams.u0);
	pElement->SetAttribute("value", szText);
	pNode->LinkEndChild(pElement);

	//v0
	pXmlComment = new TiXmlComment("相机内部参数v0");
	pNode->LinkEndChild(pXmlComment);
	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "v0");
	sprintf_s(szText, _countof(szText), "%.20e", internalAndSymmetricDistortParams.v0);
	pElement->SetAttribute("value", szText);
	pNode->LinkEndChild(pElement);

	//k1
	pXmlComment = new TiXmlComment("镜头对称畸变参数k1");
	pNode->LinkEndChild(pXmlComment);
	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "k1");
	sprintf_s(szText, _countof(szText), "%.20e", internalAndSymmetricDistortParams.radialDistrt[0]);
	pElement->SetAttribute("value", szText);
	pNode->LinkEndChild(pElement);

	//k2
	pXmlComment = new TiXmlComment("镜头对称畸变参数k2");
	pNode->LinkEndChild(pXmlComment);
	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "k2");
	sprintf_s(szText, _countof(szText), "%.20e", internalAndSymmetricDistortParams.radialDistrt[1]);
	pElement->SetAttribute("value", szText);
	pNode->LinkEndChild(pElement);

	//k3
	pXmlComment = new TiXmlComment("镜头对称畸变参数k3");
	pNode->LinkEndChild(pXmlComment);
	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "k3");
	sprintf_s(szText, _countof(szText), "%.20e", internalAndSymmetricDistortParams.radialDistrt[2]);
	pElement->SetAttribute("value", szText);
	pNode->LinkEndChild(pElement);

	//k4
	pXmlComment = new TiXmlComment("镜头对称畸变参数k4");
	pNode->LinkEndChild(pXmlComment);
	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "k4");
	sprintf_s(szText, _countof(szText), "%.20e", internalAndSymmetricDistortParams.radialDistrt[3]);
	pElement->SetAttribute("value", szText);
	pNode->LinkEndChild(pElement);

	//k5
	pXmlComment = new TiXmlComment("镜头对称畸变参数k5");
	pNode->LinkEndChild(pXmlComment);
	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "k5");
	sprintf_s(szText, _countof(szText), "%.20e", internalAndSymmetricDistortParams.radialDistrt[4]);
	pElement->SetAttribute("value", szText);
	pNode->LinkEndChild(pElement);


	return TRUE;
}



//@功能:保存根据镜头类型设定的参数
//@参数:lpszConfigFilePath,配置文件完整路径
//      lensConfig, 输出参数,保存读取的镜头参数配置。
BOOL LoadConfig(TiXmlNode *pNode, TLensConfig& lensConfig)
{
    if(pNode == NULL) return FALSE;
    TiXmlNode* pChild=NULL;

	//lensConfig.bInternalAndSymmetricDistortParamsIsValid = FALSE;
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild)
        {
            break;
        }
        const char* lpszElementName = pChild->Value();

        if(_stricmp(lpszElementName, "NormalUsage-FingerTouchWhiteBoard") == 0)
        {//手触正常使用模式参数
            LoadConfig(pChild, lensConfig.normalUsageSettings_FingerTouchWhiteBoard);
        }
        else if(_stricmp(lpszElementName, "NormalUsage-PenTouchWhiteBoard") == 0)
        {//笔触正常使用模式参数
            LoadConfig(pChild, lensConfig.normalUsageSettings_PenTouchWhiteBoard);
        }
		else if (_stricmp(lpszElementName, "NormalUsage-FingerTouchControl") == 0)
		{
			LoadConfig(pChild,lensConfig.normalUsageSettings_FingerTouchControl);
		}
		else if (_stricmp(lpszElementName, "NormalUsage-PalmTouchControl") == 0)
		{
			LoadConfig(pChild, lensConfig.normalUsageSettings_PalmTouchControl);
		}
        else if(_stricmp(lpszElementName, "InstallTunning") == 0)
        {//安装调试时的参数
            LoadConfig(pChild, lensConfig.installTunningSettings);
        }
        else if(_stricmp(lpszElementName, "LaserTunning-WhiteBoard") == 0)
        {  //电子白板激光器调试模式参数
            LoadConfig(pChild, lensConfig.laserTunningSettings_WhiteBoard);
        }
		else if(_stricmp(lpszElementName, "LaserTunning-FingerTouchControl") == 0)
		{  //手指触控激光器调试模式参数
			LoadConfig(pChild, lensConfig.laserTunningSettings_FingerTouchControl);
		}
		else if(_stricmp(lpszElementName, "LaserTunning-PalmTouchControl") == 0)
		{	//手掌互动激光器调试模式参数
			LoadConfig(pChild, lensConfig.laserTunningSettings_PalmTouchControl);
		}
        else if(_stricmp(lpszElementName, "AutoCalibrateParamsList") == 0)
        {//自动校正参数
            LoadConfig(pChild, lensConfig.autoCalibrateSettingsList);
        }
        else if(_stricmp(lpszElementName, "AutoMask") == 0)
        {//自动屏蔽参数
            LoadConfig(pChild, lensConfig.autoMaskSettings);
        }
        else if(_stricmp(lpszElementName,"LensSpec") == 0)
        {//镜头规格参数
            LoadConfig(pChild, lensConfig.lensSpecification);
        }
        else if(_stricmp(lpszElementName, "AutoCalibCompensateCoefs") == 0)
        {//自动校正补偿参数
            LoadConfig(pChild, lensConfig.autoCalibCompCoefs);
        }
		else if(_stricmp(lpszElementName, "LensInternalAndSymmetricDistortionParams") == 0)
		{	//相机镜头的内部参数和对称畸变参数
			LoadConfig(pChild, lensConfig.lensInternalAndSymmetricDistortParams);
			lensConfig.bInternalAndSymmetricDistortParamsIsValid = TRUE;
		}
		else if (_stricmp(lpszElementName, "Param") == 0)
		{
			const char* paramName = ((TiXmlElement*)pChild)->Attribute("name");

			if (_stricmp(paramName, "GuideBox") == 0)
			{
				const char* paramValue = ((TiXmlElement*)pChild)->Attribute("width");
				if (paramValue)
				{
					lensConfig.Referwidth = atoi(paramValue);
				}

				paramValue = ((TiXmlElement*)pChild)->Attribute("height");
				if (paramValue)
				{
					lensConfig.ReferHeight = atoi(paramValue);
				}

				paramValue = ((TiXmlElement*)pChild)->Attribute("left");
				if (paramValue)
				{
					lensConfig.rcGuideRectangle.left = atoi(paramValue);
				}

				paramValue = ((TiXmlElement*)pChild)->Attribute("top");
				if (paramValue)
				{
					lensConfig.rcGuideRectangle.top = atoi(paramValue);
				}

				paramValue = ((TiXmlElement*)pChild)->Attribute("right");
				if (paramValue)
				{
					lensConfig.rcGuideRectangle.right = atoi(paramValue);
				}

				paramValue = ((TiXmlElement*)pChild)->Attribute("bottom");
				if (paramValue)
				{
					lensConfig.rcGuideRectangle.bottom = atoi(paramValue);
				}

				paramValue = ((TiXmlElement*)pChild)->Attribute("color");
				if (paramValue)
				{
					DWORD dwColor = 0xFF0000;
					sscanf_s(paramValue, "%x", &dwColor);
					lensConfig.dwGuideRectangleColor = dwColor;
				}

				paramValue = ((TiXmlElement*)pChild)->Attribute("visible");
				if (paramValue && _stricmp(paramValue, "Yes") == 0)
				{
					lensConfig.bRectangleVisible = TRUE;
				}
				else
				{
					lensConfig.bRectangleVisible = FALSE;
				}
			}
		}
    }while(pChild); 

    return TRUE;
}


BOOL SaveConfig(TiXmlNode *pNode, const TLensConfig& lensConfig)
{
	
	//安装引导框位置
	TiXmlComment* pXmlComment = new TiXmlComment("引导框的位置和颜色, value range: left=[0-639]; top=[0-479]; width=[0-640]; height=[0-480]; color=[RRGGBB] RGB color value is defined in hexadecimal format, e.g: FF0000 is red,  00FF00 is green, 0000FF is blue");
	pNode->LinkEndChild(pXmlComment);

	TiXmlElement* pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "GuideBox");
	pElement->SetAttribute("width", lensConfig.Referwidth);
	pElement->SetAttribute("height", lensConfig.ReferHeight);

	pElement->SetAttribute("left", lensConfig.rcGuideRectangle.left);
	pElement->SetAttribute("top", lensConfig.rcGuideRectangle.top);
	pElement->SetAttribute("right",lensConfig.rcGuideRectangle.right);
	pElement->SetAttribute("bottom", lensConfig.rcGuideRectangle.bottom);

//	long width  = lensConfig.rcGuideRectangle.right  - lensConfig.rcGuideRectangle.left;
//	long height = lensConfig.rcGuideRectangle.bottom - lensConfig.rcGuideRectangle.top;

//	pElement->SetAttribute("width", width);
//	pElement->SetAttribute("height", height);

	char data[32];
	sprintf_s(data, _countof(data), "%x", lensConfig.dwGuideRectangleColor);
	pElement->SetAttribute("color", data);

	pElement->SetAttribute("visible", lensConfig.bRectangleVisible ? "Yes" : "No");
	pNode->LinkEndChild(pElement);


    //手指触控电子白板模式下的正常使用参数
    pXmlComment = new TiXmlComment("电子白板手指触控模式下的正常使用参数");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("NormalUsage-FingerTouchWhiteBoard");
    pNode->LinkEndChild(pElement);
    SaveConfig(pElement, lensConfig.normalUsageSettings_FingerTouchWhiteBoard);


    //笔触控电子白板模式下的正常使用参数
    pXmlComment = new TiXmlComment("电子白板笔触控模式下的正常使用参数");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("NormalUsage-PenTouchWhiteBoard");
    pNode->LinkEndChild(pElement);
    SaveConfig(pElement, lensConfig.normalUsageSettings_PenTouchWhiteBoard);

	//手指触控模式下的正常使用参数
	pXmlComment = new TiXmlComment("手指触控模式下的正常使用参数");
	pNode->LinkEndChild(pXmlComment);
	pElement = new TiXmlElement("NormalUsage-FingerTouchControl");
	pNode->LinkEndChild(pElement);
	SaveConfig(pElement,lensConfig.normalUsageSettings_FingerTouchControl);

	//手掌触控模式下的正常使用参数
	pXmlComment = new TiXmlComment("手掌触控模式下的正常使用参数");
	pNode->LinkEndChild(pXmlComment);
	pElement = new TiXmlElement("NormalUsage-PalmTouchControl");
	pNode->LinkEndChild(pElement);
	SaveConfig(pElement, lensConfig.normalUsageSettings_PalmTouchControl);

    //安装调试时的模式参数
    pXmlComment = new TiXmlComment("安装调试模式参数");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("InstallTunning");
    pNode->LinkEndChild(pElement);
    SaveConfig(pElement, lensConfig.installTunningSettings);


    //激光器调试模式参数
    pXmlComment = new TiXmlComment("电子白板激光器调试模式参数");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("LaserTunning-WhiteBoard");
    pNode->LinkEndChild(pElement);
    SaveConfig(pElement, lensConfig.laserTunningSettings_WhiteBoard);

	//激光器调试模式参数
	pXmlComment = new TiXmlComment("手指触控激光器调试模式参数");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("LaserTunning-FingerTouchControl");
	pNode->LinkEndChild(pElement);
	SaveConfig(pElement, lensConfig.laserTunningSettings_FingerTouchControl);

	//激光器调试模式参数
	pXmlComment = new TiXmlComment("手掌触控激光器调试模式参数");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("LaserTunning-PalmTouchControl");
	pNode->LinkEndChild(pElement);
	SaveConfig(pElement, lensConfig.laserTunningSettings_PalmTouchControl);


    //自动校正参数
    pXmlComment = new TiXmlComment("自动校正参数列表");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("AutoCalibrateParamsList");
    pNode->LinkEndChild(pElement);
    SaveConfig(pElement, lensConfig.autoCalibrateSettingsList);

    //自动屏蔽参数
    pXmlComment = new TiXmlComment("自动屏蔽参数");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("AutoMask");
    pNode->LinkEndChild(pElement);
    SaveConfig(pElement, lensConfig.autoMaskSettings);


    //镜头规格
    pXmlComment = new TiXmlComment("镜头规格");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("LensSpec");
    pNode->LinkEndChild(pElement);
    SaveConfig(pElement, lensConfig.lensSpecification);

    //自动校正补偿系数
    pXmlComment = new TiXmlComment("自动校正补偿系数");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("AutoCalibCompensateCoefs");
    pNode->LinkEndChild(pElement);
    SaveConfig(pElement, lensConfig.autoCalibCompCoefs);


	//相机内部和对称畸变参数
	if (lensConfig.bInternalAndSymmetricDistortParamsIsValid)
	{
		pXmlComment = new TiXmlComment("相机内部和对称畸变参数");
		pNode->LinkEndChild(pXmlComment);

		pElement = new TiXmlElement("LensInternalAndSymmetricDistortionParams");
		pNode->LinkEndChild(pElement);
		SaveConfig(pElement, lensConfig.lensInternalAndSymmetricDistortParams);
	}
    return TRUE;
}


//@功能:保存根据镜头类型设定的参数
//@参数:lpszConfigFilePath,配置文件完整路径
//      lensConfig, 输出参数,保存读取的镜头参数配置。
BOOL LoadConfig(LPCTSTR lpszConfigFilePath, TLensConfig& lensConfig)
{
    TiXmlDocument oXMLDoc;
    if (!oXMLDoc.LoadFile(CT2A(lpszConfigFilePath),TIXML_ENCODING_UTF8))
    {
        return FALSE;
    }
    TiXmlElement *pRootElement = oXMLDoc.RootElement();
    if(pRootElement == NULL)
    {
        return FALSE;
    }

    LoadConfig(pRootElement, lensConfig);
    return TRUE;
}

//@功能:保存根据镜头类型设定的参数
//
BOOL SaveConfig(LPCTSTR lpszConfigFilePath, const TLensConfig& lensConfig)
{
    TiXmlDocument oXMLDoc;
    TiXmlDeclaration Declaration("1.0","UTF-8","no");
    oXMLDoc.InsertEndChild(Declaration);

    TiXmlComment*  pXmlComment = new TiXmlComment("镜头配置参数");
    oXMLDoc.LinkEndChild(pXmlComment);

    TiXmlElement * pLensConfig = new TiXmlElement("LensConfig");
    oXMLDoc.LinkEndChild(pLensConfig);


    SaveConfig(pLensConfig, lensConfig);


    //以UTF-8编码格式保存
    TiXmlPrinter  printer;
    oXMLDoc.Accept(&printer);

    char UTF8BOM[3]={'\xEF','\xBB','\xBF'};

    std::ofstream theFile;
    //注意:
    //代码若为:theFile.open(CT2A(lpszConfigFilePath) , ios_base::out) ;
    //则路径中若有中文字符，
    //mbstowc_s(NULL,wc_Name, FILENAME_MAX,fileName,FILENAMEMAX-1)
    //返回的wc_name中为乱码
    //
    theFile.open(CT2W(lpszConfigFilePath) , ios_base::out) ;

    if(theFile.is_open())
    {
        theFile.write(UTF8BOM,3) ;

        int length = strlen(printer.CStr());

        char* utf_8_buf = (char*)malloc(length*4);
		memset(utf_8_buf, 0, length*4);

        //Unicode转为UTF8编码
        WideCharToMultiByte(
            CP_UTF8,
            0,
            CA2W(
				printer.CStr(), 
				936 //gb2312
			),
            length,
            utf_8_buf,
            length*4,
            NULL,
            NULL);


        theFile.write(utf_8_buf, strlen(utf_8_buf));

        theFile.close();

        free(utf_8_buf);
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

BOOL SaveConfig(TiXmlNode *pNode, const TSensorModeConfig & sensorModeCfg, int nModeIndex, int nSensorId)
{
	//高级设置参数
	TiXmlComment* pXmlComment = new TiXmlComment("高级设置参数");
	pNode->LinkEndChild(pXmlComment);

	TiXmlElement * pElement = new TiXmlElement("AdvanceSettings");
	pNode->LinkEndChild(pElement);
	SaveConfig(pElement, sensorModeCfg.advanceSettings);

	//手动校正参数
	pXmlComment = new TiXmlComment("手动校正参数");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("ManualCalibrate");
	pNode->LinkEndChild(pElement);
	SaveConfig(pElement, sensorModeCfg.manualCalibrateSetting);


	//校正方程系数
	pXmlComment = new TiXmlComment("校正方程参数");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("CalibateEquationParams");
	pNode->LinkEndChild(pElement);
	SaveConfig(pElement, sensorModeCfg.calibParam);

	//保存各种镜头的配置参数
	TCHAR szPath[MAX_PATH];
    _stprintf_s(
        szPath,
        _countof(szPath), 
        _T("%s\\Sensor%02d\\%s\\%s"), 
        (LPCTSTR)PROFILE::SETTINGS_BASE_DIRECTORY, 
        nSensorId,
        GetProjectModeString(EProjectionMode(nModeIndex)),
        GetCameraTypeString(g_tSysCfgData.globalSettings.eCameraType));

	//if (nModeIndex==0)
	//{
	//	switch (g_tSysCfgData.globalSettings.eCameraType)
	//	{
	//	    case E_CAMERA_MODEL_0:
	//			_stprintf_s(szPath, _countof(szPath), _T("%s\\Sensor%02d\\DesktopMode\\OV7725"), (const char*)CT2A(PROFILE::SETTINGS_BASE_DIRECTORY), nSensorId);
	//			break;

	//		case  E_CAMERA_MODEL_1:
 //               _stprintf_s(szPath, _countof(szPath), _T("%s\\Sensor%02d\\DesktopMode\\QC0308", (const char*)CT2A(PROFILE::SETTINGS_BASE_DIRECTORY), nSensorId);
	//			break;

	//		case  E_CAMERA_MODEL_2:
 //               _stprintf_s(szPath, _countof(szPath), "%s\\Sensor%02d\\DesktopMode\\OV2710", (const char*)CT2A(PROFILE::SETTINGS_BASE_DIRECTORY), nSensorId);
	//			break;
	//		default:
	//			break;
	//	}
	//}
	//else {
	//	switch (g_tSysCfgData.globalSettings.eCameraType)
	//	{
	//	     case E_CAMERA_MODEL_0:
	//			 sprintf_s(szPath, _countof(szPath), "%s\\Sensor%02d\\WallMode\\OV7725", (const char*)CT2A(PROFILE::SETTINGS_BASE_DIRECTORY), nSensorId);
	//			 break;
	//		 case  E_CAMERA_MODEL_1:
	//			 sprintf_s(szPath, _countof(szPath), "%s\\Sensor%02d\\WallMode\\QC0308", (const char*)CT2A(PROFILE::SETTINGS_BASE_DIRECTORY), nSensorId);
	//			 break;
	//		case E_CAMERA_MODEL_2:
	//			sprintf_s(szPath, _countof(szPath), "%s\\Sensor%02d\\WallMode\\OV2710", (const char*)CT2A(PROFILE::SETTINGS_BASE_DIRECTORY), nSensorId);
	//			break;
	//		default:
	//			break;

	//	}
	//}

    //递归创建子目录
	//CreateFullDirectory(CA2CT(szPath));
    CreateFullDirectory(szPath);

	for (int i = 0; i < int(E_LENS_TYPE_COUNT); i++)
	{
		//保存各种镜头的配置参数
		TCHAR szPathRatio[MAX_PATH];
		memset(szPathRatio, 0, sizeof(szPathRatio));
		_stprintf_s(szPathRatio, _countof(szPathRatio), _T("%s\\throw_ratio(%.2f).dll"), szPath, TRHOW_RATIO_LIST[i]);


		//if (PathFileExists(szPathRatio)
		//{
        SaveConfig((szPathRatio), sensorModeCfg.lensConfigs[i]);
		//}
	}
	return TRUE;
}


BOOL LoadConfig(TiXmlNode *pNode, TSensorModeConfig & sensorModeCfg, int nModeIndex, int nSensorId)
{
	if (pNode == NULL) return FALSE;
	TiXmlNode* pChild = NULL;
	do
	{
		pChild = pNode->IterateChildren(pChild);
		if (NULL == pChild)
		{
			break;
		}
		const char* lpszElementName = pChild->Value();
		if (_stricmp(lpszElementName, "AdvanceSettings") == 0)
		{	//高级设置参数
			LoadConfig(pChild, sensorModeCfg.advanceSettings);
		}
		else if (_stricmp(lpszElementName, "ManualCalibrate") == 0)
		{   //手动校正参数
			LoadConfig(pChild, sensorModeCfg.manualCalibrateSetting);
		}
		else if (_stricmp(lpszElementName, "CalibateEquationParams") == 0)
		{  //校正方程参数
			LoadConfig(pChild, sensorModeCfg.calibParam);
		}
	} while(pChild);

	//////////////////////////////////
	///保存各种镜头的配置参数
    /*
	char szPath[MAX_PATH];

	if (nModeIndex == 0)
	{   ////说明是桌面模式
		////高清摄像头PID=0x9230,VID=0x05a3
		switch (g_tSysCfgData.globalSettings.eCameraType)
		{
		   case E_CAMERA_MODEL_0:
			   sprintf_s(szPath, _countof(szPath), "%s\\Sensor%02d\\DesktopMode\\OV7725", (const char*)CT2A(PROFILE::SETTINGS_BASE_DIRECTORY), nSensorId);
			    break;
		   case E_CAMERA_MODEL_1:
			   sprintf_s(szPath, _countof(szPath), "%s\\Sensor%02d\\DesktopMode\\QC0308", (const char*)CT2A(PROFILE::SETTINGS_BASE_DIRECTORY), nSensorId);
				break;
		   case E_CAMERA_MODEL_2:

			   sprintf_s(szPath, _countof(szPath), "%s\\Sensor%02d\\DesktopMode\\OV2710", (const char*)CT2A(PROFILE::SETTINGS_BASE_DIRECTORY), nSensorId);
				break;
		   default:
			   break;
		}
	}
	else 
	{
		switch (g_tSysCfgData.globalSettings.eCameraType)
		{
			case E_CAMERA_MODEL_0:
				sprintf_s(szPath, _countof(szPath), "%s\\Sensor%02d\\WallMode\\OV7725", (const char*)CT2A(PROFILE::SETTINGS_BASE_DIRECTORY), nSensorId);
				break;
			case E_CAMERA_MODEL_1:
				sprintf_s(szPath, _countof(szPath), "%s\\Sensor%02d\\WallMode\\QC0308", (const char*)CT2A(PROFILE::SETTINGS_BASE_DIRECTORY), nSensorId);
				break;
			case E_CAMERA_MODEL_2:
				sprintf_s(szPath, _countof(szPath), "%s\\Sensor%02d\\WallMode\\OV2710", (const char*)CT2A(PROFILE::SETTINGS_BASE_DIRECTORY), nSensorId);
				break;
			default:
				break;
		}
	}
    */

    TCHAR szPath[MAX_PATH];
    _stprintf_s(
        szPath,
        _countof(szPath),
        _T("%s\\Sensor%02d\\%s\\%s"),
        (LPCTSTR)PROFILE::SETTINGS_BASE_DIRECTORY,
        nSensorId,
        GetProjectModeString(EProjectionMode(nModeIndex)),
        GetCameraTypeString(g_tSysCfgData.globalSettings.eCameraType));


	//CreateFullDirectory(CA2CT(szPath));
    CreateFullDirectory(szPath);
	for (int i = 0; i < int(E_LENS_TYPE_COUNT); i++)
	{
		//载入各种镜头的配置参数
		//char szPathRatio[MAX_PATH];
        TCHAR szPathRatio[MAX_PATH];
		memset(szPathRatio, 0, sizeof(szPathRatio));
		//sprintf_s(szPathRatio, _countof(szPathRatio), "%s\\throw_ratio(%.2f).dll", szPath, TRHOW_RATIO_LIST[i]);
        _stprintf_s(szPathRatio, _countof(szPathRatio), _T("%s\\throw_ratio(%.2f).dll"), szPath, TRHOW_RATIO_LIST[i]);

		//if (PathFileExists(CA2CT(szPathRatio)))
        if(PathFileExists(szPathRatio))
		{
			//LoadConfig(CA2CT(szPathRatio), sensorModeCfg.lensConfigs[i]);
            LoadConfig(szPathRatio, sensorModeCfg.lensConfigs[i]);
		}
	}

	return TRUE;
}

//@功能:载入一个图像传感器的信息
//@参数:pNode, 指向配置文件中<IWBSensor>节点的指针
//      sensorCfg, 输出参数, 图像传感器配置信息
//      nSensorId, 传感器编号
BOOL LoadConfig(TiXmlNode *pNode, TSensorConfig & sensorCfg, int nSensorId)
{
    if(pNode == NULL) return FALSE;
    TiXmlNode* pChild=NULL;
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild)
        {
            break;
        }
        const char* lpszElementName = pChild->Value();
        if(_stricmp(lpszElementName, "Param") == 0)
        {
            const char* paramName  = ((TiXmlElement*)pChild)->Attribute("name");
            const char* paramValue = ((TiXmlElement*)pChild)->Attribute("value");
            if(paramName && paramValue && _stricmp(paramName, "DevicePath") == 0)
            {
                sensorCfg.strFavoriteDevicePath = paramValue;
            }
            else if(paramName && paramValue && _stricmp(paramName, "VideoFormat") == 0)
            {
                sensorCfg.strFavoriteMediaType = paramValue;
            }
            else if(paramName && paramValue && _stricmp(paramName, "LensType") == 0)
            {
                double throwRatio;                
                int nGet =  sscanf_s(paramValue, "throw ratio: %lf", &throwRatio);//注意使用%lf格式, 不要使用%f
                if(nGet == 1)
                {
                    int i = 0;
                    bool bFound = false;
                    for(; i < sizeof(TRHOW_RATIO_LIST)/sizeof(double); i++)
                    {
                        if(throwRatio == TRHOW_RATIO_LIST[i])
                        {
                            bFound = true;
                            break;
                        }

                    }
                    if( bFound && i < E_LENS_TYPE_COUNT)
                    {
                         sensorCfg.eSelectedLensType = ELensType(i);
                    }
                }
            }
			else if (paramName && paramValue && _stricmp(paramName, "AutoAttachMonitor") == 0)
			{
				sensorCfg.bAutoAttachMonitor = (_stricmp(paramValue, "Auto") == 0) ? TRUE : FALSE;
			}
			else if (paramName && paramValue && _stricmp(paramName, "AttachedMonitorId") == 0)
			{
				sensorCfg.nAttachedMonitorId = atoi(paramValue);
			}
			else if (paramName && paramValue && _stricmp(paramName, "AttachedMonitorAreaType") == 0)
			{
				if(_stricmp(paramValue, "Left Half") == 0)
				{
					sensorCfg.eMonitorAreaType = E_MONITOR_AREA_TYPE_LEFT_HALF;
				}
				else if (_stricmp(paramValue, "Right Half") == 0)
				{
					sensorCfg.eMonitorAreaType = E_MONITOR_AREA_TYPE_RIGHT_HALF;
				}
				else
				{
					sensorCfg.eMonitorAreaType = E_MONITOR_AREA_TYPE_FULLSCREEN;
				}
			}
        }
		else if (_stricmp(lpszElementName, "IWBProjectionMode") == 0)
		{
			const char* szCoefMode = ((TiXmlElement*)pChild)->Attribute("Mode");
			int nModeIndex = 0;
			if (_stricmp(szCoefMode, "Desktop") == 0)
			{
				//载入桌面的设置参数	
				nModeIndex = 0;
			}
			else if (_stricmp(szCoefMode, "Wall") == 0)
			{
				//载入墙面的设置参数
				nModeIndex = 1;
			}
			LoadConfig(pChild, sensorCfg.vecSensorModeConfig[nModeIndex], nModeIndex,nSensorId);
		}

    }while(pChild);
    return TRUE;
}

//@功能:保存一个图像传感器的信息
//@参数:pNode, 指向配置文件中<IWBSensor>节点的指针
//      sensorCfg, 输入参数, 传感器配置信息
//      nSensorId, 传感器编号
BOOL SaveConfig(TiXmlNode *pNode, const TSensorConfig& sensorCfg, int nSensorId)
{
    //设备路径
    TiXmlComment* pXmlComment = new TiXmlComment("设备路径");
    pNode->LinkEndChild(pXmlComment);

    TiXmlElement * pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "DevicePath");
    pElement->SetAttribute("value", CT2A(sensorCfg.strFavoriteDevicePath));
    pNode->LinkEndChild(pElement);

    //视频格式
    pXmlComment = new TiXmlComment("视频格式");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "VideoFormat");
    pElement->SetAttribute("value", CT2A(sensorCfg.strFavoriteMediaType));
    pNode->LinkEndChild(pElement);


    //当前选择的镜头类型, 
    pXmlComment = new TiXmlComment("镜头类型)");
    pNode->LinkEndChild(pXmlComment);

    pElement = new TiXmlElement("Param");
    pElement->SetAttribute("name", "LensType");


    char szValue[32];
    sprintf_s(szValue, _countof(szValue), "throw ratio: %.2f",  TRHOW_RATIO_LIST[sensorCfg.eSelectedLensType]);
    pElement->SetAttribute("value", szValue);
    pNode->LinkEndChild(pElement);

	//传感器与屏幕关联方式
	pXmlComment = new TiXmlComment("传感器与屏幕关联方法(Auto/Manual)");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "AutoAttachMonitor");
	pElement->SetAttribute("value", sensorCfg.bAutoAttachMonitor ? "Auto" : "Manual");
	pNode->LinkEndChild(pElement);

	//传感器关联的屏幕id
	pXmlComment = new TiXmlComment("传感器关联的屏幕id");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "AttachedMonitorId");
	pElement->SetAttribute("value", sensorCfg.nAttachedMonitorId);
	pNode->LinkEndChild(pElement);

	//关联的屏幕区域类型
	pXmlComment = new TiXmlComment("关联的屏幕区域类型(Full Screen|Left Half|Right Half");
	pNode->LinkEndChild(pXmlComment);

	pElement = new TiXmlElement("Param");
	pElement->SetAttribute("name", "AttachedMonitorAreaType");
	switch (sensorCfg.eMonitorAreaType)
	{
		case E_MONITOR_AREA_TYPE_LEFT_HALF:
			pElement->SetAttribute("value", "Left Half");
			break;

		case E_MONITOR_AREA_TYPE_RIGHT_HALF:
			pElement->SetAttribute("value", "Right Half");
			break;

		default:
			pElement->SetAttribute("value", "Full Screen");
			break;
	}
	pNode->LinkEndChild(pElement);

	pXmlComment = new TiXmlComment("投影机放置方式(\"Desktop\":桌面模式; \"Wall\":墙面模式");
	pNode->LinkEndChild(pXmlComment);

	for (size_t i = 0; i< sensorCfg.vecSensorModeConfig.size() ;i++)
	{
		pElement = new TiXmlElement("IWBProjectionMode");
		if (i == 0 )
		{
			pElement->SetAttribute("Mode", "Desktop");
		}
		else if (i ==1 )
		{
			pElement->SetAttribute("Mode", "Wall");
		}
		pNode->LinkEndChild(pElement);

		SaveConfig(pElement, sensorCfg.vecSensorModeConfig[i], i,nSensorId);
	}
    return TRUE;
}

//@功能:载入所有图像传感器的配置信息
//@参数:pNode, 指向<IWBSensors>节点的指针
//      allSensorCfg, 配置信息数组
BOOL LoadConfig(TiXmlNode *pNode, std::vector<TSensorConfig> & allSensorCfg,int nModeIndex)
{
    if(pNode == NULL) return FALSE;
    const char* szSensorCount  = ((TiXmlElement*)pNode)->Attribute("count");
    if(szSensorCount == 0) return FALSE;

    //暂时不用"count"属性
    int nSensorCount = atoi(szSensorCount);
    //
    allSensorCfg.resize(SENSOR_NUMBER);
    TiXmlNode * pChild = NULL;

    int nSensorIndex = 0;
    do
    {
        pChild = pNode->IterateChildren(pChild);
        if(NULL == pChild)
        {
            break;
        }
        const char* lpszElementName = pChild->Value();
        if(_stricmp(lpszElementName, "IWBSensor") == 0)
        {
            LoadConfig(pChild, allSensorCfg[nSensorIndex], nModeIndex);
            nSensorIndex ++;
            if(nSensorIndex == allSensorCfg.size())
            {
                break;
            }
        }
    }while(pChild);
    return TRUE;
}

//@功能:载入配置文件
//@参数:pNode,
//      allSensorCfg,
BOOL SaveConfig(TiXmlNode *pNode, const std::vector<TSensorConfig>& allSensorCfg, int nModeIndex)
{

    for(size_t i=0; i< allSensorCfg.size(); i++)
    {
        TiXmlComment* pXmlComment = new TiXmlComment("摄像头传感器的配置参数");
        pNode->LinkEndChild(pXmlComment);

        TiXmlElement * pIWBSensor = new TiXmlElement("IWBSensor");
        pIWBSensor->SetAttribute("id", i);
        pNode->LinkEndChild(pIWBSensor);

        SaveConfig(pIWBSensor, allSensorCfg[i], nModeIndex);
    }//for

    return TRUE;
}

//@功能:载入配置文件
//@参数:pNode,
//      allSensorCfg,

BOOL SaveConfig(TiXmlNode *pNode, const std::vector<TSensorConfig>& allSensorCfg)
{
	for (size_t i = 0; i< allSensorCfg.size(); i++)
	{
		TiXmlComment* pXmlComment = new TiXmlComment("摄像头传感器的配置参数");
		pNode->LinkEndChild(pXmlComment);

		TiXmlElement * pIWBSensor = new TiXmlElement("IWBSensor");
		pIWBSensor->SetAttribute("id", i);
		pNode->LinkEndChild(pIWBSensor);

		SaveConfig(pIWBSensor, allSensorCfg[i], i);
	}//for

	return TRUE;
}

//@功能:载入所有图像传感器的配置信息
//@参数:pNode, 指向<IWBSensors>节点的指针
//      allSensorCfg, 配置信息数组
BOOL LoadConfig(TiXmlNode *pNode, std::vector<TSensorConfig> & allSensorCfg)
{
	if (pNode == NULL) return FALSE;
	const char* szSensorCount = ((TiXmlElement*)pNode)->Attribute("count");
	if (szSensorCount == 0) return FALSE;

	//暂时不用"count"属性
	int nSensorCount = atoi(szSensorCount);
	//
	allSensorCfg.resize(SENSOR_NUMBER);

	TiXmlNode * pChild = NULL;


	int nSensorIndex = 0;
	do
	{
		pChild = pNode->IterateChildren(pChild);
		if (NULL == pChild)
		{
			break;
		}
		const char* lpszElementName = pChild->Value();
		if (_stricmp(lpszElementName, "IWBSensor") == 0)
		{
			LoadConfig(pChild, allSensorCfg[nSensorIndex], nSensorIndex);
			nSensorIndex++;
			if (nSensorIndex == allSensorCfg.size())
			{
				break;
			}
		}

	} while (pChild);

	return TRUE;
}

//@功能:载入配置文件
//@参数:lpszConfigFilePath, 配置文件的完整路路径
BOOL LoadConfig(LPCTSTR lpszConfigFilePath, TSysConfigData& sysCfgData, int PID,int VID)
{
	if (PID == 13200 && VID == 6380)
	{
		sysCfgData.globalSettings.eCameraType = E_CAMERA_MODEL_1;
	}
	else if (PID == 37424 && VID == 1443)
	{
		sysCfgData.globalSettings.eCameraType = E_CAMERA_MODEL_2;
	}
	else {
		sysCfgData.globalSettings.eCameraType = E_CAMERA_MODEL_0;
	}

    TiXmlDocument oXMLDoc;
    if (!oXMLDoc.LoadFile(CT2A(lpszConfigFilePath),TIXML_ENCODING_UTF8))
    {
        return FALSE;
    }

    TiXmlElement *pRootElement = oXMLDoc.RootElement();
    if(pRootElement == NULL)
    {
        return FALSE;
    }

    TiXmlNode *pNode = NULL;
    do
    {
        pNode = pRootElement->IterateChildren(pNode);
        if(NULL == pNode )
        {
            break;
        }
        const char* lpszElementName = pNode->Value();

		if (_stricmp(lpszElementName, "Global") == 0)
		{   //载入全局设置
			LoadConfig(pNode, sysCfgData.globalSettings);
	    }
		else if (_stricmp(lpszElementName, "IWBSensors") == 0)
		{
			//所有传感器的配置信息
			LoadConfig(pNode, sysCfgData.vecSensorConfig);
		}
    }while(pNode);


    //多屏幕模式下的屏幕布局信息，单独保存在ScreenLayout.xml中
    TCHAR szPath[MAX_PATH];
    _stprintf_s(szPath, _countof(szPath), _T("%s\\%s"), (LPCTSTR)PROFILE::SETTINGS_BASE_DIRECTORY, (LPCTSTR)PROFILE::SCREEN_LAYOUT_FILE_NAME);
    LoadConfig(szPath, sysCfgData.vecScreenLayouts);

    return TRUE;
}

//@功能:保存配置文件
//@参数:lpszConfigFilePath, 配置文件的完成路径
BOOL SaveConfig(LPCTSTR lpszConfigFilePath, const TSysConfigData& sysCfgData)
{
    TiXmlDocument oXMLDoc;
    TiXmlDeclaration Declaration("1.0","UTF-8","no");
    oXMLDoc.InsertEndChild(Declaration);


    TiXmlComment*  pXmlComment = new TiXmlComment("配置文件根节点");
    oXMLDoc.LinkEndChild(pXmlComment);

    TiXmlElement * pSettings = new TiXmlElement("Settings");
    oXMLDoc.LinkEndChild(pSettings);


	pXmlComment = new TiXmlComment("全局配置参数");
	pSettings->LinkEndChild(pXmlComment);

	TiXmlElement * pGlobalSettings = new TiXmlElement("Global");
	pSettings->LinkEndChild(pGlobalSettings);

	SaveConfig(pGlobalSettings, sysCfgData.globalSettings);

	pXmlComment = new TiXmlComment("全部摄像头传感器的配置参数");
	pSettings->LinkEndChild(pXmlComment);

	TiXmlElement * pIWBSensors = new TiXmlElement("IWBSensors");
	//传感器个数
	pIWBSensors->SetAttribute("count", sysCfgData.vecSensorConfig.size());

	pSettings->LinkEndChild(pIWBSensors);

	SaveConfig(pIWBSensors, sysCfgData.vecSensorConfig);
    

    //多屏幕模式下的屏幕布局信息，单独保存在ScreenLayout.xml中
    TCHAR szPath[MAX_PATH];
    _stprintf_s(szPath, _countof(szPath), _T("%s\\%s"), (LPCTSTR)PROFILE::SETTINGS_BASE_DIRECTORY, (LPCTSTR)PROFILE::SCREEN_LAYOUT_FILE_NAME);
    SaveConfig(szPath, sysCfgData.vecScreenLayouts);


    //以UTF-8编码格式保存
    TiXmlPrinter  printer;
    oXMLDoc.Accept(&printer);

    char UTF8BOM[3]={'\xEF','\xBB','\xBF'};

    std::ofstream theFile;
    //注意:
    //代码若为:theFile.open(CT2A(lpszConfigFilePath) , ios_base::out) ;
    //则路径中若有中文字符，
    //mbstowc_s(NULL,wc_Name, FILENAME_MAX,fileName,FILENAMEMAX-1)
    //返回的wc_name中为乱码
    //
    theFile.open(CT2W(lpszConfigFilePath) , ios_base::out | ios_base::trunc) ;

    if(theFile.is_open())
    {
        theFile.write(UTF8BOM,3) ;

        //中文GB2312编码页号
        UINT CP_GB2312 = 936;
        
        //MBCS 转 Unicode
        CA2W wcharContent(printer.CStr(), CP_GB2312);

        //计算UTF8编码的长度
        int utf8_length = 
        WideCharToMultiByte(
            CP_UTF8,
            0,
            wcharContent,
            -1,
            NULL,
            0,
            NULL,
            NULL);

        char* utf_8_buf = (char*)malloc(utf8_length);

        //Unicode转为UTF8编码
        WideCharToMultiByte(
            CP_UTF8,
            0,
            wcharContent,
            -1,
            utf_8_buf,
            utf8_length,
            NULL,
            NULL);

        theFile.write(utf_8_buf, strlen(utf_8_buf));

        theFile.close();
        free(utf_8_buf);
    }
    return TRUE;
}

BOOL  UpDateConfig(TSensorModeConfig & sensorModeCfg, int nModeIndex, int nSensorId)
{
	///保存各种镜头的配置参数
    TCHAR szPath[MAX_PATH];
    _stprintf_s(
        szPath,
        _countof(szPath),
        _T("%s\\Sensor%02d\\%s\\%s"),
        (LPCTSTR)PROFILE::SETTINGS_BASE_DIRECTORY,
        nSensorId,
        GetProjectModeString(EProjectionMode(nModeIndex)),
        GetCameraTypeString(g_tSysCfgData.globalSettings.eCameraType));

	//CreateFullDirectory(CA2CT(szPath));

    CreateFullDirectory(szPath);

	for (int i = 0; i < int(E_LENS_TYPE_COUNT); i++)
	{
		//载入各种镜头的配置参数
		//char szPathRatio[MAX_PATH];
        TCHAR szPathRatio[MAX_PATH];
		memset(szPathRatio, 0, sizeof(szPathRatio));
		//sprintf_s(szPathRatio, _countof(szPathRatio), "%s\\throw_ratio(%.2f).dll", szPath, TRHOW_RATIO_LIST[i]);
        _stprintf_s(szPathRatio, _countof(szPathRatio), _T("%s\\throw_ratio(%.2f).dll"), szPath, TRHOW_RATIO_LIST[i]);
        
		//if (PathFileExists(CA2CT(szPathRatio)))
        if (PathFileExists(szPathRatio))
		{
			//LoadConfig(CA2CT(szPathRatio), sensorModeCfg.lensConfigs[i]);
            LoadConfig(szPathRatio, sensorModeCfg.lensConfigs[i]);
		}
	}
	return true;
}

BOOL UpdateConfig(TiXmlNode *pNode, TSensorConfig & sensorCfg, int nSensorId)
{
	if (pNode == NULL) return FALSE;
	TiXmlNode* pChild = NULL;
	do
	{
		pChild = pNode->IterateChildren(pChild);
		if (NULL == pChild)
		{
			break;
		}
		const char* lpszElementName = pChild->Value();
		if (_stricmp(lpszElementName, "IWBProjectionMode") == 0)
		{
			const char* szCoefMode = ((TiXmlElement*)pChild)->Attribute("Mode");
			int nModeIndex = 0;
			if (_stricmp(szCoefMode, "Desktop") == 0)
			{
				//载入桌面的设置参数	
				nModeIndex = 0;
			}
			else if (_stricmp(szCoefMode, "Wall") == 0)
			{
				//载入墙面的设置参数
				nModeIndex = 1;
			}
			UpDateConfig(sensorCfg.vecSensorModeConfig[nModeIndex], nModeIndex, nSensorId);
		}
	} while (pChild);

	return TRUE;
}
BOOL UpDateConfig(LPCTSTR lpszConfigFilePath, TSysConfigData& sysCfgData, int PID, int VID)
{
	if (PID ==13200 && VID == 6380)
	{
		sysCfgData.globalSettings.eCameraType = E_CAMERA_MODEL_1;
		sysCfgData.globalSettings.CMOSChipSpecification.pixel_size = 0.006;
	}
	else if (PID == 37424 && VID == 1443)
	{
		sysCfgData.globalSettings.eCameraType = E_CAMERA_MODEL_2;
		sysCfgData.globalSettings.CMOSChipSpecification.pixel_size = 0.003;
	}
	else {
		sysCfgData.globalSettings.eCameraType = E_CAMERA_MODEL_0;
		sysCfgData.globalSettings.CMOSChipSpecification.pixel_size = 0.006;
	}

	TiXmlDocument oXMLDoc;
	if (!oXMLDoc.LoadFile(CT2A(lpszConfigFilePath), TIXML_ENCODING_UTF8))
	{
		return FALSE;
	}

	TiXmlElement *pRootElement = oXMLDoc.RootElement();
	if (pRootElement == NULL)
	{
		return FALSE;
	}

	TiXmlNode *pNode = NULL;
	do
	{
		pNode = pRootElement->IterateChildren(pNode);
		if (NULL == pNode)
		{
			break;
		}
		const char* lpszElementName = pNode->Value();
		if (_stricmp(lpszElementName, "IWBSensors") == 0)
		{
			//所有传感器的配置信息
			const char* szSensorCount = ((TiXmlElement*)pNode)->Attribute("count");
			if (szSensorCount == 0) return FALSE;
			int nSensorIndex = 0;
			TiXmlNode * pChild = NULL;
			do
			{
				pChild = pNode->IterateChildren(pChild);
				if (NULL == pChild)
				{
					break;
				}
				const char* lpszElementName = pChild->Value();
				if (_stricmp(lpszElementName, "IWBSensor") == 0)
				{
					UpdateConfig(pChild, sysCfgData.vecSensorConfig[nSensorIndex], nSensorIndex);
					nSensorIndex++;
					if (nSensorIndex == sysCfgData.vecSensorConfig.size())
					{
						break;
					}
				}
			} while (pChild);
		}
	} while (pNode);

	return TRUE;

}


//@功能:保存屏幕布局数据
BOOL SaveConfig(LPCTSTR lpszConfigFilePath, const std::vector<TScreenLayout>& allScreenLayouts)
{
    TiXmlDocument oXMLDoc;
    TiXmlDeclaration Declaration("1.0", "UTF-8", "no");
    oXMLDoc.InsertEndChild(Declaration);

    TiXmlComment*  pXmlComment = new TiXmlComment("所有布局");
    oXMLDoc.LinkEndChild(pXmlComment);

    TiXmlElement * pAllLayouts = new TiXmlElement("AllScreenLayouts");
    oXMLDoc.LinkEndChild(pAllLayouts);


    for (UINT uLayoutIndex = 0; uLayoutIndex < allScreenLayouts.size(); uLayoutIndex++)
    {
        TiXmlElement * pScreenLayout = new TiXmlElement("ScreenLayout");

        const TScreenLayout& layout = allScreenLayouts[uLayoutIndex];

        const std::vector<RectF>& screens    = layout.vecScreens;
        const std::vector<RectF>& mergeAreas = layout.vecMergeAreas;

        //设置屏幕尺寸
        pScreenLayout->SetAttribute("ScreenCount", screens.size());

        for (UINT uScreenIndex = 0; uScreenIndex < screens.size(); uScreenIndex++)
        {
            char szText[32];

            sprintf_s(szText, _countof(szText), "%d#屏幕", uScreenIndex + 1);
            TiXmlComment*  pXmlComment = new TiXmlComment(szText);
            pScreenLayout->LinkEndChild(pXmlComment);

            TiXmlElement * pScreen = new TiXmlElement("Screen");
            pScreen->SetDoubleAttribute("left", (double)screens[uScreenIndex].left);
            pScreen->SetDoubleAttribute("top", (double)screens[uScreenIndex].top );
            pScreen->SetDoubleAttribute("right", (double)screens[uScreenIndex].right);
            pScreen->SetDoubleAttribute("bottom", (double)screens[uScreenIndex].bottom);
            pScreenLayout->LinkEndChild(pScreen);

            if (uScreenIndex < mergeAreas.size())
            {
                TiXmlComment*  pXmlComment = new TiXmlComment("融合区");
                pScreenLayout->LinkEndChild(pXmlComment);

                TiXmlElement * pMergeArea = new TiXmlElement("MergeArea");

                pMergeArea->SetDoubleAttribute("left" , (double)mergeAreas[uScreenIndex].left);
                pMergeArea->SetDoubleAttribute("top"  , (double)mergeAreas[uScreenIndex].top);
                pMergeArea->SetDoubleAttribute("right", (double)mergeAreas[uScreenIndex].right);
                pMergeArea->SetDoubleAttribute("bottom", (double)mergeAreas[uScreenIndex].bottom);

                pScreenLayout->LinkEndChild(pMergeArea);
            }
        }

        pAllLayouts->LinkEndChild(pScreenLayout);

    }//for


    //以UTF-8编码格式保存
    TiXmlPrinter  printer;
    oXMLDoc.Accept(&printer);

    char UTF8BOM[3] = { '\xEF','\xBB','\xBF' };

    std::ofstream theFile;
    //注意:
    //代码若为:theFile.open(CT2A(lpszConfigFilePath) , ios_base::out) ;
    //则路径中若有中文字符，
    //mbstowc_s(NULL,wc_Name, FILENAME_MAX,fileName,FILENAMEMAX-1)
    //返回的wc_name中为乱码
    //
    theFile.open(CT2W(lpszConfigFilePath), ios_base::out | ios_base::trunc);

    if (theFile.is_open())
    {
        theFile.write(UTF8BOM, 3);

        //中文GB2312编码页号
        UINT CP_GB2312 = 936;

        //MBCS 转 Unicode
        CA2W wcharContent(printer.CStr(), CP_GB2312);

        //计算UTF8编码的长度
        int utf8_length =
            WideCharToMultiByte(
                CP_UTF8,
                0,
                wcharContent,
                -1,
                NULL,
                0,
                NULL,
                NULL);

        char* utf_8_buf = (char*)malloc(utf8_length);

        //Unicode转为UTF8编码
        WideCharToMultiByte(
            CP_UTF8,
            0,
            wcharContent,
            -1,
            utf_8_buf,
            utf8_length,
            NULL,
            NULL);

        theFile.write(utf_8_buf, strlen(utf_8_buf));

        theFile.close();
        free(utf_8_buf);
    }
    return TRUE;



}

//@功能:载入所有的视频布局
BOOL LoadConfig(LPCTSTR lpszConfigFilePath,  std::vector<TScreenLayout>& allScreenLayouts)
{

    TiXmlDocument oXMLDoc;
    if (!oXMLDoc.LoadFile(CT2A(lpszConfigFilePath), TIXML_ENCODING_UTF8))
    {
        return FALSE;
    }
    TiXmlElement *pRootElement = oXMLDoc.RootElement();
    if (pRootElement == NULL)
    {
        return FALSE;
    }

    allScreenLayouts.clear();

    TiXmlNode* pChild_L1 = NULL;
    do
    {
        pChild_L1 = pRootElement->IterateChildren(pChild_L1);
        if (NULL == pChild_L1) break;

        const char* lpszElementName = pChild_L1->Value();
        if (_stricmp(lpszElementName, "ScreenLayout") == 0)
        {
            TScreenLayout layout;

            TiXmlNode* pChild_L2 = NULL;
            do
            {
                pChild_L2 = pChild_L1->IterateChildren(pChild_L2);

                if (NULL == pChild_L2) break;

                const char* lpszElementName = pChild_L2->Value();

                if (_stricmp(lpszElementName, "Screen") == 0)
                {
                    RectF screenArea;
                    memset(&screenArea, 0, sizeof(screenArea));

                    const char* paramValue = ((TiXmlElement*)pChild_L2)->Attribute("left");
                    if (paramValue)
                    {
                        screenArea.left = atof(paramValue);
                    }

                    paramValue = ((TiXmlElement*)pChild_L2)->Attribute("top");
                    if (paramValue)
                    {
                        screenArea.top = atof(paramValue);
                    }

                    paramValue = ((TiXmlElement*)pChild_L2)->Attribute("right");
                    if (paramValue)
                    {
                        screenArea.right = atof(paramValue);
                    }

                    paramValue = ((TiXmlElement*)pChild_L2)->Attribute("bottom");
                    if (paramValue)
                    {
                        screenArea.bottom = atof(paramValue);
                    }
                    layout.vecScreens.push_back(screenArea);

                }
                else if (_stricmp(lpszElementName, "MergeArea") == 0)
                {

                    RectF mergeArea;
                    memset(&mergeArea, 0, sizeof(mergeArea));

                    const char* paramValue = ((TiXmlElement*)pChild_L2)->Attribute("left");
                    if (paramValue)
                    {
                        mergeArea.left = atof(paramValue);
                    }

                    paramValue = ((TiXmlElement*)pChild_L2)->Attribute("top");
                    if (paramValue)
                    {
                        mergeArea.top = atof(paramValue);
                    }

                    paramValue = ((TiXmlElement*)pChild_L2)->Attribute("right");
                    if (paramValue)
                    {
                        mergeArea.right = atof(paramValue);
                    }

                    paramValue = ((TiXmlElement*)pChild_L2)->Attribute("bottom");
                    if (paramValue)
                    {
                        mergeArea.bottom = atof(paramValue);
                    }

                    layout.vecMergeAreas.push_back(mergeArea);
                    
                }



            } while (pChild_L2);



            allScreenLayouts.push_back(layout);
        }//if


    } while (pChild_L1);

    return TRUE;
}