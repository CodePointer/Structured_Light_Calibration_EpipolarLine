#ifndef _CDECODEGRAY_H_
#define _CDECODEGRAY_H_

#include <string>
#include <strstream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "CVisualization.h"
#include "StaticParameters.h"
#include "GlobalFunction.h"

using namespace std;
using namespace cv;

// ������������������Ѿ���д�õĸ����롣
// ����Ϊһ��Ҷ�ͼ�����Ϊһ�ŻҶ�ͼ��ÿһ��洢����projector�е����ꡣ
// ʹ��ǰ��Ҫ�ֱ����4��Set�������Ρ�
class CDecodeGray
{
private:
	int m_numDigit;			// λ��
	int m_grayCodeSize;		// �ܹ��ĸ�������Ŀ
	short * m_gray2bin;		// �����뵽���������ת��
	string m_codeFilePath;	// �洢��������ļ�·��
	string m_codeFileName;	// �洢��������ļ���
	int resRow;				// ͼ����зֱ���
	int resLine;			// ͼ����зֱ���
	bool m_vertical;		// �趨�����뷽��

	Mat * m_grePicture;	// ����ĻҶ�ͼ
	Mat * m_binPicture;	// �ӹ���Ķ�ֵͼ
	Mat m_result;		// ���

	CVisualization * m_visual;	// ������ʾ�м���

	bool AllocateSpace();		// Ϊ����ľ���short������������ռ�
	bool ReleaseSpace();		// ɾ�����пռ�
	bool Grey2Bin();			// ���Ҷ�ͼ�ӹ�Ϊ��ֵͼ���Ա��һ������
	bool CountResult();			// ���ݶ�ֵͼͳ�ƽ��
	bool Visualize();			// ��ʾ�м���

public:
	CDecodeGray();
	~CDecodeGray();

	bool Decode();
	Mat GetResult();

	bool SetMat(int num, Mat pic);						// ������Ӧ�Ҷ�ͼ
	bool SetNumDigit(int numDigit, bool ver);				// ���ø�����λ��
	bool SetMatFileName(string codeFilePath,
		string codeFileName);			// ���ô洢��������ļ���
};

#endif