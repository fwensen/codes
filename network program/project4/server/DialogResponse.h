#pragma once


// CDialogResponse 对话框

class CDialogResponse : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogResponse)

public:
	CDialogResponse(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogResponse();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_response;
};
