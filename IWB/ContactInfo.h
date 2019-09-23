#pragma once

enum EPenState
{
    E_PEN_STATE_UP   = 0,
    E_PEN_STATE_DOWN = 1,
};

struct TContactInfo
{
    UINT  uId;//从0开始的触控点编号
    POINT  pt;//触控点的屏幕坐标    
    EPenState ePenState;//笔的状态。
};