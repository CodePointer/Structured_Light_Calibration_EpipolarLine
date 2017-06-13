#include "StaticParameters.h"

// 投影仪的分辨率
const int PROJECTOR_RESLINE = 1280;
const int PROJECTOR_RESROW = 800;

// 相机的分辨率
const int CAMERA_RESLINE = 1280;
const int CAMERA_RESROW = 1024;

// 计算机偏移量
const int PC_BIASLINE = 1680;
const int PC_BIASROW = 0;

// 格雷码和PhaseShifting的数目
const int GRAY_V_NUMDIGIT = 6;
const int GRAY_H_NUMDIGIT = 5;
const int PHASE_NUMDIGIT = 4;

// Visualization 相关参数
const int SHOW_PICTURE_TIME = 500;
const bool VISUAL_DEBUG = false;

//// 棋盘格相关参数
//const bool CHESS_FRAME_NUMBER = 15;
//const int CHESS_LINE = 9;
//const int CHESS_ROW = 6;

// 数据采集相关参数
const string PROJECTOR_PATTERNPATH = "Patterns/";
const string PROJECTOR_VGRAYNAME = "vGray";
const string PROJECTOR_VGRAYCODENAME = "vGrayCode";
const string PROJECTOR_VGRAYCODESUFFIX = ".txt";
const string PROJECTOR_VPHASENAME = "vPhase";
const string PROJECTOR_DYNANAME = "dynaMat";
const string PROJECTOR_WAITNAME = "wait";
const string PROJECTOR_FILESUFFIX = ".bmp";