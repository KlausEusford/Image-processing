// ImageProcessingView.h : interface of the CImageProcessingView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSINGVIEW_H__613B030F_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_)
#define AFX_IMAGEPROCESSINGVIEW_H__613B030F_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImageProcessingView : public CScrollView
{
protected: // create from serialization only
	CImageProcessingView();
	DECLARE_DYNCREATE(CImageProcessingView)

// Attributes
public:
	BOOL DIBDWTStep(CDib* pDib, int nInv);
	CImageProcessingDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProcessingView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	double* m_pDbImage;
	int m_nSupp;
	int m_nDWTCurDepth;
	virtual ~CImageProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageProcessingView)
	afx_msg void OnFft2d();
	afx_msg void OnDft2d();
	afx_msg void OnViewHistogram();
	afx_msg void OnEnhanceSmooth();
	afx_msg void OnEnhanceMedian();
	afx_msg void OnEnhancePseudcolor();
	afx_msg void OnTransDwt();
	afx_msg void OnTransIdwt();
	afx_msg void OnEnhanceLintrans();
	afx_msg void OnEnhanceHistequ();
	afx_msg void OnRegReg();
	afx_msg void OnEnhanceSharp();
	afx_msg void OnEnhanceSmoothFr();
	afx_msg void OnEnhanceButtLow();
	afx_msg void OnEnhanceSharpFreq();
	afx_msg void OnEnhanceButtHight();
	afx_msg void OnRegionsegFix();
	afx_msg void OnAdaRegionSeg();
	afx_msg void OnEdgeRoberts();
	afx_msg void OnEdgeSobel();
	afx_msg void OnEdgePrewitt();
	afx_msg void OnEdgeLaplace();
	afx_msg void OnEdgeCanny();
	afx_msg void OnEdgeTrack();
	afx_msg void OnRegionGrow();
	afx_msg void OnMotionBackground();
	afx_msg void OnRecogMatch();
	afx_msg void OnCodingShanfino();
	afx_msg void OnDegenerationInverse();
	afx_msg void OnDegenerationMotion();
	afx_msg void OnDEGENERATIONWinner();
	afx_msg void OnRestoreInverse();
	afx_msg void OnRestoreMotion();
	afx_msg void OnRestoreWinner();
	afx_msg void OnStreetFramework();
	afx_msg void OnStreetTransform();
	afx_msg void OnTrace();
	afx_msg void OnViewBayer();
	afx_msg void OnVIEWFloydSteinberg();
	afx_msg void OnOutline();
	afx_msg void OnFrameRestore();
	afx_msg void OnMoment();
	afx_msg void OnBarycentermoment();
	afx_msg void OnAnalysisHolenum();
	afx_msg void OnFreqDct();
	afx_msg void OnFreqHotelling();
	afx_msg void OnFreqWalsh();
	afx_msg void OnCodingArith();
	afx_msg void OnCodingBitplane();
	afx_msg void OnCodingHuffman();
	afx_msg void OnCodingLoadimg();
	afx_msg void OnCodingWriteimg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ImageProcessingView.cpp
inline CImageProcessingDoc* CImageProcessingView::GetDocument()
   { return (CImageProcessingDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSINGVIEW_H__613B030F_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_)
