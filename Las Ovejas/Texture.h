#pragma once

/*
	Texture handling class
	William Goddard 2009
*/

#include "hge.h"
#include <string>

extern HGE * hge;

class Texture
{
private:
	struct { int w; int h; int r; int c; } m_Dimensions;
	HTEXTURE m_Texture;
	unsigned int m_RefCount;
	std::string m_FileName;
	bool m_IsLoaded;
public:
	Texture(char * filename, int tilewidth = 0, int tileheight = 0, int n_rows = 1, int n_columns = 1);
	~Texture();
	bool AddRef() { if (m_IsLoaded) { m_RefCount++; return true; } else return false; }
	bool DelRef();
	int GetRef() const {return m_RefCount;}
	HTEXTURE GetTex() const { return m_Texture; }
	int GetWidth() {return m_Dimensions.w;}
	int GetHeight() {return m_Dimensions.h;}
};
