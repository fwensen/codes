#pragma once


// DialogUser �Ի���

class DialogUser : public CDialogEx
{
	DECLARE_DYNAMIC(DialogUser)

public:
	DialogUser(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogUser();

// �Ի�������
	enum { IDD = IDD_USER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_UserName;
};
