#include "stdafx.h"
#include "GlobalApi.h"
#include "Cdib.h"

#include <io.h>
#include <errno.h>

#include <math.h>
#include <direct.h>


/*************************************************************************
 *
 * �������ƣ�
 *   DIBHOLENUMBER()
 *
 * ����:
 *   CDib * pDib        - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����True�����򷵻�False��
 *
 * ˵��:
 *   �ú�������ȥͼ�������С����ֵ��С����
 *
 *************************************************************************/

BOOL DIBHOLENUMBER(CDib *pDib)
{
	// ָ��Դͼ���ָ��
	BYTE *	lpSrc;

	//ͼ��Ŀ��Ⱥ͸߶�
	LONG    lWidth;
	LONG    lHeight;

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	//�õ�ͼ��Ŀ��Ⱥ͸߶�
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//�õ�ʵ�ʵ�Dibͼ��洢��С
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = SizeRealDim.cx;
	
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// ѭ������
	int i, j, s, n;

	// ��Ѩ����Ŀ�Լ������ֵ
	int nHoleNum, nMinArea;

	int nBlackPix, temp;

	// ����ͷ��촫����־
	int nDir1,nDir2;

	// �����洢��һλ����
	int *pnBinary;

	pnBinary =new int[lHeight*lLineBytes];

	// С�������ֵ���Ϊ20�����ص�
	nMinArea = 50;

	// ��ͼ���ֵ��
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			// ��ɫ����Ϊ���������0
			if(*lpSrc > 200)
			{
				pnBinary[lLineBytes * j + i] = 0;
			}
			// �����ش��-1
			else
			{
				pnBinary[lLineBytes * j + i] = -1;
			}
		}		
	}
	
	// ��Ѩ������ֵ
	nHoleNum = 1;
	
	do
	{
		s=0;
		
		// Ѱ��ÿ����Ѩ�ĳ�ʼ����ֵ 
		for (j = 1; j < lHeight - 1; j++)
		{
			for(i = 1; i < lWidth - 1; i++)
			{
				// �ҵ���ʼ����
				if(pnBinary[lLineBytes * j + i] == -1)
				{
					s = 1;
					// ������ֵ�ĳɵ�ǰ�Ŀ�Ѩ��ֵ
					pnBinary[lLineBytes * j + i] = nHoleNum;
					
					// ����ѭ��
					j = lHeight;			
					i = lLineBytes;
				}
			}
		}

		//û�г�ʼ���أ�����ѭ��
		if(s == 0)
			break;
		else
		{
			do
			{
				// ����ͷ��촫��ϵ������ֵ0
				nDir1 = 0;
				nDir2 = 0;

				// ����ɨ��
				for (j = 1; j < lHeight-1; j++)
				{
					for(i = 1; i < lWidth-1; i++)
					{
						nBlackPix = pnBinary[lLineBytes * j + i];
						
						// ��������Ѿ���ɨ�裬�����Ǳ���ɫ��������һ��ѭ��
						if(nBlackPix != -1)
							continue;

						// ����ϲ������������ֵ�Ѿ���ɨ�裬�����ڵ�ǰ�Ŀ�Ѩ����ǰ������ֵ
						// �ĳɿ�Ѩ����ֵ
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

				// ��������ȫ����ɨ�裬����ѭ��
				if(nDir1 == 0)
					break;

				// ����ɨ��
				for (j = lHeight-2; j >= 1 ; j--)
				{
					for(i = lWidth-2; i >= 1 ; i--)
					{
						nBlackPix = pnBinary[lLineBytes * j + i];						
						
						// ��������Ѿ���ɨ�裬�����Ǳ���ɫ��������һ��ѭ��
						if(nBlackPix != -1)
							continue;

						// ����²�����Ҳ������ֵ�Ѿ���ɨ�裬�����ڵ�ǰ�Ŀ�Ѩ����ǰ������ֵ
						// �ĳɿ�Ѩ����ֵ
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
		// ��Ѩ������
		nHoleNum++;
	}
	while(1);

	nHoleNum -- ;

	// Ѱ�����С����ֵ�Ŀ�Ѩ����
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

				// �����������Ѿ�������ֵ������ѭ��
				if(s > nMinArea)
					break;
			}

			if(s > nMinArea)
					break;
		}

		// С����ֵ�����򣬸����뱳��һ������ɫ��������ȥ
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

	// �洢����ֵ�����
	for(j = 0; j < lHeight; j++)
	{
		// ��
		for(i = 0; i < lWidth; i++)
		{
			// ��ֵͼ��
		    temp = pnBinary[j * lLineBytes + i] ;
			
			// ָ��λͼi��j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * j + i;
			
			// ����Դͼ��
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
 * �������ƣ�
 *   DIBMOMENT()
 *
 * ����:
 *   CDib * pDib        - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����True�����򷵻�False��
 *
 * ˵��:
 *   �ú�������ͼ������ء�
 *
 *************************************************************************/
BOOL DIBMOMENT(CDib *pDib)
{
	// ָ��Դͼ���ָ��
	BYTE *	lpSrc;

	//ͼ��Ŀ��Ⱥ͸߶�
	LONG    lWidth;
	LONG    lHeight;

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	//�õ�ͼ��Ŀ��Ⱥ͸߶�
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//�õ�ʵ�ʵ�Dibͼ��洢��С
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = SizeRealDim.cx;
	
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// ͼ��ľ�
	long nImageMoment;

	// ѭ������
	int i,j;

	int ip,jq;

	// ��ʱ����
	double temp;

	nImageMoment = 0;

	// ����һ�׾�
	ip = jq = 1;

	// ���صļ���
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
	
	// ������������
	CString CView;
	CView.Format("ͼ�������Ϊ��%d",nImageMoment);
	MessageBox(NULL,CView, "������" ,	MB_ICONINFORMATION | MB_OK);

	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBBARYCENTERMOMENT()
 *
 * ����:
 *   CDib * pDib        - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����True�����򷵻�False��
 *
 * ˵��:
 *   �ú�������ͼ������ľء�
 *
 *************************************************************************/

BOOL DIBBARYCENTERMOMENT(CDib *pDib)
{
	// ָ��Դͼ���ָ��
	BYTE *	lpSrc;

	//ͼ��Ŀ��Ⱥ͸߶�
	LONG    lWidth;
	LONG    lHeight;

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	//�õ�ͼ��Ŀ��Ⱥ͸߶�
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//�õ�ʵ�ʵ�Dibͼ��洢��С
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = SizeRealDim.cx;
	
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// ͼ������ֵ
	int  nPixelValue;

	// ͼ�����ľ�
	long nBarycenterMoment;

	// 0�ξ�m00,x�����һ�ξ�m01��y�����һ�ξ�m10
	long m00, m10, m01;

	// ����x��y����ֵ
	int nBarycenterX,nBarycenterY;
	
	// ѭ������
	int i,j;

	// ��ʱ����
	double temp;

	// ����ֵΪ��
	m00 = 0;
	m01 = 0;
	m10 = 0;
	nBarycenterMoment = 0;

	// ��0�ξ�m00,x�����һ�ξ�m01��y�����һ�ξ�m10
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

	// ����x��y����ֵ
	nBarycenterX = (int)(m01 / m00 + 0.5);
	nBarycenterY = (int)(m10 / m00 + 0.5);


	// �������ľ�
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

	// ������������
	CString CView;
	CView.Format("ͼ������ľ�Ϊ��%d,%d�������ľ�Ϊ%d"
					,nBarycenterX,nBarycenterY,nBarycenterMoment);
	MessageBox(NULL,CView, "������" ,	MB_ICONINFORMATION | MB_OK);

	return true;

}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBSTREETDIS()
 *
 * ����:
 *   CDib * pDib        - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����True�����򷵻�False��
 *
 * ˵��:
 *   �ú����������̾����ͼ����б任��
 *
 *************************************************************************/

BOOL DIBSTREETDIS(CDib *pDib)
{
	// ָ��Դͼ���ָ��
	BYTE *	lpSrc;

	//ͼ��Ŀ��Ⱥ͸߶�
	LONG    lWidth;
	LONG    lHeight;

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	//�õ�ͼ��Ŀ��Ⱥ͸߶�
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//�õ�ʵ�ʵ�Dibͼ��洢��С
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = SizeRealDim.cx;
	
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// ѭ������ʱ����
	int i,j;
	int temp, s;

	// �洢����ֵ������
	int *pnBinary, *pnStore;
	
	int nImageValue;

	pnBinary = new int[lHeight*lLineBytes];
	pnStore	 = new int[lHeight*lLineBytes];	

	// ��ͼ���ֵ��
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			// ��ɫ����Ϊ���������0
			if(*lpSrc > 200)
			{
				pnBinary[lLineBytes * j + i] = 0;
				pnStore [lLineBytes * j + i] = 0;
			}
			// �����ش��1
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

		// ���о�����ۼ�
		for (j = 1; j < lHeight - 1; j++)
		{
			for(i = 1; i < lWidth - 1; i++)
			{
				nImageValue = pnBinary[lLineBytes * j + i];
				
				// ����Ǳ�����������һ��ѭ��
				if(nImageValue == 0)
					continue;
				
				// �����ǰ����ֵ�����ܵ�ֵһ��������ֵ����һ
				if(nImageValue==pnBinary[lLineBytes * (j-1) + i] && nImageValue==pnBinary[lLineBytes * (j+1) + i])
					if(nImageValue==pnBinary[lLineBytes * j + i-1] && nImageValue==pnBinary[lLineBytes * j + i+1])
					{
						pnStore[lLineBytes * j + i]++;
						s=1;								
					}
			}
		}
		
		// �ڽ�����һ��ѭ��ǰ����ǰ�Ľ������
		for (j = 0; j < lHeight; j++)
			for(i = 1; i < lWidth; i++)
				pnBinary[lLineBytes * j + i] = pnStore[lLineBytes * j + i];
	}

	// �洢����ֵ�����
	for(j = 0; j < lHeight; j++)
	{
		// ��
		for(i = 0; i < lWidth; i++)
		{
			// �Ǽܵ�����ֵ
		    temp = pnStore[j * lLineBytes + i] ;
			
			// ָ��λͼi��j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * j + i;
			
			// ����Դͼ��
			* (lpSrc) = (BYTE)((25 - temp) * 10 + 5);			
		}
	}

	delete pnStore;
	delete pnBinary;

	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBFREAMEWORK()
 *
 * ����:
 *   CDib * pDib        - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����True�����򷵻�False��
 *
 * ˵��:
 *   �ú����������̾��룬����ͼ��ĹǼܡ�
 *
 *************************************************************************/

BOOL DIBFREAMEWORK(CDib *pDib)
{
	// ָ��Դͼ���ָ��
	BYTE *	lpSrc;

	//ͼ��Ŀ��Ⱥ͸߶�
	LONG    lWidth;
	LONG    lHeight;

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	//�õ�ͼ��Ŀ��Ⱥ͸߶�
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//�õ�ʵ�ʵ�Dibͼ��洢��С
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = SizeRealDim.cx;
	
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// ѭ������ʱ����
	int i,j;
	int temp, s;

	// �洢����ֵ������
	int *pnBinary, *pnStore;
	
	int nImageValue;

	pnBinary = new int[lHeight*lLineBytes];
	pnStore	 = new int[lHeight*lLineBytes];	

	// ��ͼ���ֵ��
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			// ��ɫ����Ϊ���������0
			if(*lpSrc > 200)
			{
				pnBinary[lLineBytes * j + i] = 0;
				pnStore [lLineBytes * j + i] = 0;
			}
			// �����ش��1
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

		// ���о�����ۼ�
		for (j = 1; j < lHeight - 1; j++)
		{
			for(i = 1; i < lWidth - 1; i++)
			{
				nImageValue = pnBinary[lLineBytes * j + i];
				
				// ����Ǳ�����������һ��ѭ��
				if(nImageValue == 0)
					continue;
				
				// �����ǰ����ֵ�����ܵ�ֵһ��������ֵ����һ
				if(nImageValue==pnBinary[lLineBytes * (j-1) + i] && nImageValue==pnBinary[lLineBytes * (j+1) + i])
					if(nImageValue==pnBinary[lLineBytes * j + i-1] && nImageValue==pnBinary[lLineBytes * j + i+1])
					{
						pnStore[lLineBytes * j + i]++;
						s=1;								
					}
			}
		}
		
		// �ڽ�����һ��ѭ��ǰ����ǰ�Ľ������
		for (j = 0; j < lHeight; j++)
			for(i = 1; i < lWidth; i++)
				pnBinary[lLineBytes * j + i] = pnStore[lLineBytes * j + i];
	}
	
	for (j = 0; j < lHeight; j++)
		for(i = 0; i < lWidth; i++)
			pnStore[lLineBytes * j + i] = 0;
	
	// �����ǰ������ֵ����Χ�Ķ�Ҫ�ߣ���Ϊ�Ǽ�	
	for (j = 1; j < lHeight - 1; j++)
		for(i = 1; i < lWidth - 1; i++)
		{
			nImageValue = pnBinary[lLineBytes * j + i] + 1;
			
			// ������ͨ����бȽ�
			if(nImageValue!=pnBinary[lLineBytes * (j-1) + i] && nImageValue!=pnBinary[lLineBytes * (j+1) + i])
					if(nImageValue!=pnBinary[lLineBytes * j + i-1] && nImageValue!=pnBinary[lLineBytes * j + i+1])
						pnStore[lLineBytes * j + i] = pnBinary[lLineBytes * j + i];
		}


	// �洢����ֵ�����
	for(j = 0; j < lHeight; j++)
	{
		// ��
		for(i = 0; i < lWidth; i++)
		{
			// �Ǽܵ�����ֵ
		    temp = pnStore[j * lLineBytes + i] ;
			
			// ָ��λͼi��j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * j + i;
			
			// ����Դͼ�񣬲�������ֵ���б任���Ա����
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
 * �������ƣ�
 *   DIBCHESSBOARDDISRESTORE()
 *
 * ����:
 *   CDib * pDib        - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����True�����򷵻�False��
 *
 * ˵��:
 *   �ú������ùǼܶ�ԭͼ����лָ���
 *
 *************************************************************************/
BOOL DIBCHESSBOARDDISRESTORE(CDib *pDib)
{
	// ָ��Դͼ���ָ��
	BYTE *	lpSrc;

	//ͼ��Ŀ��Ⱥ͸߶�
	LONG    lWidth;
	LONG    lHeight;

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	//�õ�ͼ��Ŀ��Ⱥ͸߶�
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//�õ�ʵ�ʵ�Dibͼ��洢��С
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = SizeRealDim.cx;
	
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// ѭ������
	int i, j, n, m;

	// ��ʱ����
	int temp, s;

	// �����������ֵ������
	int *pnBinary, *pnStore;
	
	int nImageValue;

	pnBinary = new int[lHeight*lLineBytes];
	pnStore	 = new int[lHeight*lLineBytes];	

	// ���鸳ֵ
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			pnBinary[lLineBytes * j + i] = 0;
			if((*lpSrc) != 255)
				pnStore [lLineBytes * j + i] = (*lpSrc);
			else
				pnStore [lLineBytes * j + i] = 0;
		}		
	}

	
	// ����ͼ��Ļָ�
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
			
			// �������ֵ���ڵ���2�����ԣ�2��nImageValue����1�����η�Χ������ֵ��1
			s = nImageValue;

			// ���ε�����
			for(m = -s; m <= s; m++)
			{
				pnBinary[lLineBytes * j + i + m] = 1;

			}

			// ���ε��ϰ벿��
			for(n = -s; n < 0; n++)
				for(m = -s - n; m <=s + n; m++)
					pnBinary[lLineBytes * (j+n) + i+m] = 1;

			// ���ε��°벿��
			for(n = 1; n <= s; n++)
				for(m = -s + n; m <= s - n; m++)
					pnBinary[lLineBytes * (j+n) + i+m] = 1;
		
		}
	}

	// �洢����ֵ�����
	for(j = 0; j < lHeight; j++)
	{
		// ��
		for(i = 0; i < lWidth; i++)
		{
			// �Ǽܵ�����ֵ
		    temp = pnBinary[j * lLineBytes + i] ;
			
			// ָ��λͼi��j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * j + i;
			
			// ����Դͼ��
			if(temp != 0)	
				// ��ɫ�������
				* (lpSrc) = 0;
			else
				// ��ɫ�������
				* (lpSrc) = 255;
		}
	}

	delete pnStore;
	delete pnBinary;

	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBOUTLINE()
 *
 * ����:
 *   CDib * pDib        - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����True�����򷵻�False��
 *
 * ˵��:
 *   �ú����Զ�ֵͼ��������������
 *
 *************************************************************************
 */

BOOL DIBOUTLINE(CDib *pDib)
{
	
	// ָ��Դͼ���ָ��
	BYTE *	lpSrc;

	//ͼ��Ŀ��Ⱥ͸߶�
	LONG    lWidth;
	LONG    lHeight;

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	//�õ�ͼ��Ŀ��Ⱥ͸߶�
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//�õ�ʵ�ʵ�Dibͼ��洢��С
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = SizeRealDim.cx;
	
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// ѭ������
	int i, j;

	int nPixelValue;

	int n1,n2,n3,n4,n5,n6,n7,n8;

	// �����������ֵ������
	int *pnBinary;
	
	pnBinary = new int[lHeight*lLineBytes];

	// ��ͼ���ֵ��
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			// ��ɫ����Ϊ���������0
			if(*lpSrc > 200)
			{
				pnBinary[lLineBytes * j + i] = 0;
			}
			// �����ش��1
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

			// �����ǰ�����ǰ�ɫ��������һ��ѭ��
			if(nPixelValue == 0)
			{
				// ����Ӧ������ֵ�ĳ���
			
				*lpSrc = 255;
				continue;
			}

			// �����ǰ�����Ǻ�ɫ
			else
			{
				// ����ܱߵ�8��ͨ��
				n1 = pnBinary[lLineBytes * (j+1) + i + 1];
				n2 = pnBinary[lLineBytes * (j+1) + i - 1];
				n3 = pnBinary[lLineBytes * (j+1) + i];
				n4 = pnBinary[lLineBytes * (j-1)+ i + 1];
				n5 = pnBinary[lLineBytes * (j-1) + i - 1];
				n6 = pnBinary[lLineBytes * (j-1) + i];
				n7 = pnBinary[lLineBytes * j + i + 1];
				n8 = pnBinary[lLineBytes * j + i - 1];
				
				//������ڵİ˸��㶼�Ǻڵ�
				if(n1&&n2&&n3&&n4&&n5&&n6&&n7&&n8 == 1)
				{
					*lpSrc = (unsigned char)255;
				}
			}
		}
	}

	delete pnBinary;

	// ����
	return TRUE;
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBTrace()
 *
 * ����:
 *   CDib * pDib        - ָ��CDib���ָ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����True�����򷵻�False��
 *
 * ˵��:
 * �ú������ڶ�ͼ����������������㡣
 *
 *************************************************************************
 */

BOOL WINAPI DIBTrace(CDib *pDib)
{
	
	// ָ��Դͼ���ָ��
	BYTE *	lpSrc;

	//ͼ��Ŀ��Ⱥ͸߶�
	LONG    lWidth;
	LONG    lHeight;

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	//�õ�ͼ��Ŀ��Ⱥ͸߶�
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//�õ�ʵ�ʵ�Dibͼ��洢��С
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = SizeRealDim.cx;
	
	//ͼ�����ݵ�ָ��
	LPBYTE  lpDIBBits = pDib->m_lpImage;
	
	//ѭ������
	long i;
	long j;

	//����ֵ
	int nPixelValue;

	//�Ƿ��ҵ���ʼ�㼰�ص���ʼ��
	bool bFindStartPoint;

	//�Ƿ�ɨ�赽һ���߽��
	bool bFindPoint;

	// �����߽��������б߽�������
	int nStartPointX, nStartPointY;
	int *nCurrPointX, *nCurrPointY;
	
	// �߽�����Ŀ
	long lNum;

	// �����洢��ֵ���ص�����
	int *pnBinary;	

	//�˸��������ʼɨ�跽��
	int Direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};
	int BeginDirect;

	// �����ڴ�
	pnBinary =new int[lLineBytes*lHeight];
	nCurrPointX = new int[lLineBytes*lHeight/4];
	nCurrPointY = new int[lLineBytes*lHeight/4];	

	// ��ͼ���ֵ��
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			// ��ɫ����Ϊ���������0
			if(*lpSrc > 200)
			{
				pnBinary[lLineBytes * j + i] = 0;
				
			}
			// �����ش��1
			else
			{
				pnBinary[lLineBytes * j + i] = 1;
			}

			// ���Ѿ�ɨ��������ش�ɰ�ɫ����
			*lpSrc = 255;
		}		
	}	

	//���ҵ������Ϸ��ı߽��
	bFindStartPoint = false;

	for (j = 0; j < lHeight; j++)
	{
		for(i = 0;i < lWidth ;i++)
		{
			nPixelValue = pnBinary[lLineBytes * j + i];
			
			if(nPixelValue == 1)
			{
				bFindStartPoint = true;

				// ����߽���ʼ��
				nStartPointY = j;
				nStartPointX  = i;
				
				// ����ѭ��
				j = lHeight;
				i = lLineBytes ;
			}		
		}
	}

	// û����ʼ�㣬����
	if(bFindStartPoint == false)
		return false;

	//������ʼ���������Ϸ�����ʼɨ�����Ҳ෽��
	BeginDirect = 0;
	//���ٱ߽�
	bFindStartPoint = false;

	//�ӳ�ʼ�㿪ʼɨ��
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

			// �ҵ���һ���߽��
			if(nPixelValue == 1)
			{
				bFindPoint = true;

				// λ�ô洢
				nCurrPointY[lNum+1] = nCurrPointY[lNum] + Direction[BeginDirect][1];
				nCurrPointX[lNum+1] = nCurrPointX[lNum] + Direction[BeginDirect][0];
				lNum ++;

				if(nCurrPointY[lNum] == nStartPointY && nCurrPointX[lNum] == nStartPointX)
				{
					bFindStartPoint = true;
				}

				//ɨ��ķ�����ʱ����ת����
				BeginDirect--;
				if(BeginDirect == -1)
					BeginDirect = 7;
				BeginDirect--;
				if(BeginDirect == -1)
					BeginDirect = 7;
			}
			else
			{
				//ɨ�跽��˳ʱ����תһ��
				BeginDirect++;
				if(BeginDirect == 8)
					BeginDirect = 0;

			}

		}
	}

	// �洢��ʾ�߽��
	for(i = 0; i <= lNum; i++)
	{
		lpSrc  = (unsigned char *)lpDIBBits + nCurrPointY[i] *lLineBytes  + nCurrPointX[i];
		*lpSrc = 0;
	}

	delete pnBinary;

	// ����
	return TRUE;
}