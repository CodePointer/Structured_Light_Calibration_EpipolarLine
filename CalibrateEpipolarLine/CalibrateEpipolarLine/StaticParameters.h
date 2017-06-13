#ifndef _STATICPARAMETERS_H_
#define _STATICPARAMETERS_H_

#include <string>
using namespace std;

// 投影仪的分辨率
extern const int PROJECTOR_RESLINE;
extern const int PROJECTOR_RESROW;

// 相机的分辨率
extern const int CAMERA_RESLINE;
extern const int CAMERA_RESROW;

// 计算机偏移量
extern const int PC_BIASLINE;
extern const int PC_BIASROW;

// 格雷码和PhaseShifting的数目
extern const int GRAY_V_NUMDIGIT;
extern const int GRAY_H_NUMDIGIT;
extern const int PHASE_NUMDIGIT;

// Visualization 相关参数
extern const int SHOW_PICTURE_TIME;
extern const bool VISUAL_DEBUG;

//// 棋盘格相关参数
//extern const bool CHESS_FRAME_NUMBER;
//extern const int CHESS_LINE;
//extern const int CHESS_ROW;

// 数据采集相关参数
extern const int COLLECTION_MAX_FRAME;
extern const string PROJECTOR_PATTERNPATH;
extern const string PROJECTOR_VGRAYNAME;
extern const string PROJECTOR_VGRAYCODENAME;
extern const string PROJECTOR_VGRAYCODESUFFIX;
extern const string PROJECTOR_VPHASENAME;
extern const string PROJECTOR_DYNANAME;
extern const string PROJECTOR_WAITNAME;
extern const string PROJECTOR_FILESUFFIX;

#endif