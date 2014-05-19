
// clientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "client.h"
#include "clientDlg.h"
#include "afxdialogex.h"
#include "DialogResponse.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CString curret_file_path;    //��ǰ�ļ�·��
CString current_file_name;	//��ǰ�ļ���
char *current_filename;     //��ǰ�ļ���
unsigned long long current_file_length;  //��ǰ�ļ�����

CFile tmp_file;            //�ļ�����
int isBusy = FREE;         //����
int result = 0;            //������
char answerbuf[25];         //���ڻظ�


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
	, m_editDownload(_T(""))
	, m_editUpload(_T(""))
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
	//DDX_Control(pDX, IDC_DOWNLOAD_PROGRESS, m_downloadProgress);
	DDX_Control(pDX, IDC_UPLOAD_PROGRESS, m_uploadProgress);
	DDX_Text(pDX, IDC_DOWNLOAD_EDIT, m_editDownload);
	DDX_Text(pDX, IDC_UPLOAD_EDIT, m_editUpload);
	DDX_Control(pDX, IDC_DOWNLOAD_BUTTON, m_buttonDownload);
	DDX_Control(pDX, IDC_UPLOAD_BUTTON, m_buttonUpload);
}

BEGIN_MESSAGE_MAP(CclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_MESSAGE(SENDFILE,OnSendFile)
	ON_BN_CLICKED(IDC_SEND_BUTTON, &CclientDlg::OnSend)
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, &CclientDlg::OnConnect)
	ON_BN_CLICKED(IDC_DISCONNECT_BUTTON, &CclientDlg::OnDisconnect)
	ON_BN_CLICKED(IDC_UPLOAD_BUTTON, &CclientDlg::OnUploadButton)
	ON_BN_CLICKED(IDC_DOWNLOAD_BUTTON, &CclientDlg::OnDownloadButton)
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
	tmpfile_buf = NULL;
	flag = FILE_SEND_CMD;
	isBusy = FREE;
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
	next_state = STATE_FREE;
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

int total = 0;
LRESULT CclientDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	int i;
	INT_PTR nRes;         //�Ӵ���
	int Event;
	int len,retval;
	char buf[128];
	char recvbuf[1024];
	char codebuf[10];
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
			next_state = STATE_RECVASK;
			WSAAsyncSelect(sock,m_hWnd,UM_SOCK,FD_READ|FD_CLOSE);
		break;

		case FD_READ:
			//������֤״̬
			if(next_state == STATE_RECVASK)
			{
				next_state = STATE_RECVRESULT;
				UpdateData(TRUE);
				len = recv(sock,recvbuf,1024,0);
				memcpy(&packet,recvbuf,1024);     //���յ������ݸ��ƹ���
				packet.pos_2 = 0; packet.pos_4 = 0;
				if(len <= 0)
				{
					retval = WSAGetLastError();
						if( retval != WSAEWOULDBLOCK)
						{
							closesocket(sock);
							MessageBox("recv() failed!","server",MB_OK);
							break;
						}

				} 
				for(i=0; i<packet.len_2_bytes; i++)
				{
					char tmpbuf[3];
					memcpy(tmpbuf, packet.str_2_bytes + packet.pos_2,3);
					result += atoi(tmpbuf);
					packet.pos_2 += 3;
				}

				for(i=0; i<packet.len_4_bytes; i++)
				{
					char tmpbuf[5];
					memcpy(tmpbuf, packet.str_4_bytes + packet.pos_4,5);
					result += atoi(tmpbuf);
					packet.pos_4 += 5;
				}
				result ^= m_editCode;
				sprintf(codebuf,"%d",result);
				retval = send(sock,codebuf,10,0);
				if(retval == SOCKET_ERROR)
				{
					MessageBox("��֤����ʧ��,�ر���!");
					closesocket(sock);
					break;
				
				}
				break;

			}

	else if(next_state == STATE_RECVRESULT)
			{
				len = recv(sock,buf,128,0);
				if(len <= 0)
				{
					retval = WSAGetLastError();
					if(retval != WSAEWOULDBLOCK)
					{
						closesocket(sock);
						sock = 0;
					}
					break;
				}
				if((answer = atoi(buf)) == 0)
				{
					next_state =  STATE_OK;
					MessageBox("��֤�ɹ�!");
					break;
				}
				else
				{

					next_state = STATE_FAIL;
					MessageBox("��֤ʧ�ܣ�������ܲ���ȷ!");
					//Sleep(5000);
					break;
				}


				break;
			}


		else if(next_state == STATE_OK)
			{
				if(isBusy == FREE)
				{
					len = recv(sock, buf, 128, 0);
					if(len <= 0)
					{
						retval = WSAGetLastError();
						if(retval != WSAEWOULDBLOCK)
						{
							closesocket(sock);
							sock = 0;
							break;
						}
					}

					if(strstr(buf, "UPLOAD") != NULL)
					{
						isBusy = BUSY;      //��ʼ׼�������ļ���Ϣ
						next_state = STATE_OK;
						flag = UPLOAD;
						sprintf(answerbuf, "%s", "FILE_INFO_UPLD");
						retval = send(sock, answerbuf, 20, 0);
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("�ظ�����ʧ�ܣ�"));
							break;
						}
						break;
					}

					else if(strstr(buf, "DOWNLOAD") != NULL)
					{
						isBusy = BUSY;
						next_state = STATE_OK;
						flag = DOWNLOAD;
						sprintf(answerbuf, "%s", "FILE_INFO_DOWNLD");
						retval = send(sock, answerbuf, 20, 0);
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("�ظ�����ʧ�ܣ�"));
							break;
						}
						break;

					}

					//��Ҫ���ضԷ��ļ�ʱ���Է�Ҫ���ļ���Ϣ
					else if(strstr(buf, "FILE_INFO_DOWNLD") != NULL)
					{

						isBusy = BUSY;
						next_state = STATE_OK;
						flag = FILE_RECV_INFO_CHECK;
						UpdateData(TRUE);
						sprintf(file_detail.filename,"%s", m_editDownload.GetBuffer(0));
						file_detail.length = 0;
						retval = send(sock, (char*)&file_detail, 128, 0);
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("�����ļ���Ϣʧ��"));
							break;
						}
						break;

					}

					//��Ҫ�ϴ��ļ����Է�ʱ���Է�Ҫ���ļ���Ϣ
					else if(strstr(buf, "FILE_INFO_UPLD") != NULL)
					{
						isBusy = BUSY;
						next_state = STATE_OK;
						sprintf(file_detail.filename,"%s", current_filename);
						file_detail.length = current_file_length;
						retval = send(sock, (char*)&file_detail, 128, 0);
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("�����ļ���Ϣʧ��"));
							break;
						}
						flag = FILE_RECV_INFO_CHECK;
						break;
					}

			
					else if(strstr(buf, "FILE_INFO_DOWNLD") != NULL)
					{
							
						UpdateData(TRUE);
						sprintf(file_detail.filename,"%s", m_editDownload.GetBuffer(0));
						file_detail.length = 0;
						retval = send(sock, (char*)&file_detail, 128, 0);
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("�����ļ���Ϣʧ��"));
							break;
						}

						flag = FILE_RECV_INFO_CHECK;

					}
					
					else 
					{
						buf[len] = 0;
						text.Format(_T("%s"),buf);
						messageSay += text;
						m_editMSG.AddString((LPCTSTR)messageSay);
						UpdateData(FALSE);
						break;
					}
				}
				
			else if(isBusy == BUSY)
			{
				if(flag == UPLOAD)
				{
					len = recv(sock, buf, 128, 0);
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
					DialogResponse dialogResponse;
					dialogResponse.m_response =  _T(" ��������Ҫ�ϴ� ") + CString(current_filename);
					UpdateData(FALSE); 

					//�����Ի���ȷ�ϻ�ȡ���ļ�����
					nRes = dialogResponse.DoModal();
					if (IDCANCEL == nRes) 
					{
						isBusy = FREE;
						next_state = STATE_OK;
						flag = FILE_SEND_CMD;
						sprintf(answerbuf, "%s", "SORRY");
						retval = send(sock, answerbuf, 15, 0);
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("�ظ�����ʧ�ܣ�"));
							break;
						}

					}

					m_uploadProgress.SetRange32(0, current_file_length);  //���ý�������Χ

					sprintf(answerbuf, "%s", "OK_TO_UPLD");
					retval = send(sock, answerbuf, 15, 0);
					if(retval == SOCKET_ERROR)
					{
						AfxMessageBox(_T("�����ϴ���֤�ظ�ʧ��!"));
						break;
					}
					AfxMessageBox(_T("����OK_TO_UPDATE"));  //������
					//tmp_flag = OK_UPLOAD;
					tmp_file.Open(_T("G:\\") + current_file_name, CFile::modeCreate | CFile::modeReadWrite);  //�򿪻򴴽��ļ�
					flag = OK_UPLOAD;
					next_state = STATE_OK;
					isBusy = BUSY;
					break;
				}

				else if(flag == FILE_RECV_INFO_CHECK)
				{

					len = recv(sock, buf, 128, 0);
					if(len == SOCKET_ERROR)
					{
						AfxMessageBox(_T("recv file error!"));
						flag = FILE_SEND_CMD;
						isBusy = FREE;
						next_state = STATE_OK;
						break;
					}
					if(strstr(buf, "OK_TO_UPLD") != NULL)
					{
						isBusy = BUSY;
						next_state = STATE_OK;
						flag = FILE_OK_SEND;
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
						send(sock, tmp_buf, 10, 0);
						tmp_file.Open(_T("G:\\")+m_editDownload, CFile::modeCreate | CFile::modeReadWrite);  //�򿪲������ļ�
						flag = FILE_OK_RECV;
						break;
					}

				}

				else if(flag == DOWNLOAD)
				{

					next_state = STATE_OK;
					len = recv(sock, buf, 128, 0);
					if(len == SOCKET_ERROR)
					{
						AfxMessageBox(_T("recv file error!"));
						flag = FILE_SEND_CMD;
						break;
					}
					memcpy(&file_detail, buf, 128);
					current_filename = file_detail.filename;
					current_file_name.Format(_T("%s"), current_filename);
					//�����Ի���ȷ���ļ����ݻ�ȡ��

					if(tmp_file.Open(_T("G:\\")+current_file_name, CFile::modeRead))
					{

						isBusy = BUSY;
						current_file_length = tmp_file.GetLength();
						//char tmp_buf[15];
						//itoa(current_file_length, tmp_buf, 10);
						//retval = send(sock, tmp_buf, 15, 0);
						tmp_file.Close();
						//if(retval == SOCKET_ERROR)
						//{
						//	AfxMessageBox(_T("����������֤�ظ�ʧ��!"));
						//	break;
						//}

					}

					else
					{
						isBusy = FREE;
						sprintf(buf, "%s", "NO SUCH FILE");
						retval = send(sock, buf, 15, 0);
						//tmp_file.Close();
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("����������֤�ظ�ʧ��!"));
							break;
						}

					}

					UpdateData(TRUE); 
					DialogResponse dialogResponse;
					dialogResponse.m_response = _T("��������Ҫ���� ") + CString(current_filename);
					UpdateData(FALSE); 
					nRes = dialogResponse.DoModal();
					if (IDCANCEL == nRes)      //ȡ��ʱ
					{ 
						isBusy = FREE;
						flag = FILE_SEND_CMD;
						//next_state = STATE_OK;
						sprintf(answerbuf, "%s", "SORRY");
						retval = send(sock, answerbuf, 15, 0);
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("�ظ�����ʧ�ܣ�"));
							break;
						}

					}
					CString  tmp_buf = _T("DO_DOWNLD");
					CString n_length;
					n_length.Format(_T("%d"), current_file_length);
					tmp_buf += n_length;
					retval = send(sock, (char*)tmp_buf.GetBuffer(0), 30, 0);
					if(retval == SOCKET_ERROR)
					{
						AfxMessageBox(_T("����������֤�ظ�ʧ��!"));
						break;
					}
					flag = OK_DOWNLOAD;
					break;
				}

				else if(flag == OK_UPLOAD)//����û��ϴ��ļ�����Ҫ׼������
				{

					next_state = STATE_OK;
					if(tmpfile_buf  != NULL)  
					{  
						delete [] tmpfile_buf;  
						tmpfile_buf = NULL;  
					}  

					tmpfile_buf = new char[1024*55];     //����һ�����ݿռ�
					memset(tmpfile_buf, 0, 1024*55);

					len = recv(sock, tmpfile_buf, 1024*55, 0);

					tmp_file.Write(tmpfile_buf, len);
					total += len;
					m_uploadProgress.SetPos(total); 
					delete [] tmpfile_buf;
					tmpfile_buf = NULL;
					if(total >= current_file_length)
					{
						isBusy = FREE;
						next_state = STATE_OK;
						flag = FILE_SEND_CMD;
						tmp_file.Close();
						AfxMessageBox(_T("�����ļ��ɹ�!"));
						break;

					}
					
					isBusy = BUSY;
					flag = OK_UPLOAD;
					break;
				}

				else if(flag == OK_DOWNLOAD)
				{


					if(tmp_file.Open(_T("G:\\")+current_file_name, CFile::modeRead))
					{
						if(tmpfile_buf != NULL)
						{
							delete tmpfile_buf;
							tmpfile_buf = NULL;
						}

						tmpfile_buf = new char[current_file_length];
						memset(tmpfile_buf, 0, current_file_length);
						tmp_file.Read(tmpfile_buf, current_file_length);
						tmp_file.Close();

						retval = send(sock, tmpfile_buf, current_file_length, 0);
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

				}

				else if(flag == FILE_OK_RECV)
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

						len = recv(sock, tmpfile_buf, 1024*52, 0);
						UpdateData(TRUE);
						
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
							flag = FILE_SEND_CMD;
						}
						flag = FILE_OK_RECV;
						next_state = STATE_OK;
						isBusy = BUSY;
						break;
					}

				}          //flag == FILE_OK_RECV


			   }      //isBusy == BUSY
				
			}
			break;

		case FD_CLOSE:
			closesocket(sock);
			break;
		}          
		break;  // switch(message) �� break;
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}

   

void CclientDlg::OnUploadButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
   if(flag == FILE_SEND_CMD)
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
			SetDlgItemText(IDC_UPLOAD_EDIT, curret_file_path);   
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
		retval = send(sock, buf, 10, 0);                       //�����ϴ�����
		if(retval == SOCKET_ERROR)
		{
			AfxMessageBox(_T("�����ϴ�����ʧ��!"));
			return;
		}
		next_state = STATE_OK;
		flag = FILE_RECV_CMD;     //׼����������ظ�	
		
   }	
		
   else if(flag == FILE_OK_SEND)
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
			   int retval = send(sock, tmpfile_buf, current_file_length, 0);
				if(retval == SOCKET_ERROR)
				{
					AfxMessageBox(_T("�����ļ�ʧ�ܣ�"));
					file.Close();
					return;
				}
			   flag = FILE_SEND_CMD;
			   next_state = STATE_OK;
			   isBusy = FREE;
			   m_uploadProgress.SetPos(retval);
		   }
	   }

   }	
		
  


}


void CclientDlg::OnDownloadButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(flag == FILE_SEND_CMD)
	{

		int retval;
		char buf[10] = "DOWNLOAD";
		retval = send(sock, buf, 10, 0);                       //�����ϴ�����
		if(retval == SOCKET_ERROR)
		{
			AfxMessageBox(_T("������������ʧ��!"));
			return;
		}

		flag = FILE_RECV_CMD;     //׼����������ظ�	

	}




}
