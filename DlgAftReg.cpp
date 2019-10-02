// DlgAftReg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "DlgAftReg.h"
#include "CDib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAftReg dialog


CDlgAftReg::CDlgAftReg(CWnd* pParent /*=NULL*/, CDib* pDibShow)
	: CDialog(CDlgAftReg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAftReg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDib = pDibShow;
	m_bCalImgLoc =	FALSE;
	
}


void CDlgAftReg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAftReg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAftReg, CDialog)
	//{{AFX_MSG_MAP(CDlgAftReg)
	ON_WM_PAINT()
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAftReg message handlers

void CDlgAftReg::OnPaint()
{
	CPaintDC dc(this); 

	if(!m_bCalImgLoc)
		CalImgLocation();

	CSize sizeDisplay;
	CPoint pointDisplay;
	if(m_pDib != NULL){
		if(!m_pDib->IsEmpty()){
			sizeDisplay.cx=m_pDib->m_lpBMIH->biWidth;
			sizeDisplay.cy=m_pDib->m_lpBMIH->biHeight;
			pointDisplay.x = m_rectImage.left;
			pointDisplay.y = m_rectImage.top;
			m_pDib->Draw(&dc,pointDisplay,sizeDisplay);	
		}
	}
		
}

/*************************************************************************
 *
 * \函数名称：
 *   CalImageLocation()
 *
 * \输入参数:
 *   无
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数设置对话框中的控件位置和大小，并设置显示图象的位置。默认的图象大小为352×288，如果图象小于
 *此大小，则控件大小设置为352×288，并将图象放置在控件中间。
 *
 *************************************************************************
 */
void CDlgAftReg::CalImgLocation()
{
	// 获得控件IDC_DlgAftReg_IMAGE的句柄，并获得控件的初始位置信息
	CWnd* pWnd=GetDlgItem(IDC_AFTREG_IMAGE);
	WINDOWPLACEMENT *winPlacement;
	winPlacement=new WINDOWPLACEMENT;
	pWnd->GetWindowPlacement(winPlacement);

	// 图象宽度
	int nImageWidth = 0;
	
	// 图象高度
	int nImageHeight = 0;

	if(m_pDib != NULL){
		nImageWidth = m_pDib->m_lpBMIH->biWidth;
		nImageHeight = m_pDib->m_lpBMIH->biHeight;
	}	
	
	// 调整控件IDC_REG_INIT_IMAGE的大小位置，并同时设置显示基准图象的位置
	if(nImageHeight > 352){		
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + nImageHeight;	
		m_rectImage.bottom = winPlacement->rcNormalPosition.bottom;
		m_rectImage.top    = winPlacement->rcNormalPosition.top;		
	}
	else{
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + 352;
		m_rectImage.bottom = winPlacement->rcNormalPosition.top + 176 + nImageHeight/2;
		m_rectImage.top    = winPlacement->rcNormalPosition.top + 176 - nImageHeight/2;
	}
	if(nImageWidth > 288){
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + nImageWidth;
		m_rectImage.right = winPlacement->rcNormalPosition.right;
		m_rectImage.left  = winPlacement->rcNormalPosition.left;			
	}
	else{
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + 288;
		m_rectImage.right = winPlacement->rcNormalPosition.left + 144 + nImageWidth/2;
		m_rectImage.left  = winPlacement->rcNormalPosition.left + 144 - nImageWidth/2;
	}
	
	// 设置IDC_DlgAftReg_IMAGE控件的大小位置
	pWnd->SetWindowPlacement(winPlacement);	

	// 获得IDC_DlgAftReg_IMAGE控件的下边位置，以便调整其他控件的位置
	int nIniImgBottom, nIniImgRight,nIniImgLeft;
	nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	nIniImgLeft   = winPlacement->rcNormalPosition.left;
	nIniImgRight  = winPlacement->rcNormalPosition.right;

	// 设置控件IDOK的位置大小
	pWnd=GetDlgItem(IDOK);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
	
	// 设置控件IDCANCEL的位置大小
	pWnd=GetDlgItem(IDCANCEL);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);

	
	
	this->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60+ 70;
	winPlacement->rcNormalPosition.left   = nIniImgLeft   - 20;
	winPlacement->rcNormalPosition.right  = nIniImgRight  + 20;
	this->SetWindowPlacement(winPlacement);

	m_bCalImgLoc = TRUE;
	
	// 释放已分配内存
	delete winPlacement;	
}
