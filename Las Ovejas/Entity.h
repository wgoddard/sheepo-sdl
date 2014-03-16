#pragma once

/*
	Abstract (non usable, derivable only) entity class
	William Goddard 2009
*/

#include <iostream>

#include "hge.h"
#include "hgesprite.h"

#include "global.h"
#include "Texture.h"

extern HGE * hge;

class Entity
{
private:
	Texture & m_Texture;
protected:
	hgeSprite * m_Sprite;   //Constructor sets hotspot by default to center point
	float m_X, m_Y; //Changed from double to flaot on 12 of sept.
	float m_W, m_H;
	float m_Rotation;
	float m_ScaleX;
	float m_ScaleY;
	bool m_FlipX;
	bool m_FlipY;
public:
	Entity(Texture & tex, int x, int y, int w, int h);
	Entity(Entity & e);
	virtual Entity & operator=(const Entity & e);
	virtual ~Entity(void);
	void SetPosition(float x, float y) { m_X = x, m_Y = y; }
	virtual void Update (const float &dt) = 0;
	virtual void Render() = 0;
	virtual void RenderBar(	const float & current, const float & maximum, float xoffset = 0, float yoffset = -20, const float sizex = 50,
							const DWORD barcolor = 0xFFFFFFFF, const DWORD linecolor = 0xFF23A0E0);
	virtual void RenderBoundingBox();
	virtual hgeRect GetRect() { hgeRect temp;  m_Sprite->GetBoundingBoxEx(m_X, m_Y, m_Rotation, m_ScaleX, m_ScaleY, &temp); return temp; }
	hgeSprite & GetSprite() { return *m_Sprite; }

};
