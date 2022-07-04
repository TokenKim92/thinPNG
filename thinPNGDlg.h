// thinPNGDlg.h : header file
#pragma once

// CThinPNGDlg dialog
class CThinPNGDlg : public CDialogEx
{
protected:
    HPEN mh_gridPen;
    HFONT mh_font;
    COLORREF m_textColor;

    CRect m_gridRect;
    CImage m_resizeImage, m_orgImage;

    int m_resizeWidth, m_resizeHeight;

    // Construction
public:
    CThinPNGDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_THINPNG_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    int CThinPNGDlg::isPngOrJpegFormat(const wchar_t *ap_path);
    void CThinPNGDlg::resizeImge(wchar_t* ap_path, int a_formatIndex);

    // Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnDropFiles(HDROP hDropInfo);
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedSizeEditBtn();
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};
