#if !defined(AFX_DLGHISTSHOW_H__82A31F26_1DBA_486B_B631_0484FF3FB8EA__INCLUDED_)
#define AFX_DLGHISTSHOW_H__82A31F26_1DBA_486B_B631_0484FF3FB8EA__INCLUDED_

#include "cdib.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHistShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHistShow dialog

class CDlgHistShow : public CDialog
{
// Construction
public:
	// DIB图象指针
	CDib* m_pDib;

	// 直方图
	LONG m_lHist[256];

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
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHISTSHOW_H__82A31F26_1DBA_486B_B631_0484FF3FB8EA__INCLUDED_)
