// DialogResponse.cpp : 实现文件
//

#include "stdafx.h"
#include "client.h"
#include "DialogResponse.h"
#include "afxdialogex.h"


// DialogResponse 对话框

IMPLEMENT_DYNAMIC(DialogResponse, CDialogEx)

DialogResponse::DialogResponse(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogResponse::IDD, pParent)
	, m_response(_T(""))
{

}

DialogResponse::~DialogResponse()
{
}

void DialogResponse::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RESPONSE_STATIC, m_response);
}


BEGIN_MESSAGE_MAP(DialogResponse, CDialogEx)
END_MESSAGE_MAP()


// DialogResponse 消息处理程序
