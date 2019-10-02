// DlgEhnLinTrans.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "DlgEhnLinTrans.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEhnLinTrans dialog


CDlgEhnLinTrans::CDlgEhnLinTrans(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEhnLinTrans::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEhnLinTrans)
	m_nX1 = 0;
	m_nX2 = 0;
	m_nY1 = 0;
	m_nY2 = 0;
	//}}AFX_DATA_INIT
}


void CDlgEhnLinTrans::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEhnLinTrans)
	DDX_Text(pDX, IDC_EDIT_LINTRANS_X1, m_nX1);
	DDV_MinMaxInt(pDX, m_nX1, 0, 255);
	DDX_Text(pDX, IDC_EDIT_LINTRANS_X2, m_nX2);
	DDV_MinMaxInt(pDX, m_nX2, 0, 255);
	DDX_Text(pDX, IDC_EDIT_LINTRANS_Y1, m_nY1);
	DDV_MinMaxInt(pDX, m_nY1, 0, 255);
	DDX_Text(pDX, IDC_EDIT_LINTRANS_Y2, m_nY2);
	DDV_MinMaxInt(pDX, m_nY2, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEhnLinTrans, CDialog)
	//{{AFX_MSG_MAP(CDlgEhnLinTrans)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_EN_KILLFOCUS(IDC_EDIT_LINTRANS_X1, OnKillfocusEditLintransX1)
	ON_EN_KILLFOCUS(IDC_EDIT_LINTRANS_X2, OnKillfocusEditLintransX2)
	ON_EN_KILLFOCUS(IDC_EDIT_LINTRANS_Y1, OnKillfocusEditLintransY1)
	ON_EN_KILLFOCUS(IDC_EDIT_LINTRANS_Y2, OnKillfocusEditLintransY2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEhnLinTrans message handlers

BOOL CDlgEhnLinTrans::OnInitDialog() 
{
	// 调用默认OnInitDialog函数
	CDialog::OnInitDialog();
	
	// 获取绘制直方图的标签
	CWnd* pWnd = GetDlgItem(IDC_LINTRANS_SHOWCOR);
	
	// 计算接受鼠标事件的有效区域
	pWnd->GetClientRect(m_rectMouse);
	pWnd->ClientToScreen(&m_rectMouse);
	
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);
	
	m_rectMouse.top -= rect.top;
	m_rectMouse.left -= rect.left;
	
	// 设置接受鼠标事件的有效区域
	m_rectMouse.top += 25;
	m_rectMouse.left += 10;
	m_rectMouse.bottom = m_rectMouse.top + 255;
	m_rectMouse.right = m_rectMouse.left + 256;
	
	// 初始化拖动状态
	m_nIsDraging = 0;
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEhnLinTrans::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// 当用户单击鼠标左键开始拖动
	if(m_rectMouse.PtInRect(point))
	{
		CRect	rectTemp;
		
		// 计算点1临近区域
		rectTemp.left = m_rectMouse.left + m_nX1 - 2;
		rectTemp.right = m_rectMouse.left + m_nX1 + 2;
		rectTemp.top = 255 + m_rectMouse.top - m_nY1 - 2;
		rectTemp.bottom = 255 + m_rectMouse.top - m_nY1 + 2;
		
		// 判断用户是不是想拖动点1
		if (rectTemp.PtInRect(point))
		{
			
			// 设置拖动状态1，拖动点1
			m_nIsDraging = 1;
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		}
		else
		{
			// 计算点2临近区域
			rectTemp.left = m_rectMouse.left + m_nX2 - 2;
			rectTemp.right = m_rectMouse.left + m_nX2 + 2;
			rectTemp.top = 255 + m_rectMouse.top - m_nY2 - 2;
			rectTemp.bottom = 255 + m_rectMouse.top - m_nY2 + 2;
			
			// 判断用户是不是想拖动点2
			if (rectTemp.PtInRect(point))
			{
				
				// 设置拖动状态为2，拖动点2
				m_nIsDraging = 2;
				
				// 更改光标
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}
	
	// 默认单击鼠标左键处理事件
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgEhnLinTrans::OnLButtonUp(UINT nFlags, CPoint point) 
{
		// 当用户释放鼠标左键停止拖动
	if (m_nIsDraging != 0)
	{
		// 重置拖动状态
		m_nIsDraging = 0;
	}
	
	// 默认释放鼠标左键处理事件
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgEhnLinTrans::OnMouseMove(UINT nFlags, CPoint point) 
{
	// 判断当前光标是否在绘制区域
	if(m_rectMouse.PtInRect(point))
	{
		// 判断是否正在拖动
		if (m_nIsDraging != 0)
		{
			// 判断正在拖动点1还是点2
			if (m_nIsDraging == 1)
			{
				// 判断是否下限<上限
				if (point.x - m_rectMouse.left < m_nX2)
				{
					// 更改下限
					m_nX1 =  point.x - m_rectMouse.left;
				}
				else
				{
					// 下限拖过上限，设置为上限-1
					m_nX1 = m_nX2 - 1;
					
					// 重设鼠标位置
					point.x = m_rectMouse.left + m_nX2 - 1;
				}
				
				// 更改Y坐标
				m_nY1 = 255 + m_rectMouse.top - point.y;
			}
			else
			{
				// 正在拖动点2
				
				// 判断是否上限>下限
				if (point.x - m_rectMouse.left > m_nX1)
				{
					// 更改下限
					m_nX2 = point.x - m_rectMouse.left;
				}
				else
				{
					// 下限拖过上限，设置为下限＋1
					m_nX2 = m_nX1 + 1;
					
					// 重设鼠标位置
					point.x = m_rectMouse.left + m_nX1 + 1;
				}
				
				// 更改Y坐标
				m_nY2 = 255 + m_rectMouse.top - point.y;
			}
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			
			// 更新
			UpdateData(FALSE);
			
			// 重绘
			InvalidateRect(m_rectMouse, TRUE);
		}
		else
		{
			CRect	rectTemp1;
			CRect	rectTemp2;
			
			// 计算点1临近区域
			rectTemp1.left = m_rectMouse.left + m_nX1 - 2;
			rectTemp1.right = m_rectMouse.left + m_nX1 + 2;
			rectTemp1.top = 255 + m_rectMouse.top - m_nY1 - 2;
			rectTemp1.bottom = 255 + m_rectMouse.top - m_nY1 + 2;
			
			// 计算点2临近区域
			rectTemp2.left = m_rectMouse.left + m_nX2 - 2;
			rectTemp2.right = m_rectMouse.left + m_nX2 + 2;
			rectTemp2.top = 255 + m_rectMouse.top - m_nY2 - 2;
			rectTemp2.bottom = 255 + m_rectMouse.top - m_nY2 + 2;
			
			// 判断用户在点1或点2旁边
			if ((rectTemp1.PtInRect(point)) || (rectTemp2.PtInRect(point)))
			{
				// 更改光标
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}
	
	// 默认鼠标移动处理事件
	CDialog::OnMouseMove(nFlags, point);
}



void CDlgEhnLinTrans::OnPaint() 
{
	// 设备上下文
	CPaintDC dc(this); // device context for painting
	
	// 字符串
	CString str;
	
	// 获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_LINTRANS_SHOWCOR);
	
	// 指针
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	pDC->Rectangle(0,0,330,300);
	
	// 创建画笔对象
	CPen* pPenRed = new CPen;
	
	// 红色画笔
	pPenRed->CreatePen(PS_SOLID, 2, RGB(255,0,0));
	
	// 创建画笔对象
	CPen* pPenBlue = new CPen;
	
	// 蓝色画笔
	pPenBlue->CreatePen(PS_SOLID, 1, RGB(0,0, 255));
	
	// 选中当前红色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// 绘制坐标轴
	pDC->MoveTo(10,10);
	
	// 垂直轴
	pDC->LineTo(10,280);
	
	// 水平轴
	pDC->LineTo(320,280);
	
	// 写坐标
	str.Format("0");
	pDC->TextOut(10, 281, str);
	
	str.Format("255");
	pDC->TextOut(265, 281, str);
	pDC->TextOut(11, 25, str);
	
	// 绘制X轴箭头
	pDC->LineTo(315,275);
	pDC->MoveTo(320,280);
	pDC->LineTo(315,285);
	
	// 绘制X轴箭头
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);
	
	// 更改成蓝色画笔
	pDC->SelectObject(pPenBlue);	
	
	// 绘制坐标值
	str.Format("(%d, %d)", m_nX1, m_nY1);
	pDC->TextOut(m_nX1 + 10, 281 - m_nY1, str);
	str.Format("(%d, %d)", m_nX2, m_nY2);
	pDC->TextOut(m_nX2 + 10, 281 - m_nY2, str);
	
	// 绘制用户指定的变换直线
	pDC->MoveTo(10, 280);
	pDC->LineTo(m_nX1 + 10, 280 - m_nY1);
	pDC->LineTo(m_nX2 + 10, 280 - m_nY2);
	pDC->LineTo(265, 25);
	
	// 绘制点边缘的小矩形
	CBrush  brush;
	brush.CreateSolidBrush(RGB(0,255,0));
	
	// 选中刷子
	CGdiObject* pOldBrush = pDC->SelectObject(&brush);
	
	// 绘制小矩形
	pDC->Rectangle(m_nX1 + 10 - 2, 280 - m_nY1 - 2, m_nX1 + 12, 280 - m_nY1 + 2);
	pDC->Rectangle(m_nX2 + 10 - 2, 280 - m_nY2 - 2, m_nX2 + 12, 280 - m_nY2 + 2);
	
	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);	
	
	// 绘制边缘
	pDC->MoveTo(10,25);
	pDC->LineTo(265,25);
	pDC->LineTo(265,280);
	
	// 删除新的画笔
	delete pPenRed;
	delete pPenBlue;
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgEhnLinTrans::OnKillfocusEditLintransX1() 
{
	// 更新
	UpdateData(TRUE);
	
	// 判断是否下限超过上限
	if (m_nX1 > m_nX2)
	{
		// 互换
		int nTemp = m_nX1;
		m_nX1 = m_nX2;
		m_nX2 = nTemp;
		nTemp = m_nY1;
		m_nY1 = m_nY2;
		m_nY2 = nTemp;
		
		// 更新
		UpdateData(FALSE);
	}
	
	// 重绘
	InvalidateRect(m_rectMouse, TRUE);
}

void CDlgEhnLinTrans::OnKillfocusEditLintransX2() 
{
	// 更新
	UpdateData(TRUE);
	
	// 判断是否下限超过上限
	if (m_nX1 > m_nX2)
	{
		// 互换
		int nTemp = m_nX1;
		m_nX1 = m_nX2;
		m_nX2 = nTemp;
		nTemp = m_nY1;
		m_nY1 = m_nY2;
		m_nY2 = nTemp;
		
		// 更新
		UpdateData(FALSE);
	}
	
	// 重绘
	InvalidateRect(m_rectMouse, TRUE);
}

void CDlgEhnLinTrans::OnKillfocusEditLintransY1() 
{
	// 更新
	UpdateData(TRUE);
	
	// 重绘
	InvalidateRect(m_rectMouse, TRUE);
}

void CDlgEhnLinTrans::OnKillfocusEditLintransY2() 
{
	// 更新
	UpdateData(TRUE);
	
	// 重绘
	InvalidateRect(m_rectMouse, TRUE);
}


void CDlgEhnLinTrans::OnOK() 
{
	// 判断是否下限超过上限
	if (m_nX1 > m_nX2)
	{
		// 互换
		int nTemp = m_nX1;
		m_nX1 = m_nX2;
		m_nX2 = nTemp;
		nTemp = m_nY1;
		m_nY1 = m_nY2;
		m_nY2 = nTemp;
		
		// 更新
		UpdateData(FALSE);
	}
	
	// 默认处理事件
	CDialog::OnOK();	

}
