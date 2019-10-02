// DlgMedian.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "DlgMedian.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMedian dialog


CDlgMedian::CDlgMedian(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMedian::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMedian)

	// 初始化数据
	m_nFilterCenX = 0;
	m_nFilterCenY = 0;
	m_nFilterHeight = 0;
	m_nFilterWidth = 0;
	m_nFilterType = -1;
	//}}AFX_DATA_INIT
}


void CDlgMedian::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMedian)
	DDX_Text(pDX, IDC_EDIT_MEDIAN_FILCENX, m_nFilterCenX);
	DDX_Text(pDX, IDC_EDIT_MEDIAN_FILCENY, m_nFilterCenY);
	DDX_Text(pDX, IDC_EDIT_MEDIAN_FILH, m_nFilterHeight);
	DDV_MinMaxInt(pDX, m_nFilterHeight, 1, 8);
	DDX_Text(pDX, IDC_EDIT_MEDIAN_FILW, m_nFilterWidth);
	DDV_MinMaxInt(pDX, m_nFilterWidth, 1, 8);
	DDX_Radio(pDX, IDC_RADIO_MEDIAN_VERTICAL, m_nFilterType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMedian, CDialog)
	//{{AFX_MSG_MAP(CDlgMedian)
	ON_BN_CLICKED(IDC_RADIO_MEDIAN_2DIMEN, OnRadioMedian2dimen)
	ON_BN_CLICKED(IDC_RADIO_MEDIAN_HORIZON, OnRadioMedianHorizon)
	ON_BN_CLICKED(IDC_RADIO_MEDIAN_VERTICAL, OnRadioMedianVertical)
	ON_BN_CLICKED(IDC_RADIO_MEDIAN_SELFDEF, OnRadioMedianSelfdef)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMedian message handlers

void CDlgMedian::OnRadioMedian2dimen() 
{
	// 3×3模板
	m_nFilterType   = 2;
	m_nFilterHeight = 3;
	m_nFilterWidth  = 3;
	m_nFilterCenX   = 1;
	m_nFilterCenY   = 1;
	
	// 设置文本框不可用
	(CEdit *) GetDlgItem(IDC_EDIT_MEDIAN_FILH)->EnableWindow(FALSE);
	(CEdit *) GetDlgItem(IDC_EDIT_MEDIAN_FILW)->EnableWindow(FALSE);
	(CEdit *) GetDlgItem(IDC_EDIT_MEDIAN_FILCENX)->EnableWindow(FALSE);
	(CEdit *) GetDlgItem(IDC_EDIT_MEDIAN_FILCENY)->EnableWindow(FALSE);
	
	// 更新
	UpdateData(FALSE);
	
}

void CDlgMedian::OnRadioMedianHorizon() 
{
	// 1×3模板
	m_nFilterType   = 1;
	m_nFilterHeight = 1;
	m_nFilterWidth  = 3;
	m_nFilterCenX   = 1;
	m_nFilterCenY   = 0;
	
	// 设置文本框不可用
	(CEdit *) GetDlgItem(IDC_EDIT_MEDIAN_FILH)->EnableWindow(FALSE);
	(CEdit *) GetDlgItem(IDC_EDIT_MEDIAN_FILW)->EnableWindow(FALSE);
	(CEdit *) GetDlgItem(IDC_EDIT_MEDIAN_FILCENX)->EnableWindow(FALSE);
	(CEdit *) GetDlgItem(IDC_EDIT_MEDIAN_FILCENY)->EnableWindow(FALSE);
	
	// 更新
	UpdateData(FALSE);
}

void CDlgMedian::OnRadioMedianVertical() 
{
	// 3×1模板
	m_nFilterType   = 0;
	m_nFilterHeight = 3;
	m_nFilterWidth  = 1;
	m_nFilterCenX   = 0;
	m_nFilterCenY   = 1;
	
	// 设置文本框不可用
	(CEdit *) GetDlgItem(IDC_EDIT_MEDIAN_FILH)->EnableWindow(FALSE);
	(CEdit *) GetDlgItem(IDC_EDIT_MEDIAN_FILW)->EnableWindow(FALSE);
	(CEdit *) GetDlgItem(IDC_EDIT_MEDIAN_FILCENX)->EnableWindow(FALSE);
	(CEdit *) GetDlgItem(IDC_EDIT_MEDIAN_FILCENY)->EnableWindow(FALSE);
	
	// 更新
	UpdateData(FALSE);
	
}

void CDlgMedian::OnRadioMedianSelfdef() 
{
	
	// 自定义模板
	(CEdit *) GetDlgItem(IDC_EDIT_MEDIAN_FILH)->EnableWindow(TRUE);
	(CEdit *) GetDlgItem(IDC_EDIT_MEDIAN_FILW)->EnableWindow(TRUE);
	(CEdit *) GetDlgItem(IDC_EDIT_MEDIAN_FILCENX)->EnableWindow(TRUE);
	(CEdit *) GetDlgItem(IDC_EDIT_MEDIAN_FILCENY)->EnableWindow(TRUE);
}

void CDlgMedian::OnOK() 
{
	// 获取用户设置（更新）
	UpdateData(TRUE);
	
	// 判断设置是否有效
	if ((m_nFilterCenX < 0) || (m_nFilterCenX > m_nFilterWidth - 1) ||
		(m_nFilterCenY < 0) || (m_nFilterCenY > m_nFilterHeight - 1))
	{
		// 提示用户参数设置错误
		MessageBox("参数设置错误！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	CDialog::OnOK();
}
