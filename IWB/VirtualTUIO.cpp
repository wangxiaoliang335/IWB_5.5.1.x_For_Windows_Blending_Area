#include "stdafx.h"
#include "VirtualTUIO.h"


CVirtualTUIO::CVirtualTUIO()
	:tuioServer(NULL),
	tcp_sender(NULL),
	m_nCxScreen(1920),
	m_nCyScreen(1080),
	strIP(""),
	nPort(0)
{
	LoadTUIOConfig();
}

CVirtualTUIO::~CVirtualTUIO()
{
	if(tuioServer != NULL)
	{
	     CloseTUIOServer();
    }

}

void CVirtualTUIO::OpenTUIOServer()
{
	if (tuioServer == NULL)
	{
		if (strIP == "" && nPort ==0)
		{
			/////UDP
	        tuioServer = new TuioServer();
		    ////TCP
	        tcp_sender = new TcpSender(3333);
		}
		else {
			if (strIP == "")
			{
				strIP = "127.0.0.1";
			}
			if (nPort == 0)
			{
				nPort = 3333;
			}
			/////UDP
			tuioServer = new TuioServer((char *)(LPCTSTR)strIP, nPort);
			////TCP
			tcp_sender = new TcpSender((char *)(LPCTSTR)strIP, nPort);
		}

	    tuioServer->addOscSender(tcp_sender);
		InitTuio();
	}
}
void CVirtualTUIO::InitTuio()
{
	TuioTime::initSession();
	frameTime = TuioTime::getSessionTime();

	m_nCxScreen = GetSystemMetrics(SM_CXSCREEN);
	m_nCyScreen = GetSystemMetrics(SM_CYSCREEN);

//	tuioServer->setSourceName("VirtualTUIO");
	tuioServer->enableCursorProfile(false);
	tuioServer->enableObjectProfile(false);
	tuioServer->enableBlobProfile(false);
}
void CVirtualTUIO::CloseTUIOServer()
{

	if (tcp_sender !=NULL)
	{
	    delete tcp_sender;
		tcp_sender = NULL;
	}
	if (tuioServer!= NULL)
	{
	    delete tuioServer; 
		tuioServer = NULL;
	}

}

BOOL CVirtualTUIO::InputTUIOPoints(const TContactInfo* pPenInfos, int nPenCount)
{

	frameTime = TuioTime::getSessionTime();
	tuioServer->initFrame(frameTime);

	if (nPenCount>1)
	{
		int i = 0;
	}
	for (int i = 0; i < nPenCount; i++)
	{
		TuioCursor* match = NULL;

		float X = (float)pPenInfos[i].pt.x / m_nCxScreen;
		float Y = (float)pPenInfos[i].pt.y / m_nCyScreen;

		if (!ActiveCursorList.empty())
		{
	       for (std::list<VecTuioCursor>::iterator iter = ActiveCursorList.begin(); iter != ActiveCursorList.end(); iter++)
	       {
			  if ((*iter).nId == pPenInfos[i].uId)
			  {
				  TuioCursor *cursor = (*iter).tuiocursor;
			      match = (*iter).tuiocursor;
				  ////找到是按下状态
				  if (pPenInfos[i].ePenState == E_PEN_STATE_DOWN)
				  {	
					    tuioServer->updateTuioCursor(cursor, X, Y);
						(*iter).bInvalid = false;
				  }
				  else {				  
					   //找到是弹起状态
					  if (!(*iter).bInvalid)
					  {
					     tuioServer->removeTuioCursor(cursor);
                         (*iter).bInvalid = true;
					  }
				  }
			  }
	       }
		}
		if (match ==NULL)
		{
			VecTuioCursor vecCursor;
			vecCursor.tuiocursor = tuioServer->addTuioCursor(X, Y);
			vecCursor.nId = pPenInfos[i].uId;
			vecCursor.bInvalid = false;
			ActiveCursorList.push_back(vecCursor);
		}
	}

	std::list<VecTuioCursor>::iterator iter;
	std::list<VecTuioCursor>::iterator it;
	for (iter = ActiveCursorList.begin(); iter != ActiveCursorList.end();)
	{
		if ((*iter).bInvalid)
		{
			//it = iter;
			//it++;
			
			iter  = ActiveCursorList.erase(iter);
			//iter = it;
		}
		else
		{
			iter++;
		}
	}

//	tuioServer->stopUntouchedMovingCursors();
	tuioServer->commitFrame(); //时间

	return TRUE;
}

void CVirtualTUIO::Reset()
{
	ActiveCursorList.clear();
}

bool CVirtualTUIO::LoadTUIOConfig()
{
	const TCHAR* lpszXMLFileName = _T("TUIOConfig.xml");
	TiXmlDocument oXMLDoc;
	if (!oXMLDoc.LoadFile(CT2CA(lpszXMLFileName), TIXML_ENCODING_UTF8))
	{
		return false;
	}
	TiXmlElement* pRootElement = oXMLDoc.RootElement();
	if (pRootElement == NULL) return false;

	TiXmlNode* pChild = NULL;
	if (pChild = pRootElement->IterateChildren("Address", pChild))
	{
		const char* NodeName = pChild->Value();//节点名称
		const char* lpszIP = ((TiXmlElement*)pChild)->Attribute("IP");
		const char* lpszPort = ((TiXmlElement*)pChild)->Attribute("Port");
		if (lpszIP == NULL || strlen(lpszIP) == 0 || lpszPort == NULL || strlen(lpszPort) == 0)
		{
			return false;
		}
		strIP = lpszIP;
		nPort = atoi(lpszPort);
	}

	return true;
}