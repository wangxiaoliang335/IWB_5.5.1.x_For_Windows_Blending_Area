#include "stdafx.h"
#include "VirtualTUIO.h"


CVirtualTUIO::CVirtualTUIO()
	:tuioServer(NULL),
	tcp_sender(NULL),
	m_nCxScreen(1920),
	m_nCyScreen(1080),
	szIP("127.0.0.1"),
	m_nPort(3333)
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
		if ( (strcmp(szIP,"127.0.0.1")==0) && m_nPort ==3333)
		{
			/////UDP
	        tuioServer = new TuioServer();
		    ////TCP
	        tcp_sender = new TcpSender(m_nPort);
		}
		else {
			/////UDP
			tuioServer = new TuioServer(szIP, m_nPort);
			////TCP
			tcp_sender = new TcpSender(szIP, m_nPort);
		}
		if (tuioServer!= NULL)
		{
	        tuioServer->addOscSender(tcp_sender);
		    InitTuio();
		}
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
	Reset();
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
	//std::list<VecTuioCursor>::iterator it;
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

void CVirtualTUIO::ReopenTUIOServer()
{
	CloseTUIOServer();
	OpenTUIOServer();
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
		strcpy_s(szIP, _countof(szIP),lpszIP);
		m_nPort = atoi(lpszPort);
	}

	return true;
}

bool CVirtualTUIO::SaveTUIOConfig()
{

	const TCHAR* lpszConfigFilePath = _T("TUIOConfig.xml");
	TiXmlDocument oXMLDoc;
	TiXmlDeclaration Declaration("1.0", "UTF-8", "no");
	oXMLDoc.InsertEndChild(Declaration);

	TiXmlElement * pConfig = new TiXmlElement("Config");
	oXMLDoc.LinkEndChild(pConfig);

	TiXmlElement * pElement = new TiXmlElement("Address");
	pElement->SetAttribute("IP", szIP);
	pElement->SetAttribute("Port", m_nPort);
	pConfig->LinkEndChild(pElement);

	//以UTF-8编码格式保存
	TiXmlPrinter  printer;
	oXMLDoc.Accept(&printer);
	char UTF8BOM[3] = { '\xEF','\xBB','\xBF' };
	std::ofstream theFile;
	theFile.open(CT2W(lpszConfigFilePath), ios_base::out | ios_base::trunc);

	if (theFile.is_open())
	{
		theFile.write(UTF8BOM, 3);

		//中文GB2312编码页号
		UINT CP_GB2312 = 936;

		//MBCS 转 Unicode
		CA2W wcharContent(printer.CStr(), CP_GB2312);

		//计算UTF8编码的长度
		int utf8_length =
			WideCharToMultiByte(
				CP_UTF8,
				0,
				wcharContent,
				-1,
				NULL,
				0,
				NULL,
				NULL);

		char* utf_8_buf = (char*)malloc(utf8_length);

		//Unicode转为UTF8编码
		WideCharToMultiByte(
			CP_UTF8,
			0,
			wcharContent,
			-1,
			utf_8_buf,
			utf8_length,
			NULL,
			NULL);

		theFile.write(utf_8_buf, strlen(utf_8_buf));

		theFile.close();
		free(utf_8_buf);
	}

	return false;
}
//@功能：设置TUIO时的IP地址和端口号
////////////////////////////////
void  CVirtualTUIO::SetIPadressAndPort(DWORD IP,int nPort)
{
	char szTempIP[24];
	sprintf_s(szTempIP,_countof(szTempIP),"%d.%d.%d.%d", (IP & 0xFF000000)>>24 , (IP & 0x00FF0000) >> 16, (IP & 0x0000FF00) >> 8, (IP & 0x000000FF));

	if (strcmp(szTempIP,szIP) != 0 || m_nPort != nPort)
	{
	   strcpy_s(szIP,_countof(szIP), szTempIP);
	   m_nPort = nPort;

       SaveTUIOConfig();
	   ReopenTUIOServer();
	}
}

//@功能：得到TUIO时的IP地址
/////////////////////////
DWORD  CVirtualTUIO::GetIPadress()
{
	DWORD   dwIP;
	dwIP = inet_addr(szIP);
	return dwIP;
}
//@功能：得到TUIO时的端口号
/////////////////////////
int CVirtualTUIO::GetPort()
{
	return m_nPort;
}
