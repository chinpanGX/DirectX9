#include "Grid.h"
#include "main.h"

void Grid::Init()
{
	//	D3DDeviceを取得
	m_pDevice = GetD3DDevice();
	//	頂点バッファ初期化
	m_pVtxBuffer = NULL;
	//	頂点初期化
	pVtx[0].pos = D3DXVECTOR3(50.0f, -25.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-50.0f, -25.0f, 0.0f);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);

	/*頂点バッファのメモリを確保関数*/
	MakeVertexField();

}

void Grid::Draw()
{
	for (int j = 0; j < 2; j++) 
	{
		for (int i = 0; i < 80; i++) 
		{
			D3DXMatrixIdentity(&m_mat);

			/*座標をセット*/
			Set(i, j);

			m_pDevice->SetTransform(D3DTS_WORLD, &m_mat);

			m_pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

			m_pDevice->SetFVF(FVF_VERTEX3D);

			//m_pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));

			m_pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 2);
		}
	}
}

void Grid::Set(int i, int j) 
{
	D3DXMATRIX mtxRot, mtxTranslate;
	if (j == 0) 
	{
		D3DXMatrixTranslation(&mtxTranslate, i - 40, 0.0f, 0.0f);
		D3DXMatrixRotationY(&mtxRot, D3DX_PI / 2.0f);
		D3DXMatrixMultiply(&m_mat, &mtxRot, &mtxTranslate);
	}
	else 
	{
		D3DXMatrixTranslation(&mtxTranslate, 0.0f, 0.0f, i - 40);
		D3DXMatrixMultiply(&m_mat, &m_mat, &mtxTranslate);
	}
}

HRESULT Grid::MakeVertexField()
{
	if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &m_pVtxBuffer, NULL))) {
		return E_FAIL;
	}

	void *pVtx2;

	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx2, 0);
	memcpy(pVtx2, pVtx, sizeof(VERTEX_3D) * 2);
	m_pVtxBuffer->Unlock();

	return S_OK;
}