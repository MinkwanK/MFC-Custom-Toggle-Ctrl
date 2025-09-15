// TogglePanel.cpp: 구현 파일
//

#include "pch.h"
#include "TogglePanel.h"


// CTogglePanel

IMPLEMENT_DYNAMIC(CTogglePanel, CWnd)

CTogglePanel::CTogglePanel()
{
	m_pToggleBoard = nullptr;
}

CTogglePanel::~CTogglePanel()
{

}

BOOL CTogglePanel::Init(CWnd* pParent, TOGGLE_ICON_TYPE eIconType)
{
	if (!GetSafeHwnd())
	{
		CWnd* parent = pParent ? pParent : GetParent();
		if (!parent) return FALSE;

		DWORD style = WS_CHILD | WS_VISIBLE
			| WS_CLIPCHILDREN | WS_CLIPSIBLINGS;					//깜빡임, 덮어그리기 감소
		DWORD exStyle = 0;// WS_EX_CLIENTEDGE;
		const UINT ID_BOARD = 1000; // 임의의 고유 ID
		CRect rcNull; rcNull.SetRectEmpty();
		BOOL ok = CreateEx(exStyle,
			AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW)),
			_T(""), // 윈도우 이름(타이틀) 필요 없음
			style,
			rcNull,
			parent,
			ID_BOARD);

		if (!ok) return FALSE;

		m_eIconType = eIconType;

		if (!m_pToggleBoard)
		{
			m_pToggleBoard = new CToggleBoard;
			m_pToggleBoard->Init(this);
		}
	}
	m_bInit = true;
	AdjustLayout();
	return TRUE;
}

void CTogglePanel::AddToggleTitle(CString sTitle, int iToggleHeight, int iChildHeight, CFont* pFont, CWnd* pChildDlg, UINT uiIcon, UINT uiIconInactive)
{
	HICON hIcon = AfxGetApp()->LoadIcon(uiIcon);
	if (!hIcon) return;

	HICON hIconInactive = AfxGetApp()->LoadIcon(uiIconInactive);
	if (!hIconInactive) return;

	TOGGLE_ICON_INFO icon = {};
	icon.hIcon = hIcon;
	icon.hIconInActive = hIconInactive;
	m_aToggleIcon.Add(icon);

	if (m_pToggleBoard && ::IsWindow(m_pToggleBoard->GetSafeHwnd()))
	{
		m_pToggleBoard->AddToggleTitle(sTitle, iToggleHeight, iChildHeight, pFont, pChildDlg);
	}
	Invalidate();
}

void CTogglePanel::AdjustLayout()
{
	if (!m_bInit) return;

	CRect rc; GetClientRect(&rc);
	constexpr int iHalf = 5;
	constexpr int iMargin = 10;
	constexpr int iSize = 20;

	int iTitleHeight = 0; // TOGGLE_ICON_TYPE::NONE
	if (m_eIconType == TOGGLE_ICON_TYPE::USE_16) iTitleHeight = 16;
	else if (m_eIconType == TOGGLE_ICON_TYPE::USE_24) iTitleHeight = 24;
	else if (m_eIconType == TOGGLE_ICON_TYPE::USE_32) iTitleHeight = 32;
	iTitleHeight += 4;// 상단(2), 하단(2)

	CRect rcBoard = rc;
	//rcBoard.top = rc.top + (rc.Height() * (TOGGLE_BOARD_ICON_RATIO * 0.01));
	rcBoard.top = rc.top + iTitleHeight;
	if (m_pToggleBoard && ::IsWindow(m_pToggleBoard->GetSafeHwnd()))
	{
		m_pToggleBoard->MoveWindow(rcBoard);
	}
}


BEGIN_MESSAGE_MAP(CTogglePanel, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CTogglePanel 메시지 처리기

void CTogglePanel::OnPaint()
{
	CPaintDC dc(this);

	constexpr int iHalf = 5;
	constexpr int iMargin = 10;

	// 아이콘 영역
	CRect rc, rcIcon; GetClientRect(&rc);

	int iIconHeight = 0; // TOGGLE_ICON_TYPE::NONE
	if (m_eIconType == TOGGLE_ICON_TYPE::USE_16) iIconHeight = 16;
	else if (m_eIconType == TOGGLE_ICON_TYPE::USE_24) iIconHeight = 24;
	else if (m_eIconType == TOGGLE_ICON_TYPE::USE_32) iIconHeight = 32;
	iIconHeight += 4;// 상단(2), 하단(2)

	rcIcon = CRect(rc.left, rc.top, rc.right, rc.top + iIconHeight);

	int iIconSize = 0;

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap memBmp;
	memBmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&memBmp);

	memDC.FillSolidRect(&rc, ::GetSysColor(COLOR_BTNFACE));

	//아이콘 그리기
	if (m_eIconType != TOGGLE_ICON_TYPE::NONE)
	{
		if (m_eIconType == TOGGLE_ICON_TYPE::USE_16) iIconSize = 16;
		else if (m_eIconType == TOGGLE_ICON_TYPE::USE_24) iIconSize = 24;
		else if (m_eIconType == TOGGLE_ICON_TYPE::USE_32) iIconSize = 32;

		int x = rcIcon.left + iMargin;
		int y = ((rcIcon.top + rcIcon.Height()) / 2) - (iIconSize / 2);

		CArray< CToggleItem*>& apToggleItem = m_pToggleBoard->GetToggleItemArray();
		HICON hIcon = NULL;
		for (int i = 0; i < apToggleItem.GetCount(); i++)
		{
			auto pToggle = apToggleItem.GetAt(i);
			if (!pToggle) continue;

			if (i >= m_aToggleIcon.GetCount()) continue;
			TOGGLE_ICON_INFO& icon = m_aToggleIcon.GetAt(i);
			if (pToggle->GetHideToggle())
			{
				hIcon = icon.hIconInActive;
				if (!hIcon) continue;
				::DrawIconEx(memDC.GetSafeHdc(), x, y, hIcon, iIconSize, iIconSize, 0, nullptr, DI_NORMAL);
			}
			else
			{
				hIcon = icon.hIcon;
				if (!hIcon) continue;
				::DrawIconEx(memDC.GetSafeHdc(), x, y, hIcon, iIconSize, iIconSize, 0, nullptr, DI_NORMAL);
			}
			CRect rcIcon(x, y, x + iIconSize, y + iIconSize);
			icon.rcIcon = rcIcon;

			x += iIconSize + iMargin;	//아이콘 오프셋 증가
		}
	}
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
	if (pOldBmp) memDC.SelectObject(pOldBmp);
	memBmp.DeleteObject();
	memDC.DeleteDC();
}


void CTogglePanel::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CTogglePanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_pToggleBoard) return;

	const int iIconCount = m_aToggleIcon.GetCount();
	CArray<CToggleItem*>& apToggle = m_pToggleBoard->GetToggleItemArray();
	const int iToggleCount = apToggle.GetCount();
	for (int i = 0; i < iIconCount; i++)
	{
		const auto rc = m_aToggleIcon.GetAt(i).rcIcon;
		if (rc.PtInRect(point) && (i < iToggleCount))
		{
			auto pTitle = apToggle.GetAt(i);
			if (pTitle) pTitle->SetHideToggle(!pTitle->GetHideToggle());
			break;
		}
	}
	m_pToggleBoard->DeployToggle();
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}


void CTogglePanel::OnDestroy()
{
	CWnd::OnDestroy();

	if (m_pToggleBoard && m_pToggleBoard->GetSafeHwnd())
	{
		m_pToggleBoard->DestroyWindow();
		delete m_pToggleBoard;
		m_pToggleBoard = nullptr;
	}
}
