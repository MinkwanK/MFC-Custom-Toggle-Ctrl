#pragma once
#define UM_TOGGLE_CHILD						(WM_USER + 7010)
#define UM_SET_TOGGLE_CHILD_NO				(WM_USER + 7011)

// CToggleTitle

class CToggleBoard;	//전방선언

class CToggleItem : public CStatic
{
	DECLARE_DYNAMIC(CToggleItem)

public:
	CToggleItem();
	virtual ~CToggleItem();

	BOOL Init(CString sTitle, CFont* pFont, CWnd* pChildDlg, CWnd* pParent);
	void InitIcon(UINT uiToggleUp, UINT uiToggleUpSelect, UINT uiToggleDown, UINT uiToggleDownSelect, int iCx, int iCy);
	void SetBkColor(COLORREF crBack) { m_crBack = crBack; }
	void SetTextColor(COLORREF crText) { m_crText = crText; }
	bool GetToggleDown() { return m_bToggleDown; }
	void SetToggleDown(bool bDown) { m_bToggleDown = bDown; }
	int GetNo() { return m_iNo; }
	void SetNo(int iNo);
	int GetChildHeight() { return m_iChildHeight; }
	void SetChildHeight(int iChildHeight) { m_iChildHeight = iChildHeight; }
	void NotifyChildChangedHeight(int iChildHeight);	//자식이 Resizing으로 높이를 바꿨을 때 부모에게 알려줄 때 쓰는 함수
	void DrawAgain();
	void DrawChild();
	CRect GetRect();
	CWnd* GetChild() { return m_pChild; }
	void DisplayChild();
	bool IsChildVisible();
	void SetToggleHeight(int iToggleHeight) { m_iToggleHeight = iToggleHeight; }
	int GetToggleHeight() { return m_iToggleHeight; }
	void SetHideToggle(bool bHide) { m_bHideToggle = bHide; }
	bool GetHideToggle() { return m_bHideToggle; }
	void InitToggle();
protected:
	DECLARE_MESSAGE_MAP()

private:
	bool		m_bInit = false;
	bool		m_bToggleDown = false;
	bool		m_bHideToggle = false;
	int			m_iNo = 0;				//순번
	int			m_iChildHeight = 0;		//현재 자식의 높이
	int			m_iToggleHeight = 0;
	CString		m_sTitle;
	COLORREF	m_crBack = RGB(255, 255, 255);
	COLORREF	m_crText = RGB(0, 0, 0);
	CRect		m_rcToggle;				//토글 클릭 이벤트 처리를 위한 영역			
	CFont* m_pFont;
	CWnd* m_pChild = nullptr;
	CToggleBoard* m_pParent = nullptr;
	HICON		m_hToggle[4];

	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
};


