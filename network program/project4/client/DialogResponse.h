#pragma once


// DialogResponse �Ի���

class DialogResponse : public CDialogEx
{
	DECLARE_DYNAMIC(DialogResponse)

public:
	DialogResponse(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogResponse();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_response;
};
