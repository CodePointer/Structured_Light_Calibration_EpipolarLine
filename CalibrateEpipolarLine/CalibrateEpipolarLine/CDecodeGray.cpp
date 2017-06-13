#include "CDecodeGray.h"

// ���캯�����趨һЩĬ�����á�
CDecodeGray::CDecodeGray()
{
	this->m_numDigit = 0;
	this->m_grayCodeSize = 0;
	this->m_gray2bin = NULL;
	this->m_codeFileName = "";
	this->resRow = CAMERA_RESROW;
	this->resLine = CAMERA_RESLINE;
	this->m_grePicture = NULL;
	this->m_binPicture = NULL;
	this->m_visual = NULL;
}

// ����������ȷ���ͷſռ䡣
CDecodeGray::~CDecodeGray()
{
	this->ReleaseSpace();
}

// ������Ӧ�Ҷ�ͼ
bool CDecodeGray::SetMat(int num, Mat pic)
{
	if (this->m_grePicture == NULL)
	{
		ErrorHandling("CDecodeGray.SetMat->grePicture Space is not allocated.");
		return false;
	}
	pic.copyTo(this->m_grePicture[num]);
	return true;
}

// ���ø�����λ��
bool CDecodeGray::SetNumDigit(int numDigit, bool ver)
{
	// �����жϲ����Ƿ�Ϸ�
	if ((numDigit <= 0) || (numDigit > 16))
		return false;
	
	// ����
	this->m_numDigit = numDigit;
	this->m_grayCodeSize = 1 << (this->m_numDigit);
	this->m_vertical = ver;

	// ������Ӧ�ռ�
	if ((this->m_gray2bin != NULL) || (this->m_grePicture != NULL) || (this->m_binPicture != NULL))
		this->ReleaseSpace();
	this->AllocateSpace();

	return true;
}

// ���ô洢��������ļ���
bool CDecodeGray::SetMatFileName(std::string codeFilePath,
	std::string codeFileName)
{
	this->m_codeFilePath = codeFilePath;
	this->m_codeFileName = codeFileName;
	return true;
}

// Ϊ����ľ���short������������ռ�
bool CDecodeGray::AllocateSpace()
{
	using namespace cv;

	// �жϲ����Ƿ�Ϸ�
	if ((this->m_numDigit <= 0) || (this->m_numDigit > 16))
		return false;

	// ����ռ䲢��ʼ��
	this->m_gray2bin = new short[this->m_grayCodeSize];
	this->m_grePicture = new Mat[this->m_numDigit * 2];
	this->m_binPicture = new Mat[this->m_numDigit];

	return true;
}

// ɾ�����пռ�
bool CDecodeGray::ReleaseSpace()
{
	if (this->m_gray2bin != NULL)
	{
		delete[](this->m_gray2bin);
		this->m_gray2bin = NULL;
	}
	if (this->m_grePicture != NULL)
	{
		delete[](this->m_grePicture);
		this->m_grePicture = NULL;
	}
	if (this->m_binPicture != NULL)
	{
		delete[](this->m_binPicture);
		this->m_binPicture = NULL;
	}
	if (this->m_visual != NULL)
	{
		delete(this->m_visual);
		this->m_visual = NULL;
	}
	return true;
}

// ���롣
bool CDecodeGray::Decode()
{
	using namespace cv;

	// ��������ļ�
	std::ifstream codeFile;
	codeFile.open(this->m_codeFilePath + this->m_codeFileName, std::ios::in);
	if (!codeFile)
	{
		ErrorHandling("Gray Decode->Open file error.");
		return false;
	}
	for (int i = 0; i < this->m_grayCodeSize; i++)
	{
		int binCode, grayCode;
		codeFile >> binCode >> grayCode;
		this->m_gray2bin[grayCode] = binCode;
	}

	// �ֱ������еĻҶ�ͼ������ת��Ϊ��ֵ����
	if (!this->Grey2Bin())
		return false;

	// ���ݶ�ֵͼ�ָ�����
	if (!this->CountResult())
		return false;

	// ���ӻ�����
	this->Visualize();

	return true;
}

// ��ȡ������
cv::Mat CDecodeGray::GetResult()
{
	Mat result;
	this->m_result.copyTo(result);
	return result;
}

// ���Ҷ�ͼ�ӹ�Ϊ��ֵͼ���Ա��һ������
bool CDecodeGray::Grey2Bin()
{
	using namespace cv;

	// ���ÿһ��binPicture
	for (int binIdx = 0; binIdx < this->m_numDigit; binIdx++)
	{
		// �趨��ֵ
		Mat tempMat1, tempMat2;
		tempMat1 = this->m_grePicture[binIdx * 2];
		tempMat2 = this->m_grePicture[binIdx * 2 + 1];
		uchar threshold = 5;

		// ��ȡ����ͼƬ�Ĳ�
		Mat tempMat;
		tempMat = this->m_grePicture[binIdx * 2] - this->m_grePicture[binIdx * 2 + 1];
		
		// �������ֵ���ж�ֵ��
		this->m_binPicture[binIdx].create(Size(this->resLine, this->resRow), CV_8UC1);
		for (int i = 0; i < this->resRow; i++)
		{
			for (int j = 0; j < this->resLine; j++)
			{
				uchar value1 = tempMat1.at<uchar>(i, j);
				uchar value2 = tempMat2.at<uchar>(i, j);
				if ((double)value1 - (double)value2 >(double)threshold)
				{
					this->m_binPicture[binIdx].at<uchar>(i, j) = 255;
				}
				else if ((double)value2 - (double)value1 >(double)threshold)
				{
					this->m_binPicture[binIdx].at<uchar>(i, j) = 0;
				}
				else
				{
					this->m_binPicture[binIdx].at<uchar>(i, j) = 128;
				}
			}
		}
	}
	return true;
}

// ���ݶ�ֵͼͳ�ƽ��
bool CDecodeGray::CountResult()
{
	double pixPeriod = 0;
	if (this->m_vertical)
		pixPeriod = PROJECTOR_RESLINE / this->m_grayCodeSize;
	else
		pixPeriod = PROJECTOR_RESROW / this->m_grayCodeSize;

	m_result.create(resRow, resLine, CV_64FC1);
	for (int i = 0; i < this->resRow; i++)
	{
		for (int j = 0; j < this->resLine; j++)
		{
			short grayCode = 0;
			bool flag = true;
			for (int binIdx = 0; binIdx < this->m_numDigit; binIdx++)
			{
				if (this->m_binPicture[binIdx].at<uchar>(i, j) == 128)
				{
					flag = false;
					break;
				}
				if (this->m_binPicture[binIdx].at<uchar>(i, j) == 255)
				{
					grayCode += 1 << binIdx;
				}
			}
			if (flag)
			{
				this->m_result.at<double>(i, j) = (double)this->m_gray2bin[grayCode] * pixPeriod;
			}
			else
			{
				this->m_result.at<double>(i, j) = -100.0;
			}
		}
	}// ���ÿ�������ֵ���������/���ϲ��ֵ
	return true;
}

// �ڼ��������ʾͼƬ
bool CDecodeGray::Visualize()
{
	if (VISUAL_DEBUG)
	{
		// ����಻���ڣ��򴴽�һ��
		if (this->m_visual == NULL)
		{
			this->m_visual = new CVisualization("GrayDecoder");
		}

		// ��ʾ�м�Ķ�ֵ��ͼ��
		for (int i = 0; i < this->m_numDigit; i++)
		{
			this->m_visual->Show(this->m_binPicture[i], 300);
		}

		// ��ʾ���ս���������һ����
		this->m_visual->Show(this->m_result, 1000, true);
	}
	return true;
}