#pragma once
#include "afxwin.h"


// CDialogLinTran 对话框

class CDialogLinTran : public CDialog
{
	DECLARE_DYNAMIC(CDialogLinTran)

public:
	CDialogLinTran(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogLinTran();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	CEdit m_edtFa;
public:
	CEdit m_edtFb;
};
