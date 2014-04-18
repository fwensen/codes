
// clientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "client.h"
#include "clientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CclientDlg �Ի���




CclientDlg::CclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CclientDlg::IDD, pParent)
	, m_editLocalPort(8888)
	, m_serverPort(8000)
	, m_editCode(0)
	, m_editStatus(_T("δ����"))
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


// CclientDlg ��Ϣ�������

BOOL CclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CclientDlg::OnPaint()
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
HCURSOR CclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CclientDlg::OnSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(sock == 0){
		MessageBox("You haven`t connect to server yet","client",MB_OK);
		return;
	}
	UpdateData(TRUE);
	CString msgBuf = _T("��˵: ");
	msgBuf	+= m_sendMsg;  //��Ҫ���͵����� 
	int len = m_sendMsg.GetLength();
	int retval = send(sock,(char*)m_sendMsg.GetBuffer(),len+1,0);
	m_sendMsg.ReleaseBuffer();
	if(retval == SOCKET_ERROR)
		{
			AfxMessageBox(_T("socket�ѹر�!"));
			return;
	
		}
	if(msgBuf != _T(""))
		m_editMSG.AddString(msgBuf);
	UpdateData(FALSE);
}


void CclientDlg::OnConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	sockaddr_in server;
	if( sock == 0)
	{
		sock = socket(AF_INET, SOCK_STREAM,0);
		//AfxMessageBox(_T(""));
	}
	else
	{
		AfxMessageBox(_T("�׽���ʹ���л���δ�ر�"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	closesocket(sock);
	m_editStatus = _T("δ����");
	UpdateData(FALSE);
	sock = 0;
}


LRESULT CclientDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	int Event;
	int len,retval;
	char buf[128];
	CString text;
	CString messageSay = _T("������˵: ");
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
			m_editStatus = _T("������");
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
