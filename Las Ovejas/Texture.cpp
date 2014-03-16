#include "Texture.h"

Texture::Texture(char * filename, int tilewidth, int tileheight, int n_rows, int n_columns)
{
	m_Dimensions.w = tilewidth;
	m_Dimensions.h = tileheight;
	m_Dimensions.r = n_rows;
	m_Dimensions.c = n_columns;

	m_RefCount = 0;
	m_FileName = filename;

	if (filename == NULL) {
		hge->System_Log("Failed to load texture %s.\n", filename);
		m_IsLoaded = false;
	}
	else {
		m_Texture = hge->Texture_Load(filename);
		m_IsLoaded = true;
		hge->System_Log("Loaded texture %s at %#x.\n", filename, m_Texture);
	}
}

Texture::~Texture(void)
{
	hge->Texture_Free(m_Texture);
	if (m_RefCount)
		hge->System_Log("Unloaded texture %s while it was still referenced by %d.\n", m_FileName.c_str(), m_RefCount);
}

bool Texture::DelRef()
{
	 if (m_RefCount)
	 { 
		 m_RefCount--;
		 return true;
	 } 
	 else
	 {
		 hge->System_Log("Trying to dereference texture %s when it is not referenced\n", m_FileName.c_str());
		 return false;
	 }
}