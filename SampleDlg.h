#pragma once
#include "ToggleCtrl/ToggleItem.h"

// CSampleDlg 대화 상자

class CSampleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSampleDlg)

public:
	CSampleDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSampleDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SAMPLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	void Init();
	bool m_bInit = false;
	bool m_bResizing = false;
	DWORD m_dwResizeTick = 0;
	int m_iToggleNo = 0;
	int m_iMinHeight = 48;
	CToggleItem* m_pToggle = nullptr;
	virtual BOOL OnInitDialog();
	LRESULT OnSetToggleChildNo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
