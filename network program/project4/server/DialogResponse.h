#pragma once


// CDialogResponse �Ի���

class CDialogResponse : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogResponse)

public:
	CDialogResponse(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogResponse();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_response;
};
