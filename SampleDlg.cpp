// SampleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MKTest.h"
#include "SampleDlg.h"
#include "afxdialogex.h"

// CSampleDlg 대화 상자

IMPLEMENT_DYNAMIC(CSampleDlg, CDialogEx)

CSampleDlg::CSampleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SAMPLE, pParent)
{

}

CSampleDlg::~CSampleDlg()
{
}

void CSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE(UM_SET_TOGGLE_CHILD_NO, CSampleDlg::OnSetToggleChildNo)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CSampleDlg 메시지 처리기
void CSampleDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	
	
}

void CSampleDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CSampleDlg::Init()
{
	SetWindowPos(nullptr, 0, 0, 0, 0,
		SWP_HIDEWINDOW | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	m_bInit = true;
}

BOOL CSampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

LRESULT CSampleDlg::OnSetToggleChildNo(WPARAM wParam, LPARAM lParam)
{
	m_iToggleNo = (int)wParam;
	CWnd* pToggle = (CWnd*)lParam;
	m_pToggle = (CToggleItem*)pToggle;
	return S_OK;
}

void CSampleDlg::OnMouseMove(UINT nFlags, CPoint point)
{	
	CString sValue;
	CWnd* pParent = GetParent();
	
	if (!pParent || !(::IsWindow(pParent->GetSafeHwnd())))
	{
		CDialogEx::OnMouseMove(nFlags, point);
		return;
	}

	// Point 부모 좌표 변환
	ClientToScreen(&point); pParent->ScreenToClient(&point);

	// 내 윈도우 좌표를 부모 좌표로 변환
	CRect rc; GetWindowRect(&rc); pParent->ScreenToClient(&rc);
	if((abs(point.y - rc.bottom) < 10)) ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS)); // 수직 리사이즈 커서 활성화

	if (GetTickCount() - m_dwResizeTick < 20)
		return;

	if (nFlags & MK_LBUTTON)
	{
		rc.bottom = point.y;
		int iNewHeight = rc.Height();
		if (iNewHeight > m_iMinHeight)
		{
			rc.bottom = point.y;
			SetWindowPos(
				nullptr,
				rc.left, rc.top,
				rc.Width(), rc.Height(),
				SWP_NOACTIVATE | SWP_SHOWWINDOW
			);
			// 부모에게 자신의 Size가 변경됐음을 알려야 한다.
			if (m_bInit && m_pToggle && ::IsWindow(m_pToggle->GetSafeHwnd()))
			{
				CRect rcClient;
				GetClientRect(&rcClient);
				m_pToggle->ScreenToClient(&rcClient);
				m_pToggle->NotifyChildChangedHeight(rcClient.Height());
			}
			m_dwResizeTick = GetTickCount();
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CSampleDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rc; GetClientRect(&rc);
	if (abs(point.y - rc.bottom) < 10)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS)); // 수직 리사이즈
		SetCapture();
		m_bResizing = true;
		return; // 기본 처리 건너뜀
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CSampleDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bResizing)
	{
		if (GetCapture() == this) ReleaseCapture();
		m_bResizing = false;

		// 부모에 최종 높이 통지 (드래그 중 실시간 통지는 버벅임/재진입 유발)
		if (m_bInit && m_pToggle && ::IsWindow(m_pToggle->GetSafeHwnd()))
		{
			CRect rcClient; GetClientRect(&rcClient); // 이미 client 좌표
			m_pToggle->PostMessage(UM_TOGGLE_CHILD, (WPARAM)m_iToggleNo, (LPARAM)rcClient.Height());
		}
		return;
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}
