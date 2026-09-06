#include "CAlphaEmpty.h"
#include "CTexture.h"
#include "CVIBuffer.h"
#include "CRenderer.h"

CAlphaEmpty::CAlphaEmpty(LPDIRECT3DDEVICE9 pGraphicDev) :CGameObject(pGraphicDev)
{
}

CAlphaEmpty::CAlphaEmpty(const CAlphaEmpty& rhs) :CGameObject(rhs)
{
}

CAlphaEmpty::~CAlphaEmpty()
{
}

HRESULT CAlphaEmpty::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	return S_OK;
}

int CAlphaEmpty::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	CRenderer::GetInstance()->Add_LeftMirrorRenderGroup(RENDER_ALPHA, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CAlphaEmpty::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	CTexture* pTex = Get_Component<CTexture>();
	if (pTex)
		pTex->Set_Texture(0);
	CVIBuffer* pBuf = Get_Component<CVIBuffer>();
	if (pBuf)
		pBuf->Render_Buffer();
	if (pTex)
		m_pGraphicDev->SetTexture(0, nullptr);
}

CAlphaEmpty* CAlphaEmpty::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAlphaEmpty* pObj = new CAlphaEmpty(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CAlphaEmpty Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CAlphaEmpty::Free()
{
	CGameObject::Free();
}
