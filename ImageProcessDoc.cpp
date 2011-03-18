// ImageProcessDoc.cpp : CImageProcessDoc 类的实现
//

#include "stdafx.h"
#include "ImageProcess.h"

#include "ImageProcessDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageProcessDoc

IMPLEMENT_DYNCREATE(CImageProcessDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageProcessDoc, CDocument)
END_MESSAGE_MAP()


// CImageProcessDoc 构造/析构

CImageProcessDoc::CImageProcessDoc()
{
	// TODO: 在此添加一次性构造代码

}

CImageProcessDoc::~CImageProcessDoc()
{
}

BOOL CImageProcessDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CImageProcessDoc 序列化

void CImageProcessDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CImageProcessDoc 诊断

#ifdef _DEBUG
void CImageProcessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProcessDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageProcessDoc 命令
