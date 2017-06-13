#ifndef _CPROJECTOR_H_
#define _CPROJECTOR_H_

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "StaticParameters.h"

using namespace std;
using namespace cv;

// ͶӰ�ǿ����ࡣ���ڿ���ͶӰ��Ͷ��Ҫ���ͼ����
// ���ã�InitProjector��presentPicture��CloseProjector
class CProjector
{
private:
	int m_resRow;			// ͶӰ�ǵ��зֱ���
	int m_resLine;			// ͶӰ�ǵ��зֱ���
	int m_biasRow;			// ���ڵ���ƫ�Ʒֱ���
	int m_biasLine;			// ���ڵ���ƫ�Ʒֱ���
	string m_winName;		// ��������

public:
	CProjector();			// ���캯��
	~CProjector();			// ��������
	bool InitProjector();							// ��ʼ���豸
	bool presentPicture(Mat pic, int time);			// ʹͶӰ�Ƿų�pic����ʱtime
	bool presentPicture(uchar x, int time);			// ʹͶӰ�Ƿų�ȫΪx��ͼƬ����ʱtime
	bool CloseProjector();							// �ر��豸
};

#endif