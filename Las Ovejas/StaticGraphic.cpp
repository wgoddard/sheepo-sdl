#include "StaticGraphic.h"

StaticGraphic::StaticGraphic(Texture * tex, int x, int y, int w, int h, float zbuf, bool usehotspot) : Entity(*tex, x, y, w, h)
{
	if (usehotspot == false)
		m_Sprite->SetHotSpot(0,0);
	//std::cout << "Static grphic has zbuf of: " << zbuf << '\n';
	m_Sprite->SetZ(zbuf);
}

StaticGraphic::~StaticGraphic(void)
{
}

void StaticGraphic::Render()
{
	m_Sprite->Render(m_X, m_Y);
}