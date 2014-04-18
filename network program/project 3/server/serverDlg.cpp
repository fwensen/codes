
// serverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "server.h"
#include "serverDlg.h"
#include "afxdialogex.h"
#include "DialogUser.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CserverDlg �Ի���




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

// CserverDlg ��Ϣ�������

BOOL CserverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CserverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CserverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CserverDlg::OnInitsocket()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int retval;
	sockaddr_in server;
	//number = 0;
	users = _T("user");
	WSAData wsa;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	WSAStartup(0x101,&wsa);

	if(main_sock != 0){
		MessageBox(_T("�������Ѿ�������"));
		return;
	}

	main_sock = socket(AF_INET,SOCK_STREAM,0);
	if(main_sock == INVALID_SOCKET)
		{
			retval = WSAGetLastError();
			AfxMessageBox(_T("socket��ʼ��ʧ�ܣ�"));
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
		AfxMessageBox(_T("����ʧ��!"));
		closesocket(main_sock);
		return;

	}
	else
	{
		AfxMessageBox(_T("�����ɹ�!"));
	}

	retval = WSAAsyncSelect(main_sock,m_hWnd,UM_SOCK,FD_ACCEPT);
	//m_start.EnableWindow(FALSE);

}

static int number = 0;       //ʹ��ȫ�ֱ���������ʹ��
LRESULT CserverDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	int i;
	INT_PTR nRes;         
	int Event;     //�¼�
	SOCKET s,ns;
	CString text;    //��list����ʾ����Ϣ
	sockaddr_in remote;
	CString messageShow;   //�յ�����Ϣ����char* ת��ΪCString
	int retval;
	char buf[128];    //�յ����ݵĻ���
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
				//�ӶԻ������ڸ��ͻ�������
				DialogUser UserDlg;
				nRes = UserDlg.DoModal();
				if (IDOK == nRes)
				{
					UpdateData(TRUE);
					user[number].name = UserDlg.m_UserName;   //
					m_showUsers.AddString(UserDlg.m_UserName);  //���û��б�����ʾ����
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
			//������ĸ��ͻ��˷�����
			for(i=0;i<N; i++)     //�����׽���Ѱ�ҿͻ��� 
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
			text += _T("˵:");
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
		AfxMessageBox(_T("socket�ѶϿ�!"));
	return;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CserverDlg::OnSendmsg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int i;
	CString userName = m_editUsername;
	//�����û�����Ѱ����Ӧ���׽���������
	for(i=0;i<N; i++)
	{
		if(user[i].name == userName)
			break;
	}
	if(user[i].u_sock == 0){
		MessageBox("You haven`t connect to server yet","client",MB_OK);
		return;
	}
	
	CString msgBuf = _T("��˵: ");
	msgBuf	+= m_sendMsg;  //��Ҫ���͵����� 
	int len = m_sendMsg.GetLength();
	int retval = send(user[i].u_sock,(char*)m_sendMsg.GetBuffer(),len+1,0);
	m_sendMsg.ReleaseBuffer();
	if(retval == SOCKET_ERROR)
		{
			AfxMessageBox(_T("socket�ѹر�!"));
			return;
	
		}
	if(msgBuf != _T(""))
		m_showMsg.AddString(msgBuf);
	UpdateData(FALSE);



}

