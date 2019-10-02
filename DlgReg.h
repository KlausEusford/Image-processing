#if !defined(AFX_DlgReg_H__096C39D7_72FA_4ABD_90AC_688669D1692C__INCLUDED_)
#define AFX_DlgReg_H__096C39D7_72FA_4ABD_90AC_688669D1692C__INCLUDED_

#include "ImageProcessingDoc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgReg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgReg dialog

class CDlgReg : public CDialog
{
// Construction
public:
	CImageProcessingDoc* m_pDoc;
	CDlgReg(CWnd* pParent = NULL, CImageProcessingDoc* pDoc = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgReg)
	enum { IDD = IDD_DLG_REG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgReg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// 基准图象数据指针，用于图象拼接
	LPBYTE m_lpBaseImg;		

// 待配准图象数据指针，用于图象拼接
	LPBYTE m_lpSampImg;		

	// 计算图象位置的标志位。FALSE表示还没有计算图象位置
	BOOL m_bCalImgLoc;		

// 选取特征点标志位。FALSE表示还没有选取
	BOOL m_bChoseFeature;	

// 设置图象等控件的位置大小	
	void CalImageLocation();	

	// 基准图象
	CDib* m_pDibInit;		 

	// 待配准图象
	CDib* m_pDibSamp;		

	// 配准后的图象
	CDib* m_pDibResult;		

	// 基准图象显示区域
	CRect m_rectInitImage;		

	// 待配准图象显示区域
	CRect m_rectResltImage;	

	// 待配准特征点位置	
	CPoint m_pPointSampl[3];	

	// 配准的特征点位置
	CPoint m_pPointBase[3];		

	// 寻找配准点
	CPoint FindMatchPoint(CDib* pDibBase, CDib* pDibSamp, CPoint pointSamp);			

	// 计算相似度
	double CalCorrelation(unsigned char* pBase, unsigned char* pSamp, 	int nBlockLen);				
	// 画出特征点
	void   DrawFeature(CDC* pDC);			

	// 获得仿射变换系数
	void   GetAffinePara(CPoint* pPointBase, CPoint* pPointSampl, double* pDbBs2SpAffPara);		
	// 计算两个矩阵相乘
	void   CalMatProduct(double* pDbSrc1, double *pDbSrc2, double *pDbDest, int nX, int nY, int nZ);			
	// 计算矩阵的逆
	BOOL   CalInvMatrix(double* pDbSrc, int nLen);	
	
	// 获得待配准图象仿射变换后的区域
	CRect  GetAftAffDim(double* pDbAffPara);	

	// 计算仿射变换后的待配准图象
	LPBYTE   SetSampImgAftAff(double* pDbAffPara, CRect rectNewImg);	

	// 计算三次插值
	unsigned char CalSpline(unsigned char *pUnchCorr, double x, double y);
	 
	// 计算仿射变换后的基准图象
	LPBYTE   SetBaseImgAftAff(CRect rectNewImg);			
	
	// Generated message map functions
	//{{AFX_MSG(CDlgReg)
	afx_msg void OnPaint();
	afx_msg void OnRegOpen();
	afx_msg void OnRegReg();
	afx_msg void OnRegChoseFeature();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_nChsFeatureNum;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DlgReg_H__096C39D7_72FA_4ABD_90AC_688669D1692C__INCLUDED_)
