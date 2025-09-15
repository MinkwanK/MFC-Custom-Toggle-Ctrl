//ToggleTitleBoard.cpp: 구현 파일


#include "pch.h"
#include "ToggleBoard.h"


// CToggleTitleBoard
/*
토클 타이틀을 삽입할 BOARD 역할
스크롤 기능, ICON 삽입
*/
IMPLEMENT_DYNAMIC(CToggleBoard, CWnd)

CToggleBoard::CToggleBoard()
{
	m_iScrollPos = 0;
	m_iTotalHeight = 0;
}

CToggleBoard::~CToggleBoard()
{
	
}

BOOL CToggleBoard::Init(CWnd* pParent)
{
	if (!GetSafeHwnd())
	{
		CWnd* parent = pParent ? pParent : GetParent();
		if (!parent) return FALSE;

		DWORD style = WS_CHILD | WS_VISIBLE
			| WS_CLIPCHILDREN | WS_CLIPSIBLINGS					//깜빡임, 덮어그리기 감소
			| WS_VSCROLL;										//수직 스크롤 켜기
		DWORD exStyle = 0;// WS_EX_CLIENTEDGE;
		const UINT ID_BOARD = 1001; // 임의의 고유 ID
		CRect rcNull; rcNull.SetRectEmpty();
		BOOL ok = CreateEx(exStyle,
			AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW)),
			_T(""), // 윈도우 이름(타이틀) 필요 없음
			style,
			rcNull,
			parent,
			ID_BOARD);

		if (!ok) return FALSE;

		UpdateScrollInfo();
	}

	return TRUE;
}

void CToggleBoard::AddToggleTitle(CString sTitle, int iToggleHeight, int iChildHeight, CFont* pFont, CWnd* pChildDlg)	//폰트 통일(추가)
{
	CToggleItem* pTitle = new CToggleItem;
	if (!pTitle) return;

	const int iCount = m_apToggleItem.GetCount();
	pTitle->Init(sTitle, pFont, pChildDlg, this);
	pTitle->SetNo(iCount);
	pTitle->SetToggleHeight(iToggleHeight);
	pTitle->SetChildHeight(iChildHeight);
	m_apToggleItem.Add(pTitle);
	DeployToggle();
}

void CToggleBoard::ClearToggleTitle()
{
	for (int i = 0; i < m_apToggleItem.GetCount(); ++i)
	{
		auto p = m_apToggleItem[i];
		if (!p) continue;

		CWnd* pChild = p->GetChild();
		if (pChild && ::IsWindow(pChild->GetSafeHwnd()))
		{
			pChild->DestroyWindow();
			pChild = nullptr;
		}

		// 토글 자체 파괴
		p->DestroyWindow();
		delete p;
		p = nullptr;
	}
	m_apToggleItem.RemoveAll();
}

//Toggle Sort and Deploy
void CToggleBoard::DeployToggle()
{
	constexpr int iMargin = 10;
	CRect rc; GetClientRect(&rc);

	// 총 높이 계산
	int iTotal = 0;
	for (int i = 0; i < m_apToggleItem.GetCount(); i++)
	{
		auto pToggle = m_apToggleItem.GetAt(i);
		if (pToggle && !pToggle->GetHideToggle())
		{
			const int iChildHeight = pToggle->IsChildVisible() ? pToggle->GetChildHeight() : 0;
			iTotal += pToggle->GetToggleHeight() + iChildHeight;
		}
	}

	//화면 실제 차지 높이 
	m_iTotalHeight = iTotal;
	UpdateScrollInfo();

	GetClientRect(&rc);

	//컨텐츠 영역
	rc.InflateRect(-iMargin, 0, -iMargin, 0);
	//스크롤이 0이면 iY = 컨텐츠 영역의 최상단
	//스크롤이 커질 수록 Y가 위로 당겨져 화면에 중간 이후의 항목이 보이게 됨.
	int iY = rc.top - m_iScrollPos;
	const int iLeft = rc.left;
	const int iRight = rc.right;

	for (int i = 0; i < m_apToggleItem.GetCount(); i++)
	{
		auto pToggle = m_apToggleItem.GetAt(i);
		if (!pToggle) continue;

		//토글이 숨기기 처리 되어있다면
		if (pToggle->GetHideToggle())
		{
			pToggle->ShowWindow(SW_HIDE);
			if (pToggle->GetChild()) pToggle->GetChild()->ShowWindow(SW_HIDE);
			pToggle->InitToggle();
			continue;
		}
		else
		{
			pToggle->ShowWindow(SW_SHOW);
		}
		//토글들을 위로 당겨주면 중간부터 볼 수 있다. 
		const int iToggleHeight = pToggle->GetToggleHeight();
		CRect rc(iLeft, iY, iRight, iY + iToggleHeight);
		pToggle->MoveWindow(rc);
		const int iChildHeight = pToggle->IsChildVisible() ? pToggle->GetChildHeight() : 0;
		if (pToggle->IsChildVisible()) pToggle->DisplayChild();
		iY += iToggleHeight + iChildHeight;
	}
	Invalidate();
}

BEGIN_MESSAGE_MAP(CToggleBoard, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(UM_TOGGLE_CHILD, CToggleBoard::OnToggleChild)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOVE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//CToggleTitleBoard 메시지 처리기
int CToggleBoard::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

void CToggleBoard::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	DeployToggle();
}

//스크롤 정보 갱신
void CToggleBoard::UpdateScrollInfo()
{
	CRect rc; GetClientRect(&rc);	//보드의 클라이언트 영역 얻기
	const int iPage = max(1, rc.Height()); //한번에 보여지는 페이지 높이
	const int iMaxRange = m_iTotalHeight; //스크롤바 범위 최대값

	SCROLLINFO si = { sizeof(SCROLLINFO) };		//스크롤바 설정
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;	//이번에 설정할 것이 범위/페이지/현재위치
	si.nMin = 0;
	si.nMax = max(0, iMaxRange - 1);
	si.nPage = iPage;							//페이지 크기(한 화면에 보여지는 높이)
	si.nPos = min(m_iScrollPos, max(0, iMaxRange - iPage));	//현재 스크롤 위치를 유효 범위 안으로 보정, 총높이 - 페이지 높이를 상한으로 삼기. 바닥을 넘지 않게.

	SetScrollInfo(SB_VERT, &si, TRUE);	//수직스크롤바에 설정 적용
	ShowScrollBar(SB_VERT, iMaxRange > iPage);   // 내용이 넘치면 보이기

	// 스크롤 위치 보정
	m_iScrollPos = si.nPos;
	for (int i = 0; i < m_apToggleItem.GetCount(); i++)
	{
		auto pTitle = m_apToggleItem.GetAt(i);
		if (pTitle)
		{
			pTitle->DrawAgain();
		}
	}
	Invalidate();
}

void CToggleBoard::ScrollTo(int iNewPos)
{
	CRect rc; GetClientRect(&rc);
	const int iPage = max(1, rc.Height());
	const int iMaxPos = max(0, m_iTotalHeight - iPage); //더 이상 아래로 못 내리는 최대 Pos

	iNewPos = min(max(iNewPos, 0), iMaxPos);
	if (iNewPos == m_iScrollPos) return;

	m_iScrollPos = iNewPos;
	SetScrollPos(SB_VERT, m_iScrollPos, TRUE);

	DeployToggle();
}

void CToggleBoard::ScrollBy(int delta)	//상대이동을 절대 단위로 바꿈
{
	ScrollTo(m_iScrollPos + delta);
}

LRESULT CToggleBoard::OnToggleChild(WPARAM wParam, LPARAM lParam)
{
	int iNo = (int)wParam;
	int iHeight = (int)lParam;
	if (iNo < 0 || iNo >= m_apToggleItem.GetCount()) return S_OK;
	if (iNo < 0 || iNo >= m_apToggleItem.GetCount()) return S_OK;

	auto pTitle = m_apToggleItem.GetAt(iNo);
	if (pTitle && pTitle->GetToggleDown())
	{
		pTitle->SetChildHeight(iHeight);
	}
	DeployToggle();
	return S_OK;
}

void CToggleBoard::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO si = { sizeof(si) };
	si.fMask = SIF_ALL;				 // 범위/페이지/현재 pos/trackpos 모두
	GetScrollInfo(SB_VERT, &si);

	int iTarget = m_iScrollPos;
	constexpr int iLine = 20;       // 한 줄(step) 이동량
	const int page = (int)si.nPage;  // UpdateScrollInfo에서 설정한 page
	CRect rc; GetClientRect(&rc);
	//const int page = max(1, rc.Height());
	switch (nSBCode) {
	case SB_LINEUP:    iTarget -= iLine; break;  // 한 줄
	case SB_LINEDOWN:  iTarget += iLine; break;
	case SB_PAGEUP:    iTarget -= page; break;
	case SB_PAGEDOWN:  iTarget += page; break;
		// 드래그 중 실시간 위치(항상 si.nTrackPos 사용!)
	case SB_THUMBTRACK:    iTarget = (int)si.nTrackPos;   break;

		// 드래그를 놓았을 때의 위치(마찬가지로 nTrackPos)
	case SB_THUMBPOSITION: iTarget = (int)si.nTrackPos;   break;

	case SB_TOP:           iTarget = si.nMin;             break;
	case SB_BOTTOM:        // 맨 아래: nMax - page + 1 (보이는 페이지 고려)
		iTarget = max(0, (int)si.nMax - (int)si.nPage + 1);
		break;

	case SB_ENDSCROLL:     return; // 스크롤 종료 알림(화면 변경 없음)
	default: break;
	}
	ScrollTo(iTarget);
}

BOOL CToggleBoard::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int iStep = 60;
	ScrollBy(zDelta > 0 ? -iStep : iStep);
	return TRUE;
}

BOOL CToggleBoard::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(rc, ::GetSysColor(COLOR_BTNFACE));

	return TRUE;
}

void CToggleBoard::OnPaint()
{
	CPaintDC dc(this);
}

void CToggleBoard::OnLButtonDown(UINT nFlags, CPoint point)
{
	/*const int iIconCount = m_aIconRect.GetCount();
	const int iToggleCount = m_apToggleItem.GetCount();
	for (int i = 0; i < m_aIconRect.GetCount(); i++)
	{
		auto rc = m_aIconRect.GetAt(i);
		if (rc.PtInRect(point) && (i < iToggleCount))
		{
			auto pTitle = m_apToggleItem.GetAt(i);
			pTitle->SetHideToggle(!pTitle->GetHideToggle());
			break;
		}
	}
	DeployToggle();*/
	CWnd::OnLButtonDown(nFlags, point);
}


void CToggleBoard::OnMove(int x, int y)
{
	CWnd::OnMove(x, y);
}


void CToggleBoard::OnDestroy()
{
	CWnd::OnDestroy();

	ClearToggleTitle();
}
