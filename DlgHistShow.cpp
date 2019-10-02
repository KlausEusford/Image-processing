// DlgHistShow.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "DlgHistShow.h"

#include "cdib.h"
#include "GlobalApi.h"

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
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHistShow message handlers
