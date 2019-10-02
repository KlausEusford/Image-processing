// DlgHistShow1.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "DlgHistShow1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHistShow dialog


CDlgHistShow::CDlgHistShow(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHistShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHistShow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgHistShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHistShow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHistShow, CDialog)
	//{{AFX_MSG_MAP(CDlgHistShow)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHistShow message handlers

BOOL CDlgHistShow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	// 设置直线原图象象素的指针
	unsigned char * lpSrc;

	// 循环变量
	int i,j;

	// 获取绘制直方图的标签
	CWnd* pWnd = GetDlgItem(IDC_DLG_HIST_SHOW);

	// 计算得到直方图
	// 图象的高度和宽度
	CSize sizeImage;
	sizeImage = m_pDib->GetDimensions();


	// 获得图象数据存储的高度和宽度
	CSize sizeSaveImage;
	sizeSaveImage = m_pDib->GetDibSaveDim();

	// 重置计数为0
	for (i = 0; i < 256; i ++)
	{
		// 清零
		m_nHist[i] = 0;
	}
	
	// 计算各个灰度值的计数，即得到直方图
	for (i = 0; i < sizeImage.cy; i ++)
	{
		for (j = 0; j < sizeImage.cx; j ++)
		{
			lpSrc = (unsigned char *)m_pDib->m_lpImage + sizeSaveImage.cx * i + j;
			
			// 计数加1
			m_nHist[*(lpSrc)]++;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgHistShow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here


	// 循环变量
	int i;
	// 获取绘制直方图文本框的标签
	CWnd* pWnd = GetDlgItem(IDC_DLG_HIST_SHOW);

	// 获取设备上下文
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	pDC->Rectangle(0, 0, 330, 300);

	// 创建画笔对象
	CPen* pPenRed = new CPen;

	// 创建红色画笔（用于绘制坐标轴）
	pPenRed->CreatePen(PS_SOLID, 1, RGB(255,0,0));

	// 选入红色画笔，并保存以前的画笔
	CPen* pOldPen = pDC->SelectObject(pPenRed);

	// 绘制坐标轴
	pDC->MoveTo(10,10);
	
	// 绘制垂直轴
	pDC->LineTo(10, 280);

	// 绘制水平轴
	pDC->LineTo(320, 280);

	// 绘制X轴刻度值
	CString strTemp;
	strTemp.Format("0");
	pDC->TextOut(10, 283, strTemp);
	strTemp.Format("50");
	pDC->TextOut(60, 283, strTemp);
	strTemp.Format("100");
	pDC->TextOut(110, 283, strTemp);
	strTemp.Format("150");
	pDC->TextOut(160, 283, strTemp);
	strTemp.Format("200");
	pDC->TextOut(210, 283, strTemp);
	strTemp.Format("255");
	pDC->TextOut(265, 283, strTemp);
	
	// 绘制X轴刻度
	for (i = 0; i < 256; i += 5)
	{
		if ((i & 1) == 0)
		{
			// 10的倍数
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 284);
		}
		else
		{
			// 5的奇数倍数
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 282);
		}
	}
	
	// 绘制X轴箭头
	pDC->MoveTo(315,275);
	pDC->LineTo(320,280);
	pDC->LineTo(315,285);
	
	// 绘制Y轴箭头
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);

	// 直方图中最大计数值
	LONG lMaxCount = 0;

	// 计算最大计数值
	for (i = 0; i <= 255; i ++)
	{
		// 判断是否大于当前最大值
		if (m_nHist[i] > lMaxCount)
		{
			// 更新最大值
			lMaxCount = m_nHist[i];
		}
	}
	
	// 输出最大计数值
	pDC->MoveTo(10, 25);
	pDC->LineTo(14, 25);
	strTemp.Format("%d", lMaxCount);
	pDC->TextOut(11, 26, strTemp);
	
	// 声名画笔对象
	CPen* pPenBlue = new CPen;

	// 创建蓝色画笔（用于绘制直方图）
	pPenBlue->CreatePen(PS_SOLID, 1, RGB(0,0,255));

	// 选入蓝色画笔
	pDC->SelectObject(pPenBlue);

	// 判断是否存在计数值
	if(lMaxCount > 0){
		// 绘制直方图
		for (i = 0; i <= 255; i ++)
		{
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 281 - (int) (m_nHist[i] * 256 / lMaxCount));
		}
	}

	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);	
	
	// 删除新的画笔
	delete pPenRed;
	delete pPenBlue;
}
