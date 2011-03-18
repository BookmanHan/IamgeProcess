#include "stdafx.h"
#include "ImageProcess.h"
#include "DialogHist.h"
#include "DialogLinTran.h"
#include "DialogLogTran.h"
#include "DialogThershold.h"
#include "DialogGramma.h"
#include "MainFrm.h"
#include <complex>
#include "BpNet.h"
#include <vector>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//(0.0006,0.8,21)
//(0.00055,0.6,20)
//(0.0006,0.8,20)
//(0.0006,0.7,22)
//(0.0006,0.7,22)
//(0.00055,0.7,22)
//(0.0005,0.7,22)

#define THE_K 0.00037
#define THE_PER 0.7
#define PLDATE_FIND_K 0.0004
#define PIC_EDGE 50

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_OPEN, &CMainFrame::OnOpen)
	ON_COMMAND(ID_EXIT, &CMainFrame::OnExit)
	ON_COMMAND(ID_HIST, &CMainFrame::OnHist)
	ON_COMMAND(ID_GRAY, &CMainFrame::OnGray)
	ON_COMMAND(ID_TRANS_LINTRAN, &CMainFrame::OnTransLintran)
	ON_COMMAND(ID_32780, &CMainFrame::On32780)
	ON_COMMAND(ID_GRAMMA_TRAN, &CMainFrame::OnGrammaTran)
	ON_COMMAND(ID_HIST_EQ, &CMainFrame::OnHistEq)
	ON_COMMAND(ID_HIST_ST, &CMainFrame::OnHistSt)
	ON_COMMAND(ID_THRESHOSD, &CMainFrame::OnThreshosd)
	ON_COMMAND(ID_SPACE_AVG_SMOOTH, &CMainFrame::OnSpaceAvgSmooth)
	ON_COMMAND(ID_SPACE_GAUSS_SMOOTH, &CMainFrame::OnSpaceGaussSmooth)
	ON_COMMAND(ID_SPACE_ADAPTER_SMOOTH, &CMainFrame::OnSpaceAdapterSmooth)
	ON_COMMAND(ID_MID_FILTER, &CMainFrame::OnMidFilter)
	ON_COMMAND(ID_LOW_FILTER, &CMainFrame::OnLowFilter)
	ON_COMMAND(ID_HIGH_FILTER, &CMainFrame::OnHighFilter)
	ON_COMMAND(ID_SOBEL, &CMainFrame::OnSobel)
	ON_COMMAND(ID_LAPLACE, &CMainFrame::OnLaplace)
	ON_COMMAND(ID_LOG, &CMainFrame::OnLog)
	ON_COMMAND(ID_LOG_BIG, &CMainFrame::OnLogBig)
	ON_COMMAND(ID_FFT, &CMainFrame::OnFft)
	ON_COMMAND(ID_IFFT, &CMainFrame::OnIfft)
	ON_COMMAND(ID_IDEAL_LPF, &CMainFrame::OnIdealLpf)
	ON_COMMAND(ID_GAUSS_FILTER, &CMainFrame::OnGaussFilter)
	ON_COMMAND(ID_GAUSS_HPF, &CMainFrame::OnGaussHpf)
	ON_COMMAND(ID_SQUARE_ERODE, &CMainFrame::OnSquareErode)
	ON_COMMAND(ID_CROSS_ERODE, &CMainFrame::OnCrossErode)
	ON_COMMAND(ID_LAPLACE_HFD, &CMainFrame::OnLaplaceHfd)
	ON_COMMAND(ID_Undo, &CMainFrame::OnUndo)
	ON_COMMAND(ID_SQUARE_DILATE, &CMainFrame::OnSquareDilate)
	ON_COMMAND(ID_CROSS_DILATE, &CMainFrame::OnCrossDilate)
	ON_COMMAND(ID_SQUARE_CLOSE, &CMainFrame::OnSquareClose)
	ON_COMMAND(ID_CROSS_CLOSE, &CMainFrame::OnCrossClose)
	ON_COMMAND(ID_SQUARE_OPEN, &CMainFrame::OnSquareOpen)
	ON_COMMAND(ID_CROSS_OPEN, &CMainFrame::OnCrossOpen)
	ON_COMMAND(ID_TRACE_BOUNDARY, &CMainFrame::OnTraceBoundary)
	ON_COMMAND(ID_REGIN_FILL, &CMainFrame::OnReginFill)
	ON_COMMAND(ID_REVERSE, &CMainFrame::OnReverse)
	ON_COMMAND(ID_32848, &CMainFrame::On32848)
	ON_COMMAND(ID_CONNECTION_REGION, &CMainFrame::OnConnectionRegion)
	ON_COMMAND(ID_THINING, &CMainFrame::OnThining)
	ON_COMMAND(ID_PIXEL_IMAGE, &CMainFrame::OnPixelImage)
	ON_COMMAND(ID_CONVEX, &CMainFrame::OnConvex)
	ON_COMMAND(ID_TOP_HAT, &CMainFrame::OnTopHat)
	ON_COMMAND(ID_TEXT, &CMainFrame::OnText)
	ON_COMMAND(ID_EXCEPT_BORDER, &CMainFrame::OnExceptBorder)
	ON_COMMAND(ID_PLATE_FIND, &CMainFrame::OnPlateFind)
	ON_COMMAND(ID_BLUE_PLATE, &CMainFrame::OnBluePlate)
	ON_COMMAND(ID_SPLIT, &CMainFrame::OnSplit)
	ON_COMMAND(ID_READ, &CMainFrame::OnRead)
	ON_COMMAND(ID_HIST_MID, &CMainFrame::OnHistMid)
	ON_COMMAND(ID_MIN_HIST, &CMainFrame::OnMinHist)
	ON_COMMAND(ID_HIST_MIN_TRANS, &CMainFrame::OnHistMinTrans)
	ON_COMMAND(ID_THREAD_HIST_TRAN, &CMainFrame::OnThreadHistTran)
	ON_COMMAND(ID_VPROJECT, &CMainFrame::OnVproject)
	ON_COMMAND(ID_HPROJECT, &CMainFrame::OnHproject)
	ON_COMMAND(ID_PRE_FIND_PLATE, &CMainFrame::OnPreFindPlate)
	ON_COMMAND(ID_SPLIT_TRANS, &CMainFrame::OnSplitTrans)
	ON_COMMAND(ID_SWALLOW_HIST_TRAN, &CMainFrame::OnSwallowHistTran)
	ON_COMMAND(ID_32891, &CMainFrame::On32891)
	ON_COMMAND(ID_CON_REGIN, &CMainFrame::OnConRegin)
	ON_COMMAND(ID_BP_NET_TRAIN, &CMainFrame::OnBpNetTrain)
	ON_COMMAND(ID_GRID_IT, &CMainFrame::OnGridIt)
	ON_COMMAND(ID_NET_SIM, &CMainFrame::OnNetSim)
	ON_COMMAND(ID_NORMAL, &CMainFrame::OnNormal)
	ON_COMMAND(ID_CHECKD, &CMainFrame::OnCheckd)
	ON_COMMAND(ID_PIC_OUT, &CMainFrame::OnPicOut)
	ON_COMMAND(ID_SIM_INIT, &CMainFrame::OnSimInit)
	ON_COMMAND(ID_OD_TEST, &CMainFrame::OnOdTest)
	ON_COMMAND(ID_STRIKE, &CMainFrame::OnStrike)
	ON_COMMAND(ID_HIST_K, &CMainFrame::OnHistK)
	ON_COMMAND(ID_BLUE_PASSAGE, &CMainFrame::OnBluePassage)
	ON_COMMAND(ID_AllDo, &CMainFrame::OnAlldo)
	ON_COMMAND(ID_SIZE_FIT, &CMainFrame::OnSizeFit)
	ON_COMMAND(ID_RENAME, &CMainFrame::OnRename)
	ON_COMMAND(ID_BINARY_PIC, &CMainFrame::OnBinaryPic)
	ON_COMMAND(ID_RIGHT_IMAGE, &CMainFrame::OnRightImage)
	ON_COMMAND(ID_GJY_TRAIN, &CMainFrame::OnGjyTrain)
	ON_COMMAND(ID_GJY_REG, &CMainFrame::OnGjyReg)
	ON_COMMAND(ID_SIGN, &CMainFrame::OnSign)
	ON_COMMAND(ID_NEW_SIM_INITI, &CMainFrame::OnNewSimIniti)
	ON_COMMAND(ID_32934, &CMainFrame::On32934)
	ON_COMMAND(ID_32935, &CMainFrame::On32935)
	ON_COMMAND(ID_REAL_DO, &CMainFrame::OnRealDo)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	ptClick.x = 0;
	ptClick.y = 0;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetMenu(NULL);
	CMenu menuMain;
	menuMain.LoadMenu(IDM_MAIN);
	SetMenu(&menuMain);
	menuMain.Detach();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序




void CMainFrame::OnOpen()
{
	CFileDialog dlgflOpen(true,L"*.bmp",L"打开所要处理的文件");

	if (dlgflOpen.DoModal() == IDOK)
	{
		 biDoc.AttachFromFile(dlgflOpen.GetFileName());
		 strfl = dlgflOpen.GetFileName();
		 biDoc.ChangeDIBToGray();

		 GetActiveView()->Invalidate();
	}
}

void CMainFrame::OnExit()
{
	exit(0);
}

void CMainFrame::OnHist()
{
	double realHist[256];
	biDoc.GetHist(realHist,256);

	CDialogHist* dlghist = new CDialogHist();
	dlghist->SetDisplayHistData(realHist,256);

	dlghist->Create(IDD_DIALOG1,this);
	dlghist->ShowWindow(SW_SHOW);
}

void CMainFrame::OnGray()
{
	DoIt();

	biDoc.IndexToGray();
	GetActiveView()->Invalidate();
}

void CMainFrame::OnTransLintran()
{
	DoIt();

	CDialogLinTran dlglt;

	dlglt.DoModal();
	GetActiveView()->Invalidate();
}

void CMainFrame::On32780()
{
	DoIt();

	CDialogLogTran dlglt;

	dlglt.DoModal();
	GetActiveView()->Invalidate();
}

void CMainFrame::OnGrammaTran()
{
	DoIt();

	CDialogGramma dlggm;

	dlggm.DoModal();
	GetActiveView()->Invalidate();
}

void CMainFrame::OnHistEq()
{
	DoIt();

	biDoc.HistEq();
	GetActiveView()->Invalidate();
}

void CMainFrame::OnHistSt()
{
	DoIt();

	CFileDialog dlgfl(true);

	if (dlgfl.DoModal() == IDOK)
	{
		CImageBmp biCur;
		biCur.AttachFromFile(dlgfl.GetFileName());
		biDoc.HistSt(&biCur);
		GetActiveView()->Invalidate();
	}
}

void CMainFrame::OnThreshosd()
{
	DoIt();

	CDialogThershold dlgthd;
	dlgthd.DoModal();

	GetActiveView()->Invalidate();
}

void CMainFrame::OnSpaceAvgSmooth()
{
	DoIt();

	biDoc.SpaceEnhanceAveSmooth();
	GetActiveView()->Invalidate();
}

void CMainFrame::OnSpaceGaussSmooth()
{
	DoIt();

	biDoc.SpaceEnhanceGaussSmooth();
	GetActiveView()->Invalidate();
}

void CMainFrame::OnSpaceAdapterSmooth()
{
	DoIt();

	biDoc.SpaceEnhaceAdapterSmooth(50);
	GetActiveView()->Invalidate();
}

void CMainFrame::OnMidFilter()
{
	DoIt();

	biDoc.SpaceEnhanceMidianFilter(3,3,1,1,0);
	GetActiveView()->Invalidate();
}

void CMainFrame::OnLowFilter()
{
	DoIt();

	biDoc.SpaceEnhanceMidianFilter(3,3,1,1,1);
	GetActiveView()->Invalidate();
}

void CMainFrame::OnHighFilter()
{
	DoIt();

	biDoc.SpaceEnhanceMidianFilter(3,3,1,1,9);
	GetActiveView()->Invalidate();
}

void CMainFrame::OnSobel()
{
	DoIt();

	biDoc.SpaceEnhanceSobel();
	GetActiveView()->Invalidate();
}

void CMainFrame::OnLaplace()
{
	DoIt();

	biDoc.SpaceEnhanceLaplace();
	GetActiveView()->Invalidate();
}

void CMainFrame::OnLog()
{
	DoIt();

	biDoc.SpaceEnhanceEoG();
	GetActiveView()->Invalidate();
}

void CMainFrame::OnLogBig()
{
	DoIt();

	biDoc.SpaceEnhanceEoGBig();
	GetActiveView()->Invalidate();
}

void CMainFrame::OnFft()
{
	DoIt();

	BeginWaitCursor();

	int tw,th,tx,ty;
	biDoc.FFTin2D(true,NULL,255,&tw,&th,&tx,&ty);
	GetActiveView()->Invalidate();

	EndWaitCursor();
}

void CMainFrame::OnIfft()
{
	DoIt();

	complex <double> *p = new complex<double>[512*512];

	BeginWaitCursor();

	long tw,th,tx,ty;
	biDoc.GetFFTin2D(p,true,255,&tw,&th,&tx,&ty);

	biDoc.IFFin2D(p,tw,th,tx,ty);

	GetActiveView()->Invalidate();

	EndWaitCursor();
}

void CMainFrame::OnIdealLpf()
{
	DoIt();

	BeginWaitCursor();

	biDoc.FreqEnhanceIdealLPF(100);

	GetActiveView()->Invalidate();

	EndWaitCursor();
}

void CMainFrame::OnGaussFilter()
{
	DoIt();

	BeginWaitCursor();

	biDoc.FreqEnhanceGaussLPF(50);

	EndWaitCursor();

	GetActiveView()->Invalidate();
}

void CMainFrame::OnGaussHpf()
{
	DoIt();

	BeginWaitCursor();

	biDoc.FreqEnhanceGaussHPF(20);

	EndWaitCursor();

	GetActiveView()->Invalidate();
}

void CMainFrame::OnSquareErode()
{
	DoIt();

	int se[3][3] = {{1,1,1},{1,1,1},{1,1,1}};

	biDoc.Erode(se);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnCrossErode()
{
	DoIt();

	int se[3][3] = {{-1,1,-1},{1,1,1},{-1,1,-1}};

	biDoc.Erode(se);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnLaplaceHfd()
{
	DoIt();

	BeginWaitCursor();

	biDoc.FreqEnhanceLaplaceHPF();

	EndWaitCursor();

	GetActiveView()->Invalidate();
}

void CMainFrame::OnUndo()
{
	biDoc.ModalFrom(biPreDoc);

	GetActiveView()->Invalidate();
}

bool CMainFrame::DoIt(void)
{
	biPreDoc.ModalFrom(biDoc);

	return false;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnSquareDilate()
{
	DoIt();

	int temp[3][3] = { {1,1,1},{1,1,1},{1,1,1} };

	biDoc.Dilate(temp);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnCrossDilate()
{
	DoIt();

	int temp[3][3] = { {-1,1,-1},{1,1,1},{-1,1,-1} };

	biDoc.Dilate(temp);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnSquareClose()
{
	DoIt();

	int temp[3][3] = { {1,1,1},{1,1,1},{1,1,1} };

	biDoc.Close(temp);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnCrossClose()
{
	DoIt();

	int temp[3][3] = { {-1,1,-1},{1,1,1},{-1,1,-1} };

	biDoc.Close(temp);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnSquareOpen()
{
	DoIt();

	int temp[3][3] = { {1,1,1},{1,1,1},{1,1,1} };

	biDoc.Open(temp);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnCrossOpen()
{
	DoIt();

	int temp[3][3] = { {-1,1,-1},{1,1,1},{-1,1,-1} };

	biDoc.Open(temp);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnTraceBoundary()
{
	DoIt();

	biDoc.TraceBoundary();

	GetActiveView()->Invalidate();
}

void CMainFrame::OnReginFill()
{
	biDoc.FillRegion(ptClick.x,ptClick.y);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnReverse()
{
	DoIt();

	biDoc.Reverse();

	GetActiveView()->Invalidate();
}

void CMainFrame::On32848()
{
	DoIt();

	CImageBmp bi;
	bi.InitPixelWithImageAndColor(&biDoc,255);
	biDoc.ModalFrom(bi);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnConnectionRegion()
{
	DoIt();

	biDoc.LabelConnectionRegion(0);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnThining()
{
	DoIt();

	biDoc.Thining();

	GetActiveView()->Invalidate();
}

void CMainFrame::OnPixelImage()
{
	DoIt();

	biDoc.PixelImage(10,200);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnConvex()
{
	DoIt();

	biDoc.Convex(false);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnTopHat()
{
	int *se[3];
	for(int i=0;i<3;i++)
	{
		se[i] = new int[3];
	}

	se[0][0] = 0;
	se[0][1] = 1;
	se[0][2] = 0;
	se[1][0] = 1;
	se[1][1] = 1;
	se[1][2] = 1;
	se[2][0] = 0;
	se[2][1] = 1;
	se[2][2] = 0;

	DoIt();

	biDoc.TopHat(3,3,1,1,(int **)se);

	GetActiveView()->Invalidate();

	for(int i=0;i<3;i++)
	{
		delete se[i];
	}
}

void CMainFrame::OnText()
{
	DoIt();
	
	biDoc.SpaceEnhanceText();
	biDoc.ExceptBorder(0);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnExceptBorder()
{
	DoIt();

	biDoc.ExceptBorder(255);

	GetActiveView()->Invalidate();
}

int GetTheLowAndHigh(int *re,int len,int*low,int *high)
{
	int res[2000];
	for(int i=0;i<len-1;i++)
	{
		res[i] = re[i+1] - re[i];
	}

	int min = 200000;
	int nMin = -1;
	int nMax = -1;
	int max = -200000;

	for(int i = 0;i<len-1;i++)
	{
		if (min > res[i])
		{
			nMin = i;
			min = res[i];
		}

		if (max < res[i])
		{
			nMax = i;
			max = res[i];
		}
	}

	*low = nMax;
	*high = nMin;

	return 0;
}

bool GetTheLowAndHighCountNumber(double *re,int len,int *low,int *high,int *lowcount,int *highcount)
{
	double dE[2000];
	double dSortE[2000];

	double sum = 0.0;
	for(int i=0;i<len;i++)
	{
		sum += re[i];
	}

	sum /= len;

	int nHigh = 0;

	if (re[0] > sum)
	{
		nHigh ++;
	}

	for(int i=0;i<len-1;i++)
	{
		if (re[i] < sum & re[i+1] > sum)
			nHigh ++;
	}

	*highcount = nHigh;

	//for(int i=0;i<len-1;i++)
	//{
	//	int n = i;

	//	for(int j=i+1;j<len-1;j++)
	//	{
	//		if (dSortE[n] < dSortE[j])
	//		{
	//			n = j;
	//		}
	//	}

	//	double tmp = dSortE[n];
	//	dSortE[n] = dSortE[i];
	//	dSortE[i] = tmp;
	//}

	return true;
}

void CMainFrame::OnPlateFind()
{
	DoIt();

	biDoc.ConnectionRegin(4,20);

	CRect rect[1000];
	biDoc.GetConnectionReginRect(rect);

	CStdioFile file;
	file.Open(L"D:\\Re.txt",CFile::modeCreate | CFile::modeReadWrite);

	int temp[3][3] = { {1,1,1},{1,1,1},{1,1,1} };

	CImageBmp ib;
	ib.AttachFromFile(strfl);
	ib.ChangeDIBToGray();
	ib.SpaceEnhanceText();
	ib.ThresholdTran(ib.GetMidHist(THE_PER));
	ib.Open(temp);

	CImageBmp ibt;
	ibt.AttachFromFile(strfl);
	ibt.ChangeDIBToGray();
	ibt.ThresholdTran(110);
	ibt.SpaceEnhanceText();
	ibt.Open(temp);

	CImageBmp color;
	color.AttachFromFile(strfl);

	struct Arg
	{
		int n;
		double color;
		double WHrate;

		Arg(int an,double acolor,double aw)
		{
			n = an;
			color =acolor;
			WHrate = aw;
		}

		Arg()
		{
		}
	};

	Arg varg[256];

	int n=-1;
	double dSum = -100;

	bool flag = false;
	int nflag = 0;

	//int nRest = 0;
	//for(int i=1;i<256;i++)
	//{
	//	if (rect[i].left == 0 && rect[i].top == 0)
	//	{
	//		nRest = i;
	//		break;
	//	}
	//}

	int nRest = 254;
	int nR = nRest;

	for(int k=0;k<3;k++)
	for(int i=1;i<=nR;i++)
	{
		if (rect[i].left == 0)
			continue;

		for(int j=1;j<nR;j++)
		{
			if (rect[j].top == 0)
				continue;

			if (abs(rect[i].left - rect[j].right) <= 25 && abs(rect[i].top - rect[j].top) <= 25)
			{	
				nRest++;
				rect[nRest].left = rect[j].left;
				rect[nRest].right = rect[i].right;
				rect[nRest].top = rect[i].top>rect[j].top?rect[j].top:rect[i].top;
				rect[nRest].bottom = rect[i].bottom >rect[j].bottom?rect[i].bottom:rect[j].bottom;
			}
		}
	}

	for(int i=1;i<nRest;i++)
	{
		if (rect[i].Width() > 155)
		{
			rect[i] = biDoc.GetHighRectFromRect(&ib,rect[i],i);
		}

		if (rect[i].Height() > 76)
		{
			rect[i] = biDoc.GetHighRectFromRect(&ib,rect[i],i);
		}
	}

	for(int i=1;i<nRest;i++)
	{
		if (rect[i].Width() > 155)
		{
			continue;
		}

		if (rect[i].Width() < 30)
			continue;
		if (rect[i].Height() > 76)
		{
			continue;
		}

		if (rect[i].Height() < 20)
			continue;

		if (float(rect[i].Width())/float(rect[i].Height()) >= 4)
		{
			continue;
		}

		if (float(rect[i].Width())/float(rect[i].Height()) <= 1.0)
			continue;

		n = i;

		double d[2000];
		ib.GetPixelCount(d,rect[i],0,true);

		int high;
		GetTheLowAndHighCountNumber(d,rect[i].Width(),0,0,0,&high);

		if (high < 3)
			continue;
		if (high > 10)
			continue;

		//if (high <6)
		//	continue;

		//if (high >9)
		//	continue;
		
		//if (high > 8)
		//	continue;

		//TRACE1("%d\n",high);

		//double nCur = ib.GetRectChangeNum(rect[i])*1.0/rect[i].Height();

		//if (nCur >dSum)
		//{
		//	dSum = nCur;
		//	n = i;
		//}

		//double dAve = ib.GetRectChangeNum(rect[i]) * 1.0 / (rect[i].Height());

		////if (dAve < 2.5)
		////{
		////	continue;
		////}

		double dAve = color.GetBlueMount(rect[i]) / double(rect[i].Width() * rect[i].Height());

		if (dAve < 0.33)
		{
			continue;
		}

		//if (dAve < 0.3)
		//{
		//	continue;
		//}
		
		nflag ++;
		n=i;

		varg[nflag] = Arg(i,dAve,double(rect[i].Width())/double(rect[i].Height()));

	}

	TRACE0("\n");

	if (n==0)
		return;

	if (nflag == 0)
	{
		if (rect[n].Width() < 3 * rect[n].Height())
		{
			rect[n].left -= (3*rect[n].Height()-rect[n].Width())/2;
			rect[n].right += (3*rect[n].Height()-rect[n].Width())/2;

			if (rect[n].left <= 0)
				rect[n].left = 0;

			if (rect[n].right >= biDoc.GetWidthPixel())
				rect[n].right = biDoc.GetWidthPixel();
		}

		rect[n].left -= PIC_EDGE;
		if (rect[n].left <= 0)
			rect[n].left = 0;

		biDoc.AttachFromFile(strfl);
		CImageBmp bi;
		bi.Resize(rect[n].Width(),rect[n].Height(),0);
		biDoc.Part(&bi,rect[n]);
		CString str;
		str.Format(L"E:\\test result\\%s_u.bmp",strfl.Mid(strfl.GetLength()-6,2));
		bi.SaveToFile(str);
	}

	if (nflag == 1)
	{
		if (rect[n].Width() < 3 * rect[n].Height())
		{
			rect[n].left -= (3*rect[n].Height()-rect[n].Width())/2;
			rect[n].right += (3*rect[n].Height()-rect[n].Width())/2;

			if (rect[n].left <= 0)
				rect[n].left = 0;

			if (rect[n].right >= biDoc.GetWidthPixel())
				rect[n].right = biDoc.GetWidthPixel();
		}

		rect[n].left -= PIC_EDGE;
		if (rect[n].left <= 0)
			rect[n].left = 0;

		biDoc.AttachFromFile(strfl);
		CImageBmp bi;
		bi.Resize(rect[n].Width(),rect[n].Height(),0);
		biDoc.Part(&bi,rect[n]);
		CString str;
		str.Format(L"E:\\test result\\%s_um.bmp",strfl.Mid(strfl.GetLength()-6,2));
		bi.SaveToFile(str);
	}

	if (nflag > 1)
	{
		int nCount = -1;
		double dMin = 10000;
		for(int i=1;i<nflag;i++)
		{
			double dCur = 1 - varg[i].color + fabs(varg[i].WHrate - 3)/3.0 + fabs(1.0 + rect[varg[i].n].top + rect[varg[i].n].Height()/2 - biDoc.GetHeight()/2)/double(biDoc.GetWidthPixel()) +  fabs(1.0 + rect[varg[i].n].left + rect[varg[i].n].Width()/2 - biDoc.GetWidthPixel()/2)/double(biDoc.GetHeight());
			if (dMin > dCur)
			{
				dMin = dCur;
				nCount = varg[i].n;
			}
		}

		if (rect[nCount].Width() < 3 * rect[nCount].Height())
		{
			rect[nCount].left -= (3*rect[nCount].Height()-rect[nCount].Width())/2;
			rect[nCount].right += (3*rect[nCount].Height()-rect[nCount].Width())/2;
			
			if (rect[nCount].left <= 0)
				rect[nCount].left = 0;

			if (rect[nCount].right >= biDoc.GetWidthPixel())
				rect[nCount].right = biDoc.GetWidthPixel();
		}


		rect[nCount].left -= PIC_EDGE;
		if (rect[nCount].left <= 0)
			rect[nCount].left = 0;

		biDoc.AttachFromFile(strfl);
		CImageBmp bi;
		bi.Resize(rect[nCount].Width(),rect[nCount].Height(),0);
		biDoc.Part(&bi,rect[nCount]);
		CString str;
		str.Format(L"E:\\test result\\%s_m.bmp",strfl.Mid(strfl.GetLength()-6,2));
		bi.SaveToFile(str);
	}

	//biDoc.AttachFromFile(strfl);
	//CImageBmp bi;
	//bi.Resize(rect[n].Width(),rect[n].Height(),0);
	//biDoc.Part(&bi,rect[n]);
	//bi.SaveToFile(L"C:\\Cool.bmp");

	//int ren[2000];
	//ib.GetChangeNum(ren,false,rect[n]);

	//int low,high;
	//GetTheLowAndHigh(ren,rect[n].Height(),&low,&high);
	//rect[n].top += low;
	//rect[n].bottom = rect[n].top + high

	GetActiveView()->Invalidate();
}

void CMainFrame::OnBluePlate()
{
	CRect rect;
	biDoc.ColorRect(&rect,RGB(0,0,255));
	CImageBmp bi;
	bi.ModalFrom(biDoc);
	bi.Resize(rect.Width(),rect.Height(),0);
	biDoc.ModalFrom(bi);
}

void CMainFrame::OnSplit()
{
	int nCount = 0;

	CRect rect[256];
	biDoc.ConnectionRegin(0,2);
	biDoc.GetConnectionReginRect(rect);

	for(int i=0;i<255;i++)
	{
		if (rect[i].left == 0 || rect[i].top == 0)
		{
			continue;
		}

		//if (rect[i].Width() < 5)
		//	continue;
		//if (rect[i].Width() > 40)
		//	continue;
		//if (rect[i].Height() > 40)
		//	continue;
		//if (rect[i].Height() < 16)
		//	continue;
		if (float(rect[i].Width())/float(rect[i].Height()) >= 2.5)
			continue;
		if (float(rect[i].Width())/float(rect[i].Height()) <= 0.15)
			continue;

		if (rect[i].Width() < biDoc.GetWidthPixel()*0.05)
			continue;
		if (rect[i].Width() > biDoc.GetWidthPixel()*0.3)
			continue;
		if (rect[i].Height() < biDoc.GetHeight()*0.3)
			continue;

		rect[i].InflateRect(1,1,1,1);

		CImageBmp biTemp;
		biDoc.AttachFromFile(strfl);
		biDoc.Part(&biTemp,rect[i]);
		
		CString str;
		str.Format(L"C:\\Tick\\Split_%d.bmp",nCount);
		nCount ++;
		biTemp.SaveToFile(str);
	}
}

void CMainFrame::OnRead()
{
	CFileFind ff;

	ff.FindFile(L"C:\\UPick\\*.*");

	double dMax = 0.0;
	CString strName;

	while(ff.FindNextFile())
	{
		if (ff.IsDots())
			continue;
		
		CImageBmp bi;
		bi.AttachFromFile(ff.GetFilePath());

		double d = biDoc.GetLikelyPercent(&bi);
		if (dMax <= d)
		{
			dMax = d;
			strName = ff.GetFileName();
		}
	}

	CString strDemo;
	strDemo.Format(L"这个图片是：%c.",strName[0]);
	AfxMessageBox(strDemo);
}

void CMainFrame::OnHistMid()
{
	CString str;
	str.Format(L"直方图中值：%d.",biDoc.GetMidHist(0.5));
	AfxMessageBox(str);
}

void CMainFrame::OnMinHist()
{
	BYTE aby[256];
	int n;
	biDoc.GetMinHist(aby,&n);

	CStdioFile flRe;
	flRe.Open(L"C:\\MinHist.txt",CFile::modeCreate | CFile::modeReadWrite);

	for(int i=0;i<n;i++)
	{
		CString str;
		str.Format(L"%d: %d.\n",i,aby[i]);

		flRe.WriteString(str);
	}

	flRe.Close();
}

void CMainFrame::OnHistMinTrans()
{
	BYTE aby[256];
	int n;
	biDoc.GetMinHist(aby,&n);

	biDoc.ThresholdTran(aby[0]);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnThreadHistTran()
{
	biDoc.ThresholdTran(biDoc.GetMidHist(0.75));

	GetActiveView()->Invalidate();
}

void CMainFrame::OnVproject()
{
	int left;
	int right;

	biDoc.VProject(&left,&right,12);

	CString str;
	str.Format(L"Left_%d:Right_%d",left,right);
	AfxMessageBox(str);
}

void CMainFrame::OnHproject()
{
	int top;
	int bottom;

	biDoc.HProject(&top,&bottom,12);

	CString str;
	str.Format(L"Left_%d:Right_%d",top,bottom);
	AfxMessageBox(str);
}

void CMainFrame::OnPreFindPlate()
{
	CRect rect;

	int a,b,c,d;

	biDoc.VProject(&a,&b,15);
	biDoc.HProject(&c,&d,15);

	biDoc.AttachFromFile(strfl);

	rect.left = a;
	rect.right = b;
	rect.top = c;
	rect.bottom = d;

	CImageBmp bi;
	biDoc.Part(&bi,rect);

	bi.SaveToFile(L"C:\\RealPlate.bmp");
}

void CMainFrame::OnSplitTrans()
{
	DoIt();

	biDoc.ThresholdTran(biDoc.GetMidHist(0.7));

	GetActiveView()->Invalidate();
}

void CMainFrame::OnSwallowHistTran()
{
	BYTE aby[256];
	int n;
	biDoc.GetMinHist(aby,&n);

	biDoc.ThresholdTran(aby[0]+30);

	GetActiveView()->Invalidate();
}

void CMainFrame::On32891()
{
	BYTE aby[256];
	int n;
	biDoc.GetMinHist(aby,&n);

	biDoc.ThresholdTran(aby[n/2] + 5);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnConRegin()
{
	DoIt();

	biDoc.ConnectionRegin(4,20);

	GetActiveView()->Invalidate();
}

void GetOutFromChar(char p,double *re)
{
	int n = 0;

	if (p >= '0' && p<='9')
	{
		n = p - '0';
	}
	else 
	{
		n = 10 + p - 'A';
	}
}

void SetDoubleArray(int n,double *re)
{
	n += 1;

	int d1,d2,d3,d4,d5,d6;

	d1 = n % 2;
	n /= 2;
	d2 = n % 2;
	n /= 2;
	d3 = n % 2;
	n /= 2;
	d4 = n % 2;
	n /= 2;
	d5 = n % 2;
	n /= 2;
	d6 = n % 2;
	n /= 2;

	if (d1 == 1)
		re[0] = 0.9;
	else
		re[0] = 0.1; 

	if (d2 == 1)
		re[1] = 0.9;
	else
		re[1] = 0.1; 

	if (d3 == 1)
		re[2] = 0.9;
	else
		re[2] = 0.1; 

	if (d4 == 1)
		re[3] = 0.9;
	else
		re[3] = 0.1; 

	if (d5 == 1)
		re[4] = 0.9;
	else
		re[4] = 0.1; 

	if (d6 == 1)
		re[5] = 0.9;
	else
		re[5] = 0.1; 
}

void DoTheNumberDoubleArray(int n,double *re)
{
	for(int i=0;i<36;i++)
	{
		re[i] = 0.05;
	}

	re[n] = 0.95;
}

void CMainFrame::OnBpNetTrain()
{
	double (*dIn)[MAX_IN_NUM] = new double[33][MAX_IN_NUM];
	double (*dOut)[MAX_OUT_NUM] =  new double[33][MAX_OUT_NUM];;

	CFileFind ff;
	ff.FindFile(L"C:\\UPick\\*.*");

	int i=0;
	while(ff.FindNextFile())
	{
		if (ff.IsDots())
			continue;

		CString str = ff.GetFileName();
		CImageBmp ib;
		ib.AttachFromFile(ff.GetFilePath());
		//ib.RightImage();
		ib.GetNetTrain(10,dIn[i]);
		SetDoubleArray(i,dOut[i]);
		
		for(int k=0;k<6;k++)
			dChar[i][k] = dOut[i][k];

		strChar[i] = (ff.GetFileName())[0];

		i++;
	}

	BpNet bn(32,65,6);

	for(int j=0;j<8000;j++)
	{
		bn.train(dIn,dOut,i-1);
	}

	bn.SaveToFile(L"C:\\Number.net");

	//CString se;
	//se.Format(L"%f",bn.e);
	//AfxMessageBox(se);
}

void CMainFrame::OnGridIt()
{
	for(int i=0;i<biDoc.GetWidthPixel();i++)
	{
		for(int j=0;j<biDoc.GetHeight();j++)
		{
			biDoc.SetPixel(i,j,biDoc.GetRectPixel(i/4,j/4,4));
		}
	}

	GetActiveView()->Invalidate();
}

void CMainFrame::OnNetSim()
{
	double d[MAX_IN_NUM];
	//biDoc.RightImage();

	biDoc.GetNetTrain(10,d);
	BpNet bn(32,65,6);

	bn.LoadFromFile(L"C:\\Number.net");
	double *b = bn.sim(d);

	int s[6];
	int sum = 0;

	double dMax = 10000;
	int n = -1;

	//for(int i=0;i<6;i++)
	//{
	//	if (b[i] > 0.6)
	//		baidu[i] = 0.9;
	//	else
	//		baidu[i] = 0.1;
	//}                                                                                                           

	for(int i=0;i<32;i++)
	{
		double dCur = 0.0;
		for(int j=0;j<6;j++)
		{
			dCur += fabs( dChar[i][j] - b[j] );
		}

		if (dMax > dCur)
		{
			n = i;
			dMax = dCur;
		}
	}

	CString str;
	str.Format(L"你选择的字符是：%c.",strChar[n]);
	AfxMessageBox(str);
}

void CMainFrame::OnNormal()
{
	CRect r;
	biDoc.GetRealRect(&r);

	biDoc.RectStretchAll(CRect(0,0,40,80),r);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnCheckd()
{
	int high,low;

	biDoc.ExceptBorder(255);
	biDoc.ThresholdTran(110);
	biDoc.SpaceEnhanceLaplace();

	int re[2000];
	biDoc.GetChangeNum(re,false,CRect(0,0,biDoc.GetWidthPixel(),biDoc.GetHeight()));
}

void CMainFrame::OnPicOut()
{
	CFileFind ff;
	ff.FindFile(L"C:\\Pick\\*.*");

	while(ff.FindNextFile())
	{
		if (ff.IsDots())
			continue;

		CImageBmp img;
		img.AttachFromFile(ff.GetFilePath());
		img.Reverse();
		CRect r;
		img.GetRealRect(&r);
		img.RectStretchAll(CRect(0,0,40,80),r);
		img.SaveToFile(CString(L"C:\\UPick\\")+ff.GetFileName());
	}
}

void CMainFrame::OnSimInit()
{
	CFileFind ff;
	ff.FindFile(L"C:\\UPick\\*.*");

	int i=0;
	while(ff.FindNextFile())
	{
		if (ff.IsDots())
			continue;

		CString str = ff.GetFileName();
		CImageBmp ib;
		ib.AttachFromFile(ff.GetFilePath());
		SetDoubleArray(i,dChar[i]);

		strChar[i] = (ff.GetFileName())[0];

		i++;
	}
}

void CMainFrame::OnOdTest()
{
	int *re = new int[biDoc.GetHeight()];
	biDoc.GetChangeNum(re,false,CRect(0,0,biDoc.GetWidthPixel(),biDoc.GetHeight()));

	CStdioFile flTest;
	flTest.Open(L"C:\\ChangeNum.txt",CFile::modeCreate | CFile::modeReadWrite);

	for(int i=0;i<biDoc.GetHeight();i++)
	{
		CString str;
		str.Format(L"ChangeNum_%d\n",re[i]);

		flTest.WriteString(str);
	}

	flTest.Close();
}

void CMainFrame::OnStrike()
{
	biDoc.SpaceEnhanceText();
	biDoc.ThresholdTran(biDoc.GetMidHist(0.9));
}

void CMainFrame::OnHistK()
{
	biDoc.ThresholdTran(biDoc.GetMidHistK(THE_K));

	GetActiveView()->Invalidate();
}

void CMainFrame::OnBluePassage()
{
	DoIt();

	biDoc.AttachFromFile(strfl);
	biDoc.BlueChange();

	GetActiveView()->Invalidate();
}

void CMainFrame::OnAlldo()
{
	srand(time(NULL));

	CFileFind ff;
	ff.FindFile(L"H:\\信息库\\纪念\\U 盘 （实训）\\VSS\\车牌识别系统图片库bmp\\01初出茅庐");

	int n = 0;
	while(ff.FindNextFile())
	{
		if (ff.IsDots())
			continue;

		biDoc.AttachFromFile(ff.GetFilePath());
		biDoc.ChangeDIBToGray();

		strfl = ff.GetFilePath();

		CString strRe;
		CString strFn = ff.GetFileName();

		OnText();
		OnHistK();
		OnSquareOpen();
		OnSquareClose();
		OnReverse();
		OnExceptBorder();
		OnPlateFind();
	}
}

void CMainFrame::OnSizeFit()
{
	
}

void CMainFrame::OnRename()
{
	CFileFind ff;

	ff.FindFile(L"C:\\test\\*.*");

	int n=0;
	while(ff.FindNextFile())
	{
		CString str;
		str.Format(L"C:\\test\\%d.bmp",n);

		MoveFile(ff.GetFilePath(),str);
		n++;
	}
}

void CMainFrame::OnBinaryPic()
{
	CFileFind ff;
	ff.FindFile(L"C:\\test\\*.*");

	int n=0;
	while(ff.FindNextFile())
	{
		if (ff.IsDots())
			continue;

		int se[3][3] = {{-1,1,-1},{-1,1,-1},{-1,1,-1}};

		CImageBmp ib;
		ib.AttachFromFile(ff.GetFilePath());
		ib.ThresholdTran(ib.GetMidHist(0.6));
		CString str;
		str.Format(L"C:\\test result\\%d.bmp",n);

		ib.Open(se);
		ib.SaveToFile(str);

		n++;
	}

}
void CMainFrame::OnRightImage()
{
	DoIt();

	biDoc.RightImage();

	GetActiveView()->Invalidate();
}

void CMainFrame::OnGjyTrain()
{
	double (*dIn)[MAX_IN_NUM] = new double[33][MAX_IN_NUM];
	double (*dOut)[MAX_OUT_NUM] =  new double[33][MAX_OUT_NUM];;

	CFileFind ff;
	ff.FindFile(L"C:\\Pick\\*.*");

	int i=0;
	while(ff.FindNextFile())
	{
		if (ff.IsDots())
			continue;

		CString str = ff.GetFileName();
		CImageBmp ib;
		ib.AttachFromFile(ff.GetFilePath());
		ib.Reverse();
		ib.RectStretchAll(CRect(0,0,ib.GetWidthByte(),ib.GetHeight()),CRect(0,0,80,160));

		ib.ChangeToNormalNetTrain(0.4);
		CRect r;
		ib.GetRealRect(&r);
		ib.RectStretchAll(CRect(0,0,80,160),r);
		ib.GetNetTrain(20,dIn[i]);
		SetDoubleArray(i,dOut[i]);

		for(int k=0;k<6;k++)
			dChar[i][k] = dOut[i][k];

		strChar[i] = (ff.GetFileName())[0];

		i++;
	}

	BpNet bn(32,65,6);

	for(int j=0;j<50000;j++)
	{
		bn.train(dIn,dOut,i-1);
	}

	bn.SaveToFile(L"C:\\Number.net");
}

void CMainFrame::OnGjyReg()
{
	double d[MAX_IN_NUM];
	//biDoc.RightImage();

	CRect r;
	biDoc.GetRealRect(&r);
	biDoc.RectStretchAll(CRect(0,0,80,160),r);
	biDoc.ChangeToNormalNetTrain(0.4);
	
	biDoc.GetRealRect(&r);
	biDoc.RectStretchAll(CRect(0,0,80,160),r);
	GetActiveView()->Invalidate();
	biDoc.GetNetTrain(20,d);
	BpNet bn(32,65,6);

	bn.LoadFromFile(L"C:\\Number.net");
	double *b = bn.sim(d);

	int s[6];
	int sum = 0;

	double dMax = 10000;
	int n = -1;

	//for(int i=0;i<6;i++)
	//{
	//	if (b[i] > 0.6)
	//		baidu[i] = 0.9;
	//	else
	//		baidu[i] = 0.1;
	//}                                                                                                           

	for(int i=0;i<32;i++)
	{
		double dCur = 0.0;
		for(int j=0;j<6;j++)
		{
			dCur += fabs( dChar[i][j] - b[j] );
		}

		if (dMax > dCur)
		{
			n = i;
			dMax = dCur;
		}
	}

	CString str;
	str.Format(L"你选择的字符是：%c.",strChar[n]);
	AfxMessageBox(str);
}

void CMainFrame::OnSign()
{
	DoIt();

	biDoc.ChangeToNormalNetTrain(0.4);

	GetActiveView()->Invalidate();
}

void CMainFrame::OnNewSimIniti()
{
	CFileFind ff;
	ff.FindFile(L"C:\\Pick\\*.*");

	int i=0;
	while(ff.FindNextFile())
	{
		if (ff.IsDots())
			continue;

		CString str = ff.GetFileName();
		CImageBmp ib;
		ib.AttachFromFile(ff.GetFilePath());
		SetDoubleArray(i,dChar[i]);

		strChar[i] = (ff.GetFileName())[0];

		i++;
	}
}

void CMainFrame::On32934()
{
	double dCur = 0.0;
	dCur = double(biDoc.GetPixelCount(0,110)) / double(biDoc.GetWidthPixel()*biDoc.GetHeight());
	
	CString str;
	str.Format(L"%lf",dCur);
	AfxMessageBox(str);
}

void CMainFrame::On32935()
{
	DoIt();

	BYTE by = biDoc.GetMidHist(0.7);
	biDoc.ThresholdTran(biDoc.GetMidHist(0.7));

	GetActiveView()->Invalidate();
}

void CMainFrame::OnRealDo()
{
	CFileFind ff;
	ff.FindFile(L"C:\\test result\\*.*");

	CStdioFile fl;
	fl.Open(L"C:\\InkePiso.txt",CFile::modeCreate | CFile::modeReadWrite);

	while(ff.FindNextFile())
	{
		for(int i=0;i<10;i++)
		{
			CString s;
			s.Format(L"C:\\Tick\\Split_%d.bmp",i);
			DeleteFile(s);
		}

		if (ff.IsDots())
			continue;

		biDoc.AttachFromFile(ff.GetFilePath());
		strfl = ff.GetFilePath();
		biDoc.ChangeDIBToGray();
		biDoc.ThresholdTran(biDoc.GetMidHist(0.67));

		biDoc.Reverse();
		biDoc.ExceptBorder(255);

		int nCount = 0;

		CRect rect[256];
		biDoc.ConnectionRegin(0,2);
		biDoc.GetConnectionReginRect(rect);

		for(int i=0;i<255;i++)
		{
			if (rect[i].left == 0 || rect[i].top == 0)
			{
				continue;
			}

			if (float(rect[i].Width())/float(rect[i].Height()) >= 2.5)
				continue;
			if (float(rect[i].Width())/float(rect[i].Height()) <= 0.15)
				continue;

			if (rect[i].Width() < biDoc.GetWidthPixel()*0.02)
				continue;
			if (rect[i].Width() > biDoc.GetWidthPixel()*0.3)
				continue;
			if (rect[i].Height() < biDoc.GetHeight()*0.3)
				continue;

			rect[i].InflateRect(1,1,1,1);

			CImageBmp biTemp;
			biDoc.AttachFromFile(strfl);
			biDoc.Part(&biTemp,rect[i]);

			CString str;
			str.Format(L"C:\\Tick\\Split_%d.bmp",nCount);
			nCount ++;
			biTemp.SaveToFile(str);
		}

		CFileFind ffn;
		ffn.FindFile(L"C:\\Tick\\*.*");

		CString strRe;

		while(ffn.FindNextFile())
		{
			if (ffn.IsDots())
				continue;

			CImageBmp ibTick;
			ibTick.AttachFromFile(ffn.GetFilePath());
			ibTick.ChangeDIBToGray();
			ibTick.ThresholdTran(biDoc.GetMidHist(0.7));
			ibTick.Reverse();

			CFileFind fft;

			fft.FindFile(L"C:\\OrgIn\\*.*");

			double dMax = 0.0;
			CString strName;

			while(fft.FindNextFile())
			{
				if (fft.IsDots())
					continue;

				CImageBmp bi;
				bi.AttachFromFile(fft.GetFilePath());

				double d = ibTick.GetLikelyPercent(&bi);
				if (dMax <= d)
				{
					dMax = d;
					strName = fft.GetFileName();
				}
			}

			strRe = strRe + strName[0];
		}

		CString strNew;
		strNew.Format(L"%s\t:\t:%s\n",strfl.Mid(strfl.GetLength()-8,2),strRe);

		fl.WriteString(strNew);
	}

	fl.Close();
}
