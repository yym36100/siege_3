#pragma once
#include <stdio.h>
#include <math.h>

#include "Widget.h"
#include ".\resources\simplefont.h"
#include "..\Surface.h"

namespace siege
{
	namespace widgets
	{
		class CSignal
		{
		public:
			float *pdata;
			u16 length;
			//enum tformat{enu16,enfloat};
			enum tsig{ensin,enrect,ennoise, enext};
			tsig sig;
			//tformat format;

			virtual ~CSignal()
			{
				delete[] pdata;
			}
			CSignal(float a = 80, float f = 2, float ph =3.14/3.0,float offs = 0,u16 l = 512, tsig sig = ensin,float *pd = 0):			
			length(l),pdata(0) , sig(sig)
			{	   
				switch (sig)
				{
				case ensin:
					pdata = new float[l];      
					for(u16 i=0;i<length;i++) 
					{

						pdata[i] = offs + a * sin(2*3.141592 * f*i / (float) length + ph);
						pdata[i] += offs + a/3.0 * sin(2*3.141592 * (50)*i / (float) length + ph);
						pdata[i] += offs + a/2.0 * sin(2*3.141592 * (f/3.0)*i / (float) length + ph);
						pdata[i] += offs + a/1.5 * sin(2*3.141592 * (f/2.0)*i / (float) length + ph);

						pdata[i] += offs + a/3.0 * rectf(2*3.141592 * (241)*i / (float) length + ph);
						pdata[i] += offs + a * sin(2*3.141592 * (512-f)*i / (float) length + ph);
						pdata[i] += offs + (a/0.0525) * (rand()-32767.0/2.0)/32768.0;

						//windowing
						//pdata[i] *= sin(3.141592 * i/(float)length);
					}
					break;
				case ennoise:
					pdata = new float[l];      
					for(u16 i=0;i<length;i++) 
					{
						pdata[i] = offs + a * (rand()-32767.0/2.0)/32768.0;

						//windowing
						pdata[i] *= sin(3.141592 * i/(float)length);
					}
					break;
				case enrect-100:
					pdata = new float[l];      
					for(u16 i=0;i<length;i++) 
					{
						pdata[i] = offs + a * sin(2*3.141592 * f*i / (float) length + ph);

						//windowing
						//pdata[i] *= sin(3.141592 * i/(float)length);
					}
					break;
				case enrect:
					pdata = new float[l];      
					for(u16 i=0;i<length;i++) 
					{
						pdata[i] = offs + a * rectf(2*3.141592 * f*i / (float) length + ph);

						//windowing
						//pdata[i] *= sin(3.141592 * i/(float)length);
					}
					break;
				case enext:
					pdata = pd;
					break;
				}
			}
			float rectf(float f)
			{
				if(fmod(f,6.28f)<3.14) return 1;
				return -1;
			}

			float getmin(void)
			{
				float res = 1000000;
				for(u16 i = 0;i<length;i++)
					if(pdata[i]<res)
						res = pdata[i];
				return res;
			}

			float getmax(void)
			{
				float res = -1000000;
				for(u16 i = 0;i<length;i++)
					if(pdata[i]>res)
						res = pdata[i];
				return res;
			}


		};

		class CGraphWidget :
			public CWidget
		{
		public:
			CSurface *surf;
			resources::CSimpleFont *sf;
			u32 gridcolor;
			u32 centercolor;
			Events lastevent;
			CSignal *sig;
			enum tdmode{enstep,eninterp};
			enum tstyle{enplot, enbar};
			tstyle style;
			tdmode dmode;
			bool autoscale;
			bool peakdetect;
			u16 clip;

			float ofs;
			float fact;
			u32 *palette;

			CGraphWidget(CSurface *ss, u16 x, u16 y, u16 w=600, u16 h=500):
			sig(0),surf(ss),CWidget(x,y,w,h), dmode(enstep), autoscale(true),peakdetect(true), style(enplot), clip(0),palette(0)
			{
				gridcolor = 0xffd0d0d0;
				centercolor= 0xffa0a0a0;
				sf = new resources::CSimpleFont(ss,0xffffffff);

			}

			virtual ~CGraphWidget(void)	{}

			void DrawGrid(void)const
			{				

				u16 ystep =40;
				u16 xstep =40;
				u16 h = rect.h;
				u16 w = rect.w;
				u16 my = h/2;
				u16 mx = w/2;

				//surf->DrawLine(rect.x + 0,rect.y + my,rect.x + w-1,rect.y +my,centercolor);
				surf->DrawHLineP(rect.x + 0,rect.x + w-1,rect.y +my,centercolor,0x55555555);
				surf->DrawVLineP(rect.x + mx,rect.y +0,rect.y +h-1,centercolor,0x55555555);

				for(u16 y=ystep;y<h/2;y+=ystep)
				{

					//draw h lines
					surf->DrawHLineP(rect.x + 0,rect.x + w-1,rect.y +my+y,gridcolor);					
					surf->DrawHLineP(rect.x + 0,rect.x + w-1,rect.y +my-y,gridcolor);
					

				}
				for(u16 x=xstep;x<w/2;x+=xstep)
				{
					//draw w lines
					surf->DrawVLineP(rect.x + mx+x,rect.y +0,rect.y +h-1,gridcolor);
					surf->DrawVLineP(rect.x + mx-x,rect.y +0,rect.y +h-1,gridcolor);
				}
			}

			void Draw()
			{
				sf->Color = 0xff808080;
				sf->Color = 0xfffefefe;
				surf->FillRect(rect,0xfff0f0f0);
				this->DrawGrid();
				surf->DrawRect(rect,0xffb0b0b0);
				//sf->DrawTextB(rect.x+(rect.w - 6*5)/2+20,rect.y+(rect.h-6)/2+8,(u8*)"(0,0)");

				DrawSignal();

				DrawCursor();
			}
			void DrawCursor(void) 
			{
				if(Highlighted)				
				{
					char buffer[220];
					float j = sig->length/(float)rect.w;
					if(clip!=0)
					{
						j = clip/(float)rect.w;
					}
					float yy = (lastevent.y-rect.y)*fact - ofs*fact;
					sprintf(buffer,"(%6.2f,%6.2f)", (float)(lastevent.x - rect.x)*j,yy);

					surf->DrawVLineP(lastevent.x,rect.y,rect.h+rect.y,0xffff8080,0xffff);
					surf->DrawHLineP(rect.x,rect.x+rect.w,lastevent.y,0xffff8080,0xffff);

					sf->Color = 0xff808080;
					sf->DrawTextB(lastevent.x+4, lastevent.y+4,(u8*)buffer);
				}			
			}

			void DrawSignal(void) 
			{
				if(!sig) return;
				float j = sig->length/(float)rect.w;
				if(clip!=0)
				{
					j = clip/(float)rect.w;
				}
				u16 x=0;
				float *pdata = (float*)sig->pdata;
				float ind=0;
				surf->lx = rect.x;
				surf->ly = rect.y;

				float val,vp1,vm1;
				float m,M;	
				fact = 1;
				ofs=0;

				u16 pi,li=0;
				float lm = sig->getmin();
				float pt = (sig->getmax()-sig->getmin())*3.0/100.0;
				if(autoscale)
				{

					m = sig->getmin();
					M = sig->getmax();
					fact = (M-m)/(float)(rect.h-8);
					ofs = 4+-m/fact;

					if(fact<0.01)
					{
						fact = 1;
						ofs = 0;
						return;
					}
				}


				for(u16 i=0;i<rect.w;i++)
				{
					if(ind>=sig->length)
						break;
					val = ofs + pdata[(u16)ind]/fact;
					if(ind<(sig->length-1))
						vp1 = ofs + pdata[(u16)ind+1]/fact;
					else
						vp1 = val;

					if(ind>1)
						vm1 = ofs + pdata[(u16)ind-1]/fact;
					else
						vm1 = val;

					if(style == enplot)
					{
						surf->DrawTo(rect.x+i,rect.y+ val,0xff008000);
					}
					else
					{
						surf->DrawVLine(rect.x+i,rect.y+val,rect.y+rect.h,0xff804040);
					}

					ind= ind + j;

					if(peakdetect)
					{
						if( ((val>vm1) && (val>vp1)) ||(val<vm1) && (val<vp1))
						{
							if(val< (lm-pt))
							{
								if(i>li+50)
								{
									//peek found
									li=i;
									lm = val;

									surf->SetPixel(rect.x+i,rect.y+val+1,0xff000000);								
									// peek text
									{
										char buffer[220];										
										float yy = (lastevent.y-rect.y)*fact - ofs*fact;
										sprintf(buffer,"(%6.2f,%6.2f)", (float)i*j,pdata[(u16)i]);
										sf->Color = 0xff808080;
										sf->DrawTextB(rect.x+i, rect.y+val,(u8*)buffer);
									}
								}
							}
						}
						if(val>lm)
						{
							lm = val;
						}
					}
				}

				{
					char buffer[200];
					sprintf(buffer,"m:%2.2f M:%2.2f fact:%2.2f ofs:%2.2f", m,M,fact,ofs);
					sf->Color = 0xff808080;
					sf->DrawTextB(rect.x+2, rect.y+2,(u8*)buffer);
				}
			}

			virtual u16 OnEvent(Events *e)
			{
				lastevent = *e;
				if(HitTest(e))
				{
					if(!Highlighted)
						ShowCursor (FALSE);
					Highlighted = true;
					switch(e->e)
					{
					case Events::mmove:
						break;
					case Events::mpress:
						return 1;
						break;
					}	
					return 1;
				}
				else
				{
					if(Highlighted)
						ShowCursor (TRUE);
					Highlighted = false;
				}
				return 0;
			}

		};

		class CGraphWidget2 :
			public CGraphWidget
		{
		public:
			/*CSurface *surf;
			resources::CSimpleFont *sf;
			u32 gridcolor;
			u32 centercolor;
			Events lastevent;
			CSignal *sig;
			enum tdmode{enstep,eninterp};
			enum tstyle{enplot, enbar};
			tstyle style;
			tdmode dmode;
			bool autoscale;
			bool peakdetect;
			u16 clip;

			float ofs;
			float fact;*/
			u8 **linep;
			//float *linedata;
			u16 nrlines;
			//u32 *palette;

			CGraphWidget2(CSurface *ss, u16 x, u16 y, u16 w=600, u16 h=500):
			CGraphWidget(ss,x,y,w,h),linep(0),nrlines(0)
			{
				//gridcolor = 0xff303030;
				//centercolor= 0xff606060;
				sf = new resources::CSimpleFont(ss,0xffffffff);
				linep = new u8*[rect.h];
				for(u16 i = 0; i < rect.h;i++)
				{
					linep[i] = new u8[rect.w];
					memset(linep[i],0xff,rect.w);
				}
			}

			virtual ~CGraphWidget2(void)	{}

			virtual void Draw()
			{
				sf->Color = 0xfffefefe;
				surf->FillRect(rect,0xfff0f0f0);

				DrawSignal();
				this->DrawGrid();
				surf->DrawRect(rect,0xffb0b0b0);

				DrawCursor();
			}

			void DrawSignal(void) 
			{
				if(!sig) return;
				float j = sig->length/(float)rect.w;
				if(clip!=0)
				{
					j = clip/(float)rect.w;
				}
				u16 x=0;
				float *pdata = (float*)sig->pdata;
				float ind=0;
				surf->lx = rect.x;
				surf->ly = rect.y;

				float val,vp1,vm1;
				float m,M;	
				fact = 1;
				ofs=0;

				u16 pi,li=0;
				float lm = sig->getmin();
				float pt = (sig->getmax()-sig->getmin())*18.0/100.0;
				if(autoscale)
				{

					m = sig->getmin();
					M = sig->getmax();
					fact = (M-m)/(255.0);
					if(fact<0.01)
					{
						fact = 1;
						ofs = 0;
						return;
					}
					ofs = -m/fact;
				}

				// draw sig
				//float val;
				u32 color;
				u8 v2;
				u16 v3;

				//update current line
				for(u16 i=0;i<rect.w;i++)
				{
					ind= ind + j;
					val = ofs + pdata[(u16)ind]/fact;
					v2 = val;
					
					(linep[nrlines])[i] = v2;
					//(linep[nrlines])[i] = 0xff;						
				}

				if(++nrlines>=rect.h)nrlines = 0;

				//draw all
				palette = 0;
				for(u16 i =0;i<rect.h;i++)
				{
					//draw each line
					for(u16 j=0;j<rect.w;j++)
					{
						v2 = (linep[i])[j];


						if(palette)
						{
							color = palette[v2];
						}
						else
						{
							color = rgba32(v2,v2,v2,0xff);
						}
						surf->SetPixelN(rect.x+j,rect.y+i,color);
					}
				}
				surf->DrawHLine(rect.x,rect.x+rect.w,rect.y+nrlines,0xffff0000);

			}
		};


	}; // widgets namespace
}; // siege namespace
