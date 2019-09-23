#pragma once


#include <list>
#include <math.h>
#include "../IWB_TUIO/TUIO/TuioServer.h"
#include "../IWB_TUIO/TUIO/TuioCursor.h"
#include "../IWB_TUIO/oscpack/osc/OscTypes.h"
#include "../IWB_TUIO/TUIO/TcpSender.h"

#include "../IWB_TUIO/sdl/include/SDL.h"
#include "../IWB_TUIO/sdl/include/SDL_thread.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include "../IWB_TUIO/sdl/include/GL/glut.h"

using namespace TUIO;

struct VecTuioCursor
{
	TuioCursor *tuiocursor;
	int         nId;
	bool        bInvalid;
};

class CVirtualTUIO
{
public:
	CVirtualTUIO();
	~CVirtualTUIO();
public:
	void   OpenTUIOServer();
	void   CloseTUIOServer();
	void   InitTuio();
	BOOL   InputTUIOPoints(const TContactInfo* pPenInfos, int nPenCount);
	bool   LoadTUIOConfig();
	void   Reset();


protected:
	TuioServer  *tuioServer;
	OscSender   *tcp_sender;
	TuioTime    frameTime;
	int         m_nCxScreen;   //屏幕的宽度
	int         m_nCyScreen;   //屏幕的高度
	CString     strIP;
	int         nPort;
	std::list<VecTuioCursor> ActiveCursorList;

};

