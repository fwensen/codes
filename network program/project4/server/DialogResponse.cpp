// DialogResponse.cpp : 实现文件
//

#include "stdafx.h"
#include "server.h"
#include "DialogResponse.h"
#include "afxdialogex.h"


// CDialogResponse 对话框

IMPLEMENT_DYNAMIC(CDialogResponse, CDialogEx)

CDialogResponse::CDialogResponse(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogResponse::IDD, pParent)
	, m_response(_T(""))
{

}

CDialogResponse::~CDialogResponse()
{
}

void CDialogResponse::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RESPONSE_STATIC, m_response);
}


BEGIN_MESSAGE_MAP(CDialogResponse, CDialogEx)
END_MESSAGE_MAP()


// CDialogResponse 消息处理程序
