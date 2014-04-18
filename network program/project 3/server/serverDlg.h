
// serverDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#define UM_SOCK WM_USER + 1
#define N 10
// CserverDlg 对话框
class CserverDlg : public CDialogEx
{
// 构造
private:
	SOCKET main_sock;
	//SOCKET ns;
	struct USER
	{
		CString name;
		SOCKET u_sock;
	} ;
	     //user的序号
	struct USER user[N];
	CString users;
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
};
