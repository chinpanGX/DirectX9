/*=================================================

	[texture.h]
	Author : 出合翔太

=================================================*/

#pragma once
#include "main.h"

//	Textureクラス
class  Texture
{
private:
	LPDIRECT3DTEXTURE9	m_pTexture[5];	//	テクスチャを格納する変数
public:
	void Load(const char *filename,int i);		//	テクスチャのロード		（テクスチャ名、格納する配列番号）
	void Unload(int i);							//	テクスチャのアンロード	（テクスチャ名、アンロードする配列番号）
	LPDIRECT3DTEXTURE9 Set(int i);				//	テスクチャのセット	（セットする配列番号）
};
