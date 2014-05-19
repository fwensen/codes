// DialogUser.cpp : 实现文件
//

#include "stdafx.h"
#include "server.h"
#include "DialogUser.h"
#include "afxdialogex.h"


// DialogUser 对话框

IMPLEMENT_DYNAMIC(DialogUser, CDialogEx)

DialogUser::DialogUser(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogUser::IDD, pParent)
	, m_UserName(_T(""))
{

}

DialogUser::~DialogUser()
{
}

void DialogUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USERNAME_EDIT, m_UserName);
}


BEGIN_MESSAGE_MAP(DialogUser, CDialogEx)
END_MESSAGE_MAP()


// DialogUser 消息处理程序
