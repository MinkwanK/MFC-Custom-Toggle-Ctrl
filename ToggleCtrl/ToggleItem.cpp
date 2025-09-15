//CToggleTitle.cpp: 구현 파일


#include "pch.h"
#include "ToggleItem.h"
#include "ToggleBoard.h"
#include "resource.h"

 //CToggleTitle

IMPLEMENT_DYNAMIC(CToggleItem, CStatic)

CToggleItem::CToggleItem()
{

}

CToggleItem::~CToggleItem()
{
	for (HICON& h : m_hToggle)
	{
		if (h) { ::DestroyIcon(h); h = nullptr; }
	}
}

BOOL CToggleItem::Init(CString sTitle, CFont* pFont, CWnd* pChildDlg, CWnd* pParent)
{
	if (!GetSafeHwnd())
	{
		if (!pParent) return FALSE;
		if (!pChildDlg) return FALSE;

		DWORD style = WS_CHILD | WS_VISIBLE;   // Child
		DWORD exStyle = 0;

		CRect rcNull; rcNull.SetRectEmpty();
		BOOL ok = Create(NULL, style, rcNull, pParent);
		if (!ok) return FALSE;
	}
	m_pFont = pFont;

	InitIcon(IDI_ICON_DOWN, IDI_ICON_DOWN, IDI_ICON_UP, IDI_ICON_UP, 16, 16);
	m_pChild = pChildDlg;
	m_pParent = (CToggleBoard*)pParent;
	m_sTitle = sTitle;
	m_bInit = true;
	return TRUE;
}

void CToggleItem::InitIcon(UINT uiToggleUp, UINT uiToggleUpSelect, UINT uiToggleDown, UINT uiToggleDownSelect, int iCx, int iCy)
{
	auto SetIcon = [&](int iIndex, UINT uiToggle)
	{
		m_hToggle[iIndex] = (HICON)::LoadImage(
			AfxGetInstanceHandle(),
			MAKEINTRESOURCE(uiToggle),
			IMAGE_ICON,
			iCx, iCy,
			LR_DEFAULTCOLOR
		);
	};
	SetIcon(0, uiToggleUp);
	SetIcon(1, uiToggleUpSelect);
	SetIcon(2, uiToggleDown);
	SetIcon(3, uiToggleDownSelect);
}

void CToggleItem::SetNo(int iNo)
{
	m_iNo = iNo;
	m_pChild->PostMessage(UM_SET_TOGGLE_CHILD_NO, (WPARAM)m_iNo, (LPARAM)this);
}

void CToggleItem::NotifyChildChangedHeight(int iChildHeight)
{
	SetChildHeight(iChildHeight);
	if (GetParent() && ::IsWindow(GetParent()->GetSafeHwnd()))
	{
		m_pParent->DeployToggle();
	}
}

void CToggleItem::DrawAgain()
{
	DrawChild();
	Invalidate();
}

void CToggleItem::DrawChild()
{
	if (m_pChild && ::IsWindow(m_pChild->GetSafeHwnd()))
	{
		m_pChild->Invalidate();
	}
}


BEGIN_MESSAGE_MAP(CToggleItem, CStatic)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_MOVE()
END_MESSAGE_MAP()

//CToggleTitle 메시지 처리기

void CToggleItem::OnPaint()
{
	CPaintDC dc(this);

	CRect rc; GetClientRect(&rc);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap memBmp;
	memBmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&memBmp);

	//배경
	memDC.FillSolidRect(&rc, m_crBack);

	//글꼴 및 텍스트
	CFont* pOldFont = nullptr;
	if (m_pFont && ::IsWindow(m_hWnd)) {
		pOldFont = memDC.SelectObject(m_pFont);
	}
	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(m_crText);

	//테두리 
	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = memDC.SelectObject(&pen);
	memDC.SelectStockObject(NULL_BRUSH);
	memDC.Rectangle(&rc);
	memDC.SelectObject(pOldPen);

	//텍스트 
	const int iHalf = 5;
	CRect rcInfl = rc;
	rcInfl.InflateRect(-iHalf, -iHalf);

	CRect rcText = rcInfl;
	rcText.right = rcInfl.left + (rcInfl.Width() * 70) / 100; // 70%
	memDC.DrawText(m_sTitle, &rcText,
		DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);


	const int iCx = 16, iCy = 16;
	//int x = rcText.right + ((rcInfl.Width() - rcText.Width()) - iCx) / 2;
	int x = rcInfl.Width() - iCx - iHalf;
	int y = rcInfl.top + (rcInfl.Height() - iCy) / 2;

	m_rcToggle.SetRect(x, y, x + iCx, y + iCy);

	// 아이콘 
	HICON hIcon = m_bToggleDown ? m_hToggle[2] : m_hToggle[0];
	::DrawIconEx(memDC.GetSafeHdc(), x, y, hIcon, iCx, iCy, 0, nullptr, DI_NORMAL);

	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
	if (pOldFont) memDC.SelectObject(pOldFont);
	if (pOldBmp) memDC.SelectObject(pOldBmp);
	memBmp.DeleteObject();
	memDC.DeleteDC();
	pen.DeleteObject();
}

CRect CToggleItem::GetRect()
{
	CRect rc; GetClientRect(&rc);
	return rc;
}

int CToggleItem::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	ModifyStyle(0, SS_NOTIFY);	//클릭 받기
	return 0;
}

void CToggleItem::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_rcToggle.PtInRect(point))
	{
		m_bToggleDown = !m_bToggleDown;

		if (m_bToggleDown)
		{
			DisplayChild();
		}
		else
		{
			m_pChild->SetWindowPos(nullptr, 0, 0, 0, 0,
				SWP_HIDEWINDOW | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
		}
		GetParent()->PostMessage(UM_TOGGLE_CHILD, m_iNo, m_iChildHeight);
	}
	Invalidate();
	CStatic::OnLButtonDown(nFlags, point);
}

//Child는 부모 좌표를 기준
void CToggleItem::DisplayChild()
{
	CRect rc;
	GetWindowRect(&rc); m_pParent->ScreenToClient(&rc);
	int iChildHeight = rc.Height(); // 현재 토글 높이 = 타이틀 영역

	//너무 작으면 보기가 불편하다.
	if (GetChildHeight() > iChildHeight)
		iChildHeight = GetChildHeight();


	rc.top = rc.bottom;
	rc.bottom = rc.top + iChildHeight;

	m_pChild->SetWindowPos(&CWnd::wndTop, rc.left, rc.top, rc.Width(), rc.Height(),
		SWP_NOACTIVATE | SWP_SHOWWINDOW);
	//순번과 높이를 넣어 n번째 토글 타이틀이 자식을 h 높이만큼 늘렸다고 알린다.
	m_iChildHeight = rc.Height();
}

bool CToggleItem::IsChildVisible()
{
	if (m_pChild)
	{
		return m_pChild->IsWindowVisible();
	}
	return false;
}

void CToggleItem::InitToggle()
{
	m_bToggleDown = false;
}

void CToggleItem::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
}

void CToggleItem::OnMove(int x, int y)
{
	CStatic::OnMove(x, y);
}
