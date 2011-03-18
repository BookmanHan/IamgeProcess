#pragma once


// CDialogFreqIdealLPF 对话框

class CDialogFreqIdealLPF : public CDialog
{
	DECLARE_DYNAMIC(CDialogFreqIdealLPF)

public:
	CDialogFreqIdealLPF(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogFreqIdealLPF();

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
