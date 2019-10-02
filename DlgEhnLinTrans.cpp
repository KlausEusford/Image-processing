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
	// ����Ĭ��OnInitDialog����
	CDialog::OnInitDialog();
	
	// ��ȡ����ֱ��ͼ�ı�ǩ
	CWnd* pWnd = GetDlgItem(IDC_LINTRANS_SHOWCOR);
	
	// �����������¼�����Ч����
	pWnd->GetClientRect(m_rectMouse);
	pWnd->ClientToScreen(&m_rectMouse);
	
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);
	
	m_rectMouse.top -= rect.top;
	m_rectMouse.left -= rect.left;
	
	// ���ý�������¼�����Ч����
	m_rectMouse.top += 25;
	m_rectMouse.left += 10;
	m_rectMouse.bottom = m_rectMouse.top + 255;
	m_rectMouse.right = m_rectMouse.left + 256;
	
	// ��ʼ���϶�״̬
	m_nIsDraging = 0;
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEhnLinTrans::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// ���û�������������ʼ�϶�
	if(m_rectMouse.PtInRect(point))
	{
		CRect	rectTemp;
		
		// �����1�ٽ�����
		rectTemp.left = m_rectMouse.left + m_nX1 - 2;
		rectTemp.right = m_rectMouse.left + m_nX1 + 2;
		rectTemp.top = 255 + m_rectMouse.top - m_nY1 - 2;
		rectTemp.bottom = 255 + m_rectMouse.top - m_nY1 + 2;
		
		// �ж��û��ǲ������϶���1
		if (rectTemp.PtInRect(point))
		{
			
			// �����϶�״̬1���϶���1
			m_nIsDraging = 1;
			
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		}
		else
		{
			// �����2�ٽ�����
			rectTemp.left = m_rectMouse.left + m_nX2 - 2;
			rectTemp.right = m_rectMouse.left + m_nX2 + 2;
			rectTemp.top = 255 + m_rectMouse.top - m_nY2 - 2;
			rectTemp.bottom = 255 + m_rectMouse.top - m_nY2 + 2;
			
			// �ж��û��ǲ������϶���2
			if (rectTemp.PtInRect(point))
			{
				
				// �����϶�״̬Ϊ2���϶���2
				m_nIsDraging = 2;
				
				// ���Ĺ��
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}
	
	// Ĭ�ϵ��������������¼�
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgEhnLinTrans::OnLButtonUp(UINT nFlags, CPoint point) 
{
		// ���û��ͷ�������ֹͣ�϶�
	if (m_nIsDraging != 0)
	{
		// �����϶�״̬
		m_nIsDraging = 0;
	}
	
	// Ĭ���ͷ������������¼�
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgEhnLinTrans::OnMouseMove(UINT nFlags, CPoint point) 
{
	// �жϵ�ǰ����Ƿ��ڻ�������
	if(m_rectMouse.PtInRect(point))
	{
		// �ж��Ƿ������϶�
		if (m_nIsDraging != 0)
		{
			// �ж������϶���1���ǵ�2
			if (m_nIsDraging == 1)
			{
				// �ж��Ƿ�����<����
				if (point.x - m_rectMouse.left < m_nX2)
				{
					// ��������
					m_nX1 =  point.x - m_rectMouse.left;
				}
				else
				{
					// �����Ϲ����ޣ�����Ϊ����-1
					m_nX1 = m_nX2 - 1;
					
					// �������λ��
					point.x = m_rectMouse.left + m_nX2 - 1;
				}
				
				// ����Y����
				m_nY1 = 255 + m_rectMouse.top - point.y;
			}
			else
			{
				// �����϶���2
				
				// �ж��Ƿ�����>����
				if (point.x - m_rectMouse.left > m_nX1)
				{
					// ��������
					m_nX2 = point.x - m_rectMouse.left;
				}
				else
				{
					// �����Ϲ����ޣ�����Ϊ���ޣ�1
					m_nX2 = m_nX1 + 1;
					
					// �������λ��
					point.x = m_rectMouse.left + m_nX1 + 1;
				}
				
				// ����Y����
				m_nY2 = 255 + m_rectMouse.top - point.y;
			}
			
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			
			// ����
			UpdateData(FALSE);
			
			// �ػ�
			InvalidateRect(m_rectMouse, TRUE);
		}
		else
		{
			CRect	rectTemp1;
			CRect	rectTemp2;
			
			// �����1�ٽ�����
			rectTemp1.left = m_rectMouse.left + m_nX1 - 2;
			rectTemp1.right = m_rectMouse.left + m_nX1 + 2;
			rectTemp1.top = 255 + m_rectMouse.top - m_nY1 - 2;
			rectTemp1.bottom = 255 + m_rectMouse.top - m_nY1 + 2;
			
			// �����2�ٽ�����
			rectTemp2.left = m_rectMouse.left + m_nX2 - 2;
			rectTemp2.right = m_rectMouse.left + m_nX2 + 2;
			rectTemp2.top = 255 + m_rectMouse.top - m_nY2 - 2;
			rectTemp2.bottom = 255 + m_rectMouse.top - m_nY2 + 2;
			
			// �ж��û��ڵ�1���2�Ա�
			if ((rectTemp1.PtInRect(point)) || (rectTemp2.PtInRect(point)))
			{
				// ���Ĺ��
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}
	
	// Ĭ������ƶ������¼�
	CDialog::OnMouseMove(nFlags, point);
}



void CDlgEhnLinTrans::OnPaint() 
{
	// �豸������
	CPaintDC dc(this); // device context for painting
	
	// �ַ���
	CString str;
	
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_LINTRANS_SHOWCOR);
	
	// ָ��
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	pDC->Rectangle(0,0,330,300);
	
	// �������ʶ���
	CPen* pPenRed = new CPen;
	
	// ��ɫ����
	pPenRed->CreatePen(PS_SOLID, 2, RGB(255,0,0));
	
	// �������ʶ���
	CPen* pPenBlue = new CPen;
	
	// ��ɫ����
	pPenBlue->CreatePen(PS_SOLID, 1, RGB(0,0, 255));
	
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// ����������
	pDC->MoveTo(10,10);
	
	// ��ֱ��
	pDC->LineTo(10,280);
	
	// ˮƽ��
	pDC->LineTo(320,280);
	
	// д����
	str.Format("0");
	pDC->TextOut(10, 281, str);
	
	str.Format("255");
	pDC->TextOut(265, 281, str);
	pDC->TextOut(11, 25, str);
	
	// ����X���ͷ
	pDC->LineTo(315,275);
	pDC->MoveTo(320,280);
	pDC->LineTo(315,285);
	
	// ����X���ͷ
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);
	
	// ���ĳ���ɫ����
	pDC->SelectObject(pPenBlue);	
	
	// ��������ֵ
	str.Format("(%d, %d)", m_nX1, m_nY1);
	pDC->TextOut(m_nX1 + 10, 281 - m_nY1, str);
	str.Format("(%d, %d)", m_nX2, m_nY2);
	pDC->TextOut(m_nX2 + 10, 281 - m_nY2, str);
	
	// �����û�ָ���ı任ֱ��
	pDC->MoveTo(10, 280);
	pDC->LineTo(m_nX1 + 10, 280 - m_nY1);
	pDC->LineTo(m_nX2 + 10, 280 - m_nY2);
	pDC->LineTo(265, 25);
	
	// ���Ƶ��Ե��С����
	CBrush  brush;
	brush.CreateSolidBrush(RGB(0,255,0));
	
	// ѡ��ˢ��
	CGdiObject* pOldBrush = pDC->SelectObject(&brush);
	
	// ����С����
	pDC->Rectangle(m_nX1 + 10 - 2, 280 - m_nY1 - 2, m_nX1 + 12, 280 - m_nY1 + 2);
	pDC->Rectangle(m_nX2 + 10 - 2, 280 - m_nY2 - 2, m_nX2 + 12, 280 - m_nY2 + 2);
	
	// �ָ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);	
	
	// ���Ʊ�Ե
	pDC->MoveTo(10,25);
	pDC->LineTo(265,25);
	pDC->LineTo(265,280);
	
	// ɾ���µĻ���
	delete pPenRed;
	delete pPenBlue;
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgEhnLinTrans::OnKillfocusEditLintransX1() 
{
	// ����
	UpdateData(TRUE);
	
	// �ж��Ƿ����޳�������
	if (m_nX1 > m_nX2)
	{
		// ����
		int nTemp = m_nX1;
		m_nX1 = m_nX2;
		m_nX2 = nTemp;
		nTemp = m_nY1;
		m_nY1 = m_nY2;
		m_nY2 = nTemp;
		
		// ����
		UpdateData(FALSE);
	}
	
	// �ػ�
	InvalidateRect(m_rectMouse, TRUE);
}

void CDlgEhnLinTrans::OnKillfocusEditLintransX2() 
{
	// ����
	UpdateData(TRUE);
	
	// �ж��Ƿ����޳�������
	if (m_nX1 > m_nX2)
	{
		// ����
		int nTemp = m_nX1;
		m_nX1 = m_nX2;
		m_nX2 = nTemp;
		nTemp = m_nY1;
		m_nY1 = m_nY2;
		m_nY2 = nTemp;
		
		// ����
		UpdateData(FALSE);
	}
	
	// �ػ�
	InvalidateRect(m_rectMouse, TRUE);
}

void CDlgEhnLinTrans::OnKillfocusEditLintransY1() 
{
	// ����
	UpdateData(TRUE);
	
	// �ػ�
	InvalidateRect(m_rectMouse, TRUE);
}

void CDlgEhnLinTrans::OnKillfocusEditLintransY2() 
{
	// ����
	UpdateData(TRUE);
	
	// �ػ�
	InvalidateRect(m_rectMouse, TRUE);
}


void CDlgEhnLinTrans::OnOK() 
{
	// �ж��Ƿ����޳�������
	if (m_nX1 > m_nX2)
	{
		// ����
		int nTemp = m_nX1;
		m_nX1 = m_nX2;
		m_nX2 = nTemp;
		nTemp = m_nY1;
		m_nY1 = m_nY2;
		m_nY2 = nTemp;
		
		// ����
		UpdateData(FALSE);
	}
	
	// Ĭ�ϴ����¼�
	CDialog::OnOK();	

}