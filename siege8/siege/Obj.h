#pragma once
#include "cdef.h"


namespace siege{

	struct stPoint
	{
		u16 x,y;
	};

	struct stRect
	{
		u16 x,y,w,h;
		stRect(u16 xx, u16 yy, u16 ww, u16 hh):
		x(xx),y(yy),w(ww),h(hh)		{}
	};

	class CObj
	{
	public:

		CObj *next;

		CObj(void):next(0)
		{
		}

		virtual ~CObj(void)
		{
			if(next) delete next;
		}

		void Add(CObj *o)
		{
			CObj *self = this;
			while(self->next)self=self->next;
			self->next = o;
		}

		CObj* operator+(CObj &r)
		{
			Add(&r);
			return this;
		}
	};

};