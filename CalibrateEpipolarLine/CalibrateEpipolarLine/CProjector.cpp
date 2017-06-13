#include "CProjector.h"

// ���캯������ʼ��һЩ����
CProjector::CProjector()
{
	this->m_resRow = PROJECTOR_RESROW;
	this->m_resLine = PROJECTOR_RESLINE;
	this->m_biasLine = PC_BIASLINE;
	this->m_biasRow = PC_BIASROW;
	this->m_winName = "Projector";
}

// �����������ݿա�
CProjector::~CProjector()
{

}

// ��ʼ���豸����Ҫ����һ�����ڣ������������ں���λ�ã��������졣
bool CProjector::InitProjector()
{
	using namespace cv;

	// ��������
	namedWindow(this->m_winName, WINDOW_NORMAL);

	// �������ڴ�С��λ��
	setWindowProperty(this->m_winName, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	resizeWindow(this->m_winName, this->m_resLine, this->m_resRow);
	moveWindow(this->m_winName, PC_BIASLINE, PC_BIASROW);

	return true;
}

// �ر��豸�����ٴ��ڡ�
bool CProjector::CloseProjector()
{
	using namespace cv;

	destroyWindow(this->m_winName);

	return true;
}

// ��ӳ��ʹͶӰ�Ƿ�ӳ��pic����ʹ�����timeʱ�䡣time��λΪms��
bool CProjector::presentPicture(cv::Mat pic, int time)
{
	using namespace cv;

	// �������Ƿ�Ϸ�
	/*if (pic.size() != Mat::MSize(this->m_resRow, this->m_resLine))
	{
	std::cout << (pic.size()).width() << std::endl;
	std::cout << pic.size()
	return false;
	}*/

	imshow(this->m_winName, pic);
	waitKey(time);

	return true;
}

bool CProjector::presentPicture(uchar x, int time)
{
	using namespace cv;

	Mat pic;
	pic.create(this->m_resRow, this->m_resLine, CV_8UC1);
	pic.setTo(x);

	imshow(this->m_winName, pic);
	waitKey(time);

	return true;
}