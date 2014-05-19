
// serverDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#define UM_SOCK WM_USER + 1

#define RAND_INT_MAX 0x7fff00
#define N 10

#define STATE_FREE 0     //FREE状态
#define STATE_RECV_AUTH 1	//接收验证信息
#define STATE_FAIL 2			//验证失败或禁止通信
#define STATE_OK 3				//验证成功或可以通信

//消息命令
#define PUBLIC_MESG 0      //普通信息  有两种类型 1聊天室中公共显示的PUBLIC  
#define PRIVATE_MESG 1     //普通信息 不在聊天室中显示的PRIVATE
#define UPLOAD 2	//上传文件	
#define DOWNLOAD 3	//下载文件
#define ONLINEUSERS 4  //查询在线用户
#define DIREC_MESG 5 //直接的聊天     没用
#define DIREC_UPLOAD 6  //直接的上传文件   没用
#define AUTH_CHICK 7  //发送PACKET	
#define AUTH_OK 8    //验证成功
#define AUTH_FAIL 9   //验证失败
#define USER_CLOSE 10 //用户关闭



#define ONLINE 0		//在线
#define OFFLINE 1		//下线或不在线


#define FILE_INFO 0         //最开始时，发送文件信息
#define FILE_RECV 1			//开始正式接收文件
#define FILE_SEND 2			//开始正式发送文件
#define FILE_DOTEX 3       //文件不存在


#define LENGTH_MAX 900      //每次发送接收文件的大小
// CserverDlg 对话框
class CserverDlg : public CDialogEx
{
// 构造
public:
	CserverDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
private:
	SOCKET main_sock;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	//临时用于存储接收的用户，现在不能得到用户名，用于配合USER
	struct TMP_RESULT
	{
		int result;
		SOCKET sock;
		int state;
		char ip[20];
		int port;
	} tmp_result[N];

	//验证包
	typedef struct PACKET{

		short len_2bytes;
		short len_4bytes;
		short two_bytes[100];
		int four_bytes[100];

	}packet_i;

	//文件信息
	typedef struct file
	{
		unsigned long long length;
		char filename[40];
	}file_info;

	//每次发送的消息   普通消息/文件消息/命令消息。。。
	struct message
	{
		char type;            //PUBLIC  PRIVATE UPLOAD .....
		char fileFlag;      //FILE_INFO  FILE_RECV  FILE_SEND
		char toWho[10];		//发至哪
		char fromWho[10];   //从哪发
		char mesg[950];          //将文件和普通消息融成一块内存中

	};

	//客户端在验证时将客户端名字和计算得到的结果放入该结构体中
	struct USER_CONFIRM
	{
		char name[10];
		int result;
	};

	//所有用户详细信息
	struct USER
	{
		char name[10];
		short isOnline;
		int passwd;
		SOCKET sock;
		CFile file_open;
		char ip[20];                    //IP
		int port;                        //用户端口号
		unsigned long long send_total;  //已发送文件大小
		unsigned long long send_length; //发送文件的大小
		unsigned long long recv_total;   //已接收文件大小
		unsigned long long recv_length;  //接收文件的大小
	}users[N];

	//所有用户，给出状态
	struct online_user
		{
			char name[10];
			short isOnline;
			char ip[20];
			int port;

		}all_users[N];

	


	char *tmp_buf;   //存临时文件数据
	CFile tmp_file;                //文件
	int next_state;                 //用于验证， 下一状态
	CButton m_buttonInit;            //初始化服务器按键
	CButton m_buttonDisconnect;     //断开服务器按键
	int m_editPort;                //端口号
	afx_msg void OnInitButton();             //初始化服务器
	afx_msg void OnDisconnectButton();       //断开服务器
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);  //消息函数
	int send_commen_message(int type, struct message mesg_recv);   //发送信息    公共信息/私信
	int check_file_exist(file_info, SOCKET);    //检测文件是否存在
	int file_ready_recv(SOCKET s , file_info);               //开始接收文件

	int file_recv_bg(SOCKET, struct message);     //接收文件
	int file_send_bg(SOCKET);                   //发送文件
	int check_online_users(SOCKET s);           //查询在线用户
};
