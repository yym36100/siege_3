#include "StdAfx.h"

#include "cdef.h"
#include "window4.h"

#ifndef GWL_USERDATA
//x64 issue
#define GWL_USERDATA        (-21)
#endif

u32 mypalette[256];
namespace siege
{
	u8 CWindow::cnt = 0;
	LRESULT CALLBACK CWindow::WndProcedure(HWND hWnd, UINT Msg,
		WPARAM wParam, LPARAM lParam)
	{
		CWindow *self = (CWindow*) GetWindowLongPtr(hWnd, GWL_USERDATA);
		LRESULT res = 0;
		if(self)
		{
		res = self->WndProc(Msg,wParam,lParam);
		}
		if (res ==0)
		{
			switch(Msg)
			{
			case WM_NCCREATE:
				SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR) ((CREATESTRUCT*)lParam)->lpCreateParams);
				break;
			case WM_PAINT:
				{
					PAINTSTRUCT ps;
					HDC hdc;
					hdc = BeginPaint(hWnd, &ps);	
					self->Paint(hdc);
					EndPaint(hWnd, &ps);
				}
				break;		
			case WM_SIZE:
				{
					u16 w = LOWORD(lParam),h=HIWORD(lParam);
					if(self)
					{
						self->Resize(w,h);
					}
				}
				break;
			}
		}
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
};