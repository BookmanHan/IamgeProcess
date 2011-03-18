// DialogLinTran.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DialogLinTran.h"

#include "MainFrm.h"

// CDialogLinTran 对话框

IMPLEMENT_DYNAMIC(CDialogLinTran, CDialog)

CDialogLinTran::CDialogLinTran(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogLinTran::IDD, pParent)
{

}

CDialogLinTran::~CDialogLinTran()
{
}

void CDialogLinTran::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtFa);
	DDX_Control(pDX, IDC_EDIT2, m_edtFb);
}


BEGIN_MESSAGE_MAP(CDialogLinTran, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialogLinTran::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDialogLinTran 消息处理程序

double StrToDouble(CString str)
{
	double sumA = 0;
	double sumB = 0;
	bool tagA = true;
	int i = 0;

	if (str.GetAt(i) == '-')
	{
		i++;
		tagA = false;
	}

	for(i=0;i<str.GetLength();i++)
	{
		if (str.GetAt(i) == '.')
		{
			break;
		}

		if (str.GetAt(i) < '0' || str.GetAt(i) > '9')
			continue;

		sumA = sumA * 10.0 + str.GetAt(i) - '0';
	}

	for(int j = str.GetLength() - 1;j>=0;j--)
	{
		if (str.GetAt(j) == '.')
		{			
			break;
		}

		if (str.GetAt(j) < '0' || str.GetAt(j) > '9')
			continue;

		sumB = (sumB + str.GetAt(j) - '0') / 10.0;
	}

	if (tagA == false)
	{
		return -(sumA + sumB);
	}
	else
	{
		return sumA + sumB;
	}
	
}

void CDialogLinTran::OnBnClickedButton1()
{
	CString strFa;
	CString strFb;

	m_edtFa.GetWindowText(strFa);
	m_edtFb.GetWindowText(strFb);

	double dFa = StrToDouble(strFa);
	double dFb = StrToDouble(strFb);

	CMainFrame* mf = (CMainFrame*)AfxGetMainWnd();
	mf->biDoc.LinTran(dFa,dFb);
	mf->GetActiveView()->Invalidate();
}
