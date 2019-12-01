/*=================================================

	[texture.cpp]
	Author : 出合翔太
	
=================================================*/

#include "main.h"
#include "texture.h"

//	構造体
struct TextureFile
{
	char filename[TEXTURE_FILENAME_MAX];
	int width;
	int height;
};

static const TextureFile g_TextureFiles[] =
{
	{ "asset/texture/field000.jpg", 160, 160 },
	{ "asset/texture/field001.jpg", 160, 160 },
	{ "asset/texture/field002.jpg", 160, 160 },
};

//	グローバル変数
static const int TEXTURE_FILE_COUNT = sizeof(g_TextureFiles) / sizeof(g_TextureFiles[0]);
static_assert(TEXTURE_INDEX_MAX == TEXTURE_FILE_COUNT, "TEXTURE_INDEX_MAX != TEXTURE_FILE_COUNT");
LPDIRECT3DTEXTURE9	g_pTextures[TEXTURE_FILE_COUNT];

//	スタティック変数
LPDIRECT3DDEVICE9	Texture::m_pDevice;

int Texture::Load()
{
	m_pDevice = GetD3DDevice();
	if (!m_pDevice) {
		return TEXTURE_FILE_COUNT;
	}

	int failed_count = 0;

	for (int i = 0; i < TEXTURE_FILE_COUNT; i++) 
	{
		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, g_TextureFiles[i].filename, &g_pTextures[i]))) 
		{
			failed_count++;
		}
	}

	return failed_count;
}

void Texture::Unload()
{
	for (int i = 0; i < TEXTURE_FILE_COUNT; i++) 
	{
		SAFE_RELEASE(g_pTextures[i]);
	}
}

LPDIRECT3DTEXTURE9 Texture::GetTexture(TextureIndex index)
{
	if (index < 0 || index >= TEXTURE_INDEX_MAX)
	{
		return NULL;
	}

	return g_pTextures[index];
}

int Texture::GetWidth(TextureIndex index)
{
	if (index < 0 || index >= TEXTURE_INDEX_MAX) 
	{
		return NULL;
	}

	return g_TextureFiles[index].width;
}

int Texture::GetHeight(TextureIndex index)
{
	if (index < 0 || index >= TEXTURE_INDEX_MAX) 
	{
		return NULL;
	}

	return g_TextureFiles[index].height;
}
