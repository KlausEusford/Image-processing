// DlgSmooth.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "DlgSmooth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSmooth dialog


CDlgSmooth::CDlgSmooth(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSmooth::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSmooth)
	m_nTemType = 0;
	m_nSmthTemCenX = 0;
	m_nSmthTemCenY = 0;
	m_dbSmthTemCoef = 0.0;
	m_nSmthTemHeight = 0;
	m_nSmthTemWidth = 0;
	//}}AFX_DATA_INIT
}


void CDlgSmooth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSmooth)	
	DDX_Text(pDX, IDC_EDIT_SELFTEM_CEN_X, m_nSmthTemCenX);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_CEN_Y, m_nSmthTemCenY);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_COEF, m_dbSmthTemCoef);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_HEIGHT, m_nSmthTemHeight);
	DDV_MinMaxInt(pDX, m_nSmthTemHeight, 2, 5);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_WIDTH, m_nSmthTemWidth);
	DDV_MinMaxInt(pDX, m_nSmthTemWidth, 2, 5);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL0  , m_pdbTemp[0 ]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL1  , m_pdbTemp[1 ]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL2  , m_pdbTemp[2 ]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL3  , m_pdbTemp[3 ]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL4  , m_pdbTemp[4 ]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL5  , m_pdbTemp[5 ]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL6  , m_pdbTemp[6 ]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL7  , m_pdbTemp[7 ]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL8  , m_pdbTemp[8 ]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL9  , m_pdbTemp[9 ]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL10 , m_pdbTemp[10]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL11 , m_pdbTemp[11]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL12 , m_pdbTemp[12]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL13 , m_pdbTemp[13]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL14 , m_pdbTemp[14]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL15 , m_pdbTemp[15]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL16 , m_pdbTemp[16]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL17 , m_pdbTemp[17]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL18 , m_pdbTemp[18]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL19 , m_pdbTemp[19]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL20 , m_pdbTemp[20]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL22 , m_pdbTemp[22]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL23 , m_pdbTemp[23]);
	DDX_Text(pDX, IDC_EDIT_SELFTEM_EL24 , m_pdbTemp[24]);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSmooth, CDialog)
	//{{AFX_MSG_MAP(CDlgSmooth)
	ON_BN_CLICKED(IDC_RADIO_AVERTEM, OnRadioAvertem)
	ON_BN_CLICKED(IDC_RADIO_GUASSTEM, OnRadioGuasstem)
	ON_BN_CLICKED(IDC_RADIO_SELFTEM, OnRadioSelftem)
	ON_EN_KILLFOCUS(IDC_EDIT_SELFTEM_HEIGHT, OnKillfocusEditSelftemHeight)
	ON_EN_KILLFOCUS(IDC_EDIT_SELFTEM_WIDTH, OnKillfocusEditSelftemWidth)
	ON_EN_CHANGE(IDC_EDIT_SELFTEM_WIDTH, OnChangeEditSelftemWidth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSmooth message handlers


void CDlgSmooth::OnRadioAvertem() 
{
	// 设置3×3平均模板参数
	m_nTemType = 0;
	m_nSmthTemHeight = 3;
	m_nSmthTemWidth  = 3;
	m_nSmthTemCenX   = 1;
	m_nSmthTemCenY   = 1;
	m_dbSmthTemCoef  = (double)1.0/9.0;

	// 设置3×3平均模板元素
	m_pdbTemp[0] = 1.0;
	m_pdbTemp[1] = 1.0;
	m_pdbTemp[2] = 1.0;
	m_pdbTemp[3] = 0.0;
	m_pdbTemp[4] = 0.0;
	m_pdbTemp[5] = 1.0;
	m_pdbTemp[6] = 1.0;
	m_pdbTemp[7] = 1.0;
	m_pdbTemp[8] = 0.0;
	m_pdbTemp[9] = 0.0;
	m_pdbTemp[10] = 1.0;
	m_pdbTemp[11] = 1.0;
	m_pdbTemp[12] = 1.0;
	m_pdbTemp[13] = 0.0;
	m_pdbTemp[14] = 0.0;
	m_pdbTemp[15] = 0.0;
	m_pdbTemp[16] = 0.0;
	m_pdbTemp[17] = 0.0;
	m_pdbTemp[18] = 0.0;
	m_pdbTemp[19] = 0.0;
	m_pdbTemp[20] = 0.0;
	m_pdbTemp[21] = 0.0;
	m_pdbTemp[22] = 0.0;
	m_pdbTemp[23] = 0.0;
	m_pdbTemp[24] = 0.0;
	
	// 更新文本框状态
	//UpdateEdit();
	
	// 更新
	UpdateData(FALSE);

	
}

void CDlgSmooth::OnRadioGuasstem() 
{
	// 3×3高斯模板
	m_nTemType = 1;
	m_nSmthTemHeight = 3;
	m_nSmthTemWidth  = 3;
	m_nSmthTemCenX   = 1;
	m_nSmthTemCenY   = 1;
	m_dbSmthTemCoef  = (double)(1.0/16.0);

	// 设置模板元素
	m_pdbTemp[0] = 1.0;
	m_pdbTemp[1] = 2.0;
	m_pdbTemp[2] = 1.0;
	m_pdbTemp[3] = 0.0;
	m_pdbTemp[4] = 0.0;
	m_pdbTemp[5] = 2.0;
	m_pdbTemp[6] = 4.0;
	m_pdbTemp[7] = 2.0;
	m_pdbTemp[8] = 0.0;
	m_pdbTemp[9] = 0.0;
	m_pdbTemp[10] = 1.0;
	m_pdbTemp[11] = 2.0;
	m_pdbTemp[12] = 1.0;
	m_pdbTemp[13] = 0.0;
	m_pdbTemp[14] = 0.0;
	m_pdbTemp[15] = 0.0;
	m_pdbTemp[16] = 0.0;
	m_pdbTemp[17] = 0.0;
	m_pdbTemp[18] = 0.0;
	m_pdbTemp[19] = 0.0;
	m_pdbTemp[20] = 0.0;
	m_pdbTemp[21] = 0.0;
	m_pdbTemp[22] = 0.0;
	m_pdbTemp[23] = 0.0;
	m_pdbTemp[24] = 0.0;
	
	// 更新文本框状态
	UpdateEdit();
	
	// 更新
	UpdateData(FALSE);
}

void CDlgSmooth::OnRadioSelftem() 
{
	// TODO: Add your control notification handler code here
	// 自定义模板
	m_nTemType = 2;
	
	// 更新文本框状态
	UpdateEdit();
}

void CDlgSmooth::OnKillfocusEditSelftemHeight() 
{
	// TODO: Add your control notification handler code here
		// 更新
	UpdateData(TRUE);
	
	// 更新文本框状态
	UpdateEdit();
	
}

void CDlgSmooth::OnKillfocusEditSelftemWidth() 
{
	// TODO: Add your control notification handler code here
	// 更新
	UpdateData(TRUE);
	
	// 更新文本框状态
	UpdateEdit();
}

void CDlgSmooth::OnOK() 
{
		// 获取用户设置（更新）
	UpdateData(TRUE);
	
	// 判断设置是否有效
	if ((m_nSmthTemCenX  < 0) || (m_nSmthTemCenX > m_nSmthTemWidth - 1) ||
		(m_nSmthTemCenY < 0) || (m_nSmthTemCenY > m_nSmthTemHeight - 1))
	{
		// 提示用户参数设置错误
		MessageBox("中心元素参数设置错误！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 更新模板元素数组（将有效元素放置在数组的前面）
	for (int i = 0; i < m_nSmthTemHeight; i++)
	{
		for (int j = 0; j < m_nSmthTemWidth; j++)
		{
			m_pdbTemp[i * m_nSmthTemWidth + j] = m_pdbTemp[i * 5 + j];
		}
	}
	
	// 更新
	UpdateData(FALSE);
	
	// 退出
	CDialog::OnOK();
}

void CDlgSmooth::OnChangeEditSelftemWidth() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}


void CDlgSmooth::UpdateEdit()
{
	BOOL	bEnable;
	
	// 循环变量
	int		i;
	int		j;
	
	// 判断是不是自定义模板
	if (m_nTemType  == 2) 
	{
		bEnable = TRUE;
	}
	else
	{
		bEnable = FALSE;
	}

	// 设置文本框可用状态
	(CEdit *) GetDlgItem(IDC_EDIT_SELFTEM_HEIGHT)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_SELFTEM_WIDTH)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_SELFTEM_COEF)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_SELFTEM_CEN_X)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT_SELFTEM_CEN_Y)->EnableWindow(bEnable);
	
	// IDC_EDIT_SELFTEM_EL0等ID其实是一个整数，它的数值定义在Resource.h中定义。

	// 设置模板元素文本框bEnable状态
	for (i = IDC_EDIT_SELFTEM_EL0; i <= IDC_EDIT_SELFTEM_EL24; i++)
	{
		// 设置文本框不可编辑
		(CEdit *) GetDlgItem(i)->EnableWindow(bEnable);
	}
	
	// 显示应该可见的模板元素文本框
	for (i = 0; i < m_nSmthTemHeight; i++)
	{
		for (j = 0; j < m_nSmthTemWidth; j++)
		{
			// 设置文本框可见
			(CEdit *) GetDlgItem(IDC_EDIT_SELFTEM_EL0 + i*5 + j)->ShowWindow(SW_SHOW);
		}
	}
	
	// 隐藏应该不可见的模板元素文本框（前m_nSmthTemHeight行的后几列）
	for (i = 0; i < m_nSmthTemHeight; i++)
	{
		for (j = m_nSmthTemWidth; j < 5; j++)
		{
			// 设置不可见
			(CEdit *) GetDlgItem(IDC_EDIT_SELFTEM_EL0 + i*5 + j)->ShowWindow(SW_HIDE);
		}
	}
	
	// 隐藏应该不可见的模板元素文本框（后几行）
	for (i = m_nSmthTemHeight; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			// 设置不可见
			(CEdit *) GetDlgItem(IDC_EDIT_SELFTEM_EL0 + i*5 + j)->ShowWindow(SW_HIDE);
		}
	}

}
