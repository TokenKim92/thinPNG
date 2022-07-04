// thinPNGDlg.cpp : implementation file

#include "pch.h"
#include "framework.h"
#include "thinPNG.h"
#include "thinPNGDlg.h"
#include "afxdialogex.h"
#include "ResizeEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CthinPNGDlg dialog
CThinPNGDlg::CThinPNGDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_THINPNG_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON);

    m_resizeWidth = m_resizeHeight = 500;
    m_textColor = RGB(220, 220, 220);
}

void CThinPNGDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CThinPNGDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CThinPNGDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CThinPNGDlg::OnBnClickedCancel)
    ON_WM_DROPFILES()
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_SIZE_EDIT_BTN, &CThinPNGDlg::OnBnClickedSizeEditBtn)
    ON_WM_DRAWITEM()
END_MESSAGE_MAP()

// CthinPNGDlg message handlers
BOOL CThinPNGDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    GetDlgItem(IDC_MAIN_RECT)->GetWindowRect(m_gridRect);
    ScreenToClient(m_gridRect);

    mh_gridPen = ::CreatePen(PS_DOT, 1, RGB(220, 220, 220));
    mh_font = ::CreateFont(18, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Abadi");

    SetBackgroundColor(RGB(30,30,30));
    LONG extendedStyle = GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
    SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, extendedStyle | WS_EX_LAYERED );
    ::SetLayeredWindowAttributes(GetSafeHwnd(), 0, 230, LWA_ALPHA);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CThinPNGDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    if (IsIconic()) {
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    } else {
        HPEN h_oldPen = (HPEN)dc.SelectObject(mh_gridPen);
        HBRUSH h_oldBrush = (HBRUSH) dc.SelectStockObject(NULL_BRUSH);
        int oldBkMode = dc.SetBkMode(TRANSPARENT);
        COLORREF oldTextColor = dc.SetTextColor(m_textColor);
        HFONT h_oldFont = (HFONT)dc.SelectObject(mh_font);

        dc.RoundRect(m_gridRect, CPoint(10, 10));
        dc.DrawText(L"Drop your PNG or JPEF files here!", m_gridRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        
        dc.SelectObject(h_oldFont);
        dc.SetTextColor(oldTextColor);
        dc.SetBkMode(oldBkMode);
        dc.SelectObject(h_oldPen);
        dc.SelectObject(h_oldBrush);
        
        //CDialogEx::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CThinPNGDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CThinPNGDlg::OnBnClickedOk()
{
    // CDialogEx::OnOK();
}

void CThinPNGDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    CDialogEx::OnCancel();
}

void CThinPNGDlg::OnDestroy()
{
    CDialogEx::OnDestroy();

    ::DeleteObject(mh_font);
    ::DeleteObject(mh_gridPen);

    if (!m_orgImage.IsNull()) m_orgImage.Destroy();
    if (!m_resizeImage.IsNull()) m_resizeImage.Destroy();
}

void CThinPNGDlg::OnDropFiles(HDROP hDropInfo)
{
    int count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

    //if (count > 20) {
    //    MessageBox(L"Up to 20 images", L"File Error", MB_OK | MB_ICONWARNING);
    //    return;
    //}

    wchar_t temp_path[MAX_PATH] = { 0, };
    int formatIndex;
    UINT8 m_dialogFlag = 0;

    for (int i = 0; i < count; i++) {
        DragQueryFile(hDropInfo, i, temp_path, MAX_PATH);
        formatIndex = isPngOrJpegFormat(temp_path);

        if (GetFileAttributes(temp_path) & FILE_ATTRIBUTE_DIRECTORY || formatIndex == -1) {
            if (!m_dialogFlag) {
                MessageBox(L"Files that are not in PNG or JPEG format are ignored.", L"Invalid file format warning", MB_OK | MB_ICONWARNING);
                m_dialogFlag = 1;
            }
        } else {
            resizeImge(temp_path, formatIndex);
        }
    }

    CDialogEx::OnDropFiles(hDropInfo);
}

void CThinPNGDlg::resizeImge(wchar_t* ap_path, int a_formatIndex)
{
    if (!m_orgImage.IsNull()) m_orgImage.Destroy();
    m_orgImage.Load(ap_path);

    if (!m_resizeImage.IsNull()) m_resizeImage.Destroy();
    m_resizeHeight = (int)((double)m_resizeWidth * m_orgImage.GetHeight() / m_orgImage.GetWidth());
    m_resizeImage.Create(m_resizeWidth, m_resizeHeight, 24);

    HDC dc = m_resizeImage.GetDC();
    ::SetStretchBltMode(dc, COLORONCOLOR);

    m_orgImage.StretchBlt(dc, 0, 0, m_resizeWidth, m_resizeHeight);
    memcpy(ap_path + a_formatIndex, L"-Resize.png", 22);

    m_resizeImage.Save(ap_path, Gdiplus::ImageFormatPNG);
    m_resizeImage.ReleaseDC();
}

int CThinPNGDlg::isPngOrJpegFormat(const wchar_t *ap_path)
{
    const wchar_t *p = ap_path;
    while (*p) {
        if (*p == '.') break;
        p++;
    }

    if (!wcscmp(p, L".png") || !wcscmp(p, L".PNG") ||
        !wcscmp(p, L".jpg") || !wcscmp(p, L".JPG") ||
        !wcscmp(p, L".jpeg") || !wcscmp(p, L".JPEG"))  return p - ap_path;
    else return -1;
}

void CThinPNGDlg::OnBnClickedSizeEditBtn()
{
    ResizeEditDlg ins_dlg;
    ins_dlg.setPixelSize(m_resizeWidth);

    if (IDOK == ins_dlg.DoModal()) {
        CSize size = ins_dlg.getPixelSize();
        m_resizeWidth = size.cx;
        m_resizeHeight = size.cy;
    }
}


void CThinPNGDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpds)
{
    if (nIDCtl == IDC_SIZE_EDIT_BTN) {
        CDC *p_dc = CDC::FromHandle( lpds->hDC);
        CRect r(lpds->rcItem);

        CString str;
        GetDlgItemText(IDC_SIZE_EDIT_BTN, str);

        p_dc->FillSolidRect(&lpds->rcItem, RGB(48, 48, 48));

        if (lpds->itemState & ODS_SELECTED) {
            p_dc->Draw3dRect(&lpds->rcItem, RGB(30, 30, 30), RGB(58, 58, 58));
            
        }
        else {
            r -= CPoint(1, 1);
            p_dc->Draw3dRect(&lpds->rcItem, RGB(68, 68, 68), RGB(88, 88, 88));
        }

        p_dc->SetBkMode(TRANSPARENT);
        p_dc->SetTextColor(m_textColor);
        p_dc->DrawText(str, r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    CDialogEx::OnDrawItem(nIDCtl, lpds);
}
