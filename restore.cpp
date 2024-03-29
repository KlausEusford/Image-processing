// ************************************************************************
//  文件名：restore.cpp
//
//  图像复原API函数库：
//
//  DIBNoRestriction()			- 图像模糊
//  DIBInverseFilter()			- 图像逆滤波复原
//  DIBNoiseDegeneration()		- 图像模糊加噪
//  DIBWinnerFilter()			- 图像维纳滤波
//	DIBMotionDegeneration()		- 图像运动模糊
//	DIBMotionRestore()			- 图像运动模糊复原
//
// *************************************************************************


#include "stdafx.h"
#include "GlobalApi.h"
#include "Cdib.h"

#include <math.h>
#include <direct.h>
#include <complex>
using namespace std;

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

/*************************************************************************
 *
 * 函数名称：
 *   DIBNoRestriction()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行模糊操作。
 *
 ************************************************************************/

BOOL WINAPI DIBNoRestriction(CDib *pDib)
{
	// 指向源图像的指针
	BYTE *	lpSrc;

	//图象的宽度和高度
	LONG    lWidth;
	LONG    lHeight;

	// 图像每行的字节数
	LONG	lLineBytes;
	
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 计算图像每行的字节数
	lLineBytes = SizeRealDim.cx;
	
	//图像数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;
	
	//循环变量
	long i;
	long j;

	//临时变量
	double temp;

	// 实际进行付立叶变换的宽度和高度
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// 保证离散傅立叶变换的宽度和高度为2的整数次方
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//用来存储源图象和变换核的时域数据
	complex<double> *pCTSrc,*pCTH;

	//用来存储源图象和变换核的频域数据
	complex<double>  *pCFSrc,*pCFH;
	
	//图像归一化因子
	double MaxNum;
	
	//输入图象的长和宽必须为2的整数倍
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// 为时域和频域的数组分配空间
	pCTSrc = new complex<double> [lHeight*lLineBytes];
	pCTH   = new complex<double> [lHeight*lLineBytes];
	
	pCFSrc = new complex<double> [lHeight*lLineBytes];
	pCFH   = new complex<double> [lHeight*lLineBytes];

	// 将数据存入时域数组
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lLineBytes; i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			if(i < 5 && j < 5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
			}
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}

	//对源图像进行FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//对变换核图像进行FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);

	//频域相乘
	for (i = 0;i <lHeight*lLineBytes;i++)
	{
		pCFSrc[i] = pCFSrc[i]*pCFH[i];
	}

	//对结果图像进行反FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//确定归一化因子
	MaxNum = 0.0;
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			temp = sqrt(pCTSrc[ lLineBytes*j + i ].real() * pCTSrc[ lLineBytes*j + i ].real()
						+pCTSrc[lLineBytes*j + i ].imag() * pCTSrc[ lLineBytes*j +i].imag());
			
			//选择归一化因子
			if( MaxNum < temp)
				MaxNum = temp;
		}
	}
	
	//转换为图像
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			*lpSrc = (unsigned char) (pCTSrc[(lLineBytes)*j + i].real()*255.0/MaxNum);
		}
	}
	
	//释放存储空间
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;

	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBInverseFilter()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE
 *
 * 说明:
 *   该函数用来对DIBNoRestriction()生成的DIB图像进行复原操作。
 *
 ************************************************************************/

BOOL WINAPI DIBInverseFilter (CDib *pDib)
{
	
    // 指向源图像的指针
	BYTE *	lpSrc;

	//图象的宽度和高度
	LONG    lWidth;
	LONG    lHeight;

	// 图像每行的字节数
	LONG	lLineBytes;
	
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 计算图像每行的字节数
	lLineBytes = SizeRealDim.cx;
	
	//图像数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;
	
	//循环变量
	long i;
	long j;

	//临时变量
	double tempre, tempim, a, b, c, d;

	// 实际进行付立叶变换的宽度和高度
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// 保证离散傅立叶变换的宽度和高度为2的整数次方
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//用来存储源图象和变换核的时域数据
	complex<double> *pCTSrc,*pCTH;

	//用来存储源图象和变换核的频域数据
	complex<double>  *pCFSrc,*pCFH;
	
	//图像归一化因子
	double MaxNum;
	
	//输入退化图象的长和宽必须为2的整数倍
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// 为时域和频域的数组分配空间
	pCTSrc = new complex<double> [lHeight*lLineBytes];
	pCTH   = new complex<double> [lHeight*lLineBytes];
	
	pCFSrc = new complex<double> [lHeight*lLineBytes];
	pCFH   = new complex<double> [lHeight*lLineBytes];

	// 将退化图象数据存入时域数组
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lLineBytes; i++)
		{
			// 指向退化图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			if(i < 5 && j < 5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
			}
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}

	//对退化图像进行FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//对变换核图像进行FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);
	
	//频域相除
	for (i = 0;i <lHeight*lLineBytes;i++)
	{
		a = pCFSrc[i].real();
		b = pCFSrc[i].imag();
		c = pCFH[i].real();
		d = pCFH[i].imag();
		
		//如果频域值太小，不予考虑
		if (c*c + d*d > 1e-3)
		{
			tempre = ( a*c + b*d ) / ( c*c + d*d );
			tempim = ( b*c - a*d ) / ( c*c + d*d );
		}

		pCFSrc[i]= complex<double>(tempre , tempim);
	}

	//对复原图像进行反FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//确定归一化因子
	MaxNum=300;

	//转换为复原图像
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向复原图像倒数第j行，第i个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			*lpSrc = (unsigned char) (pCTSrc[(lLineBytes)*j + i].real()*255.0/MaxNum);
		}
	}
	
	//释放存储空间
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;

	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBNoiseDegeneration()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 模糊加噪操作成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行模糊加噪操作。
 *
 ************************************************************************/

BOOL WINAPI DIBNoiseDegeneration (CDib *pDib)
{
	// 指向源图像的指针
	BYTE *	lpSrc;

	//图象的宽度和高度
	LONG    lWidth;
	LONG    lHeight;

	// 图像每行的字节数
	LONG	lLineBytes;
	
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 计算图像每行的字节数
	lLineBytes = SizeRealDim.cx;
	
	//图像数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;
	
	//循环变量
	long i;
	long j;

	//转换为图像，加噪
	unsigned char NoisePoint;
	
	//临时变量
	double temp;

	//图像归一化因子
	double MaxNum;

	// 实际进行付立叶变换的宽度和高度
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// 保证离散傅立叶变换的宽度和高度为2的整数次方
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//用来存储源图象和变换核的时域数据
	complex<double> *pCTSrc,*pCTH;

	//用来存储源图象和变换核的频域数据
	complex<double>  *pCFSrc,*pCFH;
		
	// 为时域和频域的数组分配空间
	pCTSrc = new complex<double> [lHeight*lLineBytes];
	pCTH   = new complex<double> [lHeight*lLineBytes];
	
	pCFSrc = new complex<double> [lHeight*lLineBytes];
	pCFH   = new complex<double> [lHeight*lLineBytes];
	
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针				
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			// 将象素值存储到时域数组中
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			
			// 频域赋零值
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			// 用来对图象做退化的系统
			if(i < 5 && j <5 )
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
			}
			
			// 频域赋零值
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}

	//对源图像进行FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//对变换核图像进行FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);

	//频域相乘
	for (i = 0;i <lHeight*lLineBytes;i++)
	{
		pCFSrc[i] = pCFSrc[i]*pCFH[i];
	}

	//对结果图像进行反FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//确定归一化因子
	MaxNum = 0.0;
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			temp = sqrt(pCTSrc[ lLineBytes*j + i ].real() * pCTSrc[ lLineBytes*j + i ].real()
						+pCTSrc[lLineBytes*j + i ].imag() * pCTSrc[ lLineBytes*j +i].imag());
			
			//选择归一化因子
			if( MaxNum < temp)
				MaxNum = temp;
		}
	}
	
	//生成伪随机数种子
	srand((unsigned)time(NULL));

	//转换为图像，并加入伪随机噪声
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 产生的噪声
			NoisePoint = rand()/2048-8;
			
			// 指向源图像倒数第j行，第i个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			// 时域加噪，存储象素值
			*lpSrc = (unsigned char) (pCTSrc[(lLineBytes)*j + i].real()*255.0/MaxNum + NoisePoint);
			
			//如果象素值过大，直接赋值255
			if(*lpSrc > 255)
				*lpSrc = 255 ;
		}
	}	
	
	//释放存储空间
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;

	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBWinnerFilter()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 维纳滤波复原操作成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像进行维纳滤波复原操作。
 *
 ************************************************************************/

BOOL WINAPI DIBWinnerFilter (CDib *pDib)
{
	// 指向源图像的指针
	BYTE *	lpSrc;

	//图象的宽度和高度
	LONG    lWidth;
	LONG    lHeight;

	// 图像每行的字节数
	LONG	lLineBytes;
	
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 计算图像每行的字节数
	lLineBytes = SizeRealDim.cx;
	
	//图像数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;
	
	//循环变量
	long i;
	long j;

	//临时变量
	double temp, tempre, tempim, 
			a, b, c, d, norm2;
	
	// 实际进行付立叶变换的宽度和高度
	LONG	lW = 1;
	LONG	lH = 1;
	
	int		wp = 0;
	int		hp = 0;

	// 保证离散傅立叶变换的宽度和高度为2的整数次方
	while(lW * 2 <= lLineBytes)
	{
		lW = lW * 2;
		wp++;
	}
	
	while(lH * 2 <= lHeight)
	{
		lH = lH * 2;
		hp++;
	}

	//用来存储源图象和变换核的时域数据
	complex<double> *pCTSrc,*pCTH;

	//用来存储源图象和变换核的频域数据
	complex<double>  *pCFSrc,*pCFH;
	
	//输入退化图象的长和宽必须为2的整数倍
	if(lW != (int) lLineBytes)
	{
		return false;
	}

	if(lH != (int) lHeight)
	{
		return false;
	}

	// 为时域和频域的数组分配空间
	pCTSrc		= new complex<double> [lHeight*lLineBytes];
	pCTH		= new complex<double> [lHeight*lLineBytes];
	
	pCFSrc		= new complex<double> [lHeight*lLineBytes];
	pCFH		= new complex<double> [lHeight*lLineBytes];

	// 滤波器加权系数
	double *pCFFilter   = new double [lHeight*lLineBytes];

	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向退化图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
	
			// 将象素值存储到时域数组中
			pCTSrc[ lLineBytes*j + i ] = complex<double>((double)*lpSrc , 0);
			
			// 频域赋零值
			pCFSrc[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
	
			// 退化系统时域及维纳滤波加权系数赋值
			if(i < 5 && j <5)
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.04 , 0.0);
				pCFFilter[ lLineBytes*j + i ] = 0.5;
			}
			else
			{
				pCTH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);	
				pCFFilter[ lLineBytes*j + i ] = 0.05;
			}

			// 频域赋零值
			pCFH[ lLineBytes*j + i ] = complex<double>(0.0 , 0.0);
		}
	}
	
	//对退化图像进行FFT
	::DIBFFT_2D(pCTSrc, lLineBytes, lHeight, pCFSrc);
	
	//对变换核图像进行FFT
	::DIBFFT_2D(pCTH, lLineBytes, lHeight, pCFH);
	
	// 计算M
	for (i = 0; i < lHeight * lLineBytes; i++)
	{
			// 赋值
			a = pCFSrc[i].real();
			b = pCFSrc[i].imag();
			c = pCFH[i].real();
			d = pCFH[i].imag();

			// |H(u,v)|*|H(u,v)|
			norm2 = c * c + d * d;

			// |H(u,v)|*|H(u,v)|/(|H(u,v)|*|H(u,v)|+a)
			temp  = (norm2 ) / (norm2 + pCFFilter[i]);

			{				
				tempre = ( a*c + b*d ) / ( c*c + d*d );
				tempim = ( b*c - a*d ) / ( c*c + d*d );

				// 求得f(u,v)
				pCFSrc[i]= complex<double>(temp*tempre , temp*tempim);
			}						
	}

	//对复原图像进行反FFT
	IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

	//转换为复原图像
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// 指向复原图像倒数第j行，第i个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			a = pCTSrc[(lLineBytes)*j + i].real();
			b = pCTSrc[(lLineBytes)*j + i].imag();

			norm2  = a*a + b*b;
			norm2  = sqrt(norm2) + 40;
	
			if(norm2 > 255)
				norm2 = 255.0;
			if(norm2 < 0)
				norm2 = 0;	

			*lpSrc = (unsigned char) (norm2);			
		}
	}

	//释放存储空间
	delete pCTSrc;
	delete pCTH;

	delete pCFSrc;
	delete pCFH;
	delete pCFFilter;

	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBMotionDegeneration()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对DIB图像模拟由匀速直线运动造成的模糊
 *
 ***********************************************************************
 */

BOOL WINAPI DIBMotionDegeneration(CDib *pDib)
{
	// 指向源图像的指针
	BYTE *	lpSrc;

	//图象的宽度和高度
	LONG    lWidth;
	LONG    lHeight;

	// 图像每行的字节数
	LONG	lLineBytes;
	
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 计算图像每行的字节数
	lLineBytes = SizeRealDim.cx;
	
	//图像数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;
	
	//循环变量
	long iColumn;
	long jRow;

	//临时变量
	int temp,m;

	// 临时变量
	double p,q;
	
	int nTotTime, nTotLen, nTime;

	//总的运动时间10s
	nTotTime = 10;

	// 总的运动距离10个象素点
	nTotLen = 10;
	
	// 摄像机的暴光系数
	double B;

	B = 0.1;

	//用来存储源图象和变换核的时域数据
	int *nImageDegener;

	// 为时域和频域的数组分配空间
	nImageDegener = new int [lHeight*lLineBytes];
	
	// 将数据存入时域数组
	for (jRow = 0; jRow < lHeight; jRow++)
	{
		for(iColumn = 0; iColumn < lLineBytes; iColumn++)
		{
			temp=0;
			
			// 指向源图像倒数第jRow行，第iColumn个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + iColumn;

			// 象素点的象素值积累
			for ( nTime = 0; nTime < nTotTime; nTime++ )
			{
				p = (float)iColumn - (float)(nTotLen)*nTime/nTotTime;
				
				if (p > 0)
				{
					q = p - floor((double)p);
					
					if(q >= 0.5)
						m = (int)ceil((double)p);
					else
						m = (int)floor((double)p);

					// 累加
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + m;
					temp = temp + *lpSrc;
				}	
			}
			
			// 乘以摄像机的暴光系数
			temp = B * temp;

			temp=(int)ceil((double)temp);
			
			// 使得temp的取值符合取值范围
			if(temp<0)
				temp=0;

			if(temp>255)
				temp=255;

			nImageDegener[lLineBytes*jRow + iColumn] = temp;
		}
	}	
	
	//转换为图像
	for (jRow = 0;jRow < lHeight ;jRow++)
	{
		for(iColumn = 0;iColumn < lLineBytes ;iColumn++)
		{
			// 指向源图像倒数第jRow行，第iColumn个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + iColumn;
	
			*lpSrc = nImageDegener[lLineBytes*jRow + iColumn];
		}
	}
	
	//释放存储空间
	delete nImageDegener;
	
	// 返回
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBMotionRestore()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对拟由匀速直线运动造成的模糊图象进行复原
 *
 ***********************************************************************
 */

BOOL WINAPI DIBMotionRestore(CDib *pDib)
{
	// 指向源图像的指针
	BYTE *	lpSrc;

	//图象的宽度和高度
	LONG    lWidth;
	LONG    lHeight;

	// 图像每行的字节数
	LONG	lLineBytes;
	
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 计算图像每行的字节数
	lLineBytes = SizeRealDim.cx;
	
	//图像数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;
	
	//循环变量
	long iColumn;
	long jRow;

	int i,n,m;

	//临时变量
	int temp1,temp2,
		totalq,q1,q2,z;

	double p,q;

	// 常量A赋值
	int A = 80;
	
	//常量B赋值
	int B = 10;
	
	//总的移动距离
	int nTotLen=10;

	// 图象宽度包含多少个ntotlen
	int K=lLineBytes/nTotLen;
	
	int error[10];

	//用来存储源图象时域数据
	int *nImageDegener;

	// 为时域数组分配空间
	nImageDegener = new int [lHeight*lLineBytes];

	// 将象素存入数组中
	for (jRow = 0; jRow < lHeight; jRow++)
	{
		for(iColumn = 0; iColumn < lLineBytes; iColumn++)
		{
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + iColumn;
	
			nImageDegener[lLineBytes*jRow + iColumn] = (*lpSrc);
		}
	}	
	
	for (jRow = 0; jRow < lHeight; jRow++)
	{		
		// 计算error[i]
		for(i = 0; i < 10; i++)
		{
			
			error[i] = 0;
			
			for(n = 0; n < K; n++)
				for(m = 0; m <= n; m++)
				{
					// 象素是否为一行的开始处
					if(i == 0 && m == 0)
					{
						temp1=temp2=0;
					}
					
					// 进行差分运算
					else
					{
						lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + m*10+i;
						temp1=*lpSrc;
						
						lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + m*10+i-1;
						temp2 = *lpSrc;
					}
					
					error[i] = error[i] + temp1 - temp2;
					
				}				
			
				error[i] = B * error[i] / K;
		}
		
		for(iColumn = 0; iColumn < lLineBytes; iColumn++)
		{			
			// 计算m，以及z
			m = iColumn / nTotLen;
			z = iColumn - m*nTotLen;			
			
			// 初始化
			totalq = 0;
			q = 0;
			
			for(n = 0; n <= m; n++)
			{
				q1 = iColumn - nTotLen*n;
				
				if(q1 == 0)
					q = 0;
				
				// 进行差分运算
				else
				{
					q2 = q1 - 1;
					
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + q1;
					temp1 = *lpSrc;
					
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + q2;
					temp2 = *lpSrc;
					
					q = (temp1 - temp2) * B;				
				}
				
				totalq = totalq + q;
			}
			
			p = error[z];

			// 得到f(x,y)的值
			temp1 = totalq + A - p;
			
			// 使得象素的取值符合取值范围
			if(temp1 < 0)
				temp1 = 0;
			
			if(temp1 > 255)
				temp1 = 255;
						
			nImageDegener[lLineBytes*jRow + iColumn] = temp1;
		}
	}

	//转换为图像
	for (jRow = 0;jRow < lHeight ;jRow++)
	{
		for(iColumn = 0;iColumn < lLineBytes ;iColumn++)
		{
			// 指向源图像倒数第jRow行，第iColumn个象素的指针			
 			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * jRow + iColumn;
	
			// 存储象素值
			*lpSrc = nImageDegener[lLineBytes*jRow + iColumn];
		}
	}	
	
	//释放存储空间
	delete nImageDegener;
	
	// 返回
	return true;
}
#undef SWAP
