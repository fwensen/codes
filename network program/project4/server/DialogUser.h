#pragma once


// DialogUser 对话框

class DialogUser : public CDialogEx
{
	DECLARE_DYNAMIC(DialogUser)

public:
	DialogUser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DialogUser();

// 对话框数据
	enum { IDD = IDD_USER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_UserName;
};
