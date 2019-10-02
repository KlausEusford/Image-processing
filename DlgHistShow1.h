#if !defined(AFX_DLGHISTSHOW1_H__0D22FC15_DE60_4FD8_A522_D9433D189AEF__INCLUDED_)
#define AFX_DLGHISTSHOW1_H__0D22FC15_DE60_4FD8_A522_D9433D189AEF__INCLUDED_

#include "cdib.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHistShow1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHistShow dialog

class CDlgHistShow : public CDialog
{
// Construction
public:
	// CDib对象指针
	CDib* m_pDib;

	// 直方图数组
	int m_nHist[256];

	CDlgHistShow(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgHistShow)
	enum { IDD = IDD_DLG_SHOW_HISTOGRAM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHistShow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHistShow)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHISTSHOW1_H__0D22FC15_DE60_4FD8_A522_D9433D189AEF__INCLUDED_)
