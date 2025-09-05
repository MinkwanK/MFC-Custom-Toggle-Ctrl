#pragma once
#include "ToggleItem.h"

enum class TOGGLE_ICON_TYPE
{
	NONE = 0,
	USE_16,
	USE_24,
	USE_32,
};

class CToggleBoard : public CWnd
{
	DECLARE_DYNAMIC(CToggleBoard)

public:
	CToggleBoard();
	virtual ~CToggleBoard();
	BOOL Init(CWnd* pParent);
	void AddToggleTitle(CString sTitle, int iToggleHeight, int iChildHeight, CFont* pFont, CWnd* pChildDlg, UINT uiToggleUp, UINT uiToggleUpClick, UINT uiToggleDown, UINT uiToggleDownClick);
	void ClearToggleTitle();
	void DeployToggle();
	CArray<CToggleItem*>& GetToggleItemArray() { return m_apToggleItem; }
protected:
	DECLARE_MESSAGE_MAP()

private:
	int m_iScrollPos = 0;	//현재 스크롤 위치
	int m_iTotalHeight = 0;	//전체 컨텐츠 높이
	bool m_bResizing = false;

	CArray<CToggleItem*> m_apToggleItem;

	void UpdateScrollInfo();
	void ScrollTo(int newPos);
	void ScrollBy(int delta);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnToggleChild(WPARAM wParam, LPARAM lParam);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
};


