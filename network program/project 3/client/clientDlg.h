
// clientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#define UM_SOCK WM_USER + 10
#define STATE_RECVASK 10
#define STATE_SENDIDEN 20
// CclientDlg �Ի���
class CclientDlg : public CDialogEx
{
// ����
public:
	CclientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
