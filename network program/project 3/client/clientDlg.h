
// clientDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#define UM_SOCK WM_USER + 10
#define STATE_RECVASK 10
#define STATE_SENDIDEN 20
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
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
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
};
