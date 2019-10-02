
#include "stdafx.h"
#include "cdib.h"
#include "math.h"
#include <direct.h>
#include <complex>
using namespace std;
#include "GlobalApi.h"

/*************************************************************************
 *
 * \�������ƣ�
 *   HistogramEqualize()
 *
 * \�������:
 *   CDib * pDib		- ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
 *
 * \����ֵ:
 *   BOOL			- �ɹ��򷵻�TRUE�����򷵻�FALSE
 *
 * \˵��:
 *   �ú�����ָ����ͼ�����ֱ��ͼ���⻯����
 *
 *************************************************************************
 */
 BOOL HistogramEqualize(CDib* pDib)
 {
	 // ָ��Դͼ���ָ��
	unsigned char*	lpSrc;
	
	// ��ʱ����
	int	nTemp;
	
	// ѭ������
	int i,j;
	
	// �ۻ�ֱ��ͼ�����Ҷ�ӳ���
	BYTE	byMap[256];
	
	// ֱ��ͼ
	int	nCount[256];
	
	// ͼ��ĸ߶ȺͿ���
	CSize sizeImage;
	sizeImage = pDib->GetDimensions();


	// ���ͼ�����ݴ洢�ĸ߶ȺͿ���
	CSize SizeSaveImage;
	SizeSaveImage = pDib->GetDibSaveDim();



	// ���ü���Ϊ0
	for (i = 0; i < 256; i ++)
	{
		// ����
		nCount[i] = 0;
	}
	
	// ��������Ҷ�ֵ�ļ��������õ�ֱ��ͼ
	for (i = 0; i < sizeImage.cy; i ++)
	{
		for (j = 0; j < sizeImage.cx; j ++)
		{
			lpSrc = (unsigned char *)pDib->m_lpImage + SizeSaveImage.cx * i + j;
			
			// ������1
			nCount[*(lpSrc)]++;
		}
	}
	
	// �����ۻ�ֱ��ͼ
	for (i = 0; i < 256; i++)
	{
		// ��ʼΪ0
		nTemp = 0;
		
		for (j = 0; j <= i ; j++)
		{
			nTemp += nCount[j];
		}
		
		// �����Ӧ���»Ҷ�ֵ
		byMap[i] = (BYTE) (nTemp * 255 / sizeImage.cy / sizeImage.cx);
	}
	
	// ÿ��
	for(i = 0; i < sizeImage.cy; i++)
	{
		// ÿ��
		for(j = 0; j < sizeImage.cx; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)pDib->m_lpImage + pDib->GetPixelOffset(i,j);
			
			// �����µĻҶ�ֵ
			*lpSrc = byMap[*lpSrc];
		}
	}
	
	// ����
	return TRUE;
 }

/*************************************************************************
 *
 * \�������ƣ�
 *   GraySegLinTrans()
 *
 * \�������:
 *   CDib* pDib			- ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
 *   int   nX1			- �ֶ����ԻҶȱ任��һ���յ��X����
 *   int   nY1			- �ֶ����ԻҶȱ任��һ���յ��Y����
 *   int   nX2			- �ֶ����ԻҶȱ任�ڶ����յ��X����
 *   int   nY2			- �ֶ����ԻҶȱ任�ڶ����յ��Y����
 *
 * \����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * \˵��:
 *   �ú���������ͼ����зֶ����ԻҶȱ任����������а����������յ������
 *
 *************************************************************************
 */
BOOL GraySegLinTrans(CDib* pDib, int nX1, int nY1, int nX2, int nY2)
{
	
	// ָ��Դͼ���ָ��
	unsigned char*	lpSrc;
	
	// ѭ������
	int i,j;
	
	// �Ҷ�ӳ���
	BYTE	byMap[256];
	
	// ͼ��ÿ�е��ֽ���
	//LONG	lLineBytes;

	// ͼ��ĸ߶ȺͿ���
	CSize sizeImage;
	sizeImage = pDib->GetDimensions();


	// ���ͼ�����ݴ洢�ĸ߶ȺͿ���
	CSize SizeSaveImage;
	SizeSaveImage = pDib->GetDibSaveDim();

	// ����ͼ��ÿ�е��ֽ���
	//lLineBytes = WIDTHBYTES(sizeImage.cx * 8);
	
	// ����Ҷ�ӳ���
	for (i = 0; i <= nX1; i++)
	{
		// �ж�nX1�Ƿ����0����ֹ��ĸΪ0��
		if (nX1 > 0)
		{
			// ���Ա任
			byMap[i] = (BYTE) nY1 * i / nX1;
		}
		else
		{
			// ֱ�Ӹ�ֵΪ0
			byMap[i] = 0;
		}
	}
	for (; i <= nX2; i++)
	{
		// �ж�nX1�Ƿ����nX2����ֹ��ĸΪ0��
		if (nX2 != nX1)
		{
			// ���Ա任
			byMap[i] = nY1 + (BYTE) ((nY2 - nY1) * (i - nX1) / (nX2 - nX1));
		}
		else
		{
			// ֱ�Ӹ�ֵΪnY1
			byMap[i] = nY1;
		}
	}
	for (; i < 256; i++)
	{
		// �ж�nX2�Ƿ����255����ֹ��ĸΪ0��
		if (nX2 != 255)
		{
			// ���Ա任
			byMap[i] = nY2 + (BYTE) ((255 - nY2) * (i - nX2) / (255 - nX2));
		}
		else
		{
			// ֱ�Ӹ�ֵΪ255
			byMap[i] = 255;
		}
	}
	
	// ��ͼ�������ֵ���б任
	// ÿ��
	for(i = 0; i < sizeImage.cy; i++)
	{
		// ÿ��
		for(j = 0; j < sizeImage.cx; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)pDib->m_lpImage + pDib->GetPixelOffset(i,j);
			
			// �����µĻҶ�ֵ
			*lpSrc = byMap[*lpSrc];
		}
	}
	
	// ����
	return TRUE;
}

/*************************************************************************
 *
 * \�������ƣ�
 *   GeneralTemplate()
 *
 * \�������:
 *   CDib * pDib		- ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
 *   int nTempWidth		- ģ��Ŀ���
 *   int nTempHeight		- ģ��ĸ߶�
 *   int nTempCenX		- ģ�����ĵ�X���꣨�����ģ�壩
 *   int nTempCenY		- ģ�����ĵ�Y���꣨�����ģ�壩
 *   double* pdbTemp		- ģ�������ָ��
 *   double* dbCoef		- ģ���ϵ��
 *
 * \����ֵ:
 *   BOOL			- �ɹ��򷵻�TRUE�����򷵻�FALSE
 *
 * \˵��:
 *   �ú�����ָ����ģ���pDibָ���ͼ�����ģ�������ģ��Ķ����˿��ȣ��߶ȣ�
 *    ���������ϵ����ģ������ݴ����pdbTemp�С���ͼ�����ģ���������
 *    Ȼ�����pDibָ���CDib�����С���Ҫע����ǣ��ú���ֻ�ܴ���8λ��ͼ��
 *   ����ָ������ݽ�������
 *
 *************************************************************************
 */
BOOL  GeneralTemplate(CDib* pDib,  int nTempWidth, int nTempHeight, 
					 int nTempCenX, int nTempCenY,
					 double* pdbTemp, double dbCoef)
{
	// ��ʱ���ͼ�����ݵ�ָ��
	LPBYTE lpImage;
	
	// ѭ������
	int i,j,k,l;
	
	// ָ��Դͼ���ָ��
	unsigned char*	lpSrc;
	
	// ָ��Ҫ���������ָ��
	unsigned char*	lpDst;

	// ������
	double	dbResult;

	// ͼ��ĸ߶ȺͿ���
	CSize sizeImage;
	sizeImage = pDib->GetDimensions();


	// ���ͼ�����ݴ洢�ĳߴ�
	int nSizeImage;
	nSizeImage = pDib->GetSizeImage();

	// ����ʱ������ݷ����ڴ�
	lpImage = (LPBYTE) new char[nSizeImage];

	// �ж��Ƿ��ڴ����ʧ��
	if (lpImage == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// ��ԭʼͼ������ݿ�������ʱ����ڴ���
	memcpy(lpImage, pDib->m_lpImage, nSizeImage);

	
	
	// ����ģ�����
	// ��(��ȥ��Ե����)
	for(i = nTempCenY ; i < sizeImage.cy - nTempHeight + nTempCenY + 1; i++)
	{
		// ��(��ȥ��Ե����)
		for(j = nTempCenX; j < sizeImage.cx - nTempWidth + nTempCenX + 1; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDst = (unsigned char*)lpImage + pDib->GetPixelOffset(i,j);
			
			dbResult = 0;
			
			// ����
			for (k = 0; k < nTempHeight; k++)
			{
				for (l = 0; l < nTempWidth; l++)
				{
					// ָ��DIB��i - nTempCenY + k�У���j - nTempCenX + l�����ص�ָ��
					lpSrc = (unsigned char*)pDib->m_lpImage + pDib->GetPixelOffset(i-nTempCenY+k, j-nTempCenX+l);
					
					// ��������ֵ
					dbResult += (* lpSrc) * pdbTemp[k * nTempWidth + l];
				}
			}
			
			// ����ϵ��
			dbResult *= dbCoef;
			
			// ȡ����ֵ
			dbResult = (double ) fabs(dbResult);
			
			// �ж��Ƿ񳬹�255
			if(dbResult > 255)
			{
				// ֱ�Ӹ�ֵΪ255
				* lpDst = 255;
			}
			else
			{
				// ��ֵ
				* lpDst = (unsigned char) (dbResult + 0.5);
			}
			
		}
	}
	
	// ���Ʊ任���ͼ��
	memcpy(pDib->m_lpImage, lpImage, nSizeImage);
	
	// �ͷ��ڴ�
	delete[]lpImage;
	
	// ����
	return TRUE;

}


/*************************************************************************
 *
 * �������ƣ�
 *   MedianFilter()
 *
 * \�������:
 *   CDib * pDib		- ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
 *   int nTempWidth		- ģ��Ŀ���
 *   int nTempHeight		- ģ��ĸ߶�
 *   int nTempCenX		- ģ�����ĵ�X���꣨�����ģ�壩
 *   int nTempCenY		- ģ�����ĵ�Y���꣨�����ģ�壩
 *
 * \����ֵ:
 *   BOOL			- �ɹ��򷵻�TRUE�����򷵻�FALSE
 *
 * ˵��:
 *   �ú�����ָ����DIBͼ�������ֵ�˲���
 *
 ************************************************************************/

BOOL MedianFilter(CDib* pDib,  int nTempWidth, int nTempHeight, 
					 int nTempCenX, int nTempCenY)
{	
	// ��ʱ���ͼ�����ݵ�ָ��
	LPBYTE lpImage;
	
	// ѭ������
	int i,j,k,l;
	
	// ָ��Դͼ���ָ��
	unsigned char*	lpSrc;
	
	// ָ��Ҫ���������ָ��
	unsigned char*	lpDst;

	// ͼ��ĸ߶ȺͿ���
	CSize sizeImage;
	sizeImage = pDib->GetDimensions();

	// ���ͼ�����ݴ洢�ĳߴ�
	int nSizeImage;
	nSizeImage = pDib->GetSizeImage();

	// ָ���˲��������ָ��
	unsigned char* pUnchFltValue;

	// ����ʱ������ݷ����ڴ�
	lpImage = (LPBYTE) new char[nSizeImage];
	// �ж��Ƿ��ڴ����ʧ��
	if (lpImage == NULL)
	{
		// ����
		return FALSE;
	}
	
	// ��ԭʼͼ������ݿ�������ʱ����ڴ���
	memcpy(lpImage, pDib->m_lpImage, nSizeImage);
	
	// ��ʱ�����ڴ棬�Ա����˲�������
	pUnchFltValue = new unsigned char[nTempHeight * nTempWidth];
	
	// �ж��Ƿ��ڴ����ʧ��
	if (pUnchFltValue == NULL)
	{
		// �ͷ��ѷ����ڴ�
		delete[]lpImage;
		
		// ����
		return FALSE;
	}
	
	// ��ʼ��ֵ�˲�
	// ��(��ȥ��Ե����)
	for(i = nTempCenY; i < sizeImage.cy - nTempHeight + nTempCenY + 1; i++)
	{
		// ��(��ȥ��Ե����)
		for(j = nTempCenX; j < sizeImage.cx - nTempWidth + nTempCenX + 1; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDst = (unsigned char*)lpImage + pDib->GetPixelOffset(i,j);
			//lpDst = (unsigned char*)lpImage + sizeImage.cx * (sizeImage.cy - 1 - i) + j;
			
			// ��ȡ�˲�������
			for (k = 0; k < nTempHeight; k++)
			{
				for (l = 0; l < nTempWidth; l++)
				{
					// ָ��DIB��i - nTempCenY + k�У���j - nTempCenX + l�����ص�ָ��
					lpSrc = (unsigned char*)pDib->m_lpImage + pDib->GetPixelOffset(i-nTempCenY+k, j-nTempCenX+l);
					//lpSrc = (unsigned char*)pDib->m_lpImage + sizeImage.cx * (sizeImage.cy - 1 - i + nTempCenY - k) + j - nTempCenX + l;
					
					// ��������ֵ
					pUnchFltValue[k * nTempWidth + l] = *lpSrc;
				}
			}
			
			// ��ȡ��ֵ
			//* lpDst = GetMedianValue(pUnchFltValue, nTempHeight * nTempWidth);
		}
	}
	
	// ���Ʊ任���ͼ��
	memcpy(pDib->m_lpImage, lpImage, nSizeImage);
	
	// �ͷ��ڴ�
	delete[]lpImage;
	delete[]pUnchFltValue;

	// ����
	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   GetMedianValue()
 *
 * ����:
 *   unsigned char * pUnchFltValue	- ָ��Ҫ��ȡ��ֵ������ָ��
 *   int   iFilterLen			- ���鳤��
 *
 * ����ֵ:
 *   unsigned char			- ����ָ���������ֵ��
 *
 * ˵��:
 *   �ú�����ð�ݷ���һά����������򣬲���������Ԫ�ص���ֵ��
 *
 ************************************************************************/

unsigned char  GetMedianValue(unsigned char * pUnchFltValue, int iFilterLen)
{
	// ѭ������
	int		i;
	int		j;
	
	// �м����
	unsigned char bTemp;
	
	// ��ð�ݷ��������������
	for (j = 0; j < iFilterLen - 1; j ++)
	{
		for (i = 0; i < iFilterLen - j - 1; i ++)
		{
			if (pUnchFltValue[i] > pUnchFltValue[i + 1])
			{
				// ����
				bTemp = pUnchFltValue[i];
				pUnchFltValue[i] = pUnchFltValue[i + 1];
				pUnchFltValue[i + 1] = bTemp;
			}
		}
	}
	
	// ������ֵ
	if ((iFilterLen & 1) > 0)
	{
		// ������������Ԫ�أ������м�һ��Ԫ��
		bTemp = pUnchFltValue[(iFilterLen + 1) / 2];
	}
	else
	{
		// ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
		bTemp = (pUnchFltValue[iFilterLen / 2] + pUnchFltValue[iFilterLen / 2 + 1]) / 2;
	}
	
	// ������ֵ
	return bTemp;
}

/*************************************************************************
 *
 * \�������ƣ�
 *   LinearSharpen()
 *
 * \�������:
 *   LPBYTE lpImage		- ָ��ͼ�����ݵ�ָ��
 *   int nWidth			- ͼ�����ݿ���
 *   int nHeight		- ͼ�����ݸ߶�
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ������ͼ����ǿ
 *   ����������������˹���Ӷ�ͼ�����������
 *   ��ԭ��ͼ���ϼ���������˹�����񻯵���Ϣ
 *
 *************************************************************************
 */
void LinearSharpen (LPBYTE lpImage, int nWidth, int nHeight)
{
	// ����ͼ���������
	int y;

	// ����ͼ��ĺ�����
	int x;

	double * pdGrad ;
	pdGrad = new double[nWidth*nHeight];

	// ��ʼ��Ϊ0
	memset(pdGrad, 0, nWidth*nHeight*sizeof(double)) ;

	// ����ģ��ϵ��
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

	//�������������ʾLaplacian��������ֵ
	int nTmp[3][3];

	// ��ʱ����
	double dGrad;

	// ģ��ѭ�����Ʊ���
	int yy ;
	int xx ;
	for(y=1; y<nHeight-1 ; y++ )
	 for(x=1 ; x<nWidth-1 ; x++ )
	 {
		 dGrad = 0 ; 
		 // Laplacian������Ҫ�ĸ�������ֵ
		 
		 // ģ���һ��
		 nTmp[0][0] = lpImage[(y-1)*nWidth + x - 1 ] ; 
		 nTmp[0][1] = lpImage[(y-1)*nWidth + x     ] ; 
		 nTmp[0][2] = lpImage[(y-1)*nWidth + x + 1 ] ; 
		 
		 // ģ��ڶ���
		 nTmp[1][0] = lpImage[y*nWidth + x - 1 ] ; 
		 nTmp[1][1] = lpImage[y*nWidth + x     ] ; 
		 nTmp[1][2] = lpImage[y*nWidth + x + 1 ] ; 
		 
		 // ģ�������
		 nTmp[2][0] = lpImage[(y+1)*nWidth + x - 1 ] ; 
		 nTmp[2][1] = lpImage[(y+1)*nWidth + x     ] ; 
		 nTmp[2][2] = lpImage[(y+1)*nWidth + x + 1 ] ; 
		 
		 // �����ݶ�
		 for(yy=0; yy<3; yy++)
			 for(xx=0; xx<3; xx++)
			 {
				 dGrad += nTmp[yy][xx] * nWeight[yy][xx] ;
			 }
			 
			 // �ݶ�ֵд���ڴ�
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
 * �������ƣ�
 *   ReplaceDIBColorTable()
 *
 * ����:
 *   CDib * pDib		- ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
 *   LPBYTE pColorsTable	- α��ɫ�����
 *
 * ����ֵ:
 *   BOOL			- �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�����ָ����α��ɫ��������滻ָ����DIBͼ�����ɫ��������pColorsTable
 *   ָ��Ҫ�滻��α��ɫ�������
 *
 ************************************************************************/
BOOL  ReplaceDIBColorTable(CDib* pDib, LPBYTE  pColorsTable)
{
	
	// ѭ������
	int i;

	// ��ɫ���ı�����
	int nColorEntries;

	// ��ʱ����
	LPRGBQUAD pDibQuad;

	// ָ��DIB����ɫ��
	pDibQuad = (LPRGBQUAD) pDib->m_lpvColorTable;

	// ��ȡDIB����ɫ���еı�����
	nColorEntries = pDib->m_nColorTableEntries;
	
	// �ж���ɫ��Ŀ�Ƿ���256ɫ
	if (nColorEntries == 256)
	{			
		// ��ȡα��ɫ���룬����DIB��ɫ��
		for (i = 0; i < nColorEntries; i++)
		{		
			// ����DIB��ɫ���ɫ����
			pDibQuad->rgbRed = pColorsTable[i * 4];
				
			// ����DIB��ɫ����ɫ����
			pDibQuad->rgbGreen = pColorsTable[i * 4 + 1];
				
			// ����DIB��ɫ����ɫ����
			pDibQuad->rgbBlue = pColorsTable[i * 4 + 2];
				
			// ����DIB��ɫ�屣��λ
			pDibQuad->rgbReserved = 0;

			pDibQuad++;
		
		}
	}

	// �������256ɫ��DIB���򲻽��д���
	else
		return FALSE;
	
	// ����
	return TRUE;

}


/*************************************************************************
 *
 * \�������ƣ�
 *   LowPassFilterEnhance()
 *
 * \�������:
 *   LPBYTE lpImage					- ָ����Ҫ��ǿ��ͼ������
 *   int nWidth							- ���ݿ���
 *   int nHeight						- ���ݸ߶�
 *   int nRadius            - ��ͨ�˲����˲��뾶
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   lpImage ��ָ����Ҫ��ǿ������ָ�롣ע�⣬���ָ��ָ���������������CDibָ���������
 *	 ��ΪCDibָ�����������ÿһ����DWORD����ġ�
 *   ������ͨ�˲������ݴ洢��lpImage���С�
 *
 *************************************************************************
 */
void LowPassFilterEnhance(LPBYTE lpImage, int nWidth, int nHeight, int nRadius)
{
	// ѭ�����Ʊ���
	int y ;
	int x ;

	double dTmpOne ;
	double dTmpTwo ;

	// ����Ҷ�任�Ŀ��Ⱥ͸߶ȣ�2���������ݣ�
	int nTransWidth ;
	int nTransHeight;

	// ͼ������ֵ
	unsigned char unchValue;

	// ָ��ʱ�����ݵ�ָ��
	complex<double> * pCTData ;
	// ָ��Ƶ�����ݵ�ָ��
	complex<double> * pCFData ;
	
	// ������и���Ҷ�任�ĵ���	��2���������ݣ�
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// ������и���Ҷ�任�ĵ��� ��2���������ݣ�
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;

	// ����Ҷ�任��ʵ�����鲿
	double dReal;
	double dImag;

	// ��ͨ�˲��İ뾶���ܳ���Ƶ������뾶
	if(nRadius>nTransWidth-1 || nRadius>nTransHeight-1)
	{
		return ;
	}
	
	// �����ڴ�
	pCTData=new complex<double>[nTransWidth * nTransHeight];
	pCFData=new complex<double>[nTransWidth * nTransHeight];

	// ��ʼ��
	// ͼ�����ݵĿ��͸߲�һ����2���������ݣ�����pCTData
	// ��һ����������Ҫ��0
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x]=complex<double>(0,0);
		}
	}

	// ��ͼ�����ݴ���pCTData
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nWidth +x];
			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
		}
	}
	
	// ����Ҷ���任
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData) ;

	// ���濪ʼʵʩ��ͨ�˲��������д���nRadius�ĸ�Ƶ��������Ϊ0
	//  ע�������Ƶ�������õķ�������ŷʽ���룬�����������
	//  || (u,v)-(0,0) || = max(|u|,|v|) 
	for(y=nRadius; y<nTransHeight; y++)
	{
		for(x=nRadius; x<nTransWidth; x++)
		{
			pCFData[y*nTransWidth + x]=complex<double>(0,0);
		}
	}
	
	// ������ͨ�˲���ͼ����з��任
	IFFT_2D(pCFData, pCTData, nWidth, nHeight);

	// ���任�����ݴ���lpImage
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

	
	// �ͷ��ڴ�
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;
	
}

/*************************************************************************
 *
 * \�������ƣ�
 *   HighPassFilterEnhance()
 *
 * \�������:
 *   LPBYTE lpImage					- ָ����Ҫ��ǿ��ͼ������
 *   int nWidth							- ���ݿ���
 *   int nHeight						- ���ݸ߶�
 *   int nRadius            - ��ͨ�˲����˲��뾶
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   lpImage ��ָ����Ҫ��ǿ������ָ�롣ע�⣬���ָ��ָ���������������CDibָ���������
 *	 ��ΪCDibָ�����������ÿһ����DWORD����ġ�
 *   ������ͨ�˲������ݴ洢��lpImage���С�
 *
 *************************************************************************
 */
void HighPassFilterEnhance(LPBYTE lpImage, int nWidth, int nHeight, int nRadius)
{
	// ѭ�����Ʊ���
	int y ;
	int x ;

	double dTmpOne ;
	double dTmpTwo ;

	// ����Ҷ�任�Ŀ��Ⱥ͸߶ȣ�2���������ݣ�
	int nTransWidth ;
	int nTransHeight;

	// ͼ������ֵ
	unsigned char unchValue;

	// ָ��ʱ�����ݵ�ָ��
	complex<double> * pCTData ;
	// ָ��Ƶ�����ݵ�ָ��
	complex<double> * pCFData ;

	double dReal;
	double dImag;
	
	// ������и���Ҷ�任�ĵ���	��2���������ݣ�
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// ������и���Ҷ�任�ĵ��� ��2���������ݣ�
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;

	// �˲��İ뾶���ܳ���Ƶ������뾶
	if(nRadius>nTransWidth-1 || nRadius>nTransHeight-1)
	{
		return ;
	}
	
	// �����ڴ�
	pCTData=new complex<double>[nTransWidth * nTransHeight];
	pCFData=new complex<double>[nTransWidth * nTransHeight];

	// ��ʼ��
	// ͼ�����ݵĿ��͸߲�һ����2���������ݣ�����pCTData
	// ��һ����������Ҫ��0
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x]=complex<double>(0,0);
		}
	}

	// ��ͼ�����ݴ���pCTData
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nWidth +x];
			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
		}
	}
	
	// ����Ҷ���任
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData) ;

	// ���濪ʼ�˲���������С��nRadius�ĵ�Ƶ��������Ϊ0
	// ���õķ�������ŷʽ���룬�����������
	//  || (u,v)-(0,0) || = max(|u|,|v|) 
	for(y=0; y<nRadius; y++)
	{
		for(x=0; x<nRadius; x++)
		{
			pCFData[y*nTransWidth + x]=complex<double>(0,0);
		}
	}
	// �����˲���ͼ����з��任
	IFFT_2D(pCFData, pCTData, nWidth, nHeight);

	// ���任�����ݴ���lpImage
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
		  // ��Ϊ��ͨ�˲�����ѵ�Ƶ�ɷ�ȥ��������ͼ��������Ƚϰ���Ϊ�˽���������
			// �ھ�����ǿ���ͼ�����һ��������ʹͼ�������������ϸ��������ִ����൱��
			// ������һЩ��Ƶ��������Ƶ��(0,0)���������������һ����������ͨЧ���ڹۿ�
			// �ϾͲ��Ǻܷ��㡣
			dReal = pCTData[y*nTransWidth + x].real() ;
			dImag = pCTData[y*nTransWidth + x].imag() ;
			unchValue = (unsigned char)max(0,min(255,(sqrt(dReal*dReal+dImag*dImag)+100 )));
			lpImage[y*nWidth + x] = unchValue ;
		}
	}

	
	// �ͷ��ڴ�
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;
	
}

/*************************************************************************
 *
 * \�������ƣ�
 *   ButterWorthLowPass()
 *
 * \�������:
 *   LPBYTE lpImage					- ָ����Ҫ��ǿ��ͼ������
 *   int nWidth							- ���ݿ���
 *   int nHeight						- ���ݸ߶�
 *   int nRadius            - ButterWorth��ͨ�˲��ġ��빦�ʡ���
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   lpImage ��ָ����Ҫ��ǿ������ָ�롣ע�⣬���ָ��ָ���������������CDibָ���������
 *	 ��ΪCDibָ�����������ÿһ����DWORD����ġ�
 *   ����ButterWorth��ͨ�˲������ݴ洢��lpImage���С�
 *
 *************************************************************************
 */
void ButterWorthLowPass(LPBYTE lpImage, int nWidth, int nHeight, int nRadius)
{
	// ѭ�����Ʊ���
	int y ;
	int x ;

	double dTmpOne ;
	double dTmpTwo ;

	// ButterWorth �˲�ϵ��
	double H       ;

	// ����Ҷ�任�Ŀ��Ⱥ͸߶ȣ�2���������ݣ�
	int nTransWidth ;
	int nTransHeight;

	double dReal    ;
	double dImag    ;
	// ͼ������ֵ
	unsigned char unchValue;

	// ָ��ʱ�����ݵ�ָ��
	complex<double> * pCTData ;
	// ָ��Ƶ�����ݵ�ָ��
	complex<double> * pCFData ;
	
	// ������и���Ҷ�任�ĵ���	��2���������ݣ�
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// ������и���Ҷ�任�ĵ��� ��2���������ݣ�
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;
	
	// �����ڴ�
	pCTData=new complex<double>[nTransWidth * nTransHeight];
	pCFData=new complex<double>[nTransWidth * nTransHeight];

	// ��ʼ��
	// ͼ�����ݵĿ��͸߲�һ����2���������ݣ�����pCTData
	// ��һ����������Ҫ��0
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x]=complex<double>(0,0);
		}
	}

	// ��ͼ�����ݴ���pCTData
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nWidth +x];
			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
		}
	}
	
	// ����Ҷ���任
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData) ;

	// ���濪ʼʵʩButterWorth��ͨ�˲�
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
	
	// ����ButterWorth��ͨ�˲���ͼ����з��任
	IFFT_2D(pCFData, pCTData, nWidth, nHeight);

	// ���任�����ݴ���lpImage
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

	
	// �ͷ��ڴ�
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;
	
}
/*************************************************************************
 *
 * \�������ƣ�
 *   ButterWorthHighPass()
 *
 * \�������:
 *   LPBYTE lpImage					- ָ����Ҫ��ǿ��ͼ������
 *   int nWidth							- ���ݿ���
 *   int nHeight						- ���ݸ߶�
 *   int nRadius            - ButterWorth��ͨ�˲��ġ��빦�ʡ���
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   lpImage ��ָ����Ҫ��ǿ������ָ�롣ע�⣬���ָ��ָ���������������
 *	 CDibָ����������� ��ΪCDibָ�����������ÿһ����DWORD����ġ�
 *   ����ButterWorth��ͨ�˲������ݴ洢��lpImage���С�
 *
 *************************************************************************
 */
void ButterWorthHighPass(LPBYTE lpImage, int nWidth, int nHeight, int nRadius)
{
	// ѭ�����Ʊ���
	int y ;
	int x ;

	double dTmpOne ;
	double dTmpTwo ;

	// ButterWorth �˲�ϵ��
	double H       ;

	// ����Ҷ�任�Ŀ��Ⱥ͸߶ȣ�2���������ݣ�
	int nTransWidth ;
	int nTransHeight;

	double dReal    ;
	double dImag    ;
	// ͼ������ֵ
	unsigned char unchValue;

	// ָ��ʱ�����ݵ�ָ��
	complex<double> * pCTData ;
	// ָ��Ƶ�����ݵ�ָ��
	complex<double> * pCFData ;
	
	// ������и���Ҷ�任�ĵ���	��2���������ݣ�
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// ������и���Ҷ�任�ĵ��� ��2���������ݣ�
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo   ;
	
	// �����ڴ�
	pCTData=new complex<double>[nTransWidth * nTransHeight];
	pCFData=new complex<double>[nTransWidth * nTransHeight];

	// ��ʼ��
	// ͼ�����ݵĿ��͸߲�һ����2���������ݣ�����pCTData
	// ��һ����������Ҫ��0
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x]=complex<double>(0,0);
		}
	}

	// ��ͼ�����ݴ���pCTData
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nWidth +x];
			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
		}
	}
	
	// ����Ҷ���任
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData) ;

	// ���濪ʼʵʩButterWorth��ͨ�˲�
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
	
	// ����ButterWorth��ͨ�˲���ͼ����з��任
	IFFT_2D(pCFData, pCTData, nWidth, nHeight);

	// ���任�����ݴ���lpImage
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

	
	// �ͷ��ڴ�
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;
	
}
