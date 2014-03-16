#pragma once

/*
	Cocoa Tree Entity
	Grows cocoa for harvest
	William Goddard 2009
*/

#include "Entity.h"
#include "Sheep.h"

#include <vector>

extern HGE * hge;

class CocoaTree :
	public Entity
{
private:
	float m_CurrentCocoa, m_MaximumCocoa, m_GrowthCocoa; //changed to float from double on sept 12
	struct Request { Sheep * target;  float amount; };
	std::vector <Request *> m_Requests;
public:
	CocoaTree(Texture * tex, int x, int y, int w, int h);
	virtual ~CocoaTree(void);
	void Update(const float & dt);
	void Render();
	void BeHarvested(const float & amount_requested, Sheep * target);

};
