
#include "stdafx.h"
#include "cdib.h"
#include "math.h"
#include <direct.h>
#include <complex>
using namespace std;
#include "GlobalApi.h"

/*************************************************************************
 *
 * \函数名称：
 *   HistogramEqualize()
 *
 * \输入参数:
 *   CDib * pDib		- 指向CDib类的指针，含有原始图象信息
 *
 * \返回值:
 *   BOOL			- 成功则返回TRUE，否则返回FALSE
 *
 * \说明:
 *   该函数对指定的图象进行直方图均衡化处理
 *
 *************************************************************************
 */
 BOOL HistogramEqualize(CDib* pDib)
 {
	 // 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 临时变量
	int	nTemp;
	
	// 循环变量
	int i,j;
	
	// 累积直方图，即灰度映射表
	BYTE	byMap[256];
	
	// 直方图
	int	nCount[256];
	
	// 图象的高度和宽度
	CSize sizeImage;
	sizeImage = pDib->GetDimensions();


	// 获得图象数据存储的高度和宽度
	CSize SizeSaveImage;
	SizeSaveImage = pDib->GetDibSaveDim();



	// 重置计数为0
	for (i = 0; i < 256; i ++)
	{
		// 清零
		nCount[i] = 0;
	}
	
	// 计算各个灰度值的计数，即得到直方图
	for (i = 0; i < sizeImage.cy; i ++)
	{
		for (j = 0; j < sizeImage.cx; j ++)
		{
			lpSrc = (unsigned char *)pDib->m_lpImage + SizeSaveImage.cx * i + j;
			
			// 计数加1
			nCount[*(lpSrc)]++;
		}
	}
	
	// 计算累积直方图
	for (i = 0; i < 256; i++)
	{
		// 初始为0
		nTemp = 0;
		
		for (j = 0; j <= i ; j++)
		{
			nTemp += nCount[j];
		}
		
		// 计算对应的新灰度值
		byMap[i] = (BYTE) (nTemp * 255 / sizeImage.cy / sizeImage.cx);
	}
	
	// 每行
	for(i = 0; i < sizeImage.cy; i++)
	{
		// 每列
		for(j = 0; j < sizeImage.cx; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)pDib->m_lpImage + pDib->GetPixelOffset(i,j);
			
			// 计算新的灰度值
			*lpSrc = byMap[*lpSrc];
		}
	}
	
	// 返回
	return TRUE;
 }

/*************************************************************************
 *
 * \函数名称：
 *   GraySegLinTrans()
 *
 * \输入参数:
 *   CDib* pDib			- 指向CDib类的指针，含有原始图象信息
 *   int   nX1			- 分段线性灰度变换第一个拐点的X坐标
 *   int   nY1			- 分段线性灰度变换第一个拐点的Y坐标
 *   int   nX2			- 分段线性灰度变换第二个拐点的X坐标
 *   int   nY2			- 分段线性灰度变换第二个拐点的Y坐标
 *
 * \返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * \说明:
 *   该函数用来对图像进行分段线性灰度变换，输入参数中包含了两个拐点的坐标
 *
 *************************************************************************
 */
BOOL GraySegLinTrans(CDib* pDib, int nX1, int nY1, int nX2, int nY2)
{
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 循环变量
	int i,j;
	
	// 灰度映射表
	BYTE	byMap[256];
	
	// 图像每行的字节数
	//LONG	lLineBytes;

	// 图象的高度和宽度
	CSize sizeImage;
	sizeImage = pDib->GetDimensions();


	// 获得图象数据存储的高度和宽度
	CSize SizeSaveImage;
	SizeSaveImage = pDib->GetDibSaveDim();

	// 计算图像每行的字节数
	//lLineBytes = WIDTHBYTES(sizeImage.cx * 8);
	
	// 计算灰度映射表
	for (i = 0; i <= nX1; i++)
	{
		// 判断nX1是否大于0（防止分母为0）
		if (nX1 > 0)
		{
			// 线性变换
			byMap[i] = (BYTE) nY1 * i / nX1;
		}
		else
		{
			// 直接赋值为0
			byMap[i] = 0;
		}
	}
	for (; i <= nX2; i++)
	{
		// 判断nX1是否等于nX2（防止分母为0）
		if (nX2 != nX1)
		{
			// 线性变换
			byMap[i] = nY1 + (BYTE) ((nY2 - nY1) * (i - nX1) / (nX2 - nX1));
		}
		else
		{
			// 直接赋值为nY1
			byMap[i] = nY1;
		}
	}
	for (; i < 256; i++)
	{
		// 判断nX2是否等于255（防止分母为0）
		if (nX2 != 255)
		{
			// 线性变换
			byMap[i] = nY2 + (BYTE) ((255 - nY2) * (i - nX2) / (255 - nX2));
		}
		else
		{
			// 直接赋值为255
			byMap[i] = 255;
		}
	}
	
	// 对图象的象素值进行变换
	// 每行
	for(i = 0; i < sizeImage.cy; i++)
	{
		// 每列
		for(j = 0; j < sizeImage.cx; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)pDib->m_lpImage + pDib->GetPixelOffset(i,j);
			
			// 计算新的灰度值
			*lpSrc = byMap[*lpSrc];
		}
	}
	
	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * \函数名称：
 *   GeneralTemplate()
 *
 * \输入参数:
 *   CDib * pDib		- 指向CDib类的指针，含有原始图象信息
 *   int nTempWidth		- 模板的宽度
 *   int nTempHeight		- 模板的高度
 *   int nTempCenX		- 模板中心的X坐标（相对于模板）
 *   int nTempCenY		- 模板中心的Y坐标（相对于模板）
 *   double* pdbTemp		- 模板数组的指针
 *   double* dbCoef		- 模板的系数
 *
 * \返回值:
 *   BOOL			- 成功则返回TRUE，否则返回FALSE
 *
 * \说明:
 *   该函数用指定的模板对pDib指向的图象进行模板操作。模板的定义了宽度，高度，
 *    中心坐标和系数，模板的数据存放在pdbTemp中。对图象进行模板操作后，仍
 *    然存放在pDib指向的CDib对象中。需要注意的是，该函数只能处理8位的图象，
 *   否则，指向的数据将出错。
 *
 *************************************************************************
 */
BOOL  GeneralTemplate(CDib* pDib,  int nTempWidth, int nTempHeight, 
					 int nTempCenX, int nTempCenY,
					 double* pdbTemp, double dbCoef)
{
	// 临时存放图像数据的指针
	LPBYTE lpImage;
	
	// 循环变量
	int i,j,k,l;
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 指向要复制区域的指针
	unsigned char*	lpDst;

	// 计算结果
	double	dbResult;

	// 图象的高度和宽度
	CSize sizeImage;
	sizeImage = pDib->GetDimensions();


	// 获得图象数据存储的尺寸
	int nSizeImage;
	nSizeImage = pDib->GetSizeImage();

	// 给临时存放数据分配内存
	lpImage = (LPBYTE) new char[nSizeImage];

	// 判断是否内存分配失败
	if (lpImage == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 将原始图像的数据拷贝到临时存放内存中
	memcpy(lpImage, pDib->m_lpImage, nSizeImage);

	
	
	// 进行模板操作
	// 行(除去边缘几行)
	for(i = nTempCenY ; i < sizeImage.cy - nTempHeight + nTempCenY + 1; i++)
	{
		// 列(除去边缘几列)
		for(j = nTempCenX; j < sizeImage.cx - nTempWidth + nTempCenX + 1; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpImage + pDib->GetPixelOffset(i,j);
			
			dbResult = 0;
			
			// 计算
			for (k = 0; k < nTempHeight; k++)
			{
				for (l = 0; l < nTempWidth; l++)
				{
					// 指向DIB第i - nTempCenY + k行，第j - nTempCenX + l个象素的指针
					lpSrc = (unsigned char*)pDib->m_lpImage + pDib->GetPixelOffset(i-nTempCenY+k, j-nTempCenX+l);
					
					// 保存象素值
					dbResult += (* lpSrc) * pdbTemp[k * nTempWidth + l];
				}
			}
			
			// 乘上系数
			dbResult *= dbCoef;
			
			// 取绝对值
			dbResult = (double ) fabs(dbResult);
			
			// 判断是否超过255
			if(dbResult > 255)
			{
				// 直接赋值为255
				* lpDst = 255;
			}
			else
			{
				// 赋值
				* lpDst = (unsigned char) (dbResult + 0.5);
			}
			
		}
	}
	
	// 复制变换后的图像
	memcpy(pDib->m_lpImage, lpImage, nSizeImage);
	
	// 释放内存
	delete[]lpImage;
	
	// 返回
	return TRUE;

}


/*************************************************************************
 *
 * 函数名称：
 *   MedianFilter()
 *
 * \输入参数:
 *   CDib * pDib		- 指向CDib类的指针，含有原始图象信息
 *   int nTempWidth		- 模板的宽度
 *   int nTempHeight		- 模板的高度
 *   int nTempCenX		- 模板中心的X坐标（相对于模板）
 *   int nTempCenY		- 模板中心的Y坐标（相对于模板）
 *
 * \返回值:
 *   BOOL			- 成功则返回TRUE，否则返回FALSE
 *
 * 说明:
 *   该函数对指定的DIB图像进行中值滤波。
 *
 ************************************************************************/

BOOL MedianFilter(CDib* pDib,  int nTempWidth, int nTempHeight, 
					 int nTempCenX, int nTempCenY)
{	
	// 临时存放图像数据的指针
	LPBYTE lpImage;
	
	// 循环变量
	int i,j,k,l;
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 指向要复制区域的指针
	unsigned char*	lpDst;

	// 图象的高度和宽度
	CSize sizeImage;
	sizeImage = pDib->GetDimensions();

	// 获得图象数据存储的尺寸
	int nSizeImage;
	nSizeImage = pDib->GetSizeImage();

	// 指向滤波器数组的指针
	unsigned char* pUnchFltValue;

	// 给临时存放数据分配内存
	lpImage = (LPBYTE) new char[nSizeImage];
	// 判断是否内存分配失败
	if (lpImage == NULL)
	{
		// 返回
		return FALSE;
	}
	
	// 将原始图像的数据拷贝到临时存放内存中
	memcpy(lpImage, pDib->m_lpImage, nSizeImage);
	
	// 暂时分配内存，以保存滤波器数组
	pUnchFltValue = new unsigned char[nTempHeight * nTempWidth];
	
	// 判断是否内存分配失败
	if (pUnchFltValue == NULL)
	{
		// 释放已分配内存
		delete[]lpImage;
		
		// 返回
		return FALSE;
	}
	
	// 开始中值滤波
	// 行(除去边缘几行)
	for(i = nTempCenY; i < sizeImage.cy - nTempHeight + nTempCenY + 1; i++)
	{
		// 列(除去边缘几列)
		for(j = nTempCenX; j < sizeImage.cx - nTempWidth + nTempCenX + 1; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpImage + pDib->GetPixelOffset(i,j);
			//lpDst = (unsigned char*)lpImage + sizeImage.cx * (sizeImage.cy - 1 - i) + j;
			
			// 读取滤波器数组
			for (k = 0; k < nTempHeight; k++)
			{
				for (l = 0; l < nTempWidth; l++)
				{
					// 指向DIB第i - nTempCenY + k行，第j - nTempCenX + l个象素的指针
					lpSrc = (unsigned char*)pDib->m_lpImage + pDib->GetPixelOffset(i-nTempCenY+k, j-nTempCenX+l);
					//lpSrc = (unsigned char*)pDib->m_lpImage + sizeImage.cx * (sizeImage.cy - 1 - i + nTempCenY - k) + j - nTempCenX + l;
					
					// 保存象素值
					pUnchFltValue[k * nTempWidth + l] = *lpSrc;
				}
			}
			
			// 获取中值
			//* lpDst = GetMedianValue(pUnchFltValue, nTempHeight * nTempWidth);
		}
	}
	
	// 复制变换后的图像
	memcpy(pDib->m_lpImage, lpImage, nSizeImage);
	
	// 释放内存
	delete[]lpImage;
	delete[]pUnchFltValue;

	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   GetMedianValue()
 *
 * 参数:
 *   unsigned char * pUnchFltValue	- 指向要获取中值的数组指针
 *   int   iFilterLen			- 数组长度
 *
 * 返回值:
 *   unsigned char			- 返回指定数组的中值。
 *
 * 说明:
 *   该函数用冒泡法对一维数组进行排序，并返回数组元素的中值。
 *
 ************************************************************************/

unsigned char  GetMedianValue(unsigned char * pUnchFltValue, int iFilterLen)
{
	// 循环变量
	int		i;
	int		j;
	
	// 中间变量
	unsigned char bTemp;
	
	// 用冒泡法对数组进行排序
	for (j = 0; j < iFilterLen - 1; j ++)
	{
		for (i = 0; i < iFilterLen - j - 1; i ++)
		{
			if (pUnchFltValue[i] > pUnchFltValue[i + 1])
			{
				// 互换
				bTemp = pUnchFltValue[i];
				pUnchFltValue[i] = pUnchFltValue[i + 1];
				pUnchFltValue[i + 1] = bTemp;
			}
		}
	}
	
	// 计算中值
	if ((iFilterLen & 1) > 0)
	{
		// 数组有奇数个元素，返回中间一个元素
		bTemp = pUnchFltValue[(iFilterLen + 1) / 2];
	}
	else
	{
		// 数组有偶数个元素，返回中间两个元素平均值
		bTemp = (pUnchFltValue[iFilterLen / 2] + pUnchFltValue[iFilterLen / 2 + 1]) / 2;
	}
	
	// 返回中值
	return bTemp;
}

/*************************************************************************
 *
 * \函数名称：
 *   LinearSharpen()
 *
 * \输入参数:
 *   LPBYTE lpImage		- 指向图象数据得指针
 *   int nWidth			- 图象数据宽度
 *   int nHeight		- 图象数据高度
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   线性锐化图象增强
 *   本函数采用拉普拉斯算子对图象进行线性锐化
 *   在原来图象上加上拉普拉斯算子锐化的信息
 *
 *************************************************************************
 */
void LinearSharpen (LPBYTE lpImage, int nWidth, int nHeight)
{
	// 遍历图象的纵坐标
	int y;

	// 遍历图象的横坐标
	int x;

	double * pdGrad ;
	pdGrad = new double[nWidth*nHeight];

	// 初始化为0
	memset(pdGrad, 0, nWidth*nHeight*sizeof(double)) ;

	// 设置模板系数
	static int nWeight[3][3] ;
	nWeight[0][0] = -1 ;   
	nWeight[0][1] = -1 ;   
	nWeight[0][2] = -1 ;   
	nWeight[1][0] = -1 ;   
	nWeight[1][1] =  8 ;   
	nWeight[1][2] = -1 ;   
	nWeight[2][0] = -1 ;   
	nWeight[2][1] = -1 ;   
	nWeight[2][2] = -1 ;   

	//这个变量用来表示Laplacian算子象素值
	int nTmp[3][3];

	// 临时变量
	double dGrad;

	// 模板循环控制变量
	int yy ;
	int xx ;
	for(y=1; y<nHeight-1 ; y++ )
	 for(x=1 ; x<nWidth-1 ; x++ )
	 {
		 dGrad = 0 ; 
		 // Laplacian算子需要的各点象素值
		 
		 // 模板第一行
		 nTmp[0][0] = lpImage[(y-1)*nWidth + x - 1 ] ; 
		 nTmp[0][1] = lpImage[(y-1)*nWidth + x     ] ; 
		 nTmp[0][2] = lpImage[(y-1)*nWidth + x + 1 ] ; 
		 
		 // 模板第二行
		 nTmp[1][0] = lpImage[y*nWidth + x - 1 ] ; 
		 nTmp[1][1] = lpImage[y*nWidth + x     ] ; 
		 nTmp[1][2] = lpImage[y*nWidth + x + 1 ] ; 
		 
		 // 模板第三行
		 nTmp[2][0] = lpImage[(y+1)*nWidth + x - 1 ] ; 
		 nTmp[2][1] = lpImage[(y+1)*nWidth + x     ] ; 
		 nTmp[2][2] = lpImage[(y+1)*nWidth + x + 1 ] ; 
		 
		 // 计算梯度
		 for(yy=0; yy<3; yy++)
			 for(xx=0; xx<3; xx++)
			 {
				 dGrad += nTmp[yy][xx] * nWeight[yy][xx] ;
			 }
			 
			 // 梯度值写入内存
			 *(pdGrad+y*nWidth+x)=dGrad;
	 }
	for(y=0; y<nHeight ; y++ )
	{
		for(x=0 ; x<nWidth ; x++ )
		{
			lpImage[y*nWidth+x] = (unsigned char)max(0,min(255,(lpImage[y*nWidth+x] + (int)pdGrad[y*nWidth+x]) ));
		}
	}
  delete []pdGrad ;
	pdGrad = NULL   ;
}


/*************************************************************************
 *
 * 函数名称：
 *   ReplaceDIBColorTable()
 *
 * 参数:
 *   CDib * pDib		- 指向CDib类的指针，含有原始图象信息
 *   LPBYTE pColorsTable	- 伪彩色编码表
 *
 * 返回值:
 *   BOOL			- 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用指定的伪彩色编码表来替换指定的DIB图像的颜色表，参数pColorsTable
 *   指向要替换的伪彩色编码表。
 *
 ************************************************************************/
BOOL  ReplaceDIBColorTable(CDib* pDib, LPBYTE  pColorsTable)
{
	
	// 循环变量
	int i;

	// 颜色表的表项数
	int nColorEntries;

	// 临时变量
	LPRGBQUAD pDibQuad;

	// 指向DIB的颜色表
	pDibQuad = (LPRGBQUAD) pDib->m_lpvColorTable;

	// 获取DIB中颜色表中的表项数
	nColorEntries = pDib->m_nColorTableEntries;
	
	// 判断颜色数目是否是256色
	if (nColorEntries == 256)
	{			
		// 读取伪彩色编码，更新DIB颜色表
		for (i = 0; i < nColorEntries; i++)
		{		
			// 更新DIB调色板红色分量
			pDibQuad->rgbRed = pColorsTable[i * 4];
				
			// 更新DIB调色板绿色分量
			pDibQuad->rgbGreen = pColorsTable[i * 4 + 1];
				
			// 更新DIB调色板蓝色分量
			pDibQuad->rgbBlue = pColorsTable[i * 4 + 2];
				
			// 更新DIB调色板保留位
			pDibQuad->rgbReserved = 0;

			pDibQuad++;
		
		}
	}

	// 如果不是256色的DIB，则不进行处理
	else
		return FALSE;
	
	// 返回
	return TRUE;

}


/*************************************************************************
 *
 * \函数名称：
 *   LowPassFilterEnhance()
 *
 * \输入参数:
 *   LPBYTE lpImage					- 指向需要增强得图象数据
 *   int nWidth							- 数据宽度
 *   int nHeight						- 数据高度
 *   int nRadius            - 低通滤波的滤波半径
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   lpImage 是指向需要增强的数据指针。注意，这个指针指向的数据区不能是CDib指向的数据区
 *	 因为CDib指向的数据区的每一行是DWORD对齐的。
 *   经过低通滤波的数据存储在lpImage当中。
 *
 *************************************************************************
 */
void LowPassFilterEnhance(LPBYTE lpImage, int nWidth, int nHeight, int nRadius)
{
	// 循环控制变量
	int y ;
	int x ;

	double dTmpOne ;
	double dTmpTwo ;

	// 傅立叶变换的宽度和高度（2的整数次幂）
	int nTransWidth ;
	int nTransHeight;

	// 图象象素值
	unsigned char unchValue;

	// 指向时域数据的指针
	complex<double> * pCTData ;
	// 指向频域数据的指针
	complex<double> * pCFData ;
	
	// 计算进行傅立叶变换的点数	（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// 计算进行傅立叶变换的点数 （2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;

	// 傅立叶变换的实部和虚部
	double dReal;
	double dImag;

	// 低通滤波的半径不能超过频域的最大半径
	if(nRadius>nTransWidth-1 || nRadius>nTransHeight-1)
	{
		return ;
	}
	
	// 分配内存
	pCTData=new complex<double>[nTransWidth * nTransHeight];
	pCFData=new complex<double>[nTransWidth * nTransHeight];

	// 初始化
	// 图像数据的宽和高不一定是2的整数次幂，所以pCTData
	// 有一部分数据需要补0
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x]=complex<double>(0,0);
		}
	}

	// 把图像数据传给pCTData
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nWidth +x];
			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
		}
	}
	
	// 傅立叶正变换
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData) ;

	// 下面开始实施低通滤波，把所有大于nRadius的高频分量设置为0
	//  注意这里高频分量采用的范数不是欧式距离，而是无穷大范数
	//  || (u,v)-(0,0) || = max(|u|,|v|) 
	for(y=nRadius; y<nTransHeight; y++)
	{
		for(x=nRadius; x<nTransWidth; x++)
		{
			pCFData[y*nTransWidth + x]=complex<double>(0,0);
		}
	}
	
	// 经过低通滤波的图象进行反变换
	IFFT_2D(pCFData, pCTData, nWidth, nHeight);

	// 反变换的数据传给lpImage
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dReal = pCTData[y*nTransWidth + x].real() ;
			dImag = pCTData[y*nTransWidth + x].imag() ;
			unchValue = (unsigned char)max(0,min(255,sqrt(dReal*dReal+dImag*dImag) ));
			lpImage[y*nWidth + x] = unchValue ;
		}
	}

	
	// 释放内存
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;
	
}

/*************************************************************************
 *
 * \函数名称：
 *   HighPassFilterEnhance()
 *
 * \输入参数:
 *   LPBYTE lpImage					- 指向需要增强得图象数据
 *   int nWidth							- 数据宽度
 *   int nHeight						- 数据高度
 *   int nRadius            - 高通滤波的滤波半径
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   lpImage 是指向需要增强的数据指针。注意，这个指针指向的数据区不能是CDib指向的数据区
 *	 因为CDib指向的数据区的每一行是DWORD对齐的。
 *   经过高通滤波的数据存储在lpImage当中。
 *
 *************************************************************************
 */
void HighPassFilterEnhance(LPBYTE lpImage, int nWidth, int nHeight, int nRadius)
{
	// 循环控制变量
	int y ;
	int x ;

	double dTmpOne ;
	double dTmpTwo ;

	// 傅立叶变换的宽度和高度（2的整数次幂）
	int nTransWidth ;
	int nTransHeight;

	// 图象象素值
	unsigned char unchValue;

	// 指向时域数据的指针
	complex<double> * pCTData ;
	// 指向频域数据的指针
	complex<double> * pCFData ;

	double dReal;
	double dImag;
	
	// 计算进行傅立叶变换的点数	（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// 计算进行傅立叶变换的点数 （2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;

	// 滤波的半径不能超过频域的最大半径
	if(nRadius>nTransWidth-1 || nRadius>nTransHeight-1)
	{
		return ;
	}
	
	// 分配内存
	pCTData=new complex<double>[nTransWidth * nTransHeight];
	pCFData=new complex<double>[nTransWidth * nTransHeight];

	// 初始化
	// 图像数据的宽和高不一定是2的整数次幂，所以pCTData
	// 有一部分数据需要补0
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x]=complex<double>(0,0);
		}
	}

	// 把图像数据传给pCTData
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nWidth +x];
			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
		}
	}
	
	// 傅立叶正变换
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData) ;

	// 下面开始滤波，把所有小于nRadius的低频分量设置为0
	// 采用的范数不是欧式距离，而是无穷大范数
	//  || (u,v)-(0,0) || = max(|u|,|v|) 
	for(y=0; y<nRadius; y++)
	{
		for(x=0; x<nRadius; x++)
		{
			pCFData[y*nTransWidth + x]=complex<double>(0,0);
		}
	}
	// 经过滤波的图象进行反变换
	IFFT_2D(pCFData, pCTData, nWidth, nHeight);

	// 反变换的数据传给lpImage
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
		  // 因为高通滤波器会把低频成分去掉，所以图象看起来会比较暗，为了解决这个问题
			// 在经过增强后的图象加上一个常数，使图象变得亮起来。严格来讲这种处理相当于
			// 增加了一些低频分量（在频率(0,0)）。不过如果不加一个常数，高通效果在观看
			// 上就不是很方便。
			dReal = pCTData[y*nTransWidth + x].real() ;
			dImag = pCTData[y*nTransWidth + x].imag() ;
			unchValue = (unsigned char)max(0,min(255,(sqrt(dReal*dReal+dImag*dImag)+100 )));
			lpImage[y*nWidth + x] = unchValue ;
		}
	}

	
	// 释放内存
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;
	
}

/*************************************************************************
 *
 * \函数名称：
 *   ButterWorthLowPass()
 *
 * \输入参数:
 *   LPBYTE lpImage					- 指向需要增强得图象数据
 *   int nWidth							- 数据宽度
 *   int nHeight						- 数据高度
 *   int nRadius            - ButterWorth低通滤波的“半功率”点
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   lpImage 是指向需要增强的数据指针。注意，这个指针指向的数据区不能是CDib指向的数据区
 *	 因为CDib指向的数据区的每一行是DWORD对齐的。
 *   经过ButterWorth低通滤波的数据存储在lpImage当中。
 *
 *************************************************************************
 */
void ButterWorthLowPass(LPBYTE lpImage, int nWidth, int nHeight, int nRadius)
{
	// 循环控制变量
	int y ;
	int x ;

	double dTmpOne ;
	double dTmpTwo ;

	// ButterWorth 滤波系数
	double H       ;

	// 傅立叶变换的宽度和高度（2的整数次幂）
	int nTransWidth ;
	int nTransHeight;

	double dReal    ;
	double dImag    ;
	// 图象象素值
	unsigned char unchValue;

	// 指向时域数据的指针
	complex<double> * pCTData ;
	// 指向频域数据的指针
	complex<double> * pCFData ;
	
	// 计算进行傅立叶变换的点数	（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// 计算进行傅立叶变换的点数 （2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;
	
	// 分配内存
	pCTData=new complex<double>[nTransWidth * nTransHeight];
	pCFData=new complex<double>[nTransWidth * nTransHeight];

	// 初始化
	// 图像数据的宽和高不一定是2的整数次幂，所以pCTData
	// 有一部分数据需要补0
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x]=complex<double>(0,0);
		}
	}

	// 把图像数据传给pCTData
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nWidth +x];
			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
		}
	}
	
	// 傅立叶正变换
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData) ;

	// 下面开始实施ButterWorth低通滤波
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			H = (double)(y*y+x*x) ; 
			H = H / (nRadius * nRadius); 
			H = 1/(1+H)            ;
			pCFData[y*nTransWidth + x]=complex<double>(pCFData[y*nTransWidth + x].real()*H,
																								 pCFData[y*nTransWidth + x].imag()*H);
		}
	}
	
	// 经过ButterWorth低通滤波的图象进行反变换
	IFFT_2D(pCFData, pCTData, nWidth, nHeight);

	// 反变换的数据传给lpImage
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dReal = pCTData[y*nTransWidth + x].real() ;
			dImag = pCTData[y*nTransWidth + x].imag() ;
			unchValue = (unsigned char)max(0,min(255,sqrt(dReal*dReal+dImag*dImag) ));
			lpImage[y*nWidth + x] = unchValue ;
		}
	}

	
	// 释放内存
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;
	
}
/*************************************************************************
 *
 * \函数名称：
 *   ButterWorthHighPass()
 *
 * \输入参数:
 *   LPBYTE lpImage					- 指向需要增强得图象数据
 *   int nWidth							- 数据宽度
 *   int nHeight						- 数据高度
 *   int nRadius            - ButterWorth高通滤波的“半功率”点
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   lpImage 是指向需要增强的数据指针。注意，这个指针指向的数据区不能是
 *	 CDib指向的数据区， 因为CDib指向的数据区的每一行是DWORD对齐的。
 *   经过ButterWorth高通滤波的数据存储在lpImage当中。
 *
 *************************************************************************
 */
void ButterWorthHighPass(LPBYTE lpImage, int nWidth, int nHeight, int nRadius)
{
	// 循环控制变量
	int y ;
	int x ;

	double dTmpOne ;
	double dTmpTwo ;

	// ButterWorth 滤波系数
	double H       ;

	// 傅立叶变换的宽度和高度（2的整数次幂）
	int nTransWidth ;
	int nTransHeight;

	double dReal    ;
	double dImag    ;
	// 图象象素值
	unsigned char unchValue;

	// 指向时域数据的指针
	complex<double> * pCTData ;
	// 指向频域数据的指针
	complex<double> * pCFData ;
	
	// 计算进行傅立叶变换的点数	（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// 计算进行傅立叶变换的点数 （2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;
	
	// 分配内存
	pCTData=new complex<double>[nTransWidth * nTransHeight];
	pCFData=new complex<double>[nTransWidth * nTransHeight];

	// 初始化
	// 图像数据的宽和高不一定是2的整数次幂，所以pCTData
	// 有一部分数据需要补0
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x]=complex<double>(0,0);
		}
	}

	// 把图像数据传给pCTData
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nWidth +x];
			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
		}
	}
	
	// 傅立叶正变换
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData) ;

	// 下面开始实施ButterWorth高通滤波
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			
			H = (double)(y*y+x*x) ; 
			H = (nRadius * nRadius) / H ; 
			H = 1/(1+H)            ;
			pCFData[y*nTransWidth + x]=complex<double>(H*(pCFData[y*nTransWidth + x].real()),
																								 H*(pCFData[y*nTransWidth + x].imag())  );
		}
	}
	
	// 经过ButterWorth高通滤波的图象进行反变换
	IFFT_2D(pCFData, pCTData, nWidth, nHeight);

	// 反变换的数据传给lpImage
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dReal = pCTData[y*nTransWidth + x].real() ;
			dImag = pCTData[y*nTransWidth + x].imag() ;
			unchValue = (unsigned char)max(0,min(255,sqrt(dReal*dReal+dImag*dImag)+100 ));
			lpImage[y*nWidth + x] = unchValue ;
		}
	}

	
	// 释放内存
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;
	
}

