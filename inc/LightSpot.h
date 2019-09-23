#pragma once
#include "../inc/MultiDimensionPoint.h"
//光亮斑位置信息
struct TLightSpot
{
    int mass                    ;//质量
	int mx                    ;//对x轴的力矩, 合并光斑时需要用到
    int my                    ;//对y轴的力矩,,合并光斑时需要用到

	 TPoint2D pt2dPosInVideo   ;//光斑在视频中的坐标
	LONG lAreaInVideo          ;//光斑的外接矩形面积(视频坐标)
	LONG lStdSpotAreaInVideo   ;//光斑所处位置的标准面积(视频坐标)
    POINT ptPosInScreen        ;//重心坐标(屏幕坐标)


    
	bool operator < (TLightSpot& right)
	{
		return (mass < right.mass);
	}

	bool operator <= (TLightSpot& right)
	{
		return (mass <= right.mass);
	}


	bool operator > (TLightSpot& right)
	{
		return (mass > right.mass);
	}

	bool operator >= (TLightSpot& right)
	{
		return (mass >= right.mass);
	}

};