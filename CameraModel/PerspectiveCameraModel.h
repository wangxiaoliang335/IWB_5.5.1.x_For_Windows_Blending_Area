#pragma once
#ifdef CAMERAMODEL_EXPORTS
#define PERSPECTIVE_MODEL_API _declspec(dllexport)
#else
#define PERSPECTIVE_MODEL_API _declspec(dllimport)
#endif

#include "../inc/MultiDimensionPoint.h"

class PERSPECTIVE_MODEL_API CPerspectiveCameraModel
{
public:
    CPerspectiveCameraModel();

    BOOL CalcParams(const TPoint2D* pPointsInImage, const TPoint2D* pPointsOnPlane, int nPtNumber, const SIZE& sImage);

    //@功能;根据视频坐标计算得出计算机屏幕坐标
    //@参数:ptImage, 光斑的视频图像坐标
    //      ptScreen, 保存计算机屏幕坐标的地址
    BOOL GetScreenPt(const TPoint2D* ptImage, TPoint2D* ptScreen, int nPtNumber);

    BOOL IsValidParameters();

    void SetParameters(const double* pParameters, int nParametersCount);

    int GetParameterCount()const;
    const double* GetParameters()const;
protected:
    double m_Parameters[9];

    double(&m_H)[3][3];
    SIZE   m_szImage;
};