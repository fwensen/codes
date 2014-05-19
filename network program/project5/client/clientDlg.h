
// clientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#define  UM_SOCK WM_USER + 12

#define MAX_LENGTH 900
#define N 10

//��Ϣ����
#define PUBLIC_MESG 0      //��ͨ��Ϣ  ���������� 1�������й�����ʾ��PUBLIC  
#define PRIVATE_MESG 1     //��ͨ��Ϣ ��������������ʾ��PRIVATE
#define UPLOAD 2	//�ϴ��ļ�	
#define DOWNLOAD 3	//�����ļ�
#define ONLINEUSERS 4  //��ѯ�����û�
#define DIREC_MESG 5 //ֱ�ӵ�����
#define DIREC_UPLOAD 6  //ֱ�ӵ��ϴ��ļ�
#define AUTH_CHICK 7  //����PACKET	
#define AUTH_OK 8    //��֤�ɹ�
#define AUTH_FAIL 9  //��֤ʧ��
#define USER_CLOSE 10 //�û�����



#define STATE_FREE 0     //FREE״̬
#define STATE_RECVAUTH 1  //�ȴ��������˷�����֤��Ϣ
#define STATE_FAIL 2			//��֤ʧ�ܻ��ֹͨ��
#define STATE_OK 3				//��֤�ɹ������ͨ��	


#define FILE_INFO 0         //�ʼʱ�������ļ���Ϣ
#define FILE_RECV 2			//��ʼ��ʽ�����ļ�
#define FILE_SEND 1			//��ʼ��ʽ�����ļ�
#define FILE_DOTEX 3       //�ļ�������

#define ONLINE 0		//����
#define OFFLINE 1		//���߻�����

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
		char toWho[10];		//������
		char fromWho[10];   //���ķ�
		char mesg[950];          //���ļ�����ͨ��Ϣ�ڳ�һ���ڴ���

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

	//������Ա��ѯ
	struct online_user
	{
		char name[10];
		short isOnline;
		char ip[20];
		int port;

	}all_users[N];


	//ֱ������IP��������û�
	struct direct_user
	{
		char name[10];
		SOCKET s;
		CFile file_open;
	}direct_users[N];


	SOCKET main_sock, conn_sock, peer_cli_sock;  //�׽���
	int next_state;

	CString CfileName;
	unsigned long long file_len;   //�ļ�����
	unsigned long long total;    //�յ����ļ��ܳ���
	unsigned long long size_file_left; //�յ��ļ�ʣ���С

	CFile tmp_file;
	CString file_name;    //�ļ���

	char *tmp_buf;        //��ʱ�洢�ļ�����
	int m_editPort;       //�������˿ں�
	int m_editPasswd;     //�û�����
	int m_directPort;     //ֱ���û��˿ں�(������������)
	CString m_directIP;   //ֱ���û�IP
	char char_directIP[20]; //��ʱ�洢ֱ���û�IP
	DWORD m_editIP;         //������IP
	CString m_editUsername;  //�û���
	CString m_editDownld;    //�����ļ���
	CString m_editUpld;      //�ϴ��ļ�����·����
	CString m_editSendmsg;   //���͸������û�����Ϣ
	CListBox m_ListMsg;      //��ʾ������Ϣ
	int m_editPeerPort;      //ֱ���Զ˶˿�
	DWORD m_editPeerIP;      //ֱ���Զ�IP
	CString m_editPeerUpld;  //ֱ�������ļ����Զ�
	CListBox m_ListPeerMsg;  //ֱ��������Ϣ
	CListBox m_ListOnlineUsers;  //�����û��б�
	CString m_editPeerSendmsg;   //ֱ��������Ϣ
	CButton m_buttonConn;        //���ӷ���������
	CButton m_buttonDisconn;     //�Ͽ�����������
	CButton m_buttonDowonld;     //�ӷ����������ļ�����
	CButton m_buttonUpld;        //��������ϴ��ļ�����
	CButton m_buttonLogin;
	CButton m_buttonSendmsg;     //������Ϣ�������û�����
	CButton m_buttonPeerConn;     //ֱ�����Ӱ���
	CButton m_buttonPeerDisconn;  //ֱ���Ͽ����Ӱ���
	CButton m_buttonPeerSendmsg;  //ֱ��������Ϣ����
	CButton m_buttonRefresh;      //ˢ�������û�����
	CButton m_buttonPeerUpld;     //��ֱ���û������ļ�����
	CProgressCtrl m_progress;     //������
	CString m_editMsgTo;          //����˽�Ÿ����û�
	CString m_editPeerMsgTo;
	CIPAddressCtrl m_CtrlIP;     //������IP
	CIPAddressCtrl m_CtrlPeerIP; //ֱ���û�IP
	int m_editLocalPort;		
	CButton m_buttonInit;
	afx_msg void OnConnect();                //���ӷ�����
	afx_msg void OnDisconnect();             //�ͷ������Ͽ�����
	afx_msg void OnDownload();              //�ӷ����������ļ�
	afx_msg void OnUpload();                //��������ϴ��ļ�
	afx_msg void OnSendmsg();               //�������û�������Ϣ     ������Ϣ/˽��
	afx_msg void OnPeerconnect();           //ֱ������IP����������������ת����Զ�����ͨ��
	afx_msg void OnPeerdisconnect();        //ֱ������IP�Ĳ���Զ˶Ͽ�����
	afx_msg void OnPeerupload();            //���Զ˷����ļ�
	afx_msg void OnPeersendmsg();           //ֱ�Ӹ��Զ˷�����Ϣ����������������
	afx_msg void OnRefresh();               //ˢ�½��水�����õ�������Ա
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);   //��Ϣ����
	afx_msg void OnInit();                         //��ʼ���ͻ��˷����������ͻ���Ҳ��Ϊһ��������
	int auth_check_result(packet_i packet);        //���ڼ��������������packet�������ó���֤���
	void message_recv_show(int type, struct message);   //��ʾ�õ�����Ϣ  ������Ϣ/˽��
	int upload_file();              //�ϴ��ļ�
	int refresh_online_users(struct message);        //ˢ��������Ա������
	int handle_direct_msg(SOCKET, struct message);   //����ֱ��ͨ������������������
	int download_file(struct message);      //���������ļ��Ľ���
	
	
};
