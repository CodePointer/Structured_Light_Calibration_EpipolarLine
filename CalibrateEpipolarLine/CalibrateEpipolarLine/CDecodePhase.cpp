#include "CDecodePhase.h"

CDecodePhase::CDecodePhase()
{
	this->m_numMat = 0;
	this->m_pixPeroid = 16;
	this->m_resRow = CAMERA_RESROW;
	this->m_resLine = CAMERA_RESLINE;
	this->m_grePicture = NULL;
	this->m_visual = NULL;
}

CDecodePhase::~CDecodePhase()
{
	this->DeleteSpace();
}

// Ϊ����ľ�������ռ�
bool CDecodePhase::AllocateSpace()
{
	// �жϲ����Ƿ�Ϸ�
	if (this->m_numMat <= 0)
		return false;

	// ����ռ䲢��ʼ��
	this->m_grePicture = new Mat[this->m_numMat];

	return true;
}

// ɾ�����пռ�
bool CDecodePhase::DeleteSpace()
{
	if (this->m_grePicture != NULL)
	{
		delete[](this->m_grePicture);
		this->m_grePicture = NULL;
	}
	if (this->m_visual != NULL)
	{
		delete(this->m_visual);
		this->m_visual = NULL;
	}
	return true;
}

// ���ݻҶ�ͼͳ�ƽ��
bool CDecodePhase::CountResult()
{
	this->m_result.create(this->m_resRow, this->m_resLine, CV_64FC1);
	Mat SinValue0;
	Mat SinValue1;

	for (int i = 0; i < this->m_resRow; i++)
	{
		for (int j = 0; j < this->m_resLine; j++)
		{
			// �Ȼ�ȡ�õ�Ҷ�ֵ
			float greyValue0 = this->m_grePicture[0].at<uchar>(i, j);	// greyValue0 = (sin(x) + 1) * 127;
			float greyValue1 = this->m_grePicture[1].at<uchar>(i, j);	// greyValue1 = (sin(x + CV_PI / 2) + 1) * 127
			float greyValue2 = this->m_grePicture[2].at<uchar>(i, j);	// greyValue2 = (sin(x + CV_PI) + 1) * 127
			float greyValue3 = this->m_grePicture[3].at<uchar>(i, j);	// greyValue3 = (sin(x + 3 * CV_PI / 2) + 1) * 127
			// ���Ҷ�ֵת��Ϊsinֵ
			float sinValue = (greyValue0 - greyValue2) / 2;
			float cosValue = (greyValue1 - greyValue3) / 2;
			// ����x
			float x = cvFastArctan(sinValue, cosValue);			
			// ����ƫ����
			float pix = (x) / (360)*(double)(this->m_pixPeroid);
			pix += 0.5;
			if (pix > this->m_pixPeroid)
			{
				pix -= this->m_pixPeroid;
			}
			this->m_result.at<double>(i, j) = (double)pix;
		}
	}

	return true;
}

// ����
bool CDecodePhase::Decode()
{
	// ���ݻҶ�ͼ�ָ�����
	if (!this->CountResult())
	{
		ErrorHandling("CDecodePhase.Decode()->CountResult fault");
		return false;
	}

	// ���ӻ�
	this->Visualize();

	return true;
}

// ��ȡ������
Mat CDecodePhase::GetResult()
{
	Mat result;
	this->m_result.copyTo(result);
	return result;
}

// ������Ӧ�Ҷ�ͼ
bool CDecodePhase::SetMat(int num, cv::Mat pic)
{
	if (this->m_grePicture == NULL)
	{
		ErrorHandling("CDecodePhase.SetMat->grePicture Space is not allocated.");
		return false;
	}
	pic.copyTo(this->m_grePicture[num]);
	return true;
}

// ���ò���
bool CDecodePhase::SetNumMat(int numMat, int pixperiod)
{
	// �жϲ����Ƿ�Ϸ�
	if ((numMat <= 0))
		return false;

	// ����
	this->m_numMat = numMat;
	this->m_pixPeroid = pixperiod;

	// ����ռ�
	if ((this->m_grePicture != NULL))
	{
		this->DeleteSpace();
	}
	this->AllocateSpace();

	return true;
}

// �ڼ��������ʾͼƬ
bool CDecodePhase::Visualize()
{
	if (VISUAL_DEBUG)
	{
		// ����಻���ڣ��򴴽�һ��
		if (this->m_visual == NULL)
		{
			this->m_visual = new CVisualization("PhaseDecoder");
		}

		// ��ʾ�Ҷ�ͼ��
		for (int i = 0; i < this->m_numMat; i++)
		{
			this->m_visual->Show(this->m_grePicture[i], 300);
		}

		// ��ʾ���ս���������һ����
		this->m_visual->Show(this->m_result, 1000, true, 0.5);
	}
	return true;
}