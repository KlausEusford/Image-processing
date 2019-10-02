#include "GlobalApi.h"
#include "stdafx.h"
#include "cdib.h"
#include "math.h"
/*************************************************************************
 *
 * \函数名称：
 *   GetFileName()
 *
 * \输入参数:
 *   CString		strFilePathName		- 图象的文件名 
 *   int		nCurFrameNum		- 当前帧的图象文件名
 *
 * \返回值:
 *   CString			- 返回给定帧数的图象文件名
 *
 * \说明:
 *   该函数根据指定文件路径名和当前图象序列的帧数获取图象文件名
 *   该函数中需要注意的是，只能读取0-999帧图象，图象为bmp格式，且按照
 *   帧数数字进行存储，例如第一帧图象文件名为×××001.bmp，第33帧图象
 *   的文件名为×××033.bmp。如果不是bmp文件，则返回"NULL"。
 *
 *************************************************************************
 */
CString GetFileName(CString strFilePathName, int nCurFrameNum)
{
	//文件的路径名
	CString strTempFileName;

	int nNumPos=strFilePathName.Find(".");
	if(nNumPos==-1){
		AfxMessageBox("Please choose a bmp file");
		return "NULL";
	}

	//表示去掉了扩展名和数字标号的路径名，在这里，限定帧数为0～999，所以采用三位来表示
	CString strFileNameNoExtNoNum=strFilePathName.Left(nNumPos-3);

	//表示标号的字符串
	CString strTempNum;

	if(nCurFrameNum<10){
		strTempNum.Format("00%d",nCurFrameNum);
	}
	else {
		if(nCurFrameNum<100 &&nCurFrameNum>=10){
			strTempNum.Format("0%d",nCurFrameNum);
		}
		else{
			strTempNum.Format("%d",nCurFrameNum);
		}
	}
	
	// 得到图象文件名
	strTempFileName=strFileNameNoExtNoNum+strTempNum+".bmp";
	
	// 返回
	return strTempFileName;
}

/*************************************************************************
 *
 * \函数名称：
 *   LoadDibSeq()
 *
 * \输入参数:
 *   CString		strFilePath		- 第一帧图象的文件名 
 *   int		nCurFrameNum		- 当前帧的图象文件名
 *   int		nTotalFrameNum		- 进行检测的图象帧数
 *   CDib*		pDib			- 指向返回CDib类的指针
 *
 * \返回值:
 *   BOOL			- 成功则返回TRUE，否则返回FALSE
 *
 * \说明:
 *   该函数根据指定文件路径名和当前图象序列的帧数读取图象数据道pDib中
 *   该函数中需要注意的是，只能读取0-999帧图象，图象为bmp格式，且按照
 *   帧数数字进行存储，例如第一帧图象文件名为×××001.bmp，第33帧图象
 *   的文件名为×××033.bmp。
 *
 *************************************************************************
 */
BOOL LoadDibSeq(CString strFilePath, int nCurFrameNum, int nTotalFrameNum, CDib* pDib)
{
	//一般来讲,程序在处理的过程中需要装载的帧号应该是由外界指定的
	//当指定的帧号不合法时,就装载当前帧作为默认值
	if (nCurFrameNum<1 || nCurFrameNum>nTotalFrameNum)
	{
		AfxMessageBox("Invalidate file frame number");
		return FALSE;
	}

	// 获得当前帧的图象文件名
	CString strTempFileName;
	strTempFileName=GetFileName(strFilePath,nCurFrameNum);
	
	CFile fileOpen=NULL;

	// 打开文件并读取
	fileOpen.Open(strTempFileName,CFile::modeRead);
	if(pDib->Read(&fileOpen)==FALSE){
		AfxMessageBox("can not open the file "+strTempFileName);
		return FALSE;
	}
	return TRUE;
}


/*************************************************************************
 *
 * \函数名称：
 *   BinaFrameDiff()
 *
 * \输入参数:
 *   unsigned char*	pUnchImg1		- 图象的文件名 
 *   unsigned char*	pUnchImg2		- 当前帧的图象文件名
 *   int		nWidth
 *   int		nHeight
 *   unsigned char*	pUnchResult
 *   int		nThreshold
 *
 * \返回值:
 *   CString			- 返回给定帧数的图象文件名
 *
 * \说明:
 *   该函数比较pUnchImg1和pUnchImg2两个区域中的内容,如果两个区域内     
 *容的差值的绝对值比Threshold大,则将pUnchResult相应的元素设置为逻辑值1,
 *用灰度255表示，否则为0，并用灰度0表示
 *
 *************************************************************************
 */
void BinaFrameDiff(unsigned char *pUnchImg1, unsigned char *pUnchImg2, 
		  int nWidth, int nHeight, unsigned char * pUnchResult,
		  int nThreshold)
{
	int nTemp=0;
	for (int i=0;i<nHeight*nWidth;i++) {
			nTemp = abs(pUnchImg1[i] - pUnchImg2[i]);			
			pUnchResult[i] = nTemp > nThreshold ? 255:0;
	} 
	return ;
}

/*************************************************************************
 *
 * \函数名称：
 *   ErodeFrameDiff()
 *
 * \输入参数:
 *   unsigned char*	pUnchImg1		- 图象数据指针
 *   int		nWidth			- 图象宽度
 *   int		nHeight			- 图象高度
 *   int		nErodeHalfWin		- 腐蚀窗口大小的一半
 *   unsigned char*	pUnchResult		- 结果数据制止
 *   int		nThreshold		- 阈值
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数进行腐蚀操作,形态学操作对pUnchImg中的每一点,计算这一点对应的
 *窗口内的一些参数,然后根据参数结果给这个点设置相应的值. 功能上相当于广义滤波	
 *
 *************************************************************************
 */
void ErodeFrameDiff(unsigned char *pUnchImg, int nWidth, int nHeight, int nErodeHalfWin, 
	  int nErodeThreshold, unsigned char *pUnchResult)
{
	// 搜索整个图象，对图象进行腐蚀处理
	for (int i=nErodeHalfWin;i<nHeight-nErodeHalfWin;i++) {
		for (int j=nErodeHalfWin;j<nWidth-nErodeHalfWin;j++) {
			// 如果帧间的差不为0才进行处理
			if (pUnchImg[i*nWidth+j] != 0) 
			{
				int iPointCount = 0;

				// 根据此点的邻域判断此点是否需要删除
				for (int r=-nErodeHalfWin;r<=nErodeHalfWin;r++) {
					for (int c=-nErodeHalfWin;c<=nErodeHalfWin;c++) {
						if (pUnchImg[(i+r)*nWidth+j+c] != 0) {
							iPointCount++;
						}
					}
				}

				// 如果邻域中不为0的个数小于设定的阈值，则强行设置为0
				if (iPointCount < nErodeThreshold) {
					pUnchResult[i*nWidth+j] = 0;
				}
				else {
					pUnchResult[i*nWidth+j] = 255;
				}
			}
			else 
			{
				pUnchResult[i*nWidth+j] = 0;
			}
		}
	}
	return ;
}

/*************************************************************************
 *
 * \函数名称：
 *   GetBackground()
 *
 * \输入参数:
 *   CString		strFilePath		- 第一帧图象的文件名 
 *   int		nTotalFrameNum		- 进行检测的图象帧数
 *   int		nImageWidth		- 图象宽度
 *   int		nImageHeight		- 图象高度
 *   unsigned char *	pUnchBackGround		- 指向返回背景数据的指针
 *
 * \返回值:
 *   BOOL			- 成功则返回TRUE，否则返回FALSE
 *
 * \说明:
 *   该函数根据指定文件名的图象序列求取静止背景
 *
 *************************************************************************
 */
BOOL GetBackground(CString strFilePath, int nTotalFrameNum, int nImageWidth, 
		   int nImageHeight, unsigned char* pUnchBackGround)
{
	// 如果此时背景已经生成,函数返回,不需要再一次计算
	/*if (pUnchBackGround!=NULL){		
		return TRUE;
	}*/

	
	// pUnchTemp1和pUnchTemp2用来计算相邻两帧之间的帧差
	// 每次只要读入一帧即可,即:假设刚刚比较k-1和k帧,那么现在比较k和
	// k+1帧,那么k帧是不需要重新读入的
	unsigned char* pUnchTemp1;
	unsigned char* pUnchTemp2;

	pUnchTemp1 = new unsigned char[nImageWidth * nImageHeight * sizeof(unsigned char)];
	pUnchTemp2 = new unsigned char[nImageWidth * nImageHeight * sizeof(unsigned char)];
	
	// 临时存放图象数据的CDib指针
	CDib* pDibTemp;
	pDibTemp = new CDib;

	// 读出第一帧数据并放入pDibTemp
	pDibTemp->Empty();
	if(!LoadDibSeq(strFilePath,1,nTotalFrameNum,pDibTemp)){
		return FALSE;
	}

	// 然后将数据取出，存放在pUnchTemp1中
	memcpy(pUnchTemp2,pDibTemp->m_lpImage,nImageWidth*nImageHeight*sizeof(unsigned char));	

	// pChResultAfterMor 是用来记录帧间变化的内存区域
	unsigned char * pUnchTrackBox = new unsigned char[(nTotalFrameNum)*
		nImageWidth*nImageHeight*sizeof(unsigned char)];

	unsigned int index = 0;

	// 帧间差的区域,二进制
	unsigned char *pUnchTemp3=new unsigned char[nImageWidth*nImageHeight*sizeof(unsigned char)];

	// 腐蚀之后的区域,二进制
	unsigned char * pUnchResultAfterMor = new unsigned char[nImageWidth*nImageHeight*sizeof(unsigned char)];

	// 对每一帧进行比较
	for (int i = 2; i<nTotalFrameNum-1; i++)	{	
		// 打开第i帧图象文件，并将图象存放在CDib对象pDibTemp中
		pDibTemp->Empty();
		if(!LoadDibSeq(strFilePath , i , nTotalFrameNum , pDibTemp)){
			return FALSE;
		}

		// 然后将数据取出，存放在pUnchTemp2中
		memcpy(pUnchTemp2,pDibTemp->m_lpImage,nImageWidth*nImageHeight);	

		// 对图象帧差进行二值化处理，并将二值化后的图象存放在pUnchTemp3中
		BinaFrameDiff(pUnchTemp1,pUnchTemp2 ,nImageWidth,nImageHeight,pUnchTemp3,10);

		// 对二值化后的图象进行腐蚀处理，在这里对腐蚀窗口的大小设置为2，阈值为7
		ErodeFrameDiff(pUnchTemp3,nImageWidth,nImageHeight,2,7,pUnchResultAfterMor);

		// 将此二值化后的程序放入pUnchTrackBox的相应位置		
		memcpy(pUnchTrackBox+index,pUnchResultAfterMor,sizeof(unsigned char)*nImageWidth*nImageHeight);

		// 计算图象数据在pUnchTrackBox中的偏移量
		index = index + nImageWidth*nImageHeight*sizeof(unsigned char);
		
		// 每做完两帧之间的比较,就使帧号下移一个,pUnchTemp1中是存k帧内容,pUnchTemp2帧是存k+1
		// 帧内容,所以,每次只要把pUnchTemp2中的内容给pTemp1,而pTemp2重新读入既可以了.
		unsigned char* pUnchTag = NULL;

		pUnchTag   = pUnchTemp1;
		pUnchTemp1 = pUnchTemp2;
		pUnchTemp2 = pUnchTag;
	}
	
	// 释放已分配内存
	delete []pUnchTemp1;	
	pUnchTemp1 = NULL;
	delete []pUnchTemp2;	
	pUnchTemp2 = NULL;	
	delete []pUnchTemp3;	
	pUnchTemp3 = NULL;
	delete []pUnchResultAfterMor;	
	pUnchResultAfterMor=NULL;
	
	// 每一帧的大小
	int nFrameSize = nImageWidth * nImageHeight * sizeof(unsigned char);
	
	// 记录最大长度	
	int * pnTrackSegLen = new int [nImageWidth*nImageHeight];

	// 记录最大长度中的中间帧标号
	int * pnTrackSegFrame = new int [nImageWidth*nImageHeight];

	// 对每一个象素点跟踪最大为0的长度，并将最大长度中的中间帧标号记录下来
	for (int y = 0; y<nImageHeight; y++)	{
		for (int x = 0; x<nImageWidth; x++)	{
			// 此象素在一帧那相对于该帧第一个元素的偏移量
			int offset = y * nImageWidth + x;

			// 初始化最大长度
			int largeLen = 0;
			int t = 1;

			// 连续为0的段的开始和结束帧标号
			int segStart,segEnd;

			// 跟踪长度
			int segLen;

			// 当前帧数
			int frameNum;
			
			segLen = 0;
			frameNum = 1;

			// 遍历整个序列，跟踪并记录此点连续为0的最大长度
			while (t < nTotalFrameNum - 1)	{
				// 如果还没有到达序列结束并且此点的不为0，则继续到下一帧搜索
				while ((t < nTotalFrameNum -1) && (pUnchTrackBox[t*nFrameSize+offset] != 0))	
					t++;

					//如果此时的t>= nTotalFrameNum - 1,则说明,已经遍历到最后一帧了
					if (t >= nTotalFrameNum - 1)	
						break;

					// 此时应为此长度的开始
					segStart = t;

					while ((t < nTotalFrameNum - 1) && (pUnchTrackBox[t*nFrameSize+offset] == 0))	
						t++;						

					// 此长度的结束帧标号
					segEnd = t - 1;

					// 获得此连续为0的帧的长度
					segLen = segEnd +1 -segStart;	

					// 判断是否为最大长度，是则进行替换
					if (segLen > largeLen)	{
						largeLen = segLen;
						frameNum = (segEnd + segStart)/2;				
					}
				} 
				
				pnTrackSegLen[offset] = largeLen;
				pnTrackSegFrame[offset] = frameNum;
		}
	}

	delete []pUnchTrackBox;	pUnchTrackBox=NULL;
	
	// 因为对每个象素而言,背景可能出现在不同帧里,此时需要把所有帧调入内存
	unsigned char* pBuffer = new unsigned char[nTotalFrameNum*(nImageWidth*nImageHeight)];

	for (int k=1; k<nTotalFrameNum; k++)	{
		pDibTemp->Empty();
		LoadDibSeq(strFilePath , k , nTotalFrameNum , pDibTemp);

		// 然后将数据取出，存放在pBuffer的相应位置中
		memcpy(pBuffer+k*nFrameSize,pDibTemp->m_lpImage,nImageWidth*nImageHeight);	
	}

	// 遍历整个图象，设置背景数据
	for (y=0; y<nImageHeight; y++)	{
		for (int x=0; x<nImageWidth; x++)	{
			// 获得此象素点在一帧数据中的偏移量
			int k = y * nImageWidth + x;	

			// 获得此象素点连续为0的最大长度的中间帧标号
			int nFrameMax = pnTrackSegFrame [k];

			// 设置具有最大长度的中间帧的数据为背景数据
			pUnchBackGround[k] = pBuffer[(nFrameMax*nFrameSize)+k];
		}
	}		

	delete []pnTrackSegLen; 
	pnTrackSegLen=NULL;
	delete []pnTrackSegFrame;	
	pnTrackSegFrame=NULL;
	delete []pBuffer; 
	pBuffer=NULL;
	
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	
	return TRUE;
}

