#ifndef _CDECODEPHASE_H_
#define _CDECODEPHASE_H_

#include <opencv2/opencv.hpp>
#include "CVisualization.h"
#include "GlobalFunction.h"
#include "StaticParameters.h"
using namespace cv;

// ���ƽ������������Ѿ���д�õ������롣
// ����Ϊһ��Ҷ�ͼ�����Ϊһ�ŻҶ�ͼ��ÿһ��洢����projector�е�ƫ�����ꡣ
class CDecodePhase
{
private:
	int m_numMat;		// ͼƬ��Ŀ��Ĭ��Ϊ2��
	int m_pixPeroid;	// �������ڡ�
	int m_resRow;				// ͼ����зֱ���
	int m_resLine;			// ͼ����зֱ���

	Mat * m_grePicture;	// ����ĻҶ�ͼ
	Mat m_result;		// ���

	CVisualization * m_visual;	// ������ʾ�м���

	bool AllocateSpace();		// Ϊ����ľ�������ռ�
	bool DeleteSpace();			// ɾ�����пռ�
	bool CountResult();			// ���ݹ�һ���Ҷ�ͼͳ�ƽ��
	bool Visualize();			// ��ʾ�м���

public:
	CDecodePhase();
	~CDecodePhase();

	bool Decode();
	Mat GetResult();

	bool SetMat(int num, Mat pic);						// ������Ӧ�Ҷ�ͼ
	bool SetNumMat(int numDigit, int pixperiod);			// ���ò���
};

#endif