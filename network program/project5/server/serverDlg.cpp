
// serverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "server.h"
#include "serverDlg.h"
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


// CserverDlg �Ի���




CserverDlg::CserverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CserverDlg::IDD, pParent)
	, m_editPort(8000)
	, main_sock(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PORT_EDIT, m_editPort);
	DDX_Control(pDX, IDC_INIT_BUTTON, m_buttonInit);
	DDX_Control(pDX, IDC_DISCONNECT_BUTTON, m_buttonDisconnect);
}

BEGIN_MESSAGE_MAP(CserverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INIT_BUTTON, &CserverDlg::OnInitButton)
	ON_BN_CLICKED(IDC_DISCONNECT_BUTTON, &CserverDlg::OnDisconnectButton)
END_MESSAGE_MAP()


// CserverDlg ��Ϣ�������

BOOL CserverDlg::OnInitDialog()
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

void CserverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


void CserverDlg::OnInitButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int retval;
	sockaddr_in server;
	int i;
	tmp_buf = NULL;
	//��ʼ��
	for(i=0; i<N; i++)
	{
		users[i].passwd = 123456;
		users[i].sock = 0;
		users[i].isOnline = 1;
		users[i].recv_length = 0;
		users[i].recv_total = 0;
		users[i].send_length = 0;
		users[i].send_total = 0;
		sprintf(users[i].name, "user%d", i);
		tmp_result[i].result = 0;
		tmp_result[i].sock = 0;
		tmp_result[i].state = -1;

	}
	

	WSAData wsa;
	UpdateData(TRUE);
	WSAStartup(0x101,&wsa);

	if(main_sock != 0){
		MessageBox(_T("�������Ѿ�������"));
		return;
	}

	//AfxMessageBox(_T("hello"));
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
	next_state = STATE_FREE;
	retval = WSAAsyncSelect(main_sock, m_hWnd, UM_SOCK, FD_ACCEPT);
	m_buttonInit.EnableWindow(FALSE);
	m_buttonDisconnect.EnableWindow(TRUE);
}


void CserverDlg::OnDisconnectButton()
{
	// TODO: 

	closesocket(main_sock);
	m_buttonInit.EnableWindow(TRUE);
	m_buttonDisconnect.EnableWindow(FALSE);

}



static int number = 0;


//�û�������ͨ��Ϣ   PUBLIC or PRIVATE
int CserverDlg::send_commen_message(int type,  struct message mesg_recv)
{
	int i;
	int retval;
	int length;
	struct message mesg_send_;
	switch(type){

	case PUBLIC_MESG:
		for(i=0; i<N; i++)
			if(users[i].isOnline == ONLINE)
			{
				mesg_send_.type = PUBLIC_MESG;
				sprintf(mesg_send_.fromWho, "%s", mesg_recv.fromWho);
				sprintf(mesg_send_.mesg, "%s", mesg_recv.mesg);
			
				//ȡ�÷��ͳ���
				length = 22 + strlen(mesg_recv.mesg) + 1;
				retval = send(users[i].sock, (char*)&mesg_send_, length, 0);
				if(retval == SOCKET_ERROR)
				{
					AfxMessageBox(_T("����ʧ��!"));
					users[i].isOnline = OFFLINE;
					continue;

				}
			}
	break;

	case PRIVATE_MESG:
		for(i=0; i<N; i++)
			//if(mesg_recv.toWho == users[i].name)
			if(strstr(mesg_recv.toWho, users[i].name) != NULL)
			{
				
				if( !(users[i].isOnline == ONLINE))
					AfxMessageBox(_T("���û�������!"));

				mesg_send_.type = PRIVATE_MESG;
				sprintf(mesg_send_.fromWho, "%s", mesg_recv.fromWho);
				sprintf(mesg_send_.mesg, mesg_recv.mesg);

				//ȡ�÷��ͳ���
		    	length =  22 + strlen(mesg_recv.mesg) + 1 ;
				retval = send(users[i].sock, (char*)&mesg_send_, length, 0);
				if(retval == SOCKET_ERROR)
				{
					AfxMessageBox(_T("����ʧ��!"));
					return 0;
					break;
				}
				break;
			}
		
	break;

	default:
	
	break;

	}

	
	return 1;

}


LRESULT CserverDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

	sockaddr_in remote;
	int i, j, find_user;
	int len;
	int retval;
	int length;
	int Event;     //�¼�
	int result = 0;
	SOCKET s,ns;
	packet_i packet;      //��֤���ݰ�
	struct message mesge_recv, mesge_send;
	struct USER_CONFIRM user_confirm;       
	file_info file_detail;        //�ļ���ϸ��Ϣ  ���� and �ļ���
	char buf[1024];
	switch(message){
	case UM_SOCK:
		s = (SOCKET)LOWORD(wParam);
		Event = LOWORD(lParam);
		switch(Event){
		case FD_ACCEPT:
			len = sizeof(remote);
			ns = accept(s,(sockaddr*)&remote,&len);
			tmp_result[number].sock = ns;
			tmp_result[number].state = STATE_FREE;
			tmp_result[number].port = htons(remote.sin_port);
			sprintf(tmp_result[number].ip, "%s", inet_ntoa(remote.sin_addr));
			if(ns == SOCKET_ERROR){
				closesocket(s);
				main_sock = 0;
				break;
			}

			MessageBox(_T("accept a connection"));
			/*  ��ʼ�������ģ���������  */
			srand( (unsigned)time( NULL ) );            
			packet.len_2bytes = 100*rand()/(RAND_MAX);
			packet.len_4bytes = 100*rand()/(RAND_MAX);
			for(i=0; i<packet.len_2bytes; i++)
			{	
				
				packet.two_bytes[i] = (10000)*rand()/RAND_MAX;
				tmp_result[number].result += packet.two_bytes[i];
				

			}

			for(i=0;i<packet.len_4bytes;i++)
			{	
				packet.four_bytes[i] = (10000)*rand()/RAND_MAX;
				tmp_result[number].result += packet.four_bytes[i];

			}
			/*  ����������ϣ��õ����������result�� */         
			//����packet
			
			mesge_send.type = AUTH_CHICK;
			memcpy(mesge_send.mesg, &packet, sizeof(packet));
			length = 22 + sizeof(packet_i) + 1;
			len = send(ns,(char*)&mesge_send,length,0);
			if(len == SOCKET_ERROR)
			{
				MessageBox(_T("��֤����ʧ��!"));
				break;
			}
			tmp_result[number].state = STATE_RECV_AUTH;
			number++;
			WSAAsyncSelect(ns,m_hWnd,UM_SOCK,FD_READ|FD_CLOSE);
			break;

		case FD_READ:
			len = recv(s,buf,sizeof(buf),0);
			if(len <= 0){
				retval = WSAGetLastError();
				if( retval != WSAEWOULDBLOCK)
				{
					closesocket(s);
					AfxMessageBox(_T("����ʧ��!"));
					//next_state = STATE_FAIL;
					break;
				}
				else
				{
					//next_state = STATE_FAIL;
					break;
				}
			}
			memcpy(&mesge_recv,buf,sizeof(mesge_recv));     //���յ������ݸ��ƹ���
			//���ҵ�ǰ�û�
			for(find_user=0; find_user<N; find_user++)
			{
				if(tmp_result[find_user].sock == s)
					break;
			}

			UpdateData(TRUE);
			if(tmp_result[find_user].state == STATE_RECV_AUTH)
			{
				
				if(mesge_recv.type != AUTH_CHICK)
				{
					next_state = STATE_FREE;
					break;
				}
				//�õ���֤��Ϣ
				memcpy(&user_confirm, mesge_recv.mesg, sizeof(user_confirm));

				//ѭ��ȡ�õ�ǰusers
				for(i=0; i<N; i++)
				{

					if(strstr(users[i].name, user_confirm.name) != NULL)
						break;
							
				}

				result = tmp_result[find_user].result;
				result ^= users[i].passwd;
				tmp_result[find_user].result = result;
				if(tmp_result[find_user].result == user_confirm.result)
				{
					tmp_result[find_user].state = STATE_OK;
					mesge_send.type = AUTH_OK;

					retval = send(s, (char*)&mesge_send, 10, 0);
					if(retval == SOCKET_ERROR)
					{
						AfxMessageBox(_T("������֤���ʧ��!"));
						break;
					}
					MessageBox(_T("��֤�ɹ�!"));


					users[i].port = tmp_result[find_user].port;
					sprintf(users[i].ip, "%s", tmp_result[find_user].ip);
					users[i].isOnline = ONLINE;
					users[i].sock = s;
					
				}
				else
				{
					MessageBox(_T("��֤ʧ��!"));
					closesocket(s);
					return 0;
				}
				break;
			}
			//����֤�ɹ���
			else if(tmp_result[find_user].state == STATE_OK)
			{
				
				switch (mesge_recv.type){
				
				//�ͻ��˹ر�
				case USER_CLOSE:
					for(i=0; i<N; i++)
					{
						if(users[i].sock == s)
							break;
					}
					users[i].isOnline = OFFLINE;   //��ʶ�ͻ�����

				break;

				//��ѯ����
				case ONLINEUSERS:
					retval = check_online_users(s);

				break;

				case PUBLIC_MESG:
					if(!send_commen_message(mesge_recv.type,  mesge_recv))
					{
						AfxMessageBox(_T("����ʧ��!"));
					}
				
				break;

				case PRIVATE_MESG:
					if(!send_commen_message(mesge_recv.type,  mesge_recv))
					{
						AfxMessageBox(_T("���ͳɹ�!"));
					}

				break;


				case UPLOAD:

					//�õ��ϴ��ļ���Ϣ
					if(mesge_recv.fileFlag == FILE_INFO)
					{
						memcpy(&file_detail, mesge_recv.mesg, sizeof(file_detail));
						file_ready_recv(s, file_detail);

					}
					
					else if(mesge_recv.fileFlag == FILE_RECV)
					{
						
						retval = file_recv_bg(s, mesge_recv);

					}

				break;

				case DOWNLOAD:
					
					if(mesge_recv.fileFlag == FILE_INFO)
					{
						memcpy(&file_detail, mesge_recv.mesg, sizeof(file_detail));
						retval = check_file_exist(file_detail, tmp_result[find_user].sock);
						if(retval == 0)
							AfxMessageBox(_T("�����ļ���Ϣʧ��!"));

					}

					else if(mesge_recv.fileFlag == FILE_SEND)
					{
						retval = file_send_bg(s);
						if(retval == 0)
							AfxMessageBox(_T("�ļ�������ϳɹ�!"));

						else if(retval == 1)
							AfxMessageBox(_T("�ļ�����ʧ��!"));

					}



				break;

				}     //switch(mesg.type)  ����
					
				

			}
			break;
		case FD_CLOSE:
			closesocket(s);
			break;
		}
		break;

	}



	return CDialogEx::WindowProc(message, wParam, lParam);
}

//����ļ��Ƿ����
int CserverDlg::check_file_exist(file_info file_detail, SOCKET s)
{

	int retval, i;
	int length;

	//�������ҵ���ǰ�û�
	for(i=0; i<N; i++)
	{
		if(users[i].sock == s)
			break;
	}
	if(i == N)
	{
		AfxMessageBox(_T("�û�������!"));
		return 2;
	}
	struct message msg_send;
	msg_send.type = DOWNLOAD;
	CString CfileName;
	CfileName.Format(_T("%s"), file_detail.filename);
	if(users[i].file_open.Open(_T("F:\\") + CfileName, CFile::modeRead))
	{

		file_info file_dtail;
		
		memcpy(file_dtail.filename, file_detail.filename, sizeof(file_detail.filename));
		file_dtail.length = users[i].file_open.GetLength();
		users[i].send_length = users[i].file_open.GetLength();

		memcpy(msg_send.mesg, &file_dtail, sizeof(file_dtail));
		length = 25 + sizeof(file_dtail);
		//users[i].file_open.Close();
		msg_send.fileFlag = FILE_INFO;
		

	}
	else
	{
		msg_send.fileFlag = FILE_DOTEX;
		length = 3;

	}
	
	retval = send(s, (char*)&msg_send, length, 0);
	if(retval == SOCKET_ERROR)
	{
		return 0;
	}	

	return 1;

}

//�����ļ� �� �ڿͻ���Ҫ�������ļ�ʱ
int CserverDlg::file_send_bg(SOCKET s)
{
	int i, length, retval;
	struct message msg_send;     //�����͵���Ϣ��
	
	//�ж�tmp_buf �Ƿ�Ϊ��
	if(tmp_buf != NULL)
	{
		delete [] tmp_buf;
		tmp_buf = NULL;
	}

	
	
	//�����û�
	for(i=0; i<N; i++)
	{
		if(users[i].sock == s)
			break;
	}
	if(i == N)
	{
		AfxMessageBox(_T("�û�������!"));
		return 3;
	}
	users[i].send_total = 0;
	//�������ڴ�
	tmp_buf = new char[users[i].send_length];

	users[i].file_open.Read(tmp_buf, users[i].send_length);    //��ȡ�ļ���tmp_buf��
	users[i].file_open.Close();                                //�ر��ļ�
	 
	unsigned long long send_size_left = users[i].send_length;    //ʣ���ļ�����
	unsigned long long  pos;            //Ŀǰ�ļ����ݶ������ֽ���

	//ѭ�������ļ�
	for(pos = 0; pos<users[i].send_length;  pos += LENGTH_MAX, send_size_left -= LENGTH_MAX)
	{

		msg_send.type = DOWNLOAD;
		msg_send.fileFlag = FILE_SEND;
		if(send_size_left < LENGTH_MAX)
		{
			char *buf = NULL;
			buf = new char[send_size_left];
			memcpy(buf, tmp_buf+pos, send_size_left);
			sprintf(msg_send.mesg, "%s", buf);
			delete buf;
			buf = NULL;
			users[i].send_total += send_size_left;
		}
		else
		{
			char buf[LENGTH_MAX];
			memcpy(buf, tmp_buf+pos, LENGTH_MAX);
			sprintf(msg_send.mesg, "%s", buf);
			users[i].send_total += LENGTH_MAX;
		}

		retval = send(users[i].sock, (char*)&msg_send, 1000, 0);
		if(retval == SOCKET_ERROR)
		{
			delete tmp_buf;
			tmp_buf = NULL;
			return 1;
		}
		
		if(users[i].send_total >= users[i].send_length)
		{
			delete tmp_buf;
			tmp_buf = NULL;
			return 0;
		}

		Sleep(10);
	}

}

//�ڿͻ���Ҫ�ϴ��ļ�ʱ����׼�������ļ�ʱ��׼��     ���ļ�����ʼ����������
int CserverDlg::file_ready_recv(SOCKET s, file_info file_dtal)
{

	int i, retval;
	CString CfileName;
	struct message mesg_send;
	//�������ҵ���Ӧ�û�
	for(i=0; i<N; i++)
	{
		if(users[i].sock == s)
			break;
	}

	users[i].recv_length = file_dtal.length;
	users[i].recv_total = 0;

	CfileName.Format(_T("%s"), file_dtal.filename);
	users[i].file_open.Open(_T("F:\\") + CfileName, CFile::modeCreate | CFile::modeWrite);
	mesg_send.type = UPLOAD;
	mesg_send.fileFlag = FILE_RECV;
	retval = send(s, (char*)&mesg_send, 4, 0);
	if(retval == SOCKET_ERROR)
	{
		AfxMessageBox(_T("�������ؿ�ʼ����ʧ��!"));
		return 0;
	}

	return 1;
}

//�����ļ������������ļ��Ŀͻ���
int CserverDlg::file_recv_bg(SOCKET s, struct message msg_recv)
{
	int i;
	for(i=0; i<N; i++)
	{
		if(users[i].sock == s)
			break;
	}

	if(tmp_buf  != NULL)  
	{  
		delete [] tmp_buf;  
		tmp_buf = NULL;  
	}  

	if(users[i].recv_length - users[i].recv_total <= LENGTH_MAX)
	{
		int left = users[i].recv_length - users[i].recv_total;
		tmp_buf = new char[left];
		memcpy(tmp_buf, msg_recv.mesg, left);
		users[i].file_open.Write(tmp_buf, left);
		delete tmp_buf;
		tmp_buf = NULL;
		users[i].recv_total += left;
	
	}

	else
	{
		tmp_buf = new char[LENGTH_MAX];     //����һ�����ݿռ�
		memcpy(tmp_buf, msg_recv.mesg, LENGTH_MAX);
		users[i].file_open.Write(tmp_buf, LENGTH_MAX);
		delete [] tmp_buf;
		tmp_buf = NULL;
		users[i].recv_total += LENGTH_MAX;

	}

	if(users[i].recv_total >= users[i].recv_length)
	{
		AfxMessageBox(_T("�ļ����ճɹ�!"));
		users[i].file_open.Close();
		return 1;
	}
	return 0;

}

//���������Ա,������������������Ŀͻ���
int CserverDlg::check_online_users(SOCKET s)
{

	int i;
	int retval;
	int length;
	for(i=0; i<N; i++)
	{
		sprintf(all_users[i].name, "%s", users[i].name);
		all_users[i].isOnline = users[i].isOnline;
		all_users[i].port = users[i].port;
		sprintf(all_users[i].ip, "%s", users[i].ip);
	}

	struct message mesg_send;
	mesg_send.type = ONLINEUSERS;
	memcpy(mesg_send.mesg, all_users, sizeof(all_users));
	length = 25 + sizeof(all_users);
	retval = send(s, (char*)&mesg_send, length, 0);
	if(retval == SOCKET_ERROR)
	{
		AfxMessageBox(_T("����������Աʧ��!"));
		return 0;
	}
	return 1;


}