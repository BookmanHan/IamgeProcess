// DialogHist.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "DialogHist.h"


// CDialogHist 对话框

IMPLEMENT_DYNAMIC(CDialogHist, CDialog)

CDialogHist::CDialogHist(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogHist::IDD, pParent)
{

}

CDialogHist::~CDialogHist()
{
}

void CDialogHist::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogHist, CDialog)
	ON_WM_PAINT()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// CDialogHist 消息处理程序

void CDialogHist::SetDisplayHistData(double * argrealHist,int argnHist)
{
	memcpy(m_realHist,argrealHist,256 * sizeof(double));
	m_nHist = argnHist;
}

void CDialogHist::OnPaint()
{
	if (m_realHist == NULL)
		return;

	CPaintDC dc(this);

	int nWidth = 256;
	int nMargin = 30;

	dc.Rectangle(nMargin - 5,nMargin - 5, nMargin + nWidth + 18,nWidth + nMargin +25);


	dc.MoveTo(nMargin,nWidth + nMargin);
	dc.LineTo(nMargin,nMargin);
	
	dc.MoveTo(nMargin,nWidth +nMargin);
	dc.LineTo(nWidth + nMargin,nWidth + nMargin);

	CString strShow;
	for(int i=0;i<256;i+=50)
	{
		strShow.Format(L"%d",i);
		dc.TextOut(int(nMargin + nWidth/256.0*i),nMargin + nWidth + 3,strShow);
	}

	for(int i=0;i<nWidth;i++)
	{
		dc.MoveTo(nMargin + i,nMargin + nWidth);
		dc.LineTo(nMargin+i,nMargin + nWidth*(1 - m_realHist[i]*5));
	}
}

void CDialogHist::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	delete this;
	// TODO: 在此处添加消息处理程序代码
}
