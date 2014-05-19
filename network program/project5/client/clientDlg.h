
// clientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#define  UM_SOCK WM_USER + 12

#define MAX_LENGTH 900
#define N 10

//消息命令
#define PUBLIC_MESG 0      //普通信息  有两种类型 1聊天室中公共显示的PUBLIC  
#define PRIVATE_MESG 1     //普通信息 不在聊天室中显示的PRIVATE
#define UPLOAD 2	//上传文件	
#define DOWNLOAD 3	//下载文件
#define ONLINEUSERS 4  //查询在线用户
#define DIREC_MESG 5 //直接的聊天
#define DIREC_UPLOAD 6  //直接的上传文件
#define AUTH_CHICK 7  //接收PACKET	
#define AUTH_OK 8    //验证成功
#define AUTH_FAIL 9  //验证失败
#define USER_CLOSE 10 //用户下线



#define STATE_FREE 0     //FREE状态
#define STATE_RECVAUTH 1  //等待服务器端发来验证信息
#define STATE_FAIL 2			//验证失败或禁止通信
#define STATE_OK 3				//验证成功或可以通信	


#define FILE_INFO 0         //最开始时，发送文件信息
#define FILE_RECV 2			//开始正式接收文件
#define FILE_SEND 1			//开始正式发送文件
#define FILE_DOTEX 3       //文件不存在

#define ONLINE 0		//在线
#define OFFLINE 1		//下线或不在线

// CclientDlg 对话框
class CclientDlg : public CDialogEx
{
// 构造
public:
	CclientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	typedef struct file
	{
		unsigned long long length;
		char filename[40];
	}file_info;

	struct message
	{
		char type;            //PUBLIC  PRIVATE UPLOAD .....
		char fileFlag;      //FILE_INFO  FILE_RECV  FILE_SEND
		char toWho[10];		//发至哪
		char fromWho[10];   //从哪发
		char mesg[950];          //将文件和普通消息融成一块内存中

	};

	typedef struct PACKET{

		short len_2bytes;
		short len_4bytes;
		short two_bytes[100];
		int four_bytes[100];

	}packet_i;

	struct USER_CONFIRM
	{
		char name[10];
		int result;
	};

	//在线人员查询
	struct online_user
	{
		char name[10];
		short isOnline;
		char ip[20];
		int port;

	}all_users[N];


	//直接输入IP后聊天的用户
	struct direct_user
	{
		char name[10];
		SOCKET s;
		CFile file_open;
	}direct_users[N];


	SOCKET main_sock, conn_sock, peer_cli_sock;  //套接字
	int next_state;

	CString CfileName;
	unsigned long long file_len;   //文件长度
	unsigned long long total;    //收到的文件总长度
	unsigned long long size_file_left; //收到文件剩余大小

	CFile tmp_file;
	CString file_name;    //文件名

	char *tmp_buf;        //临时存储文件数据
	int m_editPort;       //服务器端口号
	int m_editPasswd;     //用户密码
	int m_directPort;     //直连用户端口号(不经过服务器)
	CString m_directIP;   //直连用户IP
	char char_directIP[20]; //临时存储直连用户IP
	DWORD m_editIP;         //服务器IP
	CString m_editUsername;  //用户名
	CString m_editDownld;    //下载文件名
	CString m_editUpld;      //上传文件名（路径）
	CString m_editSendmsg;   //发送给其它用户的信息
	CListBox m_ListMsg;      //显示聊天信息
	int m_editPeerPort;      //直连对端端口
	DWORD m_editPeerIP;      //直连对端IP
	CString m_editPeerUpld;  //直连发送文件给对端
	CListBox m_ListPeerMsg;  //直连聊天信息
	CListBox m_ListOnlineUsers;  //在线用户列表
	CString m_editPeerSendmsg;   //直连发送信息
	CButton m_buttonConn;        //连接服务器按键
	CButton m_buttonDisconn;     //断开服务器按键
	CButton m_buttonDowonld;     //从服务器下载文件按键
	CButton m_buttonUpld;        //向服务器上传文件按键
	CButton m_buttonLogin;
	CButton m_buttonSendmsg;     //发送信息给其它用户按键
	CButton m_buttonPeerConn;     //直连连接按键
	CButton m_buttonPeerDisconn;  //直连断开连接按键
	CButton m_buttonPeerSendmsg;  //直连发送信息按键
	CButton m_buttonRefresh;      //刷新在线用户按键
	CButton m_buttonPeerUpld;     //向直连用户发送文件按键
	CProgressCtrl m_progress;     //进度条
	CString m_editMsgTo;          //发送私信给该用户
	CString m_editPeerMsgTo;
	CIPAddressCtrl m_CtrlIP;     //服务器IP
	CIPAddressCtrl m_CtrlPeerIP; //直连用户IP
	int m_editLocalPort;		
	CButton m_buttonInit;
	afx_msg void OnConnect();                //连接服务器
	afx_msg void OnDisconnect();             //和服务器断开连接
	afx_msg void OnDownload();              //从服务器下载文件
	afx_msg void OnUpload();                //向服务器上传文件
	afx_msg void OnSendmsg();               //向其它用户发送信息     公共信息/私信
	afx_msg void OnPeerconnect();           //直接输入IP（不经过服务器中转）与对端连接通话
	afx_msg void OnPeerdisconnect();        //直接输入IP的并与对端断开连接
	afx_msg void OnPeerupload();            //给对端发送文件
	afx_msg void OnPeersendmsg();           //直接给对端发送信息（不经过服务器）
	afx_msg void OnRefresh();               //刷新界面按键，得到在线人员
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);   //消息函数
	afx_msg void OnInit();                         //初始化客户端服务器，即客户端也作为一个服务器
	int auth_check_result(packet_i packet);        //用于计算服务器发来的packet包，并得出验证结果
	void message_recv_show(int type, struct message);   //显示得到的信息  公共信息/私信
	int upload_file();              //上传文件
	int refresh_online_users(struct message);        //刷新在线人员处理函数
	int handle_direct_msg(SOCKET, struct message);   //处理直接通话（不经过服务器）
	int download_file(struct message);      //处理下载文件的接收
	
	
};
