#pragma once
#include "cdef.h"
#include "obj.h"


#define rgba32(r,g,b,a) ((a<<24)|(r<<16)|(b<<8)|(g))
#define gray rgba32(128,128,128,255)
#define white rgba32(255,255,255,255)

#define red rgba32(255,0,0,255)
#define green rgba32(0,255,0,255)
#define blue rgba32(0,0,255,255)

namespace siege{


	class CSurface :
		public CObj
	{
	public:
		u16 w,h;
		u32 *pData;
		u16 lx,ly;

		CSurface(void):pData(0)
		{
		}

		~CSurface(void)
		{
			if(pData) delete[] pData;
		}

		CSurface(u16 w, u16 h)
		{
			this->w = w;
			this->h = h;
			pData = new u32[w*h*4];
		}

		virtual void Clear(u32 color)
		{
			u32 *p= pData;
			for(u32 i=0;i<w*h;i++)
			{
				*p++=color;
			}
		}
		/*void clear(u32 color)
		{
			for(u16 y=0;y<h;y++)
				for(u16 x=0;x<w;x++)

		}*/

		void SetPixel(u16 x, u16 y,u32 color)
		{
			if(x>=w || y>=h) return;
			u32 *p = pData + x + y*w;
			*p = color;
		}

		void SetPixelN(u16 x, u16 y,u32 color)
		{			
			u32 *p = pData + x + y*w;
			*p = color;
		}

		void DrawVLine(u16 x,u16 y1,u16 y2, u32 color)
		{
			if(x>=w || y1>=h ||y2>=h) return;
			u32 *p = pData + x + y1*w;			
			for(u16 y=y1;y<=y2;y++)
			{
				*p=color;
				p+=w;
			}
		}

		void DrawVLineP(u16 x,u16 y1,u16 y2, u32 color, u32 pattern = 0x30303030)
		{
			if(x>=w || y1>=h ||y2>=h) return;
			u32 *p = pData + x + y1*w;	
			u8 i =0;
			for(u16 y=y1;y<=y2;y++)
			{
				if(pattern & (1<<i) )
				{
					*p=color;
				}
				i++;
				if(i==32) i =0;				
				p+=w;
			}
		}

		void DrawHLine(u16 x1,u16 x2,u16 y, u32 color)
		{
			if(y>=h || x1>=w ||x2>=w) return;
			u32 *p = pData + x1 + y*w;			
			for(u16 x=x1;x<=x2;x++)
			{
				*p=color;
				p++;
			}
		}

		void DrawHLineP(u16 x1,u16 x2,u16 y, u32 color,u32 pattern = 0x30303030)
		{
			if(y>=h || x1>=w ||x2>=w) return;
			u32 *p = pData + x1 + y*w;

			u8 i =0;
			for(u16 x=x1;x<=x2;x++)
			{
				if(pattern & (1<<i) )
				{
					*p=color;
				}
				i++;
				if(i==32) i =0;
				p++;
			}
		}

		void DrawRect(u16 x, u16 y, u16 w, u16 h, u32 color)
		{
			DrawVLine(x,y,y+h,color);
			DrawVLine(x+w,y,y+h,color);

			DrawHLine(x,x+w,y,color);
			DrawHLine(x,x+w,y+h,color);
		}
		void DrawRect(stRect r,u32 color)
		{
			DrawRect(r.x,r.y,r.w,r.h,color);
		}
		void FillRect(stRect r, u32 color)
		{
			for(u16 h=r.y;h<r.y+r.h;h++)
			DrawHLine(r.x,r.x+r.w,h,color);
		}
		void DrawTo(u16 x, u16 y, u32 color)
		{
			DrawLine(lx,ly,x,y,color);
			lx = x; ly = y;
		}


#define MYABS(x) x<0?-x:x;
		void DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u32 color)
		{
			static u32 errcnt = 0;
			if(x1>=w || x2 >=w || y1>=h || y2>=h) 
			{
				errcnt++;
				return;
			}
			u32 * p = pData;			
			i16 dx=(x2-x1);
			i16 dy=(y2-y1);

			i16 sx,sy;

			int sa;
			if(dx<0) sx=-1;else sx=1;
			if(dy<0) {sy=-1;sa = -w;}else {sy=1;sa = w;}

			p+=(y1*w+x1);

			dx=MYABS(dx);dy=MYABS(dy);
			if(dx>dy)
			{
				i16 e=dy-dx>>1;
				for(;x1!=x2;)
				{					
					*p=color;
					if(e>0)
					{
						e-=dx;y1+=sy;
						p+=sa;
					}
					x1+=sx;
					p+=sx;
					e+=dy;
				}
			}
			else
			{
				i16 e=dx-dy>>1;
				for(;y1!=y2;)
				{					
					*p=color;
					if(e>0)
					{
						e-=dy;x1+=sx;p+=sx;
					}
					y1+=sy;p+=sa;
					e+=dx;
				}
			}

		}

	};
};