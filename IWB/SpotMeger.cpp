#include "stdafx.h"
//#include "headers.h"
CSpotMerger::CSpotMerger()
{
    m_nSeperateX =  m_nMergeAreaLeftBorder = m_nMergeAreaRightBorder = 0;
	//<<temp, 2017/08/22
    int nCyScreen = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	//>>
    m_nMergeDistThreshold = nCyScreen * 5 / 100;

}

CSpotMerger::~CSpotMerger()
{

}

//@功能:执行屏幕融合区光斑合并操作
//@参数:pLightSpots，输入/输出参数, 光斑数组
//      pLightSpotCount, 输入/输出参数，指向光斑个数的指针
void CSpotMerger::DoMerge(TLightSpot* pLightSpots, int* pLightSpotCount)
{
    int nSpotCount = *pLightSpotCount;

    if(nSpotCount < 2) return;

    int i = 0;

    

    int MERGE_THRESHOLD = m_nMergeDistThreshold * m_nMergeDistThreshold;//融合门限。

    enum ESpotLocation
    {   
        E_NOT_IN_MERGE_AREA,
        E_IN_RIGHT_MERGE_AREA,
        E_IN_LEFT_MERGE_AREA
    };

    
    //AtlTrace(_T("Before Merge %d"), nSpotCount);
    BOOL bDoMerge = FALSE;

    while(i < nSpotCount)
    {
#ifdef _DEBUG
        if(nSpotCount >=1)
        {
            //AtlTrace(_T("Before Merge nSpotCount = %d\n"), nSpotCount);
            int nDebug = 0;
        }
#endif
         TLightSpot& s1 = pLightSpots[i];

         ESpotLocation eS1Location = E_NOT_IN_MERGE_AREA;//S1所在区域标志

         //if(s1.ptPosInScreen.x < m_nSeperateX && (m_nSeperateX - s1.ptPosInScreen.x) < nHalfMergeAreaWidth)
         if(m_nMergeAreaLeftBorder <= s1.ptPosInScreen.x  && s1.ptPosInScreen.x<= m_nSeperateX )
         {
            eS1Location = E_IN_LEFT_MERGE_AREA;
         }
         //else if(s1.ptPosInScreen.x > m_nSeperateX && (s1.ptPosInScreen.x - m_nSeperateX ) < nHalfMergeAreaWidth)
         else if(m_nSeperateX < s1.ptPosInScreen.x && s1.ptPosInScreen.x <= m_nMergeAreaRightBorder)
         {
            eS1Location = E_IN_RIGHT_MERGE_AREA;
         }



        if( E_IN_LEFT_MERGE_AREA == eS1Location || E_IN_RIGHT_MERGE_AREA == eS1Location)
        {
            
            for(int j = i+1; j < nSpotCount; j++)
            {
                
                TLightSpot& s2 = pLightSpots[j];

                ESpotLocation eS2Location = E_NOT_IN_MERGE_AREA;//S2所在区域标志


                if(m_nMergeAreaLeftBorder <= s2.ptPosInScreen.x  && s2.ptPosInScreen.x < m_nSeperateX)
                {
                    eS2Location = E_IN_LEFT_MERGE_AREA;
                }
                else if( m_nSeperateX <= s2.ptPosInScreen.x  && s2.ptPosInScreen.x <= m_nMergeAreaRightBorder)
                {
                    eS2Location = E_IN_RIGHT_MERGE_AREA;
                }


                if(eS2Location != E_NOT_IN_MERGE_AREA) 
                {
                    int dx = s2.ptPosInScreen.x - s1.ptPosInScreen.x;
                    int dy = s2.ptPosInScreen.y - s1.ptPosInScreen.y;

                    int R2 = dx*dx + dy*dy;//两个光斑之间的距离

                    if(R2 < MERGE_THRESHOLD)
                    {
                        //光斑距离够近, 需要被合并
                        
                        
                        int total_mass = s1.mass + s2.mass;

                        //按照光点的质量权重求取合并后的屏幕坐标
                        s1.ptPosInScreen.x = (s1.mass * s1.ptPosInScreen.x)/total_mass + (s2.mass * s2.ptPosInScreen.x)/total_mass;
                        s1.ptPosInScreen.y = (s1.mass * s1.ptPosInScreen.y)/total_mass + (s2.mass * s2.ptPosInScreen.y)/total_mass;

                        s1.mass += s2.mass;


                        ////取面积最大的光斑值, 
                        ////注意:不能够简单地将光斑质量相加,否则光斑质量加倍，触发手势。
                        //if(s1.mass < s2.mass)
                        //{
                        //    s1 = s2;
                        //}
                        s1.lStdSpotAreaInVideo += s2.lStdSpotAreaInVideo;

                        //外接矩形面积相加，简化运算,
                        //复杂的计算是:
                        //bound.left   = min(s1.bound.left   , s2.bound.left  )
                        //bound.right  = max(s1.bound.right  , s2.bound.right )
                        //bound.top    = min(s1.bound.top    , s2.bound.top   )
                        //bound.bottom = max(s1.bound.bottom , s2.bound.bottom)
                        s1.lAreaInVideo += s2.lAreaInVideo;

                        //后面的元素往前挪动一个位置
                        for(int k =j+1; k < nSpotCount; k++)
                        {
                            pLightSpots[k-1] = pLightSpots[k];
                        }
                        //AtlTrace(_T("Merge a spot\n"));
                        
                        nSpotCount --;

                        bDoMerge = TRUE;
                    }//if

                }//if

            }//for
        }//if
        
        i++;

    }//while

#ifdef _DEBUG
    if(nSpotCount)
    {
        //AtlTrace(_T("After Merge nSpotCount = %d, bDoMerge=%d\n\n"), nSpotCount, bDoMerge);
    }
#endif

    *pLightSpotCount = nSpotCount;

}



//@功能:屏幕分辨率变化事件响应函数
//@参数:nScreenWidth, 新的屏幕宽度
//      nScreenHeight,新的屏幕高度
void CSpotMerger::OnDisplayChange(int nScreenWidth, int nScreenHeight)
{
    this->m_nSeperateX =nScreenWidth   >> 1;//除以2
    int nMergeAreaWidth = nScreenWidth >> 6;//除以64
    this->m_nMergeAreaLeftBorder  = m_nSeperateX - nMergeAreaWidth/2;
    this->m_nMergeAreaRightBorder = m_nSeperateX + nMergeAreaWidth/2;

    m_nMergeDistThreshold = nScreenHeight * 5 / 100;
}