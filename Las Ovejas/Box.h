#pragma once
#include "Entity.h"

extern HGE * hge;

class Box :
	public Entity
{
public:
	Box(Texture * tex, int x, int y, int w, int h);
	virtual ~Box(void);
	virtual void Update(const float &dt);
	virtual void Render();
};
