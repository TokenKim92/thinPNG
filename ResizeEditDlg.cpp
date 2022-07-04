// ResizeEditDlg.cpp : implementation file

#include "pch.h"
#include "thinPNG.h"
#include "ResizeEditDlg.h"
#include "afxdialogex.h"

// ResizeEditDlg dialog
IMPLEMENT_DYNAMIC(ResizeEditDlg, CDialogEx)

ResizeEditDlg::ResizeEditDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RESIZE_DIALOG, pParent)
{
	m_resizeWidth = 0;
}

ResizeEditDlg::~ResizeEditDlg()
{
}

void ResizeEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ResizeEditDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ResizeEditDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CthinPNGDlg message handlers
BOOL ResizeEditDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

	SetDlgItemInt(IDC_CX_EDIT, m_resizeWidth);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void ResizeEditDlg::setPixelSize(int a_cx)
{
	m_resizeWidth = a_cx;
}

int ResizeEditDlg::getPixelSize()
{
	return m_resizeWidth;
}

void ResizeEditDlg::OnBnClickedOk()
{
	m_resizeWidth = GetDlgItemInt(IDC_CX_EDIT);

	CDialogEx::OnOK();
}
