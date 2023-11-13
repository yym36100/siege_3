
#include "stdafx.h"
#include ".\siege\window4.h"

#include "siege8.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	siege::stRect r(40,40,1700,900);
	siege::CWindow *w = new siege::CWindow(r,WS_OVERLAPPEDWINDOW,NULL,(u16*)L"Test");
	
	return w->Run();
}