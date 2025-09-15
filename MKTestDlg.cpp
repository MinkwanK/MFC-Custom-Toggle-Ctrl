
// MKTestDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MKTest.h"
#include "MKTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMKTestDlg 대화 상자



CMKTestDlg::CMKTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MKTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMKTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMKTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMKTestDlg 메시지 처리기

BOOL CMKTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	Init();
	AdjustLayout();
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMKTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMKTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMKTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMKTestDlg::Init()
{
	CRect rc;
	GetClientRect(&rc);
	rc.left = 100;
	rc.top = 0;
	rc.right = 300;
	rc.bottom = rc.bottom - 5;
	constexpr int iTitleHeight = 24;
	m_panel.Init(this, TOGGLE_ICON_TYPE::USE_24);
	CString sValue = _T("TEST 1");
	m_sample.Create(IDD_DIALOG_SAMPLE, m_panel.GetBoard());
	m_sample.ShowWindow(SW_HIDE);
	CFont* pFont = this->GetFont();
	m_panel.AddToggleTitle(sValue, iTitleHeight, 60,  pFont, &m_sample, IDI_ICON_UP, IDI_ICON_UP_DISABLE);

	sValue.Format(_T("TEST 2"));
	m_sample2.Create(IDD_DIALOG_SAMPLE, m_panel.GetBoard());
	m_sample2.ShowWindow(SW_HIDE);
	m_panel.AddToggleTitle(sValue, iTitleHeight, 60, pFont, &m_sample2, IDI_ICON_DOWN, IDI_ICON_DOWN_DISABLE);

	sValue.Format(_T("TEST 3"));
	m_sample3.Create(IDD_DIALOG_SAMPLE, m_panel.GetBoard());
	m_sample3.ShowWindow(SW_HIDE);
	m_panel.AddToggleTitle(sValue, iTitleHeight, 60, pFont, &m_sample3, IDI_ICON_UP, IDI_ICON_UP_DISABLE);

	m_bInit = TRUE;
}

void CMKTestDlg::AdjustLayout()
{
	if (m_bInit)
	{
		constexpr int iHalf = 5;
		constexpr int iSize = 20;
		constexpr int iMargin = 10;
		CRect rc; GetClientRect(&rc);
		rc.InflateRect(-iSize, -(iSize));
		rc.right = rc.left + (rc.Width() * 0.5);
		rc.bottom = rc.bottom - iHalf;
		m_panel.SetWindowPos(nullptr, rc.left, rc.top, rc.Width(), rc.Height(), 0);
		//m_board.DeployToggle();
	}
}



void CMKTestDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_bInit)
	{
		AdjustLayout();
		//m_board.DeployToggle();
	}
}


void CMKTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_panel && m_panel.GetSafeHwnd())
	{
		m_panel.DestroyWindow();
	}
}
