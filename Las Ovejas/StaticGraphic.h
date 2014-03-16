#pragma once
#include "Entity.h"

class StaticGraphic :
	public Entity
{
public:
	StaticGraphic(Texture * tex, int x, int y, int w, int h, float zbuf = 1.0f, bool usehotspot = false);
	~StaticGraphic(void);
	virtual void Update(const float & dt) {}
	virtual void Render();
};
