
// serverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "server.h"
#include "serverDlg.h"
#include "afxdialogex.h"
#include "DialogUser.h"
#include "DialogResponse.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CserverDlg �Ի���

static int number = 0;      //ȫ�ֱ���������ʹ��
SOCKET current_sock = -1;
int total = 0;       //����������
unsigned long long current_file_length = 0; //��ǰ��Ҫ�����ļ���С
char *current_filename = 0;  //��ǰ��Ҫ�����ļ�����
CString current_file_name = NULL;  //��ǰ��Ҫ�����ļ�����
CString current_user = NULL;    //��ǰ����ʹ���ļ�������û�
CString curret_file_path;    //��ǰ�ļ�·��

CserverDlg::CserverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CserverDlg::IDD, pParent)
	, m_sendMsg(_T(""))
	, m_editPort(8000)
	, main_sock(0)
	, m_editUsername(_T(""))
	, m_sendFileName(_T(""))
	, m_downloadFileName(_T(""))
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
	DDX_Text(pDX, IDC_SENDFILE_EDIT, m_sendFileName);
	DDX_Text(pDX, IDC_DOWNFILE_EDIT, m_downloadFileName);
	//DDX_Control(pDX, IDC_DOWNLOAD_PROGRESS, m_downloadProgress);
	DDX_Control(pDX, IDC_UPLOAD_PROGRESS, m_uploadProgress);
}

BEGIN_MESSAGE_MAP(CserverDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INITSOCKET_BUTTON, &CserverDlg::OnInitsocket)
	ON_BN_CLICKED(IDC_DISCONNECT_BUTTON, &CserverDlg::OnDisconnect)
	ON_BN_CLICKED(IDC_SENDMSG_BUTTON, &CserverDlg::OnSendmsg)
//	ON_CBN_SELCHANGE(IDC_USER_COMBO, &CserverDlg::OnCbnSelchangeUserCombo)
ON_BN_CLICKED(IDC_SENFFILE_BUTTON, &CserverDlg::OnSendfileButton)
ON_BN_CLICKED(IDC_DOWNLOAD_BUTTON, &CserverDlg::OnDownloadButton)
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
	tmpfile_buf = NULL;
	//result = 0;
	isBusy = FREE;
	flag = FILE_SEND_CMD;
	sockaddr_in server;
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
	next_state = STATE_FREE;
	retval = WSAAsyncSelect(main_sock,m_hWnd,UM_SOCK,FD_ACCEPT);
	//m_start.EnableWindow(FALSE);

}




LRESULT CserverDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	int i;    //ѭ������
	int retval, len;
	int  num;          //��ʱ�м�����
	int Event;     //�¼�       
	CString text;    //��list����ʾ����Ϣ
	CString messageShow;   //�յ�����Ϣ����char* ת��ΪCString
	char buf[128];    //�յ����ݵĻ���
	char answerbuf[16];  //�ظ������� 0/1
	INT_PTR nRes;         //�Ӵ���
	SOCKET s,ns;
	sockaddr_in remote;
	packet_i packet;     //���ݰ�
	DialogUser UserDlg;
	char tmpbuf[5];
	char tmp[20];
	CString  tmp_buf ;
	CString n_length;


	switch(message){
	case UM_SOCK:
		s = (SOCKET)LOWORD(wParam);
		Event = LOWORD(lParam);
		switch(Event){
		case FD_ACCEPT:
			user[number].result = 0;
			len = sizeof(remote);
			ns = accept(s,(sockaddr*)&remote,&len);
			user[number].flag = FILE_SEND_CMD;
			//��ʼ�����û��ĸ�������
			user[number].u_sock = ns;             //���û����׽���
			if(ns == SOCKET_ERROR){
				closesocket(s);
				main_sock = 0;
				break;
			}
			else
			{
				MessageBox(_T("accept a connection"));
				//�ӶԻ������ڸ��ͻ�������
				nRes = UserDlg.DoModal();
				if (IDOK == nRes)
				{
					UpdateData(TRUE);
					user[number].name = UserDlg.m_UserName;   //
					m_showUsers.AddString(UserDlg.m_UserName);  //���û��б�����ʾ����
					m_editUsername = UserDlg.m_UserName;
					UpdateData(FALSE);
				}
				
			}

			/*  ��ʼ�������ģ���������  */
			srand( (unsigned)time( NULL ) );            
			packet.len_2_bytes = 100*rand()/(RAND_MAX);
			packet.len_4_bytes = 100*rand()/(RAND_MAX);
			packet.pos_2 = 0;  packet.pos_4 = 0;
			//packet.length = length;
			for(i=0;i<packet.len_2_bytes;i++)
			{	
					//packet.num[i] = new char[3];
					num = 100*rand()/RAND_MAX;
					user[number].result += num;
					sprintf(tmpbuf, "%d", num);
					memcpy(packet.str_2_bytes+packet.pos_2, tmpbuf, 3);
					packet.pos_2 += 3;
					
			}

			for(i=0;i<packet.len_4_bytes;i++)
			{	
				//packet.num[i] = new char[3];
				num = 10000*rand()/RAND_MAX;
				user[number].result += num;
				sprintf(tmpbuf, "%d", num);
				memcpy(packet.str_4_bytes+packet.pos_4, tmpbuf, 5);
				packet.pos_4 += 5;

			}

			user[number].result ^= 123456;
			/*  ����������ϣ��õ����������result�� */

			//���ͱ���
			retval = send(ns,(char*)&packet,1024,0);
			if(retval == SOCKET_ERROR)
			{
				MessageBox(_T("���ķ���ʧ��!"));
				break;
			}

			next_state = STATE_RECV_CONFIRM;
			WSAAsyncSelect(ns,m_hWnd,UM_SOCK,FD_READ|FD_CLOSE);
		break;

		case FD_READ:
			if(next_state == STATE_RECV_CONFIRM)
			{
				len = recv(s,buf,10,0);
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
				if(user[number].result == atoi(buf))
				{
					next_state = STATE_OK;
						MessageBox(_T("��֤�ɹ�!"));
					sprintf(buf, "%d", 0);
					len = send(s, buf, 3, 0);
					if(len == SOCKET_ERROR)
					{
						MessageBox(_T("��֤������ķ���ʧ��!"));
						break;
					}
					isBusy = FREE;         //���Դ��䲢������Ϣ
					break;
				}
				else
				{
				
					next_state = STATE_FAIL;
					MessageBox(_T("��֤ʧ��!"));
					sprintf(buf, "%d", 1);
					len = send(s, buf, 3, 0);
					if(len == SOCKET_ERROR)
					{
						MessageBox(_T("��֤������ķ���ʧ��!"));
						break;
					}
					break;
				}

				break;
			}


			/* �ɹ���֤�� */
			else if(next_state == STATE_OK)
			{
				
				if (isBusy == FREE)        //isBusy��FREE״̬�� ������տ����κ���Ϣ�����ļ�
				{
					UpdateData(TRUE);
					len = recv(s,buf,128,0);
					//������ĸ��ͻ��˷�����
					for(i=0;i<N; i++)     //�����׽���Ѱ�ҿͻ��� 
					{
						if(user[i].u_sock == s)
							{
							text = user[i].name;
							break;
						}
						
					}
					if(len <= 0)
					{
						retval = WSAGetLastError();
						if( retval != WSAEWOULDBLOCK)
						{
							closesocket(s);
							MessageBox(_T("recv() failed!"));
							break;
						}
						else
							break;
					}
					buf[len] = 0;

					/* ������ϴ����� */
					if(strstr(buf, "UPLOAD") != NULL)     
					{
						current_sock = user[i].u_sock;
						current_user.Format(_T("%s"), user[i].name);
						isBusy = BUSY;      //��ʼ׼�������ļ���Ϣ
						user[i].flag = UPLOAD;               //��ǰ�û������ϴ��ļ�
						next_state = STATE_OK;

						sprintf(answerbuf, "%s", "FILE_INFO_UPLD");
						retval = send(current_sock, answerbuf, 20, 0);
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("�ظ�����ʧ�ܣ�"));
							break;
						}

						
						break;
					}

					/* ������������� */
				   else if(strstr(buf, "DOWNLOAD") != NULL)
					{
						
						isBusy = BUSY; 
						user[i].flag = DOWNLOAD;
						current_sock = user[i].u_sock;
						current_user.Format(_T("%s"), user[i].name);
						next_state = STATE_OK;
						sprintf(tmp, "%s", "FILE_INFO_DOWNLD");
						retval = send(current_sock, tmp, 20, 0);
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("�ظ�����ʧ�ܣ�"));
							break;
						}

						Sleep(500);
						break;
					}

					//��Ҫ���ضԷ��ļ�ʱ���Է�Ҫ���ļ���Ϣ,����û�г��ȣ���Ϊ0
					else if(strstr(buf, "FILE_INFO_DOWNLD") != NULL)
					{
						
						isBusy = BUSY;
						next_state = STATE_OK;
						UpdateData(TRUE);
						sprintf(file_detail.filename,"%s", m_downloadFileName.GetBuffer(0));
						file_detail.length = 0;
						retval = send(current_sock, (char*)&file_detail, 128, 0);
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("�����ļ���Ϣʧ��"));
							break;
						}
						user[i].flag = FILE_RECV_INFO_CHECK;
						break;

					}

					//��Ҫ�ϴ��ļ����Է�ʱ���Է�Ҫ���ļ���Ϣ
					else if(strstr(buf, "FILE_INFO_UPLD") != NULL)
					{
						isBusy = BUSY;
						next_state = STATE_OK;
						sprintf(file_detail.filename,"%s", current_filename);
						file_detail.length = current_file_length;
						retval = send(current_sock, (char*)&file_detail, 128, 0);
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("�����ļ���Ϣʧ��"));
							break;
						}
						user[i].flag = FILE_RECV_INFO_CHECK;
						break;
					}

					else{
					/* ���������ͨ��Ϣ */
					
						text += _T("˵:");
						messageShow.Format(_T("%s"),buf);
						text += messageShow;
						//m_showMsg.AddString(text);
						isBusy = FREE;
						next_state = STATE_OK;
						break;
				
					}
				}

				//�����ļ�����, ����BUSY״̬
				else if(isBusy == BUSY)
				{
					for(i=0; i<N; i++)
					{
						if(user[i].u_sock == current_sock)
							tmp_flag = user[i].flag;             //��ȡ��ǰ�׽��ֵ�flag
						break;
					}

					if(tmp_flag == UPLOAD)      //����û��ϴ��ļ�����Ҫ׼�������ļ���Ϣ
					{
						len = recv(current_sock, buf, 128, 0);
						if(len == SOCKET_ERROR)
						{
							AfxMessageBox(_T("recv file error!"));
							flag = FREE;
							break;
						}

						memcpy(&file_detail, buf, 128);
						current_file_length = file_detail.length;
						current_filename = file_detail.filename;
						current_file_name.Format(_T("%s"), current_filename);
						UpdateData(TRUE); 
						CDialogResponse dialogResponse;
						dialogResponse.m_response = current_user + _T(" ��Ҫ�ϴ� ") + CString(current_filename);
						UpdateData(FALSE); 

						//�����Ի���ȷ�ϻ�ȡ���ļ�����
						nRes = dialogResponse.DoModal();
						if (IDCANCEL == nRes) 
						{
							isBusy = FREE;
							next_state = STATE_OK;
							user[i].flag = FILE_SEND_CMD;
							sprintf(answerbuf, "%s", "SORRY");
							retval = send(user[i].u_sock, answerbuf, 15, 0);
							if(retval == SOCKET_ERROR)
							{
								AfxMessageBox(_T("�ظ�����ʧ�ܣ�"));
								break;
							}
							
						}

						m_uploadProgress.SetRange32(0, current_file_length);  //���ý�������Χ

						sprintf(answerbuf, "%s", "OK_TO_UPLD");
						retval = send(current_sock, answerbuf, 15, 0);
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("�����ϴ���֤�ظ�ʧ��!"));
							break;
						}
						AfxMessageBox(_T("����OK_TO_UPDATE"));  //������
						//tmp_flag = OK_UPLOAD;
						tmp_file.Open(_T("F:\\")+current_file_name, CFile::modeCreate | CFile::modeReadWrite);  //�򿪻򴴽��ļ�
						user[i].flag = OK_UPLOAD;
						next_state = STATE_OK;
						isBusy = BUSY;
						break;
					}

					else if(tmp_flag == DOWNLOAD)
					{
						next_state = STATE_OK;
						len = recv(current_sock, buf, 128, 0);
						if(len == SOCKET_ERROR)
						{
							AfxMessageBox(_T("recv file error!"));
							user[i].flag = FILE_SEND_CMD;
							break;
						}
						memcpy(&file_detail, buf, 128);
						current_filename = file_detail.filename;
						current_file_name.Format(_T("%s"), current_filename);
						
						if(tmp_file.Open(_T("F:\\")+current_file_name, CFile::modeRead))
						{

							isBusy = BUSY;
							current_file_length = tmp_file.GetLength();
							tmp_file.Close();
						}

						else
						{
							isBusy = FREE;
							sprintf(buf, "%s", "NO SUCH FILE");
							retval = send(current_sock, buf, 15, 0);
							//tmp_file.Close();
							if(retval == SOCKET_ERROR)
							{
								AfxMessageBox(_T("����������֤�ظ�ʧ��!"));
								break;
							}

						}
						//�����Ի���ȷ���ļ����ݻ�ȡ��
						UpdateData(TRUE); 
						CDialogResponse dialogResponse;
						dialogResponse.m_response = current_user + _T(" ��Ҫ���� ") + CString(current_filename);
						UpdateData(FALSE); 
						nRes = dialogResponse.DoModal();
						if (IDCANCEL == nRes)      //ȡ��ʱ
						{ 
							user[i].flag = FILE_SEND_CMD;
							isBusy = FREE;
							sprintf(answerbuf, "%s", "SORRY");
							retval = send(user[i].u_sock, answerbuf, 15, 0);
							if(retval == SOCKET_ERROR)
							{
								AfxMessageBox(_T("�ظ�����ʧ�ܣ�"));
								break;
							}

						}
						user[i].flag = OK_DOWNLOAD;
						tmp_buf = _T("DO_DOWNLD");
						n_length.Format(_T("%d"), current_file_length);
						tmp_buf += n_length;
						retval = send(current_sock, (char*)tmp_buf.GetBuffer(0), 30, 0);
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("����������֤�ظ�ʧ��!"));
							break;
						}
						
						break;
					}
					
				else if(tmp_flag == OK_UPLOAD)//����û��ϴ��ļ�����Ҫ׼������
					{
						if(tmpfile_buf!=NULL)  
						{  
							delete [] tmpfile_buf;  
							tmpfile_buf=NULL;  
						}  
						tmpfile_buf = new char[1024*55];     //����һ�����ݿռ�
						memset(tmpfile_buf,0,1024*55);
						//char tmp[1024*55]; 
						len = recv(current_sock, tmpfile_buf, 1024*55, 0);
				
						tmp_file.Write(tmpfile_buf, len);
						total += len;
						m_uploadProgress.SetPos(total); 
						delete [] tmpfile_buf;
						tmpfile_buf = NULL;
						if(total >= current_file_length)
						{
							isBusy = FREE;
							next_state = STATE_OK;
							user[i].flag = FILE_SEND_CMD;
							
							tmp_file.Close();
							AfxMessageBox(_T("�����ļ��ɹ�!"));
							break;
								
						}
						user[i].flag = OK_UPLOAD;
						next_state = STATE_OK;
						isBusy = BUSY;
						break;
					}

				else if(tmp_flag == OK_DOWNLOAD)
				{
					user[i].flag = FILE_SEND_CMD;
					len = recv(current_sock, buf, 128, 0);
					if(len == SOCKET_ERROR)
					{
						AfxMessageBox(_T("recv file error!"));
						flag = FREE;
						break;
					}
					if(strstr(buf, "THANKS") != NULL)
						if(tmp_file.Open(_T("F:\\")+current_file_name, CFile::modeRead))
						{
							if(tmpfile_buf != NULL)
							{
								delete [] tmpfile_buf;
								tmpfile_buf = NULL;
							}

							tmpfile_buf = new char[current_file_length];
							memset(tmpfile_buf, 0, current_file_length);
							
							tmp_file.Read(tmpfile_buf, current_file_length);
							tmp_file.Close();
							retval = send(current_sock, tmpfile_buf, current_file_length, 0);
							delete [] tmpfile_buf;
							tmpfile_buf = NULL;
							if(retval == SOCKET_ERROR)
							{
								isBusy = FREE;
								next_state = STATE_OK;
								AfxMessageBox(_T("�����ļ�ʧ��!"));
								break;
							}

							isBusy = FREE;
							next_state = STATE_OK;
							break;

						}
						break;
				}

				else if(tmp_flag == FILE_RECV_INFO_CHECK)
				{
					len = recv(current_sock, buf, 128, 0);
					if(len == SOCKET_ERROR)
					{
						AfxMessageBox(_T("recv file error!"));
						user[i].flag = FILE_SEND_CMD;
						isBusy = FREE;
						next_state = STATE_OK;
						break;
					}
					if(strstr(buf, "OK_TO_UPLD") != NULL)
					{
						isBusy = BUSY;
						next_state = STATE_OK;
						user[i].flag = FILE_OK_SEND;
						AfxMessageBox(_T("�����ϴ���!"));
						break;
					}

					else if(strstr(buf, "DO_DOWNLD") != NULL)
					{
						isBusy = BUSY;
						next_state = STATE_OK;
						current_file_length = atoi(buf+9);
						AfxMessageBox(_T("���Խ����ļ��ˣ�"));
						char tmp_buf[10] = "THANKS";
						send(current_sock, tmp_buf, 10, 0);
						tmp_file.Open(_T("F:\\")+m_downloadFileName, CFile::modeCreate | CFile::modeReadWrite);  //�򿪲������ļ�
						user[i].flag = FILE_OK_RECV;
						break;
					}
					break;
				}

				else if(tmp_flag == FILE_OK_RECV)
				{
					if(current_file_length > 0)
					{
						if(tmpfile_buf != NULL)
						{
							delete [] tmpfile_buf;
							tmpfile_buf = NULL;
						}
						m_uploadProgress.SetRange32(0, current_file_length);
						tmpfile_buf = new char[1024*52];
						memset(tmpfile_buf, 0, 1024*52);

						len = recv(current_sock, tmpfile_buf, 1024*52, 0);

						tmp_file.Write(tmpfile_buf, len);
						total += len;
						m_uploadProgress.SetPos(total);
						delete [] tmpfile_buf;
						tmpfile_buf = NULL;
						if(total >= current_file_length)
						{
							isBusy = FREE;
							next_state = STATE_OK;
							tmp_file.Close();
							AfxMessageBox(_T("�����ļ��ɹ�!"));
							user[i].u_sock = FILE_SEND_CMD;
							break;
						}
						user[i].flag = FILE_OK_RECV;
						next_state = STATE_OK;
						isBusy = BUSY;
						break;
					}
					break;

				}
				break;

			  }             //isBusy == BUSY����
			
			  break;               
			}             // STATE_OK����
			else if(next_state = STATE_FAIL)
			{
				closesocket(s);
				break;
			
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



void CserverDlg::OnSendfileButton()
{
	UpdateData(TRUE);
	int i;
	CString userName = m_editUsername;
	//�����û�����Ѱ����Ӧ���׽���������
	for(i=0;i<N; i++)
	{
		if(user[i].name == userName)
			break;
	}
	
	current_sock = user[i].u_sock;
	if(user[i].flag == FILE_SEND_CMD)
	{

		int retval;
		int pos=0, start=0;  
		TCHAR szFilter[] = _T("�ı��ļ�(*.txt)|*.txt|�����ļ�(*.*)|*.*||");   
		// ������ļ��Ի���   
		CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);     
		// ��ʾ���ļ��Ի���   
		if (IDOK == fileDlg.DoModal())   
		{   
			// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����   
			curret_file_path = fileDlg.GetPathName();   
			SetDlgItemText(IDC_SENDFILE_EDIT, curret_file_path);   
		}

		if(curret_file_path == "")
		{
			AfxMessageBox(_T("δѡ���κ��ļ���"));
			return;
		}

		CFile file;
		if(file.Open(curret_file_path, CFile::modeRead))
		{
			current_file_length =  file.GetLength();
		}
		file.Close();
		m_uploadProgress.SetRange32(0, current_file_length);

		//��ȡ�ļ���
		while(1)  
		{  
			start = pos;  
			pos=curret_file_path.Find('\\',start+1);  
			if(pos<0)  
				break;  
		}  
		current_file_name = curret_file_path.Right(curret_file_path.GetLength()-start-1);
		current_filename = current_file_name.GetBuffer(0);
		char buf[10] = "UPLOAD";


		retval = send(user[i].u_sock, buf, 10, 0);                       //�����ϴ�����
		if(retval == SOCKET_ERROR)
		{
			AfxMessageBox(_T("�����ϴ�����ʧ��!"));
			return;
		}
		next_state = STATE_OK;
		user[i].flag = FILE_RECV_CMD;     //׼����������ظ�	

	}	

	else if(user[i].flag == FILE_OK_SEND)
	{
		CFile file;
		if(file.Open(curret_file_path, CFile::modeRead))
		{
			if(current_file_length < 1024*1024*600)              //С��600M��ֱ�Ӵ���
			{
				if(tmpfile_buf != NULL)
				{
					delete tmpfile_buf;
					tmpfile_buf = NULL;
				}
				tmpfile_buf = new char[current_file_length];
				memset(tmpfile_buf,0,current_file_length);//��ʼ��  
				file.Read(tmpfile_buf,current_file_length);//��ȡ�ļ�  
				file.Close();   
				int retval = send(current_sock, tmpfile_buf, current_file_length, 0);
				if(retval == SOCKET_ERROR)
				{
					AfxMessageBox(_T("�����ļ�ʧ�ܣ�"));
					file.Close();
					return;
				}
				user[i].flag = FILE_SEND_CMD;
				next_state = STATE_OK;
				isBusy = FREE;
				m_uploadProgress.SetPos(retval);
			}
		}

	}	




	
}

void CserverDlg::OnDownloadButton()
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

	current_sock = user[i].u_sock;
	if(user[i].flag == FILE_SEND_CMD)
	{

		int retval;
		char buf[10] = "DOWNLOAD";
		retval = send(current_sock, buf, 10, 0);                       //�����ϴ�����
		if(retval == SOCKET_ERROR)
		{
			AfxMessageBox(_T("������������ʧ��!"));
			return;
		}

		user[i].flag = FILE_RECV_CMD;     //׼����������ظ�	

	}
}
