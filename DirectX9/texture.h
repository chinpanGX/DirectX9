/*=================================================

	[texture.h]
	Author : 出合翔太

=================================================*/

#pragma once
#include <d3d9.h>

//	マクロ定義
#define TEXTURE_FILENAME_MAX (64)	//	テクスチャファイル名の長さ

//	TextureIndex列挙型（テクスチャのラベル名）
enum TextureIndex
{
    TEXTURE_INDEX_FIELD01,
	TEXTURE_INDEX_FIELD02,
	TEXTURE_INDEX_FIELD03,

    TEXTURE_INDEX_MAX
};

//	Textureクラス
class  Texture
{
private:
	static LPDIRECT3DDEVICE9	m_pDevice;
public:
	static int Load();
	static void Unload();
	static LPDIRECT3DTEXTURE9 GetTexture(TextureIndex index);
	static int GetWidth(TextureIndex index);
	static int GetHeight(TextureIndex index);
};
