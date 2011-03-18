#pragma once
#include "afxwin.h"


// CDialogGramma 对话框

class CDialogGramma : public CDialog
{
	DECLARE_DYNAMIC(CDialogGramma)

public:
	CDialogGramma(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogGramma();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	CEdit m_edtEsp;
public:
	CEdit m_edtGrammer;
};
