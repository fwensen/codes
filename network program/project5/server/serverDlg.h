
// serverDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#define UM_SOCK WM_USER + 1

#define RAND_INT_MAX 0x7fff00
#define N 10

#define STATE_FREE 0     //FREE״̬
#define STATE_RECV_AUTH 1	//������֤��Ϣ
#define STATE_FAIL 2			//��֤ʧ�ܻ��ֹͨ��
#define STATE_OK 3				//��֤�ɹ������ͨ��

//��Ϣ����
#define PUBLIC_MESG 0      //��ͨ��Ϣ  ���������� 1�������й�����ʾ��PUBLIC  
#define PRIVATE_MESG 1     //��ͨ��Ϣ ��������������ʾ��PRIVATE
#define UPLOAD 2	//�ϴ��ļ�	
#define DOWNLOAD 3	//�����ļ�
#define ONLINEUSERS 4  //��ѯ�����û�
#define DIREC_MESG 5 //ֱ�ӵ�����     û��
#define DIREC_UPLOAD 6  //ֱ�ӵ��ϴ��ļ�   û��
#define AUTH_CHICK 7  //����PACKET	
#define AUTH_OK 8    //��֤�ɹ�
#define AUTH_FAIL 9   //��֤ʧ��
#define USER_CLOSE 10 //�û��ر�



#define ONLINE 0		//����
#define OFFLINE 1		//���߻�����


#define FILE_INFO 0         //�ʼʱ�������ļ���Ϣ
#define FILE_RECV 1			//��ʼ��ʽ�����ļ�
#define FILE_SEND 2			//��ʼ��ʽ�����ļ�
#define FILE_DOTEX 3       //�ļ�������


#define LENGTH_MAX 900      //ÿ�η��ͽ����ļ��Ĵ�С
// CserverDlg �Ի���
class CserverDlg : public CDialogEx
{
// ����
public:
	CserverDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
private:
	SOCKET main_sock;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	//��ʱ���ڴ洢���յ��û������ڲ��ܵõ��û������������USER
	struct TMP_RESULT
	{
		int result;
		SOCKET sock;
		int state;
		char ip[20];
		int port;
	} tmp_result[N];

	//��֤��
	typedef struct PACKET{

		short len_2bytes;
		short len_4bytes;
		short two_bytes[100];
		int four_bytes[100];

	}packet_i;

	//�ļ���Ϣ
	typedef struct file
	{
		unsigned long long length;
		char filename[40];
	}file_info;

	//ÿ�η��͵���Ϣ   ��ͨ��Ϣ/�ļ���Ϣ/������Ϣ������
	struct message
	{
		char type;            //PUBLIC  PRIVATE UPLOAD .....
		char fileFlag;      //FILE_INFO  FILE_RECV  FILE_SEND
		char toWho[10];		//������
		char fromWho[10];   //���ķ�
		char mesg[950];          //���ļ�����ͨ��Ϣ�ڳ�һ���ڴ���

	};

	//�ͻ�������֤ʱ���ͻ������ֺͼ���õ��Ľ������ýṹ����
	struct USER_CONFIRM
	{
		char name[10];
		int result;
	};

	//�����û���ϸ��Ϣ
	struct USER
	{
		char name[10];
		short isOnline;
		int passwd;
		SOCKET sock;
		CFile file_open;
		char ip[20];                    //IP
		int port;                        //�û��˿ں�
		unsigned long long send_total;  //�ѷ����ļ���С
		unsigned long long send_length; //�����ļ��Ĵ�С
		unsigned long long recv_total;   //�ѽ����ļ���С
		unsigned long long recv_length;  //�����ļ��Ĵ�С
	}users[N];

	//�����û�������״̬
	struct online_user
		{
			char name[10];
			short isOnline;
			char ip[20];
			int port;

		}all_users[N];

	


	char *tmp_buf;   //����ʱ�ļ�����
	CFile tmp_file;                //�ļ�
	int next_state;                 //������֤�� ��һ״̬
	CButton m_buttonInit;            //��ʼ������������
	CButton m_buttonDisconnect;     //�Ͽ�����������
	int m_editPort;                //�˿ں�
	afx_msg void OnInitButton();             //��ʼ��������
	afx_msg void OnDisconnectButton();       //�Ͽ�������
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);  //��Ϣ����
	int send_commen_message(int type, struct message mesg_recv);   //������Ϣ    ������Ϣ/˽��
	int check_file_exist(file_info, SOCKET);    //����ļ��Ƿ����
	int file_ready_recv(SOCKET s , file_info);               //��ʼ�����ļ�

	int file_recv_bg(SOCKET, struct message);     //�����ļ�
	int file_send_bg(SOCKET);                   //�����ļ�
	int check_online_users(SOCKET s);           //��ѯ�����û�
};
