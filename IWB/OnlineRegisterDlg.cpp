// OnlineRegisterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IWB.h"
#include "OnlineRegisterDlg.h"

#include "../bitanswer/BitAnswerCpp.h"
#include "../bitanswer/bitanswerError.h"
extern CBitAnswer  g_bitanswer;
// COnlineRegisterDlg dialog


IMPLEMENT_DYNAMIC(COnlineRegisterDlg, CDialog)

COnlineRegisterDlg::COnlineRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COnlineRegisterDlg::IDD, pParent),
    m_bRegisteredOk(FALSE)
    , m_strSN(_T(""))
    ,m_eTouchType(E_DEVICE_PEN_TOUCH)
    ,m_eScreenType(ESingleScreenMode)

{

}

COnlineRegisterDlg::~COnlineRegisterDlg()
{
}

void COnlineRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SN_NUMBER, m_strSN);
    DDX_Text(pDX, IDC_STATIC_ONLINE_REGISTER_INFO, m_strText);
}


BEGIN_MESSAGE_MAP(COnlineRegisterDlg, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_ONLINE_REGISTER, &COnlineRegisterDlg::OnBnClickedButtonOnlineRegister)
END_MESSAGE_MAP()


// COnlineRegisterDlg message handlers

BOOL COnlineRegisterDlg::OnInitDialog()
{
   if(theApp.IsHardwareKeyExist())
   {
       GetDlgItem(IDC_EDIT_SN_NUMBER)->EnableWindow(FALSE);
       GetDlgItem(IDC_BUTTON_ONLINE_REGISTER)->EnableWindow(FALSE);
       m_strText.Format(
           _T("%s\r\n%s:%s\r\n%s:%s"), 
           g_oResStr[IDS_STRING458],//"使用硬件加密狗"信息
           g_oResStr[IDS_STRING459],
           theApp.GetUSBKeyTouchType() == E_DEVICE_PEN_TOUCH? g_oResStr[IDS_STRING460] : g_oResStr[IDS_STRING461],
           g_oResStr[IDS_STRING462],
           theApp.GetScreenType() == EDoubleScreenMode?g_oResStr[IDS_STRING464] :g_oResStr[IDS_STRING463]);
           
   }
   else
   {
        // TODO:  Add extra initialization here
        BIT_STATUS status = g_bitanswer.Login("", BIT_MODE_AUTO);
        if(status == BIT_SUCCESS)
        {  //登录成功后，提取SN显示在对话框中
           
            m_strText = g_oResStr[IDS_STRING455];//"注册成功"信息

            string sessionInfo;
            if (g_bitanswer.GetSessionInfo(XML_TYPE_SN_INFO, sessionInfo) == BIT_SUCCESS)
            {
                CComPtr<IXMLDOMDocument> xmlDoc;  // xml 文档
                CComPtr<IXMLDOMNode> rootNode;    // BitConfig节点

                HRESULT hr = S_OK;

                CoInitialize(NULL); // 初始化COM, 解析XML需要使用
                hr = xmlDoc.CoCreateInstance(CLSID_DOMDocument);
                if (hr == S_OK)
                {
                    VARIANT_BOOL vb;
                    hr = xmlDoc->loadXML(CComBSTR(sessionInfo.c_str()), &vb); //加载XML串
                    if (hr == S_OK)
                    {
                        CComPtr<IXMLDOMElement> rootElement;
                        hr = xmlDoc->get_documentElement(&rootElement); //根节点
                        if (hr == S_OK)
                        {
                            hr = rootElement->selectSingleNode(OLESTR("snInfo/sn"), &rootNode);
                            if(hr == S_OK)
                            {
                                BSTR sn;
                                hr = rootNode->get_text(&sn);
                                if (hr == S_OK)
                                {
                                    m_strSN = CW2T(sn);
                                    SysFreeString(sn);
                                }
                            }
                        }
                        rootElement.Release();
                    }
                }//if(hr == ok)

                CoUninitialize();

            }//if(bitanswer.GetSessionInfo)

            //Read Features
            BIT_UINT32 value;
            BIT_STATUS status = g_bitanswer.ReadFeature(FEATURE_TOUCH_TYPE,&value);
            if(status == BIT_SUCCESS)
            {
                m_eTouchType = (value == 0)?E_DEVICE_PEN_TOUCH:E_DEVICE_FINGER_TOUCH;
            }
            
            status = g_bitanswer.ReadFeature(FEATURE_SCREEN_TYPE,&value);
            if(status == BIT_SUCCESS)
            {
                m_eScreenType = (value == 0)?ESingleScreenMode:EDoubleScreenMode;
            }

           m_strText.Format(
           _T("%s\r\n%s:%s\r\n%s:%s"), 
           g_oResStr[IDS_STRING455],//注册成功
           g_oResStr[IDS_STRING459],
           m_eTouchType == E_DEVICE_PEN_TOUCH? g_oResStr[IDS_STRING460] : g_oResStr[IDS_STRING461],
           g_oResStr[IDS_STRING462],
           m_eScreenType == EDoubleScreenMode?g_oResStr[IDS_STRING464] :g_oResStr[IDS_STRING463]);



        }//if(status == BIT_SUCCESS)
        else
        {
            m_strText = g_oResStr[IDS_STRING456];//"未注册"信息
        }
   }
    CDialog::OnInitDialog();

	//设置对话框的缺省按钮,调用函数SetDefaultButton竟然不管用。
	//发现在对话框编辑器中, 将缺省按钮的tab stop编号设为1，可以
	//起到作用。
	//SetDefaultButton(IDC_BUTTON_ONLINE_REGISTER);
	
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void COnlineRegisterDlg::OnBnClickedButtonOnlineRegister()
{
    // TODO: Add your control notification handler code here
      UpdateData(TRUE);

      LOGIN_MODE mode = BIT_MODE_AUTO_CACHE;
          
      BIT_STATUS status = g_bitanswer.UpdateOnline(CT2CA(m_strSN));
      
      if(status == BIT_SUCCESS)
      {
         status = g_bitanswer.Login(CT2CA(m_strSN), mode);
      }
      if (status != BIT_SUCCESS)
      {
         CString message;
         message.Format(_T("%s, %s"), g_oResStr[IDS_STRING457], getBitanswerErrorText(status));
        m_strText = message;
        UpdateData(FALSE);
      }
      else
      {
         OnOK();
      }
}


void COnlineRegisterDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnCancel();
}


void COnlineRegisterDlg::SetDefaultButton(UINT uID)
{

	UINT prevId = GetDefID();
	if (prevId == uID) return;
	CButton* pPrevButton = reinterpret_cast<CButton*>(GetDlgItem(prevId));
	DWORD style = 0;
	if (pPrevButton)
	{
		// remove default push button style
		// get the style
		DWORD style = pPrevButton->GetStyle();
		style &= ~BS_DEFPUSHBUTTON;
		// set the style
		::SendMessage(pPrevButton->GetSafeHwnd(), BM_SETSTYLE, (WPARAM)style, (LPARAM)TRUE);
		
	}

	CButton* pNewDefaultButton = reinterpret_cast<CButton*>(GetDlgItem(uID));

	// inform the dialog about the new default control id
	SendMessage(DM_SETDEFID, uID);

	// get the style
	style = pNewDefaultButton->GetStyle();
	// add the default push button style
	style |= BS_DEFPUSHBUTTON;
	// set the style
	::SendMessage(pNewDefaultButton->GetSafeHwnd(), BM_SETSTYLE, (WPARAM)style, (LPARAM)TRUE);

	
}

