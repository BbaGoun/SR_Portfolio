#include "pch.h"
#include "CWheel_FR.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CWheel_FR::CWheel_FR(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CWheel_FR::CWheel_FR(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CWheel_FR::~CWheel_FR()
{
}

HRESULT CWheel_FR::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	Engine::CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Scale({ 5,5,5 });
	pComponent = m_pBufferCom = dynamic_cast<CCartWheelCol*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CartWheelCol"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

void CWheel_FR::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CWheel_FR::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	m_pTransformCom->Rotate(QUATER_YAW, 180 * fDeltaTime);
	return _int();
}

void CWheel_FR::LateUpdate_GameObject(const _float& fDeltaTime)
{
}

void CWheel_FR::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pBufferCom->Render_Buffer();
}

CWheel_FR* CWheel_FR::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWheel_FR* pObj = new CWheel_FR(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CWheel_FR Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CWheel_FR::Free()
{
	CGameObject::Free();
}
