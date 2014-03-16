#include "Box.h"

Box::Box(Texture * tex, int x, int y, int w, int h) : Entity(*tex, x, y, w, h)
{
	m_Sprite->SetHotSpot(0,0);
}

Box::~Box(void)
{
}

void Box::Update(const float &dt)
{
}

void Box::Render()
{
	m_Sprite->RenderEx(m_X, m_Y, m_Rotation, m_ScaleX, m_ScaleY);
	if (DEBUG)
		RenderBoundingBox();
}