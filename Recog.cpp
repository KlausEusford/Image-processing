
#include "stdafx.h"
#include "cdib.h"
#include "math.h"
#include "GlobalApi.h"

/*************************************************************************
 *
 * \函数名称：
 *   TemplateMatch()
 *
 * \输入参数:
 *   CDib*	pDibSrc		- 指向CDib类的指针，含有待匹配图象信息 
 *   CDib*	pDibTemplate	- 指向CDib类的指针，含有模板图象信息 
 *
 * \返回值:
 *   BOOL			- 成功则返回TRUE，否则返回FALSE
 *
 * \说明:
 *   该函数将对图象进行模板匹配操作。需要注意的是，此程序只处理256灰度级的
 *图象。
 *
 *************************************************************************
 */
BOOL TemplateMatch(CDib* pDibSrc, CDib* pDibTemplate)
{	
	// 指向源图像的指针
	LPBYTE	lpSrc,lpTemplateSrc;
	
	// 指向缓存图像的指针
	LPBYTE	lpDst;

	//循环变量
	long i;
	long j;
	long m;
	long n;

	//中间结果
	double dSigmaST;
	double dSigmaS;
	double dSigmaT;

	//相似性测度
	double R;

	//最大相似性测度
	double  dbMaxR;

	//最大相似性出现位置
	int nMaxWidth;
	int nMaxHeight;

	//像素值
	unsigned char unchPixel;
	unsigned char unchTemplatePixel;

	// 获得图象数据存储的高度和宽度
	CSize sizeSaveImage;
	sizeSaveImage = pDibSrc->GetDibSaveDim();

	// 获得模板图象数据存储的高度和宽度
	CSize sizeSaveTemplate;
	sizeSaveTemplate = pDibTemplate->GetDibSaveDim();


	// 暂时分配内存，以保存新图像
	CDib* pDibNew;
	pDibNew = new CDib;

	// 如果分配内存失败，则推出
	if(!CopyDIB(pDibSrc,pDibNew)){
		// 释放已分配内存
		pDibNew->Empty();

		// 返回
		return FALSE;
	}
	

	// 初始化新分配的内存，设定初始值为255
	lpDst = (LPBYTE)pDibNew->m_lpImage; 
	memset(lpDst, (BYTE)255, pDibNew->GetSizeImage());

	// 图象的高度
	int nImageHeight ;
	nImageHeight = pDibSrc->m_lpBMIH->biHeight;

	// 图象的宽度
	int nImageWidth;
	nImageWidth = pDibSrc->m_lpBMIH->biWidth;

	// 模板图象的高度
	int nTemplateHeight;
	nTemplateHeight = pDibTemplate->m_lpBMIH->biHeight;

	// 模板图象的宽度
	int nTemplateWidth;
	nTemplateWidth = pDibTemplate->m_lpBMIH->biWidth;

	//计算dSigmaT
	dSigmaT = 0;
	for (n = 0;n < nTemplateHeight ;n++)
	{
		for(m = 0;m < nTemplateWidth ;m++)
		{
			// 指向模板图像倒数第j行，第i个象素的指针			
			lpTemplateSrc = (LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m;
			unchTemplatePixel = (unsigned char)*lpTemplateSrc;
			dSigmaT += (double)unchTemplatePixel*unchTemplatePixel;
		}
	}

	//找到图像中最大相似性的出现位置
	 dbMaxR = 0.0;
	for (j = 0;j < nImageHeight - nTemplateHeight +1 ;j++)
	{
		for(i = 0;i < nImageWidth - nTemplateWidth + 1;i++)
		{
			dSigmaST = 0;
			dSigmaS = 0;
	
			for (n = 0;n < nTemplateHeight ;n++)
			{
				for(m = 0;m < nTemplateWidth ;m++)
				{
					// 指向源图像倒数第j+n行，第i+m个象素的指针			
					lpSrc  = (LPBYTE)pDibSrc->m_lpImage + sizeSaveImage.cx * (j+n) + (i+m);
			
					// 指向模板图像倒数第n行，第m个象素的指针			
					lpTemplateSrc  = (LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m;

					unchPixel = (unsigned char)*lpSrc;
					unchTemplatePixel = (unsigned char)*lpTemplateSrc;

					dSigmaS += (double)unchPixel*unchPixel;
					dSigmaST += (double)unchPixel*unchTemplatePixel;
				}
			}
			//计算相似性
			R = dSigmaST / ( sqrt(dSigmaS)*sqrt(dSigmaT));
			//与最大相似性比较
			if (R >  dbMaxR)
			{
				 dbMaxR = R;
				nMaxWidth = i;
				nMaxHeight = j;
			}
		}
	}

	//将最大相似性出现区域部分复制到目标图像
	for (n = 0;n < nTemplateHeight ;n++)
	{
		for(m = 0;m < nTemplateWidth ;m++)
		{
			lpTemplateSrc = (LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m;
			lpDst = (LPBYTE)pDibNew->m_lpImage + sizeSaveImage.cx * (n+nMaxHeight) + (m+nMaxWidth);
			*lpDst = *lpTemplateSrc;
		}
	}
	
	// 复制图像
	memcpy(pDibSrc->m_lpImage, pDibNew->m_lpImage, nImageWidth * nImageHeight);

	// 释放内存
	pDibNew->Empty();
	
	// 返回
	return TRUE;
}

