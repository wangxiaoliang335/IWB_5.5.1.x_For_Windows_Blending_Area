#pragma once
#ifdef EASISDKREG_INTERFACE_EXPORTS
	#define EASISDKREG_INTERFACE_API _declspec(dllexport)
#else
	#define EASISDKREG_INTERFACE_API _declspec(dllimport)

#endif

extern "C"
{

//@功能:SDK初始化
EASISDKREG_INTERFACE_API HRESULT  SKDREG_Init();


//@功能:SDK反初始化
EASISDKREG_INTERFACE_API void  SKDREG_Uninit();

//@功  能:通过读取USB软件狗,遍历检查所有正确的软件狗, 判断基本功能是否可用
//@参  数:nFunctionNo,1~15,基本功能编号
//@返回值:S_OK,基本功能使能
//        -1,软件狗不存在
//        -2,软件狗数据检验失败
//        -3,超过期限了
//        -4,软件狗硬件ID错误
//        -5,未使能
EASISDKREG_INTERFACE_API HRESULT SDKREG_IsBasicFunctionEnabled(int nFunctionNo);


//@功  能:通过读取USB软件狗,判断PPT索引SDK是否可用
//@参 数:nUSBKeyIndex, 从0开始的USB Key索引号
//@返回值:S_OK,PPT索引SDK可用
//        -1,软件狗不存在
//        -2,软件狗数据检验失败
//        -3,超过期限了
//        -4,软件狗硬件ID错误
//        -5,未使能
EASISDKREG_INTERFACE_API HRESULT SDKREG_PPTIndexSDKEnabled(UINT uUSBKeyIndex = 0);



//@功  能:通过读取USB软件狗,判断鼠标键盘事件钩子SDK是否可用
//@参 数:nUSBKeyIndex, 从0开始的USB Key索引号
//@返回值:S_OK,鼠标键盘事件钩子SDK可用
//        -1,软件狗不存在
//        -2,软件狗数据检验失败
//        -3,超过期限了
//        -4,软件狗硬件ID错误
//        -5,未使能
EASISDKREG_INTERFACE_API HRESULT SDKREG_KMEventHookSDKEnabled(UINT uUSBKeyIndex = 0);


//@功  能:通过读取USB软件狗,判断鼠标控制云台SDK是否可用
//@参 数:nUSBKeyIndex, 从0开始的USB Key索引号
//@返回值:S_OK,鼠标控制云台SDK可用
//        -1,软件狗不存在
//        -2,软件狗数据检验失败
//        -3,超过期限了
//        -4,软件狗硬件ID错误
//        -5,未使能
EASISDKREG_INTERFACE_API HRESULT SDKREG_MousePTZControlSDKEnabled(UINT uUSBKeyIndex = 0);



//@功  能:检查USB软件狗的状态, 遍历检查所有正确的软件狗。
//@返回值:S_OK,USB软件狗状态正常
//        -1,软件狗不存在
//        -2,软件狗数据检验失败
//        -3,超过期限了
//        -4,软件狗硬件ID错误,USB软件狗数据区中记录的HID和真实的HID不一致
//        -5,未使能
//        -6,系统时间错误
//        -7,写入软件狗失败
//@说明:每次检测完毕后,会将系统时间写入USB数据区
EASISDKREG_INTERFACE_API HRESULT SDKREG_CheckUSBSoftwareDog();


//@功  能:返回错误信息
//@参  数:lErrorCode,错误代码
//@返回值:错误代码对应的描述信息
EASISDKREG_INTERFACE_API const WCHAR* SDKREG_GetErrorMsg(HRESULT lErrorCode);

//@功  能:获取加密锁版本号
//@参  数:pVersion, 存放加密狗版本的数据缓冲区。
//        nUSBKeyIndex, 从0开始的USB Key索引号
//@返回值:S_OK,鼠标控制云台SDK可用
//        -1,软件狗不存在
//        -2,软件狗数据检验失败
//        -3,超过期限了
//        -4,软件狗硬件ID错误
//        -5,未使能,
//        -6,系统时间错误
//        -7,写入错误
EASISDKREG_INTERFACE_API HRESULT SDKREG_GetVersion(float* pVersion, UINT uUSBKeyIndex = 0);


//@功  能:获取加密锁对应的应用程序类型
//@参  数:pAppType, 存放应用程序类型的数据缓冲区。
//        nUSBKeyIndex, 从0开始的USB Key索引号
//@返回值:S_OK,鼠标控制云台SDK可用
//        -1,软件狗不存在
//        -2,软件狗数据检验失败
//        -3,超过期限了
//        -4,软件狗硬件ID错误
//        -5,未使能
//        -6,系统时间错误
//        -7,写入错误
//@说明:白板定位软件类型,
//      第一个字节定义
//      bit0: 定位软件类型
//            1:为手指触控
//            0:为3DTouch
//
//      第二字节定义
//      bit0:双屏拼接功能使能位
//            0，无双屏拼接功能
//            1，有双屏拼接功能
//       其他位保留未用
EASISDKREG_INTERFACE_API HRESULT SDKREG_GetAppType(int* pAppType, UINT uUSBKeyIndex = 0);


//@功 能:返回USB Key的数目
EASISDKREG_INTERFACE_API UINT SDKREG_GetUSBKeyCount();

}