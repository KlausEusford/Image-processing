#if !defined(AFX_DLGSMOOTH_H__C3C270A3_299B_4E53_ADC8_FEE54D770B97__INCLUDED_)
#define AFX_DLGSMOOTH_H__C3C270A3_299B_4E53_ADC8_FEE54D770B97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSmooth.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSmooth dialog

class CDlgSmooth : public CDialog
{
// Construction
public:

	void UpdateEdit();

	// 模板数组
	double *m_pdbTemp;

	// 声名模板类型
	int		m_nTemType;

	CDlgSmooth(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSmooth)
	enum { IDD = IDD_DLG_ENHANCE_SMOOTH };
	
	// 模板中心X坐标
	int		m_nSmthTemCenX;

	// 模板中心Y坐标
	int		m_nSmthTemCenY;

	// 模板系数
	double		m_dbSmthTemCoef;

	// 模板高度
	int		m_nSmthTemHeight;

	// 模板宽度
	int		m_nSmthTemWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSmooth)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSmooth)
	afx_msg void OnRadioAvertem();
	afx_msg void OnRadioGuasstem();
	afx_msg void OnRadioSelftem();
	afx_msg void OnKillfocusEditSelftemHeight();
	afx_msg void OnKillfocusEditSelftemWidth();
	virtual void OnOK();
	afx_msg void OnChangeEditSelftemWidth();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSMOOTH_H__C3C270A3_299B_4E53_ADC8_FEE54D770B97__INCLUDED_)
