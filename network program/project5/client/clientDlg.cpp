
// clientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "client.h"
#include "clientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CclientDlg �Ի���




CclientDlg::CclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CclientDlg::IDD, pParent)
	, m_editPort(8000)
	, m_editIP(INADDR_LOOPBACK)
	, m_editUsername(_T(""))
	, m_editDownld(_T(""))
	, m_editUpld(_T(""))
	, m_editSendmsg(_T(""))
	, m_editPeerPort(0)
	, m_editPeerIP(INADDR_LOOPBACK)
	, m_editPeerUpld(_T(""))
	, m_editPeerSendmsg(_T(""))
	, m_editMsgTo(_T(""))
	, m_editPeerMsgTo(_T(""))
	, main_sock(0)
	, conn_sock(0)
	, m_editLocalPort(0)
	, m_editPasswd(0)
	, peer_cli_sock(0)
	, m_directPort(0)
	, m_directIP(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PORT_EDIT, m_editPort);
	DDX_IPAddress(pDX, IDC_IPADDRESS, m_editIP);
	DDX_Text(pDX, IDC_USERNAME_EDIT, m_editUsername);
	DDX_Text(pDX, IDC_DOWNLOAD_EDIT, m_editDownld);
	DDX_Text(pDX, IDC_UPLOAD_EDIT, m_editUpld);
	DDX_Text(pDX, IDC_SENDMSG_EDIT, m_editSendmsg);
	DDX_Control(pDX, IDC_MSG_LIST, m_ListMsg);
	DDX_Text(pDX, IDC_PEERPORT_EDIT, m_editPeerPort);
	DDX_IPAddress(pDX, IDC_PEER_IPADDRESS, m_editPeerIP);
	DDX_Text(pDX, IDC_PEERUPLOAD_EDIT, m_editPeerUpld);
	DDX_Control(pDX, IDC_PEERMSG_LIST, m_ListPeerMsg);
	DDX_Text(pDX, IDC_PEERSENDMSG_EDIT, m_editPeerSendmsg);
	DDX_Control(pDX, IDC_CONNECT_BUTTON, m_buttonConn);
	DDX_Control(pDX, IDC_DISCONNECT_BUTTON, m_buttonDisconn);
	DDX_Control(pDX, IDC_DOWNLOAD_BUTTON, m_buttonDowonld);
	DDX_Control(pDX, IDC_UPLOAD_BUTTON, m_buttonUpld);
	//DDX_Control(pDX, IDC_LOGIN_BUTTON, m_buttonLogin);
	DDX_Control(pDX, IDC_SENDMSG_BUTTON, m_buttonSendmsg);
	DDX_Control(pDX, IDC_PEERCONNECT_BUTTON, m_buttonPeerConn);
	DDX_Control(pDX, IDC_PEERDISCONNECT_BUTTON, m_buttonPeerDisconn);
	DDX_Control(pDX, IDC_PEERSENDMSG_BUTTON, m_buttonPeerSendmsg);
	DDX_Control(pDX, IDC_REFRESH_BUTTON, m_buttonRefresh);
	DDX_Control(pDX, IDC_PEERUPLOAD_BUTTON, m_buttonPeerUpld);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	//DDX_Text(pDX, IDC_USERS_EDIT, m_editUsers);
	DDX_Text(pDX, IDC_MSGTO_EDIT, m_editMsgTo);
	DDX_Text(pDX, IDC_PEERMSGTO_EDIT, m_editPeerMsgTo);
	DDX_Control(pDX, IDC_IPADDRESS, m_CtrlIP);
	DDX_Control(pDX, IDC_PEER_IPADDRESS, m_CtrlPeerIP);
	DDX_Text(pDX, IDC_LOCALPORT_EDIT, m_editLocalPort);
	DDX_Control(pDX, IDC_INIT_BUTTON, m_buttonInit);
	DDX_Text(pDX, IDC_USERPASSWD_EDIT, m_editPasswd);
	DDX_Control(pDX, IDC_USERS_LIST, m_ListOnlineUsers);
	DDX_Text(pDX, IDC_DIRECTPORT_EDIT, m_directPort);
	DDX_Text(pDX, IDC_DIRECTIP_EDIT, m_directIP);
}

BEGIN_MESSAGE_MAP(CclientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, &CclientDlg::OnConnect)
	ON_BN_CLICKED(IDC_DISCONNECT_BUTTON, &CclientDlg::OnDisconnect)
	//ON_BN_CLICKED(IDC_LOGIN_BUTTON, &CclientDlg::OnLogin)
	ON_BN_CLICKED(IDC_DOWNLOAD_BUTTON, &CclientDlg::OnDownload)
	ON_BN_CLICKED(IDC_UPLOAD_BUTTON, &CclientDlg::OnUpload)
	ON_BN_CLICKED(IDC_SENDMSG_BUTTON, &CclientDlg::OnSendmsg)
	ON_BN_CLICKED(IDC_PEERCONNECT_BUTTON, &CclientDlg::OnPeerconnect)
	ON_BN_CLICKED(IDC_PEERDISCONNECT_BUTTON, &CclientDlg::OnPeerdisconnect)
	ON_BN_CLICKED(IDC_PEERUPLOAD_BUTTON, &CclientDlg::OnPeerupload)
	ON_BN_CLICKED(IDC_PEERSENDMSG_BUTTON, &CclientDlg::OnPeersendmsg)
	ON_BN_CLICKED(IDC_REFRESH_BUTTON, &CclientDlg::OnRefresh)
	ON_BN_CLICKED(IDC_INIT_BUTTON, &CclientDlg::OnInit)
END_MESSAGE_MAP()


// CclientDlg ��Ϣ�������

BOOL CclientDlg::OnInitDialog()
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


//������ӷ�����
void CclientDlg::OnConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int retval;
	tmp_buf = NULL;
	file_len = 0;
	total = 0;
	sockaddr_in client;
	sockaddr_in server;
	if( conn_sock == 0)
	{
		conn_sock = socket(AF_INET, SOCK_STREAM,0);
		//AfxMessageBox(_T(""));
	}
	else
	{
		AfxMessageBox(_T("�׽���ʹ���л���δ�ر�"));
		return;
	}
	UpdateData(TRUE);
	
	server.sin_family = AF_INET;
	m_CtrlIP.GetAddress(server.sin_addr.S_un.S_un_b.s_b1,server.sin_addr.S_un.S_un_b.s_b2,server.sin_addr.S_un.S_un_b.s_b3, server.sin_addr.S_un.S_un_b.s_b4);
	server.sin_port = htons(m_editPort);
	WSAAsyncSelect(conn_sock, m_hWnd, UM_SOCK, FD_CONNECT|FD_READ|FD_CLOSE);
	retval = connect(conn_sock,(sockaddr*)&server,sizeof(server));
	
}


//��ʼ���ͻ��˵ķ�����
void CclientDlg::OnInit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SOCKADDR_IN localserver;
	UpdateData(TRUE);
	WSAData wsa;
	WSAStartup(MAKEWORD(1, 1), &wsa);
	main_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(main_sock == INVALID_SOCKET)
	{
		closesocket(main_sock);
		AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
		return;
	}

	
	localserver.sin_family = AF_INET;
	localserver.sin_port = htons(m_editLocalPort);
	localserver.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if(bind(main_sock, (SOCKADDR*)&localserver, sizeof(localserver)) != 0)
		{
			closesocket(main_sock);
			AfxMessageBox(_T("��ʧ��!"));
			return;
		}

	if(listen(main_sock, 3) != 0)
	{
		closesocket(main_sock);
		AfxMessageBox(_T("����ʧ��!"));
		return;
	}

	WSAAsyncSelect(main_sock, m_hWnd, UM_SOCK, FD_ACCEPT|FD_CLOSE);

}


//�Ͽ�����������
void CclientDlg::OnDisconnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	struct message mesg_send;
	mesg_send.type = USER_CLOSE;
	int retval;
	retval = send(conn_sock, (char*)&mesg_send, 4, 0);
	if(retval == SOCKET_ERROR)
	{
		AfxMessageBox(_T("���͹ر���Ϣʧ��!"));
		
	}
	closesocket(conn_sock);
	conn_sock = 0;
	return;

}


//������������ļ�
void CclientDlg::OnDownload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int retval;
	int length;
	UpdateData(TRUE);
	struct message file_send;
	file_send.type = DOWNLOAD;
	file_send.fileFlag = FILE_INFO;
	CString file_name = m_editDownld;
	file_info file_send_info;

	sprintf(file_send_info.filename, "%s", file_name.GetBuffer(0));
	file_send_info.length = 0;
	memcpy(file_send.mesg, &file_send_info, sizeof(file_send_info));
	length = 25 + sizeof(file_send.mesg);

	retval = send(conn_sock, (char*)&file_send, length, 0);
	if(retval == SOCKET_ERROR)
	{
		AfxMessageBox(_T("���������ļ���Ϣʧ��!"));
		return;
	}
	return;
}


CString current_file_path;      //��ǰ�ļ�·��
unsigned long long current_file_length;  //��ǰ�ļ���С
CString current_file_name ;		//��ǰ�ļ���

//��������ϴ��ļ�   
void CclientDlg::OnUpload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	struct message file_send;
	file_send.type = UPLOAD;
	int retval;
	//׼�������ļ���Ϣ
	
		file_info file_send_info;
		file_send.fileFlag = FILE_INFO;
		UpdateData(TRUE);
		int pos=0, start=0;  
		TCHAR szFilter[] = _T("�ı��ļ�(*.txt)|*.txt|�����ļ�(*.*)|*.*||");   
		// ������ļ��Ի���   
		CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);     
		// ��ʾ���ļ��Ի���   
		if (IDOK == fileDlg.DoModal())   
		{   
			// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����   
			current_file_path = fileDlg.GetPathName();   
			SetDlgItemText(IDC_UPLOAD_EDIT, current_file_path);   
		}

		if(current_file_path == "")
		{
			AfxMessageBox(_T("δѡ���κ��ļ���"));
			return;
		}

		CFile file;
		if(file.Open(current_file_path, CFile::modeRead))
		{
			 file_send_info.length =  file.GetLength();
			 current_file_length = file.GetLength();
		}
		file.Close();

		//��ȡ�ļ���
		while(1)  
		{  
			start = pos;  
			pos=current_file_path.Find('\\',start+1);  
			if(pos<0)  
				break;  
		}  
	
		current_file_name = current_file_path.Right(current_file_path.GetLength()-start-1);
		sprintf(file_send_info.filename, "%s", current_file_name.GetBuffer(0));
		memcpy(file_send.mesg, &file_send_info, sizeof(file_send_info));
		int length = 25 + sizeof(file_send_info);
		retval = send(conn_sock, (char*)&file_send, length, 0);
		{
			if(retval == SOCKET_ERROR)
			{	
				AfxMessageBox(_T("�����ϴ��ļ���Ϣʧ��!"));
				return;
			}
		}
		
		return;

}
		
//������Ϣ
void CclientDlg::OnSendmsg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int retval;
	int length;
	struct message mesg_send;
	UpdateData(TRUE);
	CString sign;
	sign = m_editMsgTo;
	CString message_send = m_editSendmsg;


	if(sign == _T("public") || sign == _T(""))
	{
		
		mesg_send.type = PUBLIC_MESG;
		

	}
	else
	{
		mesg_send.type = PRIVATE_MESG;
		sprintf(mesg_send.toWho, "%s", sign.GetBuffer(0));

	}
	
	sprintf(mesg_send.fromWho, "%s", m_editUsername.GetBuffer(0));
	sprintf(mesg_send.mesg, "%s", message_send.GetBuffer(0));
	length = 25 + sizeof(message_send.GetBuffer(0));
	retval = send(conn_sock, (char*)&mesg_send, length, 0);
	if(retval == SOCKET_ERROR)
	{
		AfxMessageBox(_T("������Ϣʧ�ܣ�"));
		return;
	}

	m_ListMsg.AddString(message_send);
	return;

}

//����ֱ�ӺͶԶ����ӣ����辭��������
void CclientDlg::OnPeerconnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int retval;
	sockaddr_in server;
	if( peer_cli_sock == 0)
	{
		peer_cli_sock = socket(AF_INET, SOCK_STREAM,0);
		//AfxMessageBox(_T(""));
	}
	else
	{
		AfxMessageBox(_T("�׽���ʹ���л���δ�ر�"));
		return;
	}
	UpdateData(TRUE);

	server.sin_family = AF_INET;
	m_CtrlPeerIP.GetAddress(server.sin_addr.S_un.S_un_b.s_b1,server.sin_addr.S_un.S_un_b.s_b2,server.sin_addr.S_un.S_un_b.s_b3, server.sin_addr.S_un.S_un_b.s_b4);
	server.sin_port = htons(m_editPeerPort);
	WSAAsyncSelect(peer_cli_sock, m_hWnd, UM_SOCK, FD_CONNECT|FD_READ|FD_CLOSE);
	retval = connect(peer_cli_sock,(sockaddr*)&server,sizeof(server));

}


//�Ͽ���Զ˵�����
void CclientDlg::OnPeerdisconnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	closesocket(peer_cli_sock);
	peer_cli_sock = 0;
	return;
}

//�����ļ���ֱ�ӶԶˣ�������������
void CclientDlg::OnPeerupload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//������Ϣ��ֱ�ӶԶˣ�������������
void CclientDlg::OnPeersendmsg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int length;
	int retval;
	CString username = m_editUsername;
	struct message mesg_send;
	mesg_send.type = DIREC_MESG;
	sprintf(mesg_send.mesg, "%s", m_editPeerSendmsg.GetBuffer(0));
	sprintf(mesg_send.fromWho, "%s", username.GetBuffer(0));
	length = 25 + m_editPeerSendmsg.GetLength();
	retval = send(peer_cli_sock, (char*)&mesg_send, length, 0);
	if(retval == SOCKET_ERROR)
	{
		AfxMessageBox(_T("������Ϣʧ��!"));
		return;
	}

}


//ˢ�������û�
void CclientDlg::OnRefresh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int retval;
	struct message mesg_send;
	mesg_send.type = ONLINEUSERS;
	retval = send(conn_sock, (char*)&mesg_send, 3, 0);
	if(retval == SOCKET_ERROR)
	{
		AfxMessageBox(_T("��ѯ������Աʧ��!"));
		return;
	}


}

static int num = 0; //����������ֱ����������û�
LRESULT CclientDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

	SOCKET s, ns;
	sockaddr_in remote;
	int retval;
	int length;      //ÿ�η��͵ĳ���
	char buf[1024];
	int len;
	int Event;
	int result;     //������packet�еõ��Ľ��
	packet_i packet_recv;  //�յ���packet��
	struct message mesge_recv, mesge_send;
	file_info file_dtl;  
	struct USER_CONFIRM confirm;   //���ͱ��ͻ��˵��û����͵õ�����֤���
	switch(message)
	{
	case UM_SOCK:
		s = (SOCKET)LOWORD(wParam);
		Event = LOWORD(lParam);
		switch(Event)
		{

		case FD_CONNECT:
			MessageBox(_T("try to connect to server!"));
			WSAAsyncSelect(conn_sock,m_hWnd,UM_SOCK,FD_WRITE|FD_READ|FD_CLOSE);
			break;
		case FD_WRITE:
			MessageBox(_T("connect to server!"));
			UpdateData(FALSE);
			next_state = STATE_RECVAUTH;
			WSAAsyncSelect(conn_sock, m_hWnd, UM_SOCK, FD_READ|FD_CLOSE);
			break;

		case FD_ACCEPT:
			len = sizeof(remote);
			ns = accept(s,(sockaddr*)&remote,&len);
			if(ns == SOCKET_ERROR){
				closesocket(s);
				conn_sock = 0;
				break;
			}
			if(num == N)
			{
				AfxMessageBox(_T("�����û�����!"));
				return 0; 
			}
			else
			{
				direct_users[num].s = ns;
				++num;
				MessageBox(_T("accept a connection"));
				WSAAsyncSelect(ns,m_hWnd,UM_SOCK,FD_READ|FD_CLOSE);
			}
			m_directPort = htons(remote.sin_port);
			sprintf(char_directIP, "%s", inet_ntoa(remote.sin_addr));
			m_directIP.Format(_T("%s"), char_directIP);
			UpdateData(FALSE);
		break;

		case FD_READ:
			len = recv(s,buf,1024,0);
			if(len <= 0){
				retval = WSAGetLastError();
				if( retval != WSAEWOULDBLOCK)
				{
					closesocket(s);
					AfxMessageBox(_T("����ʧ��!"));
					next_state = STATE_FAIL;
					break;
				}
				else
				{
					next_state = STATE_FAIL;
					break;
				}
			}

			memcpy(&mesge_recv,buf,1024);     //���յ������ݸ��ƹ���
			switch(mesge_recv.type)
			{

			//�õ�����ѯ������Ա
			case ONLINEUSERS:
				retval = refresh_online_users(mesge_recv);
				if(retval != 0)
					AfxMessageBox(_T("ˢ��ʧ��!"));

			break;

			case DIREC_MESG:
				retval = handle_direct_msg(s, mesge_recv);
				
			break;

			case AUTH_CHICK:
				memcpy(&packet_recv, mesge_recv.mesg, sizeof(packet_recv));
				UpdateData(TRUE);
				result = auth_check_result(packet_recv);
				sprintf(confirm.name, "%s", m_editUsername);
				confirm.result = result;
				mesge_send.type = AUTH_CHICK;
				memcpy(mesge_send.mesg, &confirm, sizeof(confirm));
				length = 22 + sizeof(confirm) + 1;
				len = send(s, (char*)&mesge_send, length, 0);
				if(len == SOCKET_ERROR)
				{
					AfxMessageBox(_T("������֤ʧ��!"));
					break;
				}


			break;

			case AUTH_OK:
				AfxMessageBox(_T("��֤�ɹ�!"));


			break;

			case AUTH_FAIL:
				AfxMessageBox(_T("��֤ʧ��,�Ͽ�����������!"));

			break;

			//��������Ϣ
			case PUBLIC_MESG:
				message_recv_show(mesge_recv.type, mesge_recv);

			break;

			//˽��
			case PRIVATE_MESG:
				message_recv_show(mesge_recv.type, mesge_recv);

			break;

			case DIREC_UPLOAD:
				
				
			break;
				
			case DOWNLOAD:

				switch(mesge_recv.fileFlag)
				{
				case FILE_DOTEX:
					AfxMessageBox(_T("�ļ�������!"));
				break;
				
				case FILE_INFO:
					memcpy(&file_dtl, mesge_recv.mesg, sizeof(file_dtl));
					file_len = file_dtl.length;
					size_file_left = file_dtl.length;
					CfileName.Format(_T("%s"), file_dtl.filename);
					tmp_file.Open(_T("G:\\") + CfileName, CFile::modeCreate | CFile::modeWrite);
					mesge_send.type = DOWNLOAD;
					mesge_send.fileFlag = FILE_RECV;
					retval = send(s, (char*)&mesge_send, 4, 0);
					if(retval == SOCKET_ERROR)
					{
						AfxMessageBox(_T("�������ؿ�ʼ����ʧ��!"));
					}

				break;

				//�����ļ�ʱ�� ��ʼ�����ļ�
				case FILE_RECV:
					retval = download_file(mesge_recv);

				break;

				}

			break;

			case UPLOAD:
				switch(mesge_recv.fileFlag)
				{

				case FILE_SEND:
					retval = upload_file();
				break;

				default:
				break;

				}


			break;

			}

		break;

		case FD_CLOSE:
			
			AfxMessageBox(_T("�������ѹر�!"));
		break;

		}  //Event ����

	break;
	}      //message ����




	return CDialogEx::WindowProc(message, wParam, lParam);
}


int CclientDlg::handle_direct_msg(SOCKET s, struct message mesg_recv)
{

	int i;
	UpdateData(TRUE);
	CString username;
	CString msg;
	username.Format(_T("%s"), mesg_recv.fromWho);
	m_editPeerMsgTo = username;
	for(i=0; i<N; i++)
	{
		if(direct_users[i].s == s)
		{
			sprintf(direct_users[i].name, "%s", mesg_recv.fromWho);

		}
	}
	msg.Format(_T(" says: %s"), mesg_recv.mesg);
	username += msg;
	m_ListPeerMsg.AddString(username);
	UpdateData(FALSE);
	return 0;

}


int CclientDlg::refresh_online_users(struct message meg_recv)
{
	int i;
	memcpy(all_users, meg_recv.mesg, sizeof(all_users));
	CString name;
	CString ip;
	CString port;
	m_ListOnlineUsers.ResetContent();

	for(i=0; i<N; i++)
	{
		if(all_users[i].isOnline == ONLINE)
		{
			name.Format(_T("%s (IP: "), all_users[i].name);
			ip.Format(_T("%s  Port: "), all_users[i].ip);
			port.Format(_T("%d)"), all_users[i].port);
			ip += port;
			name += ip;
			m_ListOnlineUsers.AddString(name);
		}
	}

	return 0;

}


//���ڼ�����֤��Ϣ  packet��
int CclientDlg::auth_check_result(packet_i packet)
{

	UpdateData(TRUE);
	int len2 = packet.len_2bytes;
	int len4 = packet.len_4bytes;
	int result = 0, i;
	for(i=0; i<len2; i++)
	{
		result += packet.two_bytes[i];
		
	}

	for(i=0; i<len4; i++)
		result += packet.four_bytes[i];

	result ^= m_editPasswd;
	return result;

}

//    ������������˽�ź͹����������ʾ
void CclientDlg::message_recv_show(int type, struct message mesg_recv)
{

	UpdateData(TRUE);

	CString msg;
	msg.Format(_T("%s"), mesg_recv.mesg);

	CString private_msg_show = _T("˽��-");
	CString public_msg_show ;
	CString user_from;
	user_from.Format(_T("%s"), mesg_recv.fromWho);
	public_msg_show += user_from;
	public_msg_show += _T(": ");
	public_msg_show += msg;

	private_msg_show += user_from;
	private_msg_show += _T(": ");
	private_msg_show += msg;
	switch(type)
	{
	case PUBLIC_MESG:
		m_ListMsg.AddString(public_msg_show);
		
	break;

	case PRIVATE_MESG:
		m_ListMsg.AddString(private_msg_show);

	break;

	default:

	break;

	}


	return;

}

//�ϴ��ļ�
int CclientDlg::upload_file()
{

	struct message msg_send;     //�����͵���Ϣ��
	int retval;
	//�ж�tmp_buf �Ƿ�Ϊ��
	if(tmp_buf != NULL)
	{
		delete [] tmp_buf;
		tmp_buf = NULL;
	}

	tmp_buf = new char[current_file_length];
	tmp_file.Open(current_file_path, CFile::modeRead);
	tmp_file.Read(tmp_buf, current_file_length);
	tmp_file.Close();
	unsigned long long pos;
	unsigned long long left_size = current_file_length;
	for(pos=0; pos<current_file_length; pos += MAX_LENGTH)
	{
		msg_send.type = UPLOAD;
		msg_send.fileFlag = FILE_SEND;
		if(left_size <= MAX_LENGTH)
		{
			char *buf = NULL;
			buf = new char[left_size];
			memcpy(buf, tmp_buf+pos, left_size);
			sprintf(msg_send.mesg, "%s", buf);
			delete buf;
			buf = NULL;
			left_size -= left_size;
			retval = send(conn_sock, (char*)&msg_send, left_size + 23, 0);
		}
		else
		{
			char buf[MAX_LENGTH];
			memcpy(buf, tmp_buf+pos, MAX_LENGTH);
			sprintf(msg_send.mesg, "%s", buf);
			left_size -= MAX_LENGTH;
			retval = send(conn_sock, (char*)&msg_send, MAX_LENGTH + 23, 0);
		}

		
		if(retval == SOCKET_ERROR)
		{
			AfxMessageBox(_T("�ļ�����ʧ��!"));
			delete tmp_buf;
			tmp_buf = NULL;
			return 0;
		}


		if(left_size == 0)
		{
			AfxMessageBox(_T("�ļ��������!"));
			delete tmp_buf;
			tmp_buf = NULL;
			return 1;
		}

		Sleep(10);
	}

	return 0;

}

//�����ļ�
int CclientDlg::download_file(struct message mesge_recv)
{
	if(tmp_buf  != NULL)  
	{  
		delete [] tmp_buf;  
		tmp_buf = NULL;  
	}  

	if(size_file_left <= MAX_LENGTH)
	{
		tmp_buf = new char[size_file_left];
		memcpy(tmp_buf, mesge_recv.mesg, size_file_left);
		tmp_file.Write(tmp_buf, size_file_left);
		delete tmp_buf;
		tmp_buf = NULL;
		total += size_file_left;
		size_file_left -= size_file_left;
	}

	else
	{
		tmp_buf = new char[MAX_LENGTH];     //����һ�����ݿռ�
		memcpy(tmp_buf, mesge_recv.mesg, MAX_LENGTH);
		tmp_file.Write(tmp_buf, MAX_LENGTH);
		delete [] tmp_buf;
		tmp_buf = NULL;
		total += MAX_LENGTH;
		size_file_left -= MAX_LENGTH;
	}


	if(total >= file_len)
	{
		tmp_file.Close();
		AfxMessageBox(_T("�����ļ��ɹ�!"));
		
	}

	return 0;
}