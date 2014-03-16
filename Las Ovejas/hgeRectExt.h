#pragma once
#include "hgerect.h"
#include "global.h"
extern HGE * hge;

class hgeRectExt :
	public hgeRect
{
private:
	float m_Lifespan;
public:
	hgeRectExt(float _x1, float _y1, float _x2, float _y2);
	hgeRectExt(hgeRect & rect);
	hgeRectExt();
	virtual ~hgeRectExt(void);
	void Render();
	bool Update(const float & dt);
	void SetLifespan(const float & t) {m_Lifespan = t;}
};
