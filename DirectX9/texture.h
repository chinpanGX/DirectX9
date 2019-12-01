/*=================================================

	[texture.h]
	Author : �o���đ�

=================================================*/

#pragma once
#include <d3d9.h>

//	�}�N����`
#define TEXTURE_FILENAME_MAX (64)	//	�e�N�X�`���t�@�C�����̒���

//	TextureIndex�񋓌^�i�e�N�X�`���̃��x�����j
enum TextureIndex
{
    TEXTURE_INDEX_FIELD01,
	TEXTURE_INDEX_FIELD02,
	TEXTURE_INDEX_FIELD03,

    TEXTURE_INDEX_MAX
};

//	Texture�N���X
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
