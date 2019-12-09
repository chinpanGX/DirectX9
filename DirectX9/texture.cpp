/*=================================================

	[texture.cpp]
	Author : èoçá„ƒëæ
	
=================================================*/

#include "texture.h"


void Texture::Load(const char *filename,int i)
{
	D3DXCreateTextureFromFile(GetD3DDevice(), filename, &m_pTexture[i]);
}

void Texture::Unload(int i)
{
	SAFE_RELEASE(m_pTexture[i]);
}

LPDIRECT3DTEXTURE9 Texture::Set(int i)
{
	return m_pTexture[i];
}
