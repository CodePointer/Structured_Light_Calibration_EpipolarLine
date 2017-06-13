#include "CCamera.h"

// ���캯����
// ����һЩ������������ҪΪprivate����ľ�̬����
CCamera::CCamera()
{
	this->sumDeviceNum = 0;
	this->nowDeviceNum = 0;
	this->resLine = 0;
	this->resRow = 0;
	this->BufferNum = 1;
	this->ppBuffer = NULL;
}

// ����������
// ��֤������Ѿ��Ͽ���
CCamera::~CCamera()
{
	if (this->nowDeviceNum != 0)
		EndHVDevice(this->DeviceHandle);
	if (this->ppBuffer != NULL)
	{
		for (int i = 1; i < BufferNum; i++)
			if (ppBuffer[i] != NULL)
			{
				delete[](ppBuffer[i]);
				ppBuffer[i] = NULL;
			}
		delete[]ppBuffer;
		ppBuffer = NULL;
	}
}

// ��ȡ�ܹ�����ͷ��Ŀ����ʼֵΪ0��
int CCamera::getSumDeviceNum()
{
	return this->sumDeviceNum;
}

// ��ȡ��ǰ�ɼ�������ͷ��š�Ϊ0������ʹ���е�����ͷ��
int CCamera::getNowDeviceNum()
{
	return this->nowDeviceNum;
}

// ��ʼ���豸�Ա��ɼ���
// ���������������ȡ�����Ϣ����䡣
bool CCamera::InitCamera()
{
	HVSTATUS status;

	this->resRow = CAMERA_RESROW;
	this->resLine = CAMERA_RESLINE;

	// ��ȡ�豸����
	int sumNum;
	status = HVGetDeviceTotal(&sumNum);
	HV_VERIFY(status);
	this->sumDeviceNum = sumNum;

	// ��1�������
	if (this->sumDeviceNum < 1)
		return false;
	status = BeginHVDevice(1, &(this->DeviceHandle));
	HV_VERIFY(status);
	this->nowDeviceNum = 1;

	// �����������ز���
	status = HVSetSnapSpeed(this->DeviceHandle, HIGH_SPEED);
	HV_VERIFY(status);

	// ����1���������ʽ
	status = HVSetResolution(this->DeviceHandle, RES_MODE0);
	HV_VERIFY(status);
	HVSetOutputWindow(DeviceHandle, 0, 0, this->resLine, this->resRow);
	HV_VERIFY(status);

	// ���뻺������
	this->ppBuffer = new BYTE*[this->BufferNum];
	for (int i = 0; i < this->BufferNum; i++)
	{
		ppBuffer[i] = new BYTE[this->resLine*this->resRow];
	}
	this->used = this->BufferNum;

	// ��ȡ����ͼ��֤�ȶ�
	int stableNum = 5;
	Mat tempMat;
	while (stableNum-- != 0)
	{
		this->getPicture(tempMat);
	}

	return true;
}

// ���������ȡһ��ͼ��
bool CCamera::SnapShot()
{
	HVSTATUS status;
	int maxAttempt = 30;
	int attempt = 0;

	// ��ȡͼƬ
	attempt = 0;
	status = HVSnapShot(this->DeviceHandle, this->ppBuffer, this->BufferNum);
	while ((status != STATUS_OK) && (attempt < maxAttempt))
	{
		attempt++;
		status = HVSnapShot(this->DeviceHandle, this->ppBuffer, this->BufferNum);
	}

	// �ж��Ƿ��ȡ��ͼ��
	if (attempt >= maxAttempt)
	{
		std::cout << HVGetErrorString(status) << std::endl;
		return false;
	}

	this->used = 0;
	return true;
}

// ��ͼ��ת��ΪMat��ʽ
bool CCamera::getPicture(cv::Mat & pic)
{
	// �鿴�������Ƿ���ʣ��ͼƬ
	if (this->used >= this->BufferNum)
	{
		if (!this->SnapShot())
			return false;
	}

	// ����Mat
	pic =Mat(Size(this->resLine, this->resRow), CV_8UC1, this->ppBuffer[this->used]);
	this->used++;	// ���»�������Ϣ

	return true;
}

// �ر�����ͷ��ֹ�ɼ���
bool CCamera::CloseCamera()
{
	HVSTATUS status;

	// �ر��������
	status = EndHVDevice(this->DeviceHandle);
	HV_VERIFY(status);

	// ���������
	for (int i = 0; i < BufferNum; i++)
		if (ppBuffer[i] != NULL)
		{
			delete[](ppBuffer[i]);
			ppBuffer[i] = NULL;
		}
	delete[]ppBuffer;
	ppBuffer = NULL;

	return true;
}