
// MKTestDlg.h: 헤더 파일
//

#pragma once
#include "ToggleCtrl/ToggleItem.h"
#include "ToggleCtrl/ToggleBoard.h"
#include "ToggleCtrl/TogglePanel.h"
#include "SampleDlg.h"

// CMKTestDlg 대화 상자
class CMKTestDlg : public CDialogEx
{
// 생성입니다.
public:
	CMKTestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MKTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CSampleDlg m_sample;
	CSampleDlg m_sample2;
	CSampleDlg m_sample3;
	CTogglePanel m_panel;
	bool m_bInit = false;
	void Init();
	void AdjustLayout();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
