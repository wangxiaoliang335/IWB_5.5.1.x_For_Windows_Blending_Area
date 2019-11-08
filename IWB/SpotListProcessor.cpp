#include "stdafx.h"
//#include "headers.h"

//////////////////////////////////////////////////////////////////////////



//手势识别
CGestureEventGenerator g_oGLBoardGR;
CWindowsGestureRecognition g_oWinGR;
//bool g_oIsFingerTouchMode= false;

//
CVirtualHID CSpotListProcessor::m_oVirtualHID;

CSpotListProcessor::CSpotListProcessor()
:
m_lReferenceCount(0L),
m_hProcessThread(NULL),
m_uCameraCount(1),
m_bLastHIDOwnnerAfterGR(true),
m_bIsSmartPenReset(true),
m_bSimulateMode(FALSE),
m_bIsTriggeringGuesture(FALSE)
{
    Reset();
    m_hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hExitEvent  = CreateEvent(NULL, FALSE, FALSE, NULL);

}


CSpotListProcessor::~CSpotListProcessor()
{
    CloseHandle(m_hWriteEvent);
    CloseHandle(m_hExitEvent );

}

//@功能:初始化
void CSpotListProcessor::Init(UINT uCameraCount)
{
    if (uCameraCount > MAX_CAMERA_NUMBER)
    {
        uCameraCount = MAX_CAMERA_NUMBER;
    }

    m_uCameraCount = uCameraCount;

}

FILE * g_hDebugSampleFile   = NULL;
FILE * g_hDebugSampleFile2  = NULL;
FILE * g_hDebugRawInputData = NULL;

//@功能:启动处理线程
//@说明:引用计数为0时启动处理线程, 调用一次,引用计数+1,
void CSpotListProcessor::StartProcess()
{
    if(m_lReferenceCount == 0)
    {

        Reset();

        m_oVirtualHID.OpenDevice();
        StartProcessThread();

        //<<debug
#ifdef _DEBUG
        SYSTEMTIME sysTime;
        GetLocalTime(&sysTime);
        char szFileName[256];
        sprintf_s(
            szFileName,
            _countof(szFileName),
            "SampleData_%04d%02d%02d_%02d%02d%02d%03d_BeforeMerge.txt",
            sysTime.wYear,
            sysTime.wMonth,
            sysTime.wDay,
            sysTime.wHour,
            sysTime.wMinute,
            sysTime.wSecond,
            sysTime.wMilliseconds);

        fopen_s(&g_hDebugSampleFile, szFileName, "w");


        sprintf_s(
            szFileName,
            _countof(szFileName),
            "SampleData_%04d%02d%02d_%02d%02d%02d%03d_AfterMerge.txt",
            sysTime.wYear,
            sysTime.wMonth,
            sysTime.wDay,
            sysTime.wHour,
            sysTime.wMinute,
            sysTime.wSecond,
            sysTime.wMilliseconds);

        fopen_s(&g_hDebugSampleFile2, szFileName, "w");

        sprintf_s(
            szFileName,
            _countof(szFileName),
            "SampleData_%04d%02d%02d_%02d%02d%02d%03d_RawInputData.txt",
            sysTime.wYear,
            sysTime.wMonth,
            sysTime.wDay,
            sysTime.wHour,
            sysTime.wMinute,
            sysTime.wSecond,
            sysTime.wMilliseconds);

        fopen_s(&g_hDebugRawInputData, szFileName, "w");

#endif
        //debug>>

    }

    InterlockedIncrement(&m_lReferenceCount);



}

//@功能:停止
//@说明:调用一次,引用计数-1，引用计数为0时，处理线程结束
void CSpotListProcessor::StopProcess()
{

    if(m_lReferenceCount)
    {
        InterlockedDecrement(&m_lReferenceCount);
    }
    if(0 == m_lReferenceCount && this->m_hProcessThread != NULL)
    {
        StopProcessThread();
        m_oVirtualHID.CloseDevice();

        //<<debug
#ifdef _DEBUG
        if(g_hDebugSampleFile)
        {
            fclose(g_hDebugSampleFile);
            g_hDebugSampleFile = NULL;
        }

        if(g_hDebugSampleFile2)
        {
            fclose(g_hDebugSampleFile2);
            g_hDebugSampleFile2 = NULL;
        }

        if(g_hDebugRawInputData)
        {
            fclose(g_hDebugRawInputData);
            g_hDebugRawInputData = NULL;
        }
        //debug>>
#endif
    }
}

////@功能:判断光斑在融合区内是否可以合并的邻点
////@参数:ptPos，光斑位置
////     pBuddyCameraSpotList, 兄弟摄像机采集到的光斑列表
////     nBuddyCameraSpotCount, 兄弟摄像机采集到的光斑个数
//BOOL CSpotListProcessor::HasNeignborInSpotList(const POINT& ptPos, const TLightSpot* pBuddyCameraSpotList, int nBuddyCameraSpotCount)
//{
//    int threshold = this->GetSpotMerger().GetMergeDistThreshold();
//    threshold = threshold * threshold;
//
//    for(int i=0; i < nBuddyCameraSpotCount; i++)
//    {
//        const TLightSpot& spot = pBuddyCameraSpotList[i];
//
//        if(m_oSpotMerger.GetMergeAreaLeftBorder()   <= spot.ptPosInScreen .x  &&    spot.ptPosInScreen .x <= m_oSpotMerger.GetMergeAreaRightBorder())
//        {
//
//            int dx = ptPos.x - spot.ptPosInScreen.x;
//            int dy = ptPos.y - spot.ptPosInScreen.y;
//            int dist = dx*dx + dy*dy;
//
//            if(dist < threshold)
//            {
//                return TRUE;
//            }
//        }
//    }
//
//    return FALSE;
//}

//@功 能:判断光斑是否出现在触控融合区内
//@参 数:lightSpot, 光斑数据。
//       CameraIndex, 看到光斑的镜头的ID号。
//       pMergeAreaIndex, 指向保存融合区索引的内存的指针，在该触控融合区发现了要判断的光斑。
//@返回值:TRUE, 在融合区内出现。
//       FALSE, 未在融合区内出现
BOOL CSpotListProcessor::AppearInMergeArea(const TLightSpot& lightSpot, UINT CameraIndex, UINT* pMergeAreaIndex)
{
    if (m_uCameraCount == 1) return FALSE;
    if (CameraIndex >= m_uCameraCount) return FALSE;

    RECT mergeAreas[2];
    UINT mergeAreaIndex[2];
    int  areaCount = 0;

    if (CameraIndex == 0)
    {//只有右边的一个融合区
        const RECT* pMergeArea = m_oSpotMerger.GetMergeArea(CameraIndex);
        mergeAreas[0] = *pMergeArea;
        mergeAreaIndex[0] = CameraIndex;
        areaCount = 1;
    }
    else if (CameraIndex == m_uCameraCount - 1)
    {//只有左边的一个融合区
        const RECT* pMergeArea = m_oSpotMerger.GetMergeArea(CameraIndex - 1);
        mergeAreas[0] = *pMergeArea;
        mergeAreaIndex[0] = CameraIndex - 1;

        areaCount = 1;
    }
    else
    {//左右两边各有一个融合区
        const RECT* pLeftMergeArea  = m_oSpotMerger.GetMergeArea(CameraIndex -1);
        const RECT* pRightMergeArea = m_oSpotMerger.GetMergeArea(CameraIndex);

        mergeAreas[0] = *pLeftMergeArea;
        mergeAreas[1] = *pRightMergeArea;

        mergeAreaIndex[0] = CameraIndex - 1;
        mergeAreaIndex[1] = CameraIndex;

        areaCount = 2;
    }
    
    for (int i = 0; i < _countof(mergeAreas); i++)
    {
        const RECT& area = mergeAreas[i];
        if (area.left < lightSpot.ptPosInScreen.x && lightSpot.ptPosInScreen.x < area.right)
        {
            if (pMergeAreaIndex)
            {
                *pMergeAreaIndex = mergeAreaIndex[i];
            }
            return TRUE;
        }//if
        
    }//for


    return FALSE;
}


//@功 能:判断光斑是否被相邻的兄弟摄像头在融合区内看到
//@参 数:lightSpot, 光斑数据
//       CameraIndex, 看到光斑的镜头的ID号
//@返回值:TRUE, 否被相邻的兄弟摄像头看到
//       FALSE, 兄弟相机未看到
BOOL CSpotListProcessor::SeenInMergeAreaByBuddyCamera(const TLightSpot& spotTarget, UINT CameraIndex)
{
    if (m_uCameraCount == 1) return FALSE;
    if (CameraIndex >= m_uCameraCount) return FALSE;

    int buddyCameraIndex[2];
    int nBuddyCameraCount = 0;

    if (CameraIndex == 0)
    {//只有右边的一个Buddy
        buddyCameraIndex[0] = CameraIndex + 1;
        nBuddyCameraCount   = 1;
    }
    else if (CameraIndex == m_uCameraCount - 1)
    {//只有左边的一个Buddy
        buddyCameraIndex[0] = CameraIndex - 1;
        nBuddyCameraCount = 1;
    }
    else
    {//左右两边各有一个Buddy

        UINT leftBuddyIndex  = CameraIndex - 1;
        UINT rightBuddyIndex = CameraIndex + 1;

        buddyCameraIndex[0] = leftBuddyIndex;
        buddyCameraIndex[1] = rightBuddyIndex;
        nBuddyCameraCount = 2;
    }

    int threshold = this->GetSpotMerger().GetMergeDistThreshold();
    threshold = threshold * threshold;

    for (int i = 0; i < nBuddyCameraCount; i++)
    {
        DWORD dwBuddyCameraId = buddyCameraIndex[i];
        const TLightSpot* pBuddyCameraSpotList = &m_InputSpotListGroup.aryLightSpots[dwBuddyCameraId][0];
        int nBuddyCameraSpotCount = m_InputSpotListGroup.aryLightSpotsCount[dwBuddyCameraId];


        for (int j = 0; j < nBuddyCameraSpotCount; j++)
        {
            const TLightSpot& buddySpot = pBuddyCameraSpotList[i];

            if (AppearInMergeArea(buddySpot, dwBuddyCameraId))
            {//buddySpot也位于融合区, 判断目标点与buddy点之间的距离是否小于融合距离门限
                int dx = spotTarget.ptPosInScreen.x - buddySpot.ptPosInScreen.x;
                int dy = spotTarget.ptPosInScreen.y - buddySpot.ptPosInScreen.y;
                int dist = dx*dx + dy*dy;

                if (dist < threshold)
                {
                    return TRUE;
                }
            }
        }//for(j)

    }//for(i)

    return FALSE;
}

//@功  能:判断兄弟相机是否在融合区内看到了光斑
//@参  数:当前相机的Id, 由它来确定兄弟相机的id。
//@返回值: 
//         TRUE, 兄弟相机是在融合区内看到了光斑
//        FALSE, 兄弟相机是在融合区内没有发现光斑
BOOL CSpotListProcessor::BuddyCameraFoundSpotInMergeArea(UINT CameraIndex)
{
    if (m_uCameraCount == 1) return FALSE;
    if (CameraIndex >= m_uCameraCount) return FALSE;

    RECT mergeAreas[2];
    int  areaCount = 0;

    int buddyCameraIndex[2];
    int nBuddyCameraCount = 0;

    if (CameraIndex == 0)
    {//只有右边的一个Buddy
        buddyCameraIndex[0] = CameraIndex + 1;
        nBuddyCameraCount = 1;

        const RECT* pMergeArea = m_oSpotMerger.GetMergeArea(CameraIndex);
        mergeAreas[0] = *pMergeArea;
        areaCount = 1;

    }
    else if (CameraIndex == m_uCameraCount - 1)
    {//只有左边的一个Buddy
        buddyCameraIndex[0] = CameraIndex - 1;
        nBuddyCameraCount = 1;

        const RECT* pMergeArea = m_oSpotMerger.GetMergeArea(CameraIndex - 1);
        mergeAreas[0] = *pMergeArea;
        areaCount = 1;
    }
    else
    {//左右两边各有一个Buddy

        UINT leftBuddyIndex = CameraIndex - 1;
        UINT rightBuddyIndex = CameraIndex + 1;

        buddyCameraIndex[0] = leftBuddyIndex;
        buddyCameraIndex[1] = rightBuddyIndex;
        nBuddyCameraCount = 2;


        const RECT* pLeftMergeArea = m_oSpotMerger.GetMergeArea(CameraIndex - 1);
        const RECT* pRightMergeArea = m_oSpotMerger.GetMergeArea(CameraIndex);

        mergeAreas[0] = *pLeftMergeArea;
        mergeAreas[1] = *pRightMergeArea;
        areaCount = 2;
    }



    for (int i = 0; i < areaCount; i++)
    {
        const RECT& area = mergeAreas[i];
               
        DWORD dwBuddyCameraId = buddyCameraIndex[i];

        const TLightSpot* pBuddyCameraSpotList = &m_InputSpotListGroup.aryLightSpots[dwBuddyCameraId][0];
        int nBuddyCameraSpotCount = m_InputSpotListGroup.aryLightSpotsCount[dwBuddyCameraId];

        for (int j = 0; j < nBuddyCameraSpotCount; j++)
        {
            const TLightSpot& buddySpot = pBuddyCameraSpotList[j];
            if (area.left < buddySpot.ptPosInScreen.x && buddySpot.ptPosInScreen.x < area.right)
            {
                return TRUE;
            }
          
        }//for(j)

    }

    return FALSE;

}



//@功能:往光斑列表处理器中写入光斑列表信息
//@参数:pLightSpots，指向光斑列表的指针
//      nLightSpotCount, 光斑个数
//      dwCameraId, 从0开始的摄像头编号
//@功能:该函数被多个线程同时访问，因此需要确保线程安全
//
BOOL CSpotListProcessor::WriteSpotList(TLightSpot* pLightSpots, int nLightSpotCount, DWORD dwCameraId)
{
    //加锁，线程安全保护
    CComCritSecLock<CComAutoCriticalSection> lock(m_csForSpotListGroupFIFO);

    if(dwCameraId >= MAX_CAMERA_NUMBER) return FALSE;

    if(nLightSpotCount >  MAX_OBJ_NUMBER)
    {
        nLightSpotCount = MAX_OBJ_NUMBER;
    }

    //DWORD dwBuddyCameraId = 1;

    //if(dwCameraId == 1)
    //{
    //    dwBuddyCameraId = 0;
    //}

    //BOOL bNeedSkip = FALSE;//需要跳帧标志

    BOOL bDelayProcess = FALSE;//需要延迟处理标志

    BOOL bAllOutsideMergeArea = TRUE;

    if(theApp.GetScreenMode() >= EScreenModeDouble)
    {//多屏拼接模式
        for (int i = 0; i < nLightSpotCount; i++)
        {
            TLightSpot& spot = pLightSpots[i];
            spot.aux.mergeAreaIndex = UINT(-1);


            if (AppearInMergeArea(spot, dwCameraId, &spot.aux.mergeAreaIndex))
            {
                bAllOutsideMergeArea = FALSE;

                //光斑位于融合区内, 需要判断另外一个摄像头的光斑是否也位于融合区内，保证两个摄像头同时看到融合区内的光斑,
                //否则因为两个摄像头数据采样的不同步,一个摄像头采集的光斑屏幕坐标在融合区内,另外一个摄像头采集的光斑屏幕坐标在融合区以外,不会被融合，因此
                //光斑处理器会接收到两个光斑, 新生成一支笔, 尔后的光斑可能与新生成的笔匹配，造成原有的笔弹起。产生的效果就是笔迹断裂
                if(SeenInMergeAreaByBuddyCamera(spot, dwCameraId))
                {

                }
                else
                {//兄弟相机还没看到光斑, 需要延后处理
                    bDelayProcess = TRUE;
                    break;
                }

            }

        }//for(i)



        //当前摄像头的光斑数据都在合并区以外时,要保证buddy摄像头的光斑也在融合区以外,
        //否则因为两个摄像头数据采样的不同步,一个摄像头采集的光斑屏幕坐标在融合区内,另外一个摄像头采集的光斑屏幕坐标在融合区以外,不会被融合，因此
        //光斑处理器会接收到两个光斑, 新生成一支笔, 尔后的光斑可能与新生成的笔匹配，造成原有的笔弹起。
        if (bAllOutsideMergeArea)
        {
            if (BuddyCameraFoundSpotInMergeArea(dwCameraId))
            {
                bDelayProcess = TRUE;
            }


        }

        //    if(m_oSpotMerger.GetMergeAreaLeftBorder()   <= spot.ptPosInScreen .x  &&   spot.ptPosInScreen .x <= m_oSpotMerger.GetMergeAreaRightBorder())
        //    {//光斑位于融合区内, 需要判断另外一个摄像头的光斑是否也位于融合区内，保证两个摄像头同时看到融合区内的光斑,
        //     //否则因为两个摄像头数据采样的不同步,一个摄像头采集的光斑屏幕坐标在融合区内,另外一个摄像头采集的光斑屏幕坐标在融合区以外,不会被融合，因此
        //     //光斑处理器会接收到两个光斑, 新生成一支笔, 尔后的光斑可能与新生成的笔匹配，造成原有的笔弹起。产生的效果就是笔迹断裂
        //        BOOL bHasNeighbor = 
        //            HasNeignborInSpotList(
        //            spot.ptPosInScreen, 
        //            &m_InputSpotListGroup.aryLightSpots    [dwBuddyCameraId][0], 
        //            m_InputSpotListGroup.aryLightSpotsCount[dwBuddyCameraId]
        //        );

        //        if(!bHasNeighbor)
        //        {
        //            bNeedSkip = TRUE;
        //            break;
        //        }

        //        bAllOutsideMergeArea = FALSE;
        //    }

        //}

        //if(bAllOutsideMergeArea)
        //{//当前摄像头的光斑数据都在合并区以外时,要保证buddy摄像头的光斑也在融合区以外,
        // //否则因为两个摄像头数据采样的不同步,一个摄像头采集的光斑屏幕坐标在融合区内,另外一个摄像头采集的光斑屏幕坐标在融合区以外,不会被融合，因此
        // //光斑处理器会接收到两个光斑, 新生成一支笔, 尔后的光斑可能与新生成的笔匹配，造成原有的笔弹起。
        //    for(int i=0; i<  m_InputSpotListGroup.aryLightSpotsCount[dwBuddyCameraId]; i++)
        //    {
        //        const TLightSpot& spot = m_InputSpotListGroup.aryLightSpots[dwBuddyCameraId][i];

        //        if(m_oSpotMerger.GetMergeAreaLeftBorder()   <= spot.ptPosInScreen .x  &&   spot.ptPosInScreen .x <= m_oSpotMerger.GetMergeAreaRightBorder())
        //        {//兄弟摄像头有光斑仍然处于融合区内,需要跳帧。
        //            bNeedSkip = TRUE;
        //            break;
        //        }
        //    }
        //}
    }


    memcpy(&m_InputSpotListGroup.aryLightSpots[dwCameraId], pLightSpots, nLightSpotCount*sizeof(TLightSpot));
    m_InputSpotListGroup.aryLightSpotsCount[dwCameraId] = nLightSpotCount;

    if(bDelayProcess)
    {//需要延后处理,
        return FALSE;
    }
    //m_ValidSpotListGroup = m_InputSpotListGroup;


    //光斑组写入FIFO中去
    //m_SpotListGroupFIFO.Write(m_ValidSpotListGroup);
    m_SpotListGroupFIFO.Write(m_InputSpotListGroup);

    //触发写入完毕事件
    SetEvent(m_hWriteEvent);

    return TRUE;
}


//@功能:处理光斑
void CSpotListProcessor::ProcessLightSpots()
{
    //加锁，线程安全保护
    CComCritSecLock<CComAutoCriticalSection> lock(m_csForSpotListGroupFIFO);

    while(!m_SpotListGroupFIFO.IsEmpty())
    {//处理FIFO中已写入的所有数据
        const TSpotListGroup* pSpotListGroup = m_SpotListGroupFIFO.Read();
        if(pSpotListGroup == NULL) break;
        //所有摄像头的光斑数组
        TLightSpot allLightSpots[MAX_CAMERA_NUMBER*MAX_OBJ_NUMBER];

        //所有光斑个数
        int nAllLightSpotCount = 0;

        BOOL bDoubleScreenTouchMergeTemp = g_tSysCfgData.globalSettings.bDoubleScreenTouchMerge;
        
        
        /*

        //#0号传感器(位于拼接屏的左边)的光斑
        for(int i=0; i<  pSpotListGroup->aryLightSpotsCount[0]; i++)
        {
            const TLightSpot& spot = pSpotListGroup->aryLightSpots[0][i];

            //bDoubleScreenTouchMergeTemp = g_tSysCfgData.globalSettings.bDoubleScreenTouchMerge;

            if(theApp.GetScreenMode() == EScreenModeDouble && bDoubleScreenTouchMergeTemp  && spot.ptPosInScreen.x > this->GetSpotMerger().GetMergeAreaRightBorder())
            {//光斑位于融合区右边界以外, 则略过。
                continue;
            }

            allLightSpots[nAllLightSpotCount] = spot;
            nAllLightSpotCount ++;
        }

        //#1号传感器(位于拼接屏的右边)的光斑
        for(int i=0; i<  pSpotListGroup->aryLightSpotsCount[1]; i++)
        {
            const TLightSpot& spot = pSpotListGroup->aryLightSpots[1][i];
            if(theApp.GetScreenMode() == EScreenModeDouble && bDoubleScreenTouchMergeTemp && spot.ptPosInScreen.x < this->GetSpotMerger().GetMergeAreaLeftBorder())
            {//光斑位于融合区左边界以外, 则略过。
                continue;
            }
            allLightSpots[nAllLightSpotCount] = spot;
            nAllLightSpotCount ++;
        }
        */
        
        
        
        for (int nCameraIndex = 0; nCameraIndex < MAX_CAMERA_NUMBER; nCameraIndex++)
        {
            for (int i = 0; i < pSpotListGroup->aryLightSpotsCount[nCameraIndex]; i++)
            {
                const TLightSpot& spot = pSpotListGroup->aryLightSpots[nCameraIndex][i];
                allLightSpots[nAllLightSpotCount] = spot;
                nAllLightSpotCount++;
            }

        }

        //所有光斑的后续处理
        OnPostProcess(&allLightSpots[0], nAllLightSpotCount);
    }
}


void DebugContactInfo(const TContactInfo* contactInfos, int nCount)
{

    for (int i = 0; i < nCount; i++)
    {

        const TCHAR*  szEvent;
        if (contactInfos[i].ePenState == E_PEN_STATE_DOWN)
        {
            szEvent = _T("Down");
        }
        else if (contactInfos[i].ePenState == E_PEN_STATE_UP)
        {
            szEvent = _T("Up");
        }
        else
        {
            szEvent = _T("Unknown");
        }


        TCHAR szDebug[1024];
        _stprintf_s(
            szDebug,
            _countof(szDebug),
            _T("<x,y>=<%d,%d>,%s\n"),
            contactInfos[i].pt.x,
            contactInfos[i].pt.y,
            szEvent);

        OutputDebugString(szDebug);
    }
}
//@功能:所有光斑的后续处理程序
//@参数:pLightSpots, 指向光斑列表的指针
//      nLightSpotCount, 光斑个数
//@说明:该函数被函数ProcessLightSpots调用在线程ProcessThreadProc中执行
//      
void CSpotListProcessor::OnPostProcess(TLightSpot* pLightSpots, int nLightSpotCount)
{
    //双屏拼接时，融合区内的光斑合并。
    if(theApp.GetScreenMode() >= EScreenModeDouble)
    {
        m_oSpotMerger.DoMerge(pLightSpots, &nLightSpotCount);
    }

    //复位手势触发标志
    m_bIsTriggeringGuesture = FALSE;
    TContactInfo penInfo[PEN_NUM];
    int penCount = PEN_NUM;
	/////add by zhaown 2019.10.09
	if(GetActualTouchType() == E_DEVICE_PALM_TOUCH_CONTROL)
	{
		/////如果是手掌互动时，那么就不做平滑处理和插值处理。直接是原始的光斑值进行触控
		int  nScreenX = GetSystemMetrics(SM_CXSCREEN);
		int  nScreenY = GetSystemMetrics(SM_CYSCREEN);
		POINT pts[MAX_CAMERA_NUMBER*MAX_OBJ_NUMBER];

		for (int i = 0; i< nLightSpotCount; i++)
		{
			pts[i] = pLightSpots[i].ptPosInScreen;
			if ( (pts[i].x > nScreenX/8 && pts[i].x < nScreenX*7/8)||(pts[i].y > nScreenY/8 && pts[i].y < nScreenY*7 / 8) )
			{
				pts[i].x  = pts[i].x + 5 ;
				pts[i].y  = pts[i].y - 5 ;
			}
		}
		m_oSmartPenMatch.DoMatch(pts, nLightSpotCount);

		int nElementCount = 0;
		const TMatchInfo* pMatchInfo = m_oSmartPenMatch.GetAllMatchInfo(&nElementCount);

		penCount = nElementCount;

		for (int i = 0 ; i < penCount; i++ )
		{
			const TMatchInfo &refMInfo = pMatchInfo[i];
			penInfo[i].ePenState = (refMInfo.eMatchState == E_MISMATCHED) ? E_PEN_STATE_UP : E_PEN_STATE_DOWN;
			penInfo[i].uId = refMInfo.uId;
			penInfo[i].pt = refMInfo.ptPos;
		}
		m_oVirtualHID.InputPoints(penInfo, penCount);

		return;
	}

	//////检测GLBoard白板是否是打开的
    bool bHandHID2Me = DoGLBoardGestureRecognition(pLightSpots, nLightSpotCount);

#ifdef _DEBUG

    if(g_hDebugSampleFile2)
    {       
        for(int i = 0; i < nLightSpotCount; i++)
        {
            char szData[128];
            
            sprintf_s(
                szData,
                _countof(szData),
                "%g,%g,%d,%d\n",
                pLightSpots[i].pt2dPosInVideo[0],
                pLightSpots[i].pt2dPosInVideo[1],
                pLightSpots[i].ptPosInScreen.x,
                pLightSpots[i].ptPosInScreen.y);

            fwrite(szData,1,strlen(szData),g_hDebugSampleFile2);
        }
    }
#endif


    if (bHandHID2Me)
    {
        if (!m_bLastHIDOwnnerAfterGR)
        {//上一次是白板手势识别对象操作的设备，故先把设备Reset
            //m_oVirtualHID.Reset();
            g_oGLBoardGR.ResetSmartMathch();
        }
        //<<debug
        BOOL bDebug = FALSE;
        //debug>>

        m_bIsSmartPenReset = false;

        if (!DoWindowsGestureRecognition(pLightSpots, nLightSpotCount, penInfo, penCount))
        {
           //平滑笔迹
            m_oStrokFilter.DoFilter(penInfo, penCount);

			TSensorModeConfig* TSensorModeConfig = NULL;
			EProjectionMode eProjectionMode = g_tSysCfgData.globalSettings.eProjectionMode;

			TSensorModeConfig = &g_tSysCfgData.vecSensorConfig[0].vecSensorModeConfig[eProjectionMode];
		
			BOOL bEnableStrokeInterpolateTemp= TSensorModeConfig->advanceSettings.bEnableStrokeInterpolate;

            if (FALSE == bEnableStrokeInterpolateTemp)
            {   //不插值
			//	if (penCount >0 )
			//	{
			//	    AtlTrace(_T("penInfo[0].pt.x ==%d,penInfo[0].pt.y ==%d\r\n"), penInfo[0].pt.x , penInfo[0].pt.y);
			//	}

                m_oVirtualHID.InputPoints(penInfo, penCount);
#ifdef _DEBUG
                DebugContactInfo(penInfo, penCount);
#endif
            }
            else
            {
                //做Bezier插值
                m_oBezierInterpolator.DoInterpolate(penInfo, penCount);

                const CBezierInterpolator<PEN_NUM>::InterpolateContainer& container = m_oBezierInterpolator.GetInterpolateResults();

                int nSlotCount = container.GetSlotCount();

                for (int slot = 0; slot < nSlotCount; slot++)
                {
                    const TContactInfo* pInterpolateContact;
                    int nItemCount = container.GetSlotData(slot, &pInterpolateContact);
                    if (nItemCount > 0)
                    {
#ifdef _DEBUG
                        DebugContactInfo(pInterpolateContact, nItemCount);
#endif
						//AtlTrace(_T("pInterpolateContact.pt.x ==%d,pInterpolateContact.pt.y ==%d\r\n"),pInterpolateContact[0].pt.x , pInterpolateContact[0].pt.y);
						m_oVirtualHID.InputPoints(pInterpolateContact, nItemCount);

                        //延迟1ms
                        Sleep(1);
                    }

                }//for
            }
        }
        else
        {
            //置位手势触发标志
            m_bIsTriggeringGuesture = TRUE;
            bDebug = TRUE;
        }
    }
    else
    {
        //置位手势触发标志
        m_bIsTriggeringGuesture = TRUE;
        if (!m_bIsSmartPenReset)
        {
            m_oSmartPenMatch.Reset();
            m_bIsSmartPenReset = true;
        }
    }

    m_bLastHIDOwnnerAfterGR = bHandHID2Me;
}

bool CSpotListProcessor::DoGLBoardGestureRecognition(TLightSpot* pLightSpots, int &nLightSpotCount)
{
    bool bHandHID2Me = true;

    //<<commented by Jiqw : 2015/04/20
    //<<commented reason : 笔触控模式下支持多笔
    //if (GetActualTouchType() == E_DEVICE_PEN_TOUCH_WHITEBOARD)
    //{//笔触控模式下不支持手势识别
    //    return true;
    //}
    //<<commented by Jiqw : 2015/04/20

    if (g_oGLBoardGR.FindTboardWnd(pLightSpots, nLightSpotCount))
    {
        //g_oGLBoardGR.SetIsTouchPadMode(m_oVirtualHID.GetHIDMode() == E_DEV_MODE_TOUCHSCREEN);
        bool isMultiPenMode = g_oGLBoardGR.IsMultiPenMode();
		switch (GetActualTouchType())
		{
		   case E_DEVICE_PEN_TOUCH_WHITEBOARD:
			   //笔触控模式下不支持手势识别
			   if (isMultiPenMode == false) return true;
			   g_oGLBoardGR.SetIsPenTouchDevice(true);

			   break; 
		   case E_DEVICE_FINGER_TOUCH_WHITEBOARD:

			   g_oGLBoardGR.SetIsPenTouchDevice(false);
			   break;
		   case E_DEVICE_FINGER_TOUCH_CONTROL:

			   g_oGLBoardGR.SetIsPenTouchDevice(true);
			   break;
		   case E_DEVICE_PALM_TOUCH_CONTROL:

			   g_oGLBoardGR.SetIsPenTouchDevice(true);
			   break;
		   default:
			  break;
		}
//        if (GetActualTouchType() == E_DEVICE_PEN_TOUCH_WHITEBOARD)
//        {//笔触控模式下不支持手势识别
//            if (isMultiPenMode == false) return true;
//            g_oGLBoardGR.SetIsPenTouchDevice(true);
//        }
//        else
//        {
//            g_oGLBoardGR.SetIsPenTouchDevice(false);
//        }

        bHandHID2Me = g_oGLBoardGR.DoDetection(pLightSpots, nLightSpotCount);
    }    
    /*else
    {
    if (!m_bLastHIDOwnnerAfterGR)
    {
    g_oGLBoardGR.ResetSmartMathch();
    }
    }*/

    return bHandHID2Me;
}

bool CSpotListProcessor::DoWindowsGestureRecognition(const TLightSpot* pLightSpots, int nLightSpotCount, TContactInfo *penInfo, int &nPenCount)
{
    POINT pts[MAX_CAMERA_NUMBER*MAX_OBJ_NUMBER];
    for(int i=0; i< nLightSpotCount; i++)
    {
        pts[i] = pLightSpots[i].ptPosInScreen;
    }

    m_oSmartPenMatch.DoMatch(pts, nLightSpotCount);

    int nElementCount = 0;
    const TMatchInfo* pMatchInfo =  m_oSmartPenMatch.GetAllMatchInfo(&nElementCount); 
	bool bContinedType = false ;
	if (GetActualTouchType() == E_DEVICE_FINGER_TOUCH_WHITEBOARD || GetActualTouchType() == E_DEVICE_FINGER_TOUCH_CONTROL)
	{
		bContinedType = true;
	}
    bool toBeContinued = (bContinedType && !g_oGLBoardGR.IsInputInGLBorad()) ? g_oWinGR.DetermineWhethertoEnterWGR(pLightSpots, nLightSpotCount) : false;

    if (toBeContinued)
    {    
        //CPerfDetector perf(_T("**********CalcMoveDirectionByStatistics()"));
        for(int i = 0; i < nElementCount; i++)
        {
            const TMatchInfo &refMInfo = pMatchInfo[i];

            if (refMInfo.eMatchState == E_MISMATCHED)
            {
                g_oWinGR.AddTouchAfterMatch(refMInfo.uId, refMInfo.ptPos, 0);
                penInfo[i].ePenState = E_PEN_STATE_UP;
            }
            else
            {
                if (refMInfo.eMatchState == E_MATCHED)
                {
                    g_oWinGR.AddTouchAfterMatch(refMInfo.uId, refMInfo.ptPos, 1);
                }
                else
                {
                    g_oWinGR.AddTouchAfterMatch(refMInfo.uId, refMInfo.ptPos, 2);
                }

                penInfo[i].ePenState = E_PEN_STATE_DOWN;
            }

            penInfo[i].uId       = refMInfo.uId;
            penInfo[i].pt        = refMInfo.ptPos;
        }

        /*CPerfDetector perf(_T("****DoWindowsGestureRecognition()"));*/
        toBeContinued = g_oWinGR.DoRecongition();
    }
    else
    {

        /*int debugCount = 0;

        if (nLightSpotCount >= 1)
        {
        AtlTrace(_T("\n[threadId=0x%x]ElementCount =  %d, nLightSpotCount=%d\r\n"), GetCurrentThreadId(), nElementCount, nLightSpotCount);
        for(int i = 0; i < nElementCount; i++)
        {
        AtlTrace(_T("<<<, penInfo uid =  %d matchstate = %d >>\r\n"), pMatchInfo[i].uId, pMatchInfo[i].eMatchState);
        }
        }*/


        //AtlTrace(_T("<<<<AcutalCount = %d, nElementCount = %d\r\n"), nLightSpotCount, nElementCount);

        for(int i = 0; i < nElementCount; i++)
        {
            const TMatchInfo &refMInfo = pMatchInfo[i];
            penInfo[i].ePenState = (refMInfo.eMatchState == E_MISMATCHED)?E_PEN_STATE_UP:E_PEN_STATE_DOWN;
            penInfo[i].uId       = refMInfo.uId;
            penInfo[i].pt        = refMInfo.ptPos;  

            /*penInfo[i].ePenState = (pMatchInfo[i].eMatchState == E_MISMATCHED)?E_PEN_STATE_UP:E_PEN_STATE_DOWN;
            penInfo[i].uId       = pMatchInfo[i].uId;
            penInfo[i].pt        = pMatchInfo[i].ptPos;*/


            ////if (nLightSpotCount >= 2)
            //{
            //    //AtlTrace(_T("penInfo uid =  %d matchstate = %d  nElementCount=%d\r\n"), refMInfo.uId, refMInfo.eMatchState, nElementCount);
            //    AtlTrace(_T("{{{[threadId=0x%x]penInfo uid =%d matchstate =%d LightSpotCount=%d}}\r\n"), GetCurrentThreadId(), (int)pMatchInfo[i].uId, (int)pMatchInfo[i].eMatchState, nLightSpotCount);
            //}


            //debugCount ++;    
            //AtlTrace(_T("******{(Uid: %d), (state: %d), (pos:<%d, %d>)};\r\n"), refMInfo.uId, refMInfo.eMatchState, refMInfo.ptPos.x, refMInfo.ptPos.y);
        }

        /*if(debugCount >=2 )
        {
        AtlTrace(_T("debugCount=%d\n"), debugCount);


        }*/

    }    

    /*if (!toBeContinued)
    {
    if (!m_bIsSmartPenReset)
    {
    m_oSmartPenMatch.Reset();
    m_bIsSmartPenReset = true;
    }
    }*/

    nPenCount = nElementCount;

    return toBeContinued;    
}

//@功能:复位光斑处理器
void CSpotListProcessor::Reset()
{
    for(int i = 0; i < MAX_CAMERA_NUMBER; i++)
    {
        //m_ValidSpotListGroup.aryLightSpotsCount[i] = 0 ;
        m_InputSpotListGroup.aryLightSpotsCount[i] =0;
    }

    //匹配复位
    m_oSmartPenMatch.Reset();

    //Bezier插值器复位
    this->m_oBezierInterpolator.Reset();

    m_SpotListGroupFIFO.Reset();
}

//@功能:设置摄像头的个数
//@参数:uCameraCount,摄像头个数
//void CSpotListProcessor::SetCameraCount(UINT uCameraCount)
//{
//    m_uCameraCount = uCameraCount;
//}

//@功能:启动处理线程
BOOL CSpotListProcessor::StartProcessThread()
{
    assert(NULL == m_hProcessThread);
    DWORD dwThreadId = 0;//线程Id
	
	ResetEvent(this->m_hExitEvent);
    m_hProcessThread = CreateThread(
        NULL,//lpThreadAttributes
        0,   //dwStackSize
        ProcessThreadProc,//lpStartAddress,
        (LPVOID)this,     //lpParameter,
        0,                //dwCreationFlags
        &dwThreadId       //lpThreadId
        );

    return NULL != m_hProcessThread;
}


//@功能:停止处理线程
void CSpotListProcessor::StopProcessThread()
{
    //触发线程退出事件
    SetEvent(m_hExitEvent);
    WaitForSingleObject(m_hProcessThread, INFINITE);
	CloseHandle(m_hProcessThread);
    m_hProcessThread = NULL;    
}


//@功能:线程处理函数
ULONG _stdcall CSpotListProcessor::ProcessThreadProc(LPVOID lpCtx)
{
    //提高线程的优先级别
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

    CSpotListProcessor* lpThis = reinterpret_cast<CSpotListProcessor*>(lpCtx);

    HANDLE hEvents[2];
    hEvents[0] = lpThis->m_hWriteEvent;
    hEvents[1] = lpThis->m_hExitEvent;

    BOOL bExit = FALSE;

    for(;!bExit;)
    {
        DWORD dwWaitResult = WaitForMultipleObjects(_countof(hEvents), hEvents, FALSE, INFINITE);

        switch(dwWaitResult)
        {   
        case WAIT_OBJECT_0://新的写入事件
            if(lpThis->m_bSimulateMode)
            {
                lpThis->SimulateProcess();
            }
            else
            {
                lpThis->ProcessLightSpots();
            }

            break;

        case WAIT_OBJECT_0 + 1://线程退出事件
            bExit = TRUE;
            break;

        }//switch
    }


    return 0UL;
}

//调试函数
void CSpotListProcessor::SimulateProcess()
{
    TLightSpot allLightSpots[MAX_CAMERA_NUMBER*MAX_OBJ_NUMBER];
    FILE* file = NULL;
    fopen_s(&file, "simul_input_data.txt", "r");
    int nCount = 0;
    int nGroupId = 0;
    if(file)
    {
        char szText[1024];
        int nLastNo = 0;
        int nNo = 0;


        TLightSpot spot;
        memset(&spot, 0, sizeof(spot));

        int nSpotCount = 0;

        while(fgets(szText, _countof(szText),file))
        {

            sscanf_s(szText, 
                "%d,%d,%d,%d,%d",
                &nNo,
                &spot.lStdSpotAreaInVideo,
                &spot.ptPosInScreen.x,
                &spot.ptPosInScreen.y,
                &spot.mass
                );
            if(nNo != nLastNo)
            {

                if(nSpotCount != 0)
                {
                    nGroupId ++;
                    OnPostProcess(&allLightSpots[0], nSpotCount);
                    Sleep(1);
                }

                nSpotCount = 0;
            }

            allLightSpots[nSpotCount] = spot;
            nSpotCount ++;

            nLastNo = nNo;

            nCount ++;

        }
    }

    if(file)
    {
        fclose(file);
    }

    m_bSimulateMode = FALSE;
}

//@功能:屏幕分辨率变化事件响应函数
//@参数:nScreenWidth, 新的屏幕宽度
//      nScreenHeight,新的屏幕高度
void CSpotListProcessor::OnDisplayChange(int nScreenWidth, int nScreenHeight)
{
    //对角线尺寸，单位:毫米(mm)
	int nDiagonalPhysicalLength = int(g_tSysCfgData.globalSettings.fScreenDiagonalPhysicalLength);

    SIZE sizeScreen;
    sizeScreen.cx = nScreenWidth;
    sizeScreen.cy = nScreenHeight;

    //给触屏和鼠标设置屏幕物理尺寸和屏幕分辨率
    this->m_oVirtualHID.OnSetTouchScreenDimension(nDiagonalPhysicalLength, sizeScreen);

    //给笔迹相关算法设置屏幕物理尺寸和屏幕分辨率
    this->m_oSmartPenMatch.OnSetTouchScreenDimension(nDiagonalPhysicalLength, sizeScreen);

    //给笔迹滤波器设置屏幕物理尺寸和屏幕分辨率
    this->m_oStrokFilter.OnSetTouchScreenDimension(nDiagonalPhysicalLength, sizeScreen);

    //通知"光斑合并模块"屏幕分辨率发生了变化
    if(theApp.GetScreenMode() >= EScreenModeDouble)
    {
        this->m_oSpotMerger.OnDisplayChange(nScreenWidth,  nScreenHeight);
    }

    //给GLBoard手势和笔迹设置屏幕物理尺寸和屏幕分辨率
    g_oGLBoardGR.OnSetTouchScreenDimension(nDiagonalPhysicalLength, sizeScreen);

}


//@功能:判断是否正在触发手势
BOOL CSpotListProcessor::IsTriggeringGuesture()const
{
    return m_bIsTriggeringGuesture;
}