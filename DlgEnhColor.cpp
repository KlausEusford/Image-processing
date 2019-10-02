// DlgEnhColor.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "DlgEnhColor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEnhColor dialog


CDlgEnhColor::CDlgEnhColor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEnhColor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEnhColor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgEnhColor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEnhColor)
	DDX_Control(pDX, IDC_LIST_ENH_COLOR, m_lstColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEnhColor, CDialog)
	//{{AFX_MSG_MAP(CDlgEnhColor)
	ON_LBN_DBLCLK(IDC_LIST_ENH_COLOR, OnDblclkListEnhColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEnhColor message handlers

BOOL CDlgEnhColor::OnInitDialog() 
{
	// 循环变量
	int		i;
	
	// 调用默认OnInitDialog函数
	CDialog::OnInitDialog();
	
	// 添加伪彩色编码
	for (i = 0; i < m_nColorCount; i++)
	{
		m_lstColor.AddString(m_lpColorName + i * m_nNameLen);
	}

	// 选中初始编码表
	m_lstColor.SetCurSel(m_nColor);
	
	// 返回TRUE
	return TRUE;
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEnhColor::OnDblclkListEnhColor() 
{
	// 双击事件，直接调用OnOK()成员函数
	OnOK();
}


void CDlgEnhColor::OnOK() 
{	
	// 用户单击确定按钮
	m_nColor = m_lstColor.GetCurSel();
	
	// 调用默认的OnOK()函数
	CDialog::OnOK();
}
