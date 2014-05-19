
// clientDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#define UM_SOCK WM_USER + 10
#define SENDFILE WM_USER+1


const int STATE_FREE = 1;         //初始化成功后进入free状态
const int STATE_RECVASK = 2;		//和服务器建立连接后开始等待服务器发来验证的随机数据
const int STATE_OK = 4;				//验证成功后进入该状态开始正常通信
const int STATE_FAIL = 5;			//验证失败后进入该状态
const int STATE_RECVRESULT = 6;    //发送计算后的验证码后开始等待服务器端的验证结果
// CclientDlg 对话框


const int READY_TO_DO = 2;		//正在或马上将要传输文件


const int FILE_SEND_CMD = 3;     //发送文件放发送命令，自己即将发送文件
const int FILE_RECV_CMD = 4;	//发送文件方接收回复，可以发送或不可以发送
//const int FILE_SEND_FILEINFO = 4;   //发
const int FILE_RECV_INFO_CHECK = 5;
const int FILE_OK_SEND = 6;         //发送文件方可以发送文件
const int FILE_OK_RECV = 7;


const int FREE = 8;    //isBusy空闲
const int BUSY = 9;    //正在或马上要文件传递

const int UPLOAD = 10;
const int DOWNLOAD = 11;
const int OK_TO_SEND = 12;
const int OK_TO_RECV = 13;
const int OK_UPLOAD = 14;
const int OK_DOWNLOAD = 15;



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
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnSendFile(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	typedef	struct PACKET
	{
		int pos_2;
		int pos_4;
		int len_2_bytes;
		int len_4_bytes;
		char str_2_bytes[528];
		char str_4_bytes[528];

	}packet_i;

	typedef struct MESSAGE
	{
		int type;
		long long file_length;
		char *filename;
		char message[128];
	}message_all;

	//传送的文件信息
	typedef struct FILEINFO
	{
		unsigned long long length;
		char  filename[50];
	}file_info;

	file_info file_detail;   //文件详细信息
	int file_flag;			//文件发送标志，1：发送命令 2：成功接收回复后开始发送文件
	int flag;               //全局标志，正在传输文件或空闲

	unsigned long long file_length;   //文件大小

	//CString filePath; 
	

	char *tmpfile_buf;        //临时存储文件数据
	int answer;          //用于查看是否成功验证密码              
	packet_i packet;
	int next_state;
	SOCKET sock;
	afx_msg void OnSend();
	int m_editLocalPort;
	int m_serverPort;
	int m_editCode;
	//DWORD serverIP;
	CIPAddressCtrl m_serverIP;
	CString m_editStatus;
	afx_msg void OnConnect();
	afx_msg void OnDisconnect();
	CListBox m_editMSG;
	DWORD m_pserverIP;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CString m_sendMsg;
	//CProgressCtrl OnDownloadButton;
	CProgressCtrl m_uploadProgress;
	CString m_editDownload;
	CString m_editUpload;
	CButton m_buttonDownload;
	CButton m_buttonUpload;
	afx_msg void OnUploadButton();
	//static DWORD WINAPI client_thread_sendfile(LPVOID lpParam );
	afx_msg void OnDownloadButton();
};
