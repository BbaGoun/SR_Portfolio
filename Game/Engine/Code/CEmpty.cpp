#include "CEmpty.h"

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
