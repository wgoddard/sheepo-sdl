#include "hgeRectExt.h"

hgeRectExt::hgeRectExt(float _x1, float _y1, float _x2, float _y2) : hgeRect(_x1, _y1, _x2, _y2)
{
	m_Lifespan = 0;
}

hgeRectExt::hgeRectExt() : hgeRect()
{
}

hgeRectExt::hgeRectExt(hgeRect & rect) : hgeRect(rect.x1, rect.y1, rect.x2, rect.y2)
{
}

hgeRectExt::~hgeRectExt(void)
{
}

void hgeRectExt::Render()
{

	//x1 = targetRange.x1;
	//x2 = targetRange.x2;
	//y1 = targetRange.y1;
	//y2 = targetRange.y2;

	hge->Gfx_RenderLine(x1, y1, x2, y1, COLOR, 0);
	hge->Gfx_RenderLine(x2, y1, x2, y2, COLOR, 0);
	hge->Gfx_RenderLine(x2, y2, x1, y2, COLOR, 0);
	hge->Gfx_RenderLine(x1, y2, x1, y1, COLOR, 0);

}

bool hgeRectExt::Update(const float & dt)
{
	m_Lifespan -= dt;
	
	if (m_Lifespan > 0)
		return false;
	else
		return true;
}