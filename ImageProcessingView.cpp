// ImageProcessingView.cpp : implementation of the CImageProcessingView class
//

#include "stdafx.h"
#include "ImageProcessing.h"

#include "ImageProcessingDoc.h"
#include "ImageProcessingView.h"
#include  "GlobalApi.h"
#include  "DlgCoding.h"
#include <complex>
using namespace std;

#include "DlgHistShow1.h"
#include "DlgSmooth.h"
#include "DlgMedian.h"
#include "DlgEnhColor.h"
#include "DlgEhnLinTrans.h"
#include "DlgReg.h"
#include "DlgRecMatch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView

IMPLEMENT_DYNCREATE(CImageProcessingView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessingView, CScrollView)
	//{{AFX_MSG_MAP(CImageProcessingView)
	ON_COMMAND(ID_FFT_2D, OnFft2d)
	ON_COMMAND(ID_DFT_2D, OnDft2d)
	ON_COMMAND(ID_VIEW_HISTOGRAM, OnViewHistogram)
	ON_COMMAND(ID_ENHANCE_SMOOTH, OnEnhanceSmooth)
	ON_COMMAND(ID_ENHANCE_MEDIAN, OnEnhanceMedian)
	ON_COMMAND(ID_ENHANCE_PSEUDCOLOR, OnEnhancePseudcolor)
	ON_COMMAND(ID_TRANS_DWT, OnTransDwt)
	ON_COMMAND(ID_TRANS_IDWT, OnTransIdwt)
	ON_COMMAND(IDC_ENHANCE_LINTRANS, OnEnhanceLintrans)
	ON_COMMAND(IDC_ENHANCE_HISTEQU, OnEnhanceHistequ)
	ON_COMMAND(ID_REG_REG, OnRegReg)
	ON_COMMAND(IDC_ENHANCE_SHARP, OnEnhanceSharp)
	ON_COMMAND(ID_ENHANCE_SMOOTH_FR, OnEnhanceSmoothFr)
	ON_COMMAND(IDC_ENHANCE_BUTT_LOW, OnEnhanceButtLow)
	ON_COMMAND(IDC_ENHANCE_SHARP_FREQ, OnEnhanceSharpFreq)
	ON_COMMAND(IDC_ENHANCE_BUTT_HIGHT, OnEnhanceButtHight)
	ON_COMMAND(ID_REGIONSEG_FIX, OnRegionsegFix)
	ON_COMMAND(ID_ADA_REGION_SEG, OnAdaRegionSeg)
	ON_COMMAND(ID_EDGE_ROBERTS, OnEdgeRoberts)
	ON_COMMAND(ID_EDGE_SOBEL, OnEdgeSobel)
	ON_COMMAND(ID_EDGE_PREWITT, OnEdgePrewitt)
	ON_COMMAND(ID_EDGE_LAPLACE, OnEdgeLaplace)
	ON_COMMAND(ID_EDGE_CANNY, OnEdgeCanny)
	ON_COMMAND(ID_EDGE_TRACK, OnEdgeTrack)
	ON_COMMAND(ID_REGION_GROW, OnRegionGrow)
	ON_COMMAND(ID_MOTION_BACKGROUND, OnMotionBackground)
	ON_COMMAND(ID_RECOG_MATCH, OnRecogMatch)
	ON_COMMAND(ID_CODING_SHANFINO, OnCodingShanfino)
	ON_COMMAND(ID_DEGENERATION_INVERSE, OnDegenerationInverse)
	ON_COMMAND(ID_DEGENERATION_MOTION, OnDegenerationMotion)
	ON_COMMAND(ID_DEGENERATION_Winner, OnDEGENERATIONWinner)
	ON_COMMAND(ID_RESTORE_INVERSE, OnRestoreInverse)
	ON_COMMAND(ID_RESTORE_MOTION, OnRestoreMotion)
	ON_COMMAND(ID_RESTORE_WINNER, OnRestoreWinner)
	ON_COMMAND(ID_STREET_FRAMEWORK, OnStreetFramework)
	ON_COMMAND(ID_STREET_TRANSFORM, OnStreetTransform)
	ON_COMMAND(ID_TRACE, OnTrace)
	ON_COMMAND(ID_VIEW_BAYER, OnViewBayer)
	ON_COMMAND(ID_VIEW_FloydSteinberg, OnVIEWFloydSteinberg)
	ON_COMMAND(ID_OUTLINE, OnOutline)
	ON_COMMAND(ID_FRAME_RESTORE, OnFrameRestore)
	ON_COMMAND(ID_MOMENT, OnMoment)
	ON_COMMAND(ID_BARYCENTERMOMENT, OnBarycentermoment)
	ON_COMMAND(ID_ANALYSIS_HOLENUM, OnAnalysisHolenum)
	ON_COMMAND(ID_FREQ_DCT, OnFreqDct)
	ON_COMMAND(ID_FREQ_HOTELLING, OnFreqHotelling)
	ON_COMMAND(ID_FREQ_WALSH, OnFreqWalsh)
	ON_COMMAND(ID_CODING_ARITH, OnCodingArith)
	ON_COMMAND(ID_CODING_BITPLANE, OnCodingBitplane)
	ON_COMMAND(ID_CODING_HUFFMAN, OnCodingHuffman)
	ON_COMMAND(ID_CODING_LOADIMG, OnCodingLoadimg)
	ON_COMMAND(ID_CODING_WRITEIMG, OnCodingWriteimg)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView construction/destruction

CImageProcessingView::CImageProcessingView()
{
	// 为小波变换设置的参数
	// 临时存放小波变换系数内存
	m_pDbImage = NULL;	
	
	// 设置当前层数
	m_nDWTCurDepth = 0;

	// 设置小波基紧支集长度
	m_nSupp = 1;
}

CImageProcessingView::~CImageProcessingView()
{
	// 释放已分配内存
	if(m_pDbImage){
		delete[]m_pDbImage;
		m_pDbImage = NULL;
	}
}

BOOL CImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView drawing

void CImageProcessingView::OnDraw(CDC* pDC)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CSize sizeDibDisplay;		
	

	
	if(!pDoc->m_pDibInit->IsEmpty()){	
		sizeDibDisplay = pDoc->m_pDibInit->GetDimensions();
		pDoc->m_pDibInit->Draw(pDC,CPoint(0,0),sizeDibDisplay);	
	}	

}

void CImageProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CSize sizeTotal = pDoc->m_pDibInit->GetDimensions();
	SetScrollSizes(MM_TEXT, sizeTotal);

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView printing

BOOL CImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView diagnostics

#ifdef _DEBUG
void CImageProcessingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProcessingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProcessingDoc* CImageProcessingView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessingDoc)));
	return (CImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView message handlers









/*************************************************************************
 *
 * \函数名称：
 *   OnFft2d()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   运行二维快速傅立叶变换
 *
 *************************************************************************
 */
void CImageProcessingView::OnFft2d() 
{
	//图象FFT变换

	// 更改光标形状
	BeginWaitCursor();
	
	// 循环控制变量
	int y;
	int x;
	
	// 获得Doc类的指针
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();

	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的离散傅立叶变换）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的离散傅立叶变换！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}

	// 图象的宽长
	CSize sizeImage ;
	int nWidth ;
	int nHeight;

	// 获得图象的宽长
	sizeImage = pDib->GetDimensions() ;
		
	nWidth = sizeImage.cx;
	nHeight= sizeImage.cy;

	// 临时变量
	double	dTmpOne;
	double  dTmpTwo;
	
	// 傅立叶变换竖直方向点数
	int nTransHeight ;

	// 傅立叶变换水平方向点数
	int nTransWidth  ;	
	
	// 计算进行傅立叶变换的点数	（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// 计算进行傅立叶变换的点数 （2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;

	// 计算图象数据存储每行需要的字节数
	// BMP文件的每行数据存储是DWORD对齐的
	int		nSaveWidth;
	nSaveWidth = ( (nWidth << 3) + 31)/32 * 4 ;

	// 指向图象数据的指针
	LPBYTE lpImage ;
	lpImage = pDib->m_lpImage ;

	// 图象象素值
	unsigned char unchValue;

	
	// 指向时域数据的指针
	complex<double> * pCTData ;

	// 指向频域数据的指针
	complex<double> * pCFData ;

	// 分配内存
	pCTData=new complex<double>[nTransWidth * nTransHeight];
	pCFData=new complex<double>[nTransWidth * nTransHeight];

	// 初始化
	// 图象数据的宽和高不一定是2的整数次幂，所以pCTData
	// 有一部分数据需要补0
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x]=complex<double>(0,0);
		}
	}

	// 把图象数据传给pCTData
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nSaveWidth +x];
			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
		}
	}

	// 傅立叶正变换
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData) ;
	
	// 临时变量
	double dTmp;

	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dTmp = pCFData[y * nTransWidth + x].real() 
				   * pCFData[y * nTransWidth + x].real()
				 + pCFData[y * nTransWidth + x].imag() 
				   * pCFData[y * nTransWidth + x].imag();
			
			dTmp = sqrt(dTmp) ;

			// 为了显示，需要对幅度的大小进行伸缩
			dTmp /= 100        ;

			// 限制图象数据的大小
			dTmp = min(dTmp, 255) ;

			lpImage[y*nSaveWidth +x] = (unsigned char)(int)dTmp;
		}
	}

	// 为了在屏幕上显示，我们把幅度值大的部分用黑色显示
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			lpImage[y*nSaveWidth +x] = 255 - lpImage[y*nSaveWidth +x];
		}
	}
	
	// 刷新屏幕
	Invalidate();

	// 释放内存
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

	//  恢复光标形状
	EndWaitCursor();
}


/*************************************************************************
 *
 * \函数名称：
 *   OnDft2d()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   运行二维傅立叶变换
 *
 *************************************************************************
 */
void CImageProcessingView::OnDft2d() 
{
	//图象离散傅立叶变换
	
	//提示用户，直接进行离散傅立叶变换的时间很长
	MessageBox("没有使用FFT，时间可能很长！", "作者提示" ,
			MB_ICONINFORMATION | MB_OK);
	
	//更改光标形状
	BeginWaitCursor(); 

	// 循环控制变量
	int y;
	int x;
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的离散傅立叶变换）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的离散傅立叶变换！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}

	//图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	// 计算图象数据存储每行需要的字节数
	// BMP文件的每行数据存储是DWORD对齐的
	int		nSaveWidth;
	nSaveWidth = ( (nWidth << 3) + 31)/32 * 4 ;

	// 指向图象数据的指针
	LPBYTE lpImage ;
	lpImage = pDib->m_lpImage ;

	double * pTrRstRpart  = new double [nWidth*nHeight];
	double * pTrRstIpart  = new double [nWidth*nHeight];	
	
	::DIBDFT_2D(pDib, pTrRstRpart,pTrRstIpart);

	// 临时变量
	double dTmp;

	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dTmp = pTrRstRpart[y*nWidth + x] * pTrRstRpart[y*nWidth + x]
				 + pTrRstIpart[y*nWidth + x] * pTrRstIpart[y*nWidth + x];
			
			dTmp = sqrt(dTmp) ;

			// 为了显示，需要对幅度的大小进行伸缩
			dTmp /= 100        ;

			// 限制图象数据的大小
			dTmp = min(dTmp, 255) ;

			lpImage[y*nSaveWidth +x] = (unsigned char)(int)dTmp;
		}
	}

	// 为了在屏幕上显示，我们把幅度值大的部分用黑色显示
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			lpImage[y*nSaveWidth +x] = 255 - lpImage[y*nSaveWidth +x];
		}
	}

	// 释放内存
	delete pTrRstRpart;
	pTrRstRpart=NULL  ;
	
	delete pTrRstIpart;
	pTrRstIpart=NULL  ;

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

	// 恢复光标形状
	EndWaitCursor(); 
	
	// 刷新屏幕
	Invalidate();
}

void CImageProcessingView::OnFreqDct() 
{
	// 图象的离散余弦变换
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的离散余弦变换）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的离散余弦变换！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBDct(pDib);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnFreqHotelling() 
{
	// 图象霍特林变换
	
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的离散霍特林变换）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的离散霍特林变换！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}

	//  图象的霍特林变换
	DIBHOTELLING(pDib);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnFreqWalsh() 
{
	// 图象的沃尔什－哈达玛变换
	
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的沃尔什－哈达玛变换）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的离散沃尔什变换！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}

	//  进行沃尔什－哈达玛变换
	DIBWalsh(pDib);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

/*************************************************************************
 *
 * \函数名称：
 *   OnViewHistogram()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   查看直方图，弹出直方图显示界面
 *
 *************************************************************************
 */
void CImageProcessingView::OnViewHistogram() 
{
	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	// DIB的颜色数目
	int nColorTableEntries;
	nColorTableEntries = pDoc->m_pDibInit->m_nColorTableEntries;
		
	// 判断是否是8bpp位图（这里只处理8bpp位图）
	if ( nColorTableEntries != 256)
	{
		// 提示用户，不再进行处理
		MessageBox("目前只支持查看256色位图灰度直方图！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 创建对话框
	CDlgHistShow dlgHistShow;
	
	// 初始化变量值
	dlgHistShow.m_pDib = pDoc->m_pDibInit;
		
	// 显示对话框
	if (dlgHistShow.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 恢复光标
	EndWaitCursor();	
}

/*************************************************************************
 *
 * \函数名称：
 *   OnEnhanceSmooth()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   对图象进行平滑处理，并弹出平滑模板设置对话框
 *
 *************************************************************************
 */
void CImageProcessingView::OnEnhanceSmooth() 
{
	// TODO: Add your command handler code here
		// 图像平滑
	
	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();
	
	// 模板高度
	int		nTempHeight;
	
	// 模板宽度
	int		nTempWidth;
	
	// 模板系数
	double		dbTempCoef;
	
	// 模板中心元素X坐标
	int		nTempCenX;
	
	// 模板中心元素Y坐标
	int		nTempCenY;
	
	// 模板元素数组
	double	pdbTemp[25];
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平滑，其它的可以类推）
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	//if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的平滑！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 创建对话框
	CDlgSmooth dlgSmth;
	
	// 给模板数组赋初值（为平均模板）
	pdbTemp[0] = 1.0;
	pdbTemp[1] = 1.0;
	pdbTemp[2] = 1.0;
	pdbTemp[3] = 0.0;
	pdbTemp[4] = 0.0;
	pdbTemp[5] = 1.0;
	pdbTemp[6] = 1.0;
	pdbTemp[7] = 1.0;
	pdbTemp[8] = 0.0;
	pdbTemp[9] = 0.0;
	pdbTemp[10] = 1.0;
	pdbTemp[11] = 1.0;
	pdbTemp[12] = 1.0;
	pdbTemp[13] = 0.0;
	pdbTemp[14] = 0.0;
	pdbTemp[15] = 0.0;
	pdbTemp[16] = 0.0;
	pdbTemp[17] = 0.0;
	pdbTemp[18] = 0.0;
	pdbTemp[19] = 0.0;
	pdbTemp[20] = 0.0;
	pdbTemp[21] = 0.0;
	pdbTemp[22] = 0.0;
	pdbTemp[23] = 0.0;
	pdbTemp[24] = 0.0;
	
	// 初始化对话框变量值
	dlgSmth.m_nTemType  = 0;
	dlgSmth.m_nSmthTemHeight  = 3;
	dlgSmth.m_nSmthTemWidth  = 3;
	dlgSmth.m_nSmthTemCenX = 1;
	dlgSmth.m_nSmthTemCenY = 1;
	dlgSmth.m_dbSmthTemCoef  = (double) (1.0 / 9.0);
	dlgSmth.m_pdbTemp = pdbTemp;
	
	// 显示对话框，提示用户设定平移量
	if (dlgSmth.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	nTempHeight   = dlgSmth.m_nSmthTemHeight;
	nTempWidth  = dlgSmth.m_nSmthTemWidth;
	nTempCenX = dlgSmth.m_nSmthTemCenX;
	nTempCenY = dlgSmth.m_nSmthTemCenY;
	dbTempCoef  = dlgSmth.m_dbSmthTemCoef;
	
	// 删除对话框
	delete dlgSmth;	
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用Template()函数平滑DIB
	if (GeneralTemplate(pDoc->m_pDibInit, nTempWidth, nTempHeight, 
		nTempCenX, nTempCenY, pdbTemp, dbTempCoef))
	{		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 恢复光标
	EndWaitCursor();	
	
}

/*************************************************************************
 *
 * \函数名称：
 *   OnEnhanceMedian()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   对图象进行中值滤波，并弹出平滑模板设置对话框
 *
 *************************************************************************
 */
void CImageProcessingView::OnEnhanceMedian() 
{
	// 中值滤波
	
	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();
	
	// 滤波器的高度
	int nFilterHeight;
	
	// 滤波器的宽度
	int nFilterWidth;
	
	// 中心元素的X坐标
	int nFilterCenX;
	
	// 中心元素的Y坐标
	int nFilterCenY;
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平滑，其它的可以类推）
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的平滑！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	
	// 创建对话框
	CDlgMedian dlgMedian;
	
	// 初始化变量值
	dlgMedian.m_nFilterType = 0;
	dlgMedian.m_nFilterHeight = 3;
	dlgMedian.m_nFilterWidth = 1;
	dlgMedian.m_nFilterCenX = 0;
	dlgMedian.m_nFilterCenY = 1;
	
	// 显示对话框，提示用户设定平移量
	if (dlgMedian.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户的设定
	nFilterHeight = dlgMedian.m_nFilterHeight;
	nFilterWidth = dlgMedian.m_nFilterWidth;
	nFilterCenX = dlgMedian.m_nFilterCenX;
	nFilterCenY = dlgMedian.m_nFilterCenY;
	
	// 删除对话框
	delete dlgMedian;	
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用MedianFilter()函数中值滤波
	if (MedianFilter(pDoc->m_pDibInit, nFilterWidth,
		nFilterHeight, nFilterCenX, nFilterCenY ))
	
	{		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 恢复光标
	EndWaitCursor();

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
}


void CImageProcessingView::OnEnhancePseudcolor() 
{
	// 伪彩色编码
	
	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();
	
	// 保存用户选择的伪彩色编码表索引
	int		nColor;
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平滑，其它的可以类推）
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的伪彩色变换！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);

		// 返回
		return;
	}
	
	// 参数对话框
	CDlgEnhColor dlgPara;
	
	// 初始化变量值
	if (pDoc->m_nColorIndex >= 0)
	{
		// 初始选中当前的伪彩色
		dlgPara.m_nColor = pDoc->m_nColorIndex;
	}
	else
	{
		// 初始选中灰度伪彩色编码表
		dlgPara.m_nColor = 0;
	}
	
	// 指向名称数组的指针
	dlgPara.m_lpColorName = (LPSTR) ColorScaleName;
	
	// 伪彩色编码数目
	dlgPara.m_nColorCount = COLOR_SCALE_COUNT;
	
	// 名称字符串长度
	dlgPara.m_nNameLen = sizeof(ColorScaleName) / COLOR_SCALE_COUNT;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户的设定
	nColor = dlgPara.m_nColor;
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 判断伪彩色编码是否改动
	if (pDoc->m_nColorIndex != nColor)
	{
		// 调用ReplaceColorPal()函数变换调色板
		ReplaceDIBColorTable(pDoc->m_pDibInit, (LPBYTE)ColorsTable[nColor]);
		
		// 更新调色板
		pDoc->m_pDibInit->MakePalette();
		
		// 更新类成员变量
		pDoc->m_nColorIndex = nColor;
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
				
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}	
	

	// 恢复光标
	EndWaitCursor();
}

void CImageProcessingView::OnTransDwt() 
{
	// 获得文档类指针
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();

	// 指向图象的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 更改光标形状
	BeginWaitCursor();

	// 进行小波变换
	int rsl = DIBDWTStep(pDib,0);

	// 恢复光标形状
	EndWaitCursor();

	// 如果小波变换不成功，则直接返回
	if (!rsl)			
		return;

	// 设置脏标志
	pDoc->SetModifiedFlag(TRUE);

	// 更新显示
	pDoc->UpdateAllViews(FALSE);
}

void CImageProcessingView::OnTransIdwt() 
{
	// 获得文档类指针	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();

	// 指向图象的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 更改光标形状
	BeginWaitCursor();

	// 进行小波反变换
	int rsl = DIBDWTStep(pDib,1);

	// 恢复光标形状
	EndWaitCursor();

	// 如果小波变换不成功，则直接返回
	if (!rsl)			
		return;
	pDoc->UpdateAllViews(FALSE);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
}

BOOL CImageProcessingView::DIBDWTStep(CDib* pDib, int nInv)
{
	// 循环变量
	int i, j;

	// 获取图象的长度和宽度
	int nWidth  = pDib->m_lpBMIH->biWidth;
	int nHeight = pDib->m_lpBMIH->biHeight;
		
	// 获取变换的最大层数
	int nMaxWLevel = Log2(nWidth);
	int nMaxHLevel = Log2(nHeight);
	int nMaxLevel;
	if (nWidth == 1<<nMaxWLevel && nHeight == 1<<nMaxHLevel)
		nMaxLevel = min(nMaxWLevel, nMaxHLevel);

	// 获取图象的存储尺寸
	CSize sizeImageSave = pDib->GetDibSaveDim();

	// 临时变量
	double	*pDbTemp;
	BYTE	*pBits;

	// 如果小波变换的存储内存还没有分配，则分配此内存
	if(!m_pDbImage){			
		m_pDbImage = new double[nWidth*nHeight];
		if (!m_pDbImage)	return FALSE;

		// 将图象数据放入m_pDbImage中 
		for (j=0; j<nHeight; j++)
		{
			pDbTemp = m_pDbImage + j*sizeImageSave.cx;
			pBits = pDib->m_lpImage + (nHeight-1-j)*sizeImageSave.cx;		
			for (i=0; i<nWidth; i++)
				pDbTemp[i] = pBits[i];
		}
	}
	
	// 进行小波变换（或反变换）
	if (!DWTStep_2D(m_pDbImage, nMaxWLevel-m_nDWTCurDepth, nMaxHLevel-m_nDWTCurDepth,
						nMaxWLevel, nMaxHLevel, nInv, 1, m_nSupp))
		return FALSE;

	// 如果是反变换，则当前层数减1
	if (nInv)
		m_nDWTCurDepth --;
	// 否则加1
	else
		m_nDWTCurDepth ++;

	// 然后，将数据拷贝回原CDib中，并进行相应的数据转换
	int lfw = nWidth>>m_nDWTCurDepth, lfh = nHeight>>m_nDWTCurDepth;
	for (j=0; j<nHeight; j++)
	{
		pDbTemp = m_pDbImage + j*sizeImageSave.cx;
		pBits = pDib->m_lpImage + (nHeight-1-j)*sizeImageSave.cx;
		for (i=0; i<nWidth; i++)
		{
			if (j<lfh && i<lfw)
				pBits[i] = FloatToByte(pDbTemp[i]);
			else
				pBits[i] = BYTE(FloatToChar(pDbTemp[i]) ^ 0x80);			
		}
	}

	// 返回
	return TRUE;
}
void CImageProcessingView::OnEnhanceLintrans() 
{
	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	// 创建对话框
	CDlgEhnLinTrans dlgPara;
	
	// 点1坐标
	int	nX1;
	int	nY1;
	
	// 点2坐标
	int	nX2;
	int	nY2;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的灰度拉伸，其它的可以类推）
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的伪彩色变换！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);

		// 返回
		return;
	}
	
	// 初始化变量值
	dlgPara.m_nX1 = 50;
	dlgPara.m_nY1 = 30;
	dlgPara.m_nX2 = 200;
	dlgPara.m_nY2 = 220;
	
	// 显示对话框，提示用户设定拉伸位置
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户的设定
	nX1 = dlgPara.m_nX1;
	nY1 = dlgPara.m_nY1;
	nX2 = dlgPara.m_nX2;
	nY2 = dlgPara.m_nY2;
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用GrayStretch()函数进行灰度拉伸
	GraySegLinTrans(pDoc->m_pDibInit, nX1, nY1, nX2, nY2);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();
}

void CImageProcessingView::OnEnhanceHistequ() 
{
	// 直方图均衡

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();
		
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的直方图均衡，其它的可以类推）
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的伪彩色变换！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);

		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用HistogramEqualize()函数进行直方图均衡
	HistogramEqualize(pDoc->m_pDibInit);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
		
	// 恢复光标
	EndWaitCursor();	
}

/*************************************************************************
 *
 * \函数名称：
 *   OnRegReg()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数实现图象的配准
 *
 *************************************************************************
 */
void CImageProcessingView::OnRegReg() 
{	
	// 获得文档类句柄
	CImageProcessingDoc* pDoc;
	pDoc = GetDocument();

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的水平镜像，其它的可以类推）
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象配准！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 打开图象配准对话框
	CDlgReg* pDlg=new CDlgReg(NULL,pDoc);

	pDlg->DoModal();

	delete pDlg;
}

/*************************************************************************
 *
 * \函数名称：
 *   OnEnhanceSharp()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数利用Laplacian算子实现图象的锐化
 *
 *************************************************************************
 */
void CImageProcessingView::OnEnhanceSharp() 
{
	//更改光标形状
	BeginWaitCursor(); 
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 循环控制变量
	int y;
	int x;
	
	CSize sizeImage = pDib->GetDimensions();
	int nWidth = sizeImage.cx ;
	int nHeight= sizeImage.cy ;
	
	int nSaveWidth = pDib->GetDibSaveDim().cx;
	
	// 开辟内存，存储图象数据,该数据的存储不是DWORD对齐的
	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
		}
	}
	
	// 调用LinearSharpen函数进行图象锐化增强
	LinearSharpen(pUnchImage, nWidth, nHeight) ;
	
	// 增强以后的图象拷贝到pDib中，进行显示
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
		}
	}
	
	// 释放内存
	delete []pUnchImage;
	pUnchImage = NULL  ;
	
	// 恢复光标形状
	EndWaitCursor(); 
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
}

/*************************************************************************
*
* \函数名称：
*   OnEnhanceSmoothFr()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   该函数利用低通滤波实现图象平滑
*
************************************************************************
*/
void CImageProcessingView::OnEnhanceSmoothFr() 
{
	// TODO: Add your command handler code here
	//更改光标形状
	BeginWaitCursor(); 
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}

	// 循环控制变量
	int y;
	int x;
	
	CSize sizeImage = pDib->GetDimensions();
	int nWidth = sizeImage.cx ;
	int nHeight= sizeImage.cy ;

	int nSaveWidth = pDib->GetDibSaveDim().cx;
	
	// 开辟内存，存储图象数据,该数据的存储不是DWORD对齐的
	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
		}
	}

	// 调用低通滤波函数进行图象增强
	LowPassFilterEnhance(pUnchImage, nWidth, nHeight, nWidth/16) ;
	
	// 增强以后的图象拷贝到pDib中，进行显示
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
		}
	}
	
	// 释放内存
	delete []pUnchImage;
	pUnchImage = NULL  ;
	
	// 恢复光标形状
	EndWaitCursor(); 
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
}

/*************************************************************************
*
* \函数名称：
*   OnEnhanceButtLow()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   该函数利用Butterworth低通滤波实现图象平滑
*
************************************************************************
*/
void CImageProcessingView::OnEnhanceButtLow() 
{
	// TODO: Add your command handler code here
	//更改光标形状
	BeginWaitCursor(); 
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 循环控制变量
	int y;
	int x;
	
	CSize sizeImage = pDib->GetDimensions();
	int nWidth = sizeImage.cx ;
	int nHeight= sizeImage.cy ;
	
	int nSaveWidth = pDib->GetDibSaveDim().cx;
	
	// 开辟内存，存储图象数据,该数据的存储不是DWORD对齐的
	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
		}
	}
	
	// 调用ButterWorth低通滤波函数进行图象增强
	ButterWorthLowPass(pUnchImage, nWidth, nHeight, nWidth/2) ;
	
	// 增强以后的图象拷贝到pDib中，进行显示
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
		}
	}
	
	// 释放内存
	delete []pUnchImage;
	pUnchImage = NULL  ;
	
	// 恢复光标形状
	EndWaitCursor(); 
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
}

/*************************************************************************
*
* \函数名称：
*   OnEnhanceSharpFreq()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   该函数利用高通滤波实现图象增强
*
************************************************************************
*/
void CImageProcessingView::OnEnhanceSharpFreq() 
{
	// TODO: Add your command handler code here
	//更改光标形状
	BeginWaitCursor(); 
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 循环控制变量
	int y;
	int x;
	
	CSize sizeImage = pDib->GetDimensions();
	int nWidth = sizeImage.cx ;
	int nHeight= sizeImage.cy ;
	
	int nSaveWidth = pDib->GetDibSaveDim().cx;
	
	// 开辟内存，存储图象数据,该数据的存储不是DWORD对齐的
	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
		}
	}
	
	// 调用高通滤波函数进行图象增强
	HighPassFilterEnhance(pUnchImage, nWidth, nHeight, 50) ;
	
	// 增强以后的图象拷贝到pDib中，进行显示
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
		}
	}
	
	// 释放内存
	delete []pUnchImage;
	pUnchImage = NULL  ;
	
	// 恢复光标形状
	EndWaitCursor(); 
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
}

/*************************************************************************
*
* \函数名称：
*   OnEnhanceButtHight()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   该函数利用Butterworth高通滤波实现图象平滑
*
************************************************************************
*/
void CImageProcessingView::OnEnhanceButtHight() 
{	
	//更改光标形状
	BeginWaitCursor(); 
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 循环控制变量
	int y;
	int x;
	
	CSize sizeImage = pDib->GetDimensions();
	int nWidth = sizeImage.cx ;
	int nHeight= sizeImage.cy ;
	
	int nSaveWidth = pDib->GetDibSaveDim().cx;
	
	// 开辟内存，存储图象数据,该数据的存储不是DWORD对齐的
	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
		}
	}
	
	// 调用ButterWorth高通滤波函数进行图象增强
	ButterWorthHighPass(pUnchImage, nWidth, nHeight, nWidth/2) ;
	
	// 增强以后的图象拷贝到pDib中，进行显示
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
		}
	}
	
	// 释放内存
	delete []pUnchImage;
	pUnchImage = NULL  ;
	
	// 恢复光标形状
	EndWaitCursor(); 
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
}

/*************************************************************************
*
* \函数名称：
*   OnRegionsegFix()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现并行区域分割
*
************************************************************************
*/
void CImageProcessingView::OnRegionsegFix() 
{
	// TODO: Add your command handler code here

	//更改光标形状
	BeginWaitCursor(); 

	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;

	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
					
		// 返回
		return;
	}

	// 调用函数实现固定阈值分割
	RegionSegFixThreshold(pDib,200);

	// 恢复光标形状
	EndWaitCursor(); 
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
}

/*************************************************************************
*
* \函数名称：
*   OnAdaRegionSeg()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现自适应区域分割算法
*
************************************************************************
*/
void CImageProcessingView::OnAdaRegionSeg() 
{
	// TODO: Add your command handler code here
	
	//更改光标形状
	BeginWaitCursor(); 
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 自适应区域分割
	RegionSegAdaptive(pDib);
	
	// 恢复光标形状
	EndWaitCursor(); 
	
		// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
}
/*************************************************************************
*
* \函数名称：
*   OnEdgeRoberts()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现并行边界分割－Roberts算子
*
************************************************************************
*/
void CImageProcessingView::OnEdgeRoberts() 
{
	// TODO: Add your command handler code here

	//更改光标形状
	BeginWaitCursor(); 

	// 循环控制变量
	int y;
	int x;
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;

	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;
	
	// 指向梯度数据的指针
	double * pdGrad;
	
	// 按照图像的大小开辟内存空间，存储梯度计算的结果
	pdGrad=new double[nHeight*nWidth];
	
	//图像数据的指针
	LPBYTE  pImageData = pDib->m_lpImage;
	
	// 图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();
	
	// 图像在内存中每一行象素占用的实际空间
	int nSaveWidth = sizeImageSave.cx;
	
	// 应用Roberts算子求梯度
	RobertsOperator(pDib, pdGrad);
	
	
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			if(*(pdGrad+y*nWidth+x)>50)
				*(	pImageData+y*nSaveWidth+x	)=0;
			else
				*(	pImageData+y*nSaveWidth+x	)=255;			
		}
		
  //释放梯度结果使用的内存空间
	delete pdGrad;
	pdGrad=NULL;

	// 恢复光标形状
	EndWaitCursor(); 
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
}


/*************************************************************************
*
* \函数名称：
*   OnEdgeSobel()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现并行边界分割－Sobel算子
*
************************************************************************
*/
void CImageProcessingView::OnEdgeSobel() 
{
	// TODO: Add your command handler code here
	
	//更改光标形状
	BeginWaitCursor(); 

	// 循环控制变量
	int y;
	int x;
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;

	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;
	
	// 指向梯度数据的指针
	double * pdGrad;
	
	// 按照图像的大小开辟内存空间，存储梯度计算的结果
	pdGrad=new double[nHeight*nWidth];
	
	//图像数据的指针
	LPBYTE  lpImage = pDib->m_lpImage;
	
	// 图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();
	
	// 图像在内存中每一行象素占用的实际空间
	int nSaveWidth = sizeImageSave.cx;
	
	// 应用Sobel算子求梯度
	SobelOperator(pDib, pdGrad);
	
	
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			if(*(pdGrad+y*nWidth+x)>50)
				*(	lpImage+y*nSaveWidth+x	)=0;
			else
				*(	lpImage+y*nSaveWidth+x	)=255;			
		}
		
  //释放内存空间
	delete []pdGrad;
	pdGrad=NULL;
	
	// 恢复光标形状
	EndWaitCursor(); 
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
}
/*************************************************************************
*
* \函数名称：
*   OnEdgePrewitt()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现并行边界分割－Prewitt算子
*
************************************************************************
*/
void CImageProcessingView::OnEdgePrewitt() 
{
	// TODO: Add your command handler code here
	
	//更改光标形状
	BeginWaitCursor(); 

	// 循环控制变量
	int y;
	int x;
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;

	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;
	
	// 指向梯度数据的指针
	double * pdGrad;
	
	// 按照图像的大小开辟内存空间，存储梯度计算的结果
	pdGrad=new double[nHeight*nWidth];
	
	//图像数据的指针
	LPBYTE  lpImage = pDib->m_lpImage;
	
	// 图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();
	
	// 图像在内存中每一行象素占用的实际空间
	int nSaveWidth = sizeImageSave.cx;
	
	// 应用Prewitt算子求梯度
	PrewittOperator(pDib, pdGrad);
	
	
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			if(*(pdGrad+y*nWidth+x)>50)
				*(	lpImage+y*nSaveWidth+x	)=0;
			else
				*(	lpImage+y*nSaveWidth+x	)=255;			
		}
	
  //释放内存空间
	delete []pdGrad;
	pdGrad=NULL;
	
	// 恢复光标形状
	EndWaitCursor(); 
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
		
}

/*************************************************************************
*
* \函数名称：
*   OnEdgeLaplace()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现并行边界分割－拉普拉斯算子
*
************************************************************************
*/
void CImageProcessingView::OnEdgeLaplace() 
{
	// TODO: Add your command handler code here
	
	//更改光标形状
	BeginWaitCursor(); 
	
	// 循环控制变量
	int y;
	int x;
	
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;
	
	// 指向梯度数据的指针
	double * pdGrad;
	
	// 按照图像的大小开辟内存空间，存储梯度计算的结果
	pdGrad=new double[nHeight*nWidth];
	
	//图像数据的指针
	LPBYTE  lpImage = pDib->m_lpImage;
	
	// 图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();
	
	// 图像在内存中每一行象素占用的实际空间
	int nSaveWidth = sizeImageSave.cx;
	
	// 应用Laplace算子求梯度
	LaplacianOperator(pDib, pdGrad);
	
	
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			if(*(pdGrad+y*nWidth+x)>50)
				*(	lpImage+y*nSaveWidth+x	)=0;
			else
				*(	lpImage+y*nSaveWidth+x	)=255;			
		}
	
	//释放内存空间
	delete []pdGrad;
	pdGrad=NULL;
	
	// 恢复光标形状
	EndWaitCursor(); 
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
}

/*************************************************************************
*
* \函数名称：
*   OnEdgeCanny()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现并行边界分割－Canny算子
*
************************************************************************
*/
void CImageProcessingView::OnEdgeCanny() 
{
	// TODO: Add your command handler code here

	//更改光标形状
	BeginWaitCursor(); 

	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}

	// 循环控制变量
	int y; 
	int x;
	
	CSize sizeImage = pDib->GetDimensions();
	int nWidth = sizeImage.cx ;
	int nHeight= sizeImage.cy ;

	int nSaveWidth = pDib->GetDibSaveDim().cx;

	// 开辟内存，存储图象数据
	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];

	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
		}
	}
	
	// canny算子计算后的结果
	unsigned char * pUnchEdge = new unsigned char[nWidth*nHeight];
	
	// 调用canny函数进行边界提取
	Canny(pUnchImage, nWidth, nHeight, 0.4, 0.4, 0.79, pUnchEdge) ;
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pDib->m_lpImage[y*nWidth+x]=(unsigned char)(255-pUnchEdge[y*nWidth+x]);
		}
	}

	delete []pUnchImage;
	pUnchImage = NULL  ;
	delete []pUnchEdge ;
	pUnchEdge = NULL   ;
	
	// 恢复光标形状
	EndWaitCursor(); 
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
}

/*************************************************************************
*
* \函数名称：
*   OnEdgeTrack()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现边界跟踪算法
*
************************************************************************
*/
void CImageProcessingView::OnEdgeTrack() 
{
	// TODO: Add your command handler code here

	//更改光标形状
	BeginWaitCursor(); 

	// 获得Doc类的指
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	
	CDib * pDib = pDoc->m_pDibInit;

	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	
	// 图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();
	
	// 图像在内存中每一行象素占用的实际空间
	int nSaveWidth = sizeImageSave.cx;
	
	// 遍历图象的纵坐标
	int y;
	
	// 遍历图象的横坐标
	int x;
	
	// 图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;	
	
	
	// 指向图像数据的指针
	LPBYTE lpImage ;
	lpImage = pDib->m_lpImage ;
	
	// 边界跟踪后的结果区域
	unsigned char * pUnEdgeTrack ;
	
	pUnEdgeTrack = new unsigned char[nWidth * nHeight] ;
	
	EdgeTrack(pDib, pUnEdgeTrack);
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{	
			lpImage[y*nSaveWidth + x] = (unsigned char) (255-pUnEdgeTrack[y*nWidth + x]);
		}
	}
	
	//释放内存
	delete pUnEdgeTrack;
	pUnEdgeTrack = NULL;
	
	// 恢复光标形状
	EndWaitCursor(); 
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
}

/*************************************************************************
*
* \函数名称：
*   OnRegionGrow()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现区域生长算法
*
************************************************************************
*/
void CImageProcessingView::OnRegionGrow() 
{
	// TODO: Add your command handler code here

	//更改光标形状
	BeginWaitCursor(); 
	
	// 获得Doc类的指
	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	
	CDib * pDib = pDoc->m_pDibInit;

	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 遍历图象的纵坐标
	int y;
	
	// 遍历图象的横坐标
	int x;
	
	// 图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();
	
	// 图像在内存中每一行象素占用的实际空间
	int nSaveWidth = sizeImageSave.cx;
	
	// 图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;	
	
	// 指向图像数据的指针
	LPBYTE lpImage ;
	lpImage = pDib->m_lpImage ;
	
	unsigned char * pUnchRgRst = new unsigned char[nWidth * nHeight];
	// 初始化
	memset(pUnchRgRst, 0 , sizeof(unsigned char)*nWidth*nHeight );
	
	RegionGrow(pDib, pUnchRgRst);
	
	for(y=0; y<nHeight; y++)
		for(x=0; x<nWidth; x++)
		{
			lpImage[y*nSaveWidth + x] =(unsigned char) (255-pUnchRgRst[y*nWidth+x] );
		}
		
	// 释放内存
	delete []pUnchRgRst;
	pUnchRgRst = NULL  ;
	
	// 恢复光标形状
	EndWaitCursor(); 
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
}

/*************************************************************************
 *
 * \函数名称：
 *   OnMotionBackground()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数根据运动图象提取其中的静止背景。其中运动图象要求为bmp文件，并按
 *顺序排列。
 *
 *************************************************************************
 */
void CImageProcessingView::OnMotionBackground() 
{
	// 提取背景成功标志
	BOOL bFlag;
	
	// 获取文档指针
	CImageProcessingDoc* pDoc;
	pDoc = GetDocument();

	// 获得当前打开文件的文件路径名
	CString strPathName;
	strPathName = pDoc->GetPathName();

	// 序列的总帧数
	int nTotalFrameNum = 20;

	// 图象的宽度
	int nImageWidth;
	nImageWidth = pDoc->m_pDibInit->m_lpBMIH->biWidth;	

	// 图象的高度
	int nImageHeight;
	nImageHeight = pDoc->m_pDibInit->m_lpBMIH->biHeight;

	// 图象的静止背景	
	unsigned char* pUnchBackGround;
	pUnchBackGround = new unsigned char[nImageWidth*nImageHeight];
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用GetBackground函数提取静止背景
	bFlag = GetBackground(strPathName, nTotalFrameNum, nImageWidth,nImageHeight, pUnchBackGround);
	if(bFlag == FALSE){
		return;
	}

	// 将背景设置为当前显示图象
	LPBYTE lpTemp;
	lpTemp = pDoc->m_pDibInit->m_lpImage;

	// 将数据拷贝到图象中
	memcpy(lpTemp, (LPBYTE)pUnchBackGround, nImageWidth*nImageHeight);
	
	// 恢复光标形状
	EndWaitCursor(); 

	// 释放已分配内存
	delete[]pUnchBackGround;
	pUnchBackGround = NULL;
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
}

/*************************************************************************
 *
 * \函数名称：
 *   OnRecogMatch()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   根据图象模板，在待匹配的图象中找到匹配的位置
 *
 *************************************************************************
 */
void CImageProcessingView::OnRecogMatch() 
{
	CImageProcessingDoc* pDoc;
	pDoc = GetDocument();

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的水平镜像，其它的可以类推）
	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象配准！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 打开图象识别对话框
	CDlgRecMatch* pDlg = new CDlgRecMatch(NULL, pDoc);
	pDlg->DoModal();
	
	delete pDlg;
}

void CImageProcessingView::OnDegenerationInverse() 
{
	// 图象的模糊
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的图象模糊）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象模糊！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBNoRestriction(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreInverse() 
{
	// 图象的逆滤波
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的逆滤波）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的逆滤波！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBInverseFilter(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnDegenerationMotion() 
{
	// 图象的运动模糊
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的运动模糊）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运动模糊！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBMotionDegeneration(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreMotion() 
{
	// 运动模糊图象复原
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的运动模糊复原）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运动模糊复原！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBMotionRestore(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnDEGENERATIONWinner() 
{	
	// 图象的加噪模糊
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的加噪模糊）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的加噪模糊！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBNoiseDegeneration(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}


void CImageProcessingView::OnRestoreWinner() 
{
	// 图象的维纳滤波
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的维纳滤波）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的维纳滤波！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBWinnerFilter(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnMoment() 
{
	// 图象的pq阶力矩

	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBMOMENT(pDib);
	
    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnBarycentermoment() 
{
	// 图象的重心矩

	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBBARYCENTERMOMENT(pDib);

	// 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnAnalysisHolenum() 
{
	// 消去二值图象中小于阈值面积的区域
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBHOLENUMBER(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnStreetFramework() 
{
	// 街区距离骨架提取
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBFREAMEWORK(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnStreetTransform() 
{
	// 二值图象的街区距离变换
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBSTREETDIS(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnFrameRestore() 
{
	// 街区距离骨架复原
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBCHESSBOARDDISRESTORE(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}


void CImageProcessingView::OnTrace() 
{
	// 二值图象边界跟踪

	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBTrace(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
	
}

void CImageProcessingView::OnOutline() 
{
	// 二值图象边界提取

	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBOUTLINE(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnViewBayer() 
{
	// Bayer抖动法显示图象
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::LimbPatternBayer(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnVIEWFloydSteinberg() 
{
	// Floyd-Steinberg抖动法显示图象
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DitherFloydSteinberg(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnCodingHuffman() 
{
	// 哈夫曼编码表
	
	// 获取文档
	CImageProcessingDoc * pDoc = GetDocument();
	
	// 指向源图象象素的指针
	unsigned char *	lpSrc;
		
	// 图象的高度和宽度
	LONG	lHeight;
	LONG	lWidth;
	
	// 图象每行的字节数
	LONG	lLineBytes;
	
	// 图象象素总数
	LONG	lCountSum;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 数组指针用来保存各个灰度值出现概率
	double * dProba;
	
	// 当前图象颜色数目
	int		nColorNum;

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;
			
	//图象数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// 头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（只处理8-bpp位图的霍夫曼编码）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的霍夫曼编码！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	/********************************************************************
	   开始计算各个灰度级出现的概率	

	   如果需要对指定的序列进行哈夫曼编码,
	   只要将这一步改成给各个灰度级概率赋值即可
	**********************************************************************
	*/
	
	//  由头文件信息得到图象的比特数，从而得到颜色信息
	nColorNum = (int)pow(2,lpBMIH->biBitCount);

	// 分配内存
	dProba = new double[nColorNum];
	
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth = SizeDim.cx;
	lHeight = SizeDim.cy;

	// 计算图象象素总数
	lCountSum = lHeight * lWidth;
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 计算图象每行的字节数
	lLineBytes = SizeRealDim.cx;
	
	// 赋零值
	for (i = 0; i < nColorNum; i ++)
	{
		dProba[i] = 0.0;
	}
	
	// 计算各个灰度值的计数
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			// 指向图象指针
			lpSrc = lpDIBBits + lLineBytes * i + j;
			
			// 计数加1
			dProba[*(lpSrc)] = dProba[*(lpSrc)] + 1;
		}
	}
		
	
	// 计算各个灰度值出现的概率
	for (i = 0; i < nColorNum; i ++)
	{
		dProba[i] = dProba[i] / (FLOAT)lCountSum;
	}
	
	/***************************************************
	 构建霍夫曼编码的码表
	 并用对话框显示霍夫曼码表
	****************************************************/
	
	// 创建对话框
	CDlgHuffman dlgCoding;
	
	// 初始化变量值
	dlgCoding.dProba = dProba;
	dlgCoding.nColorNum = nColorNum;
	
	// 显示对话框
	dlgCoding.DoModal();
		
	// 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnCodingShanfino() 
{
	// 香农－弗诺编码表
	
	// 获取文档
	CImageProcessingDoc * pDoc = GetDocument();
	
	// 指向源图象象素的指针
	unsigned char *	lpSrc;
		
	// 图象的高度
	LONG	lHeight;
	LONG	lWidth;
	
	// 图象每行的字节数
	LONG	lLineBytes;
	
	// 获取当前DIB颜色数目
	int		nColorNum;
	
	// 图象象素总数
	LONG	lCountSum;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 保存各个灰度值出现概率的数组指针
	double * dProba;	

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;
			
	//图象数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// 头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的香农－费诺编码）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的香农－费诺编码！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	/******************************************************************************
	// 开始计算各个灰度级出现的概率	
	//
	// 如果需要对指定的序列进行香农－弗诺编码,
	//只要将这一步改成给各个灰度级概率赋值即可
	*****************************************************************************
	*/
	//  灰度值总数的计算
	nColorNum = (int)pow(2,lpBMIH->biBitCount);

	// 分配内存
	dProba = new double[nColorNum];
	
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth = SizeDim.cx;
	lHeight = SizeDim.cy;

	// 计算图象象素总数
	lCountSum = lHeight * lWidth;
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 计算图象每行的字节数
	lLineBytes = SizeRealDim.cx;

	// 计算图象象素总数
	lCountSum = lHeight * lWidth;
	
	// 重置计数为0
	for (i = 0; i < nColorNum; i ++)
	{
		dProba[i] = 0.0;
	}
	
	// 计算各个灰度值的计数（对于非256色位图，此处给数组dProba赋值方法将不同）
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			// 指向图象指针
			lpSrc = lpDIBBits + lLineBytes * i + j;
			
			// 计数加1
			dProba[*(lpSrc)] = dProba[*(lpSrc)]+ 1;
		}
	}
	
	
	// 计算各个灰度值出现的概率
	for (i = 0; i < nColorNum; i ++)
	{
		dProba[i] /= (double)lCountSum;
	}
	
	/***************************************************
	 构建香农－费诺编码的码表
	 并用对话框显示香农－费诺码表
	****************************************************/	
	
	// 创建对话框
	CDlgShannon dlgPara;
	
	// 初始化变量值
	dlgPara.m_dProba = dProba;
	dlgPara.m_nColorNum = nColorNum;
	
	// 显示对话框
	dlgPara.DoModal();

	//释放内存
	delete dProba;
		
	// 恢复光标
	EndWaitCursor();
}

void CImageProcessingView::OnCodingArith() 
{
	CDlgArith dlgCoding;
	
	// 显示对话框
	dlgCoding.DoModal();
	
}

void CImageProcessingView::OnCodingBitplane() 
{
	// 创建对话框
	CDlgBitPlane dlgCoding;	
	
	// 显示对话框
	dlgCoding.DoModal();	
	
	BYTE bBitNum = dlgCoding.m_BItNumber;

	// 获取文档
	CImageProcessingDoc * pDoc = GetDocument();
		
	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;
	
	// 头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的位平面分解）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的位平面分解！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	DIBBITPLANE(pDib,bBitNum);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessingView::OnCodingWriteimg() 
{
	// 对当前图象进行DPCM编码（存为IMG格式文件）
	
	// 获取文档
	CImageProcessingDoc * pDoc = GetDocument();
		
	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;
			
	//图象数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// 头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（处理8-bpp位图的DPCM编码）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的DPCM编码！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 文件保存路径
	CString strFilePath;
	
	// 获取原始文件名
	strFilePath = pDoc->GetPathName();
	
	// 更改后缀为IMG
	if (strFilePath.Right(4).CompareNoCase(".BMP") == 0)
	{	
		strFilePath = strFilePath.Left(strFilePath.GetLength()-3) + "IMG";
	}
	else
	{
		strFilePath += ".IMG";
	}

	// 创建SaveAs对话框
	CFileDialog dlg(FALSE, "IMG", strFilePath, 
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		            "IMG图象文件 (*.IMG) | *.IMG|所有文件 (*.*) | *.*||", NULL);
	
	// 提示用户选择保存的路径
	if (dlg.DoModal() != IDOK)
	{
		// 恢复光标
		EndWaitCursor();
	
		return;
	}
	
	// 获取用户指定的文件路径
	strFilePath = dlg.GetPathName();
	
	// CFile和CFileException对象
	CFile file;
	CFileException fe;
	
	// 尝试创建指定的IMG文件
	if (!file.Open(strFilePath, CFile::modeCreate |
	  CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		MessageBox("打开指定IMG文件时失败！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
	
		return;
	}
	
	// 调用WRITE2IMG()函数将当前的DIB保存为IMG文件
	if (::WRITE2IMG(pDib, file))
	{
		MessageBox("成功保存为IMG文件！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		MessageBox("保存为IMG文件失败！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
	}	
		
	// 恢复光标
	EndWaitCursor();
	
}


void CImageProcessingView::OnCodingLoadimg() 
{
	// 读入IMG文件
	
	// 获取文档
	CImageProcessingDoc * pDoc = GetDocument();
		
	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 文件路径
	CString strFilePath;
	
	// 创建Open对话框
	CFileDialog dlg(TRUE, "PCX", NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					"IMG图象文件 (*.PCX) | *.IMG|所有文件 (*.*) | *.*||", NULL);
	
	// 提示用户选择保存的路径
	if (dlg.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户指定的文件路径
	strFilePath = dlg.GetPathName();
	
	// CFile和CFileException对象
	CFile file;
	CFileException fe;
	
	// 尝试打开指定的PCX文件
	if (!file.Open(strFilePath, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// 提示用户
		MessageBox("打开指定PCX文件时失败！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 调用LOADIMG()函数读取指定的IMG文件
	BOOL Succ = LOADIMG(pDib, file);

	if (Succ == TRUE)
	{
		// 提示用户
		MessageBox("成功读取IMG文件！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
	}
	else
	{
		// 提示用户
		MessageBox("读取IMG文件失败！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
	}

	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();
	
}

