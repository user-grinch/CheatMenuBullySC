#pragma once
#include "pdh.h"

class OverlayWindow
{
public:
	enum DISPLAY_POS
	{
		CUSTOM,
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT
	};
	static inline DISPLAY_POS mSelectedPos = DISPLAY_POS::BOTTOM_RIGHT;
	static inline bool bCoord = false;
	static inline bool bFPS = false;
	static inline int mFPS = 0;
	static inline bool bTransparent = false;
	static inline bool bCpuUsage = false;
	static inline float fCpuUsage = 0.0f;
	static inline bool bMemUsage = false;
	static inline float fMemUsage = 0.0f;
	static inline std::vector<std::string> posNames = {
		"Custom", "Top left", "Top right", "Bottom left", "Bottom right"
	};
	static inline float fPosX = 0.0f;
	static inline float fPosY = 0.0f;
	static inline int mTotalRam = 0;

	static inline PDH_HQUERY cpuQuery;
	static inline PDH_HCOUNTER cpuTotal;

private:
	static void GetCPUUsageInit();
	static double GetCurrentCPUUsage();

public:
	OverlayWindow() = delete;
	OverlayWindow(OverlayWindow&) = delete;
	void operator=(OverlayWindow const&) = delete;

	static void Init();
	static void Draw();

};

