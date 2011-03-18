// DialogLogTran.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DialogLogTran.h"

#include "MainFrm.h"

// CDialogLogTran 对话框

IMPLEMENT_DYNAMIC(CDialogLogTran, CDialog)

CDialogLogTran::CDialogLogTran(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogLogTran::IDD, pParent)
{

}

CDialogLogTran::~CDialogLogTran()
{
}

void CDialogLogTran::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtFc);
}


BEGIN_MESSAGE_MAP(CDialogLogTran, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialogLogTran::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDialogLogTran 消息处理程序

double StrToDouble(CString str);

void CDialogLogTran::OnBnClickedButton1()
{
	CString strFc;
	m_edtFc.GetWindowText(strFc);

	double dFc = StrToDouble(strFc);

	CMainFrame* mf = (CMainFrame*)AfxGetMainWnd();
	mf->biDoc.LogTran(dFc);
	mf->GetActiveView()->Invalidate();
}
