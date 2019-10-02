#if !defined(AFX_DLGENHCOLOR_H__02EEF6BD_CD04_41A1_87E3_461CC7008153__INCLUDED_)
#define AFX_DLGENHCOLOR_H__02EEF6BD_CD04_41A1_87E3_461CC7008153__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEnhColor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEnhColor dialog

class CDlgEnhColor : public CDialog
{
// Construction
public:
	// 颜色名称字符串长度
	int m_nNameLen;
	
	// 颜色名称字符串数组指针
	LPSTR m_lpColorName;
	
	// 颜色数目
	int m_nColorCount;
	
	// 当前选择的伪彩色编码表
	int m_nColor;

	CDlgEnhColor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEnhColor)
	enum { IDD = IDD_DLG_ENHANCE_COLOR };
	CListBox	m_lstColor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEnhColor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEnhColor)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListEnhColor();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGENHCOLOR_H__02EEF6BD_CD04_41A1_87E3_461CC7008153__INCLUDED_)
