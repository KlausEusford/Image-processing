#include "stdafx.h"
#include "GlobalApi.h"
#include "Cdib.h"

#include <io.h>
#include <errno.h>

#include <math.h>
#include <direct.h>


/*************************************************************************
 *
 * 函数名称：
 *   DIBHOLENUMBER()
 *
 * 参数:
 *   CDib * pDib        - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回True，否则返回False。
 *
 * 说明:
 *   该函数将消去图象中面积小于阈值的小区域
 *
 *************************************************************************/

BOOL DIBHOLENUMBER(CDib *pDib)
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

	// 循环变量
	int i, j, s, n;

	// 空穴的数目以及面积阈值
	int nHoleNum, nMinArea;

	int nBlackPix, temp;

	// 正向和反响传播标志
	int nDir1,nDir2;

	// 用来存储的一位数组
	int *pnBinary;

	pnBinary =new int[lHeight*lLineBytes];

	// 小区域的阈值面积为20个象素点
	nMinArea = 50;

	// 将图象二值化
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			// 白色象素为背景，存成0
			if(*lpSrc > 200)
			{
				pnBinary[lLineBytes * j + i] = 0;
			}
			// 黑象素存成-1
			else
			{
				pnBinary[lLineBytes * j + i] = -1;
			}
		}		
	}
	
	// 空穴数赋初值
	nHoleNum = 1;
	
	do
	{
		s=0;
		
		// 寻找每个空穴的初始象素值 
		for (j = 1; j < lHeight - 1; j++)
		{
			for(i = 1; i < lWidth - 1; i++)
			{
				// 找到初始象素
				if(pnBinary[lLineBytes * j + i] == -1)
				{
					s = 1;
					// 将象素值改成当前的空穴数值
					pnBinary[lLineBytes * j + i] = nHoleNum;
					
					// 跳出循环
					j = lHeight;			
					i = lLineBytes;
				}
			}
		}

		//没有初始象素，跳出循环
		if(s == 0)
			break;
		else
		{
			do
			{
				// 正向和反响传播系数赋初值0
				nDir1 = 0;
				nDir2 = 0;

				// 正向扫描
				for (j = 1; j < lHeight-1; j++)
				{
					for(i = 1; i < lWidth-1; i++)
					{
						nBlackPix = pnBinary[lLineBytes * j + i];
						
						// 如果象素已经被扫描，或者是背景色，进行下一个循环
						if(nBlackPix != -1)
							continue;

						// 如果上侧或者左侧的象素值已经被扫描，且属于当前的空穴，当前的象素值
						// 改成空穴的数值
						nBlackPix=pnBinary[lLineBytes * (j-1) + i];
						if(nBlackPix == nHoleNum)
						{
							pnBinary[lLineBytes * j + i] = nHoleNum;
							nDir1 = 1;
							
							continue;
						}

						nBlackPix =pnBinary[lLineBytes * j + i - 1];						
						if(nBlackPix == nHoleNum)
						{
							pnBinary[lLineBytes * j + i] = nHoleNum;
							nDir1 = 1;
						}
					
					}
				}

				// 正向象素全部被扫描，跳出循环
				if(nDir1 == 0)
					break;

				// 反向扫描
				for (j = lHeight-2; j >= 1 ; j--)
				{
					for(i = lWidth-2; i >= 1 ; i--)
					{
						nBlackPix = pnBinary[lLineBytes * j + i];						
						
						// 如果象素已经被扫描，或者是背景色，进行下一个循环
						if(nBlackPix != -1)
							continue;

						// 如果下侧或者右侧的象素值已经被扫描，且属于当前的空穴，当前的象素值
						// 改成空穴的数值
						nBlackPix=pnBinary[lLineBytes * (j+1) + i];
						if(nBlackPix == nHoleNum)
						{
							pnBinary[lLineBytes * j + i] = nHoleNum;
							nDir2 = 1;
							
							continue;
						}

						nBlackPix =pnBinary[lLineBytes * j + i + 1];						
						if(nBlackPix == nHoleNum)
						{
							pnBinary[lLineBytes * j + i] = nHoleNum;
							nDir2 = 1;
						}
					}
				}

				if(nDir2 == 0)
					break;
			}
			while(1);
		}
		// 空穴数增加
		nHoleNum++;
	}
	while(1);

	nHoleNum -- ;

	// 寻找面积小于阈值的空穴区域
	for(n = 1; n <= nHoleNum; n++)
	{
		s = 0;
		
		for (j = 0; j < lHeight - 1; j++)
		{
			for(i = 0; i < lWidth - 1; i++)
			{
				nBlackPix =pnBinary[lLineBytes * j + i];
				
				if(nBlackPix == n)
					s++;

				// 如果区域面积已经大于阈值，跳出循环
				if(s > nMinArea)
					break;
			}

			if(s > nMinArea)
					break;
		}

		// 小于阈值的区域，赋以与背景一样的颜色，进行消去
		if(s <= nMinArea)
		{			
			for (j = 0; j < lHeight - 1; j++)
			{
				for(i = 0; i < lWidth - 1; i++)
				{
					nBlackPix =pnBinary[lLineBytes * j + i + 1];
				
					if(nBlackPix == n)
					{
						pnBinary[lLineBytes * j + i + 1] = 0;
					}			

				}
			}

		}

	}

	// 存储象素值，输出
	for(j = 0; j < lHeight; j++)
	{
		// 列
		for(i = 0; i < lWidth; i++)
		{
			// 二值图象
		    temp = pnBinary[j * lLineBytes + i] ;
			
			// 指向位图i行j列象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * j + i;
			
			// 更新源图像
			if(temp != 0)
				* (lpSrc) = 0;
			else
				* (lpSrc) = 255;
		}
	}

	delete pnBinary;

	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBMOMENT()
 *
 * 参数:
 *   CDib * pDib        - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回True，否则返回False。
 *
 * 说明:
 *   该函数计算图象的力矩。
 *
 *************************************************************************/
BOOL DIBMOMENT(CDib *pDib)
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

	// 图象的矩
	long nImageMoment;

	// 循环变量
	int i,j;

	int ip,jq;

	// 临时变量
	double temp;

	nImageMoment = 0;

	// 计算一阶矩
	ip = jq = 1;

	// 力矩的计算
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			temp = pow((double)i,ip)*pow(double(j),jq);
			temp = temp * (*lpSrc);

			nImageMoment = nImageMoment + (int)temp;
		}
	}
	
	// 将结果进行输出
	CString CView;
	CView.Format("图象的力矩为：%d",nImageMoment);
	MessageBox(NULL,CView, "计算结果" ,	MB_ICONINFORMATION | MB_OK);

	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBBARYCENTERMOMENT()
 *
 * 参数:
 *   CDib * pDib        - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回True，否则返回False。
 *
 * 说明:
 *   该函数计算图象的重心矩。
 *
 *************************************************************************/

BOOL DIBBARYCENTERMOMENT(CDib *pDib)
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

	// 图象象素值
	int  nPixelValue;

	// 图象重心矩
	long nBarycenterMoment;

	// 0次矩m00,x方向的一次矩m01和y方向的一次矩m10
	long m00, m10, m01;

	// 重心x，y坐标值
	int nBarycenterX,nBarycenterY;
	
	// 循环变量
	int i,j;

	// 临时变量
	double temp;

	// 赋初值为零
	m00 = 0;
	m01 = 0;
	m10 = 0;
	nBarycenterMoment = 0;

	// 求0次矩m00,x方向的一次矩m01和y方向的一次矩m10
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
			nPixelValue = *lpSrc;

			m00 = m00 + nPixelValue;

			temp = i * nPixelValue;
			m01  = m01 + temp;

			temp = j * nPixelValue;
			m10  = m10 + temp;
		}
	}

	// 重心x，y坐标值
	nBarycenterX = (int)(m01 / m00 + 0.5);
	nBarycenterY = (int)(m10 / m00 + 0.5);


	// 计算重心矩
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
			nPixelValue = *lpSrc;

			temp = (i - nBarycenterX) * (j - nBarycenterY);

			temp = temp * nPixelValue;

			nBarycenterMoment = nBarycenterMoment + (int)temp;
		}
	}

	// 将结果进行输出
	CString CView;
	CView.Format("图象的重心矩为（%d,%d），重心矩为%d"
					,nBarycenterX,nBarycenterY,nBarycenterMoment);
	MessageBox(NULL,CView, "计算结果" ,	MB_ICONINFORMATION | MB_OK);

	return true;

}

/*************************************************************************
 *
 * 函数名称：
 *   DIBSTREETDIS()
 *
 * 参数:
 *   CDib * pDib        - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回True，否则返回False。
 *
 * 说明:
 *   该函数利用棋盘距离对图象进行变换。
 *
 *************************************************************************/

BOOL DIBSTREETDIS(CDib *pDib)
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

	// 循环和临时变量
	int i,j;
	int temp, s;

	// 存储象素值的数组
	int *pnBinary, *pnStore;
	
	int nImageValue;

	pnBinary = new int[lHeight*lLineBytes];
	pnStore	 = new int[lHeight*lLineBytes];	

	// 将图象二值化
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			// 白色象素为背景，存成0
			if(*lpSrc > 200)
			{
				pnBinary[lLineBytes * j + i] = 0;
				pnStore [lLineBytes * j + i] = 0;
			}
			// 黑象素存成1
			else
			{
				pnBinary[lLineBytes * j + i] = 1;
				pnStore [lLineBytes * j + i] = 1;
			}
		}		
	}

	s = 1;

	while(s == 1)
	{
		s = 0;

		// 进行距离的累加
		for (j = 1; j < lHeight - 1; j++)
		{
			for(i = 1; i < lWidth - 1; i++)
			{
				nImageValue = pnBinary[lLineBytes * j + i];
				
				// 如果是背景，进行下一个循环
				if(nImageValue == 0)
					continue;
				
				// 如果当前象素值和四周的值一样，象素值增加一
				if(nImageValue==pnBinary[lLineBytes * (j-1) + i] && nImageValue==pnBinary[lLineBytes * (j+1) + i])
					if(nImageValue==pnBinary[lLineBytes * j + i-1] && nImageValue==pnBinary[lLineBytes * j + i+1])
					{
						pnStore[lLineBytes * j + i]++;
						s=1;								
					}
			}
		}
		
		// 在进行下一轮循环前将当前的结果储存
		for (j = 0; j < lHeight; j++)
			for(i = 1; i < lWidth; i++)
				pnBinary[lLineBytes * j + i] = pnStore[lLineBytes * j + i];
	}

	// 存储象素值，输出
	for(j = 0; j < lHeight; j++)
	{
		// 列
		for(i = 0; i < lWidth; i++)
		{
			// 骨架的象素值
		    temp = pnStore[j * lLineBytes + i] ;
			
			// 指向位图i行j列象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * j + i;
			
			// 更新源图像
			* (lpSrc) = (BYTE)((25 - temp) * 10 + 5);			
		}
	}

	delete pnStore;
	delete pnBinary;

	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBFREAMEWORK()
 *
 * 参数:
 *   CDib * pDib        - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回True，否则返回False。
 *
 * 说明:
 *   该函数利用棋盘距离，生成图象的骨架。
 *
 *************************************************************************/

BOOL DIBFREAMEWORK(CDib *pDib)
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

	// 循环和临时变量
	int i,j;
	int temp, s;

	// 存储象素值的数组
	int *pnBinary, *pnStore;
	
	int nImageValue;

	pnBinary = new int[lHeight*lLineBytes];
	pnStore	 = new int[lHeight*lLineBytes];	

	// 将图象二值化
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			// 白色象素为背景，存成0
			if(*lpSrc > 200)
			{
				pnBinary[lLineBytes * j + i] = 0;
				pnStore [lLineBytes * j + i] = 0;
			}
			// 黑象素存成1
			else
			{
				pnBinary[lLineBytes * j + i] = 1;
				pnStore [lLineBytes * j + i] = 1;
			}
		}		
	}

	s = 1;

	while(s == 1)
	{
		s = 0;

		// 进行距离的累加
		for (j = 1; j < lHeight - 1; j++)
		{
			for(i = 1; i < lWidth - 1; i++)
			{
				nImageValue = pnBinary[lLineBytes * j + i];
				
				// 如果是背景，进行下一个循环
				if(nImageValue == 0)
					continue;
				
				// 如果当前象素值和四周的值一样，象素值增加一
				if(nImageValue==pnBinary[lLineBytes * (j-1) + i] && nImageValue==pnBinary[lLineBytes * (j+1) + i])
					if(nImageValue==pnBinary[lLineBytes * j + i-1] && nImageValue==pnBinary[lLineBytes * j + i+1])
					{
						pnStore[lLineBytes * j + i]++;
						s=1;								
					}
			}
		}
		
		// 在进行下一轮循环前将当前的结果储存
		for (j = 0; j < lHeight; j++)
			for(i = 1; i < lWidth; i++)
				pnBinary[lLineBytes * j + i] = pnStore[lLineBytes * j + i];
	}
	
	for (j = 0; j < lHeight; j++)
		for(i = 0; i < lWidth; i++)
			pnStore[lLineBytes * j + i] = 0;
	
	// 如果当前的象素值比周围的都要高，做为骨架	
	for (j = 1; j < lHeight - 1; j++)
		for(i = 1; i < lWidth - 1; i++)
		{
			nImageValue = pnBinary[lLineBytes * j + i] + 1;
			
			// 在四连通域进行比较
			if(nImageValue!=pnBinary[lLineBytes * (j-1) + i] && nImageValue!=pnBinary[lLineBytes * (j+1) + i])
					if(nImageValue!=pnBinary[lLineBytes * j + i-1] && nImageValue!=pnBinary[lLineBytes * j + i+1])
						pnStore[lLineBytes * j + i] = pnBinary[lLineBytes * j + i];
		}


	// 存储象素值，输出
	for(j = 0; j < lHeight; j++)
	{
		// 列
		for(i = 0; i < lWidth; i++)
		{
			// 骨架的象素值
		    temp = pnStore[j * lLineBytes + i] ;
			
			// 指向位图i行j列象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * j + i;
			
			// 更新源图像，并将象素值进行变换，以便输出
			if(temp != 0)	
				* (lpSrc) = temp;
			else
				* (lpSrc) = 255;
		}
	}

	delete pnStore;
	delete pnBinary;

	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBCHESSBOARDDISRESTORE()
 *
 * 参数:
 *   CDib * pDib        - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回True，否则返回False。
 *
 * 说明:
 *   该函数利用骨架对原图象进行恢复。
 *
 *************************************************************************/
BOOL DIBCHESSBOARDDISRESTORE(CDib *pDib)
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

	// 循环变量
	int i, j, n, m;

	// 临时变量
	int temp, s;

	// 用来存放象素值的数组
	int *pnBinary, *pnStore;
	
	int nImageValue;

	pnBinary = new int[lHeight*lLineBytes];
	pnStore	 = new int[lHeight*lLineBytes];	

	// 数组赋值
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			pnBinary[lLineBytes * j + i] = 0;
			if((*lpSrc) != 255)
				pnStore [lLineBytes * j + i] = (*lpSrc);
			else
				pnStore [lLineBytes * j + i] = 0;
		}		
	}

	
	// 进行图象的恢复
	for (j = 1; j < lHeight - 1; j++)
	{
		for(i = 1; i < lWidth - 1; i++)
		{
			nImageValue = pnStore[lLineBytes * j + i];
			if(nImageValue == 0)
				continue;
			
			if(nImageValue == 1)
			{
				pnBinary[lLineBytes * j + i] = 1;
				continue;
			}
			
			// 如果象素值大于等于2，将以（2×nImageValue）＋1的菱形范围的象素值赋1
			s = nImageValue;

			// 菱形的主轴
			for(m = -s; m <= s; m++)
			{
				pnBinary[lLineBytes * j + i + m] = 1;

			}

			// 菱形的上半部分
			for(n = -s; n < 0; n++)
				for(m = -s - n; m <=s + n; m++)
					pnBinary[lLineBytes * (j+n) + i+m] = 1;

			// 菱形的下半部分
			for(n = 1; n <= s; n++)
				for(m = -s + n; m <= s - n; m++)
					pnBinary[lLineBytes * (j+n) + i+m] = 1;
		
		}
	}

	// 存储象素值，输出
	for(j = 0; j < lHeight; j++)
	{
		// 列
		for(i = 0; i < lWidth; i++)
		{
			// 骨架的象素值
		    temp = pnBinary[j * lLineBytes + i] ;
			
			// 指向位图i行j列象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * j + i;
			
			// 更新源图像
			if(temp != 0)	
				// 黑色象素输出
				* (lpSrc) = 0;
			else
				// 白色象素输出
				* (lpSrc) = 255;
		}
	}

	delete pnStore;
	delete pnBinary;

	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   DIBOUTLINE()
 *
 * 参数:
 *   CDib * pDib        - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回True，否则返回False。
 *
 * 说明:
 *   该函数对二值图象进行轮廓检出。
 *
 *************************************************************************
 */

BOOL DIBOUTLINE(CDib *pDib)
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

	// 循环变量
	int i, j;

	int nPixelValue;

	int n1,n2,n3,n4,n5,n6,n7,n8;

	// 用来存放象素值的数组
	int *pnBinary;
	
	pnBinary = new int[lHeight*lLineBytes];

	// 将图象二值化
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			// 白色象素为背景，存成0
			if(*lpSrc > 200)
			{
				pnBinary[lLineBytes * j + i] = 0;
			}
			// 黑象素存成1
			else
			{
				pnBinary[lLineBytes * j + i] = 1;
			}
		}		
	}
	
	for (j = 1; j < lHeight - 1; j++)
	{
		for(i = 1; i < lWidth - 1; i++)
		{	
			nPixelValue = pnBinary[lLineBytes * j + i];
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			// 如果当前象素是白色，进入下一个循环
			if(nPixelValue == 0)
			{
				// 将相应的象素值改成零
			
				*lpSrc = 255;
				continue;
			}

			// 如果当前象素是黑色
			else
			{
				// 检查周边的8连通域
				n1 = pnBinary[lLineBytes * (j+1) + i + 1];
				n2 = pnBinary[lLineBytes * (j+1) + i - 1];
				n3 = pnBinary[lLineBytes * (j+1) + i];
				n4 = pnBinary[lLineBytes * (j-1)+ i + 1];
				n5 = pnBinary[lLineBytes * (j-1) + i - 1];
				n6 = pnBinary[lLineBytes * (j-1) + i];
				n7 = pnBinary[lLineBytes * j + i + 1];
				n8 = pnBinary[lLineBytes * j + i - 1];
				
				//如果相邻的八个点都是黑点
				if(n1&&n2&&n3&&n4&&n5&&n6&&n7&&n8 == 1)
				{
					*lpSrc = (unsigned char)255;
				}
			}
		}
	}

	delete pnBinary;

	// 返回
	return TRUE;
}


/*************************************************************************
 *
 * 函数名称：
 *   DIBTrace()
 *
 * 参数:
 *   CDib * pDib        - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回True，否则返回False。
 *
 * 说明:
 * 该函数用于对图像进行轮廓跟踪运算。
 *
 *************************************************************************
 */

BOOL WINAPI DIBTrace(CDib *pDib)
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

	//像素值
	int nPixelValue;

	//是否找到起始点及回到起始点
	bool bFindStartPoint;

	//是否扫描到一个边界点
	bool bFindPoint;

	// 用来边界起点和所有边界点的数组
	int nStartPointX, nStartPointY;
	int *nCurrPointX, *nCurrPointY;
	
	// 边界点的数目
	long lNum;

	// 用来存储二值象素的数组
	int *pnBinary;	

	//八个方向和起始扫描方向
	int Direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};
	int BeginDirect;

	// 分配内存
	pnBinary =new int[lLineBytes*lHeight];
	nCurrPointX = new int[lLineBytes*lHeight/4];
	nCurrPointY = new int[lLineBytes*lHeight/4];	

	// 将图象二值化
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			// 白色象素为背景，存成0
			if(*lpSrc > 200)
			{
				pnBinary[lLineBytes * j + i] = 0;
				
			}
			// 黑象素存成1
			else
			{
				pnBinary[lLineBytes * j + i] = 1;
			}

			// 将已经扫描过的象素存成白色背景
			*lpSrc = 255;
		}		
	}	

	//先找到最左上方的边界点
	bFindStartPoint = false;

	for (j = 0; j < lHeight; j++)
	{
		for(i = 0;i < lWidth ;i++)
		{
			nPixelValue = pnBinary[lLineBytes * j + i];
			
			if(nPixelValue == 1)
			{
				bFindStartPoint = true;

				// 储存边界起始点
				nStartPointY = j;
				nStartPointX  = i;
				
				// 跳出循环
				j = lHeight;
				i = lLineBytes ;
			}		
		}
	}

	// 没有起始点，跳出
	if(bFindStartPoint == false)
		return false;

	//由于起始点是在左上方，起始扫描沿右侧方向
	BeginDirect = 0;
	//跟踪边界
	bFindStartPoint = false;

	//从初始点开始扫描
	lNum = 0;
	nCurrPointY[0] = nStartPointY;
	nCurrPointX[0] = nStartPointX;
	
	while(!bFindStartPoint)
	{
		bFindPoint = false;

		while(!bFindPoint)
		{
			nPixelValue = pnBinary[(nCurrPointY[lNum] + Direction[BeginDirect][1])*lLineBytes
									+ nCurrPointX[lNum] + Direction[BeginDirect][0]];

			// 找到下一个边界点
			if(nPixelValue == 1)
			{
				bFindPoint = true;

				// 位置存储
				nCurrPointY[lNum+1] = nCurrPointY[lNum] + Direction[BeginDirect][1];
				nCurrPointX[lNum+1] = nCurrPointX[lNum] + Direction[BeginDirect][0];
				lNum ++;

				if(nCurrPointY[lNum] == nStartPointY && nCurrPointX[lNum] == nStartPointX)
				{
					bFindStartPoint = true;
				}

				//扫描的方向逆时针旋转两格
				BeginDirect--;
				if(BeginDirect == -1)
					BeginDirect = 7;
				BeginDirect--;
				if(BeginDirect == -1)
					BeginDirect = 7;
			}
			else
			{
				//扫描方向顺时针旋转一格
				BeginDirect++;
				if(BeginDirect == 8)
					BeginDirect = 0;

			}

		}
	}

	// 存储显示边界点
	for(i = 0; i <= lNum; i++)
	{
		lpSrc  = (unsigned char *)lpDIBBits + nCurrPointY[i] *lLineBytes  + nCurrPointX[i];
		*lpSrc = 0;
	}

	delete pnBinary;

	// 返回
	return TRUE;
}
