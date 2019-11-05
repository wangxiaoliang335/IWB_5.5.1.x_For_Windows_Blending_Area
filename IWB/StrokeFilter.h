#pragma once
//笔迹滤波器
//X(smooth_new) = λ * X(smooth_last) + (1 - λ)*X(new)
//滤波系数 λ∈[0,1], λ越大，滤波效果越明显
//滤波系数λ的确定方法
//随着笔迹前后两帧移动距离矢量的长度, 动态更新滤波系数λ。
//设平滑系数与距离成线性反比
//λ = λmax - (λmax - λmin) * D/D_max 
//这里D为当前点相对于前一输入点的距离。
//D=0时，     λ = λmax
//D=D_max时， λ = λmin

template <int MAX_STROKE_NUM>
class CStrokeFilter
{
public:
    CStrokeFilter()
    {
        //根据物理尺寸和屏幕分辩率，计算相应的逻辑距离D1和D2
        int nScreenDiagonalPhysicalLength = int(DEFAULT_SCREEN_DIAGONAL_LENGTH);//缺省物理尺寸80inches
        int nCx = GetSystemMetrics(SM_CXVIRTUALSCREEN);
        int nCy = GetSystemMetrics(SM_CYVIRTUALSCREEN);

        SIZE screenResolution;
        screenResolution.cx = nCx;
        screenResolution.cy = nCy;

        OnSetTouchScreenDimension(nScreenDiagonalPhysicalLength, screenResolution);
    }

    ~CStrokeFilter()
    {

    }

    //@功能:设置触屏的尺寸事件响应函数
    //@参数:nPhysicalDiagonalLength, 屏幕尺寸,对角线长度, 单位:毫米
    //      szLogicalDimension, 屏幕逻辑尺寸，单位:像素
    void OnSetTouchScreenDimension(int  nPhysicalDiagonalLength, SIZE szLogicalDimension)
    {
        int nCx = szLogicalDimension.cx;
        int nCy = szLogicalDimension.cy;

        int nScreenDialgonalLogicalLength = (int)sqrt((float)(nCx*nCx + nCy*nCy));

        m_nMaxDistance = (KD2PhysicalDistance * nScreenDialgonalLogicalLength)/nPhysicalDiagonalLength;

        InitFilter();
    }


    //@功能:对输入的笔迹进行滤波处理
    //@参数:pContactInfo, 输入/输出参数, 触控点信息
    //      nCount, 输入的触控点个数
    void DoFilter(TContactInfo* pContactInfo, int nCount)
    {
        for(int i=0; i < nCount; i++)
        {
            TContactInfo& contactInfo = pContactInfo[i];
            UINT32 uId = contactInfo.uId;

            if(uId >= MAX_STROKE_NUM)
            {//数组下标越界。
                continue;
            }

            TFilterState& filter = m_aryFilters[uId];

            switch(filter.m_eFilterState)
            {
            case E_FILTER_STATE_UP://弹起状态

                if(pContactInfo->ePenState == E_PEN_STATE_DOWN)
                {

                    filter.m_eFilterState      = E_FILTER_STATE_DN;
                    filter.m_LastSmoothPoint.d[0] = double(contactInfo.pt.x);
                    filter.m_LastSmoothPoint.d[1] = double(contactInfo.pt.y);

                    filter.m_LastInputPoint  = filter.m_LastSmoothPoint;
                    filter.m_uRepeatSampleCount = 0;
                }
                 
                break;


            case E_FILTER_STATE_DN://"按下"稳态
                if(contactInfo.ePenState ==  E_PEN_STATE_UP)
                {
                    //filter.m_uFrameCount  = 0;
                    filter.m_eFilterState = E_FILTER_STATE_UP;

                    contactInfo.pt.x = long(filter.m_LastSmoothPoint.d[0] + .5);
                    contactInfo.pt.y = long(filter.m_LastSmoothPoint.d[1] + .5);

                }
                else if(contactInfo.ePenState == E_PEN_STATE_DOWN)
                {
                    const TPoint2D& ptLastInput   = filter.m_LastInputPoint;
                    const TPoint2D& ptLastSmooth  = filter.m_LastSmoothPoint;
                    const POINT& ptNewInput      = contactInfo.pt;


                    //int dx = ptNewInput.x - ptLastInput.x;
                    //int dy = ptNewInput.y - ptLastInput.y;
                    double dx = double(ptNewInput.x) - ptLastSmooth.d[0];
                    double dy = double(ptNewInput.y) - ptLastSmooth.d[1];

                    if(dx == 0 && dy == 0)
                    {
                        filter.m_uRepeatSampleCount ++;
                    }
                    else
                    {
                        filter.m_uRepeatSampleCount = 0;
                    }

                    double dx2 = dx*dx;
                    double dy2 = dy*dy;

                    double d2   = dx2 + dy2;
                    double distance = sqrt(d2);


                    TPoint2D ptSmooth;

                    if(filter.m_uRepeatSampleCount >= 3)//连续多次是同一个输入点,则给输入点就是实际输入点,跳过平滑处理
                    {
                        ptSmooth.d[0] = double(ptNewInput.x);
                        ptSmooth.d[1] = double(ptNewInput.y);
                    }
                    else if(distance < m_nMaxDistance)
                    {
                        
                        //计算平滑系数
                        //线性模型
                        //λ = λmax - (λmax - λmin) * D / D_max
                        //double smooth_coef = m_dbMaxSmoothCoef - (m_dbMaxSmoothCoef - m_dbMinSmoothCoef)*distance / m_nMaxDistance;

                        //抛物线模型
                        //λ = λmin + (λmax - λmin) * sqrt(1 - D/D_max)
                        double smooth_coef = m_dbMinSmoothCoef + (m_dbMaxSmoothCoef - m_dbMinSmoothCoef)*sqrt(1 - distance / m_nMaxDistance);
                       
                        ptSmooth.d[0] = smooth_coef * double(ptLastSmooth.d[0]) + (1.0 - smooth_coef) * double(ptNewInput.x);
                        ptSmooth.d[1] = smooth_coef * double(ptLastSmooth.d[1]) + (1.0 - smooth_coef) * double(ptNewInput.y);
                    }
                    else
                    {
                        double smooth_coef =  m_dbMinSmoothCoef;

                        ptSmooth.d[0] = smooth_coef * double(ptLastSmooth.d[0]) + (1.0 - smooth_coef) * double(ptNewInput.x);
                        ptSmooth.d[1] = smooth_coef * double(ptLastSmooth.d[1]) + (1.0 - smooth_coef) * double(ptNewInput.y);
                    }


                    
                    //经过测试，选取平滑系数0.6时，笔迹较为平滑。
                    /*
                    //注意:弹起状态时不能够滤波，否则会丢失弹起事件
                    static double smooth_coef = 0.6;
                    ptSmooth.x = smooth_coef * double(ptLastSmooth.x) + (1.0 - smooth_coef) * ptNewInput.x;
                    ptSmooth.y = smooth_coef * double(ptLastSmooth.y) + (1.0 - smooth_coef) * ptNewInput.y;
                    */
                    contactInfo.pt.x = long(ptSmooth.d[0] + .5);
                    contactInfo.pt.y = long(ptSmooth.d[1] + .5);

                    filter.m_LastSmoothPoint = ptSmooth;
                    filter.m_LastInputPoint.d[0] = double(ptNewInput.x);
                    filter.m_LastInputPoint.d[1] = double(ptNewInput.y);
                }

                break;

            }//switch


        }
        
        
    }

private:
    //<added by Jiqw 201505191136
    void InitFilter()
    {


        //初始化滤波器状态
        for(UINT32 i = 0; i < _countof(m_aryFilters); i++)
        {
            //m_aryFilters[i].m_LastSmoothPoint.x  = 0;
            //m_aryFilters[i].m_LastSmoothPoint.y  = 0;
            //m_aryFilters[i].m_LastInputPoint.x   = 0;
            //m_aryFilters[i].m_LastInputPoint.y   = 0;
            m_aryFilters[i].m_uRepeatSampleCount = 0;
            m_aryFilters[i].m_eFilterState       = E_FILTER_STATE_UP;
        }

        m_dbMaxSmoothCoef = 1.0;
        m_dbMinSmoothCoef = 0.0;
    }
    //>

protected:
    enum E_FilterState
    {
        E_FILTER_STATE_UP   ,//光笔弹起状态
        E_FILTER_STATE_DN   ,//光笔按下状态
        //E_FILTER_STATE_DN_1 ,//光斑第一次按下状态
        //E_FILTER_STATE_DN_2 ,//光斑持续按下状态
    };

    static const int DEBOUCNE_DELAY = 2;//消抖延迟
    static const int MAGINIFY = 1024;

    //滤波状态
    struct TFilterState{
    //POINT         m_LastPoints[DEBOUCNE_DELAY] ;//笔迹上一个点的位置。
    TPoint2D       m_LastSmoothPoint  ;//笔迹上一个平滑点的位置。
    TPoint2D       m_LastInputPoint   ;//笔迹上一个平滑点的位置。
    UINT           m_uRepeatSampleCount;//同一点重复采样计数
    E_FilterState m_eFilterState ;//滤波状态机状态

    };//

    TFilterState m_aryFilters[MAX_STROKE_NUM];//记录过滤器状态的数组,每支笔对应一个滤波器


    int   m_nMaxDistance;//最大平滑距离，单位像素
    double m_dbMaxSmoothCoef;//最大平滑系数
    double m_dbMinSmoothCoef;//最小平滑系数

    //static const int KD1PhysicalDistance = 5; //D1对应的物理距离，其只和物理尺寸相关,单位为mm，即物理在任何投影区域，只要物理移动距离<m_nD1PhysicalDistance，则认为其未移动
#ifdef PICO_PROJECTOR

    static const int KD1PhysicalDistance = 2; //D1对应的物理距离，其只和物理尺寸相关,单位为mm，即物理在任何投影区域，只要物理移动距离<m_nD1PhysicalDistance，则认为其未移动

    //<<added by xuke 2016/09/024
    static const int KD2PhysicalDistance = 5; //D2对应的物理距离，其只和物理尺寸相关,单位为mm，即物理在任何投影区域，只要物理移动距离>m_nD2PhysicalDistance，则认为不需要作平滑操作
    //>>

#else
    static const int KD1PhysicalDistance = 2; //D1对应的物理距离，其只和物理尺寸相关,单位为mm，即物理在任何投影区域，只要物理移动距离<m_nD1PhysicalDistance，则认为其未移动
    static const int KD2PhysicalDistance = 14*2; //D2对应的物理距离，其只和物理尺寸相关,单位为mm，即物理在任何投影区域，只要物理移动距离>m_nD2PhysicalDistance，则认为不需要作平滑操作
#endif




};
