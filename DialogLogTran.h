#pragma once
#include "afxwin.h"


// CDialogLogTran 对话框

class CDialogLogTran : public CDialog
{
	DECLARE_DYNAMIC(CDialogLogTran)

public:
	CDialogLogTran(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogLogTran();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtFc;
public:
	afx_msg void OnBnClickedButton1();
};
