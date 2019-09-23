#pragma once
//功能:彩色光标生成
class CColorCursor
{
public:
	CColorCursor()
	{

	}

	virtual ~CColorCursor()
	{

		if(this->m_hCursor)
		{
			:: DestroyIcon(this->m_hCursor);
		}
	}



	//@功能:生成实心矩形光标
	//@参数:xHotSpot, 光标热点水平坐标
	//      yHotSpot, 光标热点垂直坐标 
	//      nCxCurosr,光标尺寸宽度
	//      nCyCursor,光标尺寸高度
	//     
	//      rcSize, 矩形的位置
	//      clrFill,矩形填充颜色
	//      clrBorder, 矩形边的颜色
	//      nOverlapCursorResID, 需要叠加的光标资源的ID.
	BOOL CreateSolidRectangleCursor(
		 //int  xHotspot,
		 //int  yHotspot,
		 int nCxCursor,
		 int nCyCursor,
		 //RECT& rcSize,
		 COLORREF  clrFill,//矩形填充颜色
		 COLORREF  clrBorder,//矩形边的颜色
		UINT  nOverlapCursorResID
		)
	{

		BOOL bRet = FALSE;

		HDC hDC					= ::GetDC(NULL);
		HDC hMainDC				= ::CreateCompatibleDC(hDC); 

		 int  xHotspot = nCxCursor/2;
		 int  yHotspot = nCyCursor/2;

		//int nCxCursor           = GetSystemMetrics(SM_CXCURSOR)*2;
		//int nCyCursor           = GetSystemMetrics(SM_CYCURSOR)*2;
		HBITMAP hSrcBitmap      = ::CreateCompatibleBitmap(hDC, nCxCursor, nCyCursor);
		HBITMAP hBmpOld         = (HBITMAP)SelectObject(hMainDC, hSrcBitmap);

		COLORREF clrBackground  = RGB(0,128,128);
		

		HBRUSH   hBrushBk       = ::CreateSolidBrush(clrBackground);
	

		//填充背景
		RECT rcArea;
		rcArea.left   = 0;
		rcArea.top    = 0;
		rcArea.right  = nCxCursor;
		rcArea.bottom = nCyCursor;
		FillRect(hMainDC, &rcArea, hBrushBk);


		//绘制矩形
		HPEN     hPen           = ::CreatePen(PS_SOLID, 1, clrBorder);
		HPEN     hPenOld        = (HPEN)SelectObject(hMainDC, hPen);
		HBRUSH   hFillBrush     = ::CreateSolidBrush(clrFill);
								   SelectObject(hMainDC, hFillBrush);
		HBRUSH   hBrushOld      = (HBRUSH)SelectObject(hMainDC, hPen);

		Rectangle(hMainDC, rcArea.left,  rcArea.top, rcArea.right, rcArea.bottom);
		


		if(nOverlapCursorResID != 0)
		{

			
			HICON hCursorRes = (HICON)LoadImage(GetModuleHandle(NULL),  MAKEINTRESOURCE(nOverlapCursorResID), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);

			ICONINFO iconInfo;


			bRet = GetIconInfo(hCursorRes, &iconInfo); 

			if(bRet)
			{
				BITMAP bitmap;
				GetObject(iconInfo.hbmMask, sizeof(bitmap), &bitmap);
				HDC hMemDC = CreateCompatibleDC(hMainDC);
				HBITMAP hMemBitmap = CreateCompatibleBitmap(hMainDC, bitmap.bmWidth, bitmap.bmHeight);
				



				DeleteObject(hMemBitmap);
				DeleteDC(hMemDC);

			}


		}


		SelectObject(hMainDC, hBrushOld);
		SelectObject(hMainDC, hPenOld);
		SelectObject(hMainDC, hBmpOld);
		DeleteDC(hMainDC);
		ReleaseDC(NULL,hDC);

		do
		{

			//Create the AND and XOR masks for the bitmap
			HBITMAP hAndMask = NULL;
			HBITMAP hXorMask = NULL;
			GetMaskBitmaps(hSrcBitmap, clrBackground, hAndMask,hXorMask);
			if(NULL == hAndMask || NULL == hXorMask)
			{
				break;
			}



			//Create the cursor using the masks and the hotspot values provided
			ICONINFO iconinfo = {0};
			iconinfo.fIcon		= FALSE;
			iconinfo.xHotspot	= xHotspot;
			iconinfo.yHotspot	= yHotspot;
			iconinfo.hbmMask	= hAndMask;
			iconinfo.hbmColor	= hXorMask;

			m_hCursor = ::CreateIconIndirect(&iconinfo);

		}while(0);


		return TRUE;

	}

	operator HCURSOR ()
	{	
		return m_hCursor;
	}



protected:
	
	static void GetMaskBitmaps(HBITMAP hSourceBitmap,COLORREF clrTransparent,
							   HBITMAP &hAndMaskBitmap,HBITMAP &hXorMaskBitmap)
	{
		HDC hDC					= ::GetDC(NULL);
		HDC hMainDC				= ::CreateCompatibleDC(hDC); 
		HDC hAndMaskDC			= ::CreateCompatibleDC(hDC); 
		HDC hXorMaskDC			= ::CreateCompatibleDC(hDC); 

		//Get the dimensions of the source bitmap
		BITMAP bm;
		::GetObject(hSourceBitmap,sizeof(BITMAP),&bm);

		
		hAndMaskBitmap	= ::CreateCompatibleBitmap(hDC,bm.bmWidth,bm.bmHeight);
		hXorMaskBitmap	= ::CreateCompatibleBitmap(hDC,bm.bmWidth,bm.bmHeight);

		//Select the bitmaps to DC
		HBITMAP hOldMainBitmap = (HBITMAP)::SelectObject(hMainDC,hSourceBitmap);
		HBITMAP hOldAndMaskBitmap	= (HBITMAP)::SelectObject(hAndMaskDC,hAndMaskBitmap);
		HBITMAP hOldXorMaskBitmap	= (HBITMAP)::SelectObject(hXorMaskDC,hXorMaskBitmap);

		//Scan each pixel of the souce bitmap and create the masks
		COLORREF MainBitPixel;
		for(int x=0;x<bm.bmWidth;++x)
		{
			for(int y=0;y<bm.bmHeight;++y)
			{
				MainBitPixel = ::GetPixel(hMainDC,x,y);
				if(MainBitPixel == clrTransparent)
				{
					::SetPixel(hAndMaskDC,x,y,RGB(255,255,255));
					::SetPixel(hXorMaskDC,x,y,RGB(0,0,0));
				}
				else
				{
					::SetPixel(hAndMaskDC,x,y,RGB(0,0,0));
					::SetPixel(hXorMaskDC,x,y,MainBitPixel);
				}
			}
		}
		
		::SelectObject(hMainDC,hOldMainBitmap);
		::SelectObject(hAndMaskDC,hOldAndMaskBitmap);
		::SelectObject(hXorMaskDC,hOldXorMaskBitmap);

		::DeleteDC(hXorMaskDC);
		::DeleteDC(hAndMaskDC);
		::DeleteDC(hMainDC);

		::ReleaseDC(NULL,hDC);
	}


protected: 
	HCURSOR m_hCursor;


};