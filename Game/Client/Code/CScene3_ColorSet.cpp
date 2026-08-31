#include "pch.h"
#include "CScene3_ColorSet.h"
#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

#include "CDInputMgr.h"


CScene3_ColorSet::CScene3_ColorSet(LPDIRECT3DDEVICE9 pGraphicDev, CHAR_COLOR eID)
	:CGameObject(pGraphicDev), m_eColorNum(eID)
{
}

CScene3_ColorSet::CScene3_ColorSet(const CScene3_ColorSet& rhs)
	:CGameObject(rhs)
{
}

CScene3_ColorSet::~CScene3_ColorSet()
{
}

HRESULT CScene3_ColorSet::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_vScale = { 50,50,1 };
	m_pTransformCom->Set_Scale(m_vScale);
	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Scene3_ColorSet"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });
	Set_Show1(false);
	
	
	
	m_fFrame = m_eColorNum * 2+1;


	return S_OK;
}

void CScene3_ColorSet::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CScene3_ColorSet::Update_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == false)
		return 0;
	

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	int iIndex = static_cast<int>(m_eColorNum);
	float fFrame = static_cast<float>(iIndex * 2 + 1);

	switch (m_eColorNum)
	{
	case COLOR_RED:

		if (CDInputMgr::GetInstance()->Get_DIMouseKeyDown(DIM_LB))
		{

			if (m_fFrame == fFrame)
				m_fFrame -= 1;
			else
				m_fFrame = fFrame;
		}
		break;

	case COLOR_GREEN:
		
			if (CDInputMgr::GetInstance()->Get_DIMouseKeyDown(DIM_LB))
			{
				if (m_fFrame == fFrame)
					m_fFrame -= 1;
				else
					m_fFrame = fFrame;
			}
			break;

		
	}




	return CGameObject::Update_GameObject(fDeltaTime);
}

void CScene3_ColorSet::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == false)
		return ;
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CScene3_ColorSet::Render_GameObject()
{
	if (m_bShow == false)
		return ;
	switch (m_eColorNum)
	{

	case COLOR_RED:
		m_pTransformCom->Set_Pos({ -340, -40,4 });
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture(m_fFrame);
		m_pVIBufferCom->Render_Buffer();

	case COLOR_GREEN:
		m_pTransformCom->Set_Pos({ -280, -40,4 });
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture(m_fFrame);
		m_pVIBufferCom->Render_Buffer();
	}
}




CScene3_ColorSet* CScene3_ColorSet::Create(LPDIRECT3DDEVICE9 pGraphicDev, CHAR_COLOR eID)
{
	CScene3_ColorSet* pObj = new CScene3_ColorSet(pGraphicDev, eID);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CScene3_ColorSet Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CScene3_ColorSet::Free()
{
	CGameObject::Free();
}