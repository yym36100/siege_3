#pragma once

#include "cdef.h"
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "obj.h"
#include "surface.h"
#include "winsurface.h"
//#include ".\widgets\widget.h"
#include ".\widgets\TextWidget.h"
#include ".\widgets\ButtonWidget.h"
#include ".\widgets\BitmapWidget.h"
#include ".\widgets\GraphWidget.h"
#include ".\resources\gimphead.h"

#include "Events.h"

#include "..\resource.h"

extern u32 mypalette[256];
namespace siege{



	class CWindow
	{
	public:
		CWinSurface *surf;
		//widgets::CTextWidget *widget;
		//widgets::CButtonWidget *bw;

		widgets::CWidget *widgetlist;
		Events *e;

		

		u32 style;
		HWND hWnd,parent_hWnd;
		static u8 cnt;

		BITMAPINFO BitInfo;
		HDC hDC;
		HBITMAP hBitmap;

		u8 mode ;
		u8 pause;
		widgets::CGraphWidget *gw1;
		widgets::CSignal::tsig sigt;

		CWindow(void)
		{
		}

		CWindow(stRect r, u32 style,HWND parent_hWnd,u16 *name)
			:mode(1),pause(0),sigt(widgets::CSignal::ensin)
		{
			surf = new CWinSurface(r.w,r.h);
			ATOM res;
			this->parent_hWnd = parent_hWnd;
			widgetlist = NULL;




			HINSTANCE hInstance=GetModuleHandle(NULL);
			if(cnt==0)
			{
				WNDCLASSEX	WndClsEx;
				WndClsEx.cbSize        = sizeof(WNDCLASSEX);
				WndClsEx.style         = 0;
				WndClsEx.lpfnWndProc   = WndProcedure;
				WndClsEx.cbClsExtra    = 0;
				WndClsEx.cbWndExtra    = 0;
				WndClsEx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
				WndClsEx.hCursor       = LoadCursor(NULL, IDC_ARROW);
				WndClsEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);				
				WndClsEx.lpszMenuName  = NULL;
				WndClsEx.lpszClassName = _T("siege");
				WndClsEx.hInstance     = hInstance;
				WndClsEx.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
				res = RegisterClassEx(&WndClsEx);			
			}	
			RECT rr={0,0,r.w,r.h};
			AdjustWindowRect(&rr,style,FALSE);
			hWnd = CreateWindow(_T("siege"),(LPCWSTR)name,	style,r.x,r.y,rr.right-rr.left,rr.bottom-rr.top,parent_hWnd,NULL,hInstance,this);

			{
				FILE *f;
				u8 buff[1024];
				f=fopen("gold.raw","rb");
			
				//mypalette =0;
				if(f)
				{		
						fread(buff,768,1,f);
				fclose(f);
					for(int i=0;i<256;i++)
					{
						mypalette[i]=0+(buff[i*3+2]<<0)+(buff[i*3+1]<<8)+(buff[i*3+0]<<16);
					}
				}
				else
				{
					for(int i=0;i<256;i++)
					{
						mypalette[i]=0xff+(i<<8)+(i<<16)+(i<<24);
					}
				}
			}

			
			/*--------------------*/
			widgets::CTextWidget *tw,*tw2;
			widgets::CButtonWidget *bw1,*bw2,*bw3,*bw4,*bw5,*bw6;
			widgets::CBitmapWidget *bw;
			widgets::CGraphWidget *gw3;

			tw = new widgets::CTextWidget(surf,20,400,100,100);
			tw->SetText((u8*)"0");
			tw->setsize(2);

			tw2 = new widgets::CTextWidget(surf,10,210,100,100);
			tw2->SetText((u8*)"Voltage threshold: 10.43V");
			//tw->Draw();

			//delete tw;
			bw1 = new widgets::CButtonWidget(surf,20,10,160,20);
			bw1->SetText((u8*)"rect");
		
			bw1->handle = (u32)hWnd;
			bw1->command = 0x1;

			bw2 = new widgets::CButtonWidget(surf,20,50,160,20);
			bw2->SetText((u8*)"sin");
			bw2->handle = (u32)hWnd;
			bw2->command = 0x2;

			bw3 = new widgets::CButtonWidget(surf,20,90,160,20);
			bw3->SetText((u8*)"clip+");
			bw3->handle = (u32)hWnd;
			bw3->command = 0x3;

			bw4 = new widgets::CButtonWidget(surf,20,130,160,20);
			bw4->SetText((u8*)"clip-");
			bw4->handle = (u32)hWnd;
			bw4->command = 0x4;

			bw5 = new widgets::CButtonWidget(surf,20,170,160,20);
			bw5->SetText((u8*)"Mittudomain");
			bw5->handle = (u32)hWnd;
			bw5->command = 0x5;

			bw6 = new widgets::CButtonWidget(surf,20,170+40,160,20);
			bw6->SetText((u8*)"Play/Pause");
			bw6->handle = (u32)hWnd;
			bw6->command = 0x6;

			bw = new widgets::CBitmapWidget(surf,100,100,40,40);

			gw1 = new widgets::CGraphWidget(surf,200,10,1400,200);
			
			//bw->Draw();
			//delete bw;

			widgetlist = tw;
			widgetlist->Add(bw1);
			widgetlist->Add(bw2);
			widgetlist->Add(bw3);
			widgetlist->Add(bw4);
			widgetlist->Add(bw5);
			widgetlist->Add(bw6);
			//widgetlist->Add(tw2);
			widgetlist->Add(gw1);

				//CSignal(float a = 80, float f = 2, float ph =3.14/3.0,float offs = 0,u16 l = 50, tsig sig = ensin):
					//gw1->sig = new widgets::CSignal(80,2,len++/100.0,0,512);

			widgetlist->Add( new widgets::CGraphWidget(surf,200,10+200+20,1400,200) );
			gw1->sig = new widgets::CSignal(80,2,0,100,1024*1);

			gw3 = new widgets::CGraphWidget2(surf,200,10+200*2+20*2,1400,400);
			widgetlist->Add(gw3);
			gw3->palette = mypalette;
			

			bw = new widgets::CBitmapWidget(surf,0,500,10,10);
			widgetlist->Add( bw);
			gimphead const test1;
			bw->SetPicture((gimphead*)&test1);

			gw1->clip = 64;
			//((widgets::CGraphWidget*)(gw1->next))->style = widgets::CGraphWidget::enbar;


			
			fft();



			//widgetlist->DrawAll();
			
			/*-----------------------*/


			ShowWindow(hWnd, SW_SHOWNORMAL);
			UpdateWindow(hWnd);
			cnt++;
			SetTimer(hWnd,1982,0,0);

		}

		void fft(void)
			{
//				int fix_fftr(short f[], int m, int inverse);
				int fix_fft(short fr[], short fi[], short m, short inverse);

			
				short fr[1024*1];
				short fi[1024*1];
				u16 scale;
				for(u16 i=0;i<1024*1;i++)
				{
					//should do some windowing
					fr[i] = (short)gw1->sig->pdata[i];
					fi[i] = 0;
				/*	if (i & 0x01)
						f[(1024*2+i)>>1] = (short)gw1->sig->pdata[i];
					else
						f[i>>1] = (short)gw1->sig->pdata[i];*/
				}
				//scale = fix_fftr(f,log2N,0);
				scale = fix_fft(fr,fi,10,0);
				float *ff = new float[1024*2];
				for(u16 i=0;i<1024/1;i++)
				{
					fr[i]<<=scale;
					fi[i]<<=scale;
					ff[i]= -(fr[i]*fr[i]+fi[i]*fi[i]);				
				}
				((widgets::CGraphWidget*)(gw1->next))->sig = new widgets::CSignal(0,0,0,0,1024/2,siege::widgets::CSignal::enext,ff);
				((widgets::CGraphWidget*)(gw1->next->next))->sig = new widgets::CSignal(0,0,0,0,1024/2,siege::widgets::CSignal::enext,ff);
				//delete[] ff;

			}

		virtual ~CWindow(void)
		{
			delete widgetlist;
			widgetlist = NULL;
			cnt--;
		}

		static LRESULT CALLBACK WndProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		virtual LRESULT CALLBACK WndProc(UINT Msg,WPARAM wParam, LPARAM lParam)
		{
			LRESULT res = 0;
			switch(Msg)
			{
			case 0x1122:
				{
					//user messages
					switch(wParam)
					{
					case 0x01:
						//data = 0;			
						sigt = widgets::CSignal::enrect;
						break;
					case 0x2:
						sigt = widgets::CSignal::ensin;
						//data++;						
						break;
					case 0x5:
						sigt = widgets::CSignal::ennoise;
						//data++;						
						break;
					case 0x3:
						gw1->clip=0;
						//data--;						
						break;
						case 0x4:
						gw1->clip=256;
						//data--;						
						break;

						case 0x6:
						pause^=1;
						//data--;						
						break;
					}					
				}
				break;
				/* mouse events*/
			case WM_MOUSEMOVE:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
				{
					Events e;
					e.e=wParam==MK_LBUTTON?Events::mpress:Msg==WM_LBUTTONUP?Events::mrelease:Events::mmove;
					e.x = GET_X_LPARAM(lParam);
					e.y = GET_Y_LPARAM(lParam);

					widgets::CWidget *w = widgetlist;
					u16 res=0;
					while(w)
					{
						if(res|=w->OnEvent(&e));//break;
						w=(widgets::CWidget*)w->next;
					}	
					if(res!=0)
					{
						InvalidateRect(hWnd,0,0);
					}

				}
				break;

			case WM_DESTROY:						
				if(parent_hWnd==NULL)
				{
					KillTimer(hWnd,1982);
					PostQuitMessage(0);				
				}
				delete this;
				break;
			case WM_TIMER:
				if(pause==0)
				{
					delete gw1->sig;
					delete ((widgets::CGraphWidget*)(gw1->next))->sig;
				{
					static float freq = 2;
					char buffer[100];
					//CSignal(float a = 80, float f = 2, float ph =3.14/3.0,float offs = 0,u16 l = 50, tsig sig = ensin):
					gw1->sig = new widgets::CSignal(1000,freq,0.0,0,1024*1,sigt);
					freq+=1.1;
					if(freq>512)freq = 0.1;
					fft();
					sprintf(buffer,"f=%03.2f",freq);
					((widgets::CTextWidget*)widgetlist)->SetText((u8*)buffer);
					InvalidateRect(hWnd,0,0);
				}
				}
				break;
		/*	case WM_CHAR:
				InvalidateRect(hWnd,0,0);
				switch (wParam)
				{
				case '1':
					mode = 1;
					break;
				case '2':
					mode = 2;
					break;
				case '3':
					mode = 3;
					break;
				case 'p':
					pause^=1;
					break;
				}				
				break;*/
			}
			return res;
		}

		virtual WPARAM Run(void)
		{
			HACCEL hAccelTable;
			MSG msg;
			HINSTANCE hInstance=GetModuleHandle(NULL);
			hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SIEGE8));

			while (GetMessage(&msg, NULL, 0, 0))
			{
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			return msg.wParam;
		}
		void Resize(u16 w, u16 h)
		{
			delete surf;
			surf = new CWinSurface(w,h);
			InvalidateRect(hWnd,0,0);
		}

		void Paint(HDC dc)
		{

			widgetlist->DrawAll();
			//surf->DrawLine(10,10,100,100,0x11223344);
			surf->Paint(dc);		
		}

	}; // end class

};







