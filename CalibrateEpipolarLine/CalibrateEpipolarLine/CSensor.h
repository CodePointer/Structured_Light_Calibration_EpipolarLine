#ifndef _CSENSOR_H_
#define _CSENSOR_H_

#include <string>
#include <opencv2/opencv.hpp>
#include <strstream>
#include "CCamera.h"
#include "CProjector.h"
#include "StaticParameters.h"
#include "GlobalFunction.h"
#include "CStorage.h"

using namespace std;
using namespace cv;

// ������ģ�顣
class CSensor
{
private:
	// ��ͶӰͼ���洢��
	int m_patternNum;
	int m_nowNum;
	string m_filePath;
	string m_fileName;
	string m_fileSuffix;
	Mat * m_patterns;

	// �豸����
	CCamera * m_camera;
	CProjector * m_projector;

public:
	CSensor();
	~CSensor();

	// ��ʼ��������
	bool InitSensor();

	// �رմ�����
	bool CloseSensor();

	// ��ȡͼ��
	bool LoadPatterns(int patternNum, string filePath, string fileName, string fileSuffix);

	// �ͷ��Ѷ�ȡͼ��
	bool UnloadPatterns();

	// ����ͶӰ��ͶӰ��ͼ�񣬴�0����
	bool SetProPicture(int nowNum = 0);
	
	// ��ȡ���ͼ��
	Mat GetCamPicture();

	// ��ȡͶӰ��ͶӰ��ͼ��
	Mat GetProPicture();
	
};

#endif