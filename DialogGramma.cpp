// DialogGramma.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DialogGramma.h"

#include "MainFrm.h"

// CDialogGramma 对话框

IMPLEMENT_DYNAMIC(CDialogGramma, CDialog)

CDialogGramma::CDialogGramma(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGramma::IDD, pParent)
{

}

CDialogGramma::~CDialogGramma()
{
}

void CDialogGramma::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtEsp);
	DDX_Control(pDX, IDC_EDIT2, m_edtGrammer);
}


BEGIN_MESSAGE_MAP(CDialogGramma, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialogGramma::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDialogGramma 消息处理程序
double StrToDouble(CString str);

void CDialogGramma::OnBnClickedButton1()
{
	CString strEsp;
	CString strGra;

	m_edtEsp.GetWindowText(strEsp);
	m_edtGrammer.GetWindowText(strGra);

	double dFa = StrToDouble(strEsp);
	double dFb = StrToDouble(strGra);

	CMainFrame* mf = (CMainFrame*)AfxGetMainWnd();
	mf->biDoc.GammerTran(dFb,dFa);

	mf->GetActiveView()->Invalidate();
}
