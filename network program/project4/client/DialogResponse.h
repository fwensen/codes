#pragma once


// DialogResponse 对话框

class DialogResponse : public CDialogEx
{
	DECLARE_DYNAMIC(DialogResponse)

public:
	DialogResponse(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DialogResponse();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_response;
};
