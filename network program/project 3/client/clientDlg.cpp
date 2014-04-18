
// clientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "client.h"
#include "clientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CclientDlg 对话框




CclientDlg::CclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CclientDlg::IDD, pParent)
	, m_editLocalPort(8888)
	, m_serverPort(8000)
	, m_editCode(0)
	, m_editStatus(_T("未连接"))
	, sock(0)
	, m_serverIP()
	, m_pserverIP(INADDR_LOOPBACK)
	, m_sendMsg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOCALPORT_EDIT, m_editLocalPort);
	DDX_Text(pDX, IDC_SERVERPORT_EDIT, m_serverPort);
	DDX_Text(pDX, IDC_CODE_EDIT, m_editCode);
	DDX_Control(pDX, IDC_IPADDRESS, m_serverIP);
	DDX_Text(pDX, IDC_STATUS_EDIT, m_editStatus);
	DDX_Control(pDX, IDC_MSG_LIST, m_editMSG);
	DDX_IPAddress(pDX, IDC_IPADDRESS, m_pserverIP);
	DDX_Text(pDX, IDC_SENDMSG_EDIT, m_sendMsg);
}

BEGIN_MESSAGE_MAP(CclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND_BUTTON, &CclientDlg::OnSend)
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, &CclientDlg::OnConnect)
	ON_BN_CLICKED(IDC_DISCONNECT_BUTTON, &CclientDlg::OnDisconnect)
END_MESSAGE_MAP()


// CclientDlg 消息处理程序

BOOL CclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CclientDlg::OnSend()
{
	// TODO: 在此添加控件通知处理程序代码
	if(sock == 0){
		MessageBox("You haven`t connect to server yet","client",MB_OK);
		return;
	}
	UpdateData(TRUE);
	CString msgBuf = _T("我说: ");
	msgBuf	+= m_sendMsg;  //需要发送的数据 
	int len = m_sendMsg.GetLength();
	int retval = send(sock,(char*)m_sendMsg.GetBuffer(),len+1,0);
	m_sendMsg.ReleaseBuffer();
	if(retval == SOCKET_ERROR)
		{
			AfxMessageBox(_T("socket已关闭!"));
			return;
	
		}
	if(msgBuf != _T(""))
		m_editMSG.AddString(msgBuf);
	UpdateData(FALSE);
}


void CclientDlg::OnConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	sockaddr_in server;
	if( sock == 0)
	{
		sock = socket(AF_INET, SOCK_STREAM,0);
		//AfxMessageBox(_T(""));
	}
	else
	{
		AfxMessageBox(_T("套接字使用中或是未关闭"));
		return;
	}
	UpdateData(TRUE);
	server.sin_family = AF_INET;
	m_serverIP.GetAddress(server.sin_addr.S_un.S_un_b.s_b1,server.sin_addr.S_un.S_un_b.s_b2,server.sin_addr.S_un.S_un_b.s_b3, server.sin_addr.S_un.S_un_b.s_b4);
	server.sin_port = htons(m_serverPort);

	WSAAsyncSelect(sock,m_hWnd,UM_SOCK,FD_CLOSE|FD_CONNECT);
	connect(sock,(sockaddr*)&server,sizeof(server));

}


void CclientDlg::OnDisconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	closesocket(sock);
	m_editStatus = _T("未连接");
	UpdateData(FALSE);
	sock = 0;
}


LRESULT CclientDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	int Event;
	int len,retval;
	char buf[128];
	CString text;
	CString messageSay = _T("服务器说: ");
	switch(message)
	{
		case UM_SOCK:
		Event = LOWORD(lParam);
		switch(Event){
		case FD_CONNECT:
			MessageBox("try to connect to server!","client",MB_OK);
			WSAAsyncSelect(sock,m_hWnd,UM_SOCK,FD_WRITE|FD_READ|FD_CLOSE);
		break;
		case FD_WRITE:
			MessageBox("connect to server!","client",MB_OK);
			m_editStatus = _T("已连接");
			UpdateData(FALSE);
			WSAAsyncSelect(sock,m_hWnd,UM_SOCK,FD_READ|FD_CLOSE);
		break;
		case FD_READ:
			len = recv(sock,buf,128,0);
			if(len <= 0){
				retval = WSAGetLastError();
				if(retval != WSAEWOULDBLOCK){
					closesocket(sock);
					sock = 0;
				}
				break;
			}
			buf[len] = 0;
			text.Format(_T("%s"),buf);
			messageSay += text;
			m_editMSG.AddString((LPCTSTR)messageSay);
			//m_ack = buf;
			UpdateData(FALSE);
		break;
		case FD_CLOSE:
			closesocket(sock);
		break;
		}
	break;
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}
