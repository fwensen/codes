
// serverDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#define UM_SOCK WM_USER + 1
const int N = 100;
const int STATE_FREE = 1;           //初始化后进入free状态
const int STATE_RECV_CONFIRM = 4;   //发送随机数据后进入等待接收客户端的验证码
const int STATE_OK = 5;             //验证成功后进入正常通信
const int STATE_FAIL = 6;			//验证失败后进入验证失败状态	

const int COMMEN_MESSAGE_OR_ASK = 1;
//const int COMMON_MESSAGE = 2;
const int UPLOAD = 3;
const int DOWNLOAD = 4;
const int OK_TO_SEND = 5;
const int OK_TO_RECV = 14;
const int OK_UPLOAD = 12;
const int OK_DOWNLOAD = 13;

//const int FREE = 1;       //可发送消息          
const int READY_TO_DO = 6;  //正在传输文件
const int READY_TO_RECV_FILEINFO = 7;  //准备接收文件信息

const int FILE_SEND_CMD = 8;     //准备发送文件处理命令
const int FILE_RECV_CMD = 9;     //接收对方回应
const int FILE_RECV_INFO_CHECK = 14;
const int FILE_OK_SEND = 10;
CONST INT FILE_OK_RECV = 11;

const int BUSY = 1;
const int FREE = 2;
// CserverDlg 对话框
#include<string.h>
#include "afxcmn.h"
class CserverDlg : public CDialogEx
{
// 构造
private:
	SOCKET main_sock;
	
	//SOCKET ns;
	
	     //user的序号
	//struct USER 
	/* 报文信息  */
	typedef	struct PACKET
	{
		int pos_2;
		int pos_4;
		int len_2_bytes;
		int len_4_bytes;
		char str_2_bytes[528];
		char str_4_bytes[528];
		
	}packet_i;
	
	

	//传送的文件信息
	typedef struct FILEINFO
	{
		unsigned long long length;
		char  filename[50];
	}file_info;

	struct USER
	{
		int result;
		CString name;
		SOCKET u_sock;
		int flag;
		int passwd; 

	}user[N];
public:
	CserverDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERVER_DIALOG };

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
	//用户信息
	
	CFile tmp_file;				//用于临时打开文件
	char *tmpfile_buf;              //用于临时存储文件数据
	CString tmp_file_name;      //用于临时存储文件名
	int tmp_flag;               //临时确认文件是上传还是下载


	int isBusy;                 //是否忙于处理文件传输
	int file_flag;              //文件处理的状态

	unsigned long long tmp_file_length;
	//message_all message_packet;
	file_info file_detail;
	int flag;    //在传输文件或空闲
	int next_state;     //状态
	CListBox m_showMsg;
	CListBox m_showUsers;
	CString m_sendMsg;
	afx_msg void OnInitsocket();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	int m_editPort;
	afx_msg void OnDisconnect();
	afx_msg void OnSendmsg();
	CString m_editUsername;
	//CComboBox m_comboUser;
	//afx_msg void OnCbnSelchangeUserCombo();
	CString m_sendFileName;
	CString m_downloadFileName;
	void save_file(char*, CString);   //保存文件
	//CProgressCtrl m_downloadProgress;
	CProgressCtrl m_uploadProgress;
	afx_msg void OnSendfileButton();
	afx_msg void OnDownloadButton();
};
