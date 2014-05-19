
// serverDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#define UM_SOCK WM_USER + 1
const int N = 100;
const int STATE_FREE = 1;           //��ʼ�������free״̬
const int STATE_RECV_CONFIRM = 4;   //����������ݺ����ȴ����տͻ��˵���֤��
const int STATE_OK = 5;             //��֤�ɹ����������ͨ��
const int STATE_FAIL = 6;			//��֤ʧ�ܺ������֤ʧ��״̬	

const int COMMEN_MESSAGE_OR_ASK = 1;
//const int COMMON_MESSAGE = 2;
const int UPLOAD = 3;
const int DOWNLOAD = 4;
const int OK_TO_SEND = 5;
const int OK_TO_RECV = 14;
const int OK_UPLOAD = 12;
const int OK_DOWNLOAD = 13;

//const int FREE = 1;       //�ɷ�����Ϣ          
const int READY_TO_DO = 6;  //���ڴ����ļ�
const int READY_TO_RECV_FILEINFO = 7;  //׼�������ļ���Ϣ

const int FILE_SEND_CMD = 8;     //׼�������ļ���������
const int FILE_RECV_CMD = 9;     //���նԷ���Ӧ
const int FILE_RECV_INFO_CHECK = 14;
const int FILE_OK_SEND = 10;
CONST INT FILE_OK_RECV = 11;

const int BUSY = 1;
const int FREE = 2;
// CserverDlg �Ի���
#include<string.h>
#include "afxcmn.h"
class CserverDlg : public CDialogEx
{
// ����
private:
	SOCKET main_sock;
	
	//SOCKET ns;
	
	     //user�����
	//struct USER 
	/* ������Ϣ  */
	typedef	struct PACKET
	{
		int pos_2;
		int pos_4;
		int len_2_bytes;
		int len_4_bytes;
		char str_2_bytes[528];
		char str_4_bytes[528];
		
	}packet_i;
	
	

	//���͵��ļ���Ϣ
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
	CserverDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//�û���Ϣ
	
	CFile tmp_file;				//������ʱ���ļ�
	char *tmpfile_buf;              //������ʱ�洢�ļ�����
	CString tmp_file_name;      //������ʱ�洢�ļ���
	int tmp_flag;               //��ʱȷ���ļ����ϴ���������


	int isBusy;                 //�Ƿ�æ�ڴ����ļ�����
	int file_flag;              //�ļ������״̬

	unsigned long long tmp_file_length;
	//message_all message_packet;
	file_info file_detail;
	int flag;    //�ڴ����ļ������
	int next_state;     //״̬
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
	void save_file(char*, CString);   //�����ļ�
	//CProgressCtrl m_downloadProgress;
	CProgressCtrl m_uploadProgress;
	afx_msg void OnSendfileButton();
	afx_msg void OnDownloadButton();
};
