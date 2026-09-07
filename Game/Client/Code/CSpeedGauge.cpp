#include "pch.h"
#include "CSpeedGauge.h"

#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
CSpeedGauge::CSpeedGauge(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CSpeedGauge::CSpeedGauge(const CSpeedGauge& rhs)
	:CGameObject(rhs)
{
}

CSpeedGauge::~CSpeedGauge()
{
}

HRESULT CSpeedGauge::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Pos({ 0, -300, 10 });
	m_pTransformCom->Set_Scale({ 127.f,127.f,0.f });

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SpeedGaugeTexture"));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	m_fFrame = 0;

	return S_OK;


}

void CSpeedGauge::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CSpeedGauge::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	m_fFrame += 2 * fDeltaTime;
	if (m_fFrame > 2)
	{
		m_fFrame = 0;
	}

	return CGameObject::Update_GameObject(fDeltaTime);


}

void CSpeedGauge::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CSpeedGauge::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);
	m_pVIBufferCom->Render_Buffer();


}

CSpeedGauge* CSpeedGauge::Create(LPDIRECT3DDEVICE9 pGraphicDev)

{
	CSpeedGauge* pObj = new CSpeedGauge(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CSpeedGauge Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CSpeedGauge::Free()
{
}