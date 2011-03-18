#pragma once


// CDialogHist 对话框

class CDialogHist : public CDialog
{
	DECLARE_DYNAMIC(CDialogHist)

public:
	CDialogHist(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogHist();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	double m_realHist[256];
	int m_nHist;

	DECLARE_MESSAGE_MAP()
public:
	void SetDisplayHistData(double * argrealHist,int argnHist);
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnNcDestroy();
};
