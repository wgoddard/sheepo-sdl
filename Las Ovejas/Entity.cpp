#include "Entity.h"

extern Texture * bar;

Entity::Entity(Texture & tex, int x, int y, int w, int h) : m_Texture(tex)
{
	m_X = x;
	m_Y = y;
	m_W = w;
	m_H = h;

	m_Rotation = 0.0f;
	m_ScaleX = 1.0f;
	m_ScaleY = 1.0f;
	m_FlipX = false;
	m_FlipY = false;

	m_Texture.AddRef();
	m_Sprite = new hgeSprite(m_Texture.GetTex(), 0, 0, m_W, m_H);
	m_Sprite->SetHotSpot(m_W/2, m_H/2);
	m_Sprite->SetZ(0.2f);

}

Entity::~Entity(void)
{
	delete m_Sprite;
	m_Texture.DelRef();
}

Entity & Entity::operator=(const Entity & e)
{
	if (&e == this)
		return *this;
	return *this;
}

void Entity::RenderBar(const float & current, const float & maximum, float xoffset, float yoffset, const float sizex, const DWORD barcolor, const DWORD linecolor)
{

	float x, y, w, h;
	m_Sprite->GetTextureRect(&x, &y, &w, &h);

	m_Sprite->SetTextureRect(x, y, 50, 10);

	hgeRect bounds;

	m_Sprite->GetBoundingBoxEx(m_X + (m_W - sizex)/2 * m_ScaleX + xoffset, m_Y + yoffset, m_Rotation, m_ScaleX, m_ScaleY, &bounds);

	m_Sprite->SetTextureRect(x, y, w, h);


	float x1, x2, y1, y2;
	x1 = bounds.x1;
	x2 = bounds.x2;
	y1 = bounds.y1;
	y2 = bounds.y2;

	y2++;
	hge->Gfx_RenderLine(x1, y1, x2, y1, linecolor, 0);
	hge->Gfx_RenderLine(x2, y1, x2, y2, linecolor, 0);
	hge->Gfx_RenderLine(x2, y2, x1, y2, linecolor, 0);
	hge->Gfx_RenderLine(x1, y2, x1, y1, linecolor, 0);
	//y2-=0.5f;
	x1+=0.5f;
	

	x2 = current/maximum * (x2-x1) + x1;
	hgeSprite temp(bar->GetTex(), 0, 0, 1, 10);
	temp.SetZ(0.1f);
	//temp.SetColor(barcolor);
	temp.RenderStretch(x1, y1, x2, y2);
	//temp.Render(x1, y1);

	///hgeQuad Doesnt seem to be good with ZBUFFER or textures, besides instancing a sprite isnt much worse now
	//hgeQuad currentQuad;
	//

	////currentQuad.blend = BLEND_NOZWRITE;
	//currentQuad.blend = BLEND_DEFAULT_Z;
	//currentQuad.tex = bar->GetTex();

	//x2 = current/maximum * (x2-x1) + x1;

	//currentQuad.v[0].x = x1;	currentQuad.v[0].y = y1;
	//currentQuad.v[1].x = x2;	currentQuad.v[1].y = y1;
	//currentQuad.v[2].x = x2;	currentQuad.v[2].y = y2;
	//currentQuad.v[3].x = x1;	currentQuad.v[3].y = y2;

	//for (int i = 0; i < 4; ++i)
	//{
	//	//currentQuad.v[i].col = barcolor;
	//	//currentQuad.v[i].col = 0xFFFFFFFF;
	//	currentQuad.v[i].tx = 1;
	//	currentQuad.v[i].ty = 1;
	//	currentQuad.v[i].z = 0.1f;
	//}


	////std::cout << bar->GetTex();

	//hge->Gfx_RenderQuad(&currentQuad);


}

void Entity::RenderBoundingBox()
{
	hgeRect rect;
	m_Sprite->GetBoundingBoxEx(m_X, m_Y, m_Rotation, m_ScaleX, m_ScaleY, &rect);

	float x1, x2, y1, y2;

	x1 = rect.x1;
	x2 = rect.x2;
	y1 = rect.y1;
	y2 = rect.y2;

	hge->Gfx_RenderLine(x1, y1, x2, y1, COLOR, 0);
	hge->Gfx_RenderLine(x2, y1, x2, y2, COLOR, 0);
	hge->Gfx_RenderLine(x2, y2, x1, y2, COLOR, 0);
	hge->Gfx_RenderLine(x1, y2, x1, y1, COLOR, 0);
}