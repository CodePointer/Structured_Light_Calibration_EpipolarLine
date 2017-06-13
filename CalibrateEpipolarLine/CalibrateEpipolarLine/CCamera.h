#ifndef _CCAMERA_H_
#define _CCAMERA_H_

#include <opencv2/opencv.hpp>
#include <Windows.h>	// ����ͷ������Ҫ
#include <HVDAILT.h>
#include <HVDef.h>
#include <HVExtend.h>
#include <Raw2Rgb.h>	// ����ͷ������Ҫ
#include "StaticParameters.h"


using namespace std;
using namespace cv;

// ����ͷ�����ࡣ��ȡһ֡����ͷͼƬ����ʵ�ֶ�����ͷ�Ĺ����򿪡��رյȵȣ�
// Ĭ����ֻ��һ������ͷ��
// ���ã�InitCamera��getPicture��CloseCamera
class CCamera
{
private:
	int sumDeviceNum;			// �ܹ�������ͷ��Ŀ
	int nowDeviceNum;			// ��ǰʹ�õ�����ͷ��š�������ͷ��Ϊ0��
	HHV DeviceHandle;			// ����ͷ�豸���
	BYTE **ppBuffer;			// ͼ�񻺳���
	int BufferNum;				// ͼ�񻺳�����С����ͼƬ����
	int used;					// ����������ͼƬ����
	int resRow;					// ͼƬ�ֱ��ʣ���
	int resLine;				// ͼƬ�ֱ��ʣ���


public:
	CCamera();
	~CCamera();
	int getSumDeviceNum();		// ��ȡ�ܹ�����ͷ��Ŀ
	int getNowDeviceNum();		// ��ǰ����ͷ��
	bool InitCamera();			// ��ʼ���豸�Ա��ɼ�
	bool SnapShot();
	bool getPicture(Mat & pic);	// ��ȡһ֡ͼ�񣬲�����pic�С�
	bool CloseCamera();			// �ر�����ͷ����ֹ�ɼ�
};

#endif