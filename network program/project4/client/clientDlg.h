
// clientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#define UM_SOCK WM_USER + 10
#define SENDFILE WM_USER+1


const int STATE_FREE = 1;         //��ʼ���ɹ������free״̬
const int STATE_RECVASK = 2;		//�ͷ������������Ӻ�ʼ�ȴ�������������֤���������
const int STATE_OK = 4;				//��֤�ɹ�������״̬��ʼ����ͨ��
const int STATE_FAIL = 5;			//��֤ʧ�ܺ�����״̬
const int STATE_RECVRESULT = 6;    //���ͼ�������֤���ʼ�ȴ��������˵���֤���
// CclientDlg �Ի���


const int READY_TO_DO = 2;		//���ڻ����Ͻ�Ҫ�����ļ�


const int FILE_SEND_CMD = 3;     //�����ļ��ŷ�������Լ����������ļ�
const int FILE_RECV_CMD = 4;	//�����ļ������ջظ������Է��ͻ򲻿��Է���
//const int FILE_SEND_FILEINFO = 4;   //��
const int FILE_RECV_INFO_CHECK = 5;
const int FILE_OK_SEND = 6;         //�����ļ������Է����ļ�
const int FILE_OK_RECV = 7;


const int FREE = 8;    //isBusy����
const int BUSY = 9;    //���ڻ�����Ҫ�ļ�����

const int UPLOAD = 10;
const int DOWNLOAD = 11;
const int OK_TO_SEND = 12;
const int OK_TO_RECV = 13;
const int OK_UPLOAD = 14;
const int OK_DOWNLOAD = 15;



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

	//���͵��ļ���Ϣ
	typedef struct FILEINFO
	{
		unsigned long long length;
		char  filename[50];
	}file_info;

	file_info file_detail;   //�ļ���ϸ��Ϣ
	int file_flag;			//�ļ����ͱ�־��1���������� 2���ɹ����ջظ���ʼ�����ļ�
	int flag;               //ȫ�ֱ�־�����ڴ����ļ������

	unsigned long long file_length;   //�ļ���С

	//CString filePath; 
	

	char *tmpfile_buf;        //��ʱ�洢�ļ�����
	int answer;          //���ڲ鿴�Ƿ�ɹ���֤����              
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
