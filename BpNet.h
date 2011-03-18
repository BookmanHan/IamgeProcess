// BpNet.h: interface for the BpNet class.
// 2005.1.13－16：02 By Superman 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BPNET_H__3A171E41_1E8B_11D7_BA67_936006E9954B__INCLUDED_)
#define AFX_BPNET_H__3A171E41_1E8B_11D7_BA67_936006E9954B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SAMPLE_NUM 200

#define MAX_IN_NUM 200
#define MAX_HIDE_NUM 300
#define MAX_OUT_NUM 20

class BpNet  
{
public:
	double tjpt[MAX_OUT_NUM];
	double shuchu[MAX_OUT_NUM];
	double m[MAX_OUT_NUM];
	double * sim(double *psim);
	void train(double p[][MAX_IN_NUM],double t[][MAX_OUT_NUM],int samplenum);

	BpNet(int argIn,int argHide,int argOut);

	virtual ~BpNet();

	int inNum;  //输入结点数
	int hideNum;//隐含结点数
	int outNum;  //输出结点数

	double x[SAMPLE_NUM]; //输入向量
	double x1[SAMPLE_NUM];//隐含结点状态值
	double x2[SAMPLE_NUM];//输出结点状态值
	double o1[SAMPLE_NUM];//隐含层激活值
	double o2[SAMPLE_NUM];//输出层激活值
	double w[MAX_IN_NUM][MAX_HIDE_NUM];//隐含结点权值
	double w1[MAX_HIDE_NUM][MAX_OUT_NUM];//输出结点权值
	double rate_w; //权值学习率（输入层-隐含层)
	double rate_w1;//权值学习率 (隐含层-输出层)
	double rate_b1;//隐含层阀值学习率
	double rate_b2;//输出层阀值学习率
	double b1[MAX_HIDE_NUM];//隐含结点阀值
	double b2[MAX_OUT_NUM];//输出结点阀值
	double pp[SAMPLE_NUM];
	double qq[SAMPLE_NUM];
	double yd[SAMPLE_NUM];//希望输出值
	double e;
	double error;//允许的最大误差
public:
	void SaveToFile(CString str);
public:
	void LoadFromFile(CString str);
};

#endif // !defined(AFX_BPNET_H__3A171E41_1E8B_11D7_BA67_936006E9954B__INCLUDED_)
