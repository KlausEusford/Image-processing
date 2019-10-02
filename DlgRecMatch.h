#if !defined(AFX_DLGRECMATCH_H__7947D7E1_3494_11D0_9E74_000021CDD41E__INCLUDED_)
#define AFX_DLGRECMATCH_H__7947D7E1_3494_11D0_9E74_000021CDD41E__INCLUDED_

//#include "GlobalApi.h"
#include "CDib.h"
#include "ImageProcessingDoc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRecMatch.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRecMatch dialog

class CDlgRecMatch : public CDialog
{
// Construction
public:
	CDib* m_pDibInit;		// 初始图象
	CDib* m_pDibModel;		// 模板图象
	CDib* m_pDibResult;		// 匹配后的图象
	CRect m_rectInitImage;		// 初始图象显示区域
	CRect m_rectModelImage;		// 模板图象显示区域
	CRect m_rectResltImage;		// 匹配后图象显示区域
	BOOL m_bCalImgLoc;		// 计算图象位置的标志位。FALSE表示还没有计算图象位置
	CImageProcessingDoc* m_pDoc;	// 文档类指针
	BOOL TemplateMatch(CDib* pDibSrc, CDib* pDibTemplate);	// 模板匹配
	void CalImageLocation();	// 设置图象等控件的位置大小
	
	CDlgRecMatch(CWnd* pParent = NULL,CImageProcessingDoc* pDoc=NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRecMatch)
	enum { IDD = IDD_DLG_RECOG_MATCH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRecMatch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRecMatch)
	afx_msg void OnRecogOpenModel();
	afx_msg void OnRecogMatch();
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRECMATCH_H__7947D7E1_3494_11D0_9E74_000021CDD41E__INCLUDED_)
