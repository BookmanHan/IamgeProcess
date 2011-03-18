#pragma once
#include "afxwin.h"


// CDialogThershold 对话框

class CDialogThershold : public CDialog
{
	DECLARE_DYNAMIC(CDialogThershold)

public:
	CDialogThershold(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogThershold();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtTV;
public:
	afx_msg void OnBnClickedButton1();
};
