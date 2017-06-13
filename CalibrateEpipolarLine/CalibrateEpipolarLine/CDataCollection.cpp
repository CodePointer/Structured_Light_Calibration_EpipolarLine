#include "CDataCollection.h"

CDataCollection::CDataCollection()
{
	this->sensor_manager_ = NULL;
	this->vgray_mats_ = NULL;
	this->vphase_mats_ = NULL;
	this->dyna_mats_ = NULL;
	this->ipro_mats_ = NULL;
	this->my_debug_ = NULL;

	return;
}


CDataCollection::~CDataCollection()
{
	if (this->sensor_manager_ != NULL)
	{
		delete this->sensor_manager_;
		this->sensor_manager_ = NULL;
	}
	if (this->vgray_mats_ != NULL)
	{
		delete this->vgray_mats_;
		this->vgray_mats_ = NULL;
	}
	if (this->hgray_mats_ != NULL)
	{
		delete this->hgray_mats_;
		this->hgray_mats_ = NULL;
	}
	if (this->vphase_mats_ != NULL)
	{
		delete this->vphase_mats_;
		this->vphase_mats_ = NULL;
	}
	if (this->hphase_mats_ != NULL)
	{
		delete this->hphase_mats_;
		this->hphase_mats_ = NULL;
	}
	if (this->dyna_mats_ != NULL)
	{
		delete this->dyna_mats_;
		this->dyna_mats_ = NULL;
	}
	if (this->ipro_mats_ != NULL)
	{
		delete this->ipro_mats_;
		this->ipro_mats_ = NULL;
	}
	if (this->jpro_mats_ != NULL)
	{
		delete this->jpro_mats_;
		this->jpro_mats_ = NULL;
	}
	if (this->my_debug_ != NULL)
	{
		delete this->my_debug_;
		this->my_debug_ = NULL;
	}

	return;
}


bool CDataCollection::Init()
{
	bool status = true;

	// �ֶ�����
	this->flag_ground_truth_ = true;
	this->max_frame_num_ = 100;
	this->total_points_num_ = 10;

	// ���ڲ����趨
	this->visualize_flag_ = true;
	this->storage_flag_ = true;

	// ͼ��·��������
	this->pattern_path_ = "Patterns/";
	this->vgray_name_ = "vGray";
	this->hgray_name_ = "hGray";
	this->gray_suffix_ = ".bmp";
	this->vgray_code_name_ = "vGrayCode";
	this->hgray_code_name_ = "hGrayCode";
	this->gray_code_suffix_ = ".txt";
	this->vphase_name_ = "vPhase";
	this->hphase_name_ = "hPhase";
	this->phase_suffix_ = ".bmp";
	this->dyna_name_ = "randDot";
	this->dyna_suffix_ = ".bmp";
	this->wait_name_ = "wait";
	this->wait_suffix_ = ".bmp";

	// �洢·��������
	this->save_data_path_ = "E:/Structured_Light_Data/20170613/";
	this->dyna_frame_path_ = "dyna/";
	this->dyna_frame_name_ = "dyna_mat";
	this->dyna_frame_suffix_ = ".png";
	this->pro_frame_path_ = "pro/";
	this->ipro_frame_name_ = "ipro_mat";
	this->jpro_frame_name_ = "jpro_mat";
	this->ipro_frame_suffix_ = ".png";

	// ��ʼ��������
	if (status)
	{
		this->sensor_manager_ = new CSensor;
		status = this->sensor_manager_->InitSensor();
		if (!status)
		{
			ErrorHandling("CDataCollection::Init(), InitSensor failed.");
		}
	}

	// ��ʼ���洢�ռ�
	this->vgray_mats_ = new Mat[GRAY_V_NUMDIGIT * 2];
	this->hgray_mats_ = new Mat[GRAY_H_NUMDIGIT * 2];
	this->vphase_mats_ = new Mat[PHASE_NUMDIGIT];
	this->hphase_mats_ = new Mat[PHASE_NUMDIGIT];
	this->dyna_mats_ = new Mat[this->max_frame_num_];
	this->ipro_mats_ = new Mat[this->max_frame_num_];
	this->jpro_mats_ = new Mat[this->max_frame_num_];
	this->my_debug_ = new CVisualization("Debug");

	return status;
}


bool CDataCollection::CollectData()
{
	bool status = true;
	

	// ѭ���ɼ�����
	int nowGroupIdx = 0;
	int maxGroupNum = 1;
	cout << "Begin Collection." << endl;
	this->points_get_num_.create(Size(CAMERA_RESLINE, CAMERA_RESROW), CV_8UC1);
	this->points_get_num_.setTo(0);

	while (nowGroupIdx < maxGroupNum)
	{
		// ��¼��ǰ�ɼ�������
		nowGroupIdx++;
		printf("Now group: %d\n", nowGroupIdx);

		// �ɼ�����
		int frameIdx = 0;
		for (frameIdx = 0; frameIdx < this->max_frame_num_; frameIdx++)
		{
			// �ȴ�Signal
			if (status)
			{
				int info = this->GetInputSignal(frameIdx);
				if (info == 1)
				{
					continue;
				}
				else if (info == 2)
				{
					break;
				}
			}
			

			// �ɼ������룬����
			if (status)
			{
				status = this->CollectSingleFrame(frameIdx);
			}
			if (status)
			{
				status = this->DecodeSingleFrame(frameIdx);
			}
			if (!status)
			{
				frameIdx--;
				status = true;
			}
			else
			{
				if (this->flag_ground_truth_)
				{
					status = this->StorageData(nowGroupIdx, frameIdx);
				}
			}
		}

		// ���ڷ���ֵ���ݣ���Ҫ����ͳһ�ع˲��ж��Ƿ���Ҫ���²ɼ�
		/*if (!this->flag_ground_truth_) {
			status = this->VisualizationForDynamicScene(frameIdx);
			if (!status) {
				nowGroupIdx--;
				status = true;
			}
			else {
				for (int i = 0; i < frameIdx; i++)
					if (status)
						status = this->StorageData(nowGroupIdx, i);
			}
		}*/
	}

	return status;
}


int CDataCollection::GetInputSignal(int frameNum)
{
	// return:
	//     0: Continue
	//     1: End collection of this group
	//     2: End collection of program
	//     3: Error

	int info = 0;

	// �ȴ��ɼ�����
	if ((frameNum == 0) || (this->flag_ground_truth_))
	{
		CVisualization myCamera("DebugCamera");
		bool status = true;
		if (status)
		{
			status = this->sensor_manager_->LoadPatterns(1,
				this->pattern_path_,
				this->wait_name_,
				this->wait_suffix_);
		}
		if (status)
		{
			status = this->sensor_manager_->SetProPicture(0);
		}
		if (status)
		{
			Mat CamMat;
			printf("\tf[%d], Input(<y>, <e>:", frameNum);
			while (true)
			{
				CamMat = this->sensor_manager_->GetCamPicture();
				Mat LittleMat = CamMat(Range(502, 523), Range(630, 651));
				int key1 = this->my_debug_->Show(LittleMat, 100, false, 20);
				int key2 = myCamera.Show(CamMat, 100, false, 0.5);
				if ((key1 == 'y') || (key2 == 'y'))
				{
					printf("y\n");
					info = 0;
					break;
				}
				else if ((key1 == 'e') || (key2 == 'e'))
				{
					printf("e\n");
					info = 1;
					break;
				}
			}
		}
		if (status)
		{
			this->sensor_manager_->UnloadPatterns();
		}
	}

	return info;
}


bool CDataCollection::CollectSingleFrame(int frameNum)
{
	bool status = true;

	// ���dyna_mats_
	/*if (status)
	{
		status = this->sensor_manager_->LoadPatterns(1,
			this->pattern_path_,
			this->dyna_name_,
			this->dyna_suffix_);
	}
	if (status)
	{
		status = this->sensor_manager_->SetProPicture(0);
	}
	if (status)
	{
		Mat CamMat = this->sensor_manager_->GetCamPicture();
		CamMat.copyTo(this->dyna_mats_[frameNum]);
	}
	if (status)
	{
		status = this->sensor_manager_->UnloadPatterns();
	}
	if ((frameNum > 0) && (!this->flag_ground_truth_))
	{
		return status;
	}*/

	// ���vgray_mats_
	if (status)
	{
		status = this->sensor_manager_->LoadPatterns(GRAY_V_NUMDIGIT * 2,
			this->pattern_path_,
			this->vgray_name_,
			this->gray_suffix_);
	}
	if (status)
	{
		for (int i = 0; i < GRAY_V_NUMDIGIT * 2; i++)
		{
			if (status)
			{
				status = this->sensor_manager_->SetProPicture(i);
			}
			if (status)
			{
				Mat CamMat = this->sensor_manager_->GetCamPicture();
				CamMat.copyTo(this->vgray_mats_[i]);
			}
		}
	}
	if (status)
	{
		status = this->sensor_manager_->UnloadPatterns();
	}

	// ���vphase_mats_
	if (status)
	{
		status = this->sensor_manager_->LoadPatterns(PHASE_NUMDIGIT,
			this->pattern_path_,
			this->vphase_name_,
			this->phase_suffix_);
	}
	if (status)
	{
		for (int i = 0; i < PHASE_NUMDIGIT; i++)
		{
			if (status)
			{
				status = this->sensor_manager_->SetProPicture(i);
			}
			if (status)
			{
				Mat CamMat = this->sensor_manager_->GetCamPicture();
				CamMat.copyTo(this->vphase_mats_[i]);
			}
		}
	}
	if (status)
	{
		status = this->sensor_manager_->UnloadPatterns();
	}

	// ���hgray_mats_
	if (status)
	{
		status = this->sensor_manager_->LoadPatterns(GRAY_H_NUMDIGIT * 2,
			this->pattern_path_,
			this->hgray_name_,
			this->gray_suffix_);
	}
	if (status)
	{
		for (int i = 0; i < GRAY_H_NUMDIGIT * 2; i++)
		{
			if (status)
			{
				status = this->sensor_manager_->SetProPicture(i);
			}
			if (status)
			{
				Mat CamMat = this->sensor_manager_->GetCamPicture();
				CamMat.copyTo(this->hgray_mats_[i]);
			}
		}
	}
	if (status)
	{
		status = this->sensor_manager_->UnloadPatterns();
	}

	// ���hphase_mats_
	if (status)
	{
		status = this->sensor_manager_->LoadPatterns(PHASE_NUMDIGIT,
			this->pattern_path_,
			this->hphase_name_,
			this->phase_suffix_);
	}
	if (status)
	{
		for (int i = 0; i < PHASE_NUMDIGIT; i++)
		{
			if (status)
			{
				status = this->sensor_manager_->SetProPicture(i);
			}
			if (status)
			{
				Mat CamMat = this->sensor_manager_->GetCamPicture();
				CamMat.copyTo(this->hphase_mats_[i]);
			}
		}
	}
	if (status)
	{
		status = this->sensor_manager_->UnloadPatterns();
	}

	return status;
}


bool CDataCollection::DecodeSingleFrame(int frameNum)
{
	bool status = true;

	if ((frameNum > 0) && (!this->flag_ground_truth_))
	{
		return status;
	}

	// vgray����
	CDecodeGray vgray_decoder;
	Mat tmp_gray_mat;
	if (status)
	{
		status = vgray_decoder.SetNumDigit(GRAY_V_NUMDIGIT, true);
	}
	if (status)
	{
		status = vgray_decoder.SetMatFileName(this->pattern_path_,
			this->vgray_code_name_ + this->gray_code_suffix_);
	}
	if (status)
	{
		for (int i = 0; i < GRAY_V_NUMDIGIT * 2; i++)
		{
			if (status)
			{
				status = vgray_decoder.SetMat(i, this->vgray_mats_[i]);
			}
		}
	}
	if (status)
	{
		status = vgray_decoder.Decode();
	}
	if (status)
	{
		tmp_gray_mat = vgray_decoder.GetResult();
	}

	// vphase����
	CDecodePhase vphase_decoder;
	Mat tmp_phase_mat;
	if (status)
	{
		int v_pixPeriod = PROJECTOR_RESLINE / (1 << GRAY_V_NUMDIGIT - 1);
		status = vphase_decoder.SetNumMat(PHASE_NUMDIGIT, v_pixPeriod);
	}
	if (status)
	{
		for (int i = 0; i < PHASE_NUMDIGIT; i++)
		{
			if (status)
			{
				status = vphase_decoder.SetMat(i, this->vphase_mats_[i]);
			}
		}
	}
	if (status)
	{
		status = vphase_decoder.Decode();
	}
	if (status)
	{
		tmp_phase_mat = vphase_decoder.GetResult();
	}

	// �ϲ������ipro
	if (status)
	{
		int vGrayNum = 1 << GRAY_V_NUMDIGIT;
		int v_pixPeriod = PROJECTOR_RESLINE / (1 << GRAY_V_NUMDIGIT - 1);
		int vGrayPeriod = PROJECTOR_RESLINE / vGrayNum;
		//this->my_debug_->Show(tmp_gray_mat, 0, true, 0.5);
		//this->my_debug_->Show(tmp_phase_mat, 0, true, 0.5);
		for (int h = 0; h < CAMERA_RESROW; h++)
		{
			for (int w = 0; w < CAMERA_RESLINE; w++)
			{
				double grayVal = tmp_gray_mat.at<double>(h, w);
				double phaseVal = tmp_phase_mat.at<double>(h, w);
				if (grayVal < 0)
				{
					tmp_phase_mat.at<double>(h, w) = 0;
					continue;
				}
				if ((int)(grayVal / vGrayPeriod) % 2 == 0)
				{
					if (phaseVal >(double)v_pixPeriod * 0.75)
					{
						tmp_phase_mat.at<double>(h, w) = phaseVal - v_pixPeriod;
					}
				}
				else
				{
					if (phaseVal < (double)v_pixPeriod * 0.25)
					{
						tmp_phase_mat.at<double>(h, w) = phaseVal + v_pixPeriod;
					}
					tmp_phase_mat.at<double>(h, w) = tmp_phase_mat.at<double>(h, w) - 0.5 * v_pixPeriod;
				}
			}
		}
		/*this->my_debug_->Show(tmp_gray_mat, 0, true, 0.5);
		this->my_debug_->Show(tmp_phase_mat, 0, true, 0.5);*/
		this->ipro_mats_[frameNum] = tmp_gray_mat + tmp_phase_mat;
	}

	// hgray����
	CDecodeGray hgray_decoder;
	if (status)
	{
		status = hgray_decoder.SetNumDigit(GRAY_H_NUMDIGIT, false);
	}
	if (status)
	{
		status = hgray_decoder.SetMatFileName(this->pattern_path_,
			this->hgray_code_name_ + this->gray_code_suffix_);
	}
	if (status)
	{
		for (int i = 0; i < GRAY_H_NUMDIGIT * 2; i++)
		{
			if (status)
			{
				status = hgray_decoder.SetMat(i, this->hgray_mats_[i]);
			}
		}
	}
	if (status)
	{
		status = hgray_decoder.Decode();
	}
	if (status)
	{
		tmp_gray_mat = hgray_decoder.GetResult();
	}

	// hphase����
	CDecodePhase hphase_decoder;
	if (status)
	{
		int h_pixPeriod = PROJECTOR_RESROW / (1 << GRAY_H_NUMDIGIT - 1);
		status = hphase_decoder.SetNumMat(PHASE_NUMDIGIT, h_pixPeriod);
	}
	if (status)
	{
		for (int i = 0; i < PHASE_NUMDIGIT; i++)
		{
			if (status)
			{
				status = hphase_decoder.SetMat(i, this->hphase_mats_[i]);
			}
		}
	}
	if (status)
	{
		status = hphase_decoder.Decode();
	}
	if (status)
	{
		tmp_phase_mat = hphase_decoder.GetResult();
	}

	// �ϲ������jpro
	if (status)
	{
		int hGrayNum = 1 << GRAY_H_NUMDIGIT;
		int h_pixPeriod = PROJECTOR_RESROW / (1 << GRAY_H_NUMDIGIT - 1);
		int hGrayPeriod = PROJECTOR_RESROW / hGrayNum;
		/*this->my_debug_->Show(tmp_gray_mat, 0, true, 0.5);
		this->my_debug_->Show(tmp_phase_mat, 0, true, 0.5);*/

		// Save
		/*FileStorage fs("1.xml", FileStorage::WRITE);
		fs << "gray_mat" << tmp_gray_mat;
		fs << "phase_mat" << tmp_phase_mat;
		fs.release();*/

		for (int h = 0; h < CAMERA_RESROW; h++)
		{
			for (int w = 0; w < CAMERA_RESLINE; w++)
			{
				double grayVal = tmp_gray_mat.at<double>(h, w);
				double phaseVal = tmp_phase_mat.at<double>(h, w);
				if (grayVal < 0)
				{
					tmp_phase_mat.at<double>(h, w) = 0;
					continue;
				}
				if ((int)(grayVal / hGrayPeriod) % 2 == 0)
				{
					if (phaseVal >(double)h_pixPeriod * 0.75)
					{
						tmp_phase_mat.at<double>(h, w) = phaseVal - h_pixPeriod;
					}
				}
				else
				{
					if (phaseVal < (double)h_pixPeriod * 0.25)
					{
						tmp_phase_mat.at<double>(h, w) = phaseVal + h_pixPeriod;
					}
					tmp_phase_mat.at<double>(h, w) = tmp_phase_mat.at<double>(h, w) - 0.5 * h_pixPeriod;
				}
			}
		}
		/*this->my_debug_->Show(tmp_gray_mat, 0, true, 0.5);
		this->my_debug_->Show(tmp_phase_mat, 0, true, 0.5);*/
		this->jpro_mats_[frameNum] = tmp_gray_mat + tmp_phase_mat;
		//this->my_debug_->Show(this->jpro_mats_[frameNum], 0, true, 0.5);
	}

	// ȷ���Ƿ���ȷ�ɼ�
	if (status)
	{
		int key;
		while (true)
		{
			key = this->my_debug_->Show(this->ipro_mats_[frameNum], 500, true, 0.5);
			if (key == 'y')
			{
				status = true;
				break;
			}
			else if (key == 'n')
			{
				status = false;
				break;
			}
			key = this->my_debug_->Show(this->jpro_mats_[frameNum], 500, true, 0.5);
			if (key == 'y')
			{
				status = true;
				break;
			}
			else if (key == 'n')
			{
				status = false;
				break;
			}
			key = this->my_debug_->Show(this->dyna_mats_[frameNum], 500, false, 0.5);
			if (key == 'y')
			{
				status = true;
				break;
			}
			else if (key == 'n')
			{
				status = false;
				break;
			}
		}
	}

	return status;
}


bool CDataCollection::VisualizationForDynamicScene(int total_frame_num)
{
	int key;
	bool status = true;
	while (true) {
		key = this->my_debug_->Show(this->ipro_mats_[0], 500, true, 0.5);
		if (key == 'y')	{
			status = true;
			break;
		}
		else if (key == 'n') {
			status = false;
			break;
		}
		key = this->my_debug_->Show(this->jpro_mats_[0], 500, true, 0.5);
		if (key == 'y') {
			status = true;
			break;
		}
		else if (key == 'n') {
			status = false;
			break;
		}
		for (int frame_idx = 0; frame_idx < total_frame_num; frame_idx++) {
			key = this->my_debug_->Show(this->dyna_mats_[frame_idx], 100, false, 0.5);
			if (key == 'y') {
				status = true;
				break;
			}
			else if (key == 'n') {
				status = false;
				break;
			}
		}
	}

	return status;
}


bool CDataCollection::Close()
{
	bool status = true;

	if (status)
	{
		status = this->sensor_manager_->CloseSensor();
	}

	return status;
}


bool CDataCollection::StorageData(int groupNum, int frameNum)
{
	if (!this->storage_flag_)
		return true;

	CStorage store;

	// �趨�ļ�����
	stringstream ss;
	ss << groupNum;
	string folderName;
	ss >> folderName;
	ss.clear();
	ss << frameNum;
	string frameName;
	ss >> frameName;

	// ���涯̬֡��Ϣ
	/*store.SetMatFileName(this->save_data_path_
		+ folderName
		+ "/"
		+ this->dyna_frame_path_,
		this->dyna_frame_name_ + frameName,
		this->dyna_frame_suffix_);
	store.Store(&this->dyna_mats_[frameNum], 1);*/
	
	// �����Ӧ����ֵ��Ϣ
	/*store.SetMatFileName(this->save_data_path_
		+ folderName
		+ "/"
		+ this->ipro_frame_path_,
		this->ipro_frame_name_ + frameName,
		this->ipro_frame_suffix_);
	store.Store(&this->ipro_mats_[frameNum], 1);*/
	FileStorage fs_i(this->save_data_path_
		+ folderName
		+ "/"
		+ this->pro_frame_path_
		+ this->ipro_frame_name_
		+ frameName
		+ ".xml", FileStorage::WRITE);
	fs_i << "ipro_mat" << this->ipro_mats_[frameNum];
	fs_i.release();
	FileStorage fs_j(this->save_data_path_
		+ folderName
		+ "/"
		+ this->pro_frame_path_
		+ this->jpro_frame_name_
		+ frameName
		+ ".xml", FileStorage::WRITE);
	fs_j << "jpro_mat" << this->jpro_mats_[frameNum];
	fs_j.release();

	return true;
}


