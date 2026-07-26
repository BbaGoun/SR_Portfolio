#include "pch.h"
#include "CSkyBox.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CCameraMgr.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CSkyBox::CSkyBox(const CGameObject& rhs) : CGameObject(rhs)
{
}

CSkyBox::~CSkyBox()
{
}

HRESULT CSkyBox::Ready_GameObject()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CInnerBox*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_InnerBox"));
	pComponent->SetOwner(this);

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SkyBoxTexture"));
	pComponent->SetOwner(this);

	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = static_cast<CTransform*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Transform"));
	pComponent->SetOwner(this);

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	m_pTransformCom->m_vScale = { 1000, 1000, 1000 };

	return S_OK;
}

_int CSkyBox::Update_GameObject(const _float& fTimeDelta)
{
	return CGameObject::Update_GameObject(fTimeDelta);
}

void CSkyBox::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CSkyBox::Render_GameObject()
{
	D3DXMATRIX* matWorld;


	matWorld = m_pTransformCom->Get_World();

	m_pTextureCom->Set_Texture(0);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);

	for (int i = 0; i < CAM_GLOBAL; ++i)
	{
		switch (i) {
		case 0:
			m_pGraphicDev->SetViewport(&g_LeftView);
			break;
		case 1:
			m_pGraphicDev->SetViewport(&g_RightView);
			break;
		}
		CameraInfo camInfo = CCameraMgr::GetInstance()->GetCameraInfo(i);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &camInfo.matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &camInfo.matProj);
		m_pBufferCom->Render_Buffer();
	}
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyBox* pObj = new CSkyBox(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Obj_Test Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CSkyBox::Free()
{
	CGameObject::Free();
}
