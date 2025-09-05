#pragma once
#include "ToggleBoard.h"

typedef struct TOGGLE_ICON_INFO
{
	HICON hIcon;
	HICON hIconInActive;
	CRect rcIcon;
};
// CTogglePanel
#define TOGGLE_BOARD_ICON_RATIO		6		//보드 상단 아이콘이 차지할 영역의 비율
class CTogglePanel : public CWnd
{
	DECLARE_DYNAMIC(CTogglePanel)

public:
	CTogglePanel();
	virtual ~CTogglePanel();

	BOOL Init(CWnd* pParent, TOGGLE_ICON_TYPE eIconType = TOGGLE_ICON_TYPE::NONE);
	void AddToggleTitle(CString sTitle, int iToggleHeight, int iChildHeight, CFont* pFont, CWnd* pChildDlg, UINT uiIcon, UINT uiIconDisable, UINT uiToggleUp, UINT uiToggleUpClick, UINT uiToggleDown, UINT uiToggleDownClick);
	CToggleBoard* GetBoard() { return m_pToggleBoard; }
protected:
	DECLARE_MESSAGE_MAP()

private:
	bool m_bInit = false;
	CToggleBoard* m_pToggleBoard;
	TOGGLE_ICON_TYPE m_eIconType = TOGGLE_ICON_TYPE::NONE;
	CArray<TOGGLE_ICON_INFO> m_aToggleIcon;
	void AdjustLayout();
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


