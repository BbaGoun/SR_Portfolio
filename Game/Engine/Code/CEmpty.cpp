#include "CEmpty.h"
#include "CTexture.h"
#include "CVIBuffer.h"

CEmpty::CEmpty(LPDIRECT3DDEVICE9 pGraphicDev):CGameObject(pGraphicDev)
{
}

CEmpty::CEmpty(const CEmpty& rhs):CGameObject(rhs)
{
}

CEmpty::~CEmpty()
{
}

HRESULT CEmpty::Ready_GameObject()
{
    CGameObject::Ready_GameObject();
    return S_OK;
}

void CEmpty::Render_GameObject()
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

CEmpty* CEmpty::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEmpty* pObj = new CEmpty(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CEmpty Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CEmpty::Free()
{
    CGameObject::Free();
}
