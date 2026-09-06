#include "pch.h"
#include "CDriftSpark.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CCart.h"
#include "CCameraMgr.h"
CDriftSpark::CDriftSpark(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CDriftSpark::CDriftSpark(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CDriftSpark::~CDriftSpark()
{
}

HRESULT CDriftSpark::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Scale({ 8,4,0 });
	m_pTransformCom->Set_Pos({ -4,-1.5f,0.f });

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_DriftSpark"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CDriftSpark::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	//if (dynamic_cast<CCart*>(m_pParent)->GetDrift())
	//{
		D3DXQUATERNION q;
		D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(m_vRotation.y), 0, 0.f);
		m_pTransformCom->Set_Quaternion(&q);

		q = m_pParent->Get_Transform()->Get_WorldQuaternion();

	//}
}

_int CDriftSpark::Update_GameObject(const _float& fDeltaTime)
{
	//if (dynamic_cast<CCart*>(m_pParent)->GetDrift())
	//{
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
		CRenderer::GetInstance()->Add_LeftMirrorRenderGroup(RENDER_ALPHA, this);

		//m_fFrame += 45.f * fDeltaTime;
		//if (m_fFrame > 2.f)
		//	m_fFrame = 0;

		if (GetAsyncKeyState('A'))
			m_vRotation.y += 180 * fDeltaTime;
		if (GetAsyncKeyState('D'))
			m_vRotation.y -= 180 * fDeltaTime;
		return CGameObject::Update_GameObject(fDeltaTime);
	//}
}

void CDriftSpark::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (dynamic_cast<CCart*>(m_pParent)->GetDrift())
		CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CDriftSpark::Render_GameObject()
{
	//if (dynamic_cast<CCart*>(m_pParent)->GetDrift())
	//{
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture(0);//m_fFrame;
		m_pBufferCom->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//}
}

CDriftSpark* CDriftSpark::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDriftSpark* pObj = new CDriftSpark(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CDriftSpark Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CDriftSpark::Free()
{
	CGameObject::Free();
}
