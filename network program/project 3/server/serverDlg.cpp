
// serverDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "server.h"
#include "serverDlg.h"
#include "afxdialogex.h"
#include "DialogUser.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CserverDlg 对话框




CserverDlg::CserverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CserverDlg::IDD, pParent)
	, m_sendMsg(_T(""))
	, m_editPort(8000)
	, main_sock(0)
	, m_editUsername(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOWMSG_LIST, m_showMsg);
	DDX_Control(pDX, IDC_SHOWUSERS_LIST, m_showUsers);
	DDX_Text(pDX, IDC_SENDMSG_EDIT, m_sendMsg);

	DDX_Text(pDX, IDC_PORT_EDIT, m_editPort);
	DDX_Text(pDX, IDC_USER_EDIT, m_editUsername);
	//DDX_Control(pDX, IDC_USER_COMBO, m_comboUser);
}

BEGIN_MESSAGE_MAP(CserverDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INITSOCKET_BUTTON, &CserverDlg::OnInitsocket)
	ON_BN_CLICKED(IDC_DISCONNECT_BUTTON, &CserverDlg::OnDisconnect)
	ON_BN_CLICKED(IDC_SENDMSG_BUTTON, &CserverDlg::OnSendmsg)
//	ON_CBN_SELCHANGE(IDC_USER_COMBO, &CserverDlg::OnCbnSelchangeUserCombo)
END_MESSAGE_MAP()

// CserverDlg 消息处理程序

BOOL CserverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CserverDlg::OnPaint()
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
HCURSOR CserverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CserverDlg::OnInitsocket()
{
	// TODO: 在此添加控件通知处理程序代码
	int retval;
	sockaddr_in server;
	//number = 0;
	users = _T("user");
	WSAData wsa;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	WSAStartup(0x101,&wsa);

	if(main_sock != 0){
		MessageBox(_T("服务器已经启动！"));
		return;
	}

	main_sock = socket(AF_INET,SOCK_STREAM,0);
	if(main_sock == INVALID_SOCKET)
		{
			retval = WSAGetLastError();
			AfxMessageBox(_T("socket初始化失败！"));
			return;
		}

	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server.sin_port = htons(m_editPort);

	retval = bind(main_sock,(sockaddr *)&server,sizeof(server));
	if(retval != 0)
	{
		retval = WSAGetLastError();
		AfxMessageBox(_T("Binding faild!"));
		closesocket(main_sock);
        WSACleanup();
		return;
		
	}

	retval = listen(main_sock,5);
	if(retval != 0)
	{
		retval = WSAGetLastError();
		AfxMessageBox(_T("监听失败!"));
		closesocket(main_sock);
		return;

	}
	else
	{
		AfxMessageBox(_T("监听成功!"));
	}

	retval = WSAAsyncSelect(main_sock,m_hWnd,UM_SOCK,FD_ACCEPT);
	//m_start.EnableWindow(FALSE);

}

static int number = 0;       //使用全局变量，便于使用
LRESULT CserverDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	int i;
	INT_PTR nRes;         
	int Event;     //事件
	SOCKET s,ns;
	CString text;    //在list上显示的信息
	sockaddr_in remote;
	CString messageShow;   //收到的信息，有char* 转换为CString
	int retval;
	char buf[128];    //收到数据的缓存
	int len;
	// TODO: Add your specialized code here and/or call the base class
	switch(message){
	case UM_SOCK:
		s = (SOCKET)LOWORD(wParam);
		Event = LOWORD(lParam);
		switch(Event){
		case FD_ACCEPT:
			len = sizeof(remote);
			user[number].u_sock = accept(s,(sockaddr*)&remote,&len);
			ns = user[number].u_sock;
			if(ns == SOCKET_ERROR){
				closesocket(s);
				main_sock = 0;
				break;
			}
			else
			{
				MessageBox(_T("accept a connection"));
				//子对话框，用于给客户端命名
				DialogUser UserDlg;
				nRes = UserDlg.DoModal();
				if (IDOK == nRes)
				{
					UpdateData(TRUE);
					user[number].name = UserDlg.m_UserName;   //
					m_showUsers.AddString(UserDlg.m_UserName);  //在用户列表中显示出来
					m_editUsername = UserDlg.m_UserName;
					number++;
					UpdateData(FALSE);
				}
			}
			WSAAsyncSelect(ns,m_hWnd,UM_SOCK,FD_READ|FD_CLOSE);
		break;
		case FD_READ:
			UpdateData(TRUE);
			len = recv(s,buf,128,0);
			//检测是哪个客户端发来的
			for(i=0;i<N; i++)     //遍历套接字寻找客户名 
			{
				if(user[i].u_sock == s)
					text = user[i].name;
			}
			if(len <= 0){
				retval = WSAGetLastError();
				if( retval != WSAEWOULDBLOCK){
					closesocket(s);
					MessageBox(_T("recv() failed!"));
					break;
				}
				else
					break;
			}
			buf[len] = 0;
			text += _T("说:");
			messageShow.Format(_T("%s"),buf);
			text += messageShow;
			m_showMsg.AddString(text);
		break;
		case FD_CLOSE:
			closesocket(s);
		break;
		}
	break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CserverDlg::OnDisconnect()
{

	if(user[0].u_sock != 0)
		closesocket(user[0].u_sock);
	else
		AfxMessageBox(_T("socket已断开!"));
	return;
	// TODO: 在此添加控件通知处理程序代码
}


void CserverDlg::OnSendmsg()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int i;
	CString userName = m_editUsername;
	//遍历用户名，寻找相应的套接字描述符
	for(i=0;i<N; i++)
	{
		if(user[i].name == userName)
			break;
	}
	if(user[i].u_sock == 0){
		MessageBox("You haven`t connect to server yet","client",MB_OK);
		return;
	}
	
	CString msgBuf = _T("我说: ");
	msgBuf	+= m_sendMsg;  //需要发送的数据 
	int len = m_sendMsg.GetLength();
	int retval = send(user[i].u_sock,(char*)m_sendMsg.GetBuffer(),len+1,0);
	m_sendMsg.ReleaseBuffer();
	if(retval == SOCKET_ERROR)
		{
			AfxMessageBox(_T("socket已关闭!"));
			return;
	
		}
	if(msgBuf != _T(""))
		m_showMsg.AddString(msgBuf);
	UpdateData(FALSE);



}

