#pragma once

#include <Windows.h>


class Window
{
public:
	Window();
	~Window();

	bool init();
	bool release();
	bool broadcast();
	bool isRun();

	//	Events
	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy() = 0;
protected:
	HWND m_hwnd;
	bool m_is_run;
};