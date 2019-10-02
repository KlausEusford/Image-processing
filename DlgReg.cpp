// DlgReg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "ImageProcessingDoc.h"
#include "GlobalApi.h"
#include "DlgReg.h"
#include "DlgAftReg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgReg dialog


CDlgReg::CDlgReg(CWnd* pParent /*=NULL*/, CImageProcessingDoc* pDoc)
	: CDialog(CDlgReg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgReg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// 获取文档类指针
	m_pDoc = pDoc;

	// 设置计算图象位置标志位位FALSE
	m_bCalImgLoc = FALSE;

	// 设置基准图象为原始打开的图象
	m_pDibInit = pDoc->m_pDibInit;

	// 设置待配准图象
	m_pDibSamp = new CDib;

	// 设置选取特征点的数目初始值
	m_nChsFeatureNum = 0;

	// 设置选取特征点的标志位为FALSE
	m_bChoseFeature = FALSE;
}


void CDlgReg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgReg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgReg, CDialog)
	//{{AFX_MSG_MAP(CDlgReg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_REG_OPEN, OnRegOpen)
	ON_BN_CLICKED(IDC_REG_REG, OnRegReg)
	ON_BN_CLICKED(IDC_REG_CHOSE_FEATURE, OnRegChoseFeature)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgReg message handlers

void CDlgReg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// 如果还没有计算图象的位置，则进行计算
	if(!m_bCalImgLoc){
		CalImageLocation();
	}

	CSize sizeDisplay;
	CPoint pointDisplay;
	
	// 显示基准图象
	if(!m_pDibInit->IsEmpty()){
		sizeDisplay.cx=m_pDibInit->m_lpBMIH->biWidth;
		sizeDisplay.cy=m_pDibInit->m_lpBMIH->biHeight;
		pointDisplay.x = m_rectInitImage.left;
		pointDisplay.y = m_rectInitImage.top;
		m_pDibInit->Draw(&dc,pointDisplay,sizeDisplay);		
	}
	
	// 显示待配准图象
	if(!m_pDibSamp->IsEmpty()){
		sizeDisplay.cx=m_pDibSamp->m_lpBMIH->biWidth;
		sizeDisplay.cy=m_pDibSamp->m_lpBMIH->biHeight;
		pointDisplay.x = m_rectResltImage.left;
		pointDisplay.y = m_rectResltImage.top;
		m_pDibSamp->Draw(&dc,pointDisplay,sizeDisplay);		
	}

	// 显示特征点与配准的特征点
	DrawFeature(&dc);	
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
void CDlgReg::CalImageLocation()
{
	// 获得控件IDC_REG_INIT_IMAGE的句柄，并获得控件的初始位置信息
	CWnd* pWnd=GetDlgItem(IDC_REG_INIT_IMAGE);
	WINDOWPLACEMENT *winPlacement;
	winPlacement=new WINDOWPLACEMENT;
	pWnd->GetWindowPlacement(winPlacement);
	
	// 图象宽度
	int nImageWidth;
	nImageWidth = m_pDibInit->m_lpBMIH->biWidth;

	// 图象高度
	int nImageHeight;
	nImageHeight = m_pDibInit->m_lpBMIH->biHeight;

	
	// 调整控件IDC_REG_INIT_IMAGE的大小位置，并同时设置显示基准图象的位置
	if(nImageHeight > 352){
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + nImageHeight;	
		m_rectInitImage.bottom = winPlacement->rcNormalPosition.bottom;
		m_rectInitImage.top    = winPlacement->rcNormalPosition.top;		
	}
	else{
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + 352;
		m_rectInitImage.bottom = winPlacement->rcNormalPosition.top + 176 + nImageHeight/2;
		m_rectInitImage.top    = winPlacement->rcNormalPosition.top + 176 - nImageHeight/2;
	}
	if(nImageWidth > 288){
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + nImageWidth;
		m_rectInitImage.right = winPlacement->rcNormalPosition.right;
		m_rectInitImage.left  = winPlacement->rcNormalPosition.left;			
	}
	else{
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + 288;
		m_rectInitImage.right = winPlacement->rcNormalPosition.left + 144 + nImageWidth/2;
		m_rectInitImage.left  = winPlacement->rcNormalPosition.left + 144 - nImageWidth/2;
	}

	// 设置IDC_REG_INIT_IMAGE控件的大小位置
	pWnd->SetWindowPlacement(winPlacement);	
			
	// 获得显示基准图象控件的右边位置，以便确认显示待配准图象控件的位置
	int nIniImgRight;
	nIniImgRight = winPlacement->rcNormalPosition.right;
	
	int  nIniImgLeft;
	nIniImgLeft   = winPlacement->rcNormalPosition.left;

	// 获得IDC_REG_INIT_IMAGE控件的下边位置，以便调整其他控件的位置
	int nIniImgBottom;
	nIniImgBottom = winPlacement->rcNormalPosition.bottom;

	// 获得控件IDC_REG_RESLT_IMAGE的句柄，并获得初始位置信息
	pWnd=GetDlgItem(IDC_REG_RESLT_IMAGE);
	pWnd->GetWindowPlacement(winPlacement);
	
	// 如果还未打开待配准图象，则设置待配准图象大小和基准图象大小相等
	if(!m_pDibSamp->IsEmpty()){
		nImageWidth  = m_pDibSamp->m_lpBMIH->biWidth;
		nImageHeight = m_pDibSamp->m_lpBMIH->biHeight;
	}
	
	// 调整控件IDC_REG_RESLT_IMAGE的大小位置，并同时设置显示待配准图象的位置

	// 先调整控件的左边位置，和IDC_REG_INIT_IMAGE控件相隔15个象素
	winPlacement->rcNormalPosition.left = nIniImgRight + 15;

	if(nImageHeight > 352){		
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + nImageHeight;	
		m_rectResltImage.bottom = winPlacement->rcNormalPosition.bottom;
		m_rectResltImage.top    = winPlacement->rcNormalPosition.top;		
	}
	else{
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + 352;
		m_rectResltImage.bottom = winPlacement->rcNormalPosition.top + 176 + nImageHeight/2;
		m_rectResltImage.top    = winPlacement->rcNormalPosition.top + 176 - nImageHeight/2;
	}
	if(nImageWidth > 288){
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + nImageWidth;
		m_rectResltImage.right = winPlacement->rcNormalPosition.right;
		m_rectResltImage.left  = winPlacement->rcNormalPosition.left;			
	}
	else{
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + 288;
		m_rectResltImage.right = winPlacement->rcNormalPosition.left + 144 + nImageWidth/2;
		m_rectResltImage.left  = winPlacement->rcNormalPosition.left + 144 - nImageWidth/2;
	}

	// 设置IDC_REG_RESLT_IMAGE控件的大小位置
	pWnd->SetWindowPlacement(winPlacement);
	
	if(nIniImgBottom < winPlacement->rcNormalPosition.bottom)
		nIniImgBottom = winPlacement->rcNormalPosition.bottom;


	nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	
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

	// 设置控件IDC_REG_OPEN的位置大小
	pWnd=GetDlgItem(IDC_REG_OPEN);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
	
	// 设置控件IDC_REG_REG的位置大小
	pWnd=GetDlgItem(IDC_REG_REG);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
	
	
	// 设置控件IDC_REG_CHOSE_FEATUR的位置大小
	pWnd=GetDlgItem(IDC_REG_CHOSE_FEATURE);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);

	// 调整此对话框的大小
	//pWnd = GetDlgItem(IDD_DLG_REG);
	this->GetWindowPlacement(winPlacement);
	//winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 300;
	winPlacement->rcNormalPosition.left   = nIniImgLeft   - 20;
	winPlacement->rcNormalPosition.right  = nIniImgRight  + 20;
	this->SetWindowPlacement(winPlacement);

	// 释放已分配内存
	delete winPlacement;

	// 设置计算图象控件位置标志位为TRUE
	m_bCalImgLoc = TRUE;
}

/*************************************************************************
 *
 * \函数名称：
 *   DrawFeature()
 *
 * \输入参数:
 *   无
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数根据类的成员变量确定特征点的数目和位置，并在图象中进行显示。
 *
 *************************************************************************
 */
void CDlgReg::DrawFeature(CDC* pDC)
{
	// 循环变量
	int i;

	// 临时变量
	CPoint pointTemp;

	// 半径
	int nRadius;
	nRadius = 5;

	// 设置画图类型
	pDC->SelectStockObject(HOLLOW_BRUSH);

	// 声明画笔
	CPen penWhite(PS_SOLID,1,RGB(255,255,255));
	CPen *pOldPen;

	// 将画笔选入，并保存以前的画笔
	pOldPen = pDC->SelectObject(&penWhite);

	for(i=0; i<m_nChsFeatureNum; i++){
		// 首先显示特征点
		
		// 确定此点的显示位置
		pointTemp.x = m_pPointSampl[i].x + m_rectResltImage.left;
		pointTemp.y = m_pPointSampl[i].y + m_rectResltImage.top ;		

		// 画出此特征点，其中园的半径为nRadius
		CRect rectSamp(pointTemp.x-nRadius , pointTemp.y-nRadius , 
			pointTemp.x+nRadius , pointTemp.y+nRadius);
		pDC->Ellipse(rectSamp);

		// 再显示配准特征点
		// 确定此点的显示位置
		pointTemp.x = m_pPointBase[i].x + m_rectInitImage.left;
		pointTemp.y = m_pPointBase[i].y + m_rectInitImage.top ;		

		// 画出此特征点，其中园的半径为nRadius
		CRect rectBase(pointTemp.x-nRadius , pointTemp.y-nRadius , 
			pointTemp.x+nRadius , pointTemp.y+nRadius);
		pDC->Ellipse(rectBase);
	}

	// 回复以前的画笔
	pDC->SelectObject(pOldPen);
	penWhite.DeleteObject();
}

/*************************************************************************
 *
 * \函数名称：
 *   OnRegOpen()
 *
 * \输入参数:
 *   无
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数打开待配准图象，并将图象存放在m_pDibSamp中。
 *
 *************************************************************************
 */
void CDlgReg::OnRegOpen() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,"bmp","*.bmp");
	if(dlg.DoModal() == IDOK)
	{
 
	 	CFile file;
	 
	 	CString strPathName;
 
		strPathName = dlg.GetPathName();
 
		// 打开文件
		if( !file.Open(strPathName, CFile::modeRead | CFile::shareDenyWrite))
		{
			// 返回	
			return ;
		}

		// 读入模板图象
		if(!m_pDibSamp->Read(&file)){
			// 恢复光标形状
			EndWaitCursor();

			// 清空已分配内存
			m_pDibSamp->Empty();

			// 返回
 			return;
		}
	}
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的模板配准，其它的可以类推）
	if(m_pDibSamp->m_nColorTableEntries != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 清空已分配内存
		m_pDibSamp->Empty();

		// 返回
		return;
	}

	// 如果打开新的待配准文件，将图象位置设置标志位设为FALSE，以便再次调整位置
	m_bCalImgLoc = FALSE;

	// 更新显示
	this->UpdateData();
	this->Invalidate();
}



/*************************************************************************
 *
 * \函数名称：
 *   OnRegChoseFeatureOnRegReg()
 *
 * \输入参数:
 *   无
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数设置选取特征点标志位，然后调用函数在待配准图象中选取特征点，并
 *配准这些特征点。特征点的数目至少应该选取三个。
 *
 *************************************************************************
 */
void CDlgReg::OnRegChoseFeature() 
{
	// 如果待配准图象尚未打开，则不能进行特征点选取工作
	if((m_pDibSamp->IsEmpty())){
		AfxMessageBox("尚未打开待配准图象文件，请打开待配准图象");
		return;
	}
	// 设置选取特征点标志位
	m_bChoseFeature = TRUE;	
	
	AfxMessageBox("请在待配准图象中选取特征点");
}

/*************************************************************************
 *
 * \函数名称：
 *   OnLButtonUp()
 *
 * \输入参数:
 *   无
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数根据鼠标所标定的位置设置特征点。然后调用特征配准函数配准此特征点。
 *特征点的数目至少应该选取三个。
 *
 *************************************************************************
 */
void CDlgReg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// 循环变量
	int i,j;

	// 如果特征选取标志位为TRUE，则进行特征点的选取和配准，否则退出
	if(!m_bChoseFeature){
		return;
	}

	// 待配准图象的特征选取区域，在这里选择特征点的选择区域要比图象的区
	// 域小一圈
	CRect rectChoose;
	rectChoose.bottom = m_rectResltImage.bottom - 5;
	rectChoose.top    = m_rectResltImage.top + 5;
	rectChoose.left   = m_rectResltImage.left + 5;
	rectChoose.right  = m_rectResltImage.right - 5;

	// 特征点的区域
	CRect rectFeature;

	// 标志位，表示此点是否是已经选择的特征点
	BOOL bFlag = FALSE;

	// 判断此点是否合法，并判断此点是否已经选择，如果是，则去掉此点
	if(rectChoose.PtInRect(point))
	{
		// 如果所选择的特征点是以前的特征点，则去掉此点
		for( i = 0; i<m_nChsFeatureNum; i++){

			// 选择特征点的显示区域，以便对特征点进行取舍
			rectFeature.bottom = m_pPointSampl[i].y + m_rectResltImage.top + 5;
			rectFeature.top    = m_pPointSampl[i].y + m_rectResltImage.top - 5;
			rectFeature.left   = m_pPointSampl[i].x + m_rectResltImage.left- 5;
			rectFeature.right  = m_pPointSampl[i].x + m_rectResltImage.left+ 5;

			// 判断所选择的特征点是否为原来选择的特征点
			// 如果是，则去掉此特征点
			if(rectFeature.PtInRect(point)){
				// 将后面的特征点向前移动一位，去掉所选择的此特征点
				for(j=i; j<m_nChsFeatureNum-1; j++){
					m_pPointSampl[j] = m_pPointSampl[j+1];
					m_pPointBase[j]  = m_pPointBase[j+1];						
				}

				// 将特征点的计数减一
				m_nChsFeatureNum--;
				
				// 更新显示
				Invalidate();
				
				// 设置标志位
				bFlag = TRUE;

				// 退出
				return;
			}
		}
		
		// 在判断特征点是否已经选取够了
		if(m_nChsFeatureNum == 3){
			AfxMessageBox("你已经选取了3个特征点，如果要继续选取，你可以去掉配准不正确的特征点在进行选取");
			return;
		}

		// 如果此点是需要选取的，则进行相关操作
		if(!bFlag){
			// 将此待配准特征点选取，注意特征点的坐标是以图象的左上角为原点确定的
			m_pPointSampl[m_nChsFeatureNum].x = point.x - m_rectResltImage.left;
			m_pPointSampl[m_nChsFeatureNum].y = point.y - m_rectResltImage.top;
			
			// 配准此特征点
			m_pPointBase[m_nChsFeatureNum] = FindMatchPoint(m_pDibInit, m_pDibSamp, 
				m_pPointSampl[m_nChsFeatureNum]);

			// 将特征点计数加一
			m_nChsFeatureNum++;
		}
			
		
	}	

	// 更新显示
	Invalidate();

	CDialog::OnLButtonUp(nFlags, point);
}

/*************************************************************************
 *
 * \函数名称：
 *   OnLButtonMove()
 *
 * \输入参数:
 *   无
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   如果特征点选取标志位为TRUE，则该函数将待配准区域的鼠标设置为十字形状，
 *以便能更精确的定位特征点。
 *
 *************************************************************************
 */
void CDlgReg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// 如果不是在特征点选取状态，则不进行相关的操作
	if(m_bChoseFeature){
		// 如果鼠标在待配准图象区域中，则更改鼠标形状
		if(m_rectResltImage.PtInRect(point))
		{
			::SetCursor(LoadCursor(NULL,IDC_CROSS));
		}
		else
		{
			::SetCursor(LoadCursor(NULL,IDC_ARROW));
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

/*************************************************************************
 *
 * \函数名称：
 *   FindMatchPoint()
 *
 * \输入参数:
 *   CDib*	pDibBase	- 基准图象指针
 *   CDib*	pDibSamp	- 待配准图象指针
 *   CPoint	pointSamp	- 待配准特征点的位置
 *
 * \返回值:
 *   CPoint			- 返回在基准图象中配准的特征点的位置
 *
 * \说明:
 *   该函数根据待待配准图象中的特征点位置在基准图象中寻找配准特征点，并将
 *配准的特征点位置返回。在配准的过程中，采取的是块配准的方法进行配准，在这里
 *选取块的大小为7*7。搜索的方法为全局搜索。
 *
 *************************************************************************
 */
CPoint CDlgReg::FindMatchPoint(CDib* pDibBase, CDib* pDibSamp, 
			       CPoint pointSamp)
{
	// 循环变量
	int i,j,m,n;

	// 临时变量
	int nX, nY;

	// 配准特征点位置
	CPoint pointBase;

	// 配准数据块的尺寸
	int nBlockLen = 7;
	int nBlockHalfLen =3;

	// 基准图象数据指针	
	unsigned char* pBase;
	pBase = (unsigned char *)pDibBase->m_lpImage;
	
	// 待配准图象数据指针
	unsigned char* pSamp;
	pSamp = (unsigned char *)pDibSamp->m_lpImage;		

	// 特征点位置的数据配准块
	unsigned char* pUnchSampBlock;
	pUnchSampBlock = new unsigned char[nBlockLen*nBlockLen];

	// 临时分配内存，用于存放配准数据块
	unsigned char* pUnchBaseBlock;
	pUnchBaseBlock = new unsigned char[nBlockLen*nBlockLen];

	// 相似度
	double dbCor;

	// 最大相似度
	double dbMaxCor = 0;

	// 基准图象的存储大小
	CSize sizeBaseImg;
	sizeBaseImg = pDibBase->GetDibSaveDim();

	// 待配准图象的存储大小
	CSize sizeSampImg;
	sizeSampImg = pDibSamp->GetDibSaveDim();

	// 从待配准图象中提取以特征点为中心的nBlockLen*nBlockLen0的数据块
	for(i=-nBlockHalfLen; i<=nBlockHalfLen; i++){
		for(j=-nBlockHalfLen; j<=nBlockHalfLen; j++){
			// 计算此点在图象中的位置
			nX = pointSamp.x + i;
			nY = sizeSampImg.cy - pointSamp.y + j +1;

			// 提取图象数据
			pUnchSampBlock[(j+nBlockHalfLen)*nBlockLen + (i+nBlockHalfLen)] = 
				pSamp[nY*sizeSampImg.cx + nX];				
		}
	}

	// 基准图象的高度和宽度
	int nBaseImgHeight, nBaseImgWidth;
	nBaseImgHeight = pDibBase->m_lpBMIH->biHeight;
	nBaseImgWidth  = pDibBase->m_lpBMIH->biWidth;

	// 在基准图象中寻找配准特征点，采取的搜索方法为全局搜索
	for(m = nBlockHalfLen; m< nBaseImgHeight-nBlockHalfLen; m++){
		for(n=nBlockHalfLen; n<nBaseImgWidth-nBlockHalfLen; n++){
			// 提取以此点为中心，大小为nBlockLen*nBlockLen的数据块
			for(i=-nBlockHalfLen; i<=nBlockHalfLen; i++){
				for(j=-nBlockHalfLen; j<=nBlockHalfLen; j++){
					// 计算此点在图象中存储的位置
					nX = n + i;
					nY = sizeBaseImg.cy - m + j - 1;

					// 提取图象数据
					pUnchBaseBlock[(j+nBlockHalfLen)*nBlockLen + (i+nBlockHalfLen)] = 
						pBase[nY*sizeBaseImg.cx + nX];				
				}
			}

			// 对这两个数据块进行配准，计算相似度
			dbCor = CalCorrelation(pUnchBaseBlock, pUnchSampBlock, nBlockLen);

			// 判断是否为最大相似度，如果是，则记录此相似度和配准特征点位置
			if(dbCor > dbMaxCor){
				dbMaxCor  = dbCor;
				pointBase.x = n;
				pointBase.y = m;
			}

		}
	}
	
	return pointBase;
}

/*************************************************************************
 *
 * \函数名称：
 *   CalCorrelation()
 *
 * \输入参数:
 *   unsigned char*	pBase		- 基准图象数据指针
 *   unsigned char*	pSamp		- 待配准图象数据指针
 *   int		nBlockLen	- 配准数据块的尺度大小
 *
 * \返回值:
 *   double			- 返回两个数据块配准的相似度
 *
 * \说明:
 *   该函数对给定的两个大小为nBlockLen*nBlockLen的数据块，计算两者之间的
 *的配准相似度。其中，去掉均值以消除亮度变换的影响。
 *
 *************************************************************************
 */
double CDlgReg::CalCorrelation(unsigned char* pBase, unsigned char* pSamp, int nBlockLen)
{
	// 临时变量
	double dbSelfBase=0,dbSelfSamp=0;

	// 相似度
	double dbCor=0;

	// 块均值
	double dbMeanBase=0,dbMeanSamp=0;

	// 计算两个块的平均值
	for(int i=0;i<nBlockLen;i++)
		for(int j=0;j<nBlockLen;j++)
		{
			dbMeanBase += pBase[j*nBlockLen + i];
			dbMeanSamp += pSamp[j*nBlockLen + i];
		}
	dbMeanBase = dbMeanBase/(nBlockLen*nBlockLen);
	dbMeanSamp = dbMeanSamp/(nBlockLen*nBlockLen);

	// 求取相似度
	for(i=0;i<nBlockLen;i++)
		for(int j=0;j<nBlockLen;j++)
		{
			dbSelfBase += (pBase[j*nBlockLen + i] - dbMeanBase)*
				(pBase[j*nBlockLen + i]  - dbMeanBase);
			dbSelfSamp += (pSamp[j*nBlockLen + i] - dbMeanSamp)*
				(pSamp[j*nBlockLen + i]  - dbMeanSamp);
			dbCor += (pBase[j*nBlockLen + i] - dbMeanBase)*
				(pSamp[j*nBlockLen + i]  - dbMeanSamp);
		}
	dbCor = dbCor / sqrt(dbSelfBase * dbSelfSamp);

	// 返回相似度
	return dbCor;
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
 *   该函数对两幅图象m_pDibInit和m_pDibSamp进行配准。其中，需要先确定特征点，
 *特征点至少需要选取3个以上，这一步骤需要在“选取特征点”步骤中进行。根据选取
 *的特征点，计算得到仿射参数。最后，将配准的两幅图象拼接起来显示。
 *
 *************************************************************************
 */
void CDlgReg::OnRegReg() 
{
	// 进行配准之前，判断是否已经选取特征点
	if(!m_bChoseFeature){
		AfxMessageBox("还没有选取特征点，请先选取特征点");
		return;
	}

	// 如果选取的特征点数目不够，也不能进行处理
	if(m_nChsFeatureNum < 3){
		AfxMessageBox("请选择三个特征点，再进行图象配准");
		return;
	}

	// 仿射变换系数，仿射变换的系数为6个。此系数为从基准图象到待配准图象的变换系数
	double *pDbBs2SpAffPara;
	pDbBs2SpAffPara = new double[2*3];

	// 仿射变换系数，仿射变换的系数为6个。此系数为从待配准图象到基准图象的变换系数
	double *pDbSp2BsAffPara;
	pDbSp2BsAffPara = new double[2*3];

	// 如果已经选取了特征点，并得到了配准的特征点，则可以计算仿射变换系数了
	GetAffinePara(m_pPointBase, m_pPointSampl, pDbSp2BsAffPara);
	
	// 计算从待配准图象到基准图象的仿射变换系数
	GetAffinePara(m_pPointSampl, m_pPointBase, pDbBs2SpAffPara);

	// 计算图象经过仿射变换后的尺寸大小
	CRect rectAftAff;
	rectAftAff = GetAftAffDim(pDbSp2BsAffPara);
	
	// 根据图象的尺寸大小创建新的图象
	//CreateDIB();
	int nNewImgWidth, nNewImgHeight;
	nNewImgWidth  = rectAftAff.right - rectAftAff.left;
	nNewImgHeight = rectAftAff.bottom- rectAftAff.top;

	
	// 将基准图象放入新的图象中
	LPBYTE lpBaseImg;
	lpBaseImg = SetBaseImgAftAff(rectAftAff);

	// 将待配准图象放入新的图象中
	LPBYTE lpSampImg;
	lpSampImg = SetSampImgAftAff(pDbBs2SpAffPara, rectAftAff);

	// 将此图象用CDib类封装
	m_pDibResult = new CDib(CSize(nNewImgWidth,nNewImgHeight), 8);

	// 计算结果图象的存储大小尺寸
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// 拷贝调色板
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// 应用调色板
	m_pDibResult->MakePalette();
	
	// 分配内存给合并后的图象
	LPBYTE lpImgResult;
	lpImgResult = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];

	// 对图象进行赋值
	for( int i=0; i<nNewImgWidth; i++)
		for( int j=0; j<nNewImgHeight; j++){
			int nX = i;
			int nY = sizeSaveResult.cy - j - 1;
			lpImgResult[nY*sizeSaveResult.cx + nX] = (lpBaseImg[j*nNewImgWidth + i] +lpSampImg[j*nNewImgWidth + i])/2;
		}

	// 将指针赋值给CDib类的数据
	m_pDibResult->m_lpImage = lpImgResult;

	// 显示合并后的图象
	CDlgAftReg* pDlg;
	pDlg = new CDlgAftReg(NULL, m_pDibResult);
	pDlg->DoModal();
	
	// 删除对象
	delete pDlg;

	// 释放已分配内存
	delete[]lpBaseImg;
	delete[]lpSampImg;
	delete[]pDbBs2SpAffPara;
	delete[]pDbSp2BsAffPara;
}

/*************************************************************************
 *
 * \函数名称：
 *   GetAffinePara()
 *
 * \输入参数:
 *   double  *pDbBs2SpAffPara	- 用于存放基准图象到待配准图象的仿射变换系数
 *   double  *pDbSp2BsAffPara	- 用于存放待配准图象到基准图象的仿射变换系数
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数根据得到的三对配准的特征点，计算仿射变换系数。得到的仿射变换系数
 *存放在两个输入参数所指向的内存中。
 *
 *************************************************************************
 */
void CDlgReg::GetAffinePara(CPoint* pPointBase, CPoint* pPointSampl, double* pDbAffPara)
{
	// pDbBMatrix中存放的是基准图象中特征点的坐标，
	// 大小为2*m_nChsFeatureNum，前m_nChsFeatureNum为X坐标
	double *pDbBMatrix;
	pDbBMatrix = new double[2*m_nChsFeatureNum];

	// pDbSMatrix中存放的是待配准图象中特征点的扩展坐标
	// 大小为3*m_nChsFeatureNum，其中前m_nChsFeatureNum为X坐标
	// 中间m_nChsFeatureNum个为Y坐标，后面m_nChsFeatureNum为1
	double *pDbSMatrix;
	pDbSMatrix = new double[3*m_nChsFeatureNum];

	// pDbSMatrixT中存放的pDbSMatrix的转置矩阵，
	// 大小为m_nChsFeatureNum*3
	double *pDbSMatrixT;
	pDbSMatrixT = new double[m_nChsFeatureNum*3];

	// pDbInvMatrix为临时变量，存放的是pDbSMatrix*pDbSMatrixT的逆
	// 大小为3*3
	double *pDbInvMatrix;
	pDbInvMatrix = new double[3*3];

	// 临时内存
	double *pDbTemp;
	pDbTemp = new double[2*3];

	// 循环变量
	int count;

	// 给矩阵赋值
	for(count = 0; count<m_nChsFeatureNum; count++)
	{
		pDbBMatrix[0*m_nChsFeatureNum + count] = pPointBase[count].x;
		pDbBMatrix[1*m_nChsFeatureNum + count] = pPointBase[count].y;
		pDbSMatrix[0*m_nChsFeatureNum + count] = pPointSampl[count].x;
		pDbSMatrix[1*m_nChsFeatureNum + count] = pPointSampl[count].y;
		pDbSMatrix[2*m_nChsFeatureNum + count] = 1;
		pDbSMatrixT[count*m_nChsFeatureNum + 0] = pPointSampl[count].x;
		pDbSMatrixT[count*m_nChsFeatureNum + 1] = pPointSampl[count].y;
		pDbSMatrixT[count*m_nChsFeatureNum + 2] = 1;		
	}

	// 计算pDbSMatrix*pDbSMatrixT，并将结果放入pDbInvMatrix中
	CalMatProduct(pDbSMatrix,pDbSMatrixT,pDbInvMatrix,3,3,m_nChsFeatureNum);

	// 计算pDbInvMatrix的逆矩阵
	CalInvMatrix(pDbInvMatrix, 3);

	// 计算仿射变换系数
	CalMatProduct(pDbBMatrix, pDbSMatrixT, pDbTemp, 2, 3, m_nChsFeatureNum);
	CalMatProduct(pDbTemp, pDbInvMatrix, pDbAffPara, 2, 3, 3);

	// 释放内存
	delete[]pDbBMatrix;
	delete[]pDbSMatrix;
	delete[]pDbSMatrixT;
	delete[]pDbInvMatrix;
	delete[]pDbTemp;

}

/*************************************************************************
 *
 * \函数名称：
 *   CalMatProduct()
 *
 * \输入参数:
 *   double  *pDbSrc1	- 指向相乘矩阵1的内存
 *   double  *pDbSrc2	- 指向相乘矩阵2的内存
 *   double  *pDbDest   - 存放矩阵相乘运行结果的内存指针
 *   int     nX		- 矩阵的尺寸，具体参见函数说明
 *   int     nY		- 矩阵的尺寸，具体参见函数说明
 *   int     nZ		- 矩阵的尺寸，具体参见函数说明
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数计算两个矩阵的相乘，然后将相乘的结果存放在pDbDest中。其中pDbSrc1
 *的大小为nX*nZ，pDbSrc2的大小为nZ*nY，pDbDest的大小为nX*nY
 *
 *************************************************************************
 */
void CDlgReg::CalMatProduct(double* pDbSrc1, double *pDbSrc2, double *pDbDest, int y, int x, int z)
{
	// 循环变量
	//int i,j,m;
/*
	// 矩阵相乘
	for( i=0;i<nX;i++)
		for( j=0;j<nY;j++)
		{
			pDbDest[i*nY + j] = 0;
			for( m=0; m<nZ; m++)
				pDbDest[i*nY + j] += pDbSrc1[i*nZ + m]*pDbSrc2[m *nY + j];
		}
		*/
	for(int i=0;i<y;i++)
		for(int j=0;j<x;j++)
		{
			pDbDest[i*x + j] = 0;
			for(int m=0;m<z;m++)
				pDbDest[i*x + j] += pDbSrc1[i*z + m]*pDbSrc2[m*x + j];
		}
}

/*************************************************************************
 *
 * \函数名称：
 *   CalInvMatrix()
 *
 * \输入参数:
 *   double  *pDbSrc	- 指向矩阵的指针
 *   int     nLen	- 矩阵的尺寸 
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数计算矩阵pDbSrc的逆矩阵，其中pDbSrc的大小为nLen*nLen
 *
 *************************************************************************
 */
BOOL CDlgReg::CalInvMatrix(double *pDbSrc, int nLen)
{
	int *is,*js,i,j,k;
	double d,p;
	is = new int[nLen];
	js = new int[nLen];
	for(k=0;k<nLen;k++)
	{
		d=0.0;
		for(i=k;i<nLen;i++)
			for(j=k;j<nLen;j++)
			{
				p=fabs(pDbSrc[i*nLen + j]);
				if(p>d)
				{
					d     = p; 
					is[k] = i;
					js[k] = j;
				}
			}
		if(d+1.0==1.0)
		{
			delete is;
			delete js;
			return FALSE;
		}
		if(is[k] != k)
			for(j=0;j<nLen;j++)
			{
				p = pDbSrc[k*nLen + j];
				pDbSrc[k*nLen + j] = pDbSrc[(is[k]*nLen) + j];
				pDbSrc[(is[k])*nLen + j] = p;
			}
		if(js[k] != k)
			for(i=0; i<nLen; i++)
			{
				p = pDbSrc[i*nLen + k];
				pDbSrc[i*nLen + k] = pDbSrc[i*nLen + (js[k])];
				pDbSrc[i*nLen + (js[k])] = p;
			}

		pDbSrc[k*nLen + k]=1.0/pDbSrc[k*nLen + k];
		for(j=0; j<nLen; j++)
			if(j != k)
			{
				pDbSrc[k*nLen + j]*=pDbSrc[k*nLen + k];
			}
		for(i=0; i<nLen; i++)
			if(i != k)
				for(j=0; j<nLen; j++)
					if(j!=k)
					{
						pDbSrc[i*nLen + j] -= pDbSrc[i*nLen + k]*pDbSrc[k*nLen + j];
					}
		for(i=0; i<nLen; i++)
			if(i != k)
			{
				pDbSrc[i*nLen + k] *= -pDbSrc[k*nLen + k];
			}
	}
	for(k=nLen-1; k>=0; k--)
	{
		if(js[k] != k)
			for(j=0; j<nLen; j++)
			{
				p = pDbSrc[k*nLen + j];
				pDbSrc[k*nLen + j] = pDbSrc[(js[k])*nLen + j];
				pDbSrc[(js[k])*nLen + j] = p;
			}
		if(is[k] != k)
			for(i=0; i<nLen; i++)
			{
				p = pDbSrc[i*nLen + k];
				pDbSrc[i*nLen + k] = pDbSrc[i*nLen +(is[k])];
				pDbSrc[i*nLen + (is[k])] = p;
			}
	}
	delete is;

	return TRUE;	
}

/*************************************************************************
 *
 * \函数名称：
 *   CalInvMatrix()
 *
 * \输入参数:
 *   double  *pDbAffPara	- 仿射变换系数矩阵
 *
 * \返回值:
 *   CRect			- 返回待配准图象经仿射变换后的区域
 *
 * \说明:
 *   该函数根据仿射变换系数，计算待配准图象仿射变换后的图象尺寸大小
 *
 *************************************************************************
 */
CRect CDlgReg::GetAftAffDim(double* pDbAffPara)
{
	// 基准图象的宽度和高度
	int nBaseImgWidth, nBaseImgHeight;
	nBaseImgWidth = m_pDibInit->m_lpBMIH->biWidth;
	nBaseImgHeight= m_pDibInit->m_lpBMIH->biHeight;

	// 待配准图象的宽度和高度
	int nSamplImgWidth, nSamplImgHeight;
	nSamplImgWidth = m_pDibSamp->m_lpBMIH->biWidth;
	nSamplImgHeight= m_pDibSamp->m_lpBMIH->biHeight;

	// 基准图象的原始区域
	CRect rectBase(0,0,nBaseImgWidth,nBaseImgHeight);

	// 临时变量
	CPoint pointTemp;
	double tx,ty;
	
	// 图象的端点
	pointTemp.x = 0; 
	pointTemp.y = 0;

	// 计算点pointTemp经过仿射变换后的坐标
	tx = pDbAffPara[0*3 +0]*pointTemp.x + 
		pDbAffPara[0*3 + 1]*pointTemp.y + pDbAffPara[0*3 + 2];
	ty = pDbAffPara[1*3 + 0]*pointTemp.x + 
		pDbAffPara[1*3 + 1]*pointTemp.y + pDbAffPara[1*3 + 2];

	// 判断pointTemp经过仿射变换后是否超出原来的大小
	if(tx<rectBase.left)
		rectBase.left = (int)tx;
	if(tx>rectBase.right)
		rectBase.right = (int)tx+1;
	if(ty<rectBase.top)
		rectBase.top = (int)ty;
	if(ty>rectBase.bottom)
		rectBase.bottom = (int)ty+1;

	// 计算端点(0, nSamplImgHeight)变换后的坐标
	pointTemp.x = 0; pointTemp.y = nSamplImgHeight;
	tx = pDbAffPara[0*3 + 0]*pointTemp.x + 
		pDbAffPara[0*3 + 1]*pointTemp.y + pDbAffPara[0*3 + 2];
	ty = pDbAffPara[1*3 +0]*pointTemp.x +
		pDbAffPara[1*3 + 1]*pointTemp.y + pDbAffPara[1*3 + 2];

	// 判断是否越界
	if(tx<rectBase.left)
		rectBase.left = (int)tx;
	if(tx>rectBase.right)
		rectBase.right = (int)tx+1;
	if(ty<rectBase.top)
		rectBase.top = (int)ty;
	if(ty>rectBase.bottom)
		rectBase.bottom = (int)ty+1;

	// 计算端点(nSamplImgWidth, nSamplImgHeight)变换后的坐标
	pointTemp.x = nSamplImgWidth; pointTemp.y = nSamplImgHeight;
	tx = pDbAffPara[0*3 + 0]*pointTemp.x +
		pDbAffPara[0*3 + 1]*pointTemp.y + pDbAffPara[0*3 + 2];
	ty = pDbAffPara[1*3 + 0]*pointTemp.x +
		pDbAffPara[1*3 + 1]*pointTemp.y + pDbAffPara[1*3 + 2];

	// 判断是否越界
	if(tx<rectBase.left)
		rectBase.left = (int)tx;
	if(tx>rectBase.right)
		rectBase.right = (int)tx+1;
	if(ty<rectBase.top)
		rectBase.top = (int)ty;
	if(ty>rectBase.bottom)
		rectBase.bottom = (int)ty+1;

	// 计算端点(nSamplImgWidth, 0)变换后的坐标
	pointTemp.x = nSamplImgWidth; pointTemp.y = 0;
	tx = pDbAffPara[0*3 + 0]*pointTemp.x +
		pDbAffPara[0*3 + 1]*pointTemp.y + pDbAffPara[0*3 + 2];
	ty = pDbAffPara[1*3 + 0]*pointTemp.x +
		pDbAffPara[1*3 + 1]*pointTemp.y + pDbAffPara[1*3 + 2];

	// 判断是否越界
	if(tx<rectBase.left)
		rectBase.left = (int)tx;
	if(tx>rectBase.right)
		rectBase.right = (int)tx+1;
	if(ty<rectBase.top)
		rectBase.top = (int)ty;
	if(ty>rectBase.bottom)
		rectBase.bottom = (int)ty+1;

	// 返回待配准图象变换后的区域大小
	return(rectBase);

}

/*************************************************************************
 *
 * \函数名称：
 *   SetSampImgAftAff()
 *
 * \输入参数:
 *   double  *pDbAffPara	- 仿射变换系数矩阵
 *   CRect   rectNewImg		- 变换后图象的大小尺寸
 *
 * \返回值:
 *   LPBYTE			- 返回变换后的图象
 *
 * \说明:
 *   该函数根据仿射变换系数，计算待配准图象仿射变换后的图象。并返回此图象指针
 *此图象的大小为rectNewImg
 *
 *************************************************************************
 */
LPBYTE CDlgReg::SetSampImgAftAff(double* pDbAffPara, CRect rectNewImg)
{
	// pUnchSect是4*4大小的矩阵数组
	unsigned char *pUnchSect;
	pUnchSect = new unsigned char[4*4];

	// 新的图象宽度和高度
	int nNewImgWidth, nNewImgHeight;
	nNewImgWidth  = rectNewImg.right - rectNewImg.left;
	nNewImgHeight = rectNewImg.bottom- rectNewImg.top;

	// 待配准图象的宽度和高度
	int nSamplImgWidth, nSamplImgHeight;
	nSamplImgWidth = m_pDibSamp->m_lpBMIH->biWidth;
	nSamplImgHeight= m_pDibSamp->m_lpBMIH->biHeight;

	// 待配准图象的存储宽度
	int nSampSaveWidth;
	nSampSaveWidth = m_pDibSamp->GetDibSaveDim().cx;
	
	// pUnchAftAffSamp是一个大小为rectNewImg大小的图象，
	// 其中rectNewImg表示变换后的图象大小
	unsigned char *pUnchAftAffSamp;
	pUnchAftAffSamp = new unsigned char[nNewImgWidth * nNewImgHeight];
	
	double tx,ty;

	// 计算在变换后的图象的数据
	for(int i=0;i<rectNewImg.bottom-rectNewImg.top;i++)
		for(int j=0;j<rectNewImg.right-rectNewImg.left;j++)
		{
			tx = pDbAffPara[0*3 + 0]*(j+rectNewImg.left) +
				pDbAffPara[0*3 + 1]*(i+rectNewImg.top) + pDbAffPara[0*3 + 2];
			ty = pDbAffPara[1*3 + 0]*(j+rectNewImg.left) + 
				pDbAffPara[1*3 + 1]*(i+rectNewImg.top) + pDbAffPara[1*3 + 2];

			for(int m=(int)ty-1;m<=(int)ty+2;m++)
				for(int n=(int)tx-1;n<=(int)tx+2;n++)
				{
					if(m<0||m>=nSamplImgHeight||n<0||n>=nSamplImgWidth)
						pUnchSect[(m-(int)ty+1)*4 + (n-(int)tx+1)] = 0;
					else
						pUnchSect[(m-(int)ty+1)*4 + (n-(int)tx+1)] = 
							m_pDibSamp->m_lpImage[(nSamplImgHeight-m-1)*nSampSaveWidth + n];
				}

			// 确定变换的坐标
			ty = ty - (int)ty + 1;
			tx = tx - (int)tx + 1;

			// 确定变换后此坐标的数值
			pUnchAftAffSamp[i*nNewImgWidth + j] = CalSpline(pUnchSect,tx,ty);
		}
	
	
	// 是否内存
	delete[]pUnchSect;

	// 返回指针
	return (LPBYTE)pUnchAftAffSamp;
	
	
}

/*************************************************************************
 *
 * \函数名称：
 *   CalSpline()
 *
 * \输入参数:
 *   unsigned char	*pUnchCorr	- 插值的点
 *   double		dX		- X坐标
 *   double		dY		- Y坐标
 *
 * \返回值:
 *   unsigned char			- 插值后的值
 *
 * \说明:
 *   该函数根据邻近位置的数值进行插值。
 *此图象的大小为rectNewImg
 *
 *************************************************************************
 */
unsigned char CDlgReg::CalSpline(unsigned char *pUnchCorr, double x, double y)
{
	double ret=0, Cx, Cy;
	double Temp;

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			Temp = pUnchCorr[i*4 + j];
			if(fabs(y-i)<1)
				Cy = 1-2*fabs(y-i)*fabs(y-i)+fabs(y-i)*fabs(y-i)*fabs(y-i);
			if(fabs(y-i)>=1)
				Cy = 4-8*fabs(y-i)+5*fabs(y-i)*fabs(y-i)-fabs(y-i)*fabs(y-i)*fabs(y-i);
			if(fabs(x-j)<1)
				Cx = 1-2*fabs(x-j)*fabs(x-j)+fabs(x-j)*fabs(x-j)*fabs(x-j);
			if(fabs(x-j)>=1)
				Cx = 4-8*fabs(x-j)+5*fabs(x-j)*fabs(x-j)-fabs(x-j)*fabs(x-j)*fabs(x-j);
			ret += Temp*Cy*Cx;
		}
	if(ret<0)
		ret=0;
	if(ret>255)
		ret=255;

	return (unsigned char)ret;

}

/*************************************************************************
 *
 * \函数名称：
 *   SetBaseImgAftAff()
 *
 * \输入参数:
 *   double  *pDbAffPara	- 仿射变换系数矩阵
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数根据仿射变换系数，计算基准图象仿射变换后的图象，并返回存放此
 *数据的指针
 *
 *************************************************************************
 */
LPBYTE CDlgReg::SetBaseImgAftAff(CRect rectNewImg)
{
	// 新图象的大小
	int nNewImgWidth, nNewImgHeight;
	nNewImgWidth  = rectNewImg.right  - rectNewImg.left;
	nNewImgHeight = rectNewImg.bottom - rectNewImg.top;	

	// 变换后图象
	unsigned char *pUnchAftAffBase;
	pUnchAftAffBase = new unsigned char[nNewImgWidth*nNewImgHeight];

	// 基准图象的高度和宽度
	int nBaseWidth, nBaseHeight;
	nBaseWidth  = m_pDibInit->m_lpBMIH->biWidth;
	nBaseHeight = m_pDibInit->m_lpBMIH->biHeight;

	// 基准图象的存储宽度
	int nBaseSaveWidth;
	nBaseSaveWidth = m_pDibInit->GetDibSaveDim().cx;
	
	for(int i=0;i<rectNewImg.bottom-rectNewImg.top;i++)
		for(int j=0;j<rectNewImg.right-rectNewImg.left;j++)
		{
			if(i<-rectNewImg.top||i>=-rectNewImg.top+nBaseHeight||j<-rectNewImg.left||j>=-rectNewImg.left+nBaseWidth)
				pUnchAftAffBase[i*nNewImgWidth + j] = 0;
			else
				pUnchAftAffBase[i*nNewImgWidth + j] = m_pDibInit->m_lpImage[(nBaseHeight - (i+rectNewImg.top) - 1)*nBaseSaveWidth + (j+rectNewImg.left)];
		}

	// 返回
	return (LPBYTE)pUnchAftAffBase;

}
