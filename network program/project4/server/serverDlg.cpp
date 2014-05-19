
// serverDlg.cpp : 实现文件
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


// CserverDlg 对话框

static int number = 0;      //全局变量，便于使用
SOCKET current_sock = -1;
int total = 0;       //进度条进度
unsigned long long current_file_length = 0; //当前将要传输文件大小
char *current_filename = 0;  //当前将要传输文件名称
CString current_file_name = NULL;  //当前将要传输文件名称
CString current_user = NULL;    //当前正在使用文件传输的用户
CString curret_file_path;    //当前文件路径

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
	next_state = STATE_FREE;
	retval = WSAAsyncSelect(main_sock,m_hWnd,UM_SOCK,FD_ACCEPT);
	//m_start.EnableWindow(FALSE);

}




LRESULT CserverDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	int i;    //循环变量
	int retval, len;
	int  num;          //临时中间数据
	int Event;     //事件       
	CString text;    //在list上显示的信息
	CString messageShow;   //收到的信息，有char* 转换为CString
	char buf[128];    //收到数据的缓存
	char answerbuf[16];  //回复的数据 0/1
	INT_PTR nRes;         //子窗口
	SOCKET s,ns;
	sockaddr_in remote;
	packet_i packet;     //数据包
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
			//初始化该用户的各项内容
			user[number].u_sock = ns;             //该用户的套接字
			if(ns == SOCKET_ERROR){
				closesocket(s);
				main_sock = 0;
				break;
			}
			else
			{
				MessageBox(_T("accept a connection"));
				//子对话框，用于给客户端命名
				nRes = UserDlg.DoModal();
				if (IDOK == nRes)
				{
					UpdateData(TRUE);
					user[number].name = UserDlg.m_UserName;   //
					m_showUsers.AddString(UserDlg.m_UserName);  //在用户列表中显示出来
					m_editUsername = UserDlg.m_UserName;
					UpdateData(FALSE);
				}
				
			}

			/*  开始制作报文，并计算结果  */
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
			/*  报文制作完毕，得到结果，存在result中 */

			//发送报文
			retval = send(ns,(char*)&packet,1024,0);
			if(retval == SOCKET_ERROR)
			{
				MessageBox(_T("报文发送失败!"));
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
						MessageBox(_T("验证成功!"));
					sprintf(buf, "%d", 0);
					len = send(s, buf, 3, 0);
					if(len == SOCKET_ERROR)
					{
						MessageBox(_T("验证结果报文发送失败!"));
						break;
					}
					isBusy = FREE;         //可以传输并接收信息
					break;
				}
				else
				{
				
					next_state = STATE_FAIL;
					MessageBox(_T("验证失败!"));
					sprintf(buf, "%d", 1);
					len = send(s, buf, 3, 0);
					if(len == SOCKET_ERROR)
					{
						MessageBox(_T("验证结果报文发送失败!"));
						break;
					}
					break;
				}

				break;
			}


			/* 成功验证后 */
			else if(next_state == STATE_OK)
			{
				
				if (isBusy == FREE)        //isBusy是FREE状态， 代表接收可以任何信息除了文件
				{
					UpdateData(TRUE);
					len = recv(s,buf,128,0);
					//检测是哪个客户端发来的
					for(i=0;i<N; i++)     //遍历套接字寻找客户名 
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

					/* 如果是上传命令 */
					if(strstr(buf, "UPLOAD") != NULL)     
					{
						current_sock = user[i].u_sock;
						current_user.Format(_T("%s"), user[i].name);
						isBusy = BUSY;      //开始准备接收文件信息
						user[i].flag = UPLOAD;               //当前用户打算上传文件
						next_state = STATE_OK;

						sprintf(answerbuf, "%s", "FILE_INFO_UPLD");
						retval = send(current_sock, answerbuf, 20, 0);
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("回复命令失败！"));
							break;
						}

						
						break;
					}

					/* 如果是下载命令 */
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
							AfxMessageBox(_T("回复命令失败！"));
							break;
						}

						Sleep(500);
						break;
					}

					//当要下载对方文件时，对方要求文件信息,现在没有长度，便为0
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
							AfxMessageBox(_T("发送文件信息失败"));
							break;
						}
						user[i].flag = FILE_RECV_INFO_CHECK;
						break;

					}

					//当要上传文件到对方时，对方要求文件信息
					else if(strstr(buf, "FILE_INFO_UPLD") != NULL)
					{
						isBusy = BUSY;
						next_state = STATE_OK;
						sprintf(file_detail.filename,"%s", current_filename);
						file_detail.length = current_file_length;
						retval = send(current_sock, (char*)&file_detail, 128, 0);
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("发送文件信息失败"));
							break;
						}
						user[i].flag = FILE_RECV_INFO_CHECK;
						break;
					}

					else{
					/* 否则便是普通消息 */
					
						text += _T("说:");
						messageShow.Format(_T("%s"),buf);
						text += messageShow;
						//m_showMsg.AddString(text);
						isBusy = FREE;
						next_state = STATE_OK;
						break;
				
					}
				}

				//进行文件交流, 进入BUSY状态
				else if(isBusy == BUSY)
				{
					for(i=0; i<N; i++)
					{
						if(user[i].u_sock == current_sock)
							tmp_flag = user[i].flag;             //获取当前套接字的flag
						break;
					}

					if(tmp_flag == UPLOAD)      //如果用户上传文件，将要准备接收文件信息
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
						dialogResponse.m_response = current_user + _T(" 想要上传 ") + CString(current_filename);
						UpdateData(FALSE); 

						//弹出对话框，确认或取消文件传递
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
								AfxMessageBox(_T("回复命令失败！"));
								break;
							}
							
						}

						m_uploadProgress.SetRange32(0, current_file_length);  //设置进度条范围

						sprintf(answerbuf, "%s", "OK_TO_UPLD");
						retval = send(current_sock, answerbuf, 15, 0);
						if(retval == SOCKET_ERROR)
						{
							AfxMessageBox(_T("发送上传验证回复失败!"));
							break;
						}
						AfxMessageBox(_T("到达OK_TO_UPDATE"));  //调试用
						//tmp_flag = OK_UPLOAD;
						tmp_file.Open(_T("F:\\")+current_file_name, CFile::modeCreate | CFile::modeReadWrite);  //打开或创建文件
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
								AfxMessageBox(_T("发送下载验证回复失败!"));
								break;
							}

						}
						//弹出对话框，确认文件传递或取消
						UpdateData(TRUE); 
						CDialogResponse dialogResponse;
						dialogResponse.m_response = current_user + _T(" 想要下载 ") + CString(current_filename);
						UpdateData(FALSE); 
						nRes = dialogResponse.DoModal();
						if (IDCANCEL == nRes)      //取消时
						{ 
							user[i].flag = FILE_SEND_CMD;
							isBusy = FREE;
							sprintf(answerbuf, "%s", "SORRY");
							retval = send(user[i].u_sock, answerbuf, 15, 0);
							if(retval == SOCKET_ERROR)
							{
								AfxMessageBox(_T("回复命令失败！"));
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
							AfxMessageBox(_T("发送下载验证回复失败!"));
							break;
						}
						
						break;
					}
					
				else if(tmp_flag == OK_UPLOAD)//如果用户上传文件，将要准备接收
					{
						if(tmpfile_buf!=NULL)  
						{  
							delete [] tmpfile_buf;  
							tmpfile_buf=NULL;  
						}  
						tmpfile_buf = new char[1024*55];     //开辟一个数据空间
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
							AfxMessageBox(_T("接收文件成功!"));
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
								AfxMessageBox(_T("发送文件失败!"));
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
						AfxMessageBox(_T("可以上传了!"));
						break;
					}

					else if(strstr(buf, "DO_DOWNLD") != NULL)
					{
						isBusy = BUSY;
						next_state = STATE_OK;
						current_file_length = atoi(buf+9);
						AfxMessageBox(_T("可以接收文件了！"));
						char tmp_buf[10] = "THANKS";
						send(current_sock, tmp_buf, 10, 0);
						tmp_file.Open(_T("F:\\")+m_downloadFileName, CFile::modeCreate | CFile::modeReadWrite);  //打开并创建文件
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
							AfxMessageBox(_T("接收文件成功!"));
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

			  }             //isBusy == BUSY结束
			
			  break;               
			}             // STATE_OK结束
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



void CserverDlg::OnSendfileButton()
{
	UpdateData(TRUE);
	int i;
	CString userName = m_editUsername;
	//遍历用户名，寻找相应的套接字描述符
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
		TCHAR szFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");   
		// 构造打开文件对话框   
		CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);     
		// 显示打开文件对话框   
		if (IDOK == fileDlg.DoModal())   
		{   
			// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
			curret_file_path = fileDlg.GetPathName();   
			SetDlgItemText(IDC_SENDFILE_EDIT, curret_file_path);   
		}

		if(curret_file_path == "")
		{
			AfxMessageBox(_T("未选中任何文件！"));
			return;
		}

		CFile file;
		if(file.Open(curret_file_path, CFile::modeRead))
		{
			current_file_length =  file.GetLength();
		}
		file.Close();
		m_uploadProgress.SetRange32(0, current_file_length);

		//截取文件名
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


		retval = send(user[i].u_sock, buf, 10, 0);                       //发送上传命令
		if(retval == SOCKET_ERROR)
		{
			AfxMessageBox(_T("发送上传命令失败!"));
			return;
		}
		next_state = STATE_OK;
		user[i].flag = FILE_RECV_CMD;     //准备接收命令回复	

	}	

	else if(user[i].flag == FILE_OK_SEND)
	{
		CFile file;
		if(file.Open(curret_file_path, CFile::modeRead))
		{
			if(current_file_length < 1024*1024*600)              //小于600M就直接传输
			{
				if(tmpfile_buf != NULL)
				{
					delete tmpfile_buf;
					tmpfile_buf = NULL;
				}
				tmpfile_buf = new char[current_file_length];
				memset(tmpfile_buf,0,current_file_length);//初始化  
				file.Read(tmpfile_buf,current_file_length);//读取文件  
				file.Close();   
				int retval = send(current_sock, tmpfile_buf, current_file_length, 0);
				if(retval == SOCKET_ERROR)
				{
					AfxMessageBox(_T("发送文件失败！"));
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

	current_sock = user[i].u_sock;
	if(user[i].flag == FILE_SEND_CMD)
	{

		int retval;
		char buf[10] = "DOWNLOAD";
		retval = send(current_sock, buf, 10, 0);                       //发送上传命令
		if(retval == SOCKET_ERROR)
		{
			AfxMessageBox(_T("发送下载命令失败!"));
			return;
		}

		user[i].flag = FILE_RECV_CMD;     //准备接收命令回复	

	}
}
