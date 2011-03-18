#pragma once

#include <complex>
using namespace std;

#define WIDTHBYTES(bits) (((bits) + 31)/32*4)

class CImageBmp
{
public:
	CImageBmp(void);
public:
	~CImageBmp(void);
public:
	bool AttachFromFile(CString argstrFileName);
private:
	BITMAPINFOHEADER* m_pBMIH;
	LPBYTE *m_pData;
	LPVOID m_pColorTable;
	int m_nColorEnrties;

public:
	bool AttachFromFile(CFile &argfl);

public:
	bool AttachFromFile(LPCTSTR argstrFileName);
public:
	bool SaveToFile(CFile& argfl);
public:
	bool IsValidate(void);
public:
	bool CleanEnv(void);
public:
	bool SaveToFile(LPCTSTR argstrFileName);
public:
	int GetHeight(void);
public:
	int GetWidthPixel(void);
public:
	int GetWidthByte(void);
public:

	inline COLORREF GetPixel(int x,int y)
	{
		if (m_pBMIH->biBitCount == 8)
		{
			BYTE bytGray = m_pData[m_pBMIH->biHeight - y -1][x];
			return RGB(bytGray,bytGray,bytGray);
		}
		else if(m_pBMIH->biBitCount == 1)	// 单色图
		{
			BYTE ret = (1<<(7-x%8) & m_pData[m_pBMIH->biHeight - y - 1][x/8]);

			RGBQUAD *p = (RGBQUAD*)m_pColorTable;
			if(p[0].rgbBlue != 0)
				ret = !ret;

			if(ret)
				return RGB(255, 255, 255);		// 白色
			else 
				return RGB(0, 0, 0);			// 黑色
		}
		else if (m_pBMIH->biBitCount == 24)
		{
												return RGB
				(
					m_pData[m_pBMIH->biHeight - y - 1][3*x + 2],
					m_pData[m_pBMIH->biHeight - y - 1][3*x + 1],
					m_pData[m_pBMIH->biHeight - y - 1][3*x + 0]
				);
		}
		else
		{
			return RGB(0,0,0);
		}
	}
public:
	bool SetPixel(int x, int y, COLORREF color);
public:

	inline BYTE GetGray(int x, int y)
	{	
		COLORREF color = GetPixel(x,y);
		BYTE r,g,b,byte;
		
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if (r == g && r == b)
		{
			return r;
		}

		float ff = (0.30 * r + 0.59 * g + 0.11*b);
		byte = (int)ff;

		return byte;
	}
public:
	bool ModalFrom(CImageBmp& argib);
public:
	bool IsBinaryImage(void);
public:
	bool IsIndexedImage(void);
public:
	bool IndexToGray(void);
public:
	bool Draw(CDC* pdc);
public:
	bool DrawAt(int x, int y, CDC* pdc);
public:
	bool GetHist(double* outHist, int argSegNum);
public:
	bool DrawWithMemDC(CDC *pdc);
public:
	bool LinTran(double dFa, double dFb);
public:
	bool LogTran(double dFc);
public:
	bool GammerTran(double dGammer, double dexp);
public:
	bool ThresholdTran(int nThres);
public:
	bool HistEq(void);
public:
	bool HistSt(CImageBmp *ibStd);
public:
	bool SpaceEnhanceWithTemplate(int TmpltW, int TmpltH, int TmpltMW, int TmpltMH, float* pfTemp, double fCoef);
public:
	//void operator = (const CImageBmp & ib)
	//{
	//	if (!ib.IsValidate())
	//		return;

	//	CleanEnv();

	//	m_pBMIH = (BITMAPINFOHEADER*)new BYTE[ib.m_nColorEnrties * 4 + sizeof(BITMAPINFOHEADER)];
	//	memcpy(m_pBMIH,ib.m_pBMIH,sizeof(BITMAPINFOHEADER));

	//	m_nColorEnrties = ib.m_nColorEnrties;
	//	memcpy(m_pColorTable,ib.m_pColorTable,ib.m_nColorEnrties * 4);

	//	int nWidthBytes = WIDTHBYTES(ib.m_pBMIH->biWidth * ib.m_pBMIH->biBitCount);
	//	m_pData = new LPBYTE[m_pBMIH->biHeight];
	//	
	//	for(int i=0;i<ib.m_pBMIH->biHeight;i++)
	//	{
	//		m_pData[i] = new BYTE[nWidthBytes];
	//		memcpy(m_pData[i],ib.m_pData[i],nWidthBytes);
	//	}
	//}
public:
	bool InitPixel(int nWidth, int nHeight, BYTE bytColor);
public:
	bool SpaceEnhanceAveSmooth(void);
public:
	bool SpaceEnhanceGaussSmooth(void);
public:
	bool SpaceEnhaceAdapterSmooth(BYTE nStd);
public:
	bool SpaceEnhanceMidianFilter(int TmpltW, int TmpltH, int TmpltMW, int TmpltMH,int nOrder);
public:
	bool InitPixelWithImage(CImageBmp* ib);
	BYTE GetMid(BYTE* btArray, int n,int order);
public:
	bool SpaceEnhanceSobel(void);
public:
	bool SpaceEnhanceEoG(void);
public:
	bool SpaceEnhanceLaplace(void);
public:
	bool SpaceEnhanceEoGBig(void);
public:
	bool FFT(complex<double> * TD, complex<double> * FD, int r);
public:
	bool IFFT(complex<double> * FD, complex<double> * TD, int r);
public:
	bool FFTin2D(bool bExpand, complex<double>* out,BYTE byFillColor,int *w,int *h,int *orgx,int *orgy);
public:
	bool Resize(int w, int h, BYTE byColor);
public:
	bool IFFin2D(complex<double>* in, long nWidth, long lHeight, long loutW, long loutH);
public:
	bool GetFFTin2D(complex<double>* out,bool bExtend,BYTE byFillColor, long* lWidth, long* lHeight, long* loutWidth,long *loutHeight);
public:
	long GetFreqWitdh(bool bExpand = true);
public:
	long GetFreqHeight(bool bExpand = true);
public:
	bool FreqEnhanceFilt(double* dFilter, BYTE byFillColor);
public:
	bool TemplateIdealLPF(double* dFilter, int nFreq);
public:
	bool FreqEnhanceIdealLPF(double nFreq);
public:
	bool TemplateGaussLPF(double* adFilter, double dSigma);
public:
	bool FreqEnhanceGaussLPF(double dSigma);
public:
	bool TemplateGaussHPF(double* adFilter, double dSigma);
public:
	bool FreqEnhanceGaussHPF(double dSigma);
public:
	bool TemplateLaplaceHPF(double* adFilter);
public:
	bool Erode(int  se[3][3]);
public:
	bool FreqEnhanceLaplaceHPF(void);
public:
	bool Dilate(int  se[3][3]);
public:
	bool Open(int  se[3][3]);
public:
	bool Close(int se[3][3]);
public:
	bool TraceBoundary(void);
public:
	bool intersect(CImageBmp* ib);
public:
	bool Union(CImageBmp* ib);
public:
	bool Except(CImageBmp* ib);
public:
	bool FillRegion(int x, int y);
public:
	bool Reverse(void);
public:
	bool Equal(CImageBmp* ib);
public:
	bool InitPixelWithImageAndColor(CImageBmp* ib, BYTE color);
public:
	static bool DebugTry(CImageBmp* bi, int nSleep);
public:
	bool LabelConnectionRegion(int nConn,int nMin=1);
public:
	bool Thining(void);
public:
	bool PixelImage(int lowThre, int highThre);
public:
	bool Convex(bool bConstrain);
public:
	bool Or(CImageBmp* ib);
public:
	bool GrayDilate(int nTempW, int nTempH, int nTempMX, int nTempMY, int** se);
public:
	bool GrayErode(int nTempW, int nTempH, int nTempMX, int nTempMY, int** se);
public:
	bool GrayOpen(int nTempW, int nTempH, int nTempMX, int nTempMY, int** se);
public:
	bool GrayClose(int nTempW, int nTempH, int nTemoMX, int nTempMY, int** se);
public:
	bool TopHat(int nTempW, int nTempH, int nTempMX, int nTempMY, int** se);
public:
	bool SpaceEnhanceText(void);
public:
	bool ExceptBorder(BYTE byColor);
public:
	bool GetConnectionReginRect(CRect* rect);
public:
	int GetPixelCount(BYTE byStart , BYTE byEnd);
public:
	void Part(CImageBmp* bmp, CRect rect);
public:
	void ColorRect(CRect* rect, COLORREF color);
public:
	bool GetPRect(CRect* rect);
public:
	bool GetRealRect(CRect* rect);
public:
	double GetLikelyPercent(CImageBmp* img);
public:
	BYTE GetMidHist(double dMu);
public:
	bool GetMinHist(BYTE* aby,int *n);
public:
	void VProject(int* left, int* right,int n);
public:
	void HProject(int* top, int* bottom, int n);
public:
	void ConnectionRegin(int n, int nMin);
private:
	void ConnectionReginSet(int x, int y, int n,int nConReg,int &sum);
	bool ClearGray(BYTE by);
	bool ConnectSameRegion(BYTE byColorA , BYTE byColorB , BYTE byColorRe);
public:
	BYTE GetRectPixel(int x, int y, int rectX);
public:
	bool GetNetTrain(int rect, double* re);
public:
	int RectStretchAll(CRect size,CRect rect);
	public:
		bool ModelTo(CImageBmp* img);
public:
	bool DetectLineX(int min,int max,int ax,int ay, int bx, int by);
public:
	bool DetectLineY(int min,int max,int ax,int ay, int bx, int by);
public:
	// 区域跳点检测
	bool AreaDetect(int xmin, int xmax, int ymin, int ymax, CImageBmp* toImg);
public:
	// 画矩形
	bool DrawRecTangle(int ax, int ay, int bx, int by, COLORREF color, CImageBmp* toImg);
public:
	void ChangeDIBToGray(void);
public:
	bool GetClosestLine(int &high, int &low, CImageBmp* toImg);
public:
	bool CImageBmp::DetectLine(int min,int max,CImageBmp * toImg);

	// 判断是否为标记线
	bool IsMarkLine(int height)
	{
		//if(GetPixel(0,height) == RGB(255,0,0) && GetPixel(GetWidthPixel(),height) == RGB(255,0,0) && GetPixel(GetWidthPixel()/2,height))
		if(GetPixel(0,height) == RGB(255,0,0))
			return true;
		else
			return false;
	}

	bool CImageBmp::DetectLine2(int min,int max,CImageBmp * toImg);

public:
	bool GetChangeNum(int* re,bool Width,CRect rect);
public:
	double GetRectChangeNum(CRect rect);
public:
	double GetBlueMount(CRect rect);
public:
	int GetMidHistK(double d);
public:
	bool BlueChange(void);
public:
	bool GetHLowAndHigh(int* low, int* high, int* n);
public:
	bool GetPixelCount(double* re, CRect rect, BYTE Color, bool IsWidth);
public:
	CRect GetHighRectFromRect(CImageBmp *ib,CRect rect,int nCR);
public:
	bool RightImage(void);
public:
	int GetGJYNetTrain(double* re,int n);
public:
	bool ChangeToNormalNetTrain(double dRate);
};
