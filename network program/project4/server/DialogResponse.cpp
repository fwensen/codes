// DialogResponse.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "server.h"
#include "DialogResponse.h"
#include "afxdialogex.h"


// CDialogResponse �Ի���

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


// CDialogResponse ��Ϣ�������
