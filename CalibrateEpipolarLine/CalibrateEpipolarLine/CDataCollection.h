#ifndef _CDATACOLLECTION_H_
#define _CDATACOLLECTION_H_

#include <opencv2/opencv.hpp>
#include "CSensor.h"
#include "CDecodeGray.h"
#include "CDecodePhase.h"
#include "StaticParameters.h"
#include "GlobalFunction.h"
#include "CVisualization.h"
#include "CStorage.h"

// 数据采集类。控制整体的数据采集，屏蔽下层的内容
// 外部调用只需向其索要数据即可。
// 调用：Init，Collect，Close
class CDataCollection
{
private:
	CSensor * sensor_manager_;

	bool flag_ground_truth_;

	// 用于存储数据
	Mat * vgray_mats_;
	Mat * hgray_mats_;
	Mat * vphase_mats_;
	Mat * hphase_mats_;
	Mat * dyna_mats_;
	Mat * ipro_mats_;
	Mat * jpro_mats_;

	// 图案路径与名称
	string pattern_path_;
	string vgray_name_;
	string hgray_name_;
	string gray_suffix_;
	string vgray_code_name_;
	string hgray_code_name_;
	string gray_code_suffix_;
	string vphase_name_;
	string hphase_name_;
	string phase_suffix_;
	string dyna_name_;
	string dyna_suffix_;
	string wait_name_;
	string wait_suffix_;

	// 存储路径与名称
	string save_data_path_;
	string dyna_frame_path_;
	string dyna_frame_name_;
	string dyna_frame_suffix_;
	string pro_frame_path_;
	string ipro_frame_name_;
	string jpro_frame_name_;
	string ipro_frame_suffix_;

	// visualization
	CVisualization * my_debug_;

	// 类内参数
	bool visualize_flag_;
	bool storage_flag_;
	int max_frame_num_;
	Mat points_get_num_;
	int total_points_num_;

	bool StorageData(int groupNum, int frameNum);
	int GetInputSignal(int frameNum);
	bool CollectSingleFrame(int frameNum);
	bool DecodeSingleFrame(int frameNum);
	bool VisualizationForDynamicScene(int total_frame_num);

public:
	CDataCollection();
	~CDataCollection();
	bool Init();
	bool CollectData();
	bool Close();
};


#endif