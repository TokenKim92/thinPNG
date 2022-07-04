#pragma once
// ResizeEditDlg dialog

class ResizeEditDlg : public CDialogEx
{
protected:
	int m_resizeWidth;

	DECLARE_DYNAMIC(ResizeEditDlg)

public:
	ResizeEditDlg(CWnd* pParent = nullptr);   
	virtual ~ResizeEditDlg();

	void setPixelSize(int a_cx);
	int getPixelSize();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADJUST_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
