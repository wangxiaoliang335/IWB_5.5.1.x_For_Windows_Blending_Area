#pragma once

class ISpotListProcessor
{
public:
     //@功能:往光斑列表处理器中写入光斑列表信息
    //@参数:pLightSpots，指向光斑列表的指针
    //      nLightSpotCount, 光斑个数
    //      dwCameraId, 从0开始的摄像头编号    
    virtual BOOL WriteSpotList(TLightSpot* pLightSpots, int nLightSpotCount, DWORD dwCameraId) = 0;

    //@功能:启动处理
    virtual void StartProcess() = 0;

    //@功能:停止处理
    virtual void StopProcess() = 0;

    //@功能:屏幕分辨率变化事件响应函数
    //@参数:nScreenWidth, 新的屏幕宽度
    //      nScreenHeight,新的屏幕高度
    virtual void OnDisplayChange(int nScreenWidth, int nScreenHeight)=0;

    //@功能:判断是否正在触发手势
    virtual BOOL IsTriggeringGuesture()const = 0;
};


//光斑列表处理线程
class CSpotListProcessor:public ISpotListProcessor
{
public:
    CSpotListProcessor();

    ~CSpotListProcessor();

    //@功能:启动处理
    //@说明:引用计数为0时启动处理线程, 否则,引用计数+1,
    virtual void StartProcess();

    //@功能:停止处理
    //@说明:调用一次,引用计数-1，引用计数为0时，处理线程结束
    virtual  void StopProcess();

    //@功能:往光斑列表处理器中写入光斑列表信息
    //@参数:pLightSpots，指向光斑列表的指针
    //      nLightSpotCount, 光斑个数    
    //      dwCameraId, 从0开始的摄像头编号
    //@功能:该函数被多个线程同时访问，因此需要确保线程安全
    virtual BOOL WriteSpotList(TLightSpot* pLightSpots, int nLightSpotCount, DWORD dwCameraId);

    //@功能:复位处理器
    void Reset();

    //@功能:设置摄像头的个数
    //@说明:明确摄像头个数,有利于提高实时处理速度
    //void SetCameraCount(UINT nCameraCount);


     CSpotMerger& GetSpotMerger() { return this->m_oSpotMerger;}
     static CVirtualHID& GetVirtualHID() { return m_oVirtualHID;}



    //@功能:屏幕分辨率变化事件响应函数
    //@参数:nScreenWidth, 新的屏幕宽度
    //      nScreenHeight,新的屏幕高度
    virtual void OnDisplayChange(int nScreenWidth, int nScreenHeight);

    //@功能:判断是否正在触发手势
    virtual BOOL IsTriggeringGuesture()const;
protected:
    //@功能:启动处理线程
    BOOL StartProcessThread();


    //@功能:停止处理线程
    void StopProcessThread();

    //@功能:处理光斑
    void ProcessLightSpots(); 


    //@功能:所有光斑的后续处理程序
    //@参数:pLightSpots, 指向光斑列表的指针
    //      nLightSpotCount, 光斑个数
    //@说明:
    //      光斑后续处理程序包括:
    //      a)位于融合区内的光斑合并
    //      b)前后时刻的光斑匹配
    //      c)手势识别
    //      d)触屏, 鼠标事件的产生
    void OnPostProcess(TLightSpot* pLightSpots, int nLightSpotCount);    

    /*
     @功能：GLBoard手势识别
     @参数:pLightSpots, 指向光斑列表的指针
           nLightSpotCount, 光斑个数
     @返回值：根据返回值决定是否继续传递触屏或者鼠标事件
    */
    bool DoGLBoardGestureRecognition(TLightSpot* pLightSpots, int &nLightSpotCount);

    /*
    @功能：Windows系统下手势识别
    @参数:pLightSpots, 指向光斑列表的指针
          nLightSpotCount, 光斑个数
    @返回值：根据返回值决定是否继续传递触屏或者鼠标事件
    */
    bool DoWindowsGestureRecognition(const TLightSpot* pLightSpots, int nLightSpotCount, TContactInfo *penInfo, int &nPenCount);    

protected:
    HANDLE m_hProcessThread;
   
    BOOL HasNeignborInSpotList(const POINT& ptPos, const TLightSpot* pSpotList, int nSpotCount);

    static ULONG _stdcall ProcessThreadProc(LPVOID lpCtx);

    //FIFO元素数据状态常量
    static const int CAMERA_NUMBER = 2;

    static const int STATUS_DATA_ALL_INVALID = 0x00;//数据无效标志    
    static const int STATUS_DATA_ALL_VALID   = (1<< CAMERA_NUMBER) - 1;//数据全有效标志
    

    //FIFO元素结构
    struct TSpotListGroup
    {
        TLightSpot aryLightSpots     [CAMERA_NUMBER][MAX_OBJ_NUMBER];//[摄像头][光斑列表]二维数组
        int        aryLightSpotsCount[CAMERA_NUMBER]                ;//每个摄像头传感器捕捉到的光斑个数
    };



    //输入的光斑列表(
    TSpotListGroup    m_InputSpotListGroup;


    //有效的光斑列表数据
    TSpotListGroup    m_ValidSpotListGroup;

    //<<added by toxuke@gmail.cim ,2014/12/25
    CRoundRobinQueue<TSpotListGroup, 16>  m_SpotListGroupFIFO;//
    //>>

  
    CComAutoCriticalSection m_csForSpotListGroupFIFO;//

    LONG m_lReferenceCount;//引用计数器


    HANDLE m_hWriteEvent;//写入完毕事件
    HANDLE m_hExitEvent ;//退出事件

    static const int SAMPLE_CORRELATION_INTERVAL = 48 ;//摄像头采样相关最大允许间隔, 单位:ms

    UINT m_uCameraCount;//摄像头个数


    //融合区域内的光斑合并器
    CSpotMerger m_oSpotMerger;//

    static const int PEN_NUM = 10;
    //智能光笔匹配
    CSmartMatch<PEN_NUM> m_oSmartPenMatch;//

    //虚拟HID设备
    static CVirtualHID m_oVirtualHID;//  

    bool  m_bLastHIDOwnnerAfterGR;     //上次识别后，获取VirtualHID控制权的是否是本类的实例
    bool  m_bIsSmartPenReset;    

    //<<debug 
    //调试函数
    void SimulateProcess();
    BOOL m_bSimulateMode;
    
    //debug>>

    CStrokeFilter<PEN_NUM>  m_oStrokFilter;//笔迹过滤器
	CBezierInterpolator<PEN_NUM> m_oBezierInterpolator;//

    //正在触发手势标志
    BOOL m_bIsTriggeringGuesture;
};

///////////////////////////////////////////////////////////////////////////
//手势识别
extern CGestureEventGenerator g_oGLBoardGR;
extern CWindowsGestureRecognition g_oWinGR;
//extern bool g_oIsFingerTouchMode;           //只有在手指触碰模式下才开启手势识别