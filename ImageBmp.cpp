#include "StdAfx.h"
#include "ImageBmp.h"
#include "math.h"

#include "MainFrm.h"

const double PI = 3.1415926535892932384;

float Template_Smooth_Avg[9] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};

float Template_Smooth_Gauss[9] = 
{
	1,2,1,
	2,4,2,
	1,2,1
};

float Template_HSobel[9] = 
{
	-1,0,1,
	-2,0,2,
	-1,0,1
};

float Template_VSobel[9] = 
{
	-1,-2,-1,
	0,0,0,
	1,2,1
};

float Template_LoG[25] = 
{
	0,0,-1,0,0,
	0,-1,-2,-1,0,
	-1,-2,16,-2,-1,
	0,-1,-2,-1,0,
	0,0,-1,0,0
};

float Template_LoG_Small[9] = 
{
	1,4,1,
	1,-20,1,
	1,4,1
};

float Template_Laplace[9] = 
{
	0,-1,0,
	-1,4,-1,
	0,-1,0
};

CImageBmp::CImageBmp(void)
: m_pBMIH(NULL)
{
	m_pBMIH = NULL;
	m_pData = NULL;
	m_pColorTable = NULL;
	m_nColorEnrties = 0;
}

CImageBmp::~CImageBmp(void)
{
}

bool CImageBmp::AttachFromFile(CString argstrFileName)
{
	CFile file;
	bool reA = file.Open(argstrFileName,CFile::modeRead | CFile::shareDenyWrite);

	if (reA == true)
		;
	else
		return false;

	return AttachFromFile(file);
}

bool CImageBmp::AttachFromFile(CFile &argfl)
{
	CleanEnv();

	BITMAPFILEHEADER bmfHeader;
	bool reA;

	reA= argfl.Read(&bmfHeader,sizeof(BITMAPFILEHEADER));
	if (reA == false)
		return false;

	if (bmfHeader.bfType != MAKEWORD('B','M'))
		return false;

	m_pBMIH = (BITMAPINFOHEADER*)new BYTE[bmfHeader.bfOffBits - sizeof(bmfHeader)];
	
	reA = argfl.Read(m_pBMIH,bmfHeader.bfOffBits - sizeof(BITMAPFILEHEADER));
	if (reA == false)
	{
		CleanEnv();
		return false;
	}

	LPVOID pColorTable = NULL;
	int nColorEn = 0;
	nColorEn = (bmfHeader.bfOffBits - sizeof(bmfHeader) - sizeof(BITMAPINFOHEADER))/sizeof(RGBQUAD);
	
	if (nColorEn > 0)
	{
		pColorTable = m_pBMIH + 1;
	}
	else
	{
		pColorTable = NULL;
	}

	m_pColorTable = pColorTable;
	m_nColorEnrties = nColorEn;

	m_pBMIH->biWidth = abs(m_pBMIH->biWidth);
	int nWidthBytes = WIDTHBYTES(m_pBMIH->biBitCount * m_pBMIH->biWidth);

	m_pData = new LPBYTE[m_pBMIH->biHeight];
	for(int i = 0;i<m_pBMIH->biHeight;i++)
	{
		m_pData[i] = new BYTE[nWidthBytes];
		if (argfl.Read(m_pData[i],nWidthBytes) < nWidthBytes)
			ASSERT(false);
	}

	ASSERT(argfl.end);

	return true;
}

bool CImageBmp::AttachFromFile(LPCTSTR argstrFileName)
{
	CFile file;
	bool reA = file.Open(argstrFileName,CFile::modeRead | CFile::shareDenyWrite);

	if (reA == false)
	{
		return false;
	}

	return AttachFromFile(file);
}

bool CImageBmp::SaveToFile(CFile& argfl)
{
	if (IsValidate() == false)
		return false;

	BITMAPFILEHEADER bmfh = { 0 };
	int nWidthBytes = WIDTHBYTES(m_pBMIH->biWidth * m_pBMIH->biBitCount);

	bmfh.bfType = MAKEWORD('B','M');
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_nColorEnrties * 4;

	argfl.Write(&bmfh,sizeof(bmfh));
	argfl.Write(m_pBMIH,sizeof(BITMAPINFOHEADER) + m_nColorEnrties*4);

	for(int i=0;i<m_pBMIH->biHeight;i++)
	{
		argfl.Write(m_pData[i],nWidthBytes);
	}

	argfl.Flush();

	return true;
}

bool CImageBmp::IsValidate(void)
{
	if (m_pBMIH != NULL && m_pData != NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CImageBmp::CleanEnv(void)
{
	if (m_pBMIH != NULL && m_pData != NULL)
	{
		for(int i=0;i<m_pBMIH->biHeight;i++)
		{
			delete m_pData[i];
		}

		delete m_pData;
		m_pData = NULL;
	}

	if (m_pBMIH != NULL)
	{
		m_nColorEnrties = 0;
		m_pColorTable = NULL;

		delete m_pBMIH;
		m_pBMIH = NULL;
	}

	return true;
}

bool CImageBmp::SaveToFile(LPCTSTR argstrFileName)
{
	CFile fl;
	fl.Open(argstrFileName,CFile::modeReadWrite | CFile::modeCreate);

	return SaveToFile(fl);
}

int CImageBmp::GetHeight(void)
{
	if (!IsValidate())
		return -1;

	return m_pBMIH->biHeight;
}

int CImageBmp::GetWidthPixel(void)
{
	if (!IsValidate())
		return -1;

	return m_pBMIH->biWidth;
}

int CImageBmp::GetWidthByte(void)
{
	if (!IsValidate())
		return -1;

	return WIDTHBYTES(m_pBMIH->biWidth * m_pBMIH->biBitCount);
}

bool CImageBmp::SetPixel(int x, int y, COLORREF color)
{
	if(m_pBMIH->biBitCount == 8)			// 256色图
	{
		m_pData[m_pBMIH->biHeight - y - 1][x] = GetRValue(color);
	}
	else if(m_pBMIH->biBitCount == 1)		// 单色图
	{
		BYTE Color = GetRValue(color);


		// 令0代表黑
		RGBQUAD *p = (RGBQUAD*)m_pColorTable;
		if(p[0].rgbBlue != 0)
			Color = !Color;

		y = m_pBMIH->biHeight - y - 1;

		// 黑色点
		if (Color == 0)  // 设置对应位为0
		{
			if(x % 8==0)
			{
				m_pData[y][x/8] &= 0x7F;  
			}
			else if(x % 8==1)
			{
				m_pData[y][x/8] &= 0xBF;
			}
			else if(x % 8==2)
			{
				m_pData[y][x/8] &= 0xDF;
			}
			else if(x % 8==3)
			{
				m_pData[y][x/8] &= 0xEF;
			}
			else if(x % 8==4)
			{
				m_pData[y][x/8] &= 0xF7;
			}
			else if(x % 8==5)
			{
				m_pData[y][x/8] &= 0xFB;
			}
			else if(x % 8==6)
			{
				m_pData[y][x/8] &= 0xFD;
			}
			else if(x % 8==7)
			{
				m_pData[y][x/8] &= 0xFE;
			}
		}
		else // 白色点
		{
			if(x % 8==0)
			{
				m_pData[y][x/8] |= 0x80;
			}
			else if(x % 8==1)
			{
				m_pData[y][x/8] |= 0x40;
			}
			else if(x % 8==2)
			{
				m_pData[y][x/8] |= 0x20;
			}
			else if(x % 8==3)
			{
				m_pData[y][x/8] |= 0x10;
			}
			else if(x % 8==4)
			{
				m_pData[y][x/8] |= 0x08;
			}
			else if(x % 8==5)
			{
				m_pData[y][x/8] |= 0x04;
			}
			else if(x % 8==6)
			{
				m_pData[y][x/8] |= 0x02;
			}
			else if(x % 8==7)
			{
				m_pData[y][x/8] |= 0x01;
			}
		}
	}
	else if(m_pBMIH->biBitCount == 24)
	{
		m_pData[m_pBMIH->biHeight - y - 1][x*3] = GetBValue(color);
		m_pData[m_pBMIH->biHeight - y - 1][x*3 + 1] = GetGValue(color);
		m_pData[m_pBMIH->biHeight - y - 1][x*3 + 2] = GetRValue(color);
	}

	return true;
}

bool CImageBmp::ModalFrom(CImageBmp& argib)
{
	if (!argib.IsValidate())
	{
		return false;
	}

	CleanEnv();

	m_pBMIH = (BITMAPINFOHEADER*)new BYTE[argib.m_nColorEnrties * 4 + sizeof(BITMAPINFOHEADER)];
	memcpy(m_pBMIH,argib.m_pBMIH,sizeof(BITMAPINFOHEADER) + argib.m_nColorEnrties * 4);
	m_nColorEnrties = argib.m_nColorEnrties;

	int nWidthBytes = WIDTHBYTES(m_pBMIH->biWidth * m_pBMIH->biBitCount);
	m_pData = new LPBYTE[m_pBMIH->biHeight];

	for(int i=0;i<m_pBMIH->biHeight;i++)
	{
		m_pData[i] = new BYTE[nWidthBytes];
		memcpy(m_pData[i],argib.m_pData[i],nWidthBytes);
	}

	return false;
}

bool CImageBmp::IsBinaryImage(void)
{
	if (!IsValidate())
	{
		return false;
	}

	for(int i=0;i<GetWidthPixel();i++)
	{
		for(int j=0;j<GetHeight();j++)
		{
			BYTE bytGray = GetGray(i,j);
			if (bytGray != 0 && bytGray != 255)
			{
				return false;
			}
		}
	}

	return true;
}

bool CImageBmp::IsIndexedImage(void)
{
	if ((m_pColorTable != NULL) && (m_nColorEnrties !=0))
		return true;
	else
		return false;
}

bool CImageBmp::IndexToGray(void)
{
	if (!IsValidate())
		return false;

	if (!IsIndexedImage()) return false;
	RGBQUAD *tbRGB = (RGBQUAD*)m_pColorTable;
	
	m_pBMIH->biBitCount = 8;
	
	for(int i=0;i<GetHeight();i++)
	{
		for(int j = 0;j<GetWidthPixel();j++)
		{
			RGBQUAD rgb = *(tbRGB + GetGray(j,i));
			BYTE gray = rgb.rgbBlue*0.114 + rgb.rgbGreen*0.587 + rgb.rgbRed*0.299 + 0.5;

			SetPixel(j,i,gray);
		}
	}

	for(int i=0;i<255;i++)
	{
		(tbRGB +i)->rgbBlue = i;
		(tbRGB +i)->rgbGreen = i;
		(tbRGB +i)->rgbRed = i;
		(tbRGB +i)->rgbReserved = i;
	}

	m_nColorEnrties = 256;
	return true;
}

bool CImageBmp::Draw(CDC* pdc)
{
	return DrawAt(0,0,pdc);
}

bool CImageBmp::DrawAt(int x, int y, CDC* pdc)
{
	if (!IsValidate())
	{
		return false;
	}

	for(int i=0;i<GetHeight();i++)
	{
		SetDIBitsToDevice(pdc->m_hDC,x,y,m_pBMIH->biWidth,m_pBMIH->biHeight,0,0,i,1,m_pData[i],(BITMAPINFO*)m_pBMIH,DIB_RGB_COLORS);
	}

	return false;
}

bool CImageBmp::GetHist(double* outHist, int argSegNum)
{
	if (!IsValidate())
	{
		memset(outHist,0,argSegNum * sizeof(double));
		return false;
	}
	
	if (argSegNum <= 0 && argSegNum > 256)
	{
		memset(outHist,0,argSegNum * sizeof(double));
		return false;
	}
	
	//if (m_pBMIH->biBitCount != 8)
	//{
	//	memset(outHist,0,argSegNum * sizeof(double));
	//	return false;
	//}

	memset(outHist,0,argSegNum * sizeof(double));
	int nSegLen = 256.0 / argSegNum;

	BYTE bytGray;
	for(int i=0;i<m_pBMIH->biHeight;i++)
	{
		for(int j=0;j<m_pBMIH->biWidth;j++)
		{
			bytGray = GetGray(j,i);
			outHist[(int)(bytGray / nSegLen)] ++;
		}
	}

	int nSquare = m_pBMIH->biWidth * m_pBMIH->biHeight;

	for(int i=0;i<argSegNum;i++)
	{
		outHist[i] = outHist[i] / (double)nSquare; 
	}

	return true;
}

//这个函数还有BUG。
bool CImageBmp::DrawWithMemDC(CDC *pdc)
{
	if (!IsValidate())
		return false;

	CDC memDC;
	memDC.CreateCompatibleDC(pdc);
	CBitmap membmp;
	membmp.CreateCompatibleBitmap(pdc,m_pBMIH->biWidth,m_pBMIH->biHeight);

	for(int i=0;i<GetHeight();i++)
	{
		for(int j=0;j<GetWidthPixel();j++)
		{
			memDC.SetPixel(j,i,GetPixel(j,i));
		}
	}

	pdc->BitBlt(0,0,m_pBMIH->biWidth,m_pBMIH->biHeight,&memDC,0,0,SRCCOPY);
	
	return true;
}

bool CImageBmp::LinTran(double dFa, double dFb)
{
	if (!IsValidate())
		return false;
	//if (m_pBMIH->biBitCount != 8)
	//	return false;

	IndexToGray();

	BYTE bytGray;
	int nReTran;

	for(int i=0;i<GetHeight();i++)
	{
		for(int j=0;j<GetWidthPixel();j++)
		{
			bytGray = GetGray(j,i);
			nReTran = (int)(bytGray * dFa + dFb);

			if (nReTran > 255)
				nReTran =255;
			if (nReTran < 0)
				nReTran = 0;

			SetPixel(j,i,RGB(nReTran,nReTran,nReTran));
		}
	}

	return true;
}

bool CImageBmp::LogTran(double dFc)
{
	if (!IsValidate())
		return false;
	if (m_pBMIH->biBitCount != 8)
		return false;

	IndexToGray();

	BYTE bytGray;
	int nReTran;

	for(int i=0;i<GetHeight();i++)
	{
		for(int j=0;j<GetWidthPixel();j++)
		{
			bytGray = GetGray(j,i);
			nReTran = (int)( dFc * log((double)(bytGray + 1)));
			
			if (nReTran > 255)
				nReTran =255;
			if (nReTran < 0)
				nReTran = 0;

			SetPixel(j,i,RGB(nReTran,nReTran,nReTran));
		}
	}

	return true;
}

bool CImageBmp::GammerTran(double dGammer, double dexp)
{
	if (!IsValidate())
		return false;
	if (m_pBMIH->biBitCount != 8)
		return false;

	IndexToGray();

	BYTE bytGray;
	int nReTran;

	for(int i=0;i<GetHeight();i++)
	{
		for(int j=0;j<GetWidthPixel();j++)
		{
			bytGray = GetGray(j,i);

			nReTran = (int)( pow((bytGray + dexp)/255.0,dGammer) * 255);
		
			if (nReTran > 255)
				nReTran =255;
			if (nReTran < 0)
				nReTran = 0;

			SetPixel(j,i,RGB(nReTran,nReTran,nReTran));
		}
	}

	return true;
}

bool CImageBmp::ThresholdTran(int nThres)
{
	if (!IsValidate())
		return false;
	//if (m_pBMIH->biBitCount != 8)
	//	return false;

	BYTE bytGray;
	int nReTran;

	for(int i=0;i<GetHeight();i++)
	{
		for(int j=0;j<GetWidthPixel();j++)
		{
			bytGray = GetGray(j,i);

			if (bytGray >= nThres)
				nReTran = 255;
			else
				nReTran = 0;

			SetPixel(j,i,RGB(nReTran,nReTran,nReTran));
		}
	}

	return true;
}

bool CImageBmp::HistEq(void)
{
	if (!IsValidate())
		return false;

	BYTE bytGray;
	int nReTran;

	double dHist[256];
	double dtmp[256];

	GetHist(dHist,256);

	for(int i=0;i<256;i++)
	{
		dtmp[i] = 0;
		
		if (i == 0)
		{
			dtmp[i] = dHist[i];
		}
		else
		{
			dtmp[i] = dtmp[i - 1] + dHist[i];
		}
		
	}

	for(int i=0;i<GetHeight();i++)
	{
		for(int j=0;j<GetWidthPixel();j++)
		{
			bytGray = GetGray(j,i);

			nReTran = dtmp[bytGray] * 255;

			SetPixel(j,i,RGB(nReTran,nReTran,nReTran));
		}
	}

	return true;
}

bool CImageBmp::HistSt(CImageBmp *ibStd)
{
	if (!IsValidate())
		return false;
	if (m_pBMIH->biBitCount != 8)
		return false;

	IndexToGray();

	double dHist[256];
	double dTran[256];
	memset(dTran,-1,256);

	ibStd->GetHist(dHist,256);
	double dtmp;
	for(int i = 0;i<256;i++)
	{
		dtmp = 0;
		for(byte k=0;k<i;k++)
		{
			dtmp += dHist[k];
		}

		dTran[(int)(0.5 + 255 * dtmp)] = i;
	}

	int i = 0;
	int j = 0;
	while(i < 255)
	{
		if (dTran[i+1] != -1)
		{
			i ++;
			continue;
		}
		j = 1;
		while((dTran[i + j] == -1) && (i + j <= 255))
		{
			dTran[i + j] = dTran[i];
			j++;
		}
	}

	double datmp[256];

	for(int i=0;i<256;i++)
	{
		datmp[i] = 0;

		if (i == 0)
		{
			datmp[i] = dHist[i];
		}
		else
		{
			datmp[i] = datmp[i - 1] + dHist[i];
		}

	}

	BYTE bytGray;
	int nReTran;
	for(int i=0;i<GetHeight();i++)
	{
		for(int j=0;j<GetWidthPixel();j++)
		{
			bytGray = GetGray(j,i);

			nReTran = dTran[(int)(255 * datmp[bytGray])];

			SetPixel(j,i,RGB(nReTran,nReTran,nReTran));
		}
	}
	return true;
}

bool CImageBmp::SpaceEnhanceWithTemplate(int TmpltW, int TmpltH, int TmpltMW, int TmpltMH, float* pfTemp, double fCoef)
{
	if (!IsValidate())
		return false;
	if (m_pBMIH->biBitCount != 8)
		return false;

	IndexToGray();

	CImageBmp ibRe;
	ibRe.InitPixelWithImage(this);

	for(int i=TmpltMH;i<GetHeight() - (TmpltH - TmpltMH) + 1;i++)
	{
		for(int j=TmpltMW;j<GetWidthPixel() - (TmpltW - TmpltMW) + 1;j++)
		{
			float fRe = 0.0;
			for(int ii = 0;ii<TmpltH;ii++)
			{
				for(int ij = 0;ij < TmpltW;ij++)
				{
					fRe += GetGray(j + ij - TmpltMW,i + ii - TmpltMH) * pfTemp[ii*TmpltW + ij];
				}
			}

			fRe *= fCoef;
			fRe = (float) fabs(fRe);

			BYTE byte;
			if (fRe > 255)
				byte = 255;
			else
				byte = fRe + 0.5;

			ibRe.SetPixel(j,i,RGB(byte,byte,byte));
		}
	}

	this->ModalFrom(ibRe);

	return false;
}

bool CImageBmp::InitPixel(int nWidth, int nHeight, BYTE bytColor)
{
	CleanEnv();
	
	m_pBMIH = (BITMAPINFOHEADER *) new BYTE[sizeof(BITMAPINFOHEADER)];
	m_pColorTable = NULL;
	m_nColorEnrties = 0;

	memset(m_pBMIH,0,sizeof(BITMAPINFOHEADER));

	m_pBMIH->biBitCount = 8;
	m_pBMIH->biHeight = nHeight;
	m_pBMIH->biWidth = nWidth;

	m_pData = new LPBYTE[nHeight];

	int nWidthBytes = WIDTHBYTES(m_pBMIH->biBitCount * m_pBMIH->biWidth);
	for(int i=0;i<nHeight;i++)
	{
		m_pData[i] = new BYTE[nWidthBytes];
		memset(m_pData[i],bytColor,nWidthBytes);
	}

	return true;
}

bool CImageBmp::SpaceEnhanceAveSmooth(void)
{
	return this->SpaceEnhanceWithTemplate(3,3,1,1,Template_Smooth_Avg,(float)(1.0/9.0));
}


bool CImageBmp::SpaceEnhanceGaussSmooth(void)
{
	return this->SpaceEnhanceWithTemplate(3,3,1,1,Template_Smooth_Gauss,(float)(1.0/16.0));
}

bool CImageBmp::SpaceEnhaceAdapterSmooth(BYTE nStd)
{
	int TmpltW = 3;
	int TmpltH = 3;
	int TmpltMW = 1;
	int TmpltMH = 1;
	float *pfTemp = Template_Smooth_Gauss;
	float fCoef = 1/16.0;

	if (!IsValidate())
		return false;
	if (m_pBMIH->biBitCount != 8)
		return false;

	IndexToGray();

	CImageBmp ibRe;
	ibRe.InitPixelWithImage(this);

	for(int i=TmpltMH;i<GetHeight() - (TmpltH - TmpltMH) + 1;i++)
	{
		for(int j=TmpltMW;j<GetWidthPixel() - (TmpltW - TmpltMW) + 1;j++)
		{
			BYTE nMax = 0;
			BYTE nMin = 255;

			float fRe = 0.0;
			for(int ii = 0;ii<TmpltH;ii++)
			{
				for(int ij = 0;ij < TmpltW;ij++)
				{
					BYTE bytGray = GetGray(j + ij - TmpltMW,i + ii - TmpltMH);
					fRe += bytGray* pfTemp[ii*TmpltW + ij];
					if (nMax < bytGray)
						nMax = bytGray;
					if (nMin > bytGray)
						nMin = bytGray;
				}
			}

			if (nMax - nMin <= nStd)
			{
				ibRe.SetPixel(j,i,GetGray(j,i));
				continue;
			}

			fRe *= fCoef;
			fRe = (float) fabs(fRe);

			BYTE byte;
			if (fRe > 255)
				byte = 255;
			else
				byte = fRe + 0.5;

			ibRe.SetPixel(j,i,RGB(byte,byte,byte));
		}
	}

	this->ModalFrom(ibRe);

	return true;
}

bool CImageBmp::SpaceEnhanceMidianFilter(int TmpltW, int TmpltH, int TmpltMW, int TmpltMH,int nOrder)
{
	if (!IsValidate())
		return false;
	if (m_pBMIH->biBitCount != 8)
		return false;

	IndexToGray();

	CImageBmp ibRe;
	ibRe.InitPixelWithImage(this);

	BYTE abt[9];

	for(int i=TmpltMH;i<GetHeight() - (TmpltH - TmpltMH) + 1;i++)
	{
		for(int j=TmpltMW;j<GetWidthPixel() - (TmpltW - TmpltMW) + 1;j++)
		{
			int nk = 0;
			for(int ii = 0;ii<TmpltH;ii++)
			{
				for(int ij = 0;ij < TmpltW;ij++)
				{
					BYTE bytGray = GetGray(j + ij - TmpltMW,i + ii - TmpltMH);
					abt[nk] = bytGray;
					nk++;
				}
			}

			BYTE byte = GetMid(abt,9,nOrder);

			ibRe.SetPixel(j,i,RGB(byte,byte,byte));
		}
	}

	this->ModalFrom(ibRe);

}

bool CImageBmp::InitPixelWithImage(CImageBmp* ib)
{
	CleanEnv();

	m_pBMIH = (BITMAPINFOHEADER *) new BYTE[sizeof(BITMAPINFOHEADER) + ib->m_nColorEnrties * 4];
	m_pColorTable = m_pBMIH + 1;
	m_nColorEnrties = ib->m_nColorEnrties;

	memset(m_pBMIH,0,sizeof(BITMAPINFOHEADER) + 4 * ib->m_nColorEnrties);
	memcpy(m_pBMIH,ib->m_pBMIH,sizeof(BITMAPINFOHEADER) + 4 * ib->m_nColorEnrties);

	m_pData = new LPBYTE[ib->m_pBMIH->biHeight];
	
	int nWidthBytes = WIDTHBYTES(m_pBMIH->biBitCount * m_pBMIH->biWidth);
	for(int i=0;i<ib->m_pBMIH->biHeight;i++)
	{
		m_pData[i] = new BYTE[nWidthBytes];
		memset(m_pData[i],0,nWidthBytes);
	}

	return true;
}

BYTE CImageBmp::GetMid(BYTE* btArray, int n,int order)
{
	if (n <= 0)
		return 0;

	if (order <0 || order > n)
		return 0;

	if (btArray == NULL)
		return -1;

	for(int i=0;i<n;i++)
	{
		for(int j=i+1;j<n;j++)
		{
			if (btArray[i] <= btArray[j])
			{
				;
			}
			else
			{
				BYTE btTmp = btArray[i];
				btArray[i] = btArray[j];
				btArray[j] = btTmp;
			}
		}
	}

	if (order == 0)
	{
		if (n%2== 1)
		{
			return btArray[n/2 + 1];
		}
		else
		{
			return (btArray[n/2 + 1] + btArray[n/2]) / 2;
		}
	}
	else
	{
		return btArray[order - 1];
	}
	
}

bool CImageBmp::SpaceEnhanceSobel(void)
{
	this->SpaceEnhanceWithTemplate(3,3,1,1,Template_HSobel,1);
	this->SpaceEnhanceWithTemplate(3,3,1,1,Template_VSobel,1);

	return true;
}

bool CImageBmp::SpaceEnhanceEoG(void)
{
	this->SpaceEnhanceWithTemplate(3,3,1,1,Template_LoG_Small,1);

	return false;
}

bool CImageBmp::SpaceEnhanceLaplace(void)
{
	this->SpaceEnhanceWithTemplate(3,3,1,1,Template_Laplace,1);

	return false;
}

bool CImageBmp::SpaceEnhanceEoGBig(void)
{
	this->SpaceEnhanceWithTemplate(5,5,2,2,Template_LoG_Small,1);

	return false;
}

bool CImageBmp::FFT(complex<double> * TD, complex<double> * FD, int r)
{
	complex<double> *W,*X1,*X2,*X;
	int count = 1<<r;

	W = new complex<double>[count/2];
	X1 = new complex<double>[count];
	X2 = new complex<double>[count];

	for(int i = 0;i<count/2;i++)
	{
		double angle = -i * PI *2 /count;
		W[i] = complex<double>(cos(angle),sin(angle));
	}

	for(int i=0;i<count;i++)
	{
		X1[i] = TD[i];
	}
	memcpy(X1,TD,sizeof(complex<double>) * count);

	for(int k = 0;k < r;k++)
	{
		for(int j =0;j<(1<<k);j++)
		{
			int bfsize = 1 << (r-k);
			for(int i = 0;i<bfsize /2 ;i++)
			{
				int p = j * bfsize;
				X2[i + p] = X1[i + p] + X1[i + p + bfsize/2];
				X2[i + p +bfsize/2] = (X1[i + p] - X1[i + p + bfsize/2])* W[i * (1<<k)];
			}
		}

		X = X1;
		X1 = X2;
		X2 = X;
	}

	for(int j=0;j < count;j++)
	{
		int p = 0;
		for(int i = 0;i <r ;i++)
		{
			if (j & (1 << i))
			{
				p += 1 << (r - i -1);
			}
		}

		FD[j] = X1[p];
	}

	delete W;
	delete X1;
	delete X2;

	return true;
}

bool CImageBmp::IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	complex<double> *X;
	int count = 1 << r;
	X = new complex<double>[count];

	memcpy(X,FD,sizeof(complex<double>) * count);

	for(int i=0;i<count;i++)
	{
		X[i] = complex<double>(X[i].real(),-X[i].imag());
	}

	FFT(X,TD,r);

	for(int i=0;i<count;i++)
	{
		TD[i] = complex<double>( TD[i].real() / count , - TD[i].imag() / count );
	}

	delete X;

	return true;
}

bool CImageBmp::FFTin2D(bool bExpand, complex<double>* out,BYTE byFillColor,int *tw,int *th,int *orgx,int *orgy)
{
	if (orgx != NULL)
		*orgx = GetWidthPixel();

	if (orgy != NULL)
		*orgy  = GetHeight();

	if (!IsValidate())
		return false;

	long w,h,wp,hp;

	w = 1;
	h = 1;
	wp = 0;
	hp = 0;

	while(2 * w <= GetWidthPixel())
	{
		w *= 2;
		wp ++;
	}

	while(2 * h <= GetHeight())
	{
		h *= 2;
		hp ++;
	}

	if ( bExpand && w != GetWidthPixel() && h != GetHeight() )
	{
		w *= 2; wp ++;
		h *= 2; hp++;
	}

	complex<double> *TD = new complex<double>[w * h];
	complex<double> *FD = new complex<double>[w * h];

	if (tw != NULL)
		*tw = w;
	if (th != NULL)
		*th = h;

	for(int i=0;i<h;i++)
	{
		for(int j =0;j<w;j++)
		{
			if (bExpand)
			{
				if ((j < GetWidthPixel()) && (i < GetHeight()))
				{
					TD[j + w*i] = complex<double>(GetGray(j,i),0);
				}
				else
				{
					TD[j + w*i] = complex<double>(byFillColor,0);
				}
			}
			else
			{
				TD[j + w*i] = complex<double>(GetGray(j,i),0);
			}
		}
	}

	for(int i=0;i<h;i++)
	{
		FFT(&TD[w*i],&FD[w*i],wp);
	}

	for(int i = 0;i<h;i++)
	{
		for(int j =0;j<w;j++)
		{
			TD[i + h*j] = FD[j + w*i];
		}
	}

	for(int i=0;i < w;i++)
	{
		FFT(&TD[i*h],&FD[i * h],hp);
	}

	if (out)
	{
		for(int i = 0;i < h;i ++)
		{
			for(int j=0;j < w;j++)
			{
				out[i*w + j] = FD[j * h + i];
			}
		}
	}

	this->Resize(w,h,255);

	double dMax = 0;
	double dMin = 1E+010;

	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			double dTemp = sqrt( FD[j * h + i].real() * FD[j * h + i].real() + FD[j * h + i].imag() * FD[j*h + i].imag()) / 100;
		
			dTemp = log(1 + dTemp);

			if (dMax < dTemp)
			{
				dMax = dTemp;
			}
			
			if (dMin > dTemp)
			{
				dMin = dTemp;
			}
		}
	}

	for(int i = 0;i < h;i++)
	{
		for(int j=0;j < w;j++)
		{
			double dTemp = sqrt( FD[j * h + i].real() * FD[j * h + i].real() + FD[j * h + i].imag() * FD[j*h + i].imag()) / 100;

			dTemp = log(1 + dTemp);

			dTemp = (dTemp - dMin) / (dMax - dMin) * 255;

			this->SetPixel((j<w/2?j+w/2:j-w/2),(i < h/2?i + h/2:i - h/2),RGB(dTemp,dTemp,dTemp));
		}
	}
	
	delete TD;
	delete FD;

	return true;
}

bool CImageBmp::Resize(int w, int h, BYTE byColor)
{
	if (!IsValidate())
		return false;

	for(int i=0;i<m_pBMIH->biHeight;i++)
	{
		delete m_pData[i];
	}

	delete m_pData;

	this->m_pBMIH->biHeight = h;
	this->m_pBMIH->biWidth = w;

	int nWidthBytes = WIDTHBYTES(m_pBMIH->biWidth * m_pBMIH->biBitCount);

	m_pData = new LPBYTE[h];

	for(int i=0;i<h;i++)
	{
		m_pData[i] = new BYTE[nWidthBytes];
		memset(m_pData[i],byColor,nWidthBytes);
	}

	return true;
}

bool CImageBmp::IFFin2D(complex<double>* in, long lWidth, long lHeight, long loutW, long loutH)
{
	long w = 1;
	long h = 1;
	int wp = 0;
	int hp = 0;

	if (loutH == 0)
		loutH = lHeight;
	if (loutW == 0)
		loutW = lWidth;

	while(2 * w <= lWidth)
	{
		w *= 2;
		wp++;
	}

	while(2 * h <= lHeight)
	{
		h *= 2;
		hp ++;
	}

	complex<double> *TD = new complex<double> [w * h];
	complex<double> *FD = new complex<double>[w * h];
	this->Resize(loutW,loutH,255);

	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			FD[j + w*i] = in[j + w*i];
		}
	}

	for(int i=0;i<h;i++)
	{
		IFFT(&FD[w * i],&TD[w * i],wp);
	}

	for(int i = 0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			FD[i + h*j] = TD[j + w*i];
		}
	}

	for(int i=0;i<w;i++)
	{
		IFFT(&FD[i*h],&TD[i*h],hp);
	}

	double dMax = 0;
	double dMin = 1E+010;


	for(int i=0;i<loutH;i++)
	{
		for(int j=0;j<loutW;j++)
		{
			double dTemp = sqrt( TD[j * h + i].real() * TD[j * h + i].real() + TD[j * h + i].imag() * TD[j*h + i].imag());

			if (dMax < dTemp)
			{
				dMax = dTemp;
			}

			if (dMin > dTemp)
			{
				dMin = dTemp;
			}
		}
	}

	for(int i = 0;i < loutH;i++)
	{
		for(int j=0;j < loutW;j++)
		{
			double dTemp = sqrt( TD[j * h + i].real() * TD[j * h + i].real() + TD[j * h + i].imag() * TD[j*h + i].imag()) ;

			dTemp = (dTemp - dMin) / (dMax - dMin) * 255;

			this->SetPixel(j,i,RGB(dTemp,dTemp,dTemp));
		}
	}

	delete TD;
	delete FD;

	return true;
}

bool CImageBmp::GetFFTin2D(complex<double>* out, bool bExpand,BYTE byFillColor,long *tw,long* th, long* orgx, long* orgy)
{
	if (orgx != NULL)
		*orgx = GetWidthPixel();
	if (orgy != NULL)
		*orgy  = GetHeight();

	if (!IsValidate())
		return false;

	long w,h,wp,hp;

	w = 1;
	h = 1;
	wp = 0;
	hp = 0;

	while(2 * w <= GetWidthPixel())
	{
		w *= 2;
		wp ++;
	}

	while(2 * h <= GetHeight())
	{
		h *= 2;
		hp ++;
	}

	if ( bExpand && w != GetWidthPixel() && h != GetHeight() )
	{
		w *= 2; wp ++;
		h *= 2; hp++;
	}

	complex<double> *TD = new complex<double>[w * h];
	complex<double> *FD = new complex<double>[w * h];

	if (tw != NULL)
		*tw = w;
	if (th != NULL)
		*th = h;

	for(int i=0;i<h;i++)
	{
		for(int j =0;j<w;j++)
		{
			if (bExpand)
			{
				if ((j < GetWidthPixel()) && (i < GetHeight()))
				{
					TD[j + w*i] = complex<double>(GetGray(j,i),0);
				}
				else
				{
					TD[j + w*i] = complex<double>(byFillColor,0);
				}
			}
			else
			{
				TD[j + w*i] = complex<double>(GetGray(j,i),0);
			}
		}
	}

	for(int i=0;i<h;i++)
	{
		FFT(&TD[w*i],&FD[w*i],wp);
	}

	for(int i = 0;i<h;i++)
	{
		for(int j =0;j<w;j++)
		{
			TD[i + h*j] = FD[j + w*i];
		}
	}

	for(int i=0;i < w;i++)
	{
		FFT(&TD[i*h],&FD[i * h],hp);
	}

	if (out)
	{
		for(int i = 0;i < h;i ++)
		{
			for(int j=0;j < w;j++)
			{
				out[i*w + j] = FD[j * h + i];
			}
		}
	}

	return false;
}

long CImageBmp::GetFreqWitdh(bool bExpand)
{
	long w = 1;

	while( 2 * w <= GetWidthPixel())
	{
		w *= 2;
	}

	if ( w!=GetWidthPixel() && bExpand)
		w *= 2;

	return w;
}

long CImageBmp::GetFreqHeight(bool bExpand)
{
	long h = 1;
	while( 2 * h <= GetHeight())
		h *= 2;

	if ( h != GetHeight() && bExpand)
		h *= 2;

	return h;
}

bool CImageBmp::FreqEnhanceFilt(double* dFilter, BYTE byFillColor)
{
	if (!IsValidate())
		return true;

	long w = GetFreqWitdh();
	long h = GetFreqHeight();
	long ow;
	long oh;

	complex<double>* adFreqImg = new complex<double>[w*h];

	GetFFTin2D(adFreqImg,true,255,NULL,NULL,&ow,&oh);

	for(long i = 0;i< w*h;i++)
	{
		adFreqImg[i] = adFreqImg[i] * dFilter[i];
	}

	IFFin2D(adFreqImg,w,h,ow,oh);

	return true;
}

bool CImageBmp::TemplateIdealLPF(double* dFilter, int nFreq)
{
	long w = GetFreqWitdh();
	long h =GetFreqHeight();

	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			if (sqrt( ((double)(i - h/2)) * ( i - h/2) + ((double)(j - w/2))*(j - w/2) ) > nFreq)
			{
				dFilter[(i < h/2 ? i + h/2 : i -h/2) * w + (j < w/2?j+w/2:j-w/2)] = 0;
			}
			else
			{
				dFilter[(i < h/2 ? i + h/2 : i -h/2) * w + (j < w/2?j+w/2:j-w/2)] = 1;
			}
		}
	}

	return true;
}

bool CImageBmp::FreqEnhanceIdealLPF(double nFreq)
{
	long w = GetFreqWitdh();
	long h = GetFreqHeight();

	double *adFilter = new double[w * h];

	TemplateIdealLPF(adFilter,nFreq);

	FreqEnhanceFilt(adFilter,255);

	delete adFilter;
	
	return true;
}

bool CImageBmp::TemplateGaussLPF(double* adFilter, double dSigma)
{
	long w = GetFreqWitdh();
	long h =GetFreqHeight();

	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			adFilter[(i < h/2 ? i + h/2 : i -h/2) * w + (j < w/2?j+w/2:j-w/2)] = exp( -( pow((double)(i - h/2),2) + pow((double)(j - w/2),2) ) / 2 / pow(dSigma,2) );
		}
	}

	return true;
}

bool CImageBmp::FreqEnhanceGaussLPF(double dSigma)
{
	long w = GetFreqWitdh();
	long h = GetFreqHeight();

	double *adFilter = new double[w * h];

	TemplateGaussLPF(adFilter,dSigma);

	FreqEnhanceFilt(adFilter,255);

	delete adFilter;

	return true;
}

bool CImageBmp::TemplateGaussHPF(double* adFilter, double dSigma)
{
	long w = GetFreqWitdh();
	long h =GetFreqHeight();

	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			adFilter[(i < h/2 ? i + h/2 : i -h/2) * w + (j < w/2?j+w/2:j-w/2)] = 1 - exp( -( pow((double)(i - h/2),2) + pow((double)(j - w/2),2) ) / 2 / pow(dSigma,2) );
		}
	}

	return true;
}

bool CImageBmp::FreqEnhanceGaussHPF(double dSigma)
{
	long w = GetFreqWitdh();
	long h = GetFreqHeight();

	double *adFilter = new double[w * h];

	TemplateGaussHPF(adFilter,dSigma);

	FreqEnhanceFilt(adFilter,255);

	delete adFilter;

	return true;
}

bool CImageBmp::TemplateLaplaceHPF(double* adFilter)
{
	long w = GetFreqWitdh();
	long h =GetFreqHeight();

	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			adFilter[(i < h/2 ? i + h/2 : i -h/2) * w + (j < w/2?j+w/2:j-w/2)] = - (i < h/2 ? i + h/2 : i -h/2)*(i < h/2 ? i + h/2 : i -h/2) - (j < w/2?j+w/2:j-w/2)*(j < w/2?j+w/2:j-w/2);
		}
	}

	return true;
}

bool CImageBmp::Erode(int  se[3][3])
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	CImageBmp ib;
	ib.InitPixelWithImageAndColor(this,255);

	for(int i=1;i<nHeight - 1;i++)
	{
		for(int j=1;j<nWidth - 1;j++)
		{
			bool bMatch = true;
			for(int k=0;k<3;k++)
			{
				for(int l=0;l<3;l++)
				{
					if (se[k][l] == -1)
						continue;
					if (se[k][l] == 1)
					{
						if (GetGray(j-1+l,i-1+k) != 0)
						{
							bMatch = false;
							break;
						}
					}
					else if (se[k][l] == 0)
					{
						if (GetGray(j-1+l,i-1+k) != 255)
						{
							bMatch = false;
							break;
						}
					}
					else
					{
						return false;
					}
				}
		
			}

			if (bMatch)
				ib.SetPixel(j,i,RGB(0,0,0));
			else
				ib.SetPixel(j,i,RGB(255,255,255));
		}
	}

	this->ModalFrom(ib);

	return true;
}

bool CImageBmp::FreqEnhanceLaplaceHPF(void)
{
	long w = GetFreqWitdh();
	long h = GetFreqHeight();

	double *adFilter = new double[w * h];

	TemplateLaplaceHPF(adFilter);

	FreqEnhanceFilt(adFilter,255);

	delete adFilter;

	return true;
}

bool CImageBmp::Dilate(int  se[3][3])
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int nTemp;
	for(int i=0;i<2;i++)
	{
		for(int j=0;j<3-i;j++)
		{
			nTemp = se[i][j];
			se[i][j] = se[2-i][2-j];
			se[2-i][2-j] = nTemp;
		}
	}

	CImageBmp ib;
	ib.InitPixelWithImageAndColor(this,255);

	bool bMatch;
	for(int i=1;i<nHeight - 1;i++)
	{
		for(int j=1;j<nWidth - 1;j++)
		{
			bMatch = false;

			for(int k=0;k<3;k++)
			{
				for(int l=0;l<3;l++)
				{
					if (se[k][l] == -1)
						continue;

					if (se[k][l] == 1)
					{
						if (GetGray(j-1+l,i-1+k) == 0)
						{
							ib.SetPixel(j,i,RGB(0,0,0));
							bMatch = true;
							break;
						}
					}
					else
					{
						return false;
					}
				}
			}

			if (!bMatch)
				ib.SetPixel(j,i,RGB(255,255,255));
		}
	}

	this->ModalFrom(ib);

	return true;
}

bool CImageBmp::Open(int  se[3][3])
{
	this->Erode(se);
	this->Dilate(se);

	return true;
}

bool CImageBmp::Close(int se[3][3])
{
	this->Dilate(se);
	this->Erode(se);

	return true;
}

bool CImageBmp::TraceBoundary(void)
{
	CImageBmp ib;
	ib.InitPixelWithImage(this);

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int Direction[8][2] =  { {-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0} };
	int nBeginDirection;
	int nCurDirection = 0;
	int posX,posY;
	bool bAtStartPt;

	POINT ptStart = {2,2};
	POINT ptCur = {2,2};

	for(int i=0; i<nHeight ;i++)
	{
		ib.SetPixel(0,i,RGB(255,255,255));
		ib.SetPixel(nWidth - 1,i,RGB(255,255,255));
	}

	for(int j=0;j<nWidth;j++)
	{
		ib.SetPixel(j,0,RGB(255,255,255));
		ib.SetPixel(j,nHeight - 1,RGB(255,255,255));
	}
	
	for(int i=1;i<nHeight;i++)
	{
		for(int j=1;j<nWidth;j++)
		{
			if (GetGray(j,i) == 0)
			{
				ptStart.x = j;
				ptStart.y = i;

				ptCur = ptStart;
				bAtStartPt = true;

				while( ( (ptCur.x != ptStart.x) || (ptStart.y!= ptCur.x) ) || bAtStartPt)
				{
					bAtStartPt = false;

					posX = ptCur.x + Direction[nCurDirection][0];
					posY = ptCur.y + Direction[nCurDirection][1];

					int nSearchTimes = 1;
					while( GetGray(posX,posY) == 255 )
					{
						nCurDirection ++ ;
						if (nCurDirection >=8)
							nCurDirection -= 8;

						posX = ptCur.x + Direction[nCurDirection][0];
						posY = ptCur.y + Direction[nCurDirection][1];

						nSearchTimes++;
						if (nSearchTimes >= 8)
						{
							posX = ptCur.x;
							posY = ptCur.y;
							break;
						}
					}

					ptCur.x = posX;
					ptCur.y = posY;
				
					ib.SetPixel(ptCur.x,ptCur.y,RGB(0,0,0));

					//

					nCurDirection -= 2;
					if (nCurDirection <0)
						nCurDirection += 8;
				}

				this->ModalFrom(ib);

				return true;
			} 


		}
	}

	return true;
}

bool CImageBmp::intersect(CImageBmp* ib)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	CImageBmp ibt;
	ibt.InitPixelWithImage(this);

	if (!IsValidate())
		return false;

	if (!ib->IsValidate())
		return false;

	if (ib->GetHeight() != this->GetHeight() || this->GetWidthPixel() != ib->GetWidthPixel())
	{
		return false;
	}

	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			BYTE pixelB = ib->GetPixel(j,i);
			BYTE pixelA = this->GetPixel(j,i);

			if (pixelA != 0 || pixelB != 0)
			{
				ibt.SetPixel(j,i,RGB(255,255,255));
			}
			else
			{
				ibt.SetPixel(j,i,RGB(0,0,0));
			}
		}
	}

	this->ModalFrom(ibt);

	return true;
}

bool CImageBmp::Union(CImageBmp* ib)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	CImageBmp ibt;
	ibt.InitPixelWithImage(this);

	if (!IsValidate())
		return false;

	if (!ib->IsValidate())
		return false;

	if (ib->GetHeight() != this->GetHeight() || this->GetWidthPixel() != ib->GetWidthPixel())
	{
		return false;
	}

	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			BYTE pixelB = ib->GetPixel(j,i);
			BYTE pixelA = this->GetPixel(j,i);

			if (pixelA != 0 && pixelB != 0)
			{
				ibt.SetPixel(j,i,RGB(255,255,255));
			}
			else
			{
				ibt.SetPixel(j,i,RGB(0,0,0));
			}
		}
	}

	this->ModalFrom(ibt);

	return true;
}

bool CImageBmp::Except(CImageBmp* ib)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	CImageBmp ibt;
	ibt.InitPixelWithImage(this);

	if (!IsValidate())
		return false;

	if (!ib->IsValidate())
		return false;

	if (ib->GetHeight() != this->GetHeight() || this->GetWidthPixel() != ib->GetWidthPixel())
	{
		return false;
	}

	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			BYTE pixelB = ib->GetPixel(j,i);
			BYTE pixelA = this->GetPixel(j,i);

			if (pixelA >= pixelB)
			{
				ibt.SetPixel(j,i,RGB(pixelA - pixelB,pixelA - pixelB,pixelA - pixelB));
			}
			else
			{
				ibt.SetPixel(j,i,RGB(0,0,0));
			}
		}
	}

	this->ModalFrom(ibt);

	return true;
}

bool CImageBmp::FillRegion(int x, int y)
{
	int se[3][3] = { {-1,1,-1},{1,1,1},{-1,1,-1} };

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	CImageBmp ibRev;
	ibRev.InitPixelWithImage(this);
	ibRev.ModalFrom(*this);
	ibRev.Reverse();

	CImageBmp ibTo;
	ibTo.InitPixelWithImageAndColor(this,255);
	ibTo.SetPixel(x,y,RGB(0,0,0));

	CImageBmp ibPreTo;
	ibPreTo.ModalFrom(ibTo);

	while(true)
	{
		ibTo.Dilate(se);
		ibTo.intersect(&ibRev);

		if (ibTo.Equal(&ibPreTo))
			break;

		ibPreTo.ModalFrom(ibTo);
	}

	this->ModalFrom(ibTo);

	return true;
}

bool CImageBmp::Reverse(void)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			BYTE pixel = GetPixel(j,i);
			SetPixel(j,i,RGB(255 - pixel,255 - pixel,255-pixel));
		}
	}

	return true;
}

bool CImageBmp::Equal(CImageBmp* ib)
{
	if (!this->IsValidate())
		return false;
	if (!ib->IsValidate())
		return false;

	if (this->GetWidthPixel() != ib->GetWidthPixel())
	{
		return false;
	}

	if (this->GetHeight() != ib->GetHeight())
	{
		return false;
	}

	int nWidth = GetWidthPixel();
	int nHeight = GetHeight();

	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			if (ib->GetPixel(j,i) != this->GetPixel(j,i))
			{
				return false;
			}
		}
	}

	return true;
}

bool CImageBmp::InitPixelWithImageAndColor(CImageBmp* ib, BYTE color)
{
	CleanEnv();

	m_pBMIH = (BITMAPINFOHEADER *) new BYTE[sizeof(BITMAPINFOHEADER) + ib->m_nColorEnrties * 4];
	m_pColorTable = m_pBMIH + 1;
	m_nColorEnrties = ib->m_nColorEnrties;

	memset(m_pBMIH,0,sizeof(BITMAPINFOHEADER) + 4 * ib->m_nColorEnrties);
	memcpy(m_pBMIH,ib->m_pBMIH,sizeof(BITMAPINFOHEADER) + 4 * ib->m_nColorEnrties);

	m_pData = new LPBYTE[ib->m_pBMIH->biHeight];

	int nWidthBytes = WIDTHBYTES(m_pBMIH->biBitCount * m_pBMIH->biWidth);
	for(int i=0;i<ib->m_pBMIH->biHeight;i++)
	{
		m_pData[i] = new BYTE[nWidthBytes];
		memset(m_pData[i],color,nWidthBytes);
	}

	return true;
}

bool CImageBmp::DebugTry(CImageBmp* bi, int nSleep)
{
	static int times = 0;
	times ++;

	CString strShow;
	strShow.Format(L"调试演示：%d.",times);

	CMainFrame * mf = (CMainFrame*)AfxGetMainWnd();
	CDC *dcView = mf->GetActiveView()->GetDC();

	bi->Draw(dcView);
	dcView->TextOut(0,0,strShow);

	Sleep(nSleep);
	
	return true;
}

bool CImageBmp::LabelConnectionRegion(int nConn,int nMin)
{
	int se[3][3] = { {1,1,1},{1,1,1},{1,1,1} };

	if (nConn == 4)
	{
		se[0][0] = -1;
		se[0][2] = -1;
		se[2][0] = -1;
		se[2][2] = -1;
	}

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	CImageBmp ibOrg;
	ibOrg.ModalFrom(*this);

	int nConnRgn = 1;
	int nGray ;

	for(int i=0;i<nHeight;i++)
	{
		SetPixel(0,i,RGB(255,255,255));
		SetPixel(nWidth - 1,i,RGB(255,255,255));
	}

	for(int j=0;j<nWidth;j++)
	{
		SetPixel(j,0,RGB(255,255,255));
		SetPixel(j,nHeight - 1,RGB(255,255,255));
	}

	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			nGray = GetGray(j,i);

			if (nGray == 0)
			{
				CImageBmp ibTo;
				CImageBmp ibTemp;

				ibTo.InitPixelWithImageAndColor(this,255);
				ibTo.SetPixel(j,i,RGB(0,0,0));
				ibTemp.ModalFrom(ibTo);

				while(true)
				{
					ibTo.Dilate(se);
					ibTo.intersect(&ibOrg);

					if (ibTo.Equal(&ibTemp))
						break;

					ibTemp.ModalFrom(ibTo);
				}

				if (ibTo.GetPixelCount(0,0) <= nMin)
					continue;

				for(int k=0;k<nHeight;k++)
				{
					for(int l=0;l<nWidth;l++)
					{
						nGray = ibTo.GetPixel(l,k);
						if (nGray == 0)
						{
							SetPixel(l,k,RGB(nConnRgn,nConnRgn,nConnRgn));
						}
					}
				}

				nConnRgn ++;
				if (nConnRgn > 255)
					nConnRgn = 1;
			}
		}
	}

	return true;
}

bool CImageBmp::Thining(void)
{
	if (!IsValidate())
		return false;

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	bool b1;
	bool b2;
	bool b3;
	bool b4;

	BYTE neighbour[5][5];

	CImageBmp ibPic;

	bool bModified = true;
	while(bModified)
	{
		bModified = false;	
		ibPic.ModalFrom(*this);

		for(int j=2;j<nHeight - 2;j++)
		{
			for(int i=2;i<nWidth - 2;i++)
			{
				b1 = false;
				b2 = false;
				b3 = false;
				b4 = false;

				BYTE data = GetGray(i,j);
				if (data == 255)
					continue;

				for(int m=0;m<5;m++)
				{
					for(int n=0;n<5;n++)
					{
						neighbour[m][n] = (GetGray(i+n-2,j+m-2) == 0);
					}
				}
				
				int nCount = neighbour[1][1] + neighbour[1][2] + neighbour[1][3] +neighbour[2][1]+neighbour[2][3]+neighbour[3][1]+neighbour[3][2]+neighbour[3][3];
				
				if (nCount >= 2 && nCount <=6)
					b1 = true;

				nCount = 0;
				if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
					nCount++;
				if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
					nCount++;
				if (neighbour[2][1] == 0 && neighbour[3][1] == 1)
					nCount++;
				if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
					nCount++;
				if (neighbour[3][2] == 0 && neighbour[3][3] == 1)
					nCount++;
				if (neighbour[3][3] == 0 && neighbour[2][3] == 1)
					nCount++;
				if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
					nCount++;
				if (neighbour[1][3] == 0 && neighbour[1][2] == 1)
					nCount++;
				if (nCount == 1)
					b2 = true;

				if (neighbour[1][2] * neighbour[2][1] * neighbour[2][3] == 0)
				{
					b3 = true;
				}
				else
				{
					nCount = 0;
					if (neighbour[0][2] == 0 && neighbour[0][1] == 1)
						nCount ++;
					if (neighbour[0][1] == 0 && neighbour[1][1] == 1)
						nCount ++;
					if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
						nCount ++;
					if (neighbour[2][1] == 0 && neighbour[2][2] == 1)
						nCount ++;
					if (neighbour[2][2] == 0 && neighbour[2][3] == 1)
						nCount ++;
					if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
						nCount ++;
					if (neighbour[1][3] == 0 && neighbour[0][3] == 1)
						nCount ++;
					if (neighbour[0][3] == 0 && neighbour[0][2] == 1)
						nCount ++;
					if (nCount != 1)
						b3 = true;
				}

				if (neighbour[1][2] * neighbour[2][1] * neighbour[3][2] == 0)
				{
					b4 = true;
				}
				else
				{
					nCount = 0;
					if (neighbour[1][1] == 0 && neighbour[1][0] == 1)
						nCount++;
					if (neighbour[1][0] == 0 && neighbour[2][0] == 1)
						nCount++;
					if (neighbour[2][0] == 0 && neighbour[3][0] == 1)
						nCount++;
					if (neighbour[3][0] == 0 && neighbour[3][1] == 1)
						nCount++;
					if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
						nCount++;
					if (neighbour[3][2] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[1][2] == 1)
						nCount++;
					if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (nCount != 1)
						b4 = true;
				}

				if (b1 && b2 && b3 && b4)
				{
					ibPic.SetPixel(i,j,RGB(255,255,255));
				}
				else
				{
					ibPic.SetPixel(i,j,RGB(0,0,0));
				}
			}
		}

		this->ModalFrom(ibPic);
	}

	//this->ModalFrom(ibPic);
	
	return true;
}

bool CImageBmp::PixelImage(int lowThre, int highThre)
{
	if (!IsValidate())
		return false;
	
	this->LabelConnectionRegion(0);

	double sumX[256];
	double sumY[256];
	double sumPt[256];

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	for(int i=0;i<256;i++)
	{
		sumX[i] = 0;
		sumY[i] = 0;
		sumPt[i] = 0;
	}

	for(int i = 0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			BYTE gray = GetGray(j,i);
			sumX[gray] += j;
			sumY[gray] += i;
			sumPt[gray] ++;
		}
	}

	CImageBmp ib;
	ib.InitPixelWithImageAndColor(this,255);

	for(int i=0;i<256;i++)
	{
		if (sumPt[i] != 0)
		{
			if (sumPt[i] > highThre && sumPt[i] <lowThre)
				break;

			sumX[i] = sumX[i] / sumPt[i];
			sumY[i] = sumY[i] / sumPt[i];

			ib.SetPixel((int)(sumX[i] + 0.5),(int)(sumY[i] + 0.5),RGB(0,0,0));
		}
	}

	this->ModalFrom(ib);

	return true;
}

bool CImageBmp::Convex(bool bConstrain)
{
	int se1[3][3] = { {1,-1,-1},{1,0,-1},{1,-1,-1} };
	int se2[3][3] = { {1,1,1},{-1,0,-1},{-1,-1,-1} };
	int se3[3][3] = { {-1,-1,1},{-1,0,1},{-1,-1,1} };
	int se4[3][3] = { {-1,-1,-1},{-1,0,-1},{1,1,1} };

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	CImageBmp ib;
	ib.InitPixelWithImageAndColor(this,255);

	CImageBmp ibTemp1;
	ibTemp1.ModalFrom(*this);
	ib.ModalFrom(*this);
	while(true)
	{
		ib.Erode(se1);
		ib.Or(&ibTemp1);

		if (ib.Equal(&ibTemp1))
			break;

		ibTemp1.ModalFrom(ib);
	}

	CImageBmp ibTemp2;
	ibTemp2.ModalFrom(*this);
	ib.ModalFrom(*this);
	while(true)
	{
		ib.Erode(se2);
		ib.Or(&ibTemp2);

		if (ib.Equal(&ibTemp2))
			break;

		ibTemp2.ModalFrom(ib);
	}

	CImageBmp ibTemp3;
	ibTemp3.ModalFrom(*this);
	ib.ModalFrom(*this);
	while(true)
	{
		ib.Erode(se3);
		ib.Or(&ibTemp3);

		if (ib.Equal(&ibTemp3))
			break;

		ibTemp3.ModalFrom(ib);
	}

	CImageBmp ibTemp4;
	ibTemp4.ModalFrom(*this);
	ib.ModalFrom(*this);
	while(true)
	{
		ib.Erode(se4);
		ib.Or(&ibTemp4);

		if (ib.Equal(&ibTemp4))
			break;

		ibTemp4.ModalFrom(ib);
	}

	ib.InitPixelWithImageAndColor(this,255);

	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			if ( ibTemp1.GetPixel(j,i) == 0 || ibTemp2.GetPixel(j,i) == 0 || ibTemp3.GetPixel(j,i) == 0 || ibTemp4.GetPixel(j,i) == 0)
			{
				ib.SetPixel(j,i,RGB(0,0,0));
			}
		}
	}

	int nTop = nHeight;
	int nBottom = 0;
	int nLeft = nWidth;
	int nRight = 0;

	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			if ( GetGray(j,i) == 0 )
			{
				if (i < nTop)
					nTop = i;
				if (i > nBottom)
					nBottom = i;
				if (j < nLeft)
					nLeft = j;
				if (j > nRight)
					nRight = j;
			}
		}
	}

	if (bConstrain)
	{
		for(int i=0;i<nHeight;i++)
		{
			for(int j=0;j<nWidth;j++)
			{
				if (i < nTop || i >nBottom || j <nLeft || j>nRight)
				{
					ib.SetPixel(j,i,RGB(255,255,255));
				}
			}
		}
	}

	this->ModalFrom(ib);

	return true;
}

bool CImageBmp::Or(CImageBmp* ib)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	CImageBmp ibt;
	ibt.InitPixelWithImage(this);

	if (!IsValidate())
		return false;

	if (!ib->IsValidate())
		return false;

	if (ib->GetHeight() != this->GetHeight() || this->GetWidthPixel() != ib->GetWidthPixel())
	{
		return false;
	}

	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			BYTE pixelB = ib->GetPixel(j,i);
			BYTE pixelA = this->GetPixel(j,i);

			if (pixelA == 0 ||  pixelB == 0)
			{
				ibt.SetPixel(j,i,RGB(0,0,0));
			}
			else
			{
				ibt.SetPixel(j,i,RGB(255,255,255));
			}
		}
	}

	this->ModalFrom(ibt);

	return true;
}

bool CImageBmp::GrayDilate(int nTempW, int nTempH, int nTempMX, int nTempMY, int** se)
{
	if (!IsValidate())
		return false;

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	CImageBmp ib;
	ib.InitPixelWithImageAndColor(this,255);

	for(int i=nTempMY;i<nHeight - nTempH + nTempMY + 1;i++)
	{
		for(int j=nTempMX;j<nWidth - nTempW + nTempMX + 1;j++)
		{
			BYTE maxval = 0;
			for(int k=0;k<nTempH;k++)
			{
				for(int l=0;l<nTempW;l++)
				{
					if (se[k][l] == 1)
					{
						BYTE gray = GetGray(j - nTempMX + l,i - nTempMY + k);

						if (gray > maxval)
						{
							maxval = gray;
						}
					}
				}

				ib.SetPixel(j,i,RGB(maxval,maxval,maxval));	
			}
		}
	}

	this->ModalFrom(ib);

	return true;
}

bool CImageBmp::GrayErode(int nTempW, int nTempH, int nTempMX, int nTempMY, int** se)
{
	if (!IsValidate())
		return false;

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	CImageBmp ib;
	ib.InitPixelWithImageAndColor(this,255);

	for(int i=nTempMY;i<nHeight - nTempH + nTempMY + 1;i++)
	{
		for(int j=nTempMX;j<nWidth - nTempW + nTempMX + 1;j++)
		{
			BYTE maxval = 255;
			for(int k=0;k<nTempH;k++)
			{
				for(int l=0;l<nTempW;l++)
				{
					if (se[k][l] == 1)
					{
						BYTE gray = GetGray(j - nTempMX + l,i - nTempMY + k);

						if (gray < maxval)
						{
							maxval = gray;
						}
					}
				}

				ib.SetPixel(j,i,RGB(maxval,maxval,maxval));	
			}
		}
	}

	this->ModalFrom(ib);

	return true;
}

bool CImageBmp::GrayOpen(int nTempW, int nTempH, int nTempMX, int nTempMY, int** se)
{
	CImageBmp ib;
	ib.InitPixelWithImageAndColor(this,255);
	ib.ModalFrom(*this);

	ib.GrayErode(nTempW,nTempH,nTempMY,nTempMX,se);
	ib.GrayDilate(nTempW,nTempH,nTempMY,nTempMX,se);
	
	this->ModalFrom(ib);

	return true;
}

bool CImageBmp::GrayClose(int nTempW, int nTempH, int nTempMX, int nTempMY, int** se)
{
	CImageBmp ib;
	ib.InitPixelWithImageAndColor(this,255);
	ib.ModalFrom(*this);

	ib.GrayDilate(nTempW,nTempH,nTempMY,nTempMX,se);
	ib.GrayErode(nTempW,nTempH,nTempMY,nTempMX,se);
	
	this->ModalFrom(ib);

	return true;
}

bool CImageBmp::TopHat(int nTempW, int nTempH, int nTempMX, int nTempMY, int** se)
{
	CImageBmp ib;
	ib.InitPixelWithImageAndColor(this,255);
	ib.ModalFrom(*this);

	ib.GrayOpen(nTempW,nTempH,nTempMX,nTempMY,se);

	this->Except(&ib);

	return false;
}

bool CImageBmp::SpaceEnhanceText(void)
{
	CImageBmp ibH;
	ibH.ModalFrom(*this);

	for(int i=1;i<this->GetWidthPixel()-1;i++)
	{
		for(int j=1;j<this->GetHeight()-1;j++)
		{
			int byA = this->GetPixel(i-1,j);
			int byB = this->GetPixel(i,j);
			int byC = this->GetPixel(i+1,j);

			BYTE byRe = abs(byA + byC - 2*byC);

			ibH.SetPixel(i,j,RGB(byRe,byRe,byRe));
		}
	}

	CImageBmp ibV;
	ibV.ModalFrom(*this);

	for(int i=1;i<this->GetWidthPixel()-1;i++)
	{
		for(int j=1;j<this->GetHeight()-1;j++)
		{
			int byA = this->GetPixel(i,j-1);
			int byB = this->GetPixel(i,j);
			int byC = this->GetPixel(i,j+1);

			BYTE byRe = abs(byA + byC - 2*byC);

			ibV.SetPixel(i,j,RGB(byRe,byRe,byRe));
		}
	}

	CImageBmp ibRe;
	ibRe.ModalFrom(*this);

	for(int i=1;i<this->GetWidthPixel();i++)
	{
		for(int j=1;j<this->GetHeight();j++)
		{
			int maxv = ibV.GetPixel(i,j);
			int maxh = ibH.GetPixel(i,j);

			if (maxv > maxh)
			{
				ibRe.SetPixel(i,j,RGB(maxv,maxv,maxv));
			}
			else
			{
				ibRe.SetPixel(i,j,RGB(maxh,maxh,maxh));
			}
		}
	}

	this->ModalFrom(ibRe);

	return true;
}

bool CImageBmp::ExceptBorder(BYTE byColor)
{
	for(int i=0;i<this->GetWidthPixel();i++)
	{
		SetPixel(i,0,RGB(byColor,byColor,byColor));
	}

	for(int i=0;i<this->GetWidthPixel();i++)
	{
		SetPixel(i,GetHeight()-1,RGB(byColor,byColor,byColor));
	}

	for(int j=0;j<this->GetHeight();j++)
	{
		SetPixel(0,j,RGB(byColor,byColor,byColor));
	}

	for(int j=0;j<this->GetHeight();j++)
	{
		SetPixel(GetWidthPixel()-1,j,RGB(byColor,byColor,byColor));
	}

	return false;
}

bool CImageBmp::GetConnectionReginRect(CRect* rect)
{
	for(int i=0;i<256;i++)
	{
		rect[i].top = 0;
		rect[i].bottom = 0;
		rect[i].left = 0;
		rect[i].right = 0;
	}

	for(int i=0;i<GetWidthPixel();i++)
	{
		for(int j=0;j<GetHeight();j++)
		{
			BYTE by = GetPixel(i,j);

			if (rect[by].left == 0)
				rect[by].left = i;

			if (rect[by].right < i)
				rect[by].right = i;
		}
	}

	for(int i=0;i<GetHeight();i++)
	{
		for(int j=0;j<GetWidthPixel();j++)
		{
			BYTE by = GetPixel(j,i);

			if (rect[by].top == 0)
				rect[by].top = i;

			if (rect[by].bottom < i)
				rect[by].bottom = i;
		}
	}

	return true;
}

int CImageBmp::GetPixelCount(BYTE byStart , BYTE byEnd)
{
	int sum = 0;
	
	for(int i=0;i<this->GetWidthPixel();i++)
	{
		for(int j=0;j<this->GetHeight();j++)
		{
			BYTE by = GetPixel(i,j);
			if (by >= byStart && by<=byEnd)
				sum++;
		}
	}

	return sum;
}

void CImageBmp::Part(CImageBmp* bmp, CRect rect)
{
	bmp->ModalFrom(*this);
	bmp->Resize(rect.Width(),rect.Height(),0);
	for(int i=0;i<rect.Width();i++)
	{
		for(int j=0;j<rect.Height();j++)
		{
			bmp->SetPixel(i,j,this->GetPixel(i+rect.left,j+rect.top));
		}
	}
}

void CImageBmp::ColorRect(CRect* rect, COLORREF color)
{
	rect->top = 0;
	rect->bottom = 0;
	rect->left = 0;
	rect->right = 0;

	for(int i=0;i<GetWidthPixel();i++)
	{
		for(int j=0;j<GetHeight();j++)
		{
			int pl = 0;
			COLORREF co = GetPixel(i,j);
			pl += (GetRValue(co) - GetRValue(color))*(GetRValue(co) - GetRValue(color));
			pl += (GetBValue(co) - GetBValue(color))*(GetBValue(co) - GetBValue(color));
			pl += (GetGValue(co) - GetGValue(color))*(GetGValue(co) - GetGValue(color));
		
			if (pl < 5000)
			{
				if (rect->left == 0)
					rect->left = i;
				if (rect->right < i)
					rect->right = i;
			}
		}
	}

	for(int i=0;i<GetHeight();i++)
	{
		for(int j=0;j<GetWidthPixel();j++)
		{
			int pl = 0;
			COLORREF co = GetPixel(j,i);
			pl += (GetRValue(co) - GetRValue(color))*(GetRValue(co) - GetRValue(color));
			pl += (GetBValue(co) - GetBValue(color))*(GetBValue(co) - GetBValue(color));
			pl += (GetGValue(co) - GetGValue(color))*(GetGValue(co) - GetGValue(color));

			if (pl < 5000)
			{
				if (rect->left == 0)
					rect->left = i;
				if (rect->right < i)
					rect->right = i;
			}
		}
	}
}

bool CImageBmp::GetPRect(CRect* rect)
{
	
	return true;
}

bool CImageBmp::GetRealRect(CRect* rect)
{
	rect->top = 0;
	rect->left = 0;
	rect->right = 0;
	rect->bottom = 0;

	for(int i=0;i<GetWidthPixel();i++)
	{
		for(int j=0;j<GetHeight();j++)
		{
			BYTE by = GetGray(i,j);

			if (by != 255)
			{
				if (rect->left == 0)
				{
					rect->left = i;
				}

				if (rect->right < i)
				{
					rect->right = i;
				}
			}
		}
	}

	for(int i=0;i<GetHeight();i++)
	{
		for(int j=0;j<GetWidthPixel();j++)
		{
			BYTE by = GetGray(j,i);

			if (by != 255)
			{
				if (rect->top == 0)
				{
					rect->top = i;
				}

				if (rect->bottom < i)
				{
					rect->bottom = i;
				}
			}
		}
	}

	return true;
}

double CImageBmp::GetLikelyPercent(CImageBmp* img)
{
	CRect rctImg;
	img->GetRealRect(&rctImg);

	CRect rct;
	GetRealRect(&rct);

	int sum = 0;
	for(int i=rct.left;i<rct.right;i++)
	{
		for(int j=rct.top;j<rct.bottom;j++)
		{
			if (j == rct.bottom)
				DebugBreak();

			BYTE byOrg = GetGray(i,j);
			BYTE byImg = img->GetGray(int(float(i-rct.left)/float(rct.Width())*float(rctImg.Width()))+rctImg.left,int(float(j-rct.top)/float(rct.Height())*float(rctImg.Height()))+rctImg.top);

			if (byOrg != 255 && byImg != 255)
			{
				sum ++;
			}

			if (byOrg == 255 && byImg == 255)
			{
				sum ++;
			}
		}
	}

	double re = double(sum)/double(rct.Width()*rct.Height());

	return re;
}

BYTE CImageBmp::GetMidHist(double dMu)
{
	double dHist[256];
	GetHist(dHist,256);
	double dMutiHist[256];

	dMutiHist[0] = dHist[0];
	for(int i=1;i<256;i++)
	{
		dMutiHist[i] = dMutiHist[i-1] + dHist[i];
	}

	double dMid = 1.0;
	int nMid = 0;
	for(int i=1;i<256;i++)
	{
		if (fabs(dMutiHist[i] - dMu) < dMid)
		{
			dMid = fabs(dMutiHist[i] - dMu);
			nMid = i;
		}
	}

	return nMid - 1;
}

bool CImageBmp::GetMinHist(BYTE* aby,int *n)
{
	double dHist[256];
	GetHist(dHist,256);

	double Pre = 0.0;
	double Post;
	*n=0;

	for(int i=1;i<255;i++)
	{		
		int j=i + 1;
		for(j=i + 1;j<256;j++)
		{
			if (dHist[j] > 0.005)
			{
				Post = dHist[j];
				break;
			}
		}

		if (j == 256)
			break;

		if (dHist[i] > 0.005)
		{
			if (dHist[i] < Pre && dHist[i] > Post)
			{
				aby[(*n)++] = i;
			}
			Pre = dHist[i];
		}
		else
		{
			continue;
		}
	}

	return true;
}

void CImageBmp::VProject(int* left, int* right,int n)
{
	bool isFirst = true;
	BYTE byFirst;
	int *nChange = new int[GetWidthPixel()];

	for(int i=0;i<GetWidthPixel();i++)
	{
		nChange[i] = 0;
	}

	for(int i=0;i<GetWidthPixel();i++)
	{
		byFirst = GetGray(i,0);
		for(int j=0;j<GetHeight();j++)
		{
			BYTE byCur = GetGray(i,j);

			if (isFirst == true && byCur == byFirst)
			{
				nChange[i]++;
			}
			
			if (isFirst == false && byCur != byFirst)
			{
				nChange[i]++;
			}

			if (byCur != byFirst)
			{
				isFirst = !isFirst;
			}
		}
	}

	for(int i=5;i<GetWidthPixel()*0.6;i++)
	{
		bool isReal = true;
		for(int j=i;j<i+2;j++)
		{
			if (nChange[j] < n)
			{
				isReal = false;
			}
		}

		if (isReal == true)
		{
			*left = i;
			break;
		}
	}

	for(int i=GetWidthPixel()-6;i>=GetWidthPixel()*0.3;i--)
	{
		bool isReal = true;
		for(int j=i;j>=i-2;j--)
		{
			if (nChange[j] < n)
			{
				isReal = false;
			}
		}

		if (isReal == true)
		{
			*right = i;
			break;
		}
	}

	delete nChange;
}

void CImageBmp::HProject(int* top, int* bottom, int n)
{
	bool isFirst = true;
	BYTE byFirst;
	int *nChange = new int[GetHeight()];

	for(int i=0;i<GetHeight();i++)
	{
		nChange[i] = 0;
	}

	for(int i=0;i<GetHeight();i++)
	{
		byFirst = GetGray(0,i);
		for(int j=0;j<GetWidthPixel();j++)
		{
			BYTE byCur = GetGray(j,i);

			if (isFirst == true && byCur == byFirst)
			{
				nChange[i]++;
			}

			if (isFirst == false && byCur != byFirst)
			{
				nChange[i]++;
			}

			if (byCur != byFirst)
			{
				isFirst = !isFirst;
			}
		}
	}

	for(int i=5;i<GetHeight()*0.6;i++)
	{
		bool isReal = true;
		for(int j=i;j<i+2;j++)
		{
			if (nChange[j] < n)
			{
				isReal = false;
			}
		}

		if (isReal == true)
		{
			*top = i;
			break;
		}
	}

	for(int i=GetHeight()-6;i>=GetHeight()*0.3;i--)
	{
		bool isReal = true;
		for(int j=i;j>=i-2;j--)
		{
			if (nChange[j] < n)
			{
				isReal = false;
			}
		}

		if (isReal == true)
		{
			*bottom = i;
			break;
		}
	}

	delete nChange;
}

void CImageBmp::ConnectionRegin(int n, int nMin)
{
	int nConRgn = 1;

	for(int i=1;i<GetWidthPixel()-1;i++)
	{
		for(int j=1;j<GetHeight();j++)
		{
			if (GetGray(i,j) == 0)
			{
				int s = 0;

				ConnectionReginSet(i,j,n,nConRgn,s);

				if (s < nMin)
				{
					ClearGray(nConRgn);
					continue;
				}

				nConRgn++;
			}
		}
	}

	/*return;*/

	if (n != 4)
	{
		for(int i=1;i<GetWidthPixel()-1;i++)
		{
			for(int j=1;j<GetHeight()-1;j++)
			{
				BYTE byColor = GetGray(i,j);

				if (byColor == 255)
					continue;

				if (GetGray(i-1,j-1) != 255  && GetGray(i-1,j-1)!=byColor)
				{
					ConnectSameRegion(byColor,GetGray(i-1,j-1),byColor);
				}
				
				if (GetGray(i-1,j) != 255 && GetGray(i-1,j)!=byColor)
				{
					ConnectSameRegion(byColor,GetGray(i-1,j),byColor);
				}

				if (GetGray(i-1,j+1) != 255 && GetGray(i-1,j+1)!=byColor)
				{
					ConnectSameRegion(byColor,GetGray(i-1,j+1),byColor);
				}

				if (GetGray(i,j-1) != 255 && GetGray(i,j-1)!=byColor)
				{
					ConnectSameRegion(byColor,GetGray(i,j-1),byColor);
				}

				if (GetGray(i,j+1) != 255 && GetGray(i-1,j+1)!=byColor)
				{
					ConnectSameRegion(byColor,GetGray(i,j+1),byColor);
				}

				if (GetGray(i+1,j-1) != 255 && GetGray(i+1,j-1)!=byColor)
				{
					ConnectSameRegion(byColor,GetGray(i+1,j-1),byColor);
				}

				if (GetGray(i+1,j) != 255 && GetGray(i+1,j)!=byColor)
				{
					ConnectSameRegion(byColor,GetGray(i+1,j),byColor);
				}

				if (GetGray(i+1,j+1) != 255 && GetGray(i+1,j+1)!=byColor)
				{
					ConnectSameRegion(byColor,GetGray(i+1,j+1),byColor);
				}
			}
		}
	}
	else
	{
		for(int i=1;i<GetWidthPixel()-1;i++)
		{
			for(int j=1;j<GetHeight()-1;j++)
			{
				BYTE byColor = GetGray(i,j);

				if (byColor == 255)
					continue;

				if (GetGray(i-1,j) != 255 && GetGray(i-1,j)!=byColor)
				{
					ConnectSameRegion(byColor,GetGray(i-1,j),byColor);
				}

				if (GetGray(i,j-1) != 255 && GetGray(i,j-1)!=byColor)
				{
					ConnectSameRegion(byColor,GetGray(i,j-1),byColor);
				}

				if (GetGray(i,j+1) != 255 && GetGray(i,j+1)!=byColor)
				{
					ConnectSameRegion(byColor,GetGray(i,j+1),byColor);
				}

				if (GetGray(i+1,j) != 255 && GetGray(i+1,j)!=byColor)
				{
					ConnectSameRegion(byColor,GetGray(i+1,j),byColor);
				}
			}
		}
	}
}

void CImageBmp::ConnectionReginSet(int x, int y, int n,int nConReg,int &sum)
{
	sum++;
	SetPixel(x,y,RGB(nConReg,nConReg,nConReg));

	if (sum > 4000)
		return;

	if (n == 4)
	{
		if (GetGray(x-1,y) == 0)
		{
			ConnectionReginSet(x-1,y,n,nConReg,sum);
		}

		if (GetGray(x+1,y) == 0)
		{
			ConnectionReginSet(x+1,y,n,nConReg,sum);
		}

		if (GetGray(x,y-1) == 0)
		{
			ConnectionReginSet(x,y-1,n,nConReg,sum);
		}

		if (GetGray(x,y+1) == 0)
		{
			ConnectionReginSet(x,y+1,n,nConReg,sum);
		}
	}
	else
	{
		if (GetGray(x-1,y) == 0)
		{
			ConnectionReginSet(x-1,y,n,nConReg,sum);
		}

		if (GetGray(x+1,y) == 0)
		{
			ConnectionReginSet(x+1,y,n,nConReg,sum);
		}

		if (GetGray(x,y-1) == 0)
		{
			ConnectionReginSet(x,y-1,n,nConReg,sum);
		}

		if (GetGray(x,y+1) == 0)
		{
			ConnectionReginSet(x,y+1,n,nConReg,sum);
		}

		if (GetGray(x+1,y+1) == 0)
		{
			ConnectionReginSet(x+1,y+1,n,nConReg,sum);
		}

		if (GetGray(x+1,y-1) == 0)
		{
			ConnectionReginSet(x+1,y-1,n,nConReg,sum);
		}

		if (GetGray(x-1,y+1) == 0)
		{
			ConnectionReginSet(x-1,y+1,n,nConReg,sum);
		}

		if (GetGray(x-1,y-1) == 0)
		{
			ConnectionReginSet(x-1,y-1,n,nConReg,sum);
		}
	}

	return;
}

bool CImageBmp::ClearGray(BYTE by)
{
	for(int i=0;i<GetWidthPixel();i++)
	{
		for(int j=0;j<GetHeight();j++)
		{
			if (GetGray(i,j) == by)
			{
				SetPixel(i,j,RGB(255,255,255));
			}
		}
	}

	return true;
}

bool CImageBmp::ConnectSameRegion(BYTE byColorA , BYTE byColorB , BYTE byColorRe)
{
	for(int i=0;i<GetWidthPixel();i++)
	{
		for(int j=0;j<GetHeight();j++)
		{
			if (GetGray(i,j) == byColorA)
				SetPixel(i,j,RGB(byColorRe,byColorRe,byColorRe));
			if (GetGray(i,j) == byColorB)
				SetPixel(i,j,RGB(byColorRe,byColorRe,byColorRe));
		}
	}

	return true;
}

BYTE CImageBmp::GetRectPixel(int x, int y, int rect)
{
	if (GetWidthPixel()%rect != 0)
		return 0;
	if (GetWidthPixel()%rect != 0)
		return 0;

	double sum = 0;
	for(int i=x*rect;i<(x+1)*rect;i++)
	{
		for(int j=y*rect;j<(y+1)*rect;j++)
		{
			sum += GetGray(i,j);
		}
	}

	return (BYTE)(sum/(rect*rect));
}

bool CImageBmp::GetNetTrain(int rect, double* re)
{
	int d = 0;
	for(int i=0;i<GetWidthPixel()/rect;i++)
	{
		for(int j=0;j<GetHeight()/rect;j++)
		{
			re[d] = GetRectPixel(i,j,rect)/255.0;
			d++;
		}
	}

	return true;
}

int CImageBmp::RectStretchAll(CRect size,CRect rect)
{
	CImageBmp ib;
	ib.ModalFrom(*this);

	Resize(size.Width(),size.Height(),255);

	for(int i=0;i<size.Width();i++)
	{
		for(int j=0;j<size.Height();j++)
		{
			BYTE p;
			float x = float(i)/float(size.Width())*float(rect.Width()) + rect.left;
			float y = float(j)/float(size.Height())*float(rect.Height()) + rect.top;
			
			int a = int(x);
			int b = int(y);

			if (a+1>=rect.right)
				x = rect.right - 1;
			if (b+1>=rect.bottom)
				y = rect.bottom -1;
			//if (a>=rect.right)
			//	x = rect.right - 1;
			//if (b>=rect.bottom)
			//	y = rect.bottom - 1;

			BYTE b00 = ib.GetGray(int(x),int(y));
			BYTE b01 = ib.GetGray(int(x),int(y)+1);
			BYTE b10 = ib.GetGray(int(x)+1,int(y));
			BYTE b11 = ib.GetGray(int(x)+1,int(y)+1);

			double re = 0.0;
			double re1 = 0.0;
			double re2 = 0.0;

			re1 = (x-a)*b11 + (a+1-x)*b01;
			re2 = (x-1)*b10 + (a+1-x)*b00;
			re = (b+1-y)*re2 + (y-b)*re1;

			if (re > 255)
				p = 255;
			else if (re < 0)
				p = 0;
			else
				p = BYTE(re);

			SetPixel(i,j,RGB(p,p,p));
		}
	}

	return 0;
}

bool CImageBmp::DetectLine(int min,int max,CImageBmp * toImg)
{
	int changeNum = 0;
	BYTE preby = GetGray(0,0);
	for(int i = 0;i<GetHeight();i++)
	{
		preby = GetGray(i,0);
		for (int j = 1 ; j < GetWidthPixel() -1 ; j++ )
		{
			if (preby != GetGray(j,i))
			{
				preby = GetGray(j,i);
				changeNum++;
			}
		}
		if (changeNum>min && changeNum<max)
		{
			for (int k = 0 ; k <GetWidthPixel() ; k++)
			{
				toImg->SetPixel(k,i,RGB(255,0,0));
			}
		}
		changeNum = 0;
	}
	return true;
}

bool CImageBmp::DetectLine2(int min,int max,CImageBmp * toImg)
{
	int changeNum = 0;
	BYTE preby = GetGray(0,0);
	for(int i = 0;i<GetWidthPixel()-1;i++)
	{
		preby = GetGray(i,0);
		for (int j = 1 ; j < GetHeight() -1 ; j++ )
		{
			if (preby != GetGray(i,j))
			{
				preby = GetGray(i,j);
				changeNum++;
			}
		}
		if (changeNum>min && changeNum<max)
		{
			for (int k = 0 ; k <GetHeight() ; k++)
			{
				toImg->SetPixel(i,k,RGB(255,0,0));
			}
		}
		changeNum = 0;
	}
	return true;
}

bool CImageBmp::ModelTo(CImageBmp* img)
{
	img->Resize(GetHeight(),GetWidthPixel(),255);

	for(int i=0;i<GetWidthPixel();i++)
	{
		for(int j=0;j<GetHeight();j++)
		{
			img->SetPixel(i,j,GetPixel(i,j));
		}
	}

	img->SetPixel(GetWidthPixel()/2,GetHeight()/2,RGB(255,0,0));

	return true;
}


bool CImageBmp::DetectLineX(int min,int max,int ax,int ay, int bx, int by)
{
	int changeNum = 0;
	BYTE preby = GetGray(0,0);
	for(int i = ay;i<by;i++)
	{
		preby = GetGray(i,0);
		for (int j = ax ; j < bx -1 ; j++ )
		{
			if (preby != GetGray(j,i))
			{
				preby = GetGray(j,i);
				changeNum++;
			}
		}
	}
	return changeNum;
}

bool CImageBmp::DetectLineY(int min,int max,int ax,int ay, int bx, int by)
{
	int changeNum = 0;
	BYTE preby = GetGray(0,0);
	for(int i = ax;i<bx;i++)
	{
		preby = GetGray(i,0);
		for (int j = ay ; j < by -1 ; j++ )
		{
			if (preby != GetGray(j,i))
			{
				preby = GetGray(j,i);
				changeNum++;
			}
		}
	}
	return changeNum;
}


// 区域跳点检测
bool CImageBmp::AreaDetect(int xmin, int xmax, int ymin, int ymax, CImageBmp* toImg)
{
	int changeNumX,changeNumY;
	BYTE preby;
	//初始化大小
	//for (int i = GetWidthPixel()/5; i<GetWidthPixel()/2; i++ )
	//{
	int i = 120;
	for(int j = 1; i+j<GetWidthPixel()-1; j=j+3)
	{
		for (int k = 1; i/3 + k < GetHeight()-1; k=k+3)
		{
			changeNumX = DetectLineX(10,20,j,k,j+i,k+i/3);
			changeNumY = DetectLineX(5,10,j,k,j+i,k+i/3);
			if (changeNumX<xmax && changeNumX>xmax && changeNumY<ymax && changeNumY>ymax)
			{
				DrawRecTangle(j,k,j+i,k+i/3,RGB(255,0,0),toImg);
			}
			changeNumX = 0;
			changeNumY = 0;
		}
	}
	//}
	return false;
}


// 画矩形
bool CImageBmp::DrawRecTangle(int ax, int ay, int bx, int by, COLORREF color, CImageBmp* toImg)
{
	for (int i =ax; i<bx; i++)
	{
		toImg->SetPixel(i,ay,color);
	}
	for (int i =ay; i<by; i++)
	{
		toImg->SetPixel(bx,i,color);
	}
	for (int i =ax; i<bx; i++)
	{
		toImg->SetPixel(i,by,color);
	}
	for (int i =ay; i<by; i++)
	{
		toImg->SetPixel(ax,i,color);
	}
	return true;
}

void CImageBmp::ChangeDIBToGray(void)
{
	for(int i=0;i<GetWidthPixel();i++)
	{
		for(int j=0;j<GetHeight();j++)
		{
			BYTE by = GetGray(i,j);

			SetPixel(i,j,RGB(by,by,by));
		}
	}
}

bool CImageBmp::GetClosestLine(int &high, int &low, CImageBmp* toImg)
{
	const int length = GetHeight()/10;
	high = 0;
	low = 0;
	int sum=0;
	int max=0;
	for (int i = 0; i + length < GetHeight(); i++)
	{
		for (int j = i; j < i+length; j++)
		{
			if (toImg->IsMarkLine(j))
			{
				++sum;
			}
		}
		if (sum >= max)
		{
			high = i;
			low = i + length;
			max = sum;
		}
		sum = 0;
	}
	return true;
}

bool CImageBmp::GetChangeNum(int* re,bool Width,CRect rect)
{
	int n = 0;

	if (Width == false)
	{
		for(int i=rect.top;i<rect.bottom;i++)
		{
			BYTE byOrg = GetGray(0,i);
			re[n] = 0;

			for(int j=rect.left;j<rect.right;j++)	
			{
				if (byOrg != GetGray(j,i))
				{
					byOrg = GetGray(j,i);
					re[n] ++;
				}
			}

			n++;
		}
	}
	else
	{
		for(int i=rect.left;i<rect.right;i++)
		{
			BYTE byOrg = GetGray(i,0);
			re[n] = 0;

			for(int j=rect.top;j<rect.bottom;j++)
			{
				if (byOrg != GetGray(i,j))
				{
					byOrg = GetGray(i,j);
					re[n] ++;
				}
			}

			n++;
		}
	}
	
	return true;
}

double CImageBmp::GetRectChangeNum(CRect rect)
{
	int re[2000];

	GetChangeNum(re,false,rect);

	double sum = 0;
	for(int i=0;i<rect.Height();i++)
	{
		sum += re[i];
	}

	return sum;
}

double CImageBmp::GetBlueMount(CRect rect)
{
	double re = 0.0;

	for(int i=rect.left;i<rect.right;i++)
	{
		for(int j=rect.top;j<rect.bottom;j++)
		{
			BYTE b = GetBValue(GetPixel(i,j));
			BYTE g = GetGValue(GetPixel(i,j));
			BYTE r = GetRValue(GetPixel(i,j));

			re += double(b) / (double(b) + double(g) + double(r));
		}
	}

	return re;
}

int CImageBmp::GetMidHistK(double d)
{
	double dHist[256];
	GetHist(dHist,256);

	for(int i=80;i>=1;i--)
	{
		if (dHist[i] != 0)
		{
			double dCur = dHist[i-1] - dHist[i];
			if (d < dCur)
				return i;
		}
	}

	return 0;
}

bool CImageBmp::BlueChange(void)
{
	for(int i=0;i<GetWidthPixel();i++)
	{
		for(int j=0;j<GetHeight();j++)
		{
			BYTE by = GetBValue(GetPixel(i,j));

			SetPixel(i,j,RGB(by,by,by));
		}
	}

	return true;
}

bool CImageBmp::GetHLowAndHigh(int* low, int* high, int* n)
{
	return false;
}

bool CImageBmp::GetPixelCount(double* re, CRect rect, BYTE Color, bool IsWidth)
{
	int d = 0;
	if (IsWidth == true)
	{
		for(int i=rect.left;i<rect.right;i++)
		{
			re[d] = 0.0;

			for(int j=rect.top;j<rect.bottom;j++)
			{
				if (GetGray(i,j) == Color)
				{
					re[d]++;
				}
			}

			d++;
		}
	}
	else
	{
		for(int i=rect.top;i<rect.bottom;i++)
		{
			re[d] = 0.0;
			
			for(int j=rect.top;j<rect.bottom;j++)
			{
				if (GetGray(j,i) == Color)
				{
					re[d] ++;
				}
			}

			d++;
		}
	}

	return true;
}

CRect CImageBmp::GetHighRectFromRect(CImageBmp *ib,CRect rect,int nCR)
{
	CRect r;

	int d[2000];
	ib->GetChangeNum(d,false,rect);

	double dE1[2000];

	for(int i=0;i<rect.Height()-1;i++)
	{
		dE1[i] = d[i+1] - d[i];
	}

	double dE2[2000];

	for(int i=0;i<rect.Height() - 2;i++)
	{
		dE2[i] = dE1[i+1] - dE1[i];
	}

	int nMin = 0,nMax = 0;
	for(int i=0;i<rect.Height()-2;i++)
	{
		if (dE2[i] < dE2[nMin])
		{
			nMin = i;
		}

		if (dE2[i] > dE2[nMax])
		{
			nMax = i;
		}
	}

	if (nMax > nMin)
	{
		int swap = nMax;
		nMax = nMin;
		nMin = swap;
	}

	r.top = rect.top + nMax;
	r.bottom = rect.top+ nMin;

	r.left = 0;
	r.right = 0;
	
	double nleft = 0;
	double nRight = 0;

	for(int i=r.top;i<r.bottom;i++)
	{
		bool bLeft = false;

		for(int j=rect.left;j<rect.right;j++)
		{
			BYTE by = GetGray(j,i);

			if (bLeft == false && GetGray(j,i) == nCR)
			{
				bLeft = true;
				nleft += j;
			}
		}
	}

	r.left = nleft / r.Height();

	for(int i=r.top;i<r.bottom;i++)
	{
		bool bLeft = false;

		for(int j=GetWidthPixel()-1;j>=0;j--)
		{
			if (bLeft == false && GetGray(j,i) == nCR)
			{
				bLeft = true;
				nRight += j;
			}
		}

	}

	r.right = nRight / r.Height();

	return r;
}

bool CImageBmp::RightImage(void)
{
	CImageBmp ib;
	ib.ModalFrom(*this);

	for(int i=0;i<GetHeight();i++)
	{
		int n = 0;
		for(int j=0;j<GetWidthPixel();j++)
		{
			BYTE by = ib.GetGray(j,i);

			if (by >= 200 && n != 0)
			{
				n ++;
			}
			else if (by < 200)
			{
				n++;
			}

			if (n != 0)
			{
				SetPixel(n,i,RGB(by,by,by));
			}
		}

		for(int j=n;j<GetWidthPixel();j++)
		{
			SetPixel(j,i,RGB(255,255,255));
		}
	}


	for(int i=0;i<GetWidthPixel();i++)
	{
		int n = 0;
		for(int j=0;j<GetHeight();j++)
		{
			BYTE by = ib.GetGray(i,j);
			
			if (by >= 200 && n != 0)
			{
				n ++;
			}
			else if (by < 200)
			{
				n++;
			}

			if (n != 0)
			{
				SetPixel(i,n - 1,RGB(by,by,by));
			}
		}

		for(int j=n;j<GetHeight();j++)
		{
			SetPixel(i,j,RGB(255,255,255));
		}
	}

	return true;
}

int CImageBmp::GetGJYNetTrain(double* re,int n)
{
	int dHist[2000];
	GetChangeNum(dHist,false,CRect(0,0,GetWidthPixel(),GetHeight()));

	for(int i=0;i<GetHeight()/n;i++)
	{
		re[i] = 0.0;

		for(int k=0;k<n;k++)
		{
			re[i] += dHist[i*n+k];
		}

		re[i] = re[i] / (5*n);
	}

	GetChangeNum(dHist,true,CRect(0,0,GetWidthPixel(),GetHeight()));
	for(int i=GetHeight()/n;i<GetHeight()/n+GetWidthPixel()/n;i++)
	{
		re[i] = 0.0;
	
		for(int k=0;k<n;k++)
		{
			re[i] += dHist[i*n+k];
		}

		re[i] = re[i] / (5*n);
	}

	return 0;
}

bool CImageBmp::ChangeToNormalNetTrain(double dRate)
{
	double dCur = 0.0;
	dCur = double(GetPixelCount(0,110)) / double(GetWidthPixel()*GetHeight());

	int n=0;
	while(true)
	{
		dCur = double(GetPixelCount(0,110)) / double(GetWidthPixel()*GetHeight());

		if (fabs(dCur - dRate) < 0.005)
			break;

		if (dCur > dRate)
		{		
			int se[3][3] = {{-1,1,-1},{1,1,1},{-1,1,-1}};

			Erode(se);
		}
		else if (dCur < dRate)
		{
			int se[3][3] = {{-1,1,-1},{1,1,1},{-1,1,-1}};

			Dilate(se);
		}
		n++;

		if (n>15)
			break;
	}

	return true;
}
