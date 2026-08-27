#include "pch.h"
#include "CSpeedLine.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CCart.h"

CSpeedLine::CSpeedLine(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CSpeedLine::CSpeedLine(const CGameObject& rhs) : CGameObject(rhs)
{
}

CSpeedLine::~CSpeedLine()
{
}

HRESULT CSpeedLine::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Pos({ 0,0,1 });

	m_pTransformCom->Set_Scale({ WINCX, WINCY, 1 });
	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SpeedLine"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_bShow		= false;
	m_fFrame	= 0.f;

	return S_OK;
}

_int CSpeedLine::Update_GameObject(const _float& fDeltaTime)
{
	_vec3 vForce = m_pCart->Get_Force();
	float fSpeed = D3DXVec3Length(&vForce) * m_pCart->Get_Speed();
	if (fSpeed < 50.f)
	{		
		m_bShow = true;
		return 0;
	}
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	m_fFrame += fSpeed * 0.7f * fDeltaTime;
	if (m_fFrame > 8)
		m_fFrame = 0.f;

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CSpeedLine::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == false)
		return;
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CSpeedLine::Render_GameObject()
{
	if (m_bShow == false)
		return;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);
	m_pBufferCom->Render_Buffer();
}

CSpeedLine* CSpeedLine::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpeedLine* pObj = new CSpeedLine(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CSpeedLine Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CSpeedLine::Free()
{
	CGameObject::Free();
}
